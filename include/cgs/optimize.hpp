#ifndef CGS_OPTIMIZE_HPP
#define CGS_OPTIMIZE_HPP

/**
 * cgs_assume(expr)
 *
 * @brief Optimizer hint, do not evaluate.
 * /
 */
#ifdef __clang__
    #define cgs_assume(expr) __builtin_assume(expr)
#elif defined(_MSC_VER)
    #define cgs_assume(expr) __assume(expr)
#else
    #define cgs_assume(expr) static_cast<void>(0)
#endif

#if defined(__clang__) || defined(__GNUC__)
    #define cgs_likely(expr) static_cast<bool>(__builtin_expect(static_cast<bool>(expr), 1))
    #define cgs_unlikely(expr) static_cast<bool>(__builtin_expect(static_cast<bool>(expr), 0))
    #define cgs_unreachable() __builtin_unreachable()
#else
    #define cgs_likely(expr) static_cast<bool>(expr)
    #define cgs_unlikely(expr) static_cast<bool>(expr)
    #define cgs_unreachable() static_cast<void>(0)
#endif


#endif // CGS_OPTIMIZE_HPP
