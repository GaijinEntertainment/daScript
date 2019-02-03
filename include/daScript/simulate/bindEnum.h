#pragma once
#include "forEach.h"

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
        for (uint32_t i = 0; i < sizeof(enumArray)/sizeof(enumArray[0]); ++i)\
            add(enumArrayName[i], int32_t(enumArray[i]));\
    }\
};

#define DAS_BASE_BIND_ENUM(enum_name, das_enum_name, ...) \
  DAS_BASE_BIND_ENUM_BOTH(DAS_BIND_ENUM_QUALIFIED_HELPER, enum_name, das_enum_name, __VA_ARGS__)\
  DAS_BASE_BIND_ENUM_CAST(enum_name, #das_enum_name)

#define DAS_BASE_BIND_ENUM_98(enum_name, das_enum_name, ...) \
  enum class das_enum_name##_DasProxy {};\
  DAS_BASE_BIND_ENUM_BOTH(DAS_BIND_ENUM_UNQUALIFIED_HELPER, enum_name, das_enum_name, __VA_ARGS__)\
  DAS_BASE_BIND_ENUM_CAST(das_enum_name##_DasProxy, #das_enum_name)
