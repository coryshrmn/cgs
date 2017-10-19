#ifndef CGS_MACRO_HPP
#define CGS_MACRO_HPP

#define cgs_detail_expand(x) #x
#define cgs_expand(x) cgs_detail_expand(x)

// __LINE__ as a string
#define CGS_LINE cgs_expand(__LINE__)

#endif // CGS_MACRO_HPP
