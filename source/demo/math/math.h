#pragma once


// Most fundamental dependencies.
#include <cstdint>
#include <cmath>
#include <tuple>
#include <utility>


// Namespace definition.
namespace Demo
{
// Math subsystem.
inline namespace Math
{
// Internal stuff for math.
namespace Internal
{

}
}
}


// Public definitions.
#include "Vector2f.h"
#include "BoundingRect.h"

// Deferred inline definitions.
#include "Vector2f.operations.inl"

#include "BoundingRect.inl"
#include "Vector2f.inl"
