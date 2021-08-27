#pragma once

#include "daScript/misc/callable.h"
#include "daScript/simulate/runtime_profile.h"
#include "daScript/simulate/debug_print.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/aot_builtin.h"
#include "daScript/simulate/aot_builtin_matrix.h"
#include "daScript/simulate/aot_builtin_time.h"
#include "daScript/simulate/bin_serializer.h"
#include "daScript/simulate/runtime_table.h"
#include "daScript/simulate/interop.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4946)   // reinterpret_cast used between related classes
#endif
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#if __GNUC__>=9
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif
#pragma GCC diagnostic ignored "-Wsizeof-pointer-memaccess"
#endif

namespace das {

    #define DAS_SETBOOLOR(a,b)  (([&]()->bool{ bool & A=((a)); A=A||((b)); return A; })())
    #define DAS_SETBOOLAND(a,b) (([&]()->bool{ bool & A=((a)); A=A&&((b)); return A; })())
    #define DAS_SETBOOLXOR(a,b) (([&]()->bool{ bool & A=((a)); A=A^((b)); return A; })())

    #define DAS_MAKE_ANNOTATION(name)   ((TypeAnnotation*)(intptr_t(name)|1))

    #define DAS_COMMENT(...)

    #define DAS_STRINGIFY(x) #x
    #define DAS_TOSTRING(x) DAS_STRINGIFY(x)
    #define DAS_FILE_LINE_NAME(a,b) a " at line " DAS_TOSTRING(b)
    #define DAS_FILE_LINE   DAS_FILE_LINE_NAME(__FILE__,__LINE__)

    void das_debug ( Context * context, TypeInfo * typeInfo, const char * FILE, int LINE, vec4f res, const char * message = nullptr );

    template <typename TT>
    struct das_auto_cast {
        template <typename QQ>
        __forceinline static TT cast ( QQ && expr ) {
            return (TT) expr;
        }
    };

    template <typename TT>
    struct das_auto_cast_move {
        template <typename QQ>
        __forceinline static TT cast ( QQ && expr ) {
            TT res = expr;
            memset(&expr, 0, sizeof(QQ));
            return res;
        }
    };

    template <typename TT>
    __forceinline void das_zero ( TT & a ) {
        using TTNC = typename remove_const<TT>::type;
        memset(const_cast<TTNC *>(&a), 0, sizeof(TT));
    }

    template <typename TT, typename QQ>
    __forceinline void das_move ( TT & a, const QQ & b ) {
        using TTNC = typename remove_const<TT>::type;
        static_assert(sizeof(TT)<=sizeof(QQ),"can't move from smaller type");
        memcpy(const_cast<TTNC *>(&a), &b, sizeof(TT));
        memset((TTNC *)&b, 0, sizeof(TT));
    }

    template <typename TT, typename QQ>
    __forceinline void das_copy ( TT & a, const QQ b ) {
        using TTNC = typename remove_const<TT>::type;
        static_assert(sizeof(TT)<=sizeof(QQ),"can't copy from smaller type");
        memcpy(const_cast<TTNC *>(&a), &b, sizeof(TT));
    }

    template <typename TT>
    __forceinline bool das_equ ( const TT & a, const TT & b ) {
        return a == b;
    }

    template <typename TT>
    __forceinline bool das_nequ ( const TT & a, const TT & b ) {
        return a != b;
    }

    template <typename TT>
    __forceinline bool das_equ_val ( TT a, TT b ) {
        return a == b;
    }

    template <typename TT>
    __forceinline bool das_nequ_val ( TT a, TT b ) {
        return a != b;
    }

    template <typename TT, uint32_t mnh>
    __forceinline TT & das_global ( Context * __context__ ) {
        uint32_t offset =  __context__->globalOffsetByMangledName(mnh);
        return *(TT *)(__context__->globals + offset);
    }

    template <typename TT, uint32_t mnh>
    __forceinline void das_global_zero ( Context * __context__ ) {
        uint32_t offset =  __context__->globalOffsetByMangledName(mnh);
        memset(__context__->globals + offset, 0, sizeof(TT));
    }

    template <typename TT, uint32_t mnh>
    __forceinline TT & das_shared ( Context * __context__ ) {
        uint32_t offset =  __context__->globalOffsetByMangledName(mnh);
        return *(TT *)(__context__->shared + offset);
    }

    template <typename TT, uint32_t mnh>
    __forceinline void das_shared_zero ( Context * __context__ ) {
        uint32_t offset =  __context__->globalOffsetByMangledName(mnh);
        memset(__context__->shared + offset, 0, sizeof(TT));
    }

    __forceinline int das_memcmp ( void * left, void * right, int size ) {
        return memcmp(left, right, size);
    }

    __forceinline void das_memcpy ( void * left, void * right, int size ) {
        memcpy(left, right, size);
    }

    template <typename TT>
    __forceinline void das_ptr_inc ( TT * & ptr, int ) {
        ++ ptr;
    }

    template <typename TT>
    __forceinline void das_ptr_dec ( TT * & ptr, int ) {
        -- ptr;
    }

    template <typename TT>
    __forceinline TT * das_ptr_add ( TT * ptr, int value, int ) {
        return ptr + value;
    }

    template <typename TT>
    __forceinline TT * das_ptr_sub ( TT * & ptr, int value, int ) {
        return ptr - value;
    }

    template <typename TT>
    __forceinline void das_ptr_set_add ( TT * & ptr, int value, int ) {
        ptr += value;
    }

    template <typename TT>
    __forceinline void das_ptr_set_sub ( TT * & ptr, int value, int ) {
        ptr -= value;
    }

    __forceinline int64_t i_das_ptr_diff ( void * a, void *b, int stride ) {
        return int64_t((char *)a - (char *)b) / int64_t(stride);
    }

    template <typename TT>
    struct das_arg {
        static __forceinline TT & pass ( TT && a ) {
            return *((TT *)&a);
        }
        static __forceinline TT & pass ( TT & a ) {
            return a;
        }
    };

    template <typename TT>
    struct das_reinterpret {
        template <typename QQ>
        static __forceinline TT & pass ( QQ && a ) {
            return *((TT *)&a);
        }
        template <typename QQ>
        static __forceinline TT & pass ( QQ & a ) {
            return *((TT *)&a);
        }
        template <typename QQ>
        static __forceinline const TT & pass ( const QQ & a ) {
            return *((const TT *)&a);
        }
    };

    template <typename TT>
    struct das_reinterpret<const TT> : das_reinterpret<TT> {};

    template <>
    struct das_reinterpret<vec4f>;

    template <>
    struct das_reinterpret<const vec4f>;

    template <typename TT>
    struct das_typedecl_value {
        TT operator () () const {
            return TT();
        };
    };

    template <typename ResT,typename VecT, int index>
    struct das_swizzle_ref {
        static __forceinline ResT & swizzle ( VecT & val ) {
            return  *((ResT *)((&val.x) + index));
        }
        static __forceinline ResT & swizzle ( vec4f & valV ) {
            VecT & val = *((VecT *)&valV);
            return  *((ResT *)((&val.x) + index));
        }
    };

    template <typename ResT,typename VecT, int index>
    struct das_swizzle_seq {
        static __forceinline const ResT & swizzle ( const VecT & val ) {
            return  *((const ResT *)((&val.x) + index));
        }
    };

    template <typename ResT,typename VecT, int f0, int f1, int f2 = 0, int f3 = 0>
    struct das_swizzle {
        static __forceinline ResT swizzle ( const VecT & val ) {
            ResT res;
            res.x = *((&val.x) + f0);
            res.y = *((&val.x) + f1);
            res.z = *((&val.x) + f2);
            res.w = *((&val.x) + f3);
            return res;
        }
        static __forceinline ResT swizzle ( vec4f valT ) {
            return swizzle(cast<VecT>::to(valT));
        }
    };

    template <typename ResT,typename VecT, int f0, int f1, int f2>
    struct das_swizzle<ResT,VecT,f0,f1,f2,0> {
        static __forceinline ResT swizzle ( const VecT & val ) {
            ResT res;
            res.x = *((&val.x) + f0);
            res.y = *((&val.x) + f1);
            res.z = *((&val.x) + f2);
            return res;
        }
        static __forceinline ResT swizzle ( vec4f valT ) {
            return swizzle(cast<VecT>::to(valT));
        }
    };

    template <typename ResT,typename VecT, int f0, int f1>
    struct das_swizzle<ResT,VecT,f0,f1,0,0> {
        static __forceinline ResT swizzle ( const VecT & val ) {
            ResT res;
            res.x = *((&val.x) + f0);
            res.y = *((&val.x) + f1);
            return res;
        }
        static __forceinline ResT swizzle ( vec4f valT ) {
            return swizzle(cast<VecT>::to(valT));
        }
    };

    __forceinline bool das_get_bitfield ( uint32_t value, uint32_t mask ) {
        return (value & mask) == mask;
    }

    template <typename TT>
    struct cast_vec_ref {
        static __forceinline TT & to(vec4f & x) {
            return *(TT *)(&x);
        }
        static __forceinline TT & to(TT & x) {
            return x;
        }
        static __forceinline const TT & to(const vec4f & x) {
            return *(TT *)(&x);
        }
        static __forceinline const TT & to(const TT & x) {
            return x;
        }
    };

    template <typename TT>
    struct das_cast {
        template <typename QQ>
        static __forceinline TT & cast ( const QQ & expr ) {
            return reinterpret_cast<TT&>(const_cast<QQ &>(expr));
        }
    };

    template <typename TT>
    struct das_cast<TT *> {
        template <typename QQ>
        static __forceinline TT * cast ( QQ * expr ) {
            return reinterpret_cast<TT *>(expr);
        }
        static __forceinline TT * cast ( nullptr_t ) {
            return nullptr;
        }
        template <typename QQ>
        static __forceinline TT * cast ( const QQ * expr ) {
            return const_cast<TT *>(reinterpret_cast<const TT *>(expr));
        }
        template <typename QQ>
        static __forceinline TT * cast ( const smart_ptr_raw<QQ> & expr ) {
            return reinterpret_cast<TT *>(expr.get());
        }
        template <typename QQ>
        static __forceinline TT * cast ( const smart_ptr<QQ> & expr ) {
            return reinterpret_cast<TT *>(expr.get());
        }
        template <typename QQ>
        static __forceinline TT * cast ( const QQ & expr ) {
            return const_cast<TT *>(reinterpret_cast<const TT *>(&expr));
        }
    };

    template <typename TT>
    struct das_upcast {
        template <typename QQ>
        static __forceinline TT & cast ( const QQ & expr ) {
            return reinterpret_cast<TT &>(const_cast<QQ &>(expr));
        }
    };

    template <typename TT>
    struct das_upcast<TT *> {
        template <typename QQ>
        static __forceinline TT * cast ( QQ * expr ) {
            return static_cast<TT *>(expr);
        }
    };

    template <typename TT>
    struct das_null_coalescing {
        static __forceinline TT get ( const TT * ptr, TT value ) {
            return ptr ? *((TT *)ptr) : value;
        }
        static __forceinline TT get ( const smart_ptr_raw<TT> & sp, TT value ) {
            TT * ptr = sp.get();
            return ptr ? *((TT *)ptr) : value;
        }
    };

    template <typename TT>
    struct das_null_coalescing<TT &> {
        static __forceinline TT & get ( const TT * ptr, TT & value ) {
            return ptr ? *((TT *)ptr) : value;
        }
        static __forceinline TT get ( const smart_ptr_raw<TT> & sp, TT & value ) {
            TT * ptr = sp.get();
            return ptr ? *((TT *)ptr) : value;
        }
    };

    template <typename TT>
    struct das_null_coalescing<const TT &> {
        static __forceinline const TT & get ( const TT * ptr, const TT & value ) {
            return ptr ? *ptr : value;
        }
        static __forceinline TT get ( const smart_ptr_raw<TT> & sp, const TT & value ) {
            TT * ptr = sp.get();
            return ptr ? *ptr : value;
        }
    };

