#include <demo/spatial/spatial.h>

#include <iterator>


namespace Demo
{
inline namespace Spatial
{
namespace
{
	const bool IsLeaf( const Internal::Quad& quad )
	{
		return std::holds_alternative<Internal::Points>( quad.state );
	}

	const bool IsSubtree( const Internal::Quad& quad )
	{
		return std::holds_alternative<Internal::Quads>( quad.state );
	}

	Internal::Points& GetPoints( Internal::Quad& quad )
	{
		return std::get<Internal::Points>( quad.state );
	}

	const Internal::Points& GetPoints( const Internal::Quad& quad )
	{
		return std::get<Internal::Points>( quad.state );
	}

	Internal::Quads& GetQuarters( Internal::Quad& quad )
	{
		return std::get<Internal::Quads>( quad.state );
	}

	const Internal::Quads& GetQuarters( const Internal::Quad& quad )
	{
		return std::get<Internal::Quads>( quad.state );
	}

	const bool IsEmpty( const Internal::Quad& quad )
	{
		if( IsSubtree( quad ) )
		{
			const auto& quarters = GetQuarters( quad );
			return std::all_of( quarters.begin(), quarters.end(), []( const std::shared_ptr<Internal::Quad>& slot ) { return !slot; } );
		}

		return GetPoints( quad ).empty();
	}

	void AddOnce( std::vector<const QuadTree::Shape*>& storage, const QuadTree::Shape& value )
	{
		auto found_slot = std::upper_bound( storage.begin(), storage.end(), &value );
		if( ( found_slot == storage.begin() ) || ( *std::prev( found_slot ) != &value ) )
		{
			storage.insert( found_slot, &value );
		}
	}

	const size_t GetQuarterIndex( const Internal::Quad& quad, const Internal::Point& point )
	{
		return quad.bounds.GetNearestCornerIndex( point.GetPosition() );
	}

	Demo::BoundingRect GetQuarterBounds( const Internal::Quad& quad, const size_t quarter_index )
	{
		constexpr std::pair<size_t, size_t> quarter_min[] { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
		constexpr std::pair<size_t, size_t> quarter_max[] { { 1, 1 }, { 2, 1 }, { 2, 2 }, { 1, 2 } };

		const Vector2f corners[] { quad.bounds.min, quad.center, quad.bounds.max };

		const auto [ min_x, min_y ] = quarter_min[ quarter_index ];
		const auto [ max_x, max_y ] = quarter_max[ quarter_index ];
		return { { corners[ min_x ].x, corners[ min_y ].y }, { corners[ max_x ].x, corners[ max_y ].y }, std::ignore };
	}

	void RemovePoint( Internal::Quad& quad, const Internal::Point& point )
	{
		if( IsLeaf( quad ) )
		{
			auto& points = GetPoints( quad );
			auto found_point = std::find( points.begin(), points.end(), &point );
			std::swap( *found_point, points.back() );
			points.pop_back();
			return;
		}

		auto& quarter = GetQuarters( quad )[ GetQuarterIndex( quad, point ) ];
		if( quarter )
		{
			RemovePoint( *quarter, point );
			if( IsEmpty( *quarter ) )
			{
				quarter.reset();
			}
		}
	}
}


	QuadTree::SharedShape QuadTree::Acquire( const BoundingRect& bounds )
	{
		const auto [ shape, handle ] = m_shape_provider.Create( *this, bounds );
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
			[&shape = *shape]( const size_t index ) -> const Internal::Point*
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

	std::vector<const QuadTree::Shape*> QuadTree::Find( const BoundingRect& bounds ) const
	{
		std::vector<const Shape*> result;
		if( !m_root )
		{
			BuildTree();
		}

		std::queue<const Internal::Quad*> pending_quads;
		pending_quads.push( m_root.get() );
		while( !pending_quads.empty() )
		{
			const Internal::Quad& quad = *pending_quads.front();
			pending_quads.pop();

			if( IsLeaf( quad ) )
			{
				for( const auto point : GetPoints( quad ) )
				{
					if( bounds.ConsistsOf( point->GetPosition() ) )
					{
						AddOnce( result, *point->GetHost() );
					}
				}
			}
			else
			{
				for( const auto& quad_slot : GetQuarters( quad ) )
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

	std::vector<const QuadTree::Shape*> QuadTree::Find( const Vector2f& center, const float radius ) const
	{
		std::vector<const Shape*> result{ Find( BoundingRect{ center }.Resize( radius ) ) };

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

	void QuadTree::ReleaseShape( const Internal::ShapeProvider::Handle handle, Shape* shape )
	{
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

	void QuadTree::AddPoint( Internal::Quad& quad, const Internal::Point& point ) const
	{
		if( IsLeaf( quad ) )
		{
			auto& points = GetPoints( quad );
			if( ( points.size() < MAX_POINTS ) || ( quad.level == MAX_LEVELS ) )
			{
				points.push_back( &point );
				return;
			}

			SplitToQuarters( quad );
		}

		const size_t quarter_index = GetQuarterIndex( quad, point );
		auto& quarter = GetQuarters( quad )[ quarter_index ];
		if( !quarter )
		{
			quarter = m_quad_provider.Create( GetQuarterBounds( quad, quarter_index ), quad.level + 1 );
		}

		AddPoint( *quarter, point );
	}

	void QuadTree::SplitToQuarters( Internal::Quad& quad ) const
	{
		const Internal::Points points{ std::move( GetPoints( quad ) ) };

		quad.state.emplace<Internal::Quads>();
		for( const auto& point : points )
		{
			AddPoint( quad, *point );
		}
	}
}
}
