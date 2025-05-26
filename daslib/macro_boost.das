options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers = true


module macro_boost shared private

require daslib/ast_boost
require daslib/templates_boost
require daslib/strings_boost

[macro_function, tag_function(macro_verify_tag)]
def public macro_verify(expr : bool; prog : ProgramPtr; at : LineInfo; message : string) {
    //! Same as verify, only the check will produce macro error, followed by return [[ExpressionPtr]]
    pass
}

[tag_function_macro(tag="macro_verify_tag")]
class MacroVerifyMacro : AstFunctionAnnotation {
    //! This macro convert macro_verify(expr,message,prog,at) to the following code::
    //!    if !expr
    //!        macro_error(prog,at,message)
    //!        return [[ExpressionPtr]]
    def override transform(var call : smart_ptr<ExprCallFunc>; var errors : das_string) : ExpressionPtr {
        // if ..
        var inscope res <- qmacro_block() <| $() {
            if (!$e(call.arguments[0])) {
                macro_error($e(call.arguments[1]), $e(call.arguments[2]), $e(call.arguments[3]))
                return <- default<ExpressionPtr>
            }
        }
        res |> force_at(call.at)
        return <- res
    }
}

[macro]
class CaptureBlock : AstVisitor {
    @do_not_delete scope : table<Variable?>
    @do_not_delete captured : table<Variable?; ExprVar?>
    def override preVisitExprVar(expr : smart_ptr<ExprVar>) : void {
        if (!(expr.varFlags.local || expr.varFlags.argument || expr.varFlags._block)) {
            return
        }
        let vptr = get_ptr(expr.variable)
        if (!(scope |> key_exists(vptr))) {
            unsafe {
                captured[vptr] = reinterpret<ExprVar?> get_ptr(expr)
            }
        }
    }
    def override preVisitExprLetVariable(expr : smart_ptr<ExprLet>; arg : VariablePtr; lastArg : bool) : void {
        scope |> insert(get_ptr(arg))
    }
    def override preVisitExprForVariable(expr : smart_ptr<ExprFor>; svar : VariablePtr; last : bool) : void {
        scope |> insert(get_ptr(svar))
    }
    def override preVisitExprBlockArgument(blk : smart_ptr<ExprBlock>; arg : VariablePtr; lastArg : bool) : void {
        scope |> insert(get_ptr(arg))
    }
}

struct public CapturedVariable {
    //! Stored captured variable together with the `ExprVar` which uses it
    variable : Variable?
    expression : ExprVar?
    eref : bool             //! this one indicates if its used by reference and does not come from argument. its only used in JIT
}

[macro_function]
def public capture_block(expr : ExpressionPtr) : array<CapturedVariable> {
    //! Collect all captured variables in the expression.
    var astVisitor = new CaptureBlock()
    var inscope adapter <- make_visitor(*astVisitor)
    visit(expr, adapter)
    var res <- [for (k, v in keys(astVisitor.captured), values(astVisitor.captured)); CapturedVariable(variable=k, expression=v) ]
    unsafe {
        delete astVisitor
    }
    return <- res
}

[macro]
class ColletFinally : AstVisitor {
    @do_not_delete blocks : table<ExprBlock?>
    alwaysFor : bool
    def override preVisitExprBlock(blk : smart_ptr<ExprBlock>) : void {
        if (length(blk.finalList) > 0) {
            blocks |> insert(get_ptr(blk))
        } elif (alwaysFor && blk.blockFlags.forLoop) {
            blocks |> insert(get_ptr(blk))
        }
    }
    def override canVisitMakeBlockBody(expr : smart_ptr<ExprMakeBlock>) : bool {
        return false
    }
}

[macro_function]
def public collect_finally(expr : ExpressionPtr; alwaysFor : bool = false) {
    //! Collect all finally blocks in the expression.
    //! Returns array of ExprBlock? with all the blocks which have `finally` section
    //! Does not go into 'make_block' expression, such as `lambda`, or 'block' expressions
    var astVisitor = new ColletFinally(alwaysFor = alwaysFor)
    var inscope adapter <- make_visitor(*astVisitor)
    visit(expr, adapter)
    var res <- [for (k in keys(astVisitor.blocks)); k ]
    unsafe {
        delete astVisitor
    }
    return <- res
}

