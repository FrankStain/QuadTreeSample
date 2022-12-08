#include <main.h>
#include <demo/math/math.h>
#include <demo/spatial/spatial.h>


class QuadTree final
{
public:
	friend class Shape;

public:
	std::shared_ptr<Shape> RetainShape( const BoundingRect& bounds );

	std::vector<const Shape*> Find( const BoundingRect& bounds ) const;
	std::vector<const Shape*> Find( const Vector2f& center, const float radius ) const;

	const BoundingRect& GetBounds() const { return m_bounds; };

private:
	static void AddOnce( std::vector<const Shape*>& destination, const Shape& shape );
	void SplitToChildren( Quad& quad ) const;
	static const size_t GetQuarterIndex( const Quad& quad, const Point& point );
	static BoundingRect GetQuarterBounds( const Quad& quad, const size_t quarter_index );

	static const bool IsLeaf( const Quad& quad );

private:
	void ReleaseShape( const ShapeProvider::Handle handle, Shape* shape );

	void BuildTree() const;
	void AddPoint( Quad& quad, const Point& point ) const;
	void RemovePoint( Quad& quad, const Point& point ) const;

private:
	static constexpr size_t MAX_POINTS = 4;
	static constexpr size_t MAX_LEVELS = 8;

private:
	ShapeProvider	m_shape_provider;
	BoundingRect	m_bounds{ { 0.0f, 0.0f }, { 0.0f, 0.0f }, std::ignore };

private:
	mutable Points					m_points;
	mutable QuadProvider			m_quad_provider;
	mutable std::shared_ptr<Quad>	m_root;
};

std::shared_ptr<Shape> QuadTree::RetainShape( const BoundingRect& bounds )
{
	auto [ shape, handle ] = m_shape_provider.Create( *this, bounds );
	if( !m_bounds.ConsistsOf( bounds ) )
	{
		m_bounds.Grow( bounds );
		m_root.reset();
	}

	const size_t indices[] { 0, 1, 2, 3 };
	std::transform(
		std::begin( indices ),
		std::end( indices ),
		std::back_inserter( m_points ),
		[&shape = *shape]( const size_t index ) -> const Point*
		{
			return &shape[ index ];
		}
	);

	if( m_root )
	{
		for( const size_t index : indices )
		{
			AddPoint( *m_root, shape->GetPoint( index ) );
		}
	}

	return { shape, [this, handle = handle]( Shape* shape ){ ReleaseShape( handle, shape ); } };
}

std::vector<const Shape*> QuadTree::Find( const BoundingRect& bounds ) const
{
	std::vector<const Shape*> result;
	if( !m_root )
	{
		BuildTree();
	}

	std::queue<const Quad*> pending_quads;
	pending_quads.push( m_root.get() );
	while( !pending_quads.empty() )
	{
		const Quad& quad = *pending_quads.front();
		pending_quads.pop();

		if( IsLeaf( quad ) )
		{
			for( const auto point : std::get<Points>( quad.state ) )
			{
				if( bounds.ConsistsOf( point->GetPosition() ) )
				{
					AddOnce( result, *point->GetHost() );
				}
			}
		}
		else
		{
			for( const auto& quad_slot : std::get<Quads>( quad.state ) )
			{
				if( quad_slot && bounds.IsIntersects( quad_slot->bounds ) )
				{
					pending_quads.push( quad_slot.get() );
				}
			}
		}
	}

	return result;
}

std::vector<const Shape*> QuadTree::Find( const Vector2f& center, const float radius ) const
{
	std::vector<const Shape*> result;

	BoundingRect bounds{ BoundingRect{ center }.Resize( radius ) };
	result = Find( bounds );

	auto new_result_end = std::remove_if(
		result.begin(),
		result.end(),
		[&center, radius]( const Shape* shape ) -> const bool
		{
			return shape->GetBounds().IsIntersects( center, radius );
		}
	);

	if( new_result_end != result.end() )
	{
		result.erase( new_result_end, result.end() );
	}

	return result;
}

void QuadTree::AddOnce( std::vector<const Shape*>& destination, const Shape& shape )
{
	auto found_slot = std::upper_bound( destination.begin(), destination.end(), &shape );
	if( ( found_slot == destination.begin() ) || ( *std::prev( found_slot ) != &shape ) )
	{
		destination.insert( found_slot, &shape );
	}
}

void QuadTree::SplitToChildren( Quad& quad ) const
{
	const Points points{ std::move( std::get<Points>( quad.state ) ) };

	quad.state.emplace<Quads>();
	for( const auto& point : points )
	{
		AddPoint( quad, *point );
	}
}

const size_t QuadTree::GetQuarterIndex( const Quad& quad, const Point& point )
{
	return quad.bounds.GetNearestCornerIndex( point.GetPosition() );
}

