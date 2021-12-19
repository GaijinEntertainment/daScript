#pragma once

#include <stdio.h>
#include <inttypes.h>

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

inline int global_function ( float a, const char * b ) { printf("hello, world\n"); }

namespace outside {
    void outside_function ( void );
    namespace inside {
        void inside_function ( void );
    }
}