[macro]
class ColletLabels : AstVisitor {
    labels : table<int>
    def override preVisitExprLabel(expr : smart_ptr<ExprLabel>) : void {
        labels |> insert(expr.labelName)
    }
    def override canVisitMakeBlockBody(expr : smart_ptr<ExprMakeBlock>) : bool {
        return false
    }
}

[macro_function]
def public collect_labels(expr : ExpressionPtr) {
    //! Collect all labels in the expression. Returns array of integer with label indices
    //! Does not go into 'make_block' expression, such as `lambda`, or 'block' expressions
    var astVisitor = new ColletLabels()
    var inscope adapter <- make_visitor(*astVisitor)
    visit(expr, adapter)
    var res <- [for (k in keys(astVisitor.labels)); k ]
    unsafe {
        delete astVisitor
    }
    return <- res
}

[call_macro(name="return_skip_lockcheck")]
class private ReturnSkipLockcheck : AstCallMacro {
    //! this is similar to regular return <-, but it does not check for locks
    def override visit(prog : ProgramPtr; mod : Module?; var call : smart_ptr<ExprCallMacro>) : ExpressionPtr {
        macro_verify(call.arguments |> length == 1, prog, call.at, "expecting return_skip_lockcheck(expr)")
        return <- new ExprReturn(
            at = call.at,
            subexpr <- clone_expression(call.arguments[0]),
            returnFlags = ExprReturnFlags.skipLockCheck | ExprReturnFlags.moveSemantics
        )
    }
}

[macro_function]
def public convert_vec(var quote2: Quote2; field_values, xtype, at) {
    unsafe {
        var vec_tp = get_handled_type_index_type_declaration(reinterpret<smart_ptr<TypeAnnotation>>(xtype.annotation),
                                                             reinterpret<smart_ptr<Expression>>(null),
                                                             reinterpret<smart_ptr<Expression>>(null));
        vec_tp.flags &= ~TypeDeclFlags.ref;
        var res_field <- ExpressionPtr();
        if (field_values |> length() > 0) {
            var args: array<ExpressionPtr>;
            for (val in field_values) {
                var arg_expr <- quote2.up_only_expr(reinterpret<uint8?>(val.get_ptr()), vec_tp.firstType, at);
                args |> emplace_new(new ExprAscend(subexpr <- arg_expr));
            }
            res_field <- new ExprCall(
                at=at,
                name:="to_array_move",
                arguments:=array<ExpressionPtr>(
                    new ExprMakeArray(
                        values:=args,
                        gen2=true,
                        makeType=clone_type(vec_tp),
                        at=at
                    )
                )
            );
        } else {
            res_field <- new ExprMakeStruct(at=at, makeType=new TypeDecl(baseType = Type.tArray, firstType=clone_type(vec_tp)));
        }
        return <- res_field;
    }
}


[macro_function]
def public convert_vec_basic(var quote2: Quote2; field_values, xtype, at) {
    unsafe {
        var vec_tp = get_handled_type_index_type_declaration(reinterpret<smart_ptr<TypeAnnotation>>(xtype.annotation),
                                                             reinterpret<smart_ptr<Expression>>(null),
                                                             reinterpret<smart_ptr<Expression>>(null));
        vec_tp.flags &= ~TypeDeclFlags.ref;
        var res_field <- ExpressionPtr();
        if (field_values |> length() > 0) {
            var args: array<ExpressionPtr>;
            for (val in field_values) {
                var arg_expr <- quote2.up_only_expr(reinterpret<uint8?>(addr(val)), vec_tp, at);
                args |> emplace_new(arg_expr);
            }
            res_field <- new ExprCall(
                at=at,
                name:="to_array_move",
                arguments:=array<ExpressionPtr>(
                    new ExprMakeArray(
                        values:=args,
                        gen2=true,
                        makeType=clone_type(vec_tp),
                        at=at
                    )
                )
            );
        } else {
            res_field <- new ExprMakeStruct(at=at, makeType=new TypeDecl(baseType = Type.tArray, firstType=clone_type(vec_tp)));
        }
        return <- res_field;
    }
}


[call_macro(name="quote2")]
class private Quote2 : AstCallMacro {
    rtti_module: Module?;
    idx: int;
    depth: int;
    def tabs() {
        return build_string() <| $(writer) { write_chars(writer, ' ', 4 * depth); }
    }

