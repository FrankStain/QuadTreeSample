#pragma once


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	// The state of quad that represents the leaf of quad tree. Only leafs store the points.
	using Points = std::vector<const Point*>;

	// The state of quad that represent the root of subtree.
	using Quads = std::array<std::shared_ptr<Quad>, Demo::BoundingRect::CORNERS_COUNT>;

	// The state of quad.
	using QuadState = std::variant<Points, Quads>;
}
}
}
