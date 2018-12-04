#pragma once

#include "simulate.h"
#include "arraytype.h"

namespace yzg
{
    void array_reserve ( Context & context, Array & arr, uint32_t newCapacity );
    void array_resize ( Context & context, Array & arr, uint32_t newSize );
    
    // PUSH VALUE
    template <typename TT>
    struct SimNode_ArrayPushValue : SimNode {
        SimNode_ArrayPushValue(const LineInfo & at, SimNode * ll, SimNode * rr)
            : SimNode(at), l(ll), r(rr) {};
        virtual __m128 eval ( Context & context ) override {
            __m128 ll = l->eval(context);
            YZG_EXCEPTION_POINT;
            __m128 rr = r->eval(context);
            YZG_EXCEPTION_POINT;
            Array * pA = cast<Array *>::to(ll);
            if ( !pA->fieldSize ) pA->fieldSize = sizeof(TT);
            array_resize(context, *pA, pA->size + 1);
            TT * pl = (TT *) ( pA->data + (pA->size-1)*pA->fieldSize );
            TT * pr = (TT *) &rr;
            *pl = *pr;
            return ll;
        }
        SimNode * l, * r;
    };
    
    // PUSH REFERENCE VALUE
    struct SimNode_ArrayPushRefValue : SimNode {
        SimNode_ArrayPushRefValue(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz)
            : SimNode(at), l(ll), r(rr), size(sz) {};
        virtual __m128 eval ( Context & context ) override {
            __m128 ll = l->eval(context);
            YZG_EXCEPTION_POINT;
            __m128 rr = r->eval(context);
            YZG_EXCEPTION_POINT;
            Array * pA = cast<Array *>::to(ll);
            if ( !pA->fieldSize ) pA->fieldSize = size;
            array_resize(context, *pA, pA->size + 1);
            void * pl = pA->data + (pA->size-1)*pA->fieldSize;
            auto pr = cast<void *>::to(rr);
            memcpy ( pl, pr, size );
            return ll;
        }
        SimNode * l, * r;
        uint32_t size;
    };
}
