#pragma once

#include <stdio.h>
#include <inttypes.h>


namespace sf {
    class Foo {
    };

    class Hoo {
        void printHoo();
    };


    class Goo {
        void printHoo();
    };

    class Bar : public Foo, Hoo, public Goo {
        void printBar();
    };

};

#if 0

inline int global_function ( float a, const char * b ) { printf("hello, world\n"); }

Foo & ret_ref_foo();

namespace goo  {
    Foo ret_foo();
}

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