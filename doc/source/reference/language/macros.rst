.. _macros:

======
Macros
======

In daScript macros are the machinery which allows direct manipulation of a syntax tree.

Macros are exposed via :ref:`daslib/ast <stdlib_ast>` module and :ref:`daslib/ast_boost <stdlib_ast_boost>` helper module.

Macros are evaluated at compilation time during different compilation passes.
Macros assigned to specific module are evaluated as part of the module, every time module is included.

------------------
Compilation passes
------------------

daScript compiler performs compilation passes in the following order for each module (see :ref:`Modules <modules>`)

#. parser transforms das program to AST

    #. if there are any parsing errors, compilation stops

#. `apply` is called for every function or structure

    #. if there are any errors, compilation stops

#. infer pass repeats itself until no new transformations are reported

    #. built-in infer pass happens

        #. `transform` macros are called for every function, or expression

    #. macro passes happen

#. if there are still any errors left, compilation stops

#. `finish` is called for all function and structure macros

#. lint pass happens

    #. if there are any errors, compilation stops

#. optimization pass repeats itself until no new transformations are reported

    #. built-in optimization pass happens

    #. macro optimization pass happens

#. if there are any errors during optimization passes, compilation stops

#. if module contains any macros, simulation happens

    #. if there are any simulation errors, compilation stops

    #. module macros functions (annotated with `_macro`) are invoked

        #. if there are any errors, compilation stops

Modules are compiled in `require` order.

---------------
Invoking macros
---------------

To specify function to be evaluated in compilation time ``[_macro]`` annotation is used.
Consider the following example from :ref:`daslib/ast_boost <stdlib_ast_boost>`::

    [_macro,private]
    def setup
        if is_compiling_macros_in_module("ast_boost")
            add_new_function_annotation("macro", new MacroMacro())

`setup` function will be evaluated after compilation of each module, which includes ast_boost.
``is_compiling_macros_in_module`` function returns true if currently compiled module name matches the argument.
In this particular example function annotation ``macro`` would only be added once, when the module `ast_boost` is compiled.

Macros are invoked in the following fashion:

#. class is derived from the appropriate base macro class
#. adapter is created
#. adapter is registered with the module

For example this is how this lifetime cycle is implemented for the reader macro::

    def add_new_reader_macro ( name:string; someClassPtr )
        var ann <- make_reader_macro(name, someClassPtr)
        this_module() |> add_reader_macro(ann)
        unsafe
            delete ann

---------------------
AstFunctionAnnotation
---------------------

``AstFunctionAnnotation`` macro allows to manipulate call to specific function as well as function body.
Annotation can be added to regular or generic function.

``add_new_function_annotation`` adds function annotation to a module.
There is additionally ``[function_macro]`` annotation which accomplishes the same thing.

