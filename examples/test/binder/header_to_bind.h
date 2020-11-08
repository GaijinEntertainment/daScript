#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


enum FirstEnum {
    FirstEnum_zero = 0
,   FirstEnum_one  = 1
,   FirstEnum_two  = 2
};

enum SecondEnum {
    SecondEnum_zero = 0
,   SecondEnum_one  = 1
,   SecondEnum_two  = 2
};

struct FirstStruct {
    bool bool_field;
    int32_t int_field;
    float float_field;
};

struct SecondStruct {
    bool bool_field;
    int32_t int_field;
    float float_field;
};

//FIXME: following are used in vulkan but not supported by das yet

/*
struct StructWithArray {
    char some_chars[10];
};
*/

/*
struct StructWithBitFields {
    uint32_t field24:24;
    uint32_t field8:8;
};
*/

/*
struct StructWithSizeT {
    size_t my_size;
};
*/

/*
typedef struct SomeOpaqueData_T * SomeOpaqueHandle;
struct StructWithOpaqueHandle {
    SomeOpaqueHandle handle;
};
*/
