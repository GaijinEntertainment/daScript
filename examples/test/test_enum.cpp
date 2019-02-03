#include "module_unitTest.h"

using namespace das;

// sample of enumeration

enum class SomeEnum {
    zero
,   one
,   two
};

SomeEnum efn_takeOne_giveTwo ( SomeEnum two, Context * ctx ) {
    if (two != SomeEnum::one) {
        ctx->throw_error("not one");
    }
    return SomeEnum::two;
}

class EnumerationSomeEnum : public Enumeration {
public:
    EnumerationSomeEnum() : Enumeration("SomeEnum") {
        add("zero", int32_t(SomeEnum::zero));
        add("one", int32_t(SomeEnum::one));
        add("two", int32_t(SomeEnum::two));
    }
};

template <>
struct cast <SomeEnum> {
    static __forceinline SomeEnum to ( vec4f x )            { return (SomeEnum) v_extract_xi(v_cast_vec4i(x)); }
    static __forceinline vec4f from ( SomeEnum x )          { return v_cast_vec4f(v_splatsi(int32_t(x))); }
};

template <>
struct das::typeFactory<SomeEnum> {
    static das::TypeDeclPtr make(const das::ModuleLibrary & library ) {
        return library.makeEnumType("SomeEnum");
    }
};

// sample of weak enumeration

enum SomeEnum98 {
    SomeEnum98_zero = 0
,   SomeEnum98_one  = 1
,   SomeEnum98_two  = 2
};

SomeEnum98 efn_takeOne_giveTwo_98 ( SomeEnum98 two, Context * ctx ) {
    if (two != SomeEnum98_one) {
        ctx->throw_error("not one");
    }
    return SomeEnum98_two;
}

enum class SomeEnum98_Proxy {
};

SomeEnum98_Proxy efn_takeOne_giveTwo_98_Proxy ( SomeEnum98_Proxy two, Context * ctx ) {
    return (SomeEnum98_Proxy) efn_takeOne_giveTwo_98( (SomeEnum98)two, ctx);
}

class EnumerationSomeEnum98 : public Enumeration {
public:
    EnumerationSomeEnum98() : Enumeration("SomeEnum_98") {
        add("zero", SomeEnum98_zero);
        add("one", SomeEnum98_one);
        add("two", SomeEnum98_two);
    }
};

template <>
struct cast <SomeEnum98_Proxy> {
    static __forceinline SomeEnum98_Proxy to ( vec4f x )            { return (SomeEnum98_Proxy) v_extract_xi(v_cast_vec4i(x)); }
    static __forceinline vec4f from ( SomeEnum98_Proxy x )          { return v_cast_vec4f(v_splatsi(int32_t(x))); }
};

template <>
struct das::typeFactory<SomeEnum98_Proxy> {
    static das::TypeDeclPtr make(const das::ModuleLibrary & library ) {
        return library.makeEnumType("SomeEnum_98");
    }
};


void Module_UnitTest::addEnumTest(ModuleLibrary &lib)
{
    // enum
    addEnumeration(make_shared<EnumerationSomeEnum>());
    addExtern<DAS_BIND_FUN(efn_takeOne_giveTwo)>(*this, lib, "efn_takeOne_giveTwo", SideEffects::none);
    // enum98
    addEnumeration(make_shared<EnumerationSomeEnum98>());
    addExtern<DAS_BIND_FUN(efn_takeOne_giveTwo_98_Proxy)>(*this, lib, "efn_takeOne_giveTwo_98", SideEffects::none);
};

