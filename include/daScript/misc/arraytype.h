#pragma once

#include "daScript/misc/basic_type.h"

namespace das {
    struct SimNode;
    struct TypeInfo;
    struct FuncInfo;
    struct LineInfoArg;

    class JitFn {
    private:
        template <typename RetT, typename... Args>
        friend struct CallJitFn; // Call it only using this helper
        void *jitFn;

    public:
        JitFn() : jitFn(nullptr) {}
        JitFn(void *jitFn) : jitFn(jitFn) {}

        operator bool() const {
            return jitFn != nullptr;
        }
    };

    struct Block {
        uint32_t stackOffset;
        uint32_t argumentsOffset;
        SimNode *body;
        void *aotFunction;
        JitFn jitFunction;
        vec4f *functionArguments;
        FuncInfo *info;
        __forceinline bool operator==(const Block &b) const {
            return b.stackOffset == stackOffset && b.argumentsOffset == argumentsOffset && b.body == body && b.functionArguments == functionArguments;
        }
    };

    class Context;

    typedef vec4f (*JitBlockFunction)(Context *, vec4f *, void *, Block *);
    typedef vec4f (*JitFunction)(Context *, vec4f *, void *);

    template <typename Result, typename... Args>
    struct TBlock : Block {
        TBlock() {}
        TBlock(const TBlock &) = default;
        TBlock(const Block &that) { *(Block *)this = that; }
    };

    struct Func {
        struct SimFunction *PTR;
        Func() : PTR(nullptr) {}
        Func(SimFunction *P) : PTR(P) {}
        Func(void *P) : PTR((SimFunction *)P) {}
        __forceinline operator void *() const {
            return PTR;
        }
        __forceinline explicit operator bool() const {
            return PTR != nullptr;
        }
        __forceinline bool operator==(void *ptr) const {
            return !ptr && !PTR;
        }
        __forceinline bool operator!=(void *ptr) const {
            return ptr || PTR;
        }
        __forceinline bool operator==(const Func &b) const {
            return PTR == b.PTR;
        }
        __forceinline bool operator!=(const Func &b) const {
            return PTR != b.PTR;
        }
    };
    static_assert(sizeof(Func) == sizeof(struct SimFunction *), "has to be castable");

    template <typename Result, typename... Args>
    struct TFunc : Func {
        TFunc() {}
        TFunc(const TFunc &) = default;
        TFunc(const Func &that) { *(Func *)this = that; }
    };

    struct Lambda {
        Lambda() : capture(nullptr) {}
        Lambda(void *ptr) : capture((char *)ptr) {}
        char *capture;
        __forceinline operator void *() const {
            return capture;
        }
        __forceinline TypeInfo *getTypeInfo() const {
            return capture ? *(TypeInfo **)(capture - 16) : nullptr;
        }
        __forceinline bool operator==(const Lambda &b) const {
            return capture == b.capture;
        }
        __forceinline bool operator!=(const Lambda &b) const {
            return capture != b.capture;
        }
        __forceinline bool operator==(void *ptr) const {
            return capture == ptr;
        }
        __forceinline bool operator!=(void *ptr) const {
            return capture != ptr;
        }
    };
    static_assert(sizeof(Lambda) == sizeof(void *), "has to be castable");

    template <typename Result, typename... Args>
    struct TLambda : Lambda {
        TLambda() {}
        TLambda(const TLambda &) = default;
        TLambda(const Lambda &that) { *(Lambda *)this = that; }
    };

    struct DAS_API GcRootLambda : Lambda {
        GcRootLambda() = default;
        GcRootLambda(const GcRootLambda &) = delete;
        GcRootLambda(GcRootLambda &&other) : Lambda(other.capture), context(other.context) {
            other.capture = nullptr;
            other.context = nullptr;
        }
        GcRootLambda &operator=(const GcRootLambda &) = delete;
        __forceinline GcRootLambda &operator=(GcRootLambda &&l) {
            if (this == &l)
                return *this;
            capture = l.capture;
            context = l.context;
            l.capture = nullptr;
            l.context = nullptr;
            return *this;
        }
        GcRootLambda(const Lambda &that, Context *_context);
        ~GcRootLambda();
        void reset() {
            capture = nullptr;
            context = nullptr;
        }
        Context *context = nullptr;
    };

    struct Tuple {
        Tuple() {}
    };

#pragma pack(1)
    struct Variant {
        int32_t index = 0;
        Variant() {}
    };
#pragma pack()

#define DAS_ARRAY_MAGIC 0xA11B3DA7 // magic number for array validation (alive data)

    struct Table;

