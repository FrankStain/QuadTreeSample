#include <demo/spatial/spatial.h>

#include <iterator>


namespace Demo
{
inline namespace Spatial
{
	QuadTree::SharedShape QuadTree::Acquire( const BoundingRect& bounds )
	{
		const auto [ shape, handle ] = m_shape_provider.Create( *this, bounds );
		if( !m_bounds.ConsistsOf( bounds ) )
		{
			m_bounds.Grow( bounds );
			m_tree.Reset();
		}

		m_tree.Push( *shape );

		return { shape, [this, handle = handle]( Shape* shape ){ ReleaseShape( handle, shape ); } };
	}

	std::vector<const QuadTree::Shape*> QuadTree::Find( const BoundingRect& bounds ) const
	{
		if( m_tree.IsEmpty() )
		{
			m_tree.Build( m_bounds );
		}

		return m_tree.Find( bounds );
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
		m_tree.Pop( *shape );
		m_shape_provider.Destroy( handle );
	}
}
}
