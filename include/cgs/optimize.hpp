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
#ifndef CGS_OPTIMIZE_HPP
#define CGS_OPTIMIZE_HPP

/**
 * cgs_assume(expr)
 *
 * @brief Optimizer hint, do not evaluate.
 */
#ifdef __clang__
    #define cgs_assume(expr) __builtin_assume(static_cast<bool>(expr))
#elif defined(_MSC_VER)
    #define cgs_assume(expr) __assume(static_cast<bool>(expr))
#else
    #define cgs_assume(expr) static_cast<void>(sizeof(static_cast<bool>(expr)))
#endif

#if defined(__clang__) || defined(__GNUC__)
    #define cgs_expect(expr, val) __builtin_expect(static_cast<long>(expr), val)
    #define cgs_unreachable() __builtin_unreachable()
#else
    #define cgs_expect(expr, val) static_cast<long>(expr)
    #define cgs_unreachable() static_cast<void>(0)
#endif

/**
 * cgs_likely(expr)
 *
 * @brief Evaluate the boolean expression, predict true branch.
 */
#define cgs_likely(expr) static_cast<bool>(cgs_expect(static_cast<bool>(expr), 1))

/**
 * cgs_unlikely(expr)
 *
 * @brief Evaluate the boolean expression, predict false branch.
 */
#define cgs_unlikely(expr) static_cast<bool>(cgs_expect(static_cast<bool>(expr), 0))

#endif // CGS_OPTIMIZE_HPP
