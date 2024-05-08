//
// Created by tang on 23-3-4.
//

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <type_traits>

template<typename T>
class PublicSingleton
{
protected:
    PublicSingleton() = default;

public:
    static T* instance()
    {
        static T inst;
        return &inst;
    }
    PublicSingleton(const PublicSingleton&) = delete;
    PublicSingleton& operator=(const PublicSingleton&) = delete;
};

#endif // SINGLETON_HPP
