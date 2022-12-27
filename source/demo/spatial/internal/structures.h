#pragma once


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	/**
		@brief	Quad tree quadrant.

		Quads are the nodes in quad tree. Single quadrant can be in one of two states:

		1. The list quad, that can index the points. Such quad can't carry subtree of quads.
		2. The root of subtree, that carry it's own subtree of quads. Such quad can't index the points.

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
