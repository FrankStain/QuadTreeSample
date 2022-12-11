#pragma once


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	/**
		@brief	Shape implementation.

		Shapes are the collections of points. All points of shape always indexed by quad tree.
		Each shape is represented by bounding rect. All four corners of that bounding rect are indexed by quad tree.
		So that, the shape may be found in different quads, but each quad will index only one point of shape.
		Each point can be accessed from shape. The indexing of points is same as indexing of corners in bounding rect.
	*/
	class Shape final
	{
	// Public inner types.
	public:
		// Single point of shape.
		using Point = Point;

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

		// Get the point of shape by given index.
		inline const Point& GetPoint( const size_t index ) const		{ return m_points[ index ]; };

		// Get the abstract tag of shape.
		inline const size_t GetTag() const								{ return m_tag; };


		inline const Point& operator [] ( const size_t index ) const	{ return GetPoint( index ); };

	// Private state.
	private:
		QuadTree&		m_host;				// Quad tree that host shape.
		BoundingRect	m_bounds;			// Bounds of shape.
		Point			m_points[4];		// Hosted points.

		size_t			m_tag		= 0;	// Abstract tag.
	};
}
}
}