``AstFunctionAnnotation`` allows several different manipulations::

    class AstFunctionAnnotation
        def abstract transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        def abstract verifyCall ( var call : smart_ptr<ExprCallFunc>; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract finish ( var func:FunctionPtr; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract patch ( var func:FunctionPtr; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string; var astChanged:bool& ) : bool
        def abstract fixup ( var func:FunctionPtr; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract lint ( var func:FunctionPtr; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract complete ( var func:FunctionPtr; var ctx:smart_ptr<Context> ) : void
        def abstract isCompatible ( var func:FunctionPtr; var types:VectorTypeDeclPtr; decl:AnnotationDeclaration; var errors:das_string ) : bool
        def abstract isSpecialized : bool
        def abstract appendToMangledName ( func:FunctionPtr; decl:AnnotationDeclaration; var mangledName:das_string ) : void

``transform`` allows changing call to the function and is applied at infer pass.
Transform is the best way to replace or modify function call with other semantics.

``verifyCall`` is called durng the `lint` phase on each call to the function and is used to check if call is valid.

``apply`` is applied to function itself before the infer pass.
Apply is typically where global function body modifications or instancing occurs.

``finish`` is applied to function itself after the infer pass.
It's only called to non-generic functions or instances of the generic functions.
``finish`` is typically used to register functions, notify C++ code, etc.
Function is fully defined and inferred, and can no longer be modified.

``patch`` is called after the infer pass. If patch sets astChanged to true, the infer pass will be repeated.

``fixup`` is called after the infer pass. It's used to fixup function body.

``lint`` is called during the `lint` phase on the function itself and is used to verify if function is valid.

``complete`` is called during the `simulate` portion of context creation. At this point Context is available.

``isSpecialized`` must return true, if the particular function matching is governed by contracts.
In that case ``isCompatible`` will be called, and result taken into account.

``isCompatible`` returns true, if specialized function is compatible with arguments.
If function is not compatible, errors field must be specified.

``appendToMangledName`` is called to append mangled name to the function.
That way multiple functions with the same type signature can exist and be differentiated between.

Lets review the following example from `ast_boost` of how ``macro`` annotation is implemented::

    class MacroMacro : AstFunctionAnnotation
        def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
            compiling_program().flags |= ProgramFlags needMacroModule
            func.flags |= FunctionFlags init
            var blk <- new [[ExprBlock() at=func.at]]
            var ifm <- new [[ExprCall() at=func.at, name:="is_compiling_macros"]]
            var ife <- new [[ExprIfThenElse() at=func.at, cond<-ifm, if_true<-func.body]]
            push(blk.list,ife)
            func.body <- blk
            return true

During the `apply` pass function body is appended with ``if is_compiling_macros()`` closure,
additionally ``init`` flag is set, which is equivalent to ``_macro`` annotation.
Function annotated with ``[macro]`` will be evaluated during module compilation.

------------------
AstBlockAnnotation
------------------

AstBlockAnnotation is used to manipulate block expressions (block, lambda, local function)::

    class AstBlockAnnotation
        def abstract apply ( var blk:smart_ptr<ExprBlock>; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract finish ( var blk:smart_ptr<ExprBlock>; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool

``add_new_block_annotation`` adds function annotation to a module.
There is additionally ``[block_macro]`` annotation which accomplishes the same thing.

``apply`` is called for every block expression before the infer pass.

``finish`` is called for every block expression after infer pass.

----------------------
AstStructureAnnotation
----------------------

``AstStructureAnnotation`` macro allows to manipulate structure or class definitions via annotation::

    class AstStructureAnnotation
        def abstract apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract finish ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract patch ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string; var astChanged:bool& ) : bool
        def abstract complete ( var st:StructurePtr; var ctx:smart_ptr<Context> ) : void

``add_new_structure_annotation`` adds function annotation to a module.
There is additionally ``[structure_macro]`` annotation which accomplishes the same thing.

``AstStructureAnnotation`` allows 2 different manipulations::

    class AstStructureAnnotation
        def abstract apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract finish ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool

``apply`` is invoked before the infer pass. It is the best time to modify structure, generated some code, etc.

``finish`` is invoked after the successful infer pass. Its typically used to register structures, perform RTTI operations etc.
Structure is fully inferred and defined and can no longer be modified afterwards.

``patch`` is invoked after the infer pass. If patch sets astChanged to true, the infer pass will be repeated.

``complete`` is invoked during the `simulate` portion of context creation. At this point Context is available.

Example of such annotation is `SetupAnyAnnotation` from :ref:`daslib/ast_boost <stdlib_ast_boost>`.

------------------------
AstEnumerationAnnotation
------------------------

``AstStructureAnnotation`` macro allows to manipulate enumeration via annotation::

    class AstEnumerationAnnotation
        def abstract apply ( var st:EnumerationPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool

``add_new_enumeration_annotation`` adds function annotation to a module.
There is additionally ``[enumeration_macro]`` annotation which accomplishes the same thing.

``apply`` is invoked before the infer pass. It is the best time to modify enumeration, generated some code, etc.

---------------
AstVariantMacro
---------------

``AstVariantMacro`` is specialized in transforming ``is``, ``as``, and ``?as`` expressions.

``add_new_variant_macro`` adds variant macro to a module.
There is additionally ``[variant_macro]`` annotation which accomplishes the same thing.

Each of the 3 transformations are covered in appropriate abstract function::

    class AstVariantMacro
        def abstract visitExprIsVariant     ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprIsVariant> ) : ExpressionPtr
        def abstract visitExprAsVariant     ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprAsVariant> ) : ExpressionPtr
        def abstract visitExprSafeAsVariant ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprSafeAsVariant> ) : ExpressionPtr

Lets review the following example from :ref:`daslib/ast_boost <stdlib_ast_boost>`::

    // replacing ExprIsVariant(value,name) => ExprOp2('==",value.__rtti,"name")
    // if value is ast::Expr*
    class BetterRttiVisitor : AstVariantMacro
        def override visitExprIsVariant(prog:ProgramPtr; mod:Module?;expr:smart_ptr<ExprIsVariant>) : ExpressionPtr
            if isExpression(expr.value._type)
                var vdr <- new [[ExprField() at=expr.at, name:="__rtti", value <- clone_expression(expr.value)]]
                var cna <- new [[ExprConstString() at=expr.at, value:=expr.name]]
                var veq <- new [[ExprOp2() at=expr.at, op:="==", left<-vdr, right<-cna]]
                return veq
            return [[ExpressionPtr]]

    // note the following ussage
    class GetHintFnMacro : AstFunctionAnnotation
        def override transform ( var call : smart_ptr<ExprCall>; var errors : das_string ) : ExpressionPtr
            if call.arguments[1] is ExprConstString     // HERE EXPRESSION WILL BE REPLACED
                ...

Here the macro takes advantage of the ExprIsVariant syntax.
It replaces ``expr is TYPENAME`` expression with ``expr.__rtti = "TYPENAME"`` expression.
``isExpression`` function ensures that expr is from the ast::Expr* family, i.e. part of the daScript syntax tree.

--------------
AstReaderMacro
--------------

``AstReaderMacro`` allows embedding completely different syntax inside daScript code.

``add_new_reader_macro`` adds reader macro to a module.
There is additionally ``[reader_macro]`` annotation, which essentially automates the same thing.

Reader macro accepts characters, collects them if necessary, and returns `ast::Expression`::

    class AstReaderMacro
        def abstract accept ( prog:ProgramPtr; mod:Module?; expr:ExprReader?; ch:int; info:LineInfo ) : bool
        def abstract visit ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprReader> ) : ExpressionPtr

Reader macros are invoked via following syntax ``% READER_MACRO_NAME ~ character_sequence``.
``accept`` function notifies the correct terminator of the character sequence::

    var x = %arr~\{\}\w\x\y\n%% // invoking reader macro arr, %% is a terminator

Consider the implementation for the example above::

    [reader_macro(name="arr")]
    class ArrayReader : AstReaderMacro
        def override accept ( prog:ProgramPtr; mod:Module?; var expr:ExprReader?; ch:int; info:LineInfo ) : bool
            append(expr.sequence,ch)
            if ends_with(expr.sequence,"%%")
                let len = length(expr.sequence)
                resize(expr.sequence,len-2)
                return false
            else
                return true
        def override visit ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprReader> ) : ExpressionPtr
            let seqStr = string(expr.sequence)
            var arrT <- new [[TypeDecl() baseType=Type tInt]]
            push(arrT.dim,length(seqStr))
            var mkArr <- new [[ExprMakeArray() at = expr.at, makeType <- arrT]]
            for x in seqStr
                var mkC <- new [[ExprConstInt() at=expr.at, value=x]]
                push(mkArr.values,mkC)
            return mkArr

In ``accept`` function macro collects symbols in the sequence.
Once the sequence ends with the terminator sequence %%, ``accept`` returns false to notify for the end of read.

In ``visit`` the collected sequence is converted into make array ``[[int ch1; ch2; ..]]`` expression.

More complex examples are JsonReader macro in :ref:`daslib/json_boost <stdlib_json_boost>` or RegexReader in :ref:`daslib/regex_boost <stdlib_regex_boost>`.

------------
AstCallMacro
------------

``AstCallMacro`` operates on expressions, which have similar to function call syntax.
It occurs during the infer pass.

``add_new_call_macro`` adds call macro to a module.
``[call_macro]`` annotation automates the same thing::

        class AstCallMacro
            def abstract preVisit ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprCallMacro> ) : void
            def abstract visit ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
            def abstract canVisitArguments ( expr:smart_ptr<ExprCallMacro> ) : bool

