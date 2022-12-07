#pragma once


// Most fundamental dependencies.
#include <cstdint>
#include <cmath>


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
#include "BoundingRect.inl"
#include "Vector2f.inl"

#include "Vector2f.operations.inl"
