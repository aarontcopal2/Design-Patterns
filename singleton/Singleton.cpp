#include <stdio.h>      // printf
#include <Singleton.h>

// Static variables need to be initialized outside the class.

#ifdef LAZY_INITIALIZATION
    // Lazy initialization of Singleton obj.
    Singleton* Singleton::instance = NULL;
    std::mutex Singleton::mutex;
#else
    // Eager initialization of Singleton obj.
    Singleton* Singleton::instance = new Singleton();
#endif


std::unordered_set<intptr_t> Singleton::singleton_pointers = {};

Singleton::Singleton() {}

void Singleton::clear() {
    instance = NULL;
    singleton_pointers.clear();
}

int Singleton::get_unique_references() {
    return singleton_pointers.size();
}

#ifdef LAZY_INITIALIZATION
Singleton* Singleton::getInstance_single_check_locking() {
    // Performance::
    // Iterations: 1e7
    // Time:       0.900062[sec]
    mutex.lock();
    if (instance == NULL) {
        instance = new Singleton();
        singleton_pointers.insert((intptr_t)instance);
    }
    mutex.unlock();
    return instance;
}

Singleton* Singleton::getInstance_double_check_locking() {
    // Performance::
    // Iterations: 1e7
    // Time:       0.022817[sec]
    // Speedup : 39.45x (wrt. getInstance_single_check_locking)
    if (instance == NULL) {
        mutex.lock();
        if (instance == NULL) {
            instance = new Singleton();
            singleton_pointers.insert((intptr_t)instance);
        }
        mutex.unlock();
    }
    return instance;
}

Singleton* Singleton::getInstance_meyers_implementation() {
    // Performance::
    // Iterations: 1e7
    // Time:       0.087013[sec]
    // Speedup : ?x
	static Singleton instance;
    singleton_pointers.insert((intptr_t)&instance);
	return &instance;
}

#else   // ifndef LAZY_INITIALIZATION

Singleton* Singleton::getInstance_eager_initialization() {
    // Performance::
    // Iterations: 1e7
    // Time:       0.017646[sec]
    // Speedup : 1.29x (wrt. getInstance_double_check_locking)
    return instance;
}

#endif

// Meyers singleton