#pragma once

#include <stdio.h>
#include <inttypes.h>

typedef int ( * fnptr ) ( int, int );

void take_arg_ptr ( int a, fnptr b );

fnptr return_ptr ( void );

inline int global_function ( float a, const char * b ) { printf("hello, world\n"); }

struct Foo {
    int a;
};

Foo & ret_ref_foo();

namespace goo  {
    Foo ret_foo();
}

#if 0

typedef struct {
    int a;
    float b;
} Goo;

struct StFoo {
    int     a;
    float   b[3];
};

struct StOutFoo {
    int a;
    struct StInFoo {
        int b;
    };
};

enum Foo {
    bar1,
    bar2,
    bar3 = bar1 + bar2,
    bar4 = 444
};

enum class FooC {
    bar1,
    bar2,
    bar3
};

enum class FooCC : uint8_t {
    bar1u,
    bar2u,
    bar3u
};

namespace outside {
    void outside_function ( void );
    namespace inside {
        void inside_function ( void );
    }
}

#endif