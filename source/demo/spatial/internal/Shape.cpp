#include <demo/spatial/spatial.h>

#include <iterator>


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	Shape::Shape( QuadTree& host, const BoundingRect& bounds )
		: m_host{ &host }
		, m_bounds{ bounds }
	{
		for( size_t index = 0; index < std::size( m_points ); ++index )
		{
			m_points[ index ] = { *this, bounds.GetCorner( index ) };
		}
	}

	void Shape::SetBounds( const BoundingRect& bounds )
	{
		m_bounds = bounds;
		for( size_t index = 0; index < std::size( m_points ); ++index )
		{
			m_points[ index ].SetPosition( bounds.GetCorner( index ) );
		}
	}
}
}
}