    template <typename TT, typename RR, RR TT::*Member>
    struct das_safe_navigation {
        static __forceinline RR * get ( const TT * ptr ) {
            if ( ptr ) {
                return (RR *) &(ptr->*Member);
            } else {
                return nullptr;
            }
        }
        static __forceinline RR * get ( const smart_ptr_raw<TT> & sp ) {
            TT * ptr = sp.get();
            if ( ptr ) {
                return (RR *) &(ptr->*Member);
            } else {
                return nullptr;
            }
        }
    };

    template <typename TT, typename RR, RR TT::*Member>
    struct das_safe_navigation_ptr {
        static __forceinline RR get ( const TT * ptr ) {
            if ( ptr ) {
                return (RR)(ptr->*Member);
            } else {
                return nullptr;
            }
        }
        static __forceinline RR get ( const smart_ptr_raw<TT> & sp ) {
            TT * ptr = sp.get();
            if ( ptr ) {
                return (RR)(ptr->*Member);
            } else {
                return nullptr;
            }
        }
    };

    template <typename TT, typename RR>
    struct das_safe_navigation_handle {
        template <typename QQ>
        static __forceinline RR * get ( const TT * ptr, QQ && nav ) {
            if ( ptr ) {
                return (RR *) & nav(ptr);
            } else {
                return nullptr;
            }
        }
        template <typename QQ>
        static __forceinline RR * get ( const smart_ptr_raw<TT> & sp, QQ && nav ) {
            TT * ptr = sp.get();
            if ( ptr ) {
                return (RR *) & nav(ptr);
            } else {
                return nullptr;
            }
        }
    };

    template <typename TT, typename RR>
    struct das_safe_navigation_handle_ptr {
        template <typename QQ>
        static __forceinline RR get ( const TT * ptr, QQ && nav ) {
            if ( ptr ) {
                return (RR) nav(ptr);
            } else {
                return nullptr;
            }
        }
        template <typename QQ>
        static __forceinline RR get ( const smart_ptr_raw<TT> & sp, QQ && nav ) {
            TT * ptr = sp.get();
            if ( ptr ) {
                return (RR) nav(ptr);
            } else {
                return nullptr;
            }
        }
    };

    template <typename TT>
    struct das_index;

    template <typename TT>
    struct das_default_vector_size;

    template <typename TT>
    struct das_default_vector_size<vector<TT>> {
        static __forceinline uint32_t size( const vector<TT> & value ) {
            return uint32_t(value.size());
        }
    };

    template <typename TT, typename OT>
    struct das_default_vector_index {
        using SIZE_POLICY = das_default_vector_size<TT>;
        static __forceinline OT & at ( TT & value, int32_t index, Context * __context__ ) {
            uint32_t size = SIZE_POLICY::size(value);
            uint32_t idx = uint32_t(index);
            if ( idx>=size ) __context__->throw_error_ex("vector index out of range, %u of %u", idx, size);
            return value[index];
        }
        static __forceinline const OT & at ( const TT & value, int32_t index, Context * __context__ ) {
            uint32_t size = SIZE_POLICY::size(value);
            uint32_t idx = uint32_t(index);
            if ( idx>=size ) __context__->throw_error_ex("vector index out of range, %u of %u", idx, size);
            return value[index];
        }
        static __forceinline OT & at ( TT & value, uint32_t idx, Context * __context__ ) {
            uint32_t size = SIZE_POLICY::size(value);
            if ( idx>=size ) __context__->throw_error_ex("vector index out of range, %u of %u", idx, size);
            return value[idx];
        }
        static __forceinline const OT & at ( const TT & value, uint32_t idx, Context * __context__ ) {
            uint32_t size = SIZE_POLICY::size(value);
            if ( idx>=size ) __context__->throw_error_ex("vector index out of range, %u of %u", idx, size);
            return value[idx];
        }
    };

    template <typename TT>
    struct das_index<vector<TT>> : das_default_vector_index<vector<TT>, TT> {};

    template <typename TT>
    struct das_index<vector<TT> const> : das_default_vector_index<vector<TT>, TT> {};


    template <typename TT, typename VecT, uint32_t size>
    struct das_vec_index {
    // index
        static __forceinline TT & at ( VecT & value, int32_t index, Context * __context__ ) {
            uint32_t idx = uint32_t(index);
            if ( idx>=size ) __context__->throw_error_ex("vector index out of range, %u of %u", idx, size);
            return (&value.x)[idx];
        }
        static __forceinline const TT & at ( const VecT & value, int32_t index, Context * __context__ ) {
            uint32_t idx = uint32_t(index);
            if ( idx>=size ) __context__->throw_error_ex("vector index out of range, %u of %u", idx, size);
            return (&value.x)[idx];
        }
        static __forceinline TT & at ( VecT & value, uint32_t idx, Context * __context__ ) {
            if ( idx>=size ) __context__->throw_error_ex("vector index out of range, %u of %u", idx, size);
            return (&value.x)[idx];
        }
        static __forceinline const TT & at ( const VecT & value, uint32_t idx, Context * __context__ ) {
            if ( idx>=size ) __context__->throw_error_ex("vector index out of range, %u of %u", idx, size);
            return (&value.x)[idx];
        }
    // safe index
        static __forceinline TT * safe_at ( VecT * value, int32_t index, Context * ) {
            if (!value) return nullptr;
            uint32_t idx = uint32_t(index);
            if (idx >= size) return nullptr;
            return (&value->x) + idx;
        }
        static __forceinline const TT * safe_at ( const VecT * value, int32_t index, Context * ) {
            if (!value) return nullptr;
            uint32_t idx = uint32_t(index);
            if ( idx>=size ) return nullptr;
            return (&value->x) + idx;
        }
        static __forceinline TT * safe_at ( VecT * value, uint32_t idx, Context * ) {
            if (!value) return nullptr;
            if ( idx>=size ) return nullptr;
            return (&value->x) + idx;
        }
        static __forceinline const TT * safe_at ( const VecT * value, uint32_t idx, Context * ) {
            if (!value) return nullptr;
            if ( idx>=size ) return nullptr;
            return (&value->x) + idx;
        }
    };

    template <typename TT> struct das_index<vec2<TT>> : das_vec_index<TT, vec2<TT>, 2> {};
    template <typename TT> struct das_index<vec3<TT>> : das_vec_index<TT, vec3<TT>, 3> {};
    template <typename TT> struct das_index<vec4<TT>> : das_vec_index<TT, vec4<TT>, 4> {};

    template <typename TT> struct das_index<const vec2<TT>> : das_vec_index<TT, vec2<TT>, 2> {};
    template <typename TT> struct das_index<const vec3<TT>> : das_vec_index<TT, vec3<TT>, 3> {};
    template <typename TT> struct das_index<const vec4<TT>> : das_vec_index<TT, vec4<TT>, 4> {};

    template<> struct das_index<range> : das_vec_index<int32_t, range, 2> {};
    template<> struct das_index<const range> : das_vec_index<int32_t, range, 2> {};

    template<> struct das_index<urange> : das_vec_index<uint32_t, urange, 2> {};
    template<> struct das_index<const urange> : das_vec_index<uint32_t, urange, 2> {};

    template <typename VecT, int size>
    struct das_index<Matrix<VecT,size>> {
        using MatT = Matrix<VecT,size>;
        static __forceinline VecT & at ( MatT & value, int32_t index, Context * __context__ ) {
            uint32_t idx = uint32_t(index);
            if ( idx>=uint32_t(size) ) __context__->throw_error_ex("matrix index out of range, %u of %u", idx, size);
            return value.m[idx];
        }
        static __forceinline VecT & at ( MatT & value, uint32_t idx, Context * __context__ ) {
            if ( idx>=uint32_t(size) ) __context__->throw_error_ex("matrix index out of range, %u of %u", idx, size);
            return value.m[idx];
        }
    };

    template <typename VecT, int size>
    struct das_index<const Matrix<VecT,size>> {
        using MatT = Matrix<VecT,size>;
        static __forceinline const VecT & at ( const MatT & value, int32_t index, Context * __context__ ) {
            uint32_t idx = uint32_t(index);
            if ( idx>=uint32_t(size) ) __context__->throw_error_ex("matrix index out of range, %u of %u", idx, size);
            return value.m[idx];
        }
        static __forceinline const VecT & at ( const MatT & value, uint32_t idx, Context * __context__ ) {
            if ( idx>=uint32_t(size) ) __context__->throw_error_ex("matrix index out of range, %u of %u", idx, size);
            return value.m[idx];
        }
    };

    template <typename TT>
    struct das_index<TT * const> {
        static __forceinline TT & at ( TT * value, int32_t index, Context * ) {
            return value[index];
        }
        static __forceinline TT & at ( TT * value, uint32_t index, Context * ) {
            return value[index];
        }
    };

    template <typename TT>
    struct das_index<const TT * const> {
        static __forceinline const TT & at ( const TT * value, int32_t index, Context * ) {
            return value[index];
        }
        static __forceinline const TT & at ( const TT * value, uint32_t index, Context * ) {
            return value[index];
        }
    };

    template <typename TT>
    struct das_index<TT *> {
        static __forceinline TT & at ( TT * value, int32_t index, Context * ) {
            return value[index];
        }
        static __forceinline TT & at ( TT * value, uint32_t index, Context * ) {
            return value[index];
        }
    };

    template <typename TT, int size>
    struct TDim {
        using THIS_TYPE = TDim<TT, size>;
        enum { capacity = size };
        TT  data[size];
        TDim() {}
        __forceinline TT & operator [] ( int32_t index ) {
            return data[index];
        }
        __forceinline const TT & operator [] ( int32_t index ) const {
            return data[index];
        }
    // index
        __forceinline TT & operator () ( int32_t index, Context * __context__ ) {
            uint32_t idx = uint32_t(index);
            if ( idx>=uint32_t(size) ) __context__->throw_error_ex("index out of range, %u of %u", idx, size);
            return data[index];
        }
        __forceinline const TT & operator () ( int32_t index, Context * __context__ ) const {
            uint32_t idx = uint32_t(index);
            if ( idx>=uint32_t(size) ) __context__->throw_error_ex("index out of range, %u of %u", idx, size);
            return data[index];
        }
        __forceinline TT & operator () ( uint32_t idx, Context * __context__ ) {
            if ( idx>=uint32_t(size) ) __context__->throw_error_ex("index out of range, %u of %u", idx, size);
            return data[idx];
        }
        __forceinline const TT & operator () ( uint32_t idx, Context * __context__ ) const {
            if ( idx>=uint32_t(size) ) __context__->throw_error_ex("index out of range, %u of %u", idx, size);
            return data[idx];
        }
    // safe index
        static __forceinline TT * safe_index ( THIS_TYPE * that, int32_t index, Context * ) {
            if (!that) return nullptr;
            uint32_t idx = uint32_t(index);
            if (idx >= uint32_t(size)) return nullptr;
            return that->data + index;
        }
        static __forceinline const TT * safe_index ( const THIS_TYPE * that, int32_t index, Context * ) {
            if (!that) return nullptr;
            uint32_t idx = uint32_t(index);
            if ( idx>=uint32_t(size) ) return nullptr;
            return that->data + index;
        }
        static __forceinline TT * safe_index ( THIS_TYPE * that, uint32_t idx, Context * ) {
            if (!that) return nullptr;
            if ( idx>=uint32_t(size) ) return nullptr;
            return that->data + idx;
        }
        static __forceinline const TT * safe_index ( const THIS_TYPE * that, uint32_t idx, Context * ) {
            if (!that) return nullptr;
            if ( idx>=uint32_t(size) ) return nullptr;
            return that->data + idx;
        }
    };

