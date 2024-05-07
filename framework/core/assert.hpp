#ifndef ASSERT_HPP
#define ASSERT_HPP

#include "core/log.hpp"

#define PI_ASSERT_MSG(COND, MSG, ...) do { LC_FATAL(!(COND), MSG, ##__VA_ARGS__); } while(0)
#define PI_ASSERT(COND) PI_ASSERT_MSG(COND, #COND)

#endif // ASSERT_HPP
