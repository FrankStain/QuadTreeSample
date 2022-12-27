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

		quad.bounds		= bounds;
		quad.center		= bounds.GetCenter();
		quad.level		= level;
		quad.is_leaf	= true;

		return { &quad, [this, slot = std::prev( m_slots.end() )]( Quad* ) { Destroy( slot ); } };
	}

	void QuadProvider::Destroy( std::list<Quad>::iterator slot )
	{
		for( auto& quarter : slot->quarters )
		{
			quarter.reset();
		}

		m_slots.erase( slot );
	}
}
}
}
