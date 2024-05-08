//
// Created by tang on 23-4-24.
//

#ifndef PIINFO_HASH_UTILS_HPP
#define PIINFO_HASH_UTILS_HPP

#include <cstdint>

// FNV-1a 32bit hashing algorithm.
constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
{
    return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
}

constexpr std::uint32_t operator"" _hash(char const* s, std::size_t count)
{
    return fnv1a_32(s, count);
}

#endif  // PIINFO_HASH_UTILS_HPP
