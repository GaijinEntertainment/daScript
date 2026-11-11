#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/simulate_nodes.h"
#include "daScript/simulate/runtime_array.h"
#include "daScript/simulate/runtime_table_nodes.h"
#include "daScript/simulate/runtime_string_delete.h"
#include "daScript/simulate/debug_print.h"

namespace das
{
    vec4f SimNode_GoodArrayIterator::eval ( Context & context ) {
        DAS_PROFILE_NODE
        vec4f ll = source->eval(context);
        Array * arr = cast<Array *>::to(ll);
        char * iter = context.allocateIterator(sizeof(GoodArrayIterator),"array<> iterator", &debugInfo);
        new (iter) GoodArrayIterator(arr, stride, &debugInfo);
        return cast<char *>::from(iter);
    }

    vec4f SimNode_FixedArrayIterator::eval ( Context & context ) {
        DAS_PROFILE_NODE
        vec4f ll = source->eval(context);
        char * data = cast<char *>::to(ll);
        char * iter = context.allocateIterator(sizeof(FixedArrayIterator),"fixed array iterator", &debugInfo);
        new (iter) FixedArrayIterator(data, size, stride, &debugInfo);
        return cast<char *>::from(iter);
    }

    vec4f SimNode_DeleteArray::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto pArray = (Array *) subexpr->evalPtr(context);
        pArray = pArray + total - 1;
        for ( uint32_t i=0, is=total; i!=is; ++i, pArray-- ) {
            if ( pArray->data ) {
                if ( !pArray->isLocked() ) {
                    uint64_t oldSize = pArray->capacity * uint64_t(stride);
                    context.free(pArray->data, oldSize, &debugInfo);
                } else {
                    context.throw_error_at(debugInfo, "deleting locked array%s", errorMessage);
                    return v_zero();
                }
            }
            memset ( pArray, 0, sizeof(Array) );
        }
        return v_zero();
    }
    vec4f SimNode_DeleteTable::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto pTable = (Table *) subexpr->evalPtr(context);
        pTable = pTable + total - 1;
        for ( uint32_t i=0, is=total; i!=is; ++i, pTable-- ) {
            if ( pTable->data ) {
                if ( !pTable->isLocked() ) {
                    context.free(pTable->data, tableAllocBytes(*pTable), &debugInfo);
                } else {
                    context.throw_error_at(debugInfo, "deleting locked table%s", errorMessage);
                    return v_zero();
                }
            }
            memset ( pTable, 0, sizeof(Table) );
        }
        return v_zero();
    }
    vec4f SimNode_StringBuilder::eval ( Context & context ) {
        DAS_PROFILE_NODE
        vec4f * argValues = (vec4f *)(alloca(nArguments * sizeof(vec4f)));
        evalArgs(context, argValues);
        StringBuilderWriter writer;
        DebugDataWalker<StringBuilderWriter> walker(writer, PrintFlags::string_builder);
        for ( int i=0, is=nArguments; i!=is; ++i ) {
            walker.walk(argValues[i], types[i]);
        }
        uint64_t length = writer.tellp();
        if ( length ) {
            auto pStr = context.allocateString(writer.c_str(), uint32_t(length), &debugInfo, isTempString);
            if ( isTempString ) context.freeTempString(pStr, &debugInfo);
            return cast<char *>::from(pStr);
        } else {
            return v_zero();
        }
    }

    vec4f SimNode_StringIterator::eval ( Context & context ) {
        DAS_PROFILE_NODE
        vec4f ll = source->eval(context);
        char * str = cast<char *>::to(ll);
        char * iter = context.allocateIterator(sizeof(StringIterator),"string iterator", &debugInfo);
        new (iter) StringIterator(str, &debugInfo);
        return cast<char *>::from(iter);
    }
}