``apply`` from the :ref:`daslib/apply <stdlib_apply>` is an example of such macro::

    [call_macro(name="apply")]  // apply(value, block)
    class ApplyMacro : AstCallMacro
        def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
            ...

Note how name is provided in the ``[call_macro]`` annotation.

``preVisit`` is called before the arguments are visited.

``visit`` is called after the arguments are visited.

``canVisitArguments`` is called to determine if the macro can visit the arguments.

------------
AstPassMacro
------------

``AstPassMacro`` is one macro to rule them all. It gets entire module as an input,
and can be invoked at numerous passes::

    class AstPassMacro
        def abstract apply ( prog:ProgramPtr; mod:Module? ) : bool

``make_pass_macro`` registers class as a pass macro.

``add_new_infer_macro`` adds pass macro to the infer pass. ``[infer]`` annotation accomplishes the same thing.

``add_new_dirty_infer_macro`` adds pass macro to the `dirty` section of infer pass. ``[dirty_infer]`` annotation accomplishes the same thing.

Typically such macro creates an ``AstVisitor`` which performs necessary transformations.

----------------
AstTypeInfoMacro
----------------

``AstTypeInfoMacro`` is designed to implement custom type information inside typeinfo expression::

    class AstTypeInfoMacro
        def abstract getAstChange ( expr:smart_ptr<ExprTypeInfo>; var errors:das_string ) : ExpressionPtr
        def abstract getAstType ( var lib:ModuleLibrary; expr:smart_ptr<ExprTypeInfo>; var errors:das_string ) : TypeDeclPtr

