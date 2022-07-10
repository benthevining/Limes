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

#include <limes_namespace.h>
#include <limes_export.h>
#include <NE10.h>

LIMES_BEGIN_NAMESPACE

namespace vecops::ne10
{

/*
	The NE10 library uses function pointers internally, which must be initialized at runtime.
	Use a static object to make sure ne10_init() gets called at program startup.
 */

struct LIMES_NO_EXPORT Initializer final
{
	Initializer()
	{
		ne10_init();
	}
};

static const Initializer ne10_initializer;

}  // namespace vecops::ne10

LIMES_END_NAMESPACE
