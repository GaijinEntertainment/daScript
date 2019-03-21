#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

namespace das {
    namespace aot {
        extern StructInfo __struct_info__b620f16e;
        extern TypeInfo __type_info__48d908bf;
        extern TypeInfo __type_info__a7069c83;

        VarInfo __struct_info__b620f16e_field_0 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 12, 0xf293c4e8, "a", 0 };
        VarInfo __struct_info__b620f16e_field_1 =  { Type::tFloat, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 12, 0xfa15db70, "b", 4 };
        VarInfo * __struct_info__b620f16e_fields[2] =  { &__struct_info__b620f16e_field_0, &__struct_info__b620f16e_field_1 };
        StructInfo __struct_info__b620f16e = {"Foo", __struct_info__b620f16e_fields, 2, 8, 0, 0xb620f16e };
        TypeInfo __type_info__48d908bf = { Type::tStructure, &__struct_info__b620f16e, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 14, 0x48d908bf };
        TypeInfo __type_info__a7069c83 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 4, 0xa7069c83 };

        TypeInfo * __tinfo_0[1] = { &__type_info__a7069c83 };
        TypeInfo * __tinfo_1[1] = { &__type_info__48d908bf };
        TypeInfo * __tinfo_2[1] = { &__type_info__a7069c83 };
        TypeInfo * __tinfo_3[1] = { &__type_info__a7069c83 };
        TypeInfo * __tinfo_4[1] = { &__type_info__a7069c83 };
        TypeInfo * __tinfo_5[1] = { &__type_info__a7069c83 };

        struct Foo {
            int32_t a; /* = 19 */
            float b; /* = 20.f */
        };
        static_assert(sizeof(Foo)==8,"structure size mismatch with DAS");

        struct Foo Foo ( Context * __context__ );
        char * make_another_name ( Context * __context__, char * x );
        char * make_g_name ( Context * __context__, char * x );
        char * make_name ( Context * __context__, char * x );
        char * name_foo ( Context * __context__ );
        void stil_not_writing ( Context * __context__, char * & a, char * & b );
        char * strcat ( Context * __context__, char * a, char * b );
        bool test ( Context * __context__ );
        void up_foo ( Context * __context__, struct Foo & a );

        void __init_script ( Context * __context__ )
        {
            das_global<bool,0>(__context__) = true; /*g_cond*/
            das_global<char *,16>(__context__) = "_global_ok"; /*g_name*/
        }

        struct Foo Foo ( Context * __context__ )
        {
            return (([&]() -> struct Foo {
                struct Foo __mks_27; das_zero(__mks_27);
                __mks_27.a = 19;
                __mks_27.b = 20.f;
                return __mks_27;
            })());
        }

        char * make_another_name ( Context * __context__, char * x )
        {
            DAS_ASSERT((SimPolicy<char *>::Equ(cast<char *>::from(x),cast<char *>::from("modify_arg"),*__context__)));
            if ( das_global<bool,0>(__context__) /*g_cond*/ )
            {
                builtin_print(das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_0, cast<char *>::from(x))),__context__);
            };
            return cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(x),cast<char *>::from("_ok"),*__context__));
        }

        char * make_g_name ( Context * __context__, char * x )
        {
            return cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(x),cast<char *>::from(das_global<char *,16>(__context__) /*g_name*/),*__context__));
        }

        char * make_name ( Context * __context__, char * x )
        {
            return strcat(__context__,"hello_",x);
        }

        char * name_foo ( Context * __context__ )
        {
            struct Foo t = Foo(__context__);
            up_foo(__context__,das_arg<struct Foo>::pass(t));
            return das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_1, cast<struct Foo &>::from(t)));
        }

        void stil_not_writing ( Context * __context__, char * & a, char * & b )
        {
            DAS_ASSERT((SimPolicy<char *>::Equ(cast<char *>::from(a),cast<char *>::from("blah"),*__context__)));
            DAS_ASSERT((SimPolicy<char *>::Equ(cast<char *>::from(b),cast<char *>::from("_global_ok"),*__context__)));
            a = "it";
        }

        char * strcat ( Context * __context__, char * a, char * b )
        {
            char * c = a;
            SimPolicy<char *>::SetAdd((char *)&(c),cast<char *>::from(b),*__context__);
            return c;
        }

        bool test ( Context * __context__ )
        {
            das_global<bool,0>(__context__) /*g_cond*/ = false;
            char * t = make_name(__context__,"world");
            DAS_ASSERT((SimPolicy<char *>::Equ(cast<char *>::from(das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_2, cast<char *>::from(t)))),cast<char *>::from("hello_world"),*__context__)));
            char * q = make_another_name(__context__,"modify_arg");
            DAS_ASSERT((SimPolicy<char *>::Equ(cast<char *>::from(das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_3, cast<char *>::from(q)))),cast<char *>::from("modify_arg_ok"),*__context__)));
            char * w = name_foo(__context__);
            DAS_ASSERT((SimPolicy<char *>::Equ(cast<char *>::from(das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_4, cast<char *>::from(w)))),cast<char *>::from("[[ 20; 20]]"),*__context__)));
            char * g = make_g_name(__context__,"touch_global");
            DAS_ASSERT((SimPolicy<char *>::Equ(cast<char *>::from(das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_5, cast<char *>::from(g)))),cast<char *>::from("touch_global_global_ok"),*__context__)));
            char * T = "blah";
            stil_not_writing(__context__,T,das_global<char *,16>(__context__) /*g_name*/);
            DAS_ASSERT((SimPolicy<char *>::Equ(cast<char *>::from(T),cast<char *>::from("it"),*__context__)));
            char * tt = "_global_ok";
            T = "blah";
            stil_not_writing(__context__,T,tt);
            return true;
        }

        void up_foo ( Context * __context__, struct Foo &  a )
        {
            a.a++;
        }

        void registerAot ( AotLibrary & aotLib )
        {
            // Foo
            aotLib[0x2a31149c01f9205a] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_AotCMRES<struct Foo  ( Context * __context__ ),Foo>>();
            };
            // make_another_name
            aotLib[0xe829151dcb92f160] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<char *  ( Context * __context__, char * x ),make_another_name>>();
            };
            // make_g_name
            aotLib[0xc7427908fe894462] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<char *  ( Context * __context__, char * x ),make_g_name>>();
            };
            // make_name
            aotLib[0x7f40c2ad425df2ec] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<char *  ( Context * __context__, char * x ),make_name>>();
            };
            // name_foo
            aotLib[0x7e18a7fd28ae5051] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<char *  ( Context * __context__ ),name_foo>>();
            };
            // stil_not_writing
            aotLib[0x6de94488033e48fe] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, char * & a, char * & b ),stil_not_writing>>();
            };
            // strcat
            aotLib[0x6f432d291648d88e] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<char *  ( Context * __context__, char * a, char * b ),strcat>>();
            };
            // test
            aotLib[0xda093c962b16f0d9] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<bool  ( Context * __context__ ),test>>();
            };
            // up_foo
            aotLib[0x715156189dd7d9ae] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void  ( Context * __context__, struct Foo & a ),up_foo>>();
            };
            // [[ init script ]]
            aotLib[0xe2e00a5d2a01de8e] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<void (Context *), __init_script>>();
            };
        }
    }
}
