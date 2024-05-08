//
// Created by tang on 23-3-26.
//

#ifndef PIINFO_SAFE_VALUE_HPP
#define PIINFO_SAFE_VALUE_HPP

#include <type_traits>
#include <utility>
#include <shared_mutex>
#include <string>

#include "config.h"
#include "data/mathematics.hpp"

#if _WIN32 && defined(CALLBACK)
#undef CALLBACK
#endif

enum class value_type : u8 {
    NONE = 0,
    BOOL,
    U8,
    U16,
    U32,
    U64,
    S8,
    S16,
    S32,
    S64,
    F32,
    F64,
    VEC2, // float[2]
    VEC3, // float[3]
    VEC4, // float[4]
    STD_STRING,
    C_STRING, // char[N]

    // Special Values
    CALLBACK,
    ENUMERATE,
    IMAGE,
    UI,

    // COUNT
    COUNT,
};

template<typename T>
struct value_traits;

#define VALUE_TRAITS(T,D,N)                                    \
template<> struct value_traits<T> {                            \
        enum { store_type = (u8)value_type::D, dim = (N), };   \
    }

VALUE_TRAITS (u8, U8, 1);
VALUE_TRAITS (u16,U16,1);
VALUE_TRAITS (u32,U32,1);
VALUE_TRAITS (u64,U64,1);
VALUE_TRAITS (s8, S8, 1);
VALUE_TRAITS (s16,S16,1);
VALUE_TRAITS (s32,S32,1);
VALUE_TRAITS (s64,S64,1);
VALUE_TRAITS (f32,F32,1);
VALUE_TRAITS (f64, F64, 1);
VALUE_TRAITS (bool,BOOL,1);
VALUE_TRAITS (std::string,STD_STRING,1);

template<size_t N>
struct value_traits<char[N]> {
    enum { store_type = (u8)value_type::C_STRING, dim = (N), };
};

template<>
struct value_traits<vec2> {
    enum { store_type = (u8)value_type::VEC2, dim = 2, };
};
template<>
struct value_traits<vec3> {
    enum { store_type = (u8)value_type::VEC3, dim = 3, };
};
template<>
struct value_traits<vec4> {
    enum { store_type = (u8)value_type::VEC4, dim = 4, };
};

#undef VALUE_TRAITS

template <typename T>
concept copiable = std::is_copy_assignable_v<T> && std::is_copy_constructible_v<T>;

template <copiable T>
class safe_value final {
public:
    using holder_type = T;

    template <typename F>
    explicit safe_value(F &&init_value = {}): v_(std::forward<F>(init_value)), changed_(false) {

    }
    safe_value(): safe_value(T{}) {

    }
    ~safe_value() = default;

    safe_value(const safe_value& other) {
        v_ = other.read();
    }

    safe_value& operator=(const safe_value& other) {
        write(other.read());
        return *this;
    }

    safe_value(safe_value&&)  noexcept = default;
    safe_value& operator=(safe_value&&)  noexcept = default;

    T read() const {
        std::shared_lock l(l_);
        return v_;
    }

    void write(T new_value) {
        std::lock_guard l(l_);
        v_ = new_value;
        changed_ = true;
    }

    bool changed() const {
        return changed_;
    }

    void handled() const {
        std::lock_guard l(l_);
        changed_ = false;
    }

private:
    mutable std::shared_mutex l_;
    T v_;
    volatile mutable bool changed_;
};

#endif  // PIINFO_SAFE_VALUE_HPP
