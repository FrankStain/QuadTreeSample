#pragma once


namespace Demo
{
inline namespace Spatial
{
	/**
		@brief	Implementation of quad tree for 2D space.

		This quad tree allows to acquire the shape. Each acquired shape will be indexed on spatial search.
		The lifetime of shapes controlled by consumer, but should be less than the lifetime of quad tree.
		This conception implements the transparent lifetime management of spatial index.

		This quad tree automatically manage the bounds of indexing, the spatial index consistency, it tracks the position of acquired shapes.
		Each shape is represented by bounding rect (AABR) for optimal storing and fast indexation.

		This implementation carries no thread safety. So it should be guarded externally to allow the thread-safe usage.
	*/
	class QuadTree final
	{
	// Public inner types and friendship declarations.
	public:
		// Allow the shape to use private interface.
		friend class Internal::Shape;


		// Shape to be indexed.
		using Shape = Internal::Shape;

		// Shared pointer to shape.
		using SharedShape = std::shared_ptr<Shape>;

	// Public interface.
	public:
		// Acquire the shape. Initial bounds should be provided.
		SharedShape Acquire( const BoundingRect& bounds );


		// Perform the spatial searching of shapes in given bounds.
		std::vector<const Shape*> Find( const BoundingRect& bounds ) const;

		// Perform the spatial searching of shapes in given area.
		std::vector<const Shape*> Find( const Vector2f& center, const float radius ) const;


		// Get the bounds of indexing.
		inline const BoundingRect& GetBounds() const	{ return m_bounds; };

	// Private interface.
	private:
		// Perform the shape releasing.
		void ReleaseShape( const Internal::ShapeProvider::Handle handle, Shape* shape );

	// Private state.
	private:
		Internal::ShapeProvider	m_shape_provider;			// Provider for shapes.
		BoundingRect			m_bounds{ { 0.0f, 0.0f } };	// The indexing area.

	// Private non-state.
	private:
		mutable Internal::IndexTree	m_tree; // The quad tree.
	};
}
}
