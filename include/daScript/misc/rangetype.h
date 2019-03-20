#pragma once

namespace das
{
    template <typename TT>
    struct RangeType {
        TT  from;   //  [from,to) range
        TT  to;

        RangeType() {}
        RangeType( TT f, TT t ) : from(f), to(t) {}

        template <typename AP>
        __forceinline friend StringWriter<AP> & operator<< (StringWriter<AP> & stream, const RangeType<TT> & vec) {
            stream << vec.from << "," << vec.to;
            return stream;
        }
        __forceinline bool operator == ( const RangeType<TT> & vec ) const {
            return from==vec.from && to==vec.to;
        }
    };

    typedef RangeType<int32_t>  range;
    typedef RangeType<uint32_t> urange;
}

