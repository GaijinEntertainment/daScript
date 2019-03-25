#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

namespace das {
    namespace aot {
        extern StructInfo __struct_info__b957795f;
        extern StructInfo __struct_info__d68b18bd;
        extern TypeInfo __type_info__5ffb01ae;
        extern TypeInfo __type_info__a7069c83;
        extern EnumInfo __enum_info__a831ecd8;

        EnumValueInfo __enum_info__a831ecd8_value_0 = { "flag_a", 1 };
        EnumValueInfo __enum_info__a831ecd8_value_1 = { "flag_b", 2 };
        EnumValueInfo * __enum_info__a831ecd8_values [] = { &__enum_info__a831ecd8_value_0, &__enum_info__a831ecd8_value_1 };
        EnumInfo __enum_info__a831ecd8 = { "Flags", __enum_info__a831ecd8_values, 2, 0xa831ecd8 };
        VarInfo __struct_info__b957795f_field_0 =  { Type::tStructure, &__struct_info__d68b18bd, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 14, 0xaed014c8, "f", 0 };
        VarInfo __struct_info__b957795f_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 4, 0xa7069c83, "s", 16 };
        VarInfo * __struct_info__b957795f_fields[2] =  { &__struct_info__b957795f_field_0, &__struct_info__b957795f_field_1 };
        StructInfo __struct_info__b957795f = {"Bar", __struct_info__b957795f_fields, 2, 24, 0, 0xb957795f };
        VarInfo __struct_info__d68b18bd_field_0 =  { Type::tEnumeration, nullptr, &__enum_info__a831ecd8, nullptr, nullptr, nullptr, 0, nullptr, 12, 0xc361e3c5, "flags", 0 };
        VarInfo __struct_info__d68b18bd_field_1 =  { Type::tEnumeration, nullptr, &__enum_info__a831ecd8, nullptr, nullptr, nullptr, 0, nullptr, 12, 0xc361e3c5, "more_flags", 4 };
        VarInfo __struct_info__d68b18bd_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 12, 0xf293c4e8, "i", 8 };
        VarInfo * __struct_info__d68b18bd_fields[3] =  { &__struct_info__d68b18bd_field_0, &__struct_info__d68b18bd_field_1, &__struct_info__d68b18bd_field_2 };
        StructInfo __struct_info__d68b18bd = {"Foo", __struct_info__d68b18bd_fields, 3, 12, 1, 0xd68b18bd };
        TypeInfo __type_info__5ffb01ae = { Type::tStructure, &__struct_info__b957795f, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 6, 0x5ffb01ae };
        TypeInfo __type_info__a7069c83 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 4, 0xa7069c83 };

        TypeInfo * __tinfo_0[3] = { &__type_info__a7069c83, &__type_info__5ffb01ae, &__type_info__a7069c83 };

        enum class Flags {
            flag_a = 1,
            flag_b = 2,
        };

        struct Foo {
            /*enum*/ Flags flags;
            /*enum*/ Flags more_flags; /* = Flags::flag_a */
            int32_t i;
        };
        static_assert(sizeof(Foo)==12,"structure size mismatch with DAS");
        static_assert(offsetof(Foo,flags)==0,"structure field offset mismatch with DAS");
        static_assert(offsetof(Foo,more_flags)==4,"structure field offset mismatch with DAS");
        static_assert(offsetof(Foo,i)==8,"structure field offset mismatch with DAS");

        struct Bar {
            struct Foo f; /* = Foo(__context__) */
            char * s; /* = "text here" */
        };
        static_assert(sizeof(Bar)==24,"structure size mismatch with DAS");
        static_assert(offsetof(Bar,f)==0,"structure field offset mismatch with DAS");
        static_assert(offsetof(Bar,s)==16,"structure field offset mismatch with DAS");

        struct Bar Bar ( Context * __context__ );
        struct Foo Foo ( Context * __context__ );
        bool test ( Context * __context__ );

        void __init_script ( Context * __context__ )
        {
        }

        struct Bar Bar ( Context * __context__ )
        {
            return (([&]() -> struct Bar {
                struct Bar __mks_13; das_zero(__mks_13);
                __mks_13.f = Foo(__context__);
                __mks_13.s = "text here";
                return __mks_13;
            })());
        }

        struct Foo Foo ( Context * __context__ )
        {
            return (([&]() -> struct Foo {
                struct Foo __mks_8; das_zero(__mks_8);
                __mks_8.more_flags = Flags::flag_a;
                return __mks_8;
            })());
        }

        bool test ( Context * __context__ )
        {
            struct Bar b = Bar(__context__);
            builtin_print(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_0, cast<char *>::from("b="), cast<struct Bar &>::from(b), cast<char *>::from("\n"))),__context__);
            return true;
        }

        void registerAot ( AotLibrary & aotLib )
        {
            // Bar
            aotLib[0xf2354c04681c1efd] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_AotCMRES<struct Bar  ( Context * __context__ ),Bar>>();
            };
            // Foo
            aotLib[0xcf6ecdf20d8c2fe5] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_AotCMRES<struct Foo  ( Context * __context__ ),Foo>>();
            };
            // test
            aotLib[0xe6595d456e9eea37] = [&](Context & ctx){
                return ctx.code->makeNode<SimNode_Aot<bool  ( Context * __context__ ),test>>();
            };
        }
    }
}
