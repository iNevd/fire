//
// Created by nevd on 3/19/19.
//
#ifndef CPP_SINGLETON_H
#define CPP_SINGLETON_H

#include <utility>
template <class T, class ... Args>
class Singleton {
public:
    using SingletonDerived = Singleton<T, Args...>;     // please use: friend Derived; not friend class Derived; (https://en.cppreference.com/w/cpp/language/friend)
    static T* getInstance(Args&&... args) {
        static T t(std::forward<Args...>(args)...);    // multi-thread safe in c++11 (https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables)
        return &t;
    }

protected:
    Singleton& operator = (const Singleton&) = delete;
    Singleton(const Singleton&) = delete;
    Singleton() = default;
};

/*
 * Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 * build ok
 */
/*
 * gcc version 4.8.5 20150623 (Red Hat 4.8.5-36) (GCC)
 * error: use of deleted function ‘Class(Class&&)’
 */
/*
 * gcc (GCC) 8.3.0
 * error: use of deleted function ‘Class(Class&&)’
 */


#endif //CPP_SINGLETON_H