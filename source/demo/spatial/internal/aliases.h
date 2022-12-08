#pragma once


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	using Points	= std::vector<const Point*>;
	using Quads		= std::array<std::shared_ptr<Quad>, Demo::BoundingRect::CORNERS_COUNT>;
	using QuadState	= std::variant<Points, Quads>;
}
}
}
