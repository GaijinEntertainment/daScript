#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

namespace das {
    namespace aot {
        struct Bar {
            int32_t x; /* = 1 */
            int32_t y; /* = 2 */
            int32_t z; /* = 3 */
            int32_t w; /* = 4 */
        };

        struct Foo {
            int32_t i; /* = 5 */
            float f; /* = 6 */
            char * s; /* = "hello" */
        };

        struct Foo Foo ( Context * __context__ = nullptr );
        bool test ( Context * __context__ );

        struct Foo jestFoo = (([]() -> struct Foo {
            struct Foo __mks_29; das_zero(__mks_29);
            __mks_29.i = 3;
            __mks_29.f = 4;
            __mks_29.s = "blah";
            return __mks_29;
        })());
        struct Foo testFoo = Foo();

        struct Foo Foo ( Context * __context__ )
        {
            return (([&]() -> struct Foo {
                struct Foo __mks_20; das_zero(__mks_20);
                __mks_20.i = 5;
                __mks_20.f = 6;
                __mks_20.s = "hello";
                return __mks_20;
            })());
        }

        bool test ( Context * __context__ )
        {
            DAS_ASSERT(((testFoo.i == 5) & (testFoo.f == 6)) & (SimPolicy<char *>::Equ(cast<char *>::from(testFoo.s),cast<char *>::from("hello"),*__context__)));
            DAS_ASSERT(((jestFoo.i == 3) & (jestFoo.f == 4)) & (SimPolicy<char *>::Equ(cast<char *>::from(jestFoo.s),cast<char *>::from("blah"),*__context__)));
            struct Foo t = Foo(__context__);
            DAS_ASSERT(t.i == 5);
            DAS_ASSERT(t.f == 6);
            DAS_ASSERT(SimPolicy<char *>::Equ(cast<char *>::from(t.s),cast<char *>::from("hello"),*__context__));
            struct Foo q = (([&]() -> struct Foo {
                struct Foo __mks_40; das_zero(__mks_40);
                __mks_40.i = 6;
                __mks_40.s = "world";
                return __mks_40;
            })());
            DAS_ASSERT(q.i == 6);
            DAS_ASSERT(q.f == 0);
            DAS_ASSERT(SimPolicy<char *>::Equ(cast<char *>::from(q.s),cast<char *>::from("world"),*__context__));
            TDim<struct Foo,3> qq3 = (([&]() -> TDim<struct Foo,3> {
                TDim<struct Foo,3> __mks_44; das_zero(__mks_44);
                __mks_44(0,__context__).i = 1;
                __mks_44(1,__context__).i = 2;
                __mks_44(2,__context__).i = 3;
                return __mks_44;
            })());
            DAS_ASSERT(((qq3(0,__context__).i == 1) & (qq3(1,__context__).i == 2)) & (qq3(2,__context__).i == 3));
            TDim<int32_t,4> ar = (([&]() -> TDim<int32_t,4> {
                TDim<int32_t,4> __mka_46; das_zero(__mka_46);
                __mka_46(0,__context__) = 1;
                __mka_46(1,__context__) = 2;
                __mka_46(2,__context__) = 3;
                __mka_46(3,__context__) = 4;
                return __mka_46;
            })());
            DAS_ASSERT((((ar(0,__context__) == 1) & (ar(1,__context__) == 2)) & (ar(2,__context__) == 3)) & (ar(3,__context__) == 4));
            TDim<struct Foo,2> arf = (([&]() -> TDim<struct Foo,2> {
                TDim<struct Foo,2> __mka_48; das_zero(__mka_48);
                __mka_48(0,__context__) = Foo(__context__);
                __mka_48(1,__context__) = Foo(__context__);
                return __mka_48;
            })());
            DAS_ASSERT((arf(0,__context__).i == 5) & (arf(1,__context__).i == 5));
            return true;
        }

        void registerAot ( AotLibrary & aotLib )
        {
            // Foo
            aotLib[0x35d4cae09b5ce919] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_AotCMRES<struct Foo  ( Context * __context__ ),Foo>>();
            };
            // test
            aotLib[0x7b2c8b57df5c6bab] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<bool  ( Context * __context__ ),test>>();
            };
        }
    }
}
