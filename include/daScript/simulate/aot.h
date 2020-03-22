#pragma once

#include "daScript/misc/function_traits.h"
#include "daScript/simulate/runtime_profile.h"
#include "daScript/simulate/debug_print.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/aot_builtin.h"
#include "daScript/simulate/aot_builtin_matrix.h"
#include "daScript/simulate/aot_builtin_time.h"
#include "daScript/simulate/aot_builtin_string.h"
#include "daScript/simulate/bin_serializer.h"
#include "daScript/simulate/runtime_table.h"
#include "daScript/simulate/interop.h"

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#if __GNUC__>=9
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif
#pragma GCC diagnostic ignored "-Wsizeof-pointer-memaccess"
#endif

namespace das {

    #define DAS_MAKE_ANNOTATION(name)   ((TypeAnnotation*)(intptr_t(name)|1))

    void das_debug ( Context * context, TypeInfo * typeInfo, const char * FILE, int LINE, vec4f res, const char * message = nullptr );

    template <typename TT>
    __forceinline void das_zero ( TT & a ) {
        memset(&a, 0, sizeof(TT));
    }

    template <typename TT>
    __forceinline void das_move ( TT & a, TT & b ) {
        a = b;
        memset(&b, 0, sizeof(TT));
    }

    template <typename TT, typename QQ>
    __forceinline void das_copy ( TT & a, const QQ b ) {
        a = (TT) b;
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

    template <typename TT, int offset>
    __forceinline TT & das_global ( Context * __context__ ) {
        return *(TT *)(__context__->globals + offset);
    }

    template <typename TT, int offset>
    __forceinline void das_global_zero ( Context * __context__ ) {
        memset(__context__->globals + offset, 0, sizeof(TT));
    }

    template <typename TT, int offset>
    __forceinline TT & das_shared ( Context * __context__ ) {
        return *(TT *)(__context__->shared + offset);
    }

    template <typename TT, int offset>
    __forceinline void das_shared_zero ( Context * __context__ ) {
        memset(__context__->shared + offset, 0, sizeof(TT));
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
    };

    template <typename TT>
    struct das_null_coalescing<TT &> {
        static __forceinline TT & get ( const TT * ptr, TT & value ) {
            return ptr ? *((TT *)ptr) : value;
        }
    };

    template <typename TT>
    struct das_null_coalescing<const TT &> {
        static __forceinline const TT & get ( const TT * ptr, const TT & value ) {
            return ptr ? *ptr : value;
        }
    };

    template <typename TT, typename RR, RR TT::*Member>
    struct das_safe_navigation {
        static __forceinline RR * get ( TT * ptr ) {
            if ( ptr ) {
                return &(ptr->*Member);
            } else {
                return nullptr;
            }
        }
    };

    template <typename TT, typename RR, RR TT::*Member>
    struct das_safe_navigation_ptr {
        static __forceinline RR get ( TT * ptr ) {
            if ( ptr ) {
                return ptr->*Member;
            } else {
                return nullptr;
            }
        }
    };

    template <typename TT>
    struct das_index;

    template <typename TT, typename VecT, uint32_t size>
    struct das_vec_index {
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
    };

    template <typename TT> struct das_index<vec2<TT>> : das_vec_index<TT, vec2<TT>, 2> {};
    template <typename TT> struct das_index<vec3<TT>> : das_vec_index<TT, vec3<TT>, 3> {};
    template <typename TT> struct das_index<vec4<TT>> : das_vec_index<TT, vec4<TT>, 4> {};

    template <typename TT> struct das_index<const vec2<TT>> : das_vec_index<TT, vec2<TT>, 2> {};
    template <typename TT> struct das_index<const vec3<TT>> : das_vec_index<TT, vec3<TT>, 3> {};
    template <typename TT> struct das_index<const vec4<TT>> : das_vec_index<TT, vec4<TT>, 4> {};


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

    template <typename TT, uint32_t size>
    struct TDim {
        enum { capacity = size };
        TT  data[size];
        __forceinline TT & operator [] ( int32_t index ) {
            return data[index];
        }
        __forceinline const TT & operator [] ( int32_t index ) const {
            return data[index];
        }
        __forceinline TT & operator () ( int32_t index, Context * __context__ ) {
            uint32_t idx = uint32_t(index);
            if ( idx>=size ) __context__->throw_error_ex("index out of range, %u of %u", idx, size);
            return data[index];
        }
        __forceinline const TT & operator () ( int32_t index, Context * __context__ ) const {
            uint32_t idx = uint32_t(index);
            if ( idx>=size ) __context__->throw_error_ex("index out of range, %u of %u", idx, size);
            return data[index];
        }
        __forceinline TT & operator () ( uint32_t idx, Context * __context__ ) {
            if ( idx>=size ) __context__->throw_error_ex("index out of range, %u of %u", idx, size);
            return data[idx];
        }
        __forceinline const TT & operator () ( uint32_t idx, Context * __context__ ) const {
            if ( idx>=size ) __context__->throw_error_ex("index out of range, %u of %u", idx, size);
            return data[idx];
        }
    };

    template <typename TT, uint32_t size>
    struct das_cast< TDim<TT,size> > {
        template <typename QQ>
        static __forceinline TDim<TT,size> & cast ( const QQ * expr ) {
            return *((TDim<TT,size> *)expr);
        }
    };

    template <typename TT, uint32_t size>
    struct das_cast< const TDim<TT,size> > {
        template <typename QQ>
        static __forceinline const TDim<TT,size> & cast ( const QQ * expr ) {
            return *((const TDim<TT,size> *)expr);
        }
    };

    template <typename TT>
    struct TArray : Array {
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
        __forceinline TT & operator () ( int32_t index, Context * __context__ ) {
            uint32_t idx = uint32_t(index);
            if ( idx>=size ) __context__->throw_error_ex("array index out of range, %u of %u", idx, size);
            return ((TT *)data)[index];
        }
        __forceinline const TT & operator () ( int32_t index, Context * __context__ ) const {
            uint32_t idx = uint32_t(index);
            if ( idx>=size ) __context__->throw_error_ex("array index out of range, %u of %u", idx, size);
            return ((TT *)data)[index];
        }
        __forceinline TT & operator () ( uint32_t idx, Context * __context__ ) {
            if ( idx>=size ) __context__->throw_error_ex("array index out of range, %u of %u", idx, size);
            return ((TT *)data)[idx];
        }
        __forceinline const TT & operator () ( uint32_t idx, Context * __context__ ) const {
            if ( idx>=size ) __context__->throw_error_ex("array index out of range, %u of %u", idx, size);
            return ((TT *)data)[idx];
        }
    };

    template <typename TK, typename TV>
    struct TTable : Table {
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

    template <typename TT>
    struct das_iterator;

    template <>
    struct das_iterator <const range> {
        __forceinline das_iterator(const range & r) : that(r) {}
        __forceinline bool first ( Context *, int32_t & i ) { i = that.from; return i!=that.to; }
        __forceinline bool next  ( Context *, int32_t & i ) { i++; return i!=that.to; }
        __forceinline void close ( Context *, int32_t &   ) {}
        range that;
    };

    template <>
    struct das_iterator <range> : das_iterator<const range> {
        __forceinline das_iterator(const range & r) : das_iterator<const range>(r) {}
    };

    template <>
    struct das_iterator <char * const> {
        __forceinline das_iterator(const char * st) : str(st) {}
        __forceinline bool first ( Context *, int32_t & i ) {
            if ( !str || *str==0 ) return false;
            i = *str++;
            return true;
        }
        __forceinline bool next  ( Context *, int32_t & i ) {
            i = *str++;
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
            array_end = (TT *)(that->data + that->size*sizeof(TT));
        }
        __forceinline bool first ( Context * __context__, TT * & i ) {
            array_lock(*__context__, *that);
            i = (TT *) that->data;
            return i!=array_end;
        }
        __forceinline bool next  ( Context *, TT * & i ) {
            i++;
            return i!=array_end;
        }
        __forceinline void close ( Context * __context__, TT * & i ) {
            array_unlock(*__context__, *that);
            i = nullptr;
        }
        Array * that;
        TT * array_end;
    };

    template <typename TT>
    struct das_iterator<const TArray<TT>> {
        __forceinline das_iterator(const TArray<TT> & r) {
            that = &r;
            array_end = (TT *)(that->data + that->size*sizeof(TT));
        }
        __forceinline bool first ( Context * __context__, const TT * & i ) {
            array_lock(*__context__, *(Array *)(that)); // technically we don't need for the const array, but...
            i = (const TT *) that->data;
            return i!=array_end;
        }
        __forceinline bool next  ( Context *, const TT * & i ) {
            i++;
            return i!=array_end;
        }
        __forceinline void close ( Context * __context__, const TT * & i ) {
            array_unlock(*__context__, *(Array *)(that));  // technically we don't need for the const array, but...
            i = nullptr;
        }
        const Array * that;
        const TT * array_end;
    };

    template <typename TT, uint32_t size>
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

    template <typename TT, uint32_t size>
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
        __forceinline bool first ( Context *, TT * & i ) {
            i = array_start;
            return i!=array_end;
        }
        __forceinline bool next  ( Context *, TT * & i ) {
            i++;
            return i!=array_end;
        }
        __forceinline void close ( Context *, TT * & i ) {
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
        __forceinline bool first ( Context *, const TT * & i ) {
            i = array_start;
            return i!=array_end;
        }
        __forceinline bool next  ( Context *, const TT * & i ) {
            i++;
            return i!=array_end;
        }
        __forceinline void close ( Context *, const TT * & i ) {
            i = nullptr;
        }
        const TT *      array_start;
        const TT *      array_end;
    };

    template <typename TT>
    struct das_new {
        static __forceinline TT * make ( Context * __context__ ) {
            char * data = __context__->heap.allocate( sizeof(TT) );
            memset ( data, 0, sizeof(TT) );
            return (TT *) data;
        }
        template <typename QQ>
        static __forceinline TT * make_and_init ( Context * __context__, QQ && init ) {
            TT * data = (TT *) __context__->heap.allocate( sizeof(TT) );
            *data = init();
            return data;
        }
    };

    template <typename TT, int d0 = 0, int d1 = 0, int d2 = 0>
    struct das_new_dim;

    template <typename TT, int d>
    struct das_new_dim<TT,d,0,0> {
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

    template <typename TT>
    struct das_delete_ptr;

    template <typename TT>
    struct das_delete_ptr<TT *> {
        static __forceinline void clear ( Context * __context__, TT * ptr ) {
            __context__->heap.free((char *)ptr, sizeof(TT));
        }
    };

    template <>
    struct das_delete_ptr<char *> {
        static __forceinline void clear ( Context * __context__, char * string ) {
            const uint32_t size = stringLengthSafe(*__context__, string);
            __context__->stringHeap.freeString(string, size);
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
                    __context__->heap.free(dim.data, oldSize);
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
                    __context__->heap.free(tab.data, oldSize);
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
    __forceinline TT * das_ref ( Context *, const TT & ref ) {
        return &const_cast<TT &>(ref);
    }

    template <typename TT, typename AT, bool moveIt = false>
    struct das_ascend {
        static __forceinline TT * make(Context * __context__,TypeInfo * typeInfo,const AT & init) {
            if ( char * ptr = (char *)__context__->heap.allocate(sizeof(AT)+ (typeInfo ? 16 : 0)) ) {
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

    template <typename AT>
    struct das_ascend<Lambda,AT,false> {
        static __forceinline Lambda make(Context * __context__,TypeInfo * typeInfo,const AT & init) {
            if ( char * ptr = (char *)__context__->heap.allocate(sizeof(AT)+ (typeInfo ? 16 : 0)) ) {
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

    template <typename Result>
    struct ImplAotStaticFunction {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline vec4f call(FunctionType && fn, Context & ctx, index_sequence<I...> ) {
            return cast<Result>::from(
            fn( cast_aot_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, ctx.abiArguments()[ I ? I-1 : 0 ] )... ) );
        }
    };

    template <>
    struct ImplAotStaticFunction<void> {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline vec4f call(FunctionType && fn, Context & ctx, index_sequence<I...> ) {
            fn( cast_aot_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, ctx.abiArguments()[ I ? I-1 : 0 ] )... );
            return v_zero();
        }
    };

    template <typename Result>
    struct ImplAotStaticFunctionCMRES {
        template <typename FunctionType, typename ArgumentsType, size_t... I>
        static __forceinline Result call(FunctionType && fn, Context & ctx, index_sequence<I...> ) {
            return fn( cast_aot_arg< typename tuple_element<I, ArgumentsType>::type  >::to ( ctx, ctx.abiArguments()[ I ? I-1 : 0 ] )... );
        }
    };

    template <typename FuncT, FuncT fn>
    struct SimNode_Aot : SimNode_CallBase {
        const char * extFnName = nullptr;
        __forceinline SimNode_Aot ( ) : SimNode_CallBase(LineInfo()) {}
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            using FunctionTrait = function_traits<FuncT>;
            using Result = typename FunctionTrait::return_type;
            using Arguments = typename FunctionTrait::arguments;
            const int nargs = tuple_size<Arguments>::value;
            using Indices = make_index_sequence<nargs>;
            // TODO: sort out interop
            vec4f * aa = context.abiArg;
            vec4f stub[1] = { v_zero() };
            if ( !aa ) context.abiArg = stub;
            auto res = ImplAotStaticFunction<Result>::template
                call<FuncT,Arguments>(*fn, context, Indices());
            context.abiArg = aa;
            context.abiResult() = res;
            return res;
        }
    };

    template <typename FuncT, FuncT fn>
    struct SimNode_AotCMRES : SimNode_CallBase {
        const char * extFnName = nullptr;
        __forceinline SimNode_AotCMRES ( ) : SimNode_CallBase(LineInfo()) {}
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            using FunctionTrait = function_traits<FuncT>;
            using Result = typename FunctionTrait::return_type;
            using Arguments = typename FunctionTrait::arguments;
            const int nargs = tuple_size<Arguments>::value;
            using Indices = make_index_sequence<nargs>;
            vec4f * aa = context.abiArg;
            vec4f stub[1] = { v_zero() };
            if ( !aa ) { context.abiArg = stub; }
            using ResultValue = typename remove_const<Result>::type;
            *((ResultValue *)context.abiCMRES) = ImplAotStaticFunctionCMRES<Result>::template
                call<FuncT,Arguments>(*fn, context, Indices());
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
        StringBuilderWriter writer(__context__->stringHeap);
        writer << x;
        auto pStr = writer.c_str();
        if ( !pStr ) {
            __context__->throw_error("can't allocate string builder result, out of heap");
        }
        return pStr;
    }

    __forceinline char * das_string_builder ( Context * __context__, const SimNode_AotInteropBase & node ) {
        StringBuilderWriter writer(__context__->stringHeap);
        DebugDataWalker<StringBuilderWriter> walker(writer, PrintFlags::string_builder);
        for ( int i = 0; i!=node.nArguments; ++i ) {
            walker.walk(node.argumentValues[i], node.types[i]);
        }
        auto pStr = writer.c_str();
        if ( !pStr ) {
            __context__->throw_error("can't allocate string builder result, out of heap");
        }
        return pStr;
    }

    struct das_stack_prologue {
        __forceinline das_stack_prologue ( Context * __context__, uint32_t stackSize, const char * fn, int32_t line )
            : context(__context__) {
            if (!context->stack.push(stackSize, EP, SP)) {
                context->throw_error("stack overflow");
            }
#if DAS_ENABLE_STACK_WALK
            Prologue * pp = (Prologue *)context->stack.sp();
            pp->fileName = fn;
            pp->info = nullptr;         // FunctionDebugInfo
            pp->line = line;
#endif
        }
        __forceinline ~das_stack_prologue () {
            context->stack.pop(EP, SP);
        }
        Context * context;
        char * EP, * SP;
    };

    template <typename resType, typename ...argType>
    struct das_make_block : Block, SimNode_ClosureBlock {
        typedef function < resType ( argType... ) > BlockFn;
        __forceinline das_make_block ( Context * context, uint32_t argStackTop, uint64_t ann, BlockFn && func )
                : SimNode_ClosureBlock(LineInfo(),false,false,ann), blockFunction(func) {
            stackOffset = context->stack.spi();
            argumentsOffset = argStackTop ? (context->stack.spi() + argStackTop) : 0;
            body = this;
            functionArguments = context->abiArguments();
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
    struct das_make_block<resType> : Block, SimNode_ClosureBlock {
        typedef function < resType () > BlockFn;
        __forceinline das_make_block ( Context * context, uint32_t argStackTop, uint64_t ann, BlockFn && func )
            : SimNode_ClosureBlock(LineInfo(),false,false,ann), blockFunction(func) {
            stackOffset = context->stack.spi();
            argumentsOffset = argStackTop ? (context->stack.spi() + argStackTop) : 0;
            body = this;
            functionArguments = context->abiArguments();
        };
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            return cast<resType>::from(blockFunction());
        }
        BlockFn blockFunction;
    };

    template <>
    struct das_make_block<void> : Block, SimNode_ClosureBlock {
        typedef function < void () > BlockFn;
        __forceinline das_make_block ( Context * context, uint32_t argStackTop, uint64_t ann, BlockFn && func )
            : SimNode_ClosureBlock(LineInfo(),false,false,ann), blockFunction(func) {
            stackOffset = context->stack.spi();
            argumentsOffset = argStackTop ? (context->stack.spi() + argStackTop) : 0;
            body = this;
            functionArguments = context->abiArguments();
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
    struct das_make_block<void,argType...> : Block, SimNode_ClosureBlock {
        typedef function < void ( argType... ) > BlockFn;
        __forceinline das_make_block ( Context * context, uint32_t argStackTop, uint64_t ann, BlockFn && func )
                : SimNode_ClosureBlock(LineInfo(),false,false,ann), blockFunction(func) {
            stackOffset = context->stack.spi();
            argumentsOffset = argStackTop ? (context->stack.spi() + argStackTop) : 0;
            body = this;
            functionArguments = context->abiArguments();
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
    struct das_make_block_cmres : Block, SimNode_ClosureBlock {
        typedef function < resType ( argType... ) > BlockFn;
        __forceinline das_make_block_cmres ( Context * context, uint32_t argStackTop, uint64_t ann, BlockFn && func )
            : SimNode_ClosureBlock(LineInfo(),false,false,ann), blockFunction(func) {
            stackOffset = context->stack.spi();
            argumentsOffset = argStackTop ? (context->stack.spi() + argStackTop) : 0;
            body = this;
            functionArguments = context->abiArguments();
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
    struct das_make_block_cmres<resType> : Block, SimNode_ClosureBlock {
        typedef function < resType () > BlockFn;
        __forceinline das_make_block_cmres ( Context * context, uint32_t argStackTop, uint64_t ann, BlockFn && func )
            : SimNode_ClosureBlock(LineInfo(),false,false,ann), blockFunction(func) {
            stackOffset = context->stack.spi();
            argumentsOffset = argStackTop ? (context->stack.spi() + argStackTop) : 0;
            body = this;
            functionArguments = context->abiArguments();
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

    template <typename ResType>
    struct das_invoke {
        // vector cast
        static __forceinline ResType invoke ( Context * __context__, const Block & blk ) {
            vec4f result = __context__->invoke(blk, nullptr, nullptr);
            return cast<ResType>::to(result);
        }
        template <typename ...ArgType>
        static __forceinline ResType invoke ( Context * __context__, const Block & blk, ArgType ...arg ) {
            vec4f arguments [] = { cast<ArgType>::from(arg)... };
            vec4f result = __context__->invoke(blk, arguments, nullptr);
            return cast<ResType>::to(result);
        }
        // cmres
        static __forceinline ResType invoke_cmres ( Context * __context__, const Block & blk ) {
            typename remove_const<ResType>::type result;
            __context__->invoke(blk, nullptr, &result);
            return result;
        }
        template <typename ...ArgType>
        static __forceinline ResType invoke_cmres ( Context * __context__, const Block & blk, ArgType ...arg ) {
            vec4f arguments [] = { cast<ArgType>::from(arg)... };
            typename remove_const<ResType>::type result;
            __context__->invoke(blk, arguments, &result);
            return result;
        }
        template <typename BLK, typename ...ArgType>
        static __forceinline ResType invoke_cmres ( Context *, const BLK & blk, ArgType ...arg ) {
            return blk(arg...);
        }
    };

    template <>
    struct das_invoke<void> {
        static __forceinline void invoke ( Context * __context__, const Block & blk ) {
            __context__->invoke(blk, nullptr, nullptr);
        }
        template <typename ...ArgType>
        static __forceinline void invoke ( Context * __context__, const Block & blk, ArgType ...arg ) {
            vec4f arguments [] = { cast<ArgType>::from(arg)... };
            __context__->invoke(blk, arguments, nullptr);
        }
        template <typename BLK, typename ...ArgType>
        static __forceinline void invoke ( Context *, const BLK & blk, ArgType ...arg ) {
            return blk(arg...);
        }
    };

    template <typename ResType>
    struct das_invoke_function {
        static __forceinline ResType invoke ( Context * __context__, const Func & blk ) {
            SimFunction * simFunc = __context__->getFunction(blk.index-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            vec4f result = __context__->callOrFastcall(simFunc, nullptr, 0);
            return cast<ResType>::to(result);
        }
        template <typename ...ArgType>
        static __forceinline ResType invoke ( Context * __context__, const Func & blk, ArgType ...arg ) {
            vec4f arguments [] = { cast<ArgType>::from(arg)... };
            SimFunction * simFunc = __context__->getFunction(blk.index-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            vec4f result = __context__->callOrFastcall(simFunc, arguments, 0);
            return cast<ResType>::to(result);
        }
        static __forceinline ResType invoke_cmres ( Context * __context__, const Func & blk ) {
            SimFunction * simFunc = __context__->getFunction(blk.index-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            typename remove_const<ResType>::type result;
            __context__->callWithCopyOnReturn(simFunc, nullptr, &result, 0);
            return result;
        }
        template <typename ...ArgType>
        static __forceinline ResType invoke_cmres ( Context * __context__, const Func & blk, ArgType ...arg ) {
            vec4f arguments [] = { cast<ArgType>::from(arg)... };
            SimFunction * simFunc = __context__->getFunction(blk.index-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            typename remove_const<ResType>::type result;
            __context__->callWithCopyOnReturn(simFunc, arguments, &result, 0);
            return result;
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
            vec4f arguments[1];
            arguments[0] = cast<Lambda>::from(blk);
            vec4f result = __context__->callOrFastcall(simFunc, arguments, 0);
            return cast<ResType>::to(result);
        }
        template <typename ...ArgType>
        static __forceinline ResType invoke ( Context * __context__, const Lambda & blk, ArgType ...arg ) {
            vec4f arguments [] = { cast<void *>::from(blk.capture), (cast<ArgType>::from(arg))... };
            int32_t * fnIndex = (int32_t *)blk.capture;
            if (!fnIndex) __context__->throw_error("invoke null lambda");
            SimFunction * simFunc = __context__->getFunction(*fnIndex-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            vec4f result = __context__->callOrFastcall(simFunc, arguments, 0);
            return cast<ResType>::to(result);
        }
        static __forceinline ResType invoke_cmres ( Context * __context__, const Lambda & blk ) {
            int32_t * fnIndex = (int32_t *)blk.capture;
            if (!fnIndex) __context__->throw_error("invoke null lambda");
            SimFunction * simFunc = __context__->getFunction(*fnIndex-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            typename remove_const<ResType>::type result;
            vec4f arguments[1];
            arguments[0] = cast<Lambda>::from(blk);
            __context__->callWithCopyOnReturn(simFunc, arguments, &result, 0);
            return result;
        }
        template <typename ...ArgType>
        static __forceinline ResType invoke_cmres ( Context * __context__, const Lambda & blk, ArgType ...arg ) {
            vec4f arguments [] = { cast<void *>::from(blk.capture), (cast<ArgType>::from(arg))... };
            int32_t * fnIndex = (int32_t *)blk.capture;
            if (!fnIndex) __context__->throw_error("invoke null lambda");
            SimFunction * simFunc = __context__->getFunction(*fnIndex-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            typename remove_const<ResType>::type result;
            __context__->callWithCopyOnReturn(simFunc, arguments, &result, 0);
            return result;
        }
    };

    template <>
    struct das_invoke_lambda<void> {
        static __forceinline void invoke ( Context * __context__, const Lambda & blk ) {
            int32_t * fnIndex = (int32_t *)blk.capture;
            if (!fnIndex) __context__->throw_error("invoke null lambda");
            SimFunction * simFunc = __context__->getFunction(*fnIndex-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            vec4f arguments[1];
            arguments[0] = cast<Lambda>::from(blk);
            __context__->callOrFastcall(simFunc, arguments, 0);
        }
        template <typename ...ArgType>
        static __forceinline void invoke ( Context * __context__, const Lambda & blk, ArgType ...arg ) {
            vec4f arguments [] = { cast<void *>::from(blk.capture), (cast<ArgType>::from(arg))... };
            int32_t * fnIndex = (int32_t *)blk.capture;
            if (!fnIndex) __context__->throw_error("invoke null lambda");
            SimFunction * simFunc = __context__->getFunction(*fnIndex-1);
            if (!simFunc) __context__->throw_error("invoke null function");
            __context__->callOrFastcall(simFunc, arguments, 0);
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
            __context__->abiArg = aa;
            __context__->abiCMRES = acm;
            __context__->stack.pop(EP,SP);
            __context__->stopFlags = 0;
            __context__->exception = nullptr;
            catch_block();
        }
#else
        jmp_buf ev;
        jmp_buf * JB = __context__->throwBuf;
        __context__->throwBuf = &ev;
        if ( !setjmp(ev) ) {
            try_block();
        } else {
            __context__->throwBuf = JB;
            __context__->abiArg = aa;
            __context__->abiCMRES = acm;
            __context__->stack.pop(EP,SP);
            __context__->stopFlags = 0;
            __context__->exception = nullptr;
            catch_block();
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
        __forceinline ~das_iterator() {
            // delete that;
            // that = nullptr;
        }
        template <typename TT>
        __forceinline bool first ( Context * context, TT & i ) {
            return that->first(*context, (char *)&i);
        }
        template <typename TT>
        __forceinline bool next  ( Context * context, TT & i ) {
            return that->next(*context,(char *)&i);
        }
        template <typename TT>
        __forceinline void close ( Context * context, TT & i ) {
            that->close(*context,(char *)&i);
        }
        Iterator * that = nullptr;
    };

    template <>
    struct das_iterator <Sequence const> : das_iterator<Sequence> {
        __forceinline das_iterator(const Sequence & r)
            : das_iterator<Sequence>(r) {
        }
    };


    template <typename TK, typename TV, typename TKey>
    TV * __builtin_table_find ( Context * context, const TTable<TK, TV> & tab, TKey _key ) {
        TK key = (TK) _key;
        auto hfn = hash_function(*context, key);
        TableHash<TK> thh(context,sizeof(TV));
        int index = thh.find(tab, key, hfn);
        return (TV *) ( index!=-1 ? tab.data + index * sizeof(TV) : nullptr );
    }

    template <typename TK, typename TV, typename TKey>
    bool __builtin_table_key_exists ( Context * context, const TTable<TK, TV> & tab, TKey _key ) {
        TK key = (TK) _key;
        auto hfn = hash_function(*context, key);
        TableHash<TK> thh(context, sizeof(TV));
        return thh.find(tab, key, hfn) != -1;
    }

    template <typename TK, typename TV, typename TKey>
    bool __builtin_table_erase ( Context * context, TTable<TK,TV> & tab, TKey _key ) {
        if ( tab.lock ) context->throw_error("can't erase from locked table");
        TK key = (TK) _key;
        auto hfn = hash_function(*context, key);
        TableHash<TK> thh(context,sizeof(TV));
        return thh.erase(tab, key, hfn) != -1;
    }
}

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
