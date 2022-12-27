#pragma once


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	/**
		@brief	Indexing quad-tree.

		This type directly implements the `Quadtree` functionality. It only manage the quads and builds the tree for spatial searching.
		The managing of tree state should be made externally. This tree does not rebuild or reset itself.
		It always relies on external correctness of spatial bounds.
	*/
	class IndexTree final
	{
	// Public constants.
	public:
		// Maximum points indexed by single quad before split it to quarters.
		static constexpr size_t MAX_POINTS = 4;

		// Maximum level of quad tree depth before the quarters splitting will be stopped.
		static constexpr size_t MAX_LEVELS = 8;

	public:
		// Reset the indexing tree. Building of tree is required after reset and before the searching.
		void Reset();

		// Build the indexing tree within the given bounds.
		void Build( const Demo::BoundingRect& bounds );


		// Push the shape to indexing tree.
		void Push( const Shape& shape );

		// Pop the shape from indexing tree.
		void Pop( const Shape& shape );

		// Search for indexed shapes in a given bounds.
		std::vector<const Shape*> Find( const Demo::BoundingRect& bounds ) const;

		// Whether the tree is empty (not built).
		inline const bool IsEmpty() const			{ return m_root == nullptr; };

		// Whether the tree is built.
		inline const bool IsBuilt() const			{ return m_root != nullptr; };

	private:
		// Perform the shape re-indexation.
		void ReindexShape( Quad& quad, const Shape& shape );


		// Split the quad with indexed shapes to quarters.
		void SplitToQuarters( Quad& quad );

	private:
		Shapes					m_shapes;			// Collection of shapes to be indexed.

		QuadProvider			m_quad_provider;	// Provider of quads.
		std::shared_ptr<Quad>	m_root;				// The root of tree.
	};
}
}
}
