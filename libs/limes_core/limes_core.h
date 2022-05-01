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

#include "text/StringUtils.h"
#include "text/TextTable.h"

#include "math/mathHelpers.h"
#include "math/Fraction.h"
#include "math/Random.h"
#include "math/geometry/angle.h"
#include "math/geometry/circle.h"
#include "math/geometry/cube.h"
#include "math/geometry/cylinder.h"
#include "math/geometry/rectangle.h"
#include "math/geometry/sphere.h"
#include "math/geometry/square.h"
#include "math/geometry/triangle.h"
#include "math/geometry/volume.h"

#include "threading/Threading.h"
#include "threading/RealtimeTypeTraits.h"
#include "threading/ThreadSafeObject.h"
#include "threading/RealtimeSafeObject.h"

#include <limes_version.h>

// IWYU pragma: end_exports
