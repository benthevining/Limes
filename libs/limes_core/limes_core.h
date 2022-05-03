/*
 * ======================================================================================
 *  __    ____  __  __  ____  ___
 * (  )  (_  _)(  \/  )( ___)/ __)
 *  )(__  _)(_  )    (  )__) \__ \
 * (____)(____)(_/\/\_)(____)(___/
 *
 *  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

/*

limes_core
--------------------

The Limes core library of utilities.

Dependencies: None

*/

#pragma once

// IWYU pragma: begin_exports

#include <limes_platform.h>
#include <limes_build_type.h>

#include "misc/preprocessor.h"
#include "misc/IgnoreUnused.h"
#include "misc/TypeTraits.h"
#include "misc/Algorithm.h"
#include "misc/Version.h"
#include "misc/Functions.h"
#include "misc/streams.h"

#include "time/build_date.h"
#include "time/year.h"
#include "time/month.h"
#include "time/weekday.h"
#include "time/date.h"

#include "memory/RawData.h"
#include "memory/ReferenceCountedObject.h"
#include "memory/SharedObject.h"
#include "memory/StaticObjects.h"
#include "memory/alignment.h"
#include "memory/AlignedAllocator.h"
#include "memory/MemoryPool.h"
#include "memory/MemoryPoolAllocator.h"
#include "memory/stack_allocator.h"
#include "memory/OptionalObject.h"

#include "files/FilesystemEntry.h"
#include "files/file.h"
#include "files/directory.h"
#include "files/sym_link.h"
#include "files/misc.h"

#include "text/StringUtils.h"
#include "text/TextTable.h"

#include "math/mathHelpers.h"
#include "math/Fraction.h"
#include "math/Random.h"
#include "math/geometry/2D/angle.h"
#include "math/geometry/2D/2Dshape.h"
#include "math/geometry/2D/circle.h"
#include "math/geometry/2D/hexagon.h"
#include "math/geometry/2D/octagon.h"
#include "math/geometry/2D/pentagon.h"
#include "math/geometry/2D/rectangle.h"
#include "math/geometry/2D/square.h"
#include "math/geometry/2D/triangle.h"
#include "math/geometry/3D/3Dshape.h"
#include "math/geometry/3D/cone.h"
#include "math/geometry/3D/cube.h"
#include "math/geometry/3D/cylinder.h"
#include "math/geometry/3D/pyramid.h"
#include "math/geometry/3D/rectangular_prism.h"
#include "math/geometry/3D/sphere.h"
#include "math/geometry/3D/triangular_prism.h"

#include "threading/Threading.h"
#include "threading/RealtimeTypeTraits.h"
#include "threading/ThreadSafeObject.h"
#include "threading/RealtimeSafeObject.h"

#include <limes_version.h>

// IWYU pragma: end_exports
