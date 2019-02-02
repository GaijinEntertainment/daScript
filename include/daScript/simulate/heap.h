#pragma once

namespace das {

    struct StringHeader {
        uint32_t    hash;
        uint32_t    length;
    };
    static_assert(sizeof(StringHeader)==8, "has to be 8 bytes, or else");

    class LinearAllocator {
    public:
        LinearAllocator() = delete;
        LinearAllocator(const LinearAllocator &) = delete;
        LinearAllocator & operator = (const LinearAllocator &) = delete;

        LinearAllocator(uint32_t size) {
            linearAllocatorSize = size;
            linearAllocator = linearAllocatorBase = (char *)_mm_malloc(linearAllocatorSize, 16);
        }

        ~LinearAllocator() {
            _mm_free(linearAllocatorBase);
        }

        __forceinline uint32_t bytesAllocated() const {
            return uint32_t(linearAllocator - linearAllocatorBase);
        }

        __forceinline uint32_t bytesTotal() const {
            return linearAllocatorSize;
        }

        __forceinline void reset() {
            linearAllocator = linearAllocatorBase;
        }

        __forceinline void * getWatermark() const {
            return linearAllocator;
        }

        __forceinline void setWatermark(void * watermark) {
            assert(watermark >= linearAllocatorBase && watermark < (linearAllocatorBase + linearAllocatorSize));
            linearAllocator = (char *) watermark;
        }

        __forceinline void * reallocate(void * oldData, uint32_t oldSize, uint32_t size) {
            if (size <= oldSize) return oldData;
            size = (size + 0x0f) & ~0x0f;
            oldSize = (oldSize + 0x0f) & ~0x0f;
            if (oldData && (oldData == linearAllocator - oldSize)) {
                auto extra = size - oldSize;
                if (linearAllocator - linearAllocatorBase + extra > linearAllocatorSize) {
                    return nullptr;
                }
                linearAllocator += extra;
                return oldData;
            }
            else {
                return allocate(size);
            }
        }

        __forceinline void * allocate(uint32_t size) {
            size = (size + 0x0f) & ~0x0f;
            if (linearAllocator - linearAllocatorBase + size > linearAllocatorSize) {
                return nullptr;
            }
            auto res = linearAllocator;
            linearAllocator += size;
            return res;
        }

        __forceinline char * allocateName(const string & name) {
            if (!name.empty()) {
                auto size = uint32_t(name.length() + 1);
                if (auto str = (char *)allocate(size)) {
                    memcpy(str, name.c_str(), size);
                    return str;
                }
            }
            return nullptr;
        }

        __forceinline char * allocateString ( const char * text, uint32_t length ) {
            if ( length==0 ) {
                return nullptr;
            } else if ( char * str = (char *) allocate(1+length+sizeof(StringHeader)) ) {
                auto header = (StringHeader *) str;
                header->hash = 0;
                header->length = length;
                auto stxt = str + sizeof(StringHeader);
                if ( text ) {
                    memcpy ( stxt, text, length );
                }
                stxt[length] = 0;
                return stxt;
            } else {
                return nullptr;
            }
        }

        __forceinline char * allocateString ( const string & str ) {
            return allocateString ( str.c_str(), uint32_t(str.length()) );
        }

        __forceinline bool isHeapPtr ( const char * ptr ) const {
            return uintptr_t(ptr - linearAllocatorBase) < uintptr_t(linearAllocatorSize);
        }

    protected:
        uint32_t    linearAllocatorSize;
        char *        linearAllocator = nullptr;
        char *        linearAllocatorBase = nullptr;
    };

    class StackAllocator {
    public:
        StackAllocator(const StackAllocator &) = delete;
        StackAllocator & operator = (const StackAllocator &) = delete;

        StackAllocator(uint32_t size) {
            stackSize = size;
            stack = (char *)_mm_malloc(stackSize, 16);
            reset();
        }

        ~StackAllocator() {
            _mm_free(stack);
        }

        __forceinline uint32_t size() const {
            return stackSize;
        }

        __forceinline void reset() {
            evalTop = stackTop = stack + stackSize;
        }

        __forceinline bool push(uint32_t size, char * & EP, char * & SP ) {        // stack watermark
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
        char *        stack = nullptr;
        char *        evalTop = nullptr;
        char *        stackTop = nullptr;
        uint32_t    stackSize;
    };


    class NodeAllocator : public LinearAllocator {
    public:
        NodeAllocator(uint32_t size) : LinearAllocator(size) {}

        template<typename TT, typename... Params>
        __forceinline TT * makeNode(Params... args) {
            return new (allocate(sizeof(TT))) TT(args...);
        }

        template < template <typename TT> class NodeType, typename... Params>
        SimNode * makeNumericValueNode(Type baseType, Params... args) {
            switch (baseType) {
                case Type::tInt64:      return makeNode<NodeType<int64_t>>(args...);
                case Type::tUInt64:     return makeNode<NodeType<uint64_t>>(args...);
                case Type::tInt:        return makeNode<NodeType<int32_t>>(args...);
                case Type::tUInt:       return makeNode<NodeType<uint32_t>>(args...);
                case Type::tFloat:      return makeNode<NodeType<float>>(args...);
                default:
                    assert(0 && "we should not even be here");
                    return nullptr;
            }
        }

        template < template <typename TT> class NodeType, typename... Params>
        SimNode * makeValueNode(Type baseType, Params... args) {
            switch (baseType) {
            case Type::tBool:           return makeNode<NodeType<bool>>(args...);
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
            case Type::tBlock:          return makeNode<NodeType<Block>>(args...);
            default:
                assert(0 && "we should not even be here");
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
            default:
                assert(0 && "we should not even be here");
                return nullptr;
            }
        }
    };
}