``add_new_typeinfo_macro`` adds reader macro to a module.
There is additionally ``[typeinfo_macro]`` annotation, which essentially automates the same thing.

``getAstChange`` returns newly generated ast for the typeinfo expression.
Alternatively it returns null if no changes are required, or if there is an error.
In case of error errors string must be filled.

``getAstType`` returns type of the new typeinfo expression.

---------------
AstForLoopMacro
---------------

``AstForLoopMacro`` is designed to implement custom processing of the for loop expressions::

    class AstForLoopMacro
        def abstract visitExprFor ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprFor> ) : ExpressionPtr

``add_new_for_loop_macro`` adds reader macro to a module.
There is additionally ``[for_loop_macro]`` annotation, which essentially automates the same thing.

``visitExprFor`` is similar to that of the `AstVisitor`. It returns new expression, or null if no changes are required.

---------------
AstCaptureMacro
---------------

``AstCaptureMacro`` is designed to implement custom capturing and finalization of lambda expressions.

    class AstCaptureMacro
        def abstract captureExpression ( prog:Program?; mod:Module?; expr:ExpressionPtr; etype:TypeDeclPtr ) : ExpressionPtr
        def abstract captureFunction ( prog:Program?; mod:Module?; var lcs:Structure?; var fun:FunctionPtr ) : void

``add_new_capture_macro`` adds reader macro to a module.
There is additionally ``[capture_macro]`` annotation, which essentially automates the same thing.

``captureExpression`` is called when an expression is captured. It returns new expression, or null if no changes are required.

``captureFunction`` is called when a function is captured. This is where custom finalization can be added to `final` section of the function body.

----------------
AstCommentReader
----------------

``AstCommentReader`` is designed to implement custom processing of the comment expressions::

    class AstCommentReader
        def abstract open ( prog:ProgramPtr; mod:Module?; cpp:bool; info:LineInfo ) : void
        def abstract accept ( prog:ProgramPtr; mod:Module?; ch:int; info:LineInfo ) : void
        def abstract close ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract beforeStructure ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterStructure ( st:StructurePtr; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract beforeStructureFields ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterStructureField ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterStructureFields ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract beforeFunction ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterFunction ( fn:FunctionPtr; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract beforeGlobalVariables ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterGlobalVariable ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterGlobalVariables ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract beforeVariant ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterVariant ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract beforeEnumeration ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterEnumeration ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract beforeAlias ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterAlias ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void

``add_new_comment_reader`` adds reader macro to a module.
There is additionally ``[comment_reader]`` annotation, which essentially automates the same thing.

``open`` occurs when a any comment is starting parsing.

``accept`` occurs for every character of the comment.

``close`` occrus when a coment is over.

``beforeStructure`` and ``afterStructure`` occur before and after each structure or class declaration, regardless if it contains comments.

``beforeStructureFields`` and ``afterStructureFields`` occur before and after each structure or class fields, regardless if it contains comments.

``afterStructureField`` occurs after each field declaration.

``beforeFunction`` and ``afterFunction`` occur before and after each function declaration, regardless if it contains comments.

``beforeGlobalVariables`` and ``afterGlobalVariables`` occur before and after each global variables declaration, regardless if it contains comments.

``afterGlobalVariable`` occur after each individual global variable declaration.

``beforeVariant`` and ``afterVariant`` occur before and after each variant declaration, regardless if it contains comments.

``beforeEnumeration`` and ``afterEnumeration`` occur before and after each enumeration declaration, regardless if it contains comments.

``beforeAlias`` and ``afterAlias`` occur before and after each alias type declaration, regardless if it contains comments.

----------
AstVisitor
----------

``AstVisitor`` implements visitor pattern for the daScript expression tree.
It contains callback for every single expression in prefix and postfix form, as well as some additional callbacks::

    class AstVisitor
        ...
        // find
            def abstract preVisitExprFind(expr:smart_ptr<ExprFind>) : void          // prefix
            def abstract visitExprFind(expr:smart_ptr<ExprFind>) : ExpressionPtr    // postifx
        ...

Postfix callback can return expression to replace the one passed to the callback.

PrintVisitor form `ast_print` example implements printing of every single expression in daScript syntax.

``make_visitor`` creates visitor adapter from the class, derived from the AstVisitor.
Adapter then can be applied to a program via ``visit`` function::

    var astVisitor = new PrintVisitor()
    var astVisitorAdapter <- make_visitor(*astVisitor)
    visit(this_program(), astVisitorAdapter)

If expression needs to be visited, and can potentially be fully substituted, ``visit_expression`` function should be used::

    expr <- visit_expression(expr,astVisitorAdapter)