options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers = true

module soa private

//! Structure-of-Arrays (SOA) macro.
//!
//! Annotate a struct with ``[soa]`` to generate a parallel SOA layout:
//! every field becomes an array, and ``[]`` indexing returns an ``SOA_INDEX``
//! proxy that redirects field access (``soa[i].field`` becomes ``soa.field[i]``).
//! Also generates ``length``, ``push``, ``emplace``, ``push_clone``, and ``erase``.

require daslib/ast_boost
require daslib/templates_boost
require daslib/strings_boost

// replacing a[index].b with a.b[index]

struct public SOA_INDEX {
    //! Proxy type returned by the ``[]`` operator on an SOA structure.
    //! Field access on this proxy is rewritten by `SoaCallMacro` to index into the correct column array.
}

[tag_function(soa_call)]
def public operator .(src : SOA_INDEX explicit; field : string) {
    //! Field access operator for SOA_INDEX; rewritten by SoaCallMacro to convert soa[index].field into soa.field[index].
    // its important that this operator returns `void`, because it would prevent the compiler from cascading infer of wrong type
    assert(false, "this code should never been instanced. soa_call macro replaces the soa[index].field with soa.field[index]")
}

[tag_function_macro(tag="soa_call")]
class SoaCallMacro : AstFunctionAnnotation {
    //! Rewrites ``soa[index].field`` into ``soa.field[index]`` at compile time.
    //! This is the core of the SOA access pattern — it transforms AOS-style element access
    //! into column-wise array indexing for better cache locality.
    def override transform(var expr : smart_ptr<ExprCallFunc>; var errors : das_string) : ExpressionPtr {
        if (is_in_completion()) {
            return <- default<ExpressionPtr>
        }
        if (expr.arguments |> length != 2) {
            errors := "expecting soa[index].field"
            return <- default<ExpressionPtr>
        }
        if (!(expr.arguments[0] is ExprCall)) {
            errors := "expecting soa[index] as first argument, got {expr.__rtti}"
            return <- default<ExpressionPtr>
        }
        let idx_call = expr.arguments[0] as ExprCall
        if (idx_call.name != "[]" || idx_call.arguments |> length != 2) {
            errors := "expecting soa[index] as first argument, got {describe(expr.arguments[1])}"
            return <- default<ExpressionPtr>
        }
        if (!(expr.arguments[1] is ExprConstString)) {
            errors := "expecting field"
            return <- default<ExpressionPtr>
        }
        let fld = expr.arguments[1] as ExprConstString
        var inscope e_field <- new ExprField(at = expr.at,
            value <- clone_expression(idx_call.arguments[0]),
            name := fld.value,
            fieldFlags = ExprFieldFieldFlags.no_promotion
        )
        var inscope e_at <- new ExprAt(at = expr.at,
            subexpr <- e_field,
            index <- clone_expression(idx_call.arguments[1]),
            atFlags = ExprAtFlags.no_promotion
        )
        return <- e_at
    }
}

