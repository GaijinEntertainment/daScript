#pragma once

namespace yzg
{
    struct SimNode;
    
    struct Block {
        uint32_t    stackOffset;
        uint32_t    argumentsOffset;
        SimNode *   body;
    };
    
    struct Array {
        char *      data;
        uint32_t    size;
        uint32_t    capacity;
        uint32_t    lock;
    };
    
    struct Table : Array {
        char *      keys;
        int8_t *    distance;
        uint32_t    maxLookups;
    };
}
