#pragma once


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	// Collection of indexed shapes.
	using Shapes = std::vector<const Shape*>;

	// Collection of quad quarters.
	using Quarters = std::array<std::shared_ptr<Quad>, Demo::BoundingRect::CORNERS_COUNT>;
}
}
}