[structure_macro(name=soa)]
class SoaStructMacro : AstStructureAnnotation {
    //! Generates a Structure-of-Arrays layout from a regular struct.
    //! For a struct ``Foo`` with fields ``x : float`` and ``y : float``, generates:
    //!
    //!   * ``Foo`SOA`` — struct where every field is ``array<FieldType>``
    //!   * ``operator []`` returning ``SOA_INDEX`` proxy
    //!   * ``length``, ``push``, ``emplace``, ``push_clone``, ``erase`` functions
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        if (is_in_completion()) {
            var inscope tdef <- new TypeDecl(at = st.at,
                baseType = Type.tArray,
                alias := "{st.name}`SOA",
                firstType <- new TypeDecl(at = st.at,
                    baseType = Type.tStructure,
                    structType = get_ptr(st)
                )
            )
            compiling_module() |> add_alias(tdef)
            return true
        }
        if (st.fields |> length == 0) {
            errors := "soa macro expects at least one field"
            return false
        }
        // soa
        var inscope stype <- clone_structure(get_ptr(st))
        stype.name := "{stype.name}`SOA"
        for (fld in stype.fields) {
            move_new(fld._type) <| make_field_type(fld)
        }
        // [] operator
        var inscope idx_op <- make_index_op(st, stype)
        compiling_module() |> add_function(idx_op)
        // length()
        var inscope length_op <- make_length(st, stype)
        compiling_module() |> add_function(length_op)
        // push
        var inscope push_op <- make_named_call(st, stype, "push", "push", true)
        compiling_module() |> add_generic(push_op)
        // push_clone
        var inscope push_clone_op <- make_named_call(st, stype, "push_clone", "push_clone", true)
        compiling_module() |> add_function(push_clone_op)
        // emplace
        var inscope emplace_op <- make_named_call(st, stype, "emplace", "emplace", false)
        compiling_module() |> add_function(emplace_op)
        // erase
        var inscope erase_op <- make_erase(st, stype)
        compiling_module() |> add_function(erase_op)
        // now types
        compiling_module() |> add_structure(stype)
        return true
    }
    def make_field_type(fld : FieldDeclaration) : TypeDeclPtr {
        return <- new TypeDecl(at = fld.at,
                        baseType = Type.tArray,
                        firstType <- clone_type(fld._type)
                    )
    }
    def make_index_op(st : StructurePtr; var stype : StructurePtr) : FunctionPtr {
        var inscope idx_op <- new Function(at = st.at, atDecl = st.at, name := "[]")
        idx_op.flags |= FunctionFlags.generated
        move_new(idx_op.result) <| new TypeDecl(baseType = Type.autoinfer, at = st.at)
        var inscope idx_body <- quote() <| $() {
            return SOA_INDEX()
        }
        var inscope qres <- move_unquote_block(idx_body)
        qres.blockFlags &= ~ExprBlockFlags.isClosure
        move(idx_op.body) <| qres
        idx_op.arguments |> emplace_new <| new Variable(
            name := "st",
            at = st.at,
            _type <- new TypeDecl(baseType = Type.tStructure, at = st.at, structType = get_ptr(stype), flags = TypeDeclFlags.constant)
        )
        idx_op.arguments |> emplace_new <| new Variable(
            name := "index",
            at = st.at,
            _type <- new TypeDecl(baseType = Type.tInt, at = st.at, flags = TypeDeclFlags.constant)
        )
        return <- idx_op
    }
    def make_length(st : StructurePtr; var stype : StructurePtr) : FunctionPtr {
        var inscope length_op <- new Function(at = st.at, atDecl = st.at, name := "length")
        length_op.flags |= FunctionFlags.generated
        move_new(length_op.result) <| new TypeDecl(baseType = Type.tInt, at = st.at)
        var inscope length_body <- new ExprBlock(at = st.at)
        var inscope length_call <- new ExprCall(at = st.at, name := "length")
        length_call.arguments |> emplace_new <| new ExprField(at = st.at,
            value <- new ExprVar(at = st.at, name := "st"),
            name := st.fields[0].name,
            fieldFlags = ExprFieldFieldFlags.no_promotion
        )
        length_body.list |> emplace_new <| new ExprReturn(at = st.at,
            subexpr <- length_call
        )
        length_op.arguments |> emplace_new <| new Variable(
            name := "st",
            at = st.at,
            _type <- new TypeDecl(baseType = Type.tStructure, at = st.at, structType = get_ptr(stype), flags = TypeDeclFlags.constant)
        )
        move(length_op.body) <| length_body
        return <- length_op
    }
    def make_any_named_call(st : StructurePtr; var stype : StructurePtr; var argT : TypeDeclPtr&; fnName, callName : string; argField : bool) : FunctionPtr {
        var inscope push_op <- new Function(at = st.at, atDecl = st.at, name := fnName)
        push_op.flags |= FunctionFlags.generated
        move_new(push_op.result) <| new TypeDecl(baseType = Type.tVoid, at = st.at)
        var inscope push_body <- new ExprBlock(at = st.at)
        for (fld in st.fields) {
            if (true) {
                var inscope push_call <- new ExprCall(at = st.at, name := callName)
                push_call.arguments |> emplace_new <| new ExprField(at = st.at,
                    value <- new ExprVar(at = st.at, name := "st"),
                    name := fld.name,
                    fieldFlags = ExprFieldFieldFlags.no_promotion
                )
                if (argField) {
                    push_call.arguments |> emplace_new <| new ExprField(at = st.at,
                        value <- new ExprVar(at = st.at, name := "arg"),
                        name := fld.name,
                        fieldFlags = ExprFieldFieldFlags.no_promotion
                    )
                } else {
                    push_call.arguments |> emplace_new <| new ExprVar(at = st.at, name := "arg")
                }
                push_body.list |> emplace(push_call)
            }
        }
        push_op.arguments |> emplace_new <| new Variable(
            name := "st",
            at = st.at,
            _type <- new TypeDecl(baseType = Type.tStructure, at = st.at, structType = get_ptr(stype))
        )
        push_op.arguments |> emplace_new <| new Variable(
            name := "arg",
            at = st.at,
            _type <- argT
        )
        move(push_op.body) <| push_body
        return <- push_op
    }
    def make_erase(st : StructurePtr; var stype : StructurePtr) {
        var inscope argT <- new TypeDecl(baseType = Type.tInt, at = st.at)
        return <- make_any_named_call(st, stype, argT, "erase", "erase", false)
    }
    def make_named_call(var st : StructurePtr; var stype : StructurePtr; fnName, callName : string; isConst : bool = false) : FunctionPtr {
        var inscope argT <- new TypeDecl(baseType = Type.tStructure, at = st.at, structType = get_ptr(st), flags = isConst ? TypeDeclFlags.constant : bitfield(0))
        return <- make_any_named_call(st, stype, argT, fnName, callName, true)
    }
}

