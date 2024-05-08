#ifndef MACROS_H
#define MACROS_H

#include "config.h"

#define FLAG(BIT) ((1UL)<<(BIT))
#define MASK(S,N) ((((1UL)<<(N))-1UL)<<(S))

#endif // MACROS_H
