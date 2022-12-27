#pragma once


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	/**
		@brief	Quad tree quadrant.

		Quads are the nodes in quad tree. Each quad can simultaneously store the shapes and quarter quads.
		After the creation the quad represents the leaf. Leafs store only shapes. But any leaf may be converted to subtree.
		Once the quad becomes subtree, it still store the shapes. But subtree stores only shapes that intersects with more than one quarter of quad.
		So that, the shapes are indexed by the most local quad, where the shape is completely inside.

		Each quad is placed at some level of quad tree and describes it's own bounding rect.
		If quad represents leaf, it's bound consists of each indexed point.
		If quad represents subtree, stored quads represent the quarters of quad bounds.
	*/
	struct Quad final
	{
		Shapes			shapes;				// Collection of shapes uniquely indexed by quad.
		Quarters		quarters;			// Quarters of quad.

		size_t			level;				// Level of quadrant in quad tree.

		BoundingRect	bounds;				// Bounding rect of quadrant.
		Vector2f		center;				// Center of quadrant bounds.

		bool			is_leaf = false;	// Whether the quad stores no subtree of quarters.
	};
}
}
}