class CollectAndReplaceIteratorFields : AstVisitor {
    //! AST visitor that collects and replaces SOA iterator field access.
    it_prefix : string
    names : table<string; bool>
    def CollectAndReplaceIteratorFields(prefix : string) {
        it_prefix = prefix
    }
    def override visitExprField(var expr : smart_ptr<ExprField>) : ExpressionPtr {
        if (expr.value is ExprVar) {
            if ((expr.value as ExprVar).name == it_prefix) {
                let ename = string(expr.name)
                names |> insert(ename, true)
                return <- new ExprVar(at = expr.at, name := it_prefix + "`" + ename)
            }
        }
        return <- expr
    }
}

def collect_and_replace_iterator_fields(prefix : string; blk : ExpressionPtr) : array<string> {
    var names : array<string>
    var astVisitor = new CollectAndReplaceIteratorFields(prefix)
    var inscope astVisitorAdapter <- make_visitor(*astVisitor)
    visit(blk, astVisitorAdapter)
    for (n in keys(astVisitor.names)) {
        names |> push(n)
    }
    unsafe {
        delete astVisitor
    }
    return <- names
}

[for_loop_macro(name=soa_for_loop)]
class SoaForLoop : AstForLoopMacro {
    //! For-loop macro that transforms SOA array iteration.
    def override visitExprFor(prog : ProgramPtr; mod : Module?; expr : smart_ptr<ExprFor>) : ExpressionPtr {
        if (is_in_completion()) {
            return <- default<ExpressionPtr>
        }
        var soa_index = -1
        for (index, its in count(), expr.sources) {
            if (its._type != null && its._type.isStructure) {
                var is_soa = false
                peek(its._type.structType.name) <| $(sn) {
                    if (sn |> ends_with("`SOA")) {
                        is_soa = true
                    }
                }
                if (is_soa) {
                    soa_index = index
                    break
                }
            }
        }
        if (soa_index != -1) {
            let it_prefix = string(expr.iterators[soa_index])
            var inscope new_for_e <- clone_expression(expr)
            var new_for = new_for_e as ExprFor
            new_for.sources |> erase(soa_index)
            new_for.iterators |> erase(soa_index)
            new_for.iteratorsAt |> erase(soa_index)
            new_for.iteratorsAka |> erase(soa_index)
            new_for.iteratorsTags |> erase(soa_index)
            var names <- collect_and_replace_iterator_fields(it_prefix, new_for.body)
            if (names |> length == 0) {
                names |> push <| string(expr.sources[soa_index]._type.structType.fields[0].name)
            }
            for (itn in names) {
                let si = new_for.iterators |> length            // todo: add 'push'
                new_for.iterators |> resize(si + 1)
                new_for.iterators[si] := "{it_prefix}`{itn}"
                new_for.iteratorsAka |> resize(si + 1)
                new_for.iteratorsAka[si] := ""
                new_for.iteratorsAt |> push(expr.iteratorsAt[soa_index])
                new_for.iteratorsTags |> emplace_new <| clone_expression(expr.iteratorsTags[soa_index])
                new_for.sources |> emplace_new <| new ExprField(at = expr.sources[soa_index].at,
                    value <- clone_expression(expr.sources[soa_index]),
                    name := itn,
                    fieldFlags = ExprFieldFieldFlags.no_promotion
                )
            }
            new_for.iteratorVariables |> clear()
            return <- new_for_e
        }
        return <- default<ExpressionPtr>
    }
}

