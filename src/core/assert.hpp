#ifndef ASSERT_HPP
#define ASSERT_HPP

#include "core/log.hpp"

#define FW_ASSERT_MSG(COND, MSG, ...) do { LC_FATAL(!(COND), MSG, ##__VA_ARGS__); } while(0)
#define FW_ASSERT(COND) FW_ASSERT_MSG(COND, #COND)

#endif // ASSERT_HPP