    __forceinline bool is_char_in_set ( int32_t ch, const TDim<uint32_t,8> & bitset ) {
        return bitset[ch>>5] & (1u<<uint32_t(ch));
    }

    template <typename TT, int size>
    struct das_cast< TDim<TT,size> > {
        template <typename QQ>
        static __forceinline TDim<TT,size> & cast ( const QQ * expr ) {
            return *((TDim<TT,size> *)expr);
        }
    };

    template <typename TT, int size>
    struct das_cast< const TDim<TT,size> > {
        template <typename QQ>
        static __forceinline const TDim<TT,size> & cast ( const QQ * expr ) {
            return *((const TDim<TT,size> *)expr);
        }
    };

    template <typename TT>
    struct TArray : Array {
        using THIS_TYPE = TArray<TT>;
        enum { stride = sizeof(TT) };
        TArray()  {}
        TArray(TArray & arr) { moveA(arr); }
        TArray(TArray && arr ) { moveA(arr); }
        TArray & operator = ( TArray & arr ) { moveA(arr); return *this; }
        TArray & operator = ( TArray && arr ) { moveA(arr); return *this; }
        __forceinline void moveA ( Array & arr ) {
            data = arr.data; arr.data = 0;
            size = arr.size; arr.size = 0;
            capacity = arr.capacity; arr.capacity = 0;
            lock = arr.lock; arr.lock = 0;
            flags = arr.flags; arr.flags = 0;
        }
        __forceinline TT & operator [] ( int32_t index ) {
            return ((TT *)data)[index];
        }
        __forceinline const TT & operator [] ( int32_t index ) const {
            return ((TT *)data)[index];
        }
    // index
        __forceinline TT & operator () ( int32_t index, Context * __context__ ) {
            uint32_t idx = uint32_t(index);
            if ( idx>=size ) __context__->throw_error_ex("array index out of range, %u of %u", idx, size);
            return ((TT *)data)[index];
        }
        __forceinline const TT & operator () ( int32_t index, Context * __context__ ) const {
            uint32_t idx = uint32_t(index);
            if ( idx>=size ) __context__->throw_error_ex("array index out of range, %u of %u", idx, size);
            return ((const TT *)data)[index];
        }
        __forceinline TT & operator () ( uint32_t idx, Context * __context__ ) {
            if ( idx>=size ) __context__->throw_error_ex("array index out of range, %u of %u", idx, size);
            return ((TT *)data)[idx];
        }
        __forceinline const TT & operator () ( uint32_t idx, Context * __context__ ) const {
            if ( idx>=size ) __context__->throw_error_ex("array index out of range, %u of %u", idx, size);
            return ((const TT *)data)[idx];
        }
    // safe index
        static __forceinline TT * safe_index ( THIS_TYPE * that, int32_t index, Context * ) {
            if (!that) return nullptr;
            uint32_t idx = uint32_t(index);
            if (idx >= that->size) return nullptr;
            return ((TT *)that->data) + index;
        }
        static __forceinline const TT  * safe_index ( const THIS_TYPE * that, int32_t index, Context * ) {
            if (!that) return nullptr;
            uint32_t idx = uint32_t(index);
            if ( idx>=that->size ) return nullptr;
            return ((const TT *)that->data) + index;
        }
        static __forceinline TT * safe_index ( THIS_TYPE * that, uint32_t idx, Context * ) {
            if (!that) return nullptr;
            if ( idx>=that->size ) return nullptr;
            return ((TT *)that->data) + idx;
        }
        static __forceinline const TT  * safe_index ( const THIS_TYPE * that, uint32_t idx, Context * ) {
            if (!that) return nullptr;
            if ( idx>=that->size ) return nullptr;
            return ((const TT *)that->data) + idx;
        }
    };

    template <typename TT>
    struct is_workhorse_type {
        enum {
            value =
                    is_enum<TT>::value
                ||  is_pointer<TT>::value
                ||  is_arithmetic<TT>::value
                ||  is_same<TT,bool>::value
                ||  is_same<TT,Bitfield>::value
                ||  is_same<TT,range>::value
                ||  is_same<TT,urange>::value
                ||  is_same<TT,int2>::value
                ||  is_same<TT,int3>::value
                ||  is_same<TT,int4>::value
                ||  is_same<TT,uint2>::value
                ||  is_same<TT,uint3>::value
                ||  is_same<TT,uint4>::value
                ||  is_same<TT,float2>::value
                ||  is_same<TT,float3>::value
                ||  is_same<TT,float4>::value
        };
    };

    template <typename TK, typename TV>
    struct TTable : Table {
        static_assert(is_workhorse_type<TK>::value,"only supported for `workhorse` types");
        using THIS_TYPE = TTable<TK, TV>;
        TTable()  {}
        TTable(TTable & arr) { moveT(arr); }
        TTable(TTable && arr ) { moveT(arr); }
        TTable & operator = ( TTable & arr ) { moveT(arr); return *this; }
        TTable & operator = ( TTable && arr ) { moveT(arr); return *this; }
        __forceinline void moveT ( Table & arr ) {
            data = arr.data; arr.data = 0;
            size = arr.size; arr.size = 0;
            capacity = arr.capacity; arr.capacity = 0;
            lock = arr.lock; arr.lock = 0;
            flags = arr.flags; arr.flags = 0;
            keys = arr.keys; arr.keys = 0;
            hashes = arr.hashes; arr.hashes = 0;
            maxLookups = arr.maxLookups; arr.maxLookups = 0;
            shift = arr.shift; arr.shift = 0;
        }
        __forceinline TV & operator () ( const TK & key, Context * __context__ ) {
            TableHash<TK> thh(__context__,sizeof(TV));
            auto hfn = hash_function(*__context__, key);
            int index = thh.reserve(*this, key, hfn);
            return ((TV *)data)[index];
        }
        static __forceinline TV * safe_index ( THIS_TYPE * that, const TK & key, Context * __context__ ) {
            if (!that) return nullptr;
            TableHash<TK> thh(__context__,sizeof(TV));
            auto hfn = hash_function(*__context__, key);
            int index = thh.find(*that, key, hfn);
            return index==-1 ? nullptr : ((TV *)that->data) + index;
        }
        static __forceinline const TV * safe_index ( const THIS_TYPE * that, const TK & key, Context * __context__ ) {
            if (!that) return nullptr;
            TableHash<TK> thh(__context__,sizeof(TV));
            auto hfn = hash_function(*__context__, key);
            int index = thh.find(*that, key, hfn);
            return index==-1 ? nullptr : ((const TV *)that->data) + index;
        }
    };

    template <int tupleSize, typename ...TA>
    struct TTuple : Tuple {
        TTuple() {}
        TTuple(const TTuple & arr) { moveT(arr); }
        TTuple(TTuple && arr ) { moveT(arr); }
        TTuple & operator = ( const TTuple & arr ) { moveT(arr); return *this; }
        TTuple & operator = ( TTuple && arr ) { moveT(arr); return *this; }
        __forceinline void moveT ( const Tuple & arr ) {
            memcpy ( data, &arr, tupleSize );
        }
        char data[tupleSize];
    };

    template <typename TT, int offset>
    struct das_get_tuple_field {
        static __forceinline TT & get ( const Tuple & t ) {
            char * data = (char *) &t;
            return *(TT *)(data + offset);
        }
    };

    template <typename TT, int offset>
    struct das_get_tuple_field_ptr {
        static __forceinline TT & get ( const Tuple * t ) {
            char * data = (char *) t;
            return *(TT *)(data + offset);
        }
    };

    template <typename RR, int offset>
    struct das_safe_navigation_tuple {
        static __forceinline RR * get ( const Tuple * ptr ) {
            if ( ptr ) {
                char * data = (char *) ptr;
                return (RR *)(data + offset);
            } else {
                return nullptr;
            }
        }
    };

    template <int variantSize, typename ...TA>
    struct TVariant : Variant {
        TVariant() {}
        TVariant(const TVariant & arr) { moveT(arr); }
        TVariant(TVariant && arr ) { moveT(arr); }
        TVariant & operator = ( const TVariant & arr ) { moveT(arr); return *this; }
        TVariant & operator = ( TVariant && arr ) { moveT(arr); return *this; }
        __forceinline void moveT ( const TVariant & arr ) {
            memcpy ( data, &arr, variantSize );
        }
        char data[variantSize];
    };

    template <typename TT, int offset, int variant>
    struct das_get_variant_field {
        static __forceinline TT & get ( const Variant & t ) {
            char * data = (char *) &t;
            return *(TT *)(data + offset);
        }
        static __forceinline TT & set ( const Variant & t ) {
            char * data = (char *) &t;
            *(int32_t *)data = int32_t(variant);
            return *(TT *)(data + offset);
        }
        static __forceinline TT & as ( const Variant & t, Context * __context__ ) {
            char * data = (char *) &t;
            auto cv = *(int32_t *)data;
            if ( cv != variant ) __context__->throw_error_ex("variant mismatch %i, expecting %i", variant, cv);
            return *(TT *)(data + offset);
        }
        static __forceinline TT * safe_as ( const Variant & t ) {
            char * data = (char *) &t;
            auto cv = *(int32_t *)data;
            return cv==variant ? (TT *)(data + offset) : nullptr;
        }
        static __forceinline TT * safe_as ( const Variant * t ) {
            char * data = (char *) t;
            if ( !data ) return nullptr;
            auto cv = *(int32_t *)data;
            return cv==variant ? (TT *)(data + offset) : nullptr;
        }
        static __forceinline TT safe_as_ptr ( const Variant & t ) {
            char * data = (char *) &t;
            auto cv = *(int32_t *)data;
            return cv==variant ? *(TT *)(data + offset) : nullptr;
        }
        static __forceinline TT safe_as_ptr ( const Variant * t ) {
            char * data = (char *) t;
            if ( !data ) return nullptr;
            auto cv = *(int32_t *)data;
            return cv==variant ? *(TT *)(data + offset) : nullptr;
        }
        static __forceinline bool is ( const Variant & t) {
            char * data = (char *) &t;
            auto cv = *(int32_t *)data;
            return cv == variant;
        }
    };

    template <typename TT, int offset, int variant>
    struct das_get_variant_field_ptr {
        static __forceinline TT & get ( const Variant * t ) {
            char * data = (char *) t;
            return *(TT *)(data + offset);
        }
    };

    template <typename RR, int offset, int variant>
    struct das_safe_navigation_variant {
        static __forceinline RR * get ( const Variant * ptr ) {
            if ( ptr ) {
                char * data = (char *) ptr;
                return (RR *)(data + offset);
            } else {
                return nullptr;
            }
        }
    };

    template <typename TT>
    struct TSequence : Sequence {
        __forceinline TSequence () { this->iter = nullptr; }
        __forceinline TSequence ( Sequence && s ) { this->iter = s.iter; s.iter = nullptr; }
        __forceinline TSequence ( const Sequence & s ) { this->iter = s.iter; }
        __forceinline TSequence<TT> & operator = ( Sequence && s ) { this->iter = s.iter; s.iter = nullptr;return *this; }
        __forceinline TSequence<TT> & operator = ( const Sequence & s ) { this->iter = s.iter; return *this; }
    };

    template <typename TT>
    struct das_iterator;

    template <typename TT>
    struct das_iterator <const RangeType<TT>> {
        __forceinline das_iterator(const RangeType<TT> & r) : that(r) {}
        __forceinline bool first ( Context *, TT & i ) { i = that.from; return i < that.to; }
        __forceinline bool next  ( Context *, TT & i ) { i++; return i!=that.to; }
        __forceinline void close ( Context *, TT &   ) {}
        RangeType<TT> that;
    };

    template <typename TT>
    struct das_iterator <RangeType<TT>> : das_iterator<const RangeType<TT>> {
        __forceinline das_iterator(const RangeType<TT> & r) : das_iterator<const RangeType<TT>>(r) {}
    };

