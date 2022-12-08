#pragma once


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	class QuadProvider final
	{
	public:
		std::shared_ptr<Quad> Create( const BoundingRect& bounds, const size_t level );

	private:
		void Destroy( std::list<Quad>::iterator slot );

	private:
		std::list<Quad>	m_slots;
	};
}
}
}
