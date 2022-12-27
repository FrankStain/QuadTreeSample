#include <demo/spatial/spatial.h>


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
namespace
{
	// Whether the quad is empty.
	const bool IsEmpty( const Quad& quad )
	{
		if( !quad.shapes.empty() )
		{
			return false;
		}

		return quad.is_leaf || std::all_of( quad.quarters.begin(), quad.quarters.end(), []( const std::shared_ptr<Quad>& slot ) { return !slot; } );
	}

	// Get the index of quad quarter, where the given shape may be placed.
	const size_t GetQuarterIndex( const Quad& quad, const Shape& shape )
	{
		return quad.bounds.GetNearestCornerIndex( shape.GetBounds().GetCenter() );
	}

	// Get the bounds of quad quarter by given index.
	Demo::BoundingRect GetQuarterBounds( const Quad& quad, const size_t quarter_index )
	{
		constexpr std::pair<size_t, size_t> min_field_indices[] { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
		constexpr std::pair<size_t, size_t> max_field_indices[] { { 1, 1 }, { 2, 1 }, { 2, 2 }, { 1, 2 } };

		const Vector2f corners[] { quad.bounds.min, quad.center, quad.bounds.max };

		const auto [ min_x, min_y ] = min_field_indices[ quarter_index ];
		const auto [ max_x, max_y ] = max_field_indices[ quarter_index ];
		return { { corners[ min_x ].x, corners[ min_y ].y }, { corners[ max_x ].x, corners[ max_y ].y }, std::ignore };
	}

	// Remove the given shape from indexing.
	void UnindexShape( Quad& quad, const Shape& shape )
	{
		auto found_slot = std::find( quad.shapes.begin(), quad.shapes.end(), &shape );
		if( found_slot == quad.shapes.end() )
		{
			for( auto& quarter : quad.quarters )
			{
				if( quarter && quarter->bounds.ConsistsOf( shape.GetBounds() ) )
				{
					UnindexShape( *quarter, shape );

					if( IsEmpty( *quarter ) )
					{
						quarter.reset();
					}
				}
			}
		}
		else
		{
			quad.shapes.erase( found_slot );
		}
	}
}


	void IndexTree::Reset()
	{
		m_root.reset();
	}

	void IndexTree::Build( const Demo::BoundingRect& bounds )
	{
		m_root = m_quad_provider.Create( bounds, 1 );

		auto shapes_end = m_shapes.end();
		for( auto current = m_shapes.begin(); current != shapes_end; ++current )
		{
			if( *current == nullptr )
			{
				std::swap( *current, *( --shapes_end ) );
			}

			ReindexShape( *m_root, *( *current ) );
		}

		if( shapes_end == m_shapes.end() )
		{
			return;
		}

		m_shapes.erase( shapes_end, m_shapes.end() );
	}

	void IndexTree::Push( const Shape& shape )
	{
		m_shapes.push_back( &shape );
		if( m_root )
		{
			ReindexShape( *m_root, shape );
		}
	}

	void IndexTree::Pop( const Shape& shape )
	{
		*std::find( m_shapes.begin(), m_shapes.end(), &shape ) = nullptr;

		if( m_root )
		{
			UnindexShape( *m_root, shape );
		}
	}


	std::vector<const Shape*> IndexTree::Find( const Demo::BoundingRect& bounds ) const
	{
		std::vector<const Shape*> result;

		std::queue<const Quad*> pending_quads;
		pending_quads.push( m_root.get() );
		while( !pending_quads.empty() )
		{
			const Quad& quad = *pending_quads.front();
			pending_quads.pop();

			for( const auto shape : quad.shapes )
			{
				if( bounds.IsIntersects( shape->GetBounds() ) )
				{
					result.push_back( shape );
				}
			}

			for( const auto& quarter : quad.quarters )
			{
				if( quarter && bounds.IsIntersects( quarter->bounds ) )
				{
					pending_quads.push( quarter.get() );
				}
			}
		}

		return result;
	}

	void IndexTree::ReindexShape( Quad& quad, const Shape& shape )
	{
		if( quad.is_leaf )
		{
			if( ( quad.shapes.size() < MAX_POINTS ) || ( quad.level >= MAX_LEVELS ) )
			{
				quad.shapes.push_back( &shape );
				return;
			}

			SplitToQuarters( quad );
		}

		const size_t quarter_index = GetQuarterIndex( quad, shape );
		auto& quarter = quad.quarters[ quarter_index ];

		const Demo::BoundingRect quarter_bounds{ GetQuarterBounds( quad, quarter_index ) };
		if( quarter_bounds.ConsistsOf( shape.GetBounds() ) )
		{
			if( !quarter )
			{
				quarter = m_quad_provider.Create( quarter_bounds, quad.level + 1 );
			}

			return ReindexShape( *quarter, shape );
		}
		else
		{
			quad.shapes.push_back( &shape );
			return;
		}
	}

	void IndexTree::SplitToQuarters( Quad& quad )
	{
		quad.is_leaf = false;
		for( const auto shape : Shapes{ std::move( quad.shapes ) } )
		{
			ReindexShape( quad, *shape );
		}
	}
}
}
}
