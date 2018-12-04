#pragma once

namespace yzg
{
    #pragma pack(1)
    struct Array
    {
        union {
            struct {
                char *      data;
                uint32_t    size        : 24;
                uint32_t    capacity    : 24;
                uint32_t    fieldSize   : 16;
            };
            __m128          _stub;
        };
    };
    #pragma pack()
    static_assert(sizeof(Array)==sizeof(__m128), "it has to fit into __m128");
}
