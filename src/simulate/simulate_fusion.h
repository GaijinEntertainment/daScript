#pragma once

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/simulate_nodes.h"

#ifndef DAS_FUSION
    #define DAS_FUSION  0
#endif

// Safe vec4f load for a workhorse-sized source. Picks the load width by sizeof(CTYPE)
// at compile time so we never v_ldu past a 4/8/12-byte source — important when the
// source sits within 16 bytes of an unmapped page (real SIGSEGV under TSan, not just
// a sanitizer over-read warning). Each branch is a single SIMD intrinsic; memcpy
// fallback only on exotic sizes.
#define DAS_LDU_WORKHORSE(dst, src_ptr, CTYPE) \
    do { \
        if constexpr (sizeof(CTYPE) == sizeof(vec4f))         (dst) = v_ldu((const float *)(src_ptr)); \
        else if constexpr (sizeof(CTYPE) == sizeof(float) * 3) (dst) = v_ldu_p3_safe((const float *)(src_ptr)); \
        else if constexpr (sizeof(CTYPE) == sizeof(float) * 2) (dst) = v_ldu_half((const float *)(src_ptr)); \
        else if constexpr (sizeof(CTYPE) == sizeof(float))     (dst) = v_set_x(*(const float *)(src_ptr)); \
        else { vec4f __r = v_zero(); memcpy(&__r, (src_ptr), sizeof(CTYPE)); (dst) = __r; } \
    } while (0)

namespace das {

    typedef char * StringPtr;
    typedef void * VoidPtr;

    __forceinline vec4f fusionLoadArgument ( const char * src, uint32_t size ) {
        switch ( size ) {
            case 4:     return v_ldu_x((const float *)src);
            case 8:     return v_ldu_half(src);
            case 16:    return v_ldu((const float *)src);
            case 1:     return v_cast_vec4f(v_seti_x(*(const uint8_t *)src));
            case 2:     return v_cast_vec4f(v_seti_x(*(const uint16_t *)src));
            default: {
                vec4f result = v_zero();
                memcpy(&result, src, size);
                return result;
            }
        }
    }

    struct SimNodeInfo {
        string  name;
        string  typeName;
        size_t  typeSize;
    };

    typedef das_hash_map<SimNode *,SimNodeInfo> SimNodeInfoLookup;

    struct FusionPoint {
        virtual SimNode * fuse ( const SimNodeInfoLookup &, SimNode * node, Context * ) const { return node; }
        static bool is ( const SimNodeInfoLookup & info, SimNode * node, const char * name );
        static bool is2 ( const SimNodeInfoLookup & info, SimNode * lnode, SimNode * rnode, const char * lname, const char * rname );
        static bool is ( const SimNodeInfoLookup & info, SimNode * node, const char * name, const char * typeName );
    protected:
        ~FusionPoint() = default;
    };

    const char * getSimSourceName(SimSourceType st);

    struct SimNode_Op1Fusion : SimNode_WithErrorMessage {
        SimNode_Op1Fusion(const char * msg = nullptr) : SimNode_WithErrorMessage(LineInfo(),msg) {}
        void set(const char * opn, Type bt, const LineInfo & at) {
            op = opn;
            baseType = bt;
            debugInfo = at;
        }
        virtual SimNode * visit(SimVisitor & vis) override;
        const char *    op = nullptr;
        Type            baseType;
        SimSource       subexpr;
    };

    struct SimNode_Op2Fusion : SimNode_WithErrorMessage {
        SimNode_Op2Fusion(const char * msg = nullptr) : SimNode_WithErrorMessage(LineInfo(),msg) {}
        void set(const char * opn, Type bt, const LineInfo & at) {
            op = opn;
            baseType = bt;
            debugInfo = at;
        }
        virtual SimNode * visit(SimVisitor & vis) override;
        const char *    op = nullptr;
        Type            baseType;
        SimSource       l, r;
    };