    def up_capture(var data: uint8 const?, type_name: string, at: LineInfo) {
        unsafe {
            var  annotation = unsafe(rtti_module |> module_find_annotation(type_name));
            assert(annotation != null);
            var inscope tp <- new TypeDecl(baseType = Type.tHandle,
                annotation = unsafe(reinterpret<TypeAnnotation?> annotation))
            assert(tp.annotation.isBasicStructureAnnotation);
            var inscope mkStr <- new ExprMakeStruct(makeType<-clone_type(tp))
            var inscope mkS1 <- new MakeStruct(uninitialized)
            let ann = reinterpret<BasicStructureAnnotation?>(tp.annotation);
            for_each_field(*ann) <| $(name, cppName, xtype, offset) {
                var fdata = data + int(offset);
                let tstr = describe(xtype)
                var fld1 : ExpressionPtr;
                if (xtype.baseType == Type.tHandle) {
                    fld1 <- up_capture(fdata, tstr, at);
                } else {
                    fld1 <- walk_and_convert(fdata, xtype, at);
                }
                var inscope mkFld <- new MakeFieldDecl(name := name, value <- fld1, at=any_loc());
                if (!xtype.canCopy) {
                    mkFld.flags = MakeFieldDeclFlags.moveSemantics
                }
                emplace(*mkS1, mkFld);
            }
            emplace(mkStr.structs, mkS1)
            mkStr.makeStructFlags |= ExprMakeStructFlags.useInitializer;
            return <- mkStr;
        }
    }

    def up_only_expr(var data: uint8 const?; info : TypeDeclPtr; at: LineInfo) : ExpressionPtr {
        unsafe {
            if (describe(info) == "ast::Expression") {
                var annotation = unsafe(rtti_module |> module_find_annotation((reinterpret<ExpressionPtr>(data)).__rtti));
                var inscope tp <- new TypeDecl(baseType = Type.tHandle,
                    annotation = unsafe(reinterpret<TypeAnnotation?> annotation))
                return <- up_expr(data, tp, at);
            } else {
                return <- up_expr(data, info, at);
            }
        }
    }

    def any_loc() {
        idx++;
        return LineInfo(column=uint(idx), line=uint(idx));
    }

