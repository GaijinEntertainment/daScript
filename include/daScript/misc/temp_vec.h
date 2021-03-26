#pragma once

namespace das {
    // only raw pod
    template <typename TT, size_t maxSize>
    struct temp_vector {
        static_assert(std::is_pod<TT>::value,"temp_vector is only supported for pod types");
        using this_type = temp_vector<TT,maxSize>;
        temp_vector() = default;
        temp_vector ( const this_type & t ) { copy(t); }
        temp_vector ( this_type && t ) { copy(t); t.total=0; }
        this_type & operator = ( const this_type & t ) { copy(t); return *this; }
        this_type & operator = ( this_type && t ) { copy(t); t.total=0; return *this; }
        __forceinline TT * begin() { return _data; }
        __forceinline const TT * begin() const { return _data; }
        __forceinline TT * end() { return _data + total; }
        __forceinline const TT * end() const { return _data + total; }
        __forceinline size_t size() const { return total; }
        __forceinline TT & operator [] ( size_t index ) { return _data[index]; }
        __forceinline const TT & operator [] ( size_t index ) const { return _data[index]; }
        __forceinline void copy ( const this_type & t ) {
            memcpy(_data, t._data, t.total*sizeof(TT) );
            total = t.total;
        }
        __forceinline void resize ( size_t sz ) {
            DAS_ASSERT(sz < maxSize);
            total = sz;
        }
        __forceinline TT & back() {
            DAS_ASSERT(total > 0);
            return _data[total-1];
        }
        __forceinline const TT & back() const {
            DAS_ASSERT(total > 0);
            return _data[total-1];
        }
        __forceinline void push_back ( const TT & value ) {
            DAS_ASSERT(total < maxSize);
            _data[total++] = value;
        }
        __forceinline TT * erase ( TT * it ) {
            DAS_ASSERT(total > 0);
            *it = _data[--total];
            return it;
        }
        TT      _data[maxSize];
        size_t  total = 0;
    };
}
