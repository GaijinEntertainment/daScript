#pragma once

#include "daScript/misc/memory_model.h"

namespace das {

    class StackAllocator {
    public:
        StackAllocator(const StackAllocator &) = delete;
        StackAllocator & operator = (const StackAllocator &) = delete;

        StackAllocator(uint32_t size) {
            stackSize = size;
            stack = stackSize ? (char*)das_aligned_alloc16(stackSize) : nullptr;
            reset();
        }

        virtual ~StackAllocator() {
            if ( stack ) {
                das_aligned_free16(stack);
                stack = nullptr;
            }
        }

        __forceinline void letGo () {
            stack = nullptr;
        }

        __forceinline void copy ( const StackAllocator & src ) {
            stack = src.stack;
            evalTop = src.evalTop;
            stackTop = src.stackTop;
            stackSize = src.stackSize;
        }

        __forceinline void acquire( const StackAllocator & owner, StackAllocator & ST ) {
            ST.copy(*this);
            copy(owner);
        }

        __forceinline void release( StackAllocator & ST ) {
            copy(ST);
        }

        __forceinline uint32_t size() const {
            return stackSize;
        }

        __forceinline bool empty() {
            return stackTop == (stack + stackSize);
        }

        __forceinline void reset() {
            evalTop = stackTop = stack + stackSize;
        }

        __forceinline bool push(uint32_t size, char * & EP, char * & SP ) {        // stack watermark
            DAS_ASSERTF(stack,"can't push on null stack");
            if (stackTop - size < stack ) {
                return false;
            }
            EP = evalTop;
            SP = stackTop;
            stackTop -= size;
            evalTop = stackTop;
            return true;
        }

        __forceinline void watermark ( char * & EP, char * & SP ) {
            EP = evalTop;
            SP = stackTop;
        }

        __forceinline void pop(char * EP, char * SP ) {    // restore stack to watermark
            evalTop = EP;
            stackTop = SP;
        }

        __forceinline void invoke(uint32_t et, char * & EP, char * & SP ) {    // pass bottom portion of the watermark
            EP = evalTop;
            SP = stackTop;
            evalTop = stack + et;
        }

        __forceinline char * ap() const {                // allocation stack
            return stackTop;
        }

        __forceinline char * sp() const {                // execution stack
            return evalTop;
        }

        __forceinline uint32_t spi() const {            // execution stack offset
            return uint32_t(evalTop - stack);
        }

        __forceinline char * bottom() const {            // bottom of the stack
            return stack;
        }

        __forceinline char * top() const {                // top of the stack
            return stack + stackSize;
        }
    protected:
        char *      stack = nullptr;
        char *      evalTop = nullptr;
        char *      stackTop = nullptr;
        uint32_t    stackSize = 0;
    };

    class HeapAllocator : public MemoryModel {
        enum { default_page_size = 4096 };
    public:
        HeapAllocator() : MemoryModel(default_page_size) {}
        char * allocateName ( const string & name );
    };

    struct StringHeader {
        uint32_t    hash;
        uint32_t    length;
    };
    static_assert(sizeof(StringHeader)==8, "has to be 8 bytes, or else");

    class StringAllocator : public HeapAllocator {
    public:
        StringAllocator() : HeapAllocator() { alignMask = 0; }
        virtual void setInitialSize ( uint32_t size );
        char * allocateString ( const char * text, uint32_t length );
        __forceinline char * allocateString ( const string & str ) {
            return allocateString ( str.c_str(), uint32_t(str.length()) );
        }
        void freeString ( char * text, uint32_t length );
        void reportAllocations();
    };

    struct NodePrefix {
        uint32_t    magic = 0xdeadc0de;
        uint32_t    size = 0;
        uint32_t    padd0 = 0, padd1 = 0;
        NodePrefix ( size_t sz ) : size(uint32_t(sz)) {}
    };
    static_assert(sizeof(NodePrefix)==sizeof(vec4f), "node prefix must be one alignment line");

    class NodeAllocator : public HeapAllocator {
    public:
        bool prefixWithHeader = true;
        uint32_t totalNodesAllocated = 0;
    public:
        NodeAllocator() = default;

        template<typename TT, typename... Params>
        __forceinline TT * makeNode(Params... args) {
            totalNodesAllocated ++;
            if ( prefixWithHeader ) {
                char * data = allocate(sizeof(TT) + sizeof(NodePrefix));
                new (data) NodePrefix(sizeof(TT));
                return new (data + sizeof(NodePrefix)) TT(args...);
            } else {
                return new (allocate(sizeof(TT))) TT(args...);
            }
        }

        template < template <typename TT> class NodeType, typename... Params>
        SimNode * makeNumericValueNode(Type baseType, Params... args) {
            switch (baseType) {
                case Type::tInt64:      return makeNode<NodeType<int64_t>>(args...);
                case Type::tUInt64:     return makeNode<NodeType<uint64_t>>(args...);
                case Type::tInt:        return makeNode<NodeType<int32_t>>(args...);
                case Type::tUInt:       return makeNode<NodeType<uint32_t>>(args...);
                case Type::tFloat:      return makeNode<NodeType<float>>(args...);
                case Type::tDouble:     return makeNode<NodeType<double>>(args...);
                case Type::tPointer:    return makeNode<NodeType<void *>>(args...);
                case Type::tLambda:     return makeNode<NodeType<Lambda>>(args...);
                default:
                    DAS_ASSERTF(0, "we should not even be here. we are calling makeNumericValueNode on an uspported baseType."
                                "likely new numeric type been added.");
                    return nullptr;
            }
        }

