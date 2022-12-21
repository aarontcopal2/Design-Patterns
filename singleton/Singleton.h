#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <cstdint>              // intptr_t
#include <mutex>                // mutex
#include <unordered_set>        // unordered_set

#define LAZY_INITIALIZATION

class Singleton {
public:
    static void clear();
    static int get_unique_references();
#ifdef LAZY_INITIALIZATION
    static Singleton* getInstance_single_check_locking();
    static Singleton* getInstance_double_check_locking();
    static Singleton* getInstance_meyers_implementation();
#else
    static Singleton* getInstance_eager_initialization();
#endif

private:
    Singleton();
    static Singleton *instance;
    static std::mutex mutex;
    static std::unordered_set<intptr_t> singleton_pointers;
};

#endif // SINGLETON_H_