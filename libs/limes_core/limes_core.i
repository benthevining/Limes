%module limes_core

%{
#define SWIG_FILE_WITH_INIT
#include limes_namespace.h

#include time/weekday.h
%}

%include time/weekday.h

namespace limes::time
{
	%template(weekday) Weekday<true>;
	%template(ISOweekday) Weekday<false>;
}
