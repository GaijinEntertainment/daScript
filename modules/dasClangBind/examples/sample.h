#pragma once

class B {
private:
    struct Foo {
        int a;
        float b;
    };
public:
    Foo f;
};

/*
struct A {
    int x;
};
*/

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