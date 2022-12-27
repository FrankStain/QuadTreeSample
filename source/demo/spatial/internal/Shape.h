#pragma once


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	/**
		@brief	Shape implementation.

		Shapes are interface to communicate with spatial index. Shape is described by bounding rect. Shapes are indexed in quadtree.
		Once the shape moves the bounds using `SetBounds`, it always re-indexed until it goes out indexing bounds.
	*/
	class Shape final
	{
	// Lifetime management.
	public:
		Shape() = delete;
		inline ~Shape() noexcept = default;

		Shape( QuadTree& host, const BoundingRect& bounds ) noexcept;

	// Public interface.
	public:
		// Set new bounds of shape.
		void SetBounds( const BoundingRect& bounds );


		// Set the abstract tag for shape.
		inline void SetTag( const size_t value )						{ m_tag = value; };


		// Get current bounds of shape.
		inline const BoundingRect& GetBounds() const					{ return m_bounds; };

		// Get the abstract tag of shape.
		inline const size_t GetTag() const								{ return m_tag; };

	// Private state.
	private:
		QuadTree&		m_host;				// Quad tree that host shape.
		BoundingRect	m_bounds;			// Bounds of shape.

		size_t			m_tag		= 0;	// Abstract tag.
	};
}
}
}
