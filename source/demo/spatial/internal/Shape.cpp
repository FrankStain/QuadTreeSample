#include <demo/spatial/spatial.h>

#include <iterator>


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	Shape::Shape( QuadTree& host, const BoundingRect& bounds ) noexcept
		: m_host{ host }
		, m_bounds{ bounds }
	{
	}

	void Shape::SetBounds( const BoundingRect& bounds )
	{
		m_bounds = bounds;
	}
}
}
}
