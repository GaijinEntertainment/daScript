options indenting = 4
options no_aot = true

module is_local shared

require ast public
require rtti
require daslib/ast_boost

def is_local_expr ( expr:ExpressionPtr )
    if expr is ExprVar
        unsafe
            let ev = reinterpret<ExprVar?>(expr)
            return ev.varFlags.local
    elif expr is ExprAt
        unsafe
            let ea = reinterpret<ExprAt?>(expr)
            if ea.subexpr._type!=null && ea.subexpr._type.dim.length!=0
                return is_local_expr(ea.subexpr)
    elif expr is ExprField
        unsafe
            let ef = reinterpret<ExprField?>(expr)
            if !(ef.value._type.baseType==Type tHandle) || (ef.value._type.isLocal)
                return is_local_expr(ef.value)
    return false

def is_local_or_global_expr ( expr:ExpressionPtr )
    if expr is ExprVar
        unsafe
            let ev = reinterpret<ExprVar?>(expr)
            return ev.varFlags.local || !(ev.varFlags.argument || ev.varFlags._block)
    elif expr is ExprAt
        unsafe
            let ea = reinterpret<ExprAt?>(expr)
            if ea.subexpr._type!=null && ea.subexpr._type.dim.length!=0
                return is_local_or_global_expr(ea.subexpr)
    elif expr is ExprField
        unsafe
            let ef = reinterpret<ExprField?>(expr)
            if !(ef.value._type.baseType==Type tHandle) || (ef.value._type.isLocal)
                return is_local_or_global_expr(ef.value)
    return false

def is_scope_expr ( expr:ExpressionPtr )
    if expr is ExprVar
        return true
    elif expr is ExprAt
        unsafe
            let ea = reinterpret<ExprAt?>(expr)
            if ea.subexpr._type!=null && ea.subexpr._type.dim.length!=0
                return is_scope_expr(ea.subexpr)
    elif expr is ExprField
        unsafe
            let ef = reinterpret<ExprField?>(expr)
            if !(ef.value._type.baseType==Type tHandle) || (ef.value._type.isLocal)
                return is_scope_expr(ef.value)
    return false