    template <>
    struct das_iterator <char * const> {
        __forceinline das_iterator(const char * st) : str(st) {}
        __forceinline bool first ( Context *, int32_t & i ) {
            if ( !str || *str==0 ) return false;
            i = uint8_t(*str++);
            return true;
        }
        __forceinline bool next  ( Context *, int32_t & i ) {
            i = uint8_t(*str++);
            return i!=0;
        }
        __forceinline void close ( Context *, int32_t &   ) {}
        const char * str;
    };

    template <>
    struct das_iterator <char *> : das_iterator<char * const> {
        __forceinline das_iterator(const char * st) : das_iterator<char * const>(st) {}
    };

    template <typename TT>
    struct das_iterator<TArray<TT>> {
        __forceinline das_iterator(TArray<TT> & r) {
            that = &r;
            array_end = (TT *)(that->data + that->size * sizeof(TT));
        }
        template <typename QQ>
        __forceinline bool first(Context * __context__, QQ * & i) {
            context = __context__;
            array_lock(*__context__, *that);
            i = (QQ *)that->data;
            return i != (QQ *)array_end;
        }
        template <typename QQ>
        __forceinline bool next(Context *, QQ * & i) {
            i++;
            return i != (QQ *)array_end;
        }
        template <typename QQ>
        __forceinline void close(Context * __context__, QQ * & i) {
            array_unlock(*__context__, *that);
            context = nullptr;
            i = nullptr;
        }
        ~das_iterator() {
            TT * dummy = nullptr;
            if (context) close(context, dummy);
        }
        Array * that;
        TT * array_end;
        Context * context = nullptr;
    };

    template <typename TT>
    struct das_iterator<const TArray<TT>> {
        __forceinline das_iterator(const TArray<TT> & r) {
            that = &r;
            array_end = (TT *)(that->data + that->size*sizeof(TT));
        }
        template <typename QQ>
        __forceinline bool first ( Context * __context__, const QQ * & i ) {
            context = __context__;
            array_lock(*__context__, *(Array *)(that)); // technically we don't need for the const array, but...
            i = (const QQ *) that->data;
            return i!=(const QQ *)array_end;
        }
        template <typename QQ>
        __forceinline bool next  ( Context *, const QQ * & i ) {
            i++;
            return i!=(const QQ *)array_end;
        }
        template <typename QQ>
        __forceinline void close ( Context * __context__, const QQ * & i ) {
            array_unlock(*__context__, *(Array *)(that));  // technically we don't need for the const array, but...
            context = nullptr;
            i = nullptr;
        }
        ~das_iterator() {
            const TT * dummy = nullptr;
            if (context) close(context, dummy);
        }
        const Array * that;
        const TT * array_end;
        Context * context = nullptr;
    };

    template <typename TT, int size>
    struct das_iterator<TDim<TT,size>> {
        __forceinline das_iterator(TDim<TT,size> & r) : that(&r) {
            array_end = that->data + size;
        }
        __forceinline bool first ( Context *, TT * & i ) {
            i = (TT *) that->data;
            return i!=array_end;
        }
        __forceinline bool next  ( Context *, TT * & i ) {
            i++;
            return i!=array_end;
        }
        __forceinline void close ( Context *, TT * & i ) {
            i = nullptr;
        }
        TDim<TT,size> * that;
        TT *            array_end;
    };

    template <typename TT, int size>
    struct das_iterator<const TDim<TT,size>> {
        __forceinline das_iterator(const TDim<TT,size> & r) : that(&r) {
            array_end = that->data + size;
        }
        __forceinline bool first ( Context *, const TT * & i ) {
            i = (const TT *) that->data;
            return i!=array_end;
        }
        __forceinline bool next  ( Context *, const TT * & i ) {
            i++;
            return i!=array_end;
        }
        __forceinline void close ( Context *, const TT * & i ) {
            i = nullptr;
        }
        const TDim<TT,size> * that;
        const TT *            array_end;
    };


    template <typename TT>
    struct das_iterator<vector<TT>> {
        __forceinline das_iterator(vector<TT> & r) {
            array_start = r.data();
            array_end = array_start + r.size();
        }
        template <typename QQ>
        __forceinline bool first ( Context *, QQ * & i ) {
            i = (QQ *)array_start;
            return i!=(QQ *)array_end;
        }
        template <typename QQ>
        __forceinline bool next  ( Context *, QQ * & i ) {
            i++;
            return i!=(QQ *)array_end;
        }
        template <typename QQ>
        __forceinline void close ( Context *, QQ * & i ) {
            i = nullptr;
        }
        TT *            array_start;
        TT *            array_end;
    };

    template <typename TT>
    struct das_iterator<const vector<TT>> {
        __forceinline das_iterator(const vector<TT> & r) {
            array_start = r.data();
            array_end = array_start + r.size();
        }
        template <typename QQ>
        __forceinline bool first ( Context *, const QQ * & i ) {
            i = (const QQ *)array_start;
            return i!=(const QQ *)array_end;
        }
        template <typename QQ>
        __forceinline bool next  ( Context *, const QQ * & i ) {
            i++;
            return i!=(const QQ *)array_end;
        }
        template <typename QQ>
        __forceinline void close ( Context *, const QQ * & i ) {
            i = nullptr;
        }
        const TT *      array_start;
        const TT *      array_end;
    };

    template <typename TT>
    struct das_new {
        static __forceinline TT * make ( Context * __context__ ) {
            char * data = __context__->heap->allocate( sizeof(TT) );
            if ( !data ) __context__->throw_error("out of heap");
            memset ( data, 0, sizeof(TT) );
            return (TT *) data;
        }
        template <typename QQ>
        static __forceinline TT * make_and_init ( Context * __context__, QQ && init ) {
            TT * data = (TT *) __context__->heap->allocate( sizeof(TT) );
            if ( !data ) __context__->throw_error("out of heap");
            *data = init();
            return data;
        }
    };

    template <typename TT>
    struct das_new_persistent {
        static __forceinline TT * make ( Context * ) {
            char * data = (char *) das_aligned_alloc16(sizeof(TT));
            memset ( data, 0, sizeof(TT) );
            return (TT *) data;
        }
        template <typename QQ>
        static __forceinline TT * make_and_init ( Context *, QQ && init ) {
            TT * data = (TT *) das_aligned_alloc16(sizeof(TT));
            *data = init();
            return data;
        }
    };

    template <typename TT, bool is_smart>
    struct das_new_handle;

    template <typename TT>
    struct das_new_handle<TT,false> {
        static __forceinline TT * make ( Context * ) {
            return new TT();
        }
    };

    template <typename TT>
    struct das_new_handle<TT,true> {
        static __forceinline smart_ptr_raw<TT> make ( Context * ) {
            smart_ptr_raw<TT> p;
            p.ptr = new TT();
            p.ptr->addRef();
            return p;
        }
    };

    template <typename TT, int d>
    struct das_new_dim {
        static __forceinline TDim<TT *,d> make ( Context * __context__ ) {
            TDim<TT *,d> res;
            for ( int i=0; i!=d; ++i ) {
                res[i] = das_new<TT>::make(__context__);
            }
            return res;
        }
        template <typename QQ>
        static __forceinline TDim<TT *,d> make_and_init ( Context * __context__, QQ && init ) {
            TDim<TT *,d> res;
            for ( int i=0; i!=d; ++i ) {
                res[i] = das_new<TT>::make_and_init(__context__,init);
            }
            return res;
        }
    };

    template <typename TT, int d, bool is_smart>
    struct das_new_dim_handle;

    template <typename TT, int d>
    struct das_new_dim_handle<TT,d,false> {
        static __forceinline TDim<TT *,d> make ( Context * __context__ ) {
            TDim<TT *,d> res;
            for ( int i=0; i!=d; ++i ) {
                res[i] = das_new_handle<TT,false>::make(__context__);
            }
            return res;
        }
    };

    template <typename TT, int d>
    struct das_new_dim_handle<TT,d,true> {
        static __forceinline TDim<smart_ptr_raw<TT>,d> make ( Context * __context__ ) {
            TDim<smart_ptr_raw<TT>,d> res;
            for ( int i=0; i!=d; ++i ) {
                res[i] = das_new_handle<TT,true>::make(__context__);
            }
            return res;
        }
    };

    template <typename TT>
    struct das_delete_lambda_struct;

    template <typename TT>
    struct das_delete_lambda_struct<TT *> {
        static __forceinline void clear ( Context * __context__, TT * ptr ) {
            __context__->heap->free(((char *)ptr)-16, sizeof(TT)+16);
        }
    };

    template <typename TT>
    struct das_delete_ptr;

    template <typename TT>
    struct das_delete_ptr<TT *> {
        static __forceinline void clear ( Context * __context__, TT * ptr ) {
            __context__->heap->free((char *)ptr, sizeof(TT));
        }
    };

    template <typename TT>
    struct das_delete_handle;

    template <typename TT>
    struct das_delete_handle<TT *> {
        static __forceinline void clear ( Context *, TT * & ptr ) {
            delete ptr;
            ptr = nullptr;
        }
    };

    template <typename TT>
    struct das_delete_handle<smart_ptr_raw<TT>> {
        static __forceinline void clear ( Context *, smart_ptr_raw<TT> & p ) {
            if ( p ) {
                p.ptr->delRef();
                p.ptr = nullptr;
            }
        }
    };

    template <typename TT>
    struct das_delete_handle<smart_ptr<TT>> {
        static __forceinline void clear ( Context *, smart_ptr<TT> & p ) {
            p.reset();
        }
    };

    template <typename TT>
    struct das_delete_persistent;

    template <typename TT>
    struct das_delete_persistent<TT *> {
        static __forceinline void clear ( Context *, TT * & ptr ) {
            if ( ptr ) {
                das_aligned_free16(ptr);
                ptr = nullptr;
            }
        }
    };

    template <typename TT>
    struct das_delete;

    template <typename TT>
    struct das_delete<TT *> {
        static __forceinline void clear ( Context * __context__, TT * & ptr ) {
            das_delete_ptr<TT *>::clear(__context__,ptr);
            ptr = nullptr;
        }
    };

    template <typename TT, uint32_t size>
    struct das_delete<TDim<TT,size>> {
        static __forceinline void clear ( Context * __context__, TDim<TT,size> & dim ) {
            for ( uint32_t i=size; i!=0; --i ) {
                das_delete<TT>::clear(__context__,dim[i-1]);
            }
        }
    };

    template <typename TT>
    struct das_delete<TArray<TT>> {
        static __forceinline void clear ( Context * __context__, TArray<TT> & dim ) {
            if ( dim.data ) {
                if ( !dim.lock ) {
                    uint32_t oldSize = dim.capacity*sizeof(TT);
                    __context__->heap->free(dim.data, oldSize);
                } else {
                    __context__->throw_error("can't delete locked array");
                }
            }
            memset ( &dim, 0, sizeof(TArray<TT>) );
        }
    };

    template <typename TKey, typename TVal>
    struct das_delete<TTable<TKey,TVal>> {
        static __forceinline void clear ( Context * __context__, TTable<TKey,TVal> & tab ) {
            if ( tab.data ) {
                if ( !tab.lock ) {
                    uint32_t oldSize = tab.capacity*(sizeof(TKey)+sizeof(TVal)+sizeof(uint32_t));
                    __context__->heap->free(tab.data, oldSize);
                } else {
                    __context__->throw_error("can't delete locked table");
                }
            }
            memset ( &tab, 0, sizeof(TTable<TKey,TVal>) );
        }
    };

    template <>
    struct das_delete<Lambda> {
        static __forceinline void clear ( Context * __context__, Lambda & lambda ) {
            if ( lambda.capture ) {
                int32_t * fnIndex = (int32_t *) lambda.capture;
                SimFunction * simFunc = __context__->getFunction(fnIndex[1]-1);
                if (!simFunc) __context__->throw_error("lambda finalizer is a null function");
                vec4f argValues[1] = {
                    cast<void *>::from(lambda.capture)
                };
                __context__->call(simFunc, argValues, 0);
                lambda.capture = nullptr;
            }
        }
    };