        template < template <typename TT> class NodeType, typename... Params>
        SimNode * makeValueNode(Type baseType, Params... args) {
            switch (baseType) {
            case Type::tBool:           return makeNode<NodeType<bool>>(args...);
            case Type::tInt8:           return makeNode<NodeType<int8_t>>(args...);
            case Type::tUInt8:          return makeNode<NodeType<uint8_t>>(args...);
            case Type::tInt16:          return makeNode<NodeType<int16_t>>(args...);
            case Type::tUInt16:         return makeNode<NodeType<uint16_t>>(args...);
            case Type::tInt64:          return makeNode<NodeType<int64_t>>(args...);
            case Type::tUInt64:         return makeNode<NodeType<uint64_t>>(args...);
            case Type::tEnumeration:    return makeNode<NodeType<int32_t>>(args...);
            case Type::tInt:            return makeNode<NodeType<int32_t>>(args...);
            case Type::tInt2:           return makeNode<NodeType<int2>>(args...);
            case Type::tInt3:           return makeNode<NodeType<int3>>(args...);
            case Type::tInt4:           return makeNode<NodeType<int4>>(args...);
            case Type::tUInt:           return makeNode<NodeType<uint32_t>>(args...);
            case Type::tUInt2:          return makeNode<NodeType<uint2>>(args...);
            case Type::tUInt3:          return makeNode<NodeType<uint3>>(args...);
            case Type::tUInt4:          return makeNode<NodeType<uint4>>(args...);
            case Type::tFloat:          return makeNode<NodeType<float>>(args...);
            case Type::tFloat2:         return makeNode<NodeType<float2>>(args...);
            case Type::tFloat3:         return makeNode<NodeType<float3>>(args...);
            case Type::tFloat4:         return makeNode<NodeType<float4>>(args...);
            case Type::tRange:          return makeNode<NodeType<range>>(args...);
            case Type::tURange:         return makeNode<NodeType<urange>>(args...);
            case Type::tString:         return makeNode<NodeType<char *>>(args...);
            case Type::tPointer:        return makeNode<NodeType<void *>>(args...);
            case Type::tIterator:       return makeNode<NodeType<void *>>(args...);
            case Type::tFunction:       return makeNode<NodeType<Func>>(args...);
            case Type::tLambda:         return makeNode<NodeType<Lambda>>(args...);
            case Type::tDouble:         return makeNode<NodeType<double>>(args...);
            default:
                DAS_ASSERTF(0, "we should not even be here. we are calling makeValueNode on an uspported baseType."
                              "likely new by-value builtin type been added.");
                return nullptr;
            }
        }

        template < template <int TT> class NodeType, typename... Params>
        SimNode * makeNodeUnroll(int count, Params... args) {
            switch (count) {
            case  0: return makeNode<NodeType< 0>>(args...);
            case  1: return makeNode<NodeType< 1>>(args...);
            case  2: return makeNode<NodeType< 2>>(args...);
            case  3: return makeNode<NodeType< 3>>(args...);
            case  4: return makeNode<NodeType< 4>>(args...);
            case  5: return makeNode<NodeType< 5>>(args...);
            case  6: return makeNode<NodeType< 6>>(args...);
            case  7: return makeNode<NodeType< 7>>(args...);
            case  8: return makeNode<NodeType< 8>>(args...);
            case  9: return makeNode<NodeType< 9>>(args...);
            case 10: return makeNode<NodeType<10>>(args...);
            case 11: return makeNode<NodeType<11>>(args...);
            case 12: return makeNode<NodeType<12>>(args...);
            case 13: return makeNode<NodeType<13>>(args...);
            case 14: return makeNode<NodeType<14>>(args...);
            case 15: return makeNode<NodeType<15>>(args...);
            case 16: return makeNode<NodeType<16>>(args...);
            case 17: return makeNode<NodeType<17>>(args...);
            case 18: return makeNode<NodeType<18>>(args...);
            case 19: return makeNode<NodeType<19>>(args...);
            case 20: return makeNode<NodeType<20>>(args...);
            case 21: return makeNode<NodeType<21>>(args...);
            case 22: return makeNode<NodeType<22>>(args...);
            case 23: return makeNode<NodeType<23>>(args...);
            case 24: return makeNode<NodeType<24>>(args...);
            case 25: return makeNode<NodeType<25>>(args...);
            case 26: return makeNode<NodeType<26>>(args...);
            case 27: return makeNode<NodeType<27>>(args...);
            case 28: return makeNode<NodeType<28>>(args...);
            case 29: return makeNode<NodeType<29>>(args...);
            case 30: return makeNode<NodeType<30>>(args...);
            case 31: return makeNode<NodeType<31>>(args...);
            case 32: return makeNode<NodeType<32>>(args...);
            default:
                DAS_ASSERTF(0, "we should not even be here. we are calling makeNodeUnroll on a large number or a negative number."
                            "if its negative, there is some issue with the logic of count."
                            "if its large, bigger specialization for unroll should be added.");
                return nullptr;
            }
        }
    };

    class DebugInfoAllocator : public NodeAllocator {
    public:
        DebugInfoAllocator() {
            prefixWithHeader = false;
            initial_page_count = 1;
            pageSize = 1024;
        }
        virtual uint32_t growPages(uint32_t pages) const override { return pages; }
        map<uint32_t,TypeInfo *>    lookup;
    };
}
