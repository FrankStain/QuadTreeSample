#include <demo/spatial/spatial.h>


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	std::shared_ptr<Quad> QuadProvider::Create( const BoundingRect& bounds, const size_t level )
	{
		auto& quad = m_slots.emplace_back();

		quad.bounds = bounds;
		quad.center = bounds.GetCenter();
		quad.level	= level;

		return { &quad, [this, slot = std::prev( m_slots.end() )]( Quad* ) { Destroy( slot ); } };
	}

	void QuadProvider::Destroy( std::list<Quad>::iterator slot )
	{
		slot->state.emplace<Points>();
		m_slots.erase( slot );
	}
}
}
}
