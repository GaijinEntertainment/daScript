#pragma once

namespace das
{
    struct SimNode;

    struct Block {
        uint32_t    stackOffset;
        uint32_t    argumentsOffset;
        SimNode *   body;
        vec4f *     functionArguments;
        __forceinline bool operator == ( const Block & b ) const {
            return b.stackOffset==stackOffset && b.argumentsOffset==argumentsOffset
                && b.body==body && b.functionArguments==b.functionArguments;
        }
    };

    struct Func {
        int32_t     index;
        __forceinline bool operator == ( const Func & b ) const {
            return index == b.index;
        }
        __forceinline bool operator != ( const Func & b ) const {
            return index != b.index;
        }
    };
    static_assert(sizeof(Func)==sizeof(int32_t), "has to be castable");

    struct Lambda {
        void *      capture;
        __forceinline bool operator == ( const Lambda & b ) const {
            return capture == b.capture;
        }
        __forceinline bool operator != ( const Lambda & b ) const {
            return capture != b.capture;
        }
    };
    static_assert(sizeof(Lambda)==sizeof(void *), "has to be castable");

    struct Array {
        char *      data;
        uint32_t    size;
        uint32_t    capacity;
        uint32_t    lock;
    };

    struct Table : Array {
        char *      keys;
        uint32_t *  hashes;
        uint32_t    maxLookups;
        uint32_t    shift;
    };

    struct EnumStub {
        int32_t     value;
    };
}