    template <typename TT>
    struct das_final_call {
        TT finalizer;
        das_final_call() = delete;
        das_final_call(const das_final_call &) = delete;
        das_final_call & operator = (const das_final_call &) = delete;
        das_final_call & operator = (das_final_call &&) = delete;
        das_final_call(das_final_call &&) = default;
        __forceinline das_final_call ( TT && fn ) : finalizer(move(fn)) {}
        __forceinline ~das_final_call () { finalizer(); }
    };

    template <typename TT>
    inline das_final_call<TT> das_finally(TT && fn) {
        return das_final_call<TT>(move(fn));
    }

    template <typename TT>
    __forceinline TT & das_deref ( Context * __context__, const TT * ptr ) {
        if ( !ptr ) __context__->throw_error("dereferencing null pointer");
        return *((TT *)ptr);
    }

    template <typename TT>
    __forceinline TT & das_deref ( Context * __context__, const smart_ptr<TT> & ptr ) {
        if ( !ptr ) __context__->throw_error("dereferencing null pointer");
        return *ptr.get();
    }

    template <typename TT>
    __forceinline TT & das_deref ( Context * __context__, const smart_ptr_raw<TT> & ptr ) {
        if ( !ptr ) __context__->throw_error("dereferencing null pointer");
        return *ptr.get();
    }

    template <typename TT>
    __forceinline TT * das_ref ( Context *, const TT & ref ) {
        return &const_cast<TT &>(ref);
    }

    template <typename TT, typename AT, bool moveIt = false>
    struct das_ascend {
        static __forceinline TT * make(Context * __context__,TypeInfo * typeInfo,const AT & init) {
            if ( char * ptr = (char *)__context__->heap->allocate(sizeof(AT)+ (typeInfo ? 16 : 0)) ) {
                if ( typeInfo ) {
                    *((TypeInfo **)ptr) = typeInfo;
                    ptr += 16;
                }
                memcpy(ptr, &init, sizeof(AT));
                if (moveIt) {
                    memset((char *)&init, 0, sizeof(AT));
                }
                return (TT *) ptr;
            } else {
                __context__->throw_error("out of heap");
                return nullptr;
            }
        }
    };

    template <bool smart, typename TT>
    struct das_ascend_handle;

    template <typename TT>
    struct das_ascend_handle<false,TT *> {
        template <typename QQ>
        static __forceinline TT * make(Context * __context__,TypeInfo * ,QQ && init) {
            TT * ptr = das_new_handle<TT,false>::make(__context__);
            if ( ptr ) {
                init(*ptr);
                return ptr;
            } else {
                __context__->throw_error("new of handled type returned null");
                return nullptr;
            }
        }
    };
    template <typename TT>
    struct das_ascend_handle<true,smart_ptr_raw<TT>> {
        template <typename QQ>
        static __forceinline smart_ptr_raw<TT> make(Context * __context__,TypeInfo * ,QQ && init) {
            smart_ptr_raw<TT> ptr = das_new_handle<TT,true>::make(__context__);
            if ( ptr ) {
                init(*ptr);
                return ptr;
            } else {
                __context__->throw_error("new of handled type returned null");
                return nullptr;
            }
        }
    };

    template <typename AT>
    struct das_ascend<Lambda,AT,false> {
        static __forceinline Lambda make(Context * __context__,TypeInfo * typeInfo,const AT & init) {
            if ( char * ptr = (char *)__context__->heap->allocate(sizeof(AT)+ (typeInfo ? 16 : 0)) ) {
                if ( typeInfo ) {
                    *((TypeInfo **)ptr) = typeInfo;
                    ptr += 16;
                }
                memcpy(ptr, &init, sizeof(AT));
                return Lambda(ptr);
            } else {
                __context__->throw_error("out of heap");
                return Lambda(nullptr);
            }
        }
    };

    template <typename TT>
    struct cast_aot_arg {
        static __forceinline TT to ( Context &, vec4f x ) {
            return cast<TT>::to(x);
        }
    };

    template <>
    struct cast_aot_arg<Context *> {
        static __forceinline Context * to ( Context & ctx, vec4f ) {
            return &ctx;
        }
    };

    template <>
    struct cast_aot_arg<vec4f> {
        static __forceinline vec4f to ( Context &, vec4f x ) {
            return x;
        }
    };

    template <typename R, typename ...Arg, size_t... I>
    __forceinline R CallAotStaticFunction ( R (* fn) (Arg...), Context & ctx, index_sequence<I...> ) {
        return fn(cast_aot_arg<Arg>::to(ctx,ctx.abiArguments()[I?I-1:0])...);
    }

    template <typename FunctionType>
    struct ImplAotStaticFunction;

    template <typename R, typename ...Arg>
    struct ImplAotStaticFunction<R (*)(Arg...)> {
        static __forceinline vec4f call ( R (*fn) (Arg...), Context & ctx ) {
            return cast<R>::from(
                CallAotStaticFunction<R,Arg...>(fn,ctx,make_index_sequence<sizeof...(Arg)>())
            );
        }
    };

    template <typename ...Arg>
    struct ImplAotStaticFunction<void (*)(Arg...)> {
        static __forceinline vec4f call ( void (*fn) (Arg...), Context & ctx ) {
            CallAotStaticFunction<void,Arg...>(fn,ctx,make_index_sequence<sizeof...(Arg)>());
            return v_zero();
        }
    };

    template <typename FunctionType>
    struct ImplAotStaticFunctionCMRES;

    template <typename R, typename ...Arg>
    struct ImplAotStaticFunctionCMRES<R (*)(Arg...)> {
        static __forceinline void call ( R (*fn) (Arg...), Context & ctx ) {
            using result = typename remove_const<R>::type;
            *((result *) ctx.abiCMRES) = CallAotStaticFunction<R,Arg...>(fn,ctx,make_index_sequence<sizeof...(Arg)>());
        }
    };

    template <typename FuncT, FuncT fn>
    struct SimNode_Aot : SimNode_CallBase {
        __forceinline SimNode_Aot ( ) : SimNode_CallBase(LineInfo()) {
            aotFunction = (void*) fn;
        }
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            vec4f * aa = context.abiArg;
            vec4f stub[1];
            if ( !aa ) context.abiArg = stub;
            auto res = ImplAotStaticFunction<FuncT>::call(*fn, context);
            context.abiArg = aa;
            context.abiResult() = res;
            return res;
        }
    };

    template <typename FuncT, FuncT fn>
    struct SimNode_AotCMRES : SimNode_CallBase {
        __forceinline SimNode_AotCMRES ( ) : SimNode_CallBase(LineInfo()) {}
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            vec4f * aa = context.abiArg;
            vec4f stub[1];
            if ( !aa ) context.abiArg = stub;
            ImplAotStaticFunctionCMRES<FuncT>::call(*fn, context);
            context.abiArg = aa;
            context.abiResult() = cast<void *>::from(context.abiCMRES);
            return context.abiResult();
        }
    };

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4100)
#endif

    struct SimNode_AotInteropBase : SimNode_CallBase {
        __forceinline SimNode_AotInteropBase() : SimNode_CallBase(LineInfo()) {}
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            return v_zero();
        };
        vec4f *     argumentValues;
    };

#ifdef _MSC_VER
#pragma warning(pop)
#endif

    template <int argumentCount>
    struct SimNode_AotInterop : SimNode_AotInteropBase {
        template <typename ...VI>
        __forceinline SimNode_AotInterop ( TypeInfo ** tinfo, VI... args ) {
            nArguments = argumentCount;
            types = tinfo;
            argumentValues = argValues;
            vec4f argsE[argumentCount] = { args... };
            for ( int i=0; i!=argumentCount; ++i ) {
                argumentValues[i] = argsE[i];
            }
        };
        vec4f       argValues[argumentCount];
    };

    template <>
    struct SimNode_AotInterop<0> : SimNode_AotInteropBase {
        __forceinline SimNode_AotInterop () {
            nArguments = 0;
            types = nullptr;
            argumentValues = nullptr;
        }
    };

    template <typename TT>
    __forceinline char * das_lexical_cast ( TT x, Context * __context__ ) {
        StringBuilderWriter writer;
        writer << x;
        auto length = writer.tellp();
        if ( length ) {
            return __context__->stringHeap->allocateString(writer.c_str(), length);
        } else {
            return nullptr;
        }
    }

    __forceinline char * das_string_builder ( Context * __context__, const SimNode_AotInteropBase & node ) {
        StringBuilderWriter writer;
        DebugDataWalker<StringBuilderWriter> walker(writer, PrintFlags::string_builder);
        for ( int i = 0; i!=node.nArguments; ++i ) {
            walker.walk(node.argumentValues[i], node.types[i]);
        }
        auto length = writer.tellp();
        if ( length ) {
            return __context__->stringHeap->allocateString(writer.c_str(), length);
        } else {
            return nullptr;
        }
    }

    struct das_stack_prologue {
        __forceinline das_stack_prologue ( Context * __context__, uint32_t stackSize, const char * fn )
            : context(__context__) {
            if (!context->stack.push(stackSize, EP, SP)) {
                context->throw_error("stack overflow");
            }
#if DAS_ENABLE_STACK_WALK
            Prologue * pp = (Prologue *)context->stack.sp();
            pp->info = nullptr;
            pp->fileName = fn;
            pp->stackSize = stackSize;
#endif
        }
        __forceinline ~das_stack_prologue () {
            context->stack.pop(EP, SP);
        }
        Context * context;
        char * EP, * SP;
    };

    struct das_make_block_base : Block, SimNode_ClosureBlock {
        das_make_block_base ( Context * context, uint32_t argStackTop, uint64_t ann, FuncInfo * fi )
                : SimNode_ClosureBlock(LineInfo(),false,false,ann) {
            stackOffset = context->stack.spi();
            argumentsOffset = argStackTop ? (context->stack.spi() + argStackTop) : 0;
            body = this;
            functionArguments = context->abiArguments();
            info = fi;
        }
    };

    template <typename resType, typename ...argType>
    struct das_make_block : das_make_block_base {
        typedef callable < resType ( argType... ) > BlockFn;
        template <typename Functor>
        __forceinline das_make_block ( Context * context, uint32_t argStackTop, uint64_t ann, FuncInfo * fi, Functor && func )
                : das_make_block_base(context,argStackTop,ann,fi), blockFunction(func) {
            aotFunction = &blockFunction;
        };
        template <size_t... I>
        __forceinline resType callBlockFunction(vec4f * args, index_sequence<I...>) {
            return blockFunction((cast<argType>::to(args[I]))...);
        }
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            vec4f ** arguments = (vec4f **)(context.stack.bottom() + argumentsOffset);
            using Indices = make_index_sequence<sizeof...(argType)>;
            return cast<resType>::from ( callBlockFunction(*arguments, Indices()) );
        }
        BlockFn blockFunction;
    };

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4100)
#endif

    template <typename resType>
    struct das_make_block<resType> : das_make_block_base {
        typedef callable < resType () > BlockFn;
        template <typename Functor>
        __forceinline das_make_block ( Context * context, uint32_t argStackTop, uint64_t ann, FuncInfo * fi, Functor && func )
                : das_make_block_base(context,argStackTop,ann,fi), blockFunction(func) {
            aotFunction = &blockFunction;
        };
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            return cast<resType>::from(blockFunction());
        }
        BlockFn blockFunction;
    };

    template <>
    struct das_make_block<void> : das_make_block_base {
        typedef callable < void () > BlockFn;
        template <typename Functor>
        __forceinline das_make_block ( Context * context, uint32_t argStackTop, uint64_t ann, FuncInfo * fi, Functor && func )
                : das_make_block_base(context,argStackTop,ann,fi), blockFunction(func) {
            aotFunction = &blockFunction;
        };
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            blockFunction ( );
            return v_zero();
        }
        BlockFn blockFunction;
    };

