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

// potentially allow undefined behavior, don't evaluate the expression
#define cgs_assert(expr) (static_cast<void>(0))

#else

#include <sstream> // ostringstream message builder

#ifdef CGS_ASSERT_THROW
#include <stdexcept> // logic_error
#else
#include <cstdlib> // abort
#endif

namespace cgs { namespace detail {

#ifdef CGS_ASSERT_THROW
[[noreturn]] inline void assertThrow(const char* expression, const char* sourceName, unsigned int sourceLine)
{
    std::ostringstream message;
    message << "Assertion failed (" << expression << ") at " << sourceName << ":" << sourceLine;
    throw std::logic_error(message.str());
}
#define cgs_detail_assert_fail(expression) cgs::detail::assertThrow(#expression, __FILE__, __LINE__)
#else // CGS_ASSERT_ABORT
[[noreturn]] inline void assertAbort(const char* expression, const char* sourceName, unsigned int sourceLine)
{
    std::ostringstream message;
    message << "Assertion failed (" << expression << ") at " << sourceName << ":" << sourceLine << "\n";
    std::cerr << message.str() << std::flush;
    std::abort();
}
#define cgs_detail_assert_fail(expression) cgs::detail::assertAbort(#expression, __FILE__, __LINE__)
#endif

}} // namespace cgs::detail

#define cgs_assert(expression) ( \
    static_cast<bool>(expression) \
        ? static_cast<void>(0) \
        : cgs_detail_assert_fail(expression) \
    )

#endif

#endif // CGS_ASSERT_HPP
