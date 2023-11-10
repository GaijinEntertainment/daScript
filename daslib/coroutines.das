options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers = true

module coroutines shared public

require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost
require daslib/defer

[call_macro(name="yeild_from")]
class private YieldFrom : AstCallMacro
    //! This macro converts yield_from(THAT) expression into::
    //!
    //!     for t in THAT
    //!         yield t
    //!
    //! The idea is that coroutine or generator can continuesly yield from another sub-coroutine or generator.
    def override visit ( prog:ProgramPtr; mod:Module?; var call:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        macro_verify( call.arguments |> length==1,prog,call.at,"expecting yeild_from(iterator)" )
        macro_verify( call.arguments[0]._type!=null,prog,call.at,"expecting iterator" )
        macro_verify( call.arguments[0]._type.isIterator,prog,call.at,"expecting iterator" )
        let iname = make_unique_private_name("_yield_from_iterator",call.at)
        return <- qmacro_block <|
            for $i(iname) in $e(call.arguments[0])
                yield $i(iname)

[call_macro(name="co_continue")]
class private CoContinue : AstCallMacro
    //! This macro converts co_continue to yield true.
    //! The idea is that coroutine without specified type is underneath a coroutine which yields bool.
    //! That way co_continue() does not distract from the fact that it is a generator<bool>.
    def override visit ( prog:ProgramPtr; mod:Module?; var call:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        // TODO: verify if we are in coroutine
        // TODO: verify if we are in coroutine which returns bool
        macro_verify( call.arguments |> length==0,prog,call.at,"expecting co_continue()" )
        return <- qmacro_expr <|
            yield true

// [sideeffects]
def async_continue()
    panic("async_continue() should not be called directly, add [coroutine] annotation to the function")

def public async(var t : auto(T); var a : iterator<variant<res : T; wait : bool>>) : T
    panic("async() should not be called directly, add [coroutine] annotation to the function")
    return [[T]]

// [call_macro(name="async_continue")]
// class private AsyncContinue : AstCallMacro {}
    //! This macro converts co_continue to yield true.
    //! The idea is that coroutine without specified type is underneath a coroutine which yields bool.
    //! That way co_continue() does not distract from the fact that it is a generator<bool>.
    // def override visit ( prog:ProgramPtr; mod:Module?; var call:smart_ptr<ExprCallMacro> ) : ExpressionPtr
    //     // TODO: verify if we are in coroutine
    //     // TODO: verify if we are in coroutine which returns bool
    //     macro_verify( call.arguments |> length==0,prog,call.at,"expecting async_continue()" )
    //     return <- qmacro_expr <|
    //         yield <- [[auto wait=true]]

[call_macro(name="co_await")]
class private CoAwait : AstCallMacro
    //! This macro converts co_await(sub_coroutine) into::
    //!
    //!     for t in subroutine
    //!         yield t
    //!
    //! The idea is that coroutine or generator can wait for a sub-coroutine to finish.
    def override visit ( prog:ProgramPtr; mod:Module?; var call:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        // TODO: verify if we are calling co_await on a coroutine.
        macro_verify( call.arguments |> length==1,prog,call.at,"expecting co_await(subroutine)" )
        let iname = make_unique_private_name("_co_await_iterator",call.at)
        return <- qmacro_block <|
            for $i(iname) in $e(call.arguments[0])
                yield $i(iname)

[function_macro(name="coroutine")]
class private CoroutineMacro : AstFunctionAnnotation
    //! This macro converts coroutine function into generator, adds return false.
    //! daScript impelmentation of coroutine is generator based. Function is converted into a state machine,
    //! which can be resumed and suspended. The function is converted into a generator.
    //! Generator yields bool if its a void coroutine, and yields the return type otherwise.
    //! If return type is specified coroutine can serve as an advanced form of a generator.
    def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        var inscope retT : TypeDeclPtr
        var simpleCoroutine = true
        if func.result.baseType == Type tVoid || func.result.baseType == Type autoinfer
            move_new(retT) <| qmacro_type(type<bool>)
        else
            simpleCoroutine = false
            // move_new(retT) <| clone_type(func.result)
            // debug(retT)
            retT |> move_new() <| new [[TypeDecl() baseType=Type tVariant, at=func.at]]
            retT.argNames |> resize(2)
            retT.argNames[0] := "res"
            retT.argNames[1] := "wait"

            retT.argTypes |> resize(2)
            retT.argTypes[0] |> move_new() <| clone_type(func.result)
            retT.argTypes[1] |> move_new() <| typeinfo(ast_typedecl type<bool>)

        var astVisitor = new CollectAndReplaceIteratorFields()
        astVisitor.simpleCoroutine = simpleCoroutine
        astVisitor.retType := retT
        var inscope astVisitorAdapter <- make_visitor(*astVisitor)
        visit(func.body, astVisitorAdapter)
        delete astVisitor.retType
        unsafe
            delete astVisitor

            // print(func.body |> describe())
        var inscope blk <- qmacro_block <|
            return <- generator<$t(retT)>() <|
                $e(func.body)
                return false


        move(func.body) <| blk
        move_new(func.result) <| qmacro_type(type<iterator<$t(retT)>>)
        return true


class CollectAndReplaceIteratorFields : AstVisitor
    simpleCoroutine : bool
    retType : smart_ptr<TypeDecl>
    def override visitExprYield(var expr:smart_ptr<ExprYield>) : ExpressionPtr
        if simpleCoroutine
            return <- expr
        if expr._type != null && expr._type |> is_same_type(retType, RefMatters no, ConstMatters no, TemporaryMatters no)
            return <- expr
        return <- qmacro_expr <|
            yield <- [[$t(retType) res<-$e(expr.subexpr)]]

    def override visitExprCall(var expr:smart_ptr<ExprCall>) : ExpressionPtr
        if expr.name == "async_continue"
            if simpleCoroutine
                return <- qmacro_expr <|
                    yield true
            else
                return <- qmacro_expr <|
                    yield <- [[$t(retType) wait=true]]
        // elif expr.name == "async"
        //     return <- qmacro_expr <|
        //         for it in $e(expr.arguments[1])
        //             if it is res
        //                 $e(expr.arguments[0]) <- it as res
        return <- expr

    def override visitExprLet(var expr:smart_ptr<ExprLet>) : ExpressionPtr
        if length(expr.variables) != 1
            return <- expr
        for v in expr.variables
            if v.init != null && v.init is ExprCall
                var call = v.init as ExprCall
                if call.name == "async"
                    var inscope cr := call.arguments[0]
                    var inscope blk <- qmacro_block <|
                        var $i(v.name) : $t(v._type)
                        for it in $e(cr)
                            if it is res
                                $i(v.name) := it as res
                            else
                                if $v(simpleCoroutine)
                                    yield true
                                else
                                    yield <- [[$t(retType) wait=true]]
                    var block0 = blk as ExprBlock
                    block0.blockFlags &= ~ExprBlockFlags isClosure
                    // var inscope res <- new [[ExprBlock() at=call.at]]
                    // res.list |> resize(block0.list |> length)
                    // for b, b0 in res.list, block0.list
                    //     b := b0
                    block0.blockFlags |= ExprBlockFlags needCollapse
                    block0.blockFlags |= ExprBlockFlags isCollapseable
                    print(block0 |> describe())
                    return <- blk
        return <- expr


def public cr_get ( var a : iterator<variant<res : auto(T); wait : bool>> ) : T
    //! This function runs coroutine until it is finished and returns the result.
    var result : T -& -const
    static_if typeinfo(is_ref_type type<T>)
        for it in a
            if it is res
                result := it as res
                print("temp res {result} \n")
            else
                print("temp wait {it as wait} \n")
        return <- result
    else
        for it in a
            if it is res
                result = it as res
                print("temp res {result} \n")
            else
                print("temp wait {it as wait} \n")
        return result

def public cr_run ( var a : iterator<bool> )
    //! This function runs coroutine until it is finished.
    for t in a
        pass

[sideeffects]
def public cr_run ( var a : iterator<auto> )
    //! This function runs coroutine until it is finished.
    for t in a
        pass

def public cr_run_all ( var a : array<iterator<auto>> )
    //! This function runs all coroutines until they are finished.
    while true
        var i = length(a)
        if i==0
            break
        while i > 0
            i --
            var t : bool
            next(a[i],t)
            if empty(a[i])
                delete a[i]
                a |> erase(i)