#ifdef _MSC_VER
#pragma warning(pop)
#endif

    template <typename ...argType>
    struct das_make_block<void,argType...> : das_make_block_base {
        typedef callable < void ( argType... ) > BlockFn;
        template <typename Functor>
        __forceinline das_make_block ( Context * context, uint32_t argStackTop, uint64_t ann, FuncInfo * fi, Functor && func )
                : das_make_block_base(context,argStackTop,ann,fi), blockFunction(func) {
            aotFunction = &blockFunction;
        };
        template <size_t... I>
        __forceinline void callBlockFunction(vec4f * args, index_sequence<I...>) {
            blockFunction((cast<argType>::to(args[I]))...);
        }
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            vec4f ** arguments = (vec4f **)(context.stack.bottom() + argumentsOffset);
            using Indices = make_index_sequence<sizeof...(argType)>;
            callBlockFunction(*arguments, Indices());
            return v_zero();
        }
        BlockFn blockFunction;
    };

    template <typename resType, typename ...argType>
    struct das_make_block_cmres : das_make_block_base {
        typedef callable < resType ( argType... ) > BlockFn;
        template <typename Functor>
        __forceinline das_make_block_cmres ( Context * context, uint32_t argStackTop, uint64_t ann, FuncInfo * fi, Functor && func )
                : das_make_block_base(context,argStackTop,ann,fi), blockFunction(func) {
            aotFunction = &blockFunction;
        };
        template <size_t... I>
        __forceinline resType callBlockFunction(vec4f * args, index_sequence<I...>) {
            return blockFunction((cast<argType>::to(args[I]))...);
        }
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            auto ba = (BlockArguments *) ( context.stack.bottom() + argumentsOffset );
            using ResultType = typename remove_const<resType>::type;
            ResultType * result = (ResultType *) ba->copyOrMoveResult;
            using Indices = make_index_sequence<sizeof...(argType)>;
            *result = callBlockFunction(ba->arguments, Indices());
            return cast<void *>::from(result);
        }
        BlockFn blockFunction;
    };

    template <typename resType>
    struct das_make_block_cmres<resType> : das_make_block_base {
        typedef callable < resType () > BlockFn;
        template <typename Functor>
        __forceinline das_make_block_cmres ( Context * context, uint32_t argStackTop, uint64_t ann, FuncInfo * fi, Functor && func )
                : das_make_block_base(context,argStackTop,ann,fi), blockFunction(func) {
            aotFunction = &blockFunction;
        };
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            auto ba = (BlockArguments *) ( context.stack.bottom() + argumentsOffset );
            using ResultType = typename remove_const<resType>::type;
            ResultType * result = (ResultType *) ba->copyOrMoveResult;
            *result = blockFunction();
            return cast<void *>::from(result);
        }
        BlockFn blockFunction;
    };

    __forceinline Func getDasClassMethod ( void * self, int offset ) {
        return *(Func *)(((char *)self) + offset);
    }

    template <typename ResType>
    struct das_invoke {
        // vector cast
        static __forceinline ResType invoke ( Context * __context__, const Block & blk ) {
            using BlockFn = callable < ResType () >;
            if ( blk.aotFunction ) {
                auto fn = (BlockFn *) blk.aotFunction;
                return (*fn) ();
            } else {
                vec4f result = __context__->invoke(blk, nullptr, nullptr);
                return cast<ResType>::to(result);
            }
        }
        template <typename ...ArgType>
        static __forceinline ResType invoke ( Context * __context__, const Block & blk, ArgType ...arg ) {
            using BlockFn = callable < ResType ( ArgType... ) >;
            if ( blk.aotFunction ) {
                auto fn = (BlockFn *) blk.aotFunction;
                return (*fn) ( forward<ArgType>(arg)... );
            } else {
                vec4f arguments [] = { cast<ArgType>::from(arg)... };
                vec4f result = __context__->invoke(blk, arguments, nullptr);
                return cast<ResType>::to(result);
            }
        }
        // cmres
        static __forceinline ResType invoke_cmres ( Context * __context__, const Block & blk ) {
            using BlockFn = callable < ResType () >;
            if ( blk.aotFunction ) {
                auto fn = (BlockFn *) blk.aotFunction;
                return (*fn) ();
            } else {
                typename remove_const<ResType>::type result;
                __context__->invoke(blk, nullptr, &result);
                return result;
            }
        }
        template <typename ...ArgType>
        static __forceinline ResType invoke_cmres ( Context * __context__, const Block & blk, ArgType ...arg ) {
            using BlockFn = callable < ResType ( ArgType... ) >;
            if ( blk.aotFunction ) {
                auto fn = (BlockFn *) blk.aotFunction;
                return (*fn) ( forward<ArgType>(arg)... );
            } else {
                vec4f arguments [] = { cast<ArgType>::from(arg)... };
                typename remove_const<ResType>::type result;
                __context__->invoke(blk, arguments, &result);
                return result;
            }
        }
        template <typename BLK, typename ...ArgType>
        static __forceinline ResType invoke_cmres ( Context *, const BLK & blk, ArgType ...arg ) {
            return blk(forward<ArgType>(arg)...);
        }
    };

    template <>
    struct das_invoke<void> {
        static __forceinline void invoke ( Context * __context__, const Block & blk ) {
            using BlockFn = callable < void () >;
            if ( blk.aotFunction ) {
                auto fn = (BlockFn *) blk.aotFunction;
                (*fn) ();
            } else {
                __context__->invoke(blk, nullptr, nullptr);
            }
        }
        template <typename ...ArgType>
        static __forceinline void invoke ( Context * __context__, const Block & blk, ArgType ...arg ) {
            using BlockFn = callable < void ( ArgType... ) >;
            if ( blk.aotFunction ) {
                auto fn = (BlockFn *) blk.aotFunction;
                (*fn) ( forward<ArgType>(arg)... );
            } else {
                vec4f arguments [] = { cast<ArgType>::from(arg)... };
                __context__->invoke(blk, arguments, nullptr);
            }
        }
        template <typename BLK, typename ...ArgType>
        static __forceinline void invoke ( Context *, const BLK & blk, ArgType ...arg ) {
            return blk(forward<ArgType>(arg)...);
        }
    };

    template <typename ResType>
    struct das_invoke_function {
        static __forceinline ResType invoke ( Context * __context__, const Func & blk ) {
            SimFunction * simFunc = __context__->getFunction(blk.index-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            if ( simFunc->aotFunction ) {
                using fnPtrType = ResType (*) ( Context * );
                auto fnPtr = (fnPtrType) simFunc->aotFunction;
                return (*fnPtr) ( __context__ );
            } else {
                vec4f result = __context__->callOrFastcall(simFunc, nullptr, 0);
                return cast<ResType>::to(result);
            }
        }
        template <typename ...ArgType>
        static __forceinline ResType invoke ( Context * __context__, const Func & blk, ArgType ...arg ) {
            SimFunction * simFunc = __context__->getFunction(blk.index-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            if ( simFunc->aotFunction ) {
                using fnPtrType = ResType (*) ( Context *, ArgType... );
                auto fnPtr = (fnPtrType) simFunc->aotFunction;
                return (*fnPtr) ( __context__, forward<ArgType>(arg)... );
            } else {
                vec4f arguments [] = { cast<ArgType>::from(arg)... };
                vec4f result = __context__->callOrFastcall(simFunc, arguments, 0);
                return cast<ResType>::to(result);
            }
        }
        static __forceinline ResType invoke_cmres ( Context * __context__, const Func & blk ) {
            SimFunction * simFunc = __context__->getFunction(blk.index-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            if ( simFunc->aotFunction ) {
                using fnPtrType = ResType (*) ( Context * );
                auto fnPtr = (fnPtrType) simFunc->aotFunction;
                return (*fnPtr) ( __context__ );
            } else {
                typename remove_const<ResType>::type result;
                __context__->callWithCopyOnReturn(simFunc, nullptr, &result, 0);
                return result;
            }
        }
        template <typename ...ArgType>
        static __forceinline ResType invoke_cmres ( Context * __context__, const Func & blk, ArgType ...arg ) {
            vec4f arguments [] = { cast<ArgType>::from(arg)... };
            SimFunction * simFunc = __context__->getFunction(blk.index-1);
            if ( simFunc->aotFunction ) {
                using fnPtrType = ResType (*) ( Context *, ArgType... );
                auto fnPtr = (fnPtrType) simFunc->aotFunction;
                return (*fnPtr) ( __context__, forward<ArgType>(arg)... );
            } else {
                if (!simFunc) __context__->throw_error("invoke null function");
                typename remove_const<ResType>::type result;
                __context__->callWithCopyOnReturn(simFunc, arguments, &result, 0);
                return result;
            }
        }
    };

    template <>
    struct das_invoke_function<void> {
        static __forceinline void invoke ( Context * __context__, const Func & blk ) {
            SimFunction * simFunc = __context__->getFunction(blk.index-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            __context__->callOrFastcall(simFunc, nullptr, 0);
        }
        template <typename ...ArgType>
        static __forceinline void invoke ( Context * __context__, const Func & blk, ArgType ...arg ) {
            vec4f arguments [] = { cast<ArgType>::from(arg)... };
            SimFunction * simFunc = __context__->getFunction(blk.index-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            __context__->callOrFastcall(simFunc, arguments, 0);
        }
    };

    template <typename ResType>
    struct das_invoke_lambda {
        static __forceinline ResType invoke ( Context * __context__, const Lambda & blk ) {
            int32_t * fnIndex = (int32_t *)blk.capture;
            if (!fnIndex) __context__->throw_error("invoke null lambda");
            SimFunction * simFunc = __context__->getFunction(*fnIndex-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            if ( simFunc->aotFunction ) {
                using fnPtrType = ResType (*) ( Context *, void * );
                auto fnPtr = (fnPtrType) simFunc->aotFunction;
                return (*fnPtr) ( __context__, blk.capture );
            } else {
                vec4f arguments[1];
                arguments[0] = cast<Lambda>::from(blk);
                vec4f result = __context__->callOrFastcall(simFunc, arguments, 0);
                return cast<ResType>::to(result);
            }
        }
        template <typename ...ArgType>
        static __forceinline ResType invoke ( Context * __context__, const Lambda & blk, ArgType ...arg ) {
            int32_t * fnIndex = (int32_t *)blk.capture;
            if (!fnIndex) __context__->throw_error("invoke null lambda");
            SimFunction * simFunc = __context__->getFunction(*fnIndex-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            if ( simFunc->aotFunction ) {
                using fnPtrType = ResType (*) ( Context *, void *, ArgType... );
                auto fnPtr = (fnPtrType) simFunc->aotFunction;
                return (*fnPtr) ( __context__, blk.capture, forward<ArgType>(arg)... );
            } else {
                vec4f arguments [] = { cast<void *>::from(blk.capture), (cast<ArgType>::from(arg))... };
                vec4f result = __context__->callOrFastcall(simFunc, arguments, 0);
                return cast<ResType>::to(result);
            }
        }
        static __forceinline ResType invoke_cmres ( Context * __context__, const Lambda & blk ) {
            int32_t * fnIndex = (int32_t *)blk.capture;
            if (!fnIndex) __context__->throw_error("invoke null lambda");
            SimFunction * simFunc = __context__->getFunction(*fnIndex-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            if ( simFunc->aotFunction ) {
                using fnPtrType = ResType (*) ( Context *, void * );
                auto fnPtr = (fnPtrType) simFunc->aotFunction;
                return (*fnPtr) ( __context__, blk.capture );
            } else {
                typename remove_const<ResType>::type result;
                vec4f arguments[1];
                arguments[0] = cast<Lambda>::from(blk);
                __context__->callWithCopyOnReturn(simFunc, arguments, &result, 0);
                return result;
            }
        }
        template <typename ...ArgType>
        static __forceinline ResType invoke_cmres ( Context * __context__, const Lambda & blk, ArgType ...arg ) {
            int32_t * fnIndex = (int32_t *)blk.capture;
            if (!fnIndex) __context__->throw_error("invoke null lambda");
            SimFunction * simFunc = __context__->getFunction(*fnIndex-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            if ( simFunc->aotFunction ) {
                using fnPtrType = ResType (*) ( Context *, void *, ArgType... );
                auto fnPtr = (fnPtrType) simFunc->aotFunction;
                return (*fnPtr) ( __context__, blk.capture, forward<ArgType>(arg)... );
            } else {
                vec4f arguments [] = { cast<void *>::from(blk.capture), (cast<ArgType>::from(arg))... };
                typename remove_const<ResType>::type result;
                __context__->callWithCopyOnReturn(simFunc, arguments, &result, 0);
                return result;
            }
        }
    };

    template <>
    struct das_invoke_lambda<void> {
        static __forceinline void invoke ( Context * __context__, const Lambda & blk ) {
            int32_t * fnIndex = (int32_t *)blk.capture;
            if (!fnIndex) __context__->throw_error("invoke null lambda");
            SimFunction * simFunc = __context__->getFunction(*fnIndex-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            if ( simFunc->aotFunction ) {
                using fnPtrType = void (*) ( Context *, void * );
                auto fnPtr = (fnPtrType) simFunc->aotFunction;
                (*fnPtr) ( __context__, blk.capture );
            } else {
                vec4f arguments[1];
                arguments[0] = cast<Lambda>::from(blk);
                __context__->callOrFastcall(simFunc, arguments, 0);
            }
        }
        template <typename ...ArgType>
        static __forceinline void invoke ( Context * __context__, const Lambda & blk, ArgType ...arg ) {
            int32_t * fnIndex = (int32_t *)blk.capture;
            if (!fnIndex) __context__->throw_error("invoke null lambda");
            SimFunction * simFunc = __context__->getFunction(*fnIndex-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            if ( simFunc->aotFunction ) {
                using fnPtrType = void (*) ( Context *, void *, ArgType... );
                auto fnPtr = (fnPtrType) simFunc->aotFunction;
                (*fnPtr) ( __context__, blk.capture, forward<ArgType>(arg)... );
            } else {
                vec4f arguments [] = { cast<void *>::from(blk.capture), (cast<ArgType>::from(arg))... };
                __context__->callOrFastcall(simFunc, arguments, 0);
            }
        }
    };

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4611)   // interaction between '_setjmp' and C++ object destruction is non-portable
#endif

    template <typename TA, typename TB>
    inline void das_try_recover ( Context * __context__, TA && try_block, TB && catch_block ) {
        auto aa = __context__->abiArg; auto acm = __context__->abiCMRES;
        char * EP, * SP;
        __context__->stack.watermark(EP,SP);
#if DAS_ENABLE_EXCEPTIONS
        try {
            try_block();
        } catch ( const dasException & ) {
            catch_block();
            __context__->abiArg = aa;
            __context__->abiCMRES = acm;
            __context__->stack.pop(EP,SP);
            __context__->stopFlags = 0;
            __context__->exception = nullptr;
        }
#else
        jmp_buf ev;
        jmp_buf * JB = __context__->throwBuf;
        __context__->throwBuf = &ev;
        if ( !setjmp(ev) ) {
            try_block();
        } else {
            catch_block();
            __context__->throwBuf = JB;
            __context__->abiArg = aa;
            __context__->abiCMRES = acm;
            __context__->stack.pop(EP,SP);
            __context__->stopFlags = 0;
            __context__->exception = nullptr;
        }
        __context__->throwBuf = JB;
#endif
    }

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

    template <typename TT>
    struct das_call_interop {
        static __forceinline TT call ( InteropFunction * func, Context * __context__, const SimNode_AotInteropBase & node ) {
            vec4f result = (*func) ( *__context__, (SimNode_CallBase *)&node, node.argumentValues );
            return cast<TT>::to(result);
        }
    };

    template<>
    struct das_call_interop<void> {
        static __forceinline void call ( InteropFunction * func, Context * __context__, const SimNode_AotInteropBase & node ) {
            (*func) ( *__context__, (SimNode_CallBase *)&node, node.argumentValues );
        }
    };

    template <>
    struct das_iterator <Sequence> {
        __forceinline das_iterator(const Sequence & r) {
            that = r.iter;
            if ( !that ) {
                DAS_ASSERTF(false,"iterator is empty or already consumed");
                // context.throw_error_at(debugInfo,"iterator is empty or already consumed");
            } else {
                r.iter = nullptr;
            }
        }
        template <typename TT>
        __forceinline bool first ( Context * __context__, TT & i ) {
            context = __context__;
            that->isOpen = true;
            return that->first(*__context__, (char *)&i);
        }
        template <typename TT>
        __forceinline bool next  ( Context * __context__, TT & i ) {
            return that->next(*__context__,(char *)&i);
        }
        template <typename TT>
        __forceinline void close ( Context * __context__, TT & i ) {
            that->close(*__context__,(char *)&i);
            context = nullptr;
        }
        ~das_iterator() {
            if (context) that->close(*context, nullptr);
        }
        Iterator * that = nullptr;
        Context * context = nullptr;
    };

    template <>
    struct das_iterator <Sequence const> : das_iterator<Sequence> {
        __forceinline das_iterator(const Sequence & r)
            : das_iterator<Sequence>(r) {
        }
    };


    template <typename TK, typename TV, typename TKey>
    __forceinline TV * __builtin_table_find ( Context * context, const TTable<TK, TV> & tab, TKey _key ) {
        TK key = (TK) _key;
        auto hfn = hash_function(*context, key);
        TableHash<TK> thh(context,sizeof(TV));
        int index = thh.find(tab, key, hfn);
        return (TV *) ( index!=-1 ? tab.data + index * sizeof(TV) : nullptr );
    }

    template <typename TK, typename TV, typename TKey>
    __forceinline bool __builtin_table_key_exists ( Context * context, const TTable<TK, TV> & tab, TKey _key ) {
        TK key = (TK) _key;
        auto hfn = hash_function(*context, key);
        TableHash<TK> thh(context, sizeof(TV));
        return thh.find(tab, key, hfn) != -1;
    }

    template <typename TK, typename TV, typename TKey>
    __forceinline bool __builtin_table_erase ( Context * context, TTable<TK,TV> & tab, TKey _key ) {
        if ( tab.lock ) context->throw_error("can't erase from locked table");
        TK key = (TK) _key;
        auto hfn = hash_function(*context, key);
        TableHash<TK> thh(context,sizeof(TV));
        return thh.erase(tab, key, hfn) != -1;
    }

    template <typename VectorType>
    struct StdVectorIterator : Iterator {
        using OT = typename VectorType::value_type;
        StdVectorIterator  ( VectorType * ar ) : array(ar) {}
        virtual bool first ( Context &, char * _value ) override {
            char ** value = (char **) _value;
            char * data     = (char *) array->data();
            uint32_t size   = (uint32_t) array->size();
            *value          = data;
            array_end       = data + size * sizeof(OT);
            return (bool) size;
        }
        virtual bool next  ( Context &, char * _value ) override {
            char ** value = (char **) _value;
            char * data = *value + sizeof(OT);
            *value = data;
            return data != array_end;
        }
        virtual void close ( Context & context, char * _value ) override {
            if ( _value ) {
                char ** value = (char **) _value;
                *value = nullptr;
            }
            context.heap->free((char *)this, sizeof(StdVectorIterator));
        }
        VectorType * array;
        char * array_end = nullptr;
    };

    template <typename TT>
    Sequence das_vector_each_sequence ( const TT & vec, Context * context ) {
        using VectorIterator = StdVectorIterator<TT>;
        char * iter = context->heap->allocate(sizeof(VectorIterator));
        context->heap->mark_comment(iter, "std::vector<> iterator");
        new (iter) VectorIterator((TT *)&vec);
        return { (Iterator *) iter };
    }

    template <typename TT, typename QQ = typename TT::value_type>
    __forceinline TSequence<QQ &> das_vector_each ( TT & vec, Context * context ) {
        return das_vector_each_sequence(vec,context);
    }

    template <typename TT, typename QQ = typename TT::value_type>
    __forceinline TSequence<const QQ &> das_vector_each_const ( const TT & vec, Context * context ) {
        return das_vector_each_sequence(vec,context);
    }

    template <typename TT, typename QQ = typename TT::value_type>
    __forceinline void das_vector_push ( TT & vec, const QQ & value, int32_t at, Context * context ) {
        if ( uint32_t(at)>vec.size() ) {
            context->throw_error_ex("insert index out of range, %i of %u", at, uint32_t(vec.size()));
        }
        vec.insert(vec.begin() + at, value);
    }

    template <typename TT, typename QQ = typename TT::value_type>
    __forceinline void das_vector_push_value ( TT & vec, QQ value, int32_t at, Context * context ) {
        if ( uint32_t(at)>vec.size() ) {
            context->throw_error_ex("insert index out of range, %i of %u", at, uint32_t(vec.size()));
        }
        vec.insert(vec.begin() + at, value);
    }

    template <typename TT, typename QQ = typename TT::value_type>
    __forceinline void das_vector_emplace ( TT & vec, QQ & value, int32_t at ) {
        vec.emplace(vec.begin()+at, move(value));
    }

    template <typename TT, typename QQ = typename TT::value_type>
    __forceinline void das_vector_push_back ( TT & vec, const QQ & value ) {
        vec.push_back(value);
    }

    template <typename TT, typename QQ = typename TT::value_type>
    __forceinline void das_vector_push_back_value ( TT & vec, QQ value ) {
        vec.push_back(value);
    }

    template <typename TT, typename QQ = typename TT::value_type>
    __forceinline void das_vector_emplace_back ( TT & vec, QQ & value ) {
        vec.emplace_back(move(value));
    }

    template <typename TT>
    __forceinline void das_vector_pop ( TT & vec ) {
        vec.pop_back();
    }

    template <typename TT>
    __forceinline void das_vector_clear ( TT & vec ) {
        vec.clear();
    }

    template <typename TT>
    __forceinline void das_vector_resize ( TT & vec, int32_t newSize ) {
        vec.resize(newSize);
    }

    template <typename TT>
    __forceinline void das_vector_erase ( TT & vec, int32_t index, Context * context ) {
        if ( uint32_t(index)>vec.size() ) {
            context->throw_error_ex("erasing vector index out of range %i of %i", index, int32_t(vec.size()));
        }
        vec.erase(vec.begin() + index);
    }

    template <typename TT>
    void peek_das_string_T(const string & str, TT && block, Context *) {
        block((char *)str.c_str());
    }

    void peek_das_string(const string & str, const TBlock<void,TTemporary<const char *>> & block, Context * context);

    char * builtin_string_clone ( const char *str, Context * context );

    template <typename TT, typename QQ>
    struct das_clone {
        static __forceinline void clone ( TT & dst, const QQ & src ) {
            dst = src;
        }
    };

    char * to_das_string(const string & str, Context * ctx);
    void set_das_string(string & str, const char * bs);

    __forceinline bool das_str_equ ( const string & a, const string & b ) { return a==b; }
    __forceinline bool das_str_nequ ( const string & a, const string & b ) { return a!=b; }

    __forceinline int32_t enum_to_int   ( EnumStub stub )   { return stub.value; }
    __forceinline int32_t enum8_to_int  ( EnumStub8 stub )  { return stub.value; }
    __forceinline int32_t enum16_to_int ( EnumStub16 stub ) { return stub.value; }

    __forceinline uint32_t enum_to_uint   ( EnumStub stub )   { return uint32_t(stub.value); }
    __forceinline uint32_t enum8_to_uint  ( EnumStub8 stub )  { return uint32_t(stub.value); }
    __forceinline uint32_t enum16_to_uint ( EnumStub16 stub ) { return uint32_t(stub.value); }


    template <typename CType>
    struct das_using {
        template <typename TT>
        static __forceinline void use ( TT && block ) {
            CType value;
            block ( value );
        }
        template <typename TT, typename A1>
        static __forceinline void use ( A1 a1, TT && block ) {
            CType value ( a1 );
            block ( value );
        }
        template <typename TT, typename A1, typename A2>
        static __forceinline void use ( A1 a1, A2 a2, TT && block ) {
            CType value ( a1, a2 );
            block ( value );
        }
        template <typename TT, typename A1, typename A2, typename A3>
        static __forceinline void use ( A1 a1, A2 a2, A3 a3, TT && block ) {
            CType value ( a1, a2, a3 );
            block ( value );
        }
        template <typename TT, typename A1, typename A2, typename A3, typename A4>
        static __forceinline void use ( A1 a1, A2 a2, A3 a3, A4 a4, TT && block ) {
            CType value ( a1, a2, a3, a4 );
            block ( value );
        }
        template <typename TT, typename A1, typename A2, typename A3, typename A4, typename A5>
        static __forceinline void use ( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, TT && block ) {
            CType value ( a1, a2, a3, a4, a5 );
            block ( value );
        }
        template <typename TT, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
        static __forceinline void use ( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, TT && block ) {
            CType value ( a1, a2, a3, a4, a5, a6 );
            block ( value );
        }
    };

#define DAS_CALL_MEMBER(type_and_fun)    \
    das::das_call_member< DAS_BIND_MEMBER_FUN(type_and_fun) >


#define DAS_CALL_MEMBER_CPP(type_and_fun) \
    "das_call_member< DAS_BIND_MEMBER_FUN(" #type_and_fun ")>::invoke"

    template <typename FuncT, FuncT fun>
    struct das_call_member;

    template <typename R, typename CC, typename ...Args, R (CC::*func)(Args...) >
    struct das_call_member < R (CC::*)(Args...),  func> {
        static R invoke ( CC & THIS, Args... args ) {
            return ((THIS).*(func)) ( args... );
        }
    };

    template <typename R, typename CC, typename ...Args, R (CC::*func)(Args...) const >
    struct das_call_member < R (CC::*)(Args...) const,  func> {
        static R invoke ( const CC & THIS, Args... args ) {
            return ((THIS).*(func)) ( args... );
        }
    };

    template <typename ET>
    struct das_operator_enum_AND {
        static ET invoke ( ET a, ET b ) {
            return ET(int(a) & int(b));
        }
        template <typename EA, typename EB>
        static ET invoke ( EA a, EB b ) {
            return ET(int(a) & int(b));
        }
    };
    template <typename ET>
    struct das_operator_enum_OR {
        static ET invoke ( ET a, ET b ) {
            return ET(int(a) | int(b));
        }
        template <typename EA, typename EB>
        static ET invoke ( EA a, EB b ) {
            return ET(int(a) | int(b));
        }
    };

    template <typename ET>
    struct das_operator_enum_XOR {
        static ET invoke ( ET a, ET b ) {
            return ET(int(a) ^ int(b));
        }
        template <typename EA, typename EB>
        static ET invoke ( EA a, EB b ) {
            return ET(int(a) ^ int(b));
        }
    };

    template <typename ET>
    struct das_operator_enum_OR_EQU {
        static void invoke ( ET & a, ET b ) {
            a = ET(int(a) | int(b));
        }
        template <typename EA, typename EB>
        static void invoke ( EA & a, EB b ) {
            a = EA(int(a) | int(b));
        }
    };

    template <typename ET>
    struct das_operator_enum_XOR_EQU {
        static void invoke ( ET & a, ET b ) {
            a = ET(int(a) ^ int(b));
        }
        template <typename EA, typename EB>
        static void invoke ( EA & a, EB b ) {
            a = EA(int(a) ^ int(b));
        }
    };

    template <typename ET>
    struct das_operator_enum_AND_EQU {
        static void invoke ( ET & a, ET b ) {
            a = ET(int(a) & int(b));
        }
        template <typename EA, typename EB>
        static void invoke ( EA & a, EB b ) {
            a = EA(int(a) & int(b));
        }
    };

    template <typename ET>
    struct das_operator_enum_AND_AND {
        static bool invoke ( ET a, ET b ) {
            return (int(a) & int(b))!=0;
        }
        template <typename EA, typename EB>
        static bool invoke ( EA a, EB b ) {
            return (int(a) & int(b))!=0;
        }
    };

    template <typename ET>
    struct das_operator_enum_NOT {
        static ET invoke ( ET a ) {
            return ET(~int(a));
        }
        template <typename EA>
        static ET invoke ( EA a ) {
            return ET(~int(a));
        }
    };

    template <typename CompareFn, typename TT>
    struct scblk;

    template <typename CompareFn, typename TT>
    struct scblk {
        template <int dimSize>
        static __forceinline void srt ( TDim<TT,dimSize> & arr, int32_t, int32_t, CompareFn && cmp, Context * context ) {
            sort(arr.data, arr.data + dimSize, cmp);
        }
        template <int dimSize>
        static __forceinline void srtr ( TDim<TT,dimSize> & arr, int32_t elemSize, int32_t length, CompareFn && cmp, Context * context ) {
            srt(arr,elemSize,length,forward<CompareFn>(cmp),context);
        }
    };

    template <typename TT>
    struct scblk < const Block &,TT > {
        template <int dimSize>
        static __forceinline void srtr ( TDim<TT,dimSize> & arr, int32_t, int32_t length, const Block & cmp, Context * context ) {
            vec4f bargs[2];
            auto data = (TT *) arr.data;
            context->invokeEx(cmp, bargs, nullptr, [&](SimNode * code) {
                sort ( data, data+length, [&](TT x, TT y) -> bool {
                    bargs[0] = cast<TT>::from(x);
                    bargs[1] = cast<TT>::from(y);
                    return code->evalBool(*context);
                });
            });
        }
        template <int dimSize>
        static __forceinline void srt ( TDim<TT,dimSize> & arr, int32_t, int32_t length, const Block & cmp, Context * context ) {
            vec4f bargs[2];
            auto data = (TT *) arr.data;
            context->invokeEx(cmp, bargs, nullptr, [&](SimNode * code) {
                sort ( data, data+length, [&](const TT & x, const TT & y) -> bool {
                    bargs[0] = cast<const TT &>::from(x);
                    bargs[1] = cast<const TT &>::from(y);
                    return code->evalBool(*context);
                });
            });
        }
    };

    template <typename CompareFn, typename TT, int32_t dimSize>
    __forceinline void builtin_sort_dim_any_cblock_T ( TDim<TT,dimSize> & arr, int32_t elemSize, int32_t length, CompareFn && cmp, Context * context ) {
        scblk<CompareFn,TT>::srt(arr,elemSize,length,forward<CompareFn>(cmp),context);
    }

    template <typename CompareFn, typename TT, int32_t dimSize>
    __forceinline void builtin_sort_dim_any_ref_cblock_T ( TDim<TT,dimSize> & arr, int32_t elemSize, int32_t length, CompareFn && cmp, Context * context ) {
        scblk<CompareFn,TT>::srtr(arr,elemSize,length,forward<CompareFn>(cmp),context);
    }

    template <typename TT>
    void builtin_sort_cblock ( vec4f arr, int32_t, int32_t length, const TBlock<bool,TT,TT> & cmp, Context * context ) {
        vec4f bargs[2];
        auto data = cast<TT *>::to(arr);
        context->invokeEx(cmp, bargs, nullptr, [&](SimNode * code) {
            sort ( data, data+length, [&](TT x, TT y) -> bool {
                bargs[0] = cast<TT>::from(x);
                bargs[1] = cast<TT>::from(y);
                return code->evalBool(*context);
            });
        });
    }

    template <typename CompareFn, typename TT>
    struct scblk_array;

    template <typename CompareFn, typename TT>
    struct scblk_array {
        static __forceinline void srt ( Array & arr, int32_t, int32_t, CompareFn && cmp, Context * context ) {
            if ( arr.size<=1 ) return;
            array_lock(*context, arr);
            auto sdata = (TT *) arr.data;
            das::sort(sdata, sdata + arr.size, cmp);
            array_unlock(*context, arr);
        }
        static __forceinline void srtr ( Array & arr, int32_t elemSize, int32_t length, CompareFn && cmp, Context * context ) {
            srt(arr,elemSize,length,forward<CompareFn>(cmp),context);
        }
    };

    template <typename TT>
    struct scblk_array < const Block &,TT > {
        static __forceinline void srtr ( Array & arr, int32_t, int32_t, const Block & cmp, Context * context ) {
            if ( arr.size<=1 ) return;
            vec4f bargs[2];
            auto data = (TT *) arr.data;
            array_lock(*context, arr);
            context->invokeEx(cmp, bargs, nullptr, [&](SimNode * code) {
                das::sort ( data, data+arr.size, [&](TT x, TT y) -> bool {
                    bargs[0] = cast<TT>::from(x);
                    bargs[1] = cast<TT>::from(y);
                    return code->evalBool(*context);
                });
            });
            array_unlock(*context, arr);
        }
        static __forceinline void srt ( Array & arr, int32_t, int32_t, const Block & cmp, Context * context ) {
            if ( arr.size<=1 ) return;
            vec4f bargs[2];
            auto data = (TT *) arr.data;
            array_lock(*context, arr);
            context->invokeEx(cmp, bargs, nullptr, [&](SimNode * code) {
                das::sort ( data, data+arr.size, [&](const TT & x, const TT & y) -> bool {
                    bargs[0] = cast<const TT &>::from(x);
                    bargs[1] = cast<const TT &>::from(y);
                    return code->evalBool(*context);
                });
            });
            array_unlock(*context, arr);
        }
    };

    template <typename TT>
    void builtin_sort_cblock_array ( Array & arr, int32_t elemSize, int32_t elemCount, const TBlock<bool,TT,TT> & cmp, Context * context ) {
        scblk_array<const Block &,TT>::srtr(arr,elemSize,elemCount,cmp,context);
    }

    template <typename CompareFn, typename TT>
    __forceinline void builtin_sort_array_any_cblock_T ( TArray<TT> & arr, int32_t elemSize, int32_t elemCount, CompareFn && cmp, Context * context ) {
        scblk_array<CompareFn,TT>::srt(arr,elemSize,elemCount,forward<CompareFn>(cmp),context);
    }

    template <typename CompareFn, typename TT>
    __forceinline void builtin_sort_array_any_ref_cblock_T ( TArray<TT> & arr, int32_t elemSize, int32_t elemCount, CompareFn && cmp, Context * context ) {
        scblk_array<CompareFn,TT>::srtr(arr,elemSize,elemCount,forward<CompareFn>(cmp),context);
    }

    __forceinline vec4f cvt_float4 ( int2 i ) { return v_cvt_vec4f(v_cast_vec4i(vec4f(i))); }
    __forceinline vec4f cvt_float4 ( int3 i ) { return v_cvt_vec4f(v_cast_vec4i(vec4f(i))); }
    __forceinline vec4f cvt_float4 ( int4 i ) { return v_cvt_vec4f(v_cast_vec4i(vec4f(i))); }
    __forceinline vec4f cvt_float4 ( uint2 i ) { return v_cvt_vec4f(v_cast_vec4i(vec4f(i))); }
    __forceinline vec4f cvt_float4 ( uint3 i ) { return v_cvt_vec4f(v_cast_vec4i(vec4f(i))); }
    __forceinline vec4f cvt_float4 ( uint4 i ) { return v_cvt_vec4f(v_cast_vec4i(vec4f(i))); }
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
