#include "precomp.h"

#include "ast.h"
#include "runtime_array.h"
#include "runtime_table.h"
#include "ast_interop.h"

namespace yzg
{
    
    // core functions
    
    __m128 builtin_print ( Context & context, SimNode_Call *, __m128 * args ) {
        context.to_out(to_rts(args[0]));
        return _mm_setzero_ps();
    }
    
    __m128 builtin_breakpoint ( Context & context, SimNode_Call * call, __m128 * ) {
        context.breakPoint(call->debug.column, call->debug.line);
        return _mm_setzero_ps();
    }
    
    __m128 builtin_stackwalk ( Context & context, SimNode_Call *, __m128 * ) {
        context.stackWalk();
        return _mm_setzero_ps();
    }
    
    __m128 builtin_terminate ( Context & context, SimNode_Call *, __m128 * ) {
        context.stopFlags |= EvalFlags::stopForTerminate;
        return _mm_setzero_ps();
    }
    
    // array functions
    
    int builtin_array_size ( Array * arr ) {
        return arr->size;
    }
    
    int builtin_array_capacity ( Array * arr ) {
        return arr->capacity;
    }
    
    int builtin_table_size ( Table * arr ) {
        return arr->size;
    }
    
    int builtin_table_capacity ( Table * arr ) {
        return arr->capacity;
    }
    
    void Module_BuiltIn::addRuntime(ModuleLibrary & lib) {
        // functions
        addInterop<builtin_print,void,char *>   (*this, lib, "print");
        addInterop<builtin_terminate,void>      (*this, lib, "terminate");
        addInterop<builtin_breakpoint,void>     (*this, lib, "breakpoint");
        addInterop<builtin_stackwalk,void>      (*this, lib, "stackwalk");
        // function-like expresions
        addCall<ExprAssert>     ("assert");
        addCall<ExprDebug>      ("debug");
        addCall<ExprHash>       ("hash");
        // table functions
        addExtern<decltype(builtin_table_size), builtin_table_size>(*this, lib, "length");
        addExtern<decltype(builtin_table_capacity), builtin_table_capacity>(*this, lib, "capacity");
        // array functions
        addExtern<decltype(builtin_array_size), builtin_array_size>(*this, lib, "length");
        addExtern<decltype(builtin_array_capacity), builtin_array_capacity>(*this, lib, "capacity");
        // shared expressions
        addCall<ExprErase>("erase");
        addCall<ExprFind>("find");
        // table expressions
        addCall<ExprTableKeysOrValues<SimNode_TableIterator<TableKeysIterator>,true>>    ("keys");
        addCall<ExprTableKeysOrValues<SimNode_TableIterator<TableValuesIterator>,false>> ("values");
        // array expresisons
        addCall<ExprArrayPush>  ("push");
        addCall<ExprArrayCallWithSizeOrIndex<SimNode_ArrayResize>>  ("resize");
        addCall<ExprArrayCallWithSizeOrIndex<SimNode_ArrayReserve>> ("reserve");

    }
}
