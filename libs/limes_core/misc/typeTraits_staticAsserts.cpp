#include "TypeTraits.h"
#include <limes_namespace.h>
#include <vector>
#include <list>

LIMES_BEGIN_NAMESPACE

static_assert (is_specialization<std::vector<int>, std::vector>(), "is_specialization test");
static_assert (! is_specialization<std::vector<int>, std::list>(), "is_specialization test");

static_assert (is_one_of<int, double, float, int>::value, "is_one_of test");
static_assert (is_none_of<int, long, float, double>::value, "is_none_of test");

// covariance_check

LIMES_END_NAMESPACE