BoundingRect QuadTree::GetQuarterBounds( const Quad& quad, const size_t quarter_index )
{
	constexpr std::pair<size_t, size_t> quarter_min[] { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
	constexpr std::pair<size_t, size_t> quarter_max[] { { 1, 1 }, { 2, 1 }, { 2, 2 }, { 1, 2 } };

	const Vector2f corners[] { quad.bounds.min, quad.center, quad.bounds.max };

	const auto [ min_x, min_y ] = quarter_min[ quarter_index ];
	const auto [ max_x, max_y ] = quarter_max[ quarter_index ];
	return { { corners[ min_x ].x, corners[ min_y ].y }, { corners[ max_x ].x, corners[ max_y ].y }, std::ignore };
}

const bool QuadTree::IsLeaf( const Quad& quad )
{
	return std::holds_alternative<Points>( quad.state );
}

void QuadTree::ReleaseShape( const ShapeProvider::Handle handle, Shape* shape )
{
	if( !shape || ( shape != m_shape_provider.Query( handle ) ) )
	{
		return;
	}

	for( auto& point : m_points )
	{
		if( point && ( point->GetHost() == shape ) )
		{
			point = nullptr;
		}
	}

	if( m_root )
	{
		constexpr size_t indices[] { 0, 1, 2, 3 };
		for( const size_t index : indices )
		{
			RemovePoint( *m_root, shape->GetPoint( index ) );
		}
	}

	m_shape_provider.Destroy( handle );
}

void QuadTree::BuildTree() const
{
	m_root = m_quad_provider.Create( m_bounds, 1 );

	auto valid_end = m_points.end();
	for( auto current = m_points.begin(); current != valid_end; ++current )
	{
		if( !(*current) )
		{
			std::swap( *current, *( --valid_end ) );
		}

		AddPoint( *m_root, *(*current) );
	}

	if( valid_end == m_points.end() )
	{
		return;
	}

	m_points.erase( valid_end, m_points.end() );
}

void QuadTree::AddPoint( Quad& quad, const Point& point ) const
{
	if( IsLeaf( quad ) )
	{
		auto& points = std::get<Points>( quad.state );
		if( ( points.size() < MAX_POINTS ) || ( quad.level == MAX_LEVELS ) )
		{
			points.push_back( &point );
			return;
		}

		SplitToChildren( quad );
	}

	const size_t quarter_index = GetQuarterIndex( quad, point );
	auto& quad_quarters = std::get<Quads>( quad.state );
	auto& quarter = quad_quarters[ quarter_index ];
	if( !quarter )
	{
		quarter = m_quad_provider.Create( GetQuarterBounds( quad, quarter_index ), quad.level + 1 );
	}

	AddPoint( *quarter, point );
}

void QuadTree::RemovePoint( Quad& quad, const Point& point ) const
{
	if( IsLeaf( quad ) )
	{
		auto& points = std::get<Points>( quad.state );
		points.erase( std::find( points.begin(), points.end(), &point ) );
		return;
	}

	const size_t quarter_index = GetQuarterIndex( quad, point );
	auto& quad_quarters = std::get<Quads>( quad.state );
	auto& quarter = quad_quarters[ quarter_index ];
	if( quarter )
	{
		RemovePoint( *quarter, point );
	}

	if( IsLeaf( *quarter ) )
	{
		if( std::get<Points>( quarter->state ).empty() )
		{
			quarter.reset();
		}
	}
	else
	{
		const auto& quarters = std::get<Quads>( quarter->state );
		if( std::all_of( quarters.begin(), quarters.end(), []( const std::shared_ptr<Quad>& slot ) { return !slot; } ) )
		{
			quarter.reset();
		}
	}
}


int main()
{
	const size_t shapes_count = 105;

	QuadTree tree;

	std::vector<std::shared_ptr<Shape>> shapes;
	shapes.reserve( shapes_count );

	{
		std::minstd_rand						randomizer{ std::random_device{}() };
		std::uniform_real_distribution<double>	random_distribution{ 0.0, 1.0 };

		for( size_t index = 0; index < shapes_count; ++index )
		{
			const Vector2f center{
				float( 10.0 * ( random_distribution( randomizer ) - 0.5 ) ),
				float( 10.0 * ( random_distribution( randomizer ) - 0.5 ) )
			};
			const Vector2f size{
				float( 6.0 * random_distribution( randomizer ) ),
				float( 6.0 * random_distribution( randomizer ) )
			};

			shapes.emplace_back( tree.RetainShape( BoundingRect{ center }.Resize( size.Maximized( { 1.0f, 1.0f } ) * 0.5f ) ) );
			shapes.back()->SetTag( index );
		}
	}

	{
		auto matches = tree.Find( { { 2.5f, 2.5f }, { 3.95f, 4.8f }, std::ignore } );
	}

	auto new_end = std::remove_if( shapes.begin(), shapes.end(), []( const std::shared_ptr<Shape>& shape ) { return ( shape->GetTag() & 1 ) != 0; } );
	shapes.erase( new_end, shapes.end() );

	{
		auto matches = tree.Find( tree.GetBounds() );
		for( const auto& shape : matches )
		{
			const bool is_valid = ( shape->GetTag() & 1 ) == 0;
		}
	}

	return 0;
}
