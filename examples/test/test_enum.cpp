#include "module_unitTest.h"

using namespace das;

#define DAS_EXPAND(x) x
#define  DAS_FOR_EACH_1(WHAT, ARG, X) WHAT(X, ARG)
#define  DAS_FOR_EACH_2(WHAT, ARG, X, ...) WHAT(X, ARG) DAS_EXPAND(DAS_FOR_EACH_1(WHAT, ARG, __VA_ARGS__))
#define  DAS_FOR_EACH_3(WHAT, ARG, X, ...) WHAT(X, ARG) DAS_EXPAND(DAS_FOR_EACH_2(WHAT, ARG, __VA_ARGS__))
#define  DAS_FOR_EACH_4(WHAT, ARG, X, ...) WHAT(X, ARG) DAS_EXPAND(DAS_FOR_EACH_3(WHAT, ARG, __VA_ARGS__))
#define  DAS_FOR_EACH_5(WHAT, ARG, X, ...) WHAT(X, ARG) DAS_EXPAND(DAS_FOR_EACH_4(WHAT, ARG, __VA_ARGS__))
#define  DAS_FOR_EACH_6(WHAT, ARG, X, ...) WHAT(X, ARG) DAS_EXPAND(DAS_FOR_EACH_5(WHAT, ARG, __VA_ARGS__))
#define  DAS_FOR_EACH_7(WHAT, ARG, X, ...) WHAT(X, ARG) DAS_EXPAND(DAS_FOR_EACH_6(WHAT, ARG, __VA_ARGS__))
#define  DAS_FOR_EACH_8(WHAT, ARG, X, ...) WHAT(X, ARG) DAS_EXPAND(DAS_FOR_EACH_7(WHAT, ARG, __VA_ARGS__))
#define  DAS_FOR_EACH_9(WHAT, ARG, X, ...) WHAT(X, ARG) DAS_EXPAND(DAS_FOR_EACH_8(WHAT, ARG, __VA_ARGS__))
#define DAS_FOR_EACH_10(WHAT, ARG, X, ...) WHAT(X, ARG) DAS_EXPAND(DAS_FOR_EACH_9(WHAT, ARG, __VA_ARGS__))
#define DAS_FOR_EACH_11(WHAT, ARG, X, ...) WHAT(X, ARG)DAS_EXPAND(DAS_FOR_EACH_10(WHAT, ARG, __VA_ARGS__))
#define DAS_FOR_EACH_12(WHAT, ARG, X, ...) WHAT(X, ARG)DAS_EXPAND(DAS_FOR_EACH_11(WHAT, ARG, __VA_ARGS__))
#define DAS_FOR_EACH_13(WHAT, ARG, X, ...) WHAT(X, ARG)DAS_EXPAND(DAS_FOR_EACH_12(WHAT, ARG, __VA_ARGS__))
#define DAS_FOR_EACH_14(WHAT, ARG, X, ...) WHAT(X, ARG)DAS_EXPAND(DAS_FOR_EACH_13(WHAT, ARG, __VA_ARGS__))
#define DAS_FOR_EACH_15(WHAT, ARG, X, ...) WHAT(X, ARG)DAS_EXPAND(DAS_FOR_EACH_14(WHAT, ARG, __VA_ARGS__))
#define DAS_FOR_EACH_16(WHAT, ARG, X, ...) WHAT(X, ARG)DAS_EXPAND(DAS_FOR_EACH_15(WHAT, ARG, __VA_ARGS__))
//... repeat as needed

#define DAS_FOR_EACH_NARG(...) DAS_FOR_EACH_NARG_(__VA_ARGS__, DAS_FOR_EACH_RSEQ_N())
#define DAS_FOR_EACH_NARG_(...) DAS_EXPAND(DAS_FOR_EACH_ARG_N(__VA_ARGS__))
#define DAS_FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N
#define DAS_FOR_EACH_RSEQ_N() 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define CONCATENATE(x,y) x##y
#define DAS_FOR_EACH_(N, what, arg, ...) DAS_EXPAND(CONCATENATE(DAS_FOR_EACH_, N)(what, arg, __VA_ARGS__))
#define DAS_FOR_EACH(what, arg, ...) DAS_FOR_EACH_(DAS_FOR_EACH_NARG(__VA_ARGS__), what, arg, __VA_ARGS__)

