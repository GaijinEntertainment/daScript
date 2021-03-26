#pragma once

namespace das {
    template <typename TT, size_t maxSize>
    struct temp_vector {
        __forceinline TT * next() {
            DAS_VERIFY(total < maxSize);
            char * result = data + total * sizeof(TT);
            total += sizeof(TT);
            return (TT *) result;
        }
        __forceinline TT * begin() { return (TT *) data; }
        __forceinline const TT * begin() const { return (const TT *) data; }
        __forceinline TT * end() { return (TT *) (data + total*sizeof(TT)); }
        __forceinline const TT * end() const { return (const TT *) (data + total*sizeof(TT)); }
        __forceinline size_t size() const { return total; }
        __forceinline void push_back ( const TT & value ) { new (next()) TT(value); }
        char    data[sizeof(TT) * maxSize];
        size_t  total = 0;
    };
}
