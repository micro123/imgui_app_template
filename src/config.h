#ifndef FW_CONFIG_H
#define FW_CONFIG_H

#include <cstdint>

#define S(n) using s##n = int##n##_t
#define U(n) using u##n = uint##n##_t

S(8);
S(16);
S(32);
S(64);
U(8);
U(16);
U(32);
U(64);

#undef S
#undef U

#endif /* FW_CONFIG_H */
