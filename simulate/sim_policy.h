#pragma once

namespace  yzg {
    
    class Context;
    
    // POLICY BASED OPERATIONS
    
    template <typename TT>
    struct SimPolicy_CoreType {
        static __forceinline bool Equ     ( TT a, TT b, Context & ) { return a == b;  }
        static __forceinline bool NotEqu  ( TT a, TT b, Context & ) { return a != b;  }
    };
    
    template <typename TT>
    struct SimPolicy_GroupByAdd : SimPolicy_CoreType<TT> {
        static __forceinline TT Unp ( TT x, Context & ) { return x; }
        static __forceinline TT Unm ( TT x, Context & ) { return -x; }
        static __forceinline TT Add ( TT a, TT b, Context & ) { return a + b; }
        static __forceinline TT Sub ( TT a, TT b, Context & ) { return a - b; }
        static __forceinline TT & SetAdd  ( TT & a, TT b, Context & ) { return a += b; }
        static __forceinline TT & SetSub  ( TT & a, TT b, Context & ) { return a -= b; }
    };
    
    template <typename TT>
    struct SimPolicy_Ordered {
        // ordered
        static __forceinline bool LessEqu ( TT a, TT b, Context & ) { return a <= b;  }
        static __forceinline bool GtEqu   ( TT a, TT b, Context & ) { return a >= b;  }
        static __forceinline bool Less    ( TT a, TT b, Context & ) { return a < b;  }
        static __forceinline bool Gt      ( TT a, TT b, Context & ) { return a > b;  }
    };
    
    template <typename TT>
    struct SimPolicy_Type : SimPolicy_GroupByAdd<TT>, SimPolicy_Ordered<TT> {
        // numeric
        static __forceinline void Inc ( TT & x, Context & ) { ++x; }
        static __forceinline void Dec ( TT & x, Context & ) { --x; }
        static __forceinline TT IncPost ( TT & x, Context & ) { return x++; }
        static __forceinline TT DecPost ( TT & x, Context & ) { return x--; }
        static __forceinline TT Div ( TT a, TT b, Context & ) { return a / b; }
        static __forceinline TT Mul ( TT a, TT b, Context & ) { return a * b; }
        static __forceinline TT & SetDiv  ( TT & a, TT b, Context & ) { return a /= b; }
        static __forceinline TT & SetMul  ( TT & a, TT b, Context & ) { return a *= b; }
    };
    
    struct SimPolicy_Bool : SimPolicy_CoreType<bool> {
        static __forceinline bool BoolNot ( bool x, Context & ) { return !x; }
        static __forceinline bool BoolAnd ( bool a, bool b, Context & ) { return a && b; }
        static __forceinline bool BoolOr  ( bool a, bool b, Context & ) { return a || b; }
        static __forceinline bool BoolXor ( bool a, bool b, Context & ) { return a ^ b; }
        static __forceinline bool & SetBoolAnd  ( bool & a, bool b, Context & ) { return a &= b; }
        static __forceinline bool & SetBoolOr   ( bool & a, bool b, Context & ) { return a |= b; }
        static __forceinline bool & SetBoolXor  ( bool & a, bool b, Context & ) { return a ^= b; }
    };
    
    template <typename TT>
    struct SimPolicy_Bin : SimPolicy_Type<TT> {
        static __forceinline TT Mod ( TT a, TT b, Context & ) { return a % b; }
        static __forceinline TT BinNot ( TT x, Context & ) { return ~x; }
        static __forceinline TT BinAnd ( TT a, TT b, Context & ) { return a & b; }
        static __forceinline TT BinOr  ( TT a, TT b, Context & ) { return a | b; }
        static __forceinline TT BinXor ( TT a, TT b, Context & ) { return a ^ b; }
        static __forceinline TT & SetBinAnd ( TT & a, TT b, Context & ) { return a &= b; }
        static __forceinline TT & SetBinOr  ( TT & a, TT b, Context & ) { return a |= b; }
        static __forceinline TT & SetBinXor ( TT & a, TT b, Context & ) { return a ^= b; }
        static __forceinline TT & SetMod  ( TT & a, TT b, Context & ) { return a %= b; }
    };
    
    struct SimPolicy_Int : SimPolicy_Bin<int32_t> {};
    struct SimPolicy_UInt : SimPolicy_Bin<uint32_t> {};
    struct SimPolicy_Int64 : SimPolicy_Bin<int64_t> {};
    struct SimPolicy_UInt64 : SimPolicy_Bin<uint64_t> {};
    struct SimPolicy_Float : SimPolicy_Type<float> {
        static __forceinline float Mod ( float a, float b, Context & ) { return fmod(a,b); }
        static __forceinline float & SetMod ( float & a, float b, Context & ) { return a = fmod(a,b); }
    };
    struct SimPolicy_Pointer : SimPolicy_CoreType<void *> {};
    
    template <typename TT>
    struct SimPolicy;
    
    template <> struct SimPolicy<bool> : SimPolicy_Bool {};
    template <> struct SimPolicy<int32_t> : SimPolicy_Bin<int32_t> {};
    template <> struct SimPolicy<uint32_t> : SimPolicy_Bin<uint32_t> {};
    template <> struct SimPolicy<int64_t> : SimPolicy_Bin<int64_t> {};
    template <> struct SimPolicy<uint64_t> : SimPolicy_Bin<uint64_t> {};
    template <> struct SimPolicy<float> : SimPolicy_Float {};
    template <> struct SimPolicy<void *> : SimPolicy_Pointer {};
}
