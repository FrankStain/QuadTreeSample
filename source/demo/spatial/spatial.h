#pragma once


// Most fundamental dependencies.
#include <demo/math/math.h>

#include <array>
#include <vector>
#include <list>
#include <queue>
#include <memory>
#include <optional>
#include <variant>
#include <utility>


// Namespace definition.
namespace Demo
{
// Spatial subsystem.
inline namespace Spatial
{
// Internal stuff for spatial interface.
namespace Internal
{

}
}
}


// Most fundamental declarations.
#include "forwards.h"
#include "internal/forwards.h"

// Internal definitions.
#include "internal/aliases.h"
#include "internal/structures.h"

#include "internal/Point.h"
#include "internal/Shape.h"

#include "internal/ShapeProvider.h"
#include "internal/QuadProvider.h"

// Public definitions.

// Deferred inline definitions.