    struct Array {
        char *data;
        uint64_t size;
        uint64_t capacity;
    // TArray, Table can set manually during copying.
    protected:
        // use friend helpers to lock-unlock.
        uint32_t magic;
        uint32_t lock;
    public:
        union {
            struct {
                bool shared : 1;
                bool hopeless : 1;          // needs to be deleted without fuss (exceptions)
                bool forego_lock_check : 1; // don't need to check if elements are locked
                bool tableNoHash : 1;       // Table only: key type stores no per-slot hash (non-string) - see tableHashSlotBytes
                bool borrowed : 1;          // a mark_locked view over foreign storage (fmap/temp_array) — array_forget_locked requires it, so an owned array that merely holds a lock can never be "forgotten" (leaked)
                bool scratch : 1;           // owner promises no interior alias escapes a grow: growth frees the old buffer eagerly even in a verySafeContext
            };
            uint32_t flags;
        };
        // On 32-bit ABIs Array has uint64_t members forcing 8-byte alignment, so
        // sizeof(Array) is 40 with 4 trailing pad bytes. Itanium ABI (clang on
        // wasm/linux/darwin 32-bit) lets the derived `Table` reuse that trailing
        // padding, placing Table::keys at offset 36; MSVC 32-bit doesn't, placing
        // it at offset 40. Make the padding explicit so the dsize matches sizeof
        // on every ABI and Table::keys lives at a single, predictable offset.
        uint32_t _pad_after_flags = 0u;
        __forceinline bool isLocked() const { return lock; }

        friend DAS_API int builtin_array_lock_count ( const Array & arr );
        friend DAS_API void array_mark_locked(Array &arr, void *data, uint64_t capacity);
        friend DAS_API void array_mark_locked(Array &arr, void *data, uint64_t size, uint64_t capacity);
        friend DAS_API bool array_forget_locked(Array &arr);
        friend DAS_API void array_lock(Context &context, Array &arr, LineInfo *at);
        friend DAS_API void array_unlock(Context &context, Array &arr, LineInfo *at);
        friend DAS_API void table_lock(Context &context, Table &arr, LineInfo *at);
        friend DAS_API void table_unlock(Context &context, Table &arr, LineInfo *at);
        template <typename KeyType> friend class TableHash;
    };

    class Context;

    DAS_API void array_mark_locked(Array &arr, void *data, uint64_t capacity);
    DAS_API void array_mark_locked(Array &arr, void *data, uint64_t size, uint64_t capacity);
    DAS_API void array_lock(Context &context, Array &arr, LineInfo *at);
    DAS_API void array_unlock(Context &context, Array &arr, LineInfo *at);
    DAS_API void array_reserve(Context &context, Array &arr, uint64_t newCapacity, uint32_t stride, LineInfo *at);
    DAS_API void array_reserve_scratch(Context &context, Array &arr, uint64_t newCapacity, uint32_t stride, LineInfo *at);  // eager even in a verySafeContext
    DAS_API void array_resize(Context &context, Array &arr, uint64_t newSize, uint32_t stride, bool zero, LineInfo *at);
    DAS_API void array_grow(Context &context, Array &arr, uint64_t newSize, uint32_t stride); // always grows
    DAS_API void array_clear(Context &context, Array &arr, LineInfo *at);

    typedef uint32_t TableHashKey;

#define DAS_TABLE_MAGIC 0xA11B37AB // magic number for table validation (alive table)

    struct Table : Array {
        char *keys;
        TableHashKey *hashes;
        uint64_t tombstones;
    };

    DAS_API void table_clear(Context &context, Table &arr, LineInfo *at);
    DAS_API void table_lock(Context &context, Table &arr, LineInfo *at);
    DAS_API void table_unlock(Context &context, Table &arr, LineInfo *at);
    DAS_API void table_reserve_impl(Context &context, Table &arr, int32_t baseType, uint64_t newCapacity, uint32_t valueTypeSize, LineInfo *at);

    struct Sequence;
    DAS_API void builtin_table_keys(Sequence &result, const Table &tab, int32_t stride, Context *__context__, LineInfoArg *at);
    DAS_API void builtin_table_values(Sequence &result, const Table &tab, int32_t stride, Context *__context__, LineInfoArg *at);
    DAS_API void builtin_table_get_key(void * result, const Table & tab, const void * value_ptr, int32_t value_stride, int32_t key_stride, Context * __context__, LineInfoArg * at);

    template<> struct ToBasicType<Sequence>     { enum { type = Type::tIterator }; };
    template<> struct ToBasicType<Sequence *>   { enum { type = Type::tIterator }; };
    template<> struct ToBasicType<Array *>      { enum { type = Type::tArray }; };
    template<> struct ToBasicType<Table *>      { enum { type = Type::tTable }; };
    template<> struct ToBasicType<Array>        { enum { type = Type::tArray }; };
    template<> struct ToBasicType<Table>        { enum { type = Type::tTable }; };
    template<> struct ToBasicType<Block>        { enum { type = Type::tBlock }; };
    template<> struct ToBasicType<Func>         { enum { type = Type::tFunction }; };
    template<> struct ToBasicType<Lambda>       { enum { type = Type::tLambda }; };
    template<> struct ToBasicType<Tuple>        { enum { type = Type::tTuple }; };
    template<> struct ToBasicType<Variant>      { enum { type = Type::tVariant }; };
    template<> struct ToBasicType<Context *>    { enum { type = Type::fakeContext }; };
    template<> struct ToBasicType<LineInfoArg *>{ enum { type = Type::fakeLineInfo }; };

}

