/*
   Copyright 2017 Cory Sherman

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef CGS_ASSERT_HPP
#define CGS_ASSERT_HPP

#include "cgs/macro.hpp" // CGS_LINE
#include "cgs/optimize.hpp" // cgs_assume, cgs_likely

#include <cstdio> // fputs, stderr
#include <cstdlib> // abort
#include <stdexcept> // logic_error

/*
To customize how logic errors are handled, you can `#define` one of:
* `CGS_VIOLATE_ABORT` print to stderr and abort if an assertion fails
* `CGS_VIOLATE_IGNORE` do not evaluate asserted expressions
* `CGS_VIOLATE_THROW` throw if an assertion fails

*/

// default if no option is set:
// IGNORE on NDEBUG.
// ABORT on debug
#if !(defined(CGS_VIOLATE_ABORT) || defined(CGS_VIOLATE_IGNORE) || defined(CGS_VIOLATE_THROW))
    #ifdef NDEBUG
        #define CGS_VIOLATE_IGNORE
    #else
        #define CGS_VIOLATE_ABORT
    #endif
#endif

// validate option
#if defined(CGS_VIOLATE_ABORT) + defined(CGS_VIOLATE_IGNORE) + defined(CGS_VIOLATE_THROW) != 1
    #error There can only be one CGS_VIOLATE_*
#endif

#define cgs_assert_abort(expression) ( cgs_likely(expression) \
    ? static_cast<void>(0) \
    : (std::fputs("Assertion failed (" #expression ") at " __FILE__ ":" CGS_LINE "\n", stderr), \
        std::fflush(stderr), \
        std::abort() \
    ) \
)

#define cgs_assert_ignore(expr) cgs_assume(expr)

#define cgs_assert_throw(expression) ( cgs_likely(expression) \
    ? static_cast<void>(0) \
    : throw std::logic_error("Assertion failed (" #expression ") at " __FILE__ ":" CGS_LINE) \
)

#ifdef CGS_VIOLATE_ABORT
    #define cgs_assert(expr) cgs_assert_abort(expr)
#elif defined(CGS_VIOLATE_IGNORE)
    #define cgs_assert(expr) cgs_assert_ignore(expr)
#elif defined(CGS_VIOLATE_THROW)
    #define cgs_assert(expr) cgs_assert_throw(expr)
#endif

#endif // CGS_ASSERT_HPP
