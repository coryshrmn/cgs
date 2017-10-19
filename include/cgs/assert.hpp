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

/*
To customize how logic errors are handled, you can `#define` one of:
* `CGS_ASSERT_IGNORE` do not evaluate asserted expressions
* `CGS_ASSERT_THROW` throw if an assertion fails
* `CGS_ASSERT_ABORT` print to stderr and abort if an assertion fails

*/

// default if no option is set:
// IGNORE on NDEBUG.
// ABORT on debug
#if !(defined(CGS_ASSERT_IGNORE) || defined(CGS_ASSERT_THROW) || defined(CGS_ASSERT_ABORT))
    #ifdef NDEBUG
        #define CGS_ASSERT_IGNORE
    #else
        #define CGS_ASSERT_ABORT
    #endif
#endif

// validate option
#if defined(CGS_ASSERT_IGNORE) + defined(CGS_ASSERT_THROW) + defined(CGS_ASSERT_ABORT) != 1
    #error There can only be one CGS_ASSERT_*
#endif

#ifdef CGS_ASSERT_IGNORE

    // undefined behavior unless expr, don't evaluate the expression

    #define cgs_assert(expr) cgs_assume(expr)

#else // not ignoring; either throw or abort

    #ifdef CGS_ASSERT_THROW
        #include <stdexcept> // logic_error
    #else
        #include <cstdlib> // abort
        #include <cstdio> // fputs, stderr
    #endif

    #ifdef CGS_ASSERT_THROW
        #define cgs_detail_assert_fail(expression) throw std::logic_error("Assertion failed (" #expression ") at " __FILE__ ":" CGS_LINE)
    #elif defined(CGS_ASSERT_ABORT)
        #define cgs_detail_assert_fail(expression) ( \
            std::fputs("Assertion failed (" #expression ") at " __FILE__ ":" CGS_LINE "\n", stderr), \
            std::fflush(stderr), \
            std::abort() \
        )
    #endif

    #define cgs_assert(expression) ( \
        cgs_likely(expression) \
            ? static_cast<void>(0) \
            : cgs_detail_assert_fail(expression) \
        )

#endif

#endif // CGS_ASSERT_HPP
