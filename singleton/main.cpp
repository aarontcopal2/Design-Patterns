// Compile:  nmake -f .\Makefile.win
// Clean:    nmake -f .\Makefile.win clean

#include <chrono>           // chrono::steady_clock::now
#include <iostream>         // cout      

#include <Singleton.h>

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<float> fsec;

/*
* Singleton DP: belongs to creational type pattern
* This pattern is used when we need to ensure that only one object of a particular
* class needs to be created. All further references to the objects are referred to
* the same underlying instance created.
* 
* Implementation Guidelines:
* Provide global access to the single instance by:
* 1. Declaring all constructors of that class to be private.
* 2. Providing static method that returns a reference to the instance.
* 3. The instance is stored as a private static variable.
* 
* Questions:
* 1. What are the types of DP?
* A. Creational, ...
* 
* 2. Why use static variables?
* A. Static variables are not tied to a class's object. These objects are not created in the heap.
* 
* 3. Will race condition occur in placement new?
* A. It can. The problem is not because of allocation-initialization strategy of creating C++ objects,
*    It is because a thread can get swapped out of execution immediately after entering the if-loop,
*    hence causing two threads to create singleton objects.
*/

int n = 1e7;

void print_helper(const std::string &fn_name, const fsec &fs) {
    printf("%s\n", fn_name.c_str());
    printf("Singleton object requested %d times. Unique reference count: %d\n",
                n, Singleton::get_unique_references());
    printf("Elapsed time = %f [s]\n\n", fs.count());
}

int main() {
    Singleton *s;

    Singleton::clear();

#ifdef LAZY_INITIALIZATION
    auto begin = Time::now();
    #pragma omp parallel for
    for (int j = 0; j < n; j++) {
        s = Singleton::getInstance_single_check_locking();
    }
    auto end = Time::now();
    fsec fs = end - begin;
    print_helper("Single Check Locking", fs);

    Singleton::clear();

    begin = Time::now();
    #pragma omp parallel for
    for (int j = 0; j < n; j++) {
        s = Singleton::getInstance_double_check_locking();
    }
    end = Time::now();
    fs = end - begin;
    print_helper("Double Check Locking", fs);

    Singleton::clear();

    begin = Time::now();
    #pragma omp parallel for
    for (int j = 0; j < n; j++) {
        s = Singleton::getInstance_meyers_implementation();
    }
    end = Time::now();
    fs = end - begin;
    print_helper("Meyer's Implementation", fs);

#else
    Singleton::clear();

    auto begin = Time::now();
    #pragma omp parallel for
    for (int j = 0; j < n; j++) {
        s = Singleton::getInstance_eager_initialization();
    }
    auto end = Time::now();
    fsec fs = end - begin;
    print_helper("Eager Initialization", fs);
#endif
}