    //! this is similar to regular return <-, but it does not check for locks
    def up_expr(var data: uint8 const?; info : TypeDeclPtr; at: LineInfo) : ExpressionPtr {
        if (data == null) {
            return <- null;
        }
        unsafe {
            var ann = info.annotation != null ? info.annotation : info.firstType.annotation;
            if (ann != null && ann.isBasicStructureAnnotation) {
                assert(ann.isBasicStructureAnnotation);
                var inscope mkS1 <- new MakeStruct(uninitialized)
                unsafe {
                    for_each_field(*reinterpret<BasicStructureAnnotation?>(ann)) <| $(name, cppName, xtype, offset) {
                        if (name == "pBlock" || (name == "_block" && (reinterpret<Expression>(data)).__rtti == "ExprReturn")) {
                            return;
                        }
                        var fdata = data + int(offset);
                        let tstr = describe(xtype)

                        var res_field <- ExpressionPtr();
                        var flags : MakeFieldDeclFlags = MakeFieldDeclFlags.moveSemantics;
                        if (tstr == "smart_ptr<ast::Expression>") {
                            print("{tabs()} typo {tstr} {name} {(reinterpret<Expression>(data)).__rtti}\n");
                        } else {
                            print("{tabs()} typo {tstr} {name}\n");
                        }
                        // if ((xtype.annotation != null && !xtype.annotation.canNew) || (xtype.firstType != null && xtype.firstType.annotation != null && !xtype.firstType.annotation.canNew)) {
                        if (tstr |> starts_with("rtti::FileInfo") || tstr |> starts_with("rtti::Module")) {
                            print("Ignore {tstr}\n");
                            return;
                        } elif (tstr == "$::dasvector`smart_ptr`Expression") {
                            assume field_values = *reinterpret<dasvector`smart_ptr`Expression?>(fdata);
                            res_field <- convert_vec(self, field_values, xtype, any_loc())
                            flags = MakeFieldDeclFlags.cloneSemantics;
                        } elif (tstr == "$::dasvector`smart_ptr`TypeDecl") {
                            assume field_values = *reinterpret<dasvector`smart_ptr`TypeDecl?>(fdata);
                            res_field <- convert_vec(self, field_values, xtype, any_loc())
                            flags = MakeFieldDeclFlags.cloneSemantics;
                        } elif (tstr == "$::dasvector`smart_ptr`Variable") {
                            assume field_values = *reinterpret<dasvector`smart_ptr`Variable?>(fdata);
                            res_field <- convert_vec(self, field_values, xtype, any_loc())
                            flags = MakeFieldDeclFlags.cloneSemantics;
                        } elif (tstr == "$::dasvector`int") {
                            // assume field_values = *reinterpret<dasvector`int?>(fdata);
                            // res_field <- convert_vec_basic(self, field_values, xtype, any_loc())
                            // flags = MakeFieldDeclFlags.cloneSemantics;
                            return;
                        } elif (xtype.baseType == Type.tPointer && xtype.firstType.baseType == Type.tHandle ) {
                            var pdata = *(reinterpret<uint8 const ??> fdata)
                            if (pdata == null) {
                                return;
                            }
                            depth++;
                            var subexpr <- up_only_expr(pdata, xtype.firstType, any_loc());
                            depth--;
                            var sub_tp = new TypeDecl(baseType = Type.tPointer, firstType = clone_type((reinterpret<smart_ptr<ExprMakeStruct>>(subexpr)).makeType), at=any_loc());
                            if (xtype.flags.smartPtr) {
                                sub_tp.flags |= TypeDeclFlags.smartPtr;
                            }
                            print("{describe(sub_tp)} {sub_tp}");
                            sub_tp.flags = xtype.flags;
                            res_field := new ExprAscend(subexpr<-subexpr, ascType<-sub_tp, at=any_loc());
                            print("type {res_field._type}\n");
                        } elif (tstr == "$::das_string") {
                            res_field <- new ExprConstString(value:=*reinterpret<string?>(fdata), at=any_loc());
                            flags = MakeFieldDeclFlags.cloneSemantics
                        } elif (xtype.baseType == Type.tPointer || tstr |> starts_with("$::dasvector") || tstr == "rtti::AnnotationList" || tstr == "rtti::AnnotationArgumentList" ) {
                            print("Ignore ${tstr}\n");
                            return;
                        } elif (xtype.baseType == Type.tHandle) {
                            depth++;
                            res_field <- up_expr(fdata, xtype, any_loc());
                            depth--;
                        } else {
                            res_field := walk_and_convert(fdata, xtype, any_loc());
                        }
                        let res_at = res_field.at;
                        var inscope mkFld <- new MakeFieldDecl(name := name, value <- res_field, at:=res_at);
                        if (!xtype.canCopy) {
                            mkFld.flags = flags;
                        }
                        emplace(*mkS1, mkFld);
                    }
                }
                var inscope mkStr <- new ExprMakeStruct(makeType<-clone_type(info), at=at)
                emplace(mkStr.structs, mkS1)
                mkStr.makeStructFlags |= ExprMakeStructFlags.useInitializer;
                return <- mkStr;
            } else {
                print("Reinterpret!!! {describe(info)} {info}\n");
                var res_field: ExpressionPtr;
                if (describe(info) == "$::das_string") {
                    res_field <- new ExprConstString(value:=*reinterpret<string?>(data), at=at);
                } else {
                    res_field := walk_and_convert(data, info, at);
                }
                return <- res_field
            }
        }
    }
    def override visit(prog : ProgramPtr; mod : Module?; var call : smart_ptr<ExprCallMacro>) : ExpressionPtr {
        // return <- new ExprAscend(subexpr <- new ExprConstBool(value := true))
        unsafe {
            assume arg_expr = call.arguments[0];

            prog.get_ptr() |> for_each_module_no_order() <| $(cur_mod) {
                if (cur_mod.name == "ast") {
                    rtti_module := cur_mod;
                }
            }
            assert(rtti_module != null);
            var annotation = unsafe(rtti_module |> module_find_annotation(arg_expr.__rtti));
            var inscope tp <- new TypeDecl(baseType = Type.tHandle,
                annotation = unsafe(reinterpret<TypeAnnotation?> annotation))
            tp.flags |= TypeDeclFlags.smartPtr;
            depth++;
            return <- new ExprAscend(subexpr<-up_expr(reinterpret<uint8 const?> arg_expr.get_ptr(), tp, call.at));
            depth++;
        }
    }
}

