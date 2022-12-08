#pragma once


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	struct Quad final
	{
		BoundingRect	bounds;
		Vector2f		center;
		size_t			level;
		QuadState		state;
	};
}
}
}
