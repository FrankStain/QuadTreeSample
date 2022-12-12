#pragma once


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	/**
		@brief	Provider of quad instances.

		This provider is a mock for true quad memory pool.
		Currently it only gives the valid interface to create the quads.
	*/
	class QuadProvider final
	{
	// Public interface.
	public:
		// Create new quad. The instance returned will be destroyed once there no shared pointers reference it.
		std::shared_ptr<Quad> Create( const BoundingRect& bounds, const size_t level );

	// Private interface.
	private:
		// Perform the quad destruction by given iterator.
		void Destroy( std::list<Quad>::iterator slot );

	// Private state.
	private:
		std::list<Quad>	m_slots; // Storage for quads.
	};
}
}
}