#define DAS_BIND_ENUM_PRINT_HELPER(x, ARG) #x,
#define DAS_BIND_ENUM_QUALIFIED_HELPER(x, ARG) ARG::x,
#define DAS_BIND_ENUM_UNQUALIFIED_HELPER(x, ARG) x,

// sample of enumeration
#define DAS_BASE_BIND_ENUM_CAST(enum_name, das_enum_name)\
template <>\
struct das::cast <enum_name> {\
  static __forceinline enum_name to ( vec4f x )            { return (enum_name) v_extract_xi(v_cast_vec4i(x)); }\
  static __forceinline vec4f from ( enum_name x )          { return v_cast_vec4f(v_splatsi(int32_t(x))); }\
};\
template <>\
struct das::typeFactory<enum_name> {\
    static das::TypeDeclPtr make(const das::ModuleLibrary & library ) {\
        return library.makeEnumType(das_enum_name);\
    }\
};

#define DAS_BIND_ENUM_CAST(enum_name) DAS_BASE_BIND_ENUM_CAST(enum_name, #enum_name)

#define DAS_BASE_BIND_ENUM_BOTH(helper, enum_name, das_enum_name, ...) \
class Enumeration##das_enum_name : public das::Enumeration {\
public:\
    Enumeration##das_enum_name() : das::Enumeration(#das_enum_name) {\
        enum_name enumArray[] = { DAS_FOR_EACH(helper, enum_name, __VA_ARGS__) };\
        static const char *enumArrayName[] = { DAS_FOR_EACH(DAS_BIND_ENUM_PRINT_HELPER, enum_name, __VA_ARGS__) };\
        for (uint32_t i = 0; i < sizeof(enumArrayName)/sizeof(enumArrayName[0]); ++i)\
            add(enumArrayName[i], int32_t(i));\
    }\
};

#define DAS_BASE_BIND_ENUM(enum_name, das_enum_name, ...) \
  DAS_BASE_BIND_ENUM_BOTH(DAS_BIND_ENUM_QUALIFIED_HELPER, enum_name, das_enum_name, __VA_ARGS__)\
  DAS_BASE_BIND_ENUM_CAST(enum_name, #das_enum_name)

#define DAS_BASE_BIND_ENUM_98(enum_name, das_enum_name, ...) \
  enum class SomeEnum98##_DasProxy {};\
  DAS_BASE_BIND_ENUM_BOTH(DAS_BIND_ENUM_UNQUALIFIED_HELPER, enum_name, das_enum_name, __VA_ARGS__)\
  DAS_BASE_BIND_ENUM_CAST(enum_name##_DasProxy, #das_enum_name)

enum class SomeEnum {
    zero
,   one
,   two
};

DAS_BASE_BIND_ENUM(SomeEnum, SomeEnum, zero, one, two)

SomeEnum efn_takeOne_giveTwo ( SomeEnum one) {
    return (one == SomeEnum::one) ? SomeEnum::two : SomeEnum::zero;
}


// sample of weak enumeration

enum SomeEnum98 {
    SomeEnum98_zero = 0
,   SomeEnum98_one  = 1
,   SomeEnum98_two  = 2
};

DAS_BASE_BIND_ENUM_98(SomeEnum98, SomeEnum98, SomeEnum98_zero, SomeEnum98_one, SomeEnum98_two)

SomeEnum98 efn_takeOne_giveTwo_98 ( SomeEnum98 one ) {
    return (one == SomeEnum98_one) ? SomeEnum98_two : SomeEnum98_zero;
}

SomeEnum98_DasProxy efn_takeOne_giveTwo_98_DasProxy ( SomeEnum98_DasProxy two) {
    return (SomeEnum98_DasProxy) efn_takeOne_giveTwo_98( (SomeEnum98)two);
}

void Module_UnitTest::addEnumTest(ModuleLibrary &lib)
{
    // enum
    addEnumeration(make_shared<EnumerationSomeEnum>());
    addExtern<DAS_BIND_FUN(efn_takeOne_giveTwo)>(*this, lib, "efn_takeOne_giveTwo", SideEffects::none);
    // enum98
    addEnumeration(make_shared<EnumerationSomeEnum98>());
    addExtern<DAS_BIND_FUN(efn_takeOne_giveTwo_98_DasProxy)>(*this, lib, "efn_takeOne_giveTwo_98", SideEffects::none);
};