    struct FusionPointOp1 : FusionPoint {
        virtual SimNode * match(const SimNodeInfoLookup &, SimNode *, SimNode *, Context *) const = 0;
        virtual void set(SimNode_Op1Fusion * result, SimNode * node) const = 0;
        SimNode * fuseOp1(const SimNodeInfoLookup & info, SimNode * node, SimNode * node_x, Context * context) const;
    protected:
        ~FusionPointOp1() = default;
    };

    struct FusionPointOp2 : FusionPoint {
        virtual SimNode * match(const SimNodeInfoLookup &, SimNode *, SimNode *, SimNode *, Context *, bool & anyLeft, bool & anyRight) const = 0;
        virtual void set(SimNode_Op2Fusion * result, SimNode * node) const = 0;
        SimNode * fuseOp2(const SimNodeInfoLookup & info, SimNode * node, SimNode * node_l, SimNode * node_r, Context * context) const;
    protected:
        ~FusionPointOp2() = default;
    };

    struct FusionEntry {
        const char *        name;
        const char *        typeName;
        const FusionPoint * point;
    };

    struct FusionSlot {
        uint32_t hash;
        uint32_t entry;
    };

    struct FusionTable {
        uint64_t            filter[16];
        const FusionEntry * entries;
        const FusionSlot *  slots;
        uint32_t            mask;
    };

    constexpr uint32_t fusionFilterBit ( uint32_t hash ) {
        return (hash >> 16) & 1023;
    }

    constexpr uint32_t fusionHash ( const char * name, const char * typeName ) {
        uint32_t hash = 2166136261u;
        for ( ; *name; ++name ) hash = (hash ^ uint8_t(*name)) * 16777619u;
        hash = (hash ^ uint8_t('<')) * 16777619u;
        for ( ; *typeName; ++typeName ) hash = (hash ^ uint8_t(*typeName)) * 16777619u;
        return hash;
    }

    constexpr uint32_t fusionIndexSize ( size_t count ) {
        uint32_t size = 1;
        while ( size < count * 4 ) size <<= 1;
        return size;
    }

    template <size_t N>
    struct FusionIndex {
        static constexpr uint32_t size = fusionIndexSize(N);
        FusionSlot slots[size] = {};
        uint64_t filter[16] = {};
    };

    template <size_t N>
    constexpr FusionIndex<N> makeFusionIndex ( const FusionEntry (&entries)[N] ) {
        FusionIndex<N> index;
        for ( size_t i = 0; i != N; ++i ) {
            uint32_t hash = fusionHash(entries[i].name, entries[i].typeName);
            uint32_t slot = hash & (index.size - 1);
            while ( index.slots[slot].entry ) slot = (slot + 1) & (index.size - 1);
            index.slots[slot] = { hash, uint32_t(i + 1) };
            uint32_t bit = fusionFilterBit(hash);
            index.filter[bit >> 6] |= uint64_t(1) << (bit & 63);
        }
        return index;
    }

    template <size_t N>
    constexpr FusionTable makeFusionTable ( const FusionIndex<N> & index, const FusionEntry (&entries)[N] ) {
        FusionTable table = {};
        for ( int i = 0; i != 16; ++i ) table.filter[i] = index.filter[i];
        table.entries = entries;
        table.slots = index.slots;
        table.mask = index.size - 1;
        return table;
    }

    template <typename FP>
    constexpr FP fusionPoint {};

#define FUSION_ENTRY(NAME,TYPENAME,FP) \
    { NAME, TYPENAME, &fusionPoint<FP> },

#define FUSION_TABLE(TABLE,ENTRIES) \
    static constexpr auto TABLE##_index = makeFusionIndex(ENTRIES); \
    extern const FusionTable TABLE; \
    const FusionTable TABLE = makeFusionTable(TABLE##_index, ENTRIES);
}

extern DAS_CC_API void register_fusion ();
