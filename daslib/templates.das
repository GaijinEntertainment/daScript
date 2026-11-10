options gen2
options indenting = 4
options strict_smart_pointers

module templates shared private

//! Template and type-introspection macros.
//!
//! Provides ``decltype`` / ``decltype_noref`` macros for obtaining
//! compile-time ``TypeDecl`` values, and the ``[template]`` function
//! annotation for removing template-only arguments from instantiated
//! generic functions.
//! See also ``templates_boost`` for template substitution and code generation.

require daslib/ast
require daslib/rtti
require strings
require daslib/ast_boost

[call_macro(name="decltype")]
class DecltypeMacro : AstCallMacro {
    //! This macro returns ``ast::TypeDecl`` for the corresponding expression.
    remove_ref = false
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        if (expr.arguments |> length != 1) {
            macro_error(prog, expr.at, "decltype expecting 1 argument")
            return null
        }
        if (expr.arguments[0]._type == null || expr.arguments[0]._type.isAutoOrAlias) {
            macro_error(prog, expr.at, "decltype requires fully inferred type")
            return null
        }
        var tt = clone_type(expr.arguments[0]._type)
        if (remove_ref) {
            tt.flags.ref = false
        }
        var mkt = new ExprTypeDecl(at = expr.at,
            typeexpr <- tt
        )
        return <- mkt
    }
}

[call_macro(name="decltype_noref")]
class DecltypeNoRefMacro : DecltypeMacro {
    //! This macro returns TypeDecl for the corresponding expression, minus the ref (&) portion.
    override remove_ref = true
}

def private keep_aliases(t : TypeDeclPtr; var blk : ExprBlock?; var bound : table<string; bool>&;
                         erased : bool; const_under_dim : bool) {
    if (t == null) return
    if (!empty(t.alias) && t.baseType != Type.alias && t.baseType != Type.autoinfer
            && !key_exists(bound, string(t.alias))) {
        bound |> insert(string(t.alias), true)
        if (erased) {
            var tt = clone_type(t)
            if (const_under_dim) {
                tt.flags.constant = true
            }
            var carrier = new TypeDecl(at = blk.at, baseType = Type.tFunction,
                firstType <- new TypeDecl(at = blk.at, baseType = Type.tVoid))
            carrier.argTypes |> emplace(tt)
            var vlet = new ExprLet(at = blk.at, atInit = blk.at)
            vlet.variables |> emplace_new() <| new Variable(at = blk.at,
                name := "`alias`{t.alias}",
                _type <- carrier,
                flags = VariableFlags.generated | VariableFlags.can_shadow)
            blk.list |> emplace(vlet, 0)
        }
    }
    let under_dim = t.baseType == Type.tFixedArray && (t.flags.constant || const_under_dim)
    keep_aliases(t.firstType, blk, bound, erased, under_dim)
    keep_aliases(t.secondType, blk, bound, erased, false)
    for (at in t.argTypes) {
        keep_aliases(at, blk, bound, erased, false)
    }
}

def private keep_erased_aliases(src : Function?; erased : table<string; bool>;
                                var blk : ExprBlock?) {
    var inscope bound : table<string; bool>
    for (farg in src.arguments) {
        keep_aliases(farg._type, blk, bound, key_exists(erased, string(farg.name)), false)
    }
}

[function_macro(name="template")]
class TemplateMacro : AstFunctionAnnotation {
    //! This macro is used to remove unused (template) arguments from the instantiation of the generic function.
    def override transform(var expr : ExprCallFunc?; var errors : das_string) : ExpressionPtr {
        if (expr.func == null) {
            compiling_program() |> macro_error(expr.at, "expecting inferred function")
            return expr
        }
        let taidx = find_index_if(each(expr.func.annotations)) $(ann) {
            return ann.annotation.name == "template"
        }
        if (taidx == -1) return expr
        var ffunc = expr.func
        var newcall = unsafe(reinterpret<ExprCallFunc?>(clone_expression(expr)))
        var fclone = clone_function(ffunc)
        fclone.fromGeneric = ffunc
        fclone.flags.privateFunction = true
        newcall.func = null
        var inscope hashes : array<uint64>
        var inscope erased : table<string; bool>
        hashes |> reserve(length(fclone.annotations[taidx].arguments))
        for (t in fclone.annotations[taidx].arguments) {
            let argidx = find_index_if(each(fclone.arguments)) $(farg) {
                return farg.name == t.name
            }
            if (argidx == -1) {
                errors := "can't find template argument {t.name} (or duplicate)"
                return null
            }
            {
                var argExpr = newcall.arguments[argidx]
                if (argExpr is ExprRef2Value) {
                    argExpr = unsafe((newcall.arguments[argidx] as ExprRef2Value).subexpr)
                }
                let canStrip = t.basicType == Type.tBool ? t.bValue : true
                if (canStrip && !(argExpr is ExprTypeDecl)) {
                    errors := "template argument must be type<...> and not {describe(newcall.arguments[argidx])}. {newcall.arguments[argidx].__rtti}"
                    return null
                }
                let tname = describe_typedecl(argExpr._type, true, true, true)
                hashes |> push(hash(tname))
            }
            erased |> insert(string(t.name), true)
            fclone.arguments |> erase(argidx)
            newcall.arguments |> erase(argidx)
        }
        keep_erased_aliases(ffunc, erased, fclone.body as ExprBlock)
        fclone.annotations |> erase(taidx)
        let extra = build_string() $(var w) {
            for (h in hashes) {
                w |> write("`{h}")
            }
        }
        fclone.name := "`template{extra}`{fclone.name}"
        newcall.name := "_::{fclone.name}"
        compiling_module() |> add_function(fclone)
        return <- newcall
    }
}
