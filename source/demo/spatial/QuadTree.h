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
	// Public constants.
	public:
		// Maximum points indexed by single quad before split it to quarters.
		static constexpr size_t MAX_POINTS = 4;

		// Maximum level of quad tree depth before the quarters splitting will be stopped.
		static constexpr size_t MAX_LEVELS = 8;

	// Public inner types and friendship declarations.
	public:
		// Allow the shape to use private interface.
		friend class Internal::Shape;

		// Allow the point to use private interface.
		friend class Internal::Point;


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


		// Build the quad tree.
		void BuildTree() const;

		// Add the point to quad tree.
		void AddPoint( Internal::Quad& quad, const Internal::Point& point ) const;

		// Split the leaf quad to quarters. Given quad becomes the root of subtree after the operation.
		void SplitToQuarters( Internal::Quad& quad ) const;

	// Private state.
	private:
		Internal::ShapeProvider	m_shape_provider;			// Provider for shapes.
		BoundingRect			m_bounds{ { 0.0f, 0.0f } };	// The indexing area.

	// Private non-state.
	private:
		mutable Internal::Points				m_points;			// Blob of points to be indexed.
		mutable Internal::QuadProvider			m_quad_provider;	// Provider for quads.
		mutable std::shared_ptr<Internal::Quad>	m_root;				// Root of quad tree.
	};
}
}
