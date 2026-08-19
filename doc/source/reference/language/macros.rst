.. _macros:

======
Macros
======

In Daslang, macros are the machinery that allow direct manipulation of the syntax tree.

Macros are exposed via the :ref:`daslib/ast <stdlib_ast>` module and :ref:`daslib/ast_boost <stdlib_ast_boost>` helper module.
Every example on this page assumes both are required, and lives in a file which declares a
:ref:`module <modules>` — a file that carries macros has to be one:

.. das-doc: given module doc_macros
.. code-block:: das

    require daslib/ast
    require daslib/ast_boost

Macros are evaluated at compilation time during different compilation passes.
Macros assigned to a specific module are evaluated as part of the module every time that module is included.

------------------
Compilation passes
------------------

The Daslang compiler performs compilation passes in the following order for each module (see :ref:`Modules <modules>`):

#. Parser transforms das program to AST

    #. If there are any parsing errors, compilation stops

#. ``apply`` is called for every function or structure

    #. If there are any errors, compilation stops

#. Infer pass repeats itself until no new transformations are reported

    #. Built-in infer pass happens

        #. ``transform`` macros are called for every function or expression

    #. Macro passes happen

#. If there are still any errors left, compilation stops

#. ``finish`` is called for all functions and structure macros

#. Lint pass happens

    #. If there are any errors, compilation stops

#. Optimization pass repeats itself until no new transformations are reported

    #. Built-in optimization pass happens

    #. Macro optimization pass happens

#. If there are any errors during optimization passes, compilation stops

#. If the module contains any macros, simulation happens

    #. If there are any simulation errors, compilation stops

    #. Module macro functions (annotated with ``_macro``) are invoked

        #. If there are any errors, compilation stops

Modules are compiled in ``require`` order.

---------------
Invoking macros
---------------

The ``[_macro]`` annotation is used to specify functions that should be evaluated at compilation time .
Consider the following example from :ref:`daslib/ast_boost <stdlib_ast_boost>`:

.. code-block:: das

    [_macro]
    def private setup {
        if ( is_compiling_macros_in_module("ast_boost") ) {
            add_new_function_annotation("macro", new MacroMacro())
        }
    }

The ``setup`` function is evaluated after the compilation of each module, which includes ast_boost.
The ``is_compiling_macros_in_module`` function returns true if the currently compiled module name matches the argument.
In this particular example, the function annotation ``macro`` would only be added once: when the module ``ast_boost`` is compiled.

Macros are invoked in the following fashion:

#. Class is derived from the appropriate base macro class
#. Adapter is created
#. Adapter is registered with the module

For example, this is how this lifetime cycle is implemented for the reader macro:

.. code-block:: das

    def add_new_reader_macro ( name:string; var someClassPtr ) {
        var ann = make_reader_macro(name, someClassPtr)
        this_module() |> add_reader_macro(ann)
    }

---------------------
AstFunctionAnnotation
---------------------

The ``AstFunctionAnnotation`` macro allows you to manipulate calls to specific functions as well as their function bodies.
Annotations can be added to regular or generic functions.

``add_new_function_annotation`` adds a function annotation to a module.
There is additionally the ``[function_macro]`` annotation which accomplishes the same thing.

``AstFunctionAnnotation`` allows several different manipulations:

.. das-doc: signatures
.. code-block:: das

    class AstFunctionAnnotation {
        def abstract transform ( var call : ExprCallFunc?; var errors : das_string ) : ExpressionPtr
        def abstract verifyCall ( var call : ExprCallFunc?; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract generic_apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract finish ( var func:FunctionPtr; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract patch ( var func:FunctionPtr; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string; var astChanged:bool& ) : bool
        def abstract fixup ( var func:FunctionPtr; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract lint ( var func:FunctionPtr; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract complete ( var func:FunctionPtr; var ctx:smart_ptr<Context> ) : void
        def abstract isCompatible ( var func:FunctionPtr; var types:VectorTypeDeclPtr; decl:AnnotationDeclaration; var errors:das_string ) : bool
        def abstract isSpecialized : bool
        def abstract appendToMangledName ( func:FunctionPtr; decl:AnnotationDeclaration; var mangledName:das_string ) : void
        def abstract isAppliedToGeneric : bool
    }

``transform`` lets you change calls to the function and is applied at the infer pass.
Transform is the best way to replace or modify function calls with other semantics.

``verifyCall`` is called during the ``lint`` phase on each call to the function and is used to check if the call is valid.

``apply`` is applied to the function itself before the infer pass.
Apply is typically where global function body modifications or instancing occurs.

``generic_apply`` is applied to each instance of a generic function as it is specialized,
after the type substitution. Use it for transformations that need the resolved types of the
instance. For purely structural rewrites that do not need types resolved, override
``apply`` and return true from ``isAppliedToGeneric`` so the rewrite happens once on the
generic template instead.

``finish`` is applied to the function itself after the infer pass.
It's only called on non-generic functions or instances of the generic functions.
``finish`` is typically used to register functions, notify C++ code, etc.
After this, the function is fully defined and inferred, and can no longer be modified.

``patch`` is called after the infer pass. If patch sets astChanged to true, the infer pass will be repeated.

``fixup`` is called after the infer pass. It's used to fixup the function's body.

``lint`` is called during the ``lint`` phase on the function itself and is used to verify that the function is valid.

``complete`` is called during the ``simulate`` portion of context creation. At this point Context is available.

``isSpecialized`` must return true if the particular function matching is governed by contracts.
In that case, ``isCompatible`` is called, and the result taken into account.

``isCompatible`` returns true if a specialized function is compatible with the given arguments.
If a function is not compatible, the errors field must be specified.

``appendToMangledName`` is called to append a mangled name to the function.
That way multiple functions with the same type signature can exist and be differentiated between.

``isAppliedToGeneric`` returns true if the annotation's ``apply`` should also run on
generic function templates at parse time. By default it is false, meaning ``apply`` only
runs on non-generic functions; generic instances receive ``generic_apply`` instead.
Annotations that perform purely structural rewrites (no type information needed) — for
example, ``[class_method]`` injecting a ``self`` argument — return true so that the
rewrite happens once on the template and every instantiation inherits it.

Lets review the following example from ``ast_boost`` of how the ``macro`` annotation is implemented
(the excerpt is quoted from that module — a second declaration of ``MacroMacro`` cannot coexist with
it in one program):

.. das-doc: fragment
.. code-block:: das

    class MacroMacro : AstFunctionAnnotation {
        def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool {
            compiling_program().flags.needMacroModule = true
            func.flags.macroInit = true
            var blk = new ExprBlock(at=func.at)
            var ifm = new ExprCall(at=func.at, name:="is_compiling_macros")
            var ife = new ExprIfThenElse(at=func.at, cond=ifm, if_true=func.body)
            emplace(blk.list,ife)
            func.body = blk
            return true
        }
    }

During the ``apply`` pass the function body is appended with the ``if is_compiling_macros()`` closure.
Additionally, the ``macroInit`` flag is set, which is what the ``_macro`` annotation itself sets.
Functions annotated with ``[macro]`` are evaluated during module compilation.

------------------
AstBlockAnnotation
------------------

``AstBlockAnnotation`` is used to manipulate block expressions (blocks, lambdas, local functions):

.. das-doc: signatures
.. code-block:: das

    class AstBlockAnnotation {
        def abstract apply ( var blk:ExprBlock?; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract finish ( var blk:ExprBlock?; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool
    }

``add_new_block_annotation`` adds a block annotation to a module.
There is additionally the ``[block_macro]`` annotation which accomplishes the same thing.

``apply`` is called for every block expression before the infer pass.

``finish`` is called for every block expression after infer pass.

----------------------
AstStructureAnnotation
----------------------

The ``AstStructureAnnotation`` macro lets you manipulate structure or class definitions via annotation:

.. das-doc: signatures
.. code-block:: das

    class AstStructureAnnotation {
        def abstract apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract finish ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract patch ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string; var astChanged:bool& ) : bool
        def abstract complete ( var st:StructurePtr; var ctx:smart_ptr<Context> ) : void
        def abstract aotPrefix ( var st:StructurePtr; args:AnnotationArgumentList; var writer:StringBuilderWriter ) : void
        def abstract aotBody ( var st:StructurePtr; args:AnnotationArgumentList; var writer:StringBuilderWriter ) : void
        def abstract aotSuffix ( var st:StructurePtr; args:AnnotationArgumentList; var writer:StringBuilderWriter ) : void
    }

``add_new_structure_annotation`` adds a structure annotation to a module.
There is additionally the ``[structure_macro]`` annotation which accomplishes the same thing.

``apply`` is invoked before the infer pass. It is the best time to modify the structure, generate some code, etc.

``finish`` is invoked after the successful infer pass. Its typically used to register structures, perform RTTI operations, etc.
After this, the structure is fully inferred and defined and can no longer be modified afterwards.

``patch`` is invoked after the infer pass. If patch sets astChanged to true, the infer pass will be repeated.

``complete`` is invoked during the ``simulate`` portion of context creation. At this point Context is available.

``aotPrefix``, ``aotBody`` and ``aotSuffix`` write C++ text into the generated AOT code: before the
generated ``struct``, inside its body, and after its closing brace.

An example of such annotation is ``SetupAnyAnnotation`` from :ref:`daslib/ast_boost <stdlib_ast_boost>`.

------------------------
AstEnumerationAnnotation
------------------------

The ``AstEnumerationAnnotation`` macro lets you manipulate enumerations via annotation:

.. das-doc: signatures
.. code-block:: das

    class AstEnumerationAnnotation {
        def abstract apply ( var st:EnumerationPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
    }

``add_new_enumeration_annotation`` adds an enumeration annotation to a module.
There is additionally the ``[enumeration_macro]`` annotation which accomplishes the same thing.

``apply`` is invoked before the infer pass. It is the best time to modify the enumeration, generate some code, etc.

An enumeration macro is a class registered with ``[enumeration_macro]``. It lives in its own
module, because the annotation it registers has to exist before the file that uses it is parsed:

.. das-doc: file enum_macro_mod.das
.. code-block:: das

    module enum_macro_mod public

    require daslib/ast
    require daslib/ast_boost

    [enumeration_macro(name="enum_total")]
    class EnumTotalAnnotation : AstEnumerationAnnotation {
        def override apply(var enu : EnumerationPtr; var group : ModuleGroup;
                           args : AnnotationArgumentList;
                           var errors : das_string) : bool {
            // modify enu.list or generate code
            return true
        }
    }

A file which requires that module can then annotate its enumerations:

.. code-block:: das

    require enum_macro_mod

    [enum_total]
    enum Direction { North, South, East, West }

.. seealso::

   Tutorial: :ref:`tutorial_macro_enumeration_macro` — step-by-step
   enumeration macro examples (enum modification and code generation)

   Standard library: ``daslib/enum_trait.das`` —
   :ref:`enum_trait module reference <stdlib_enum_trait>`

---------------
AstVariantMacro
---------------

``AstVariantMacro`` is specialized in transforming ``is``, ``as``, and ``?as`` expressions.

``add_new_variant_macro`` adds a variant macro to a module.
There is additionally the ``[variant_macro]`` annotation which accomplishes the same thing.

Each of the 3 transformations are covered in the appropriate abstract function:

.. das-doc: signatures
.. code-block:: das

    class AstVariantMacro {
        def abstract visitExprIsVariant     ( prog:ProgramPtr; mod:Module?; expr:ExprIsVariant? ) : ExpressionPtr
        def abstract visitExprAsVariant     ( prog:ProgramPtr; mod:Module?; expr:ExprAsVariant? ) : ExpressionPtr
        def abstract visitExprSafeAsVariant ( prog:ProgramPtr; mod:Module?; expr:ExprSafeAsVariant? ) : ExpressionPtr
    }

Let's review the following example from :ref:`daslib/ast_boost <stdlib_ast_boost>`:

.. das-doc: fragment
.. code-block:: das

    // replacing ExprIsVariant(value,name) => ExprOp2("==", value.__rtti, "name")
    // if value is ast::Expr*
    class BetterRttiVisitor : AstVariantMacro {
        def override visitExprIsVariant(prog:ProgramPtr; mod:Module?;expr:ExprIsVariant?) : ExpressionPtr {
            if ( isExpression(expr.value._type) ) {
                var vdr = new ExprField(at=expr.at, name:="__rtti", value = clone_expression(expr.value))
                var cna = new ExprConstString(at=expr.at, value:=expr.name)
                var veq = new ExprOp2(at=expr.at, op:="==", left=vdr, right=cna)
                return veq
            }
            return default<ExpressionPtr>
        }
    }

    // note the following usage
    class GetHintFnMacro : AstFunctionAnnotation {
        def override transform ( var call : ExprCallFunc?; var errors : das_string ) : ExpressionPtr {
            if ( call.arguments[1] is ExprConstString ) {    // HERE EXPRESSION WILL BE REPLACED
                ...
            }
        }
    }

Here, the macro takes advantage of the ExprIsVariant syntax.
It replaces the ``expr is TYPENAME`` expression with an ``expr.__rtti == "TYPENAME"`` expression.
The ``isExpression`` function ensures that ``expr`` is from the ``ast::Expr*`` family, i.e. part of the Daslang syntax tree.

--------------
AstReaderMacro
--------------

``AstReaderMacro`` allows embedding a completely different syntax inside Daslang code.

``add_new_reader_macro`` adds a reader macro to a module.
There is additionally the ``[reader_macro]`` annotation, which essentially automates the same thing.

Reader macros accept characters, collect them if necessary, and produce output
via one of two patterns:

.. das-doc: signatures
.. code-block:: das

    class AstReaderMacro {
        def abstract accept ( prog:ProgramPtr; mod:Module?; expr:ExprReader?; ch:int; info:LineInfo ) : bool
        def abstract visit ( prog:ProgramPtr; mod:Module?; expr:ExprReader? ) : ExpressionPtr
        def abstract suffix ( prog:ProgramPtr; mod:Module?; expr:ExprReader?; info:LineInfo; var outLine:int&; var outFile:FileInfo?& ) : string
    }

Reader macros are invoked via the ``% READER_MACRO_NAME ~ character_sequence`` syntax.
The ``accept`` function notifies the correct terminator of the character sequence:

.. code-block:: das

    require arr_macro_mod       // the module below, which registers `arr`

    var x = %arr~\{\}\w\x\y\n%% // invoking reader macro arr, %% is a terminator

Consider the implementation for the example above.  Like every macro, it lives in its own
module, so that the reader macro is registered before the file using it is parsed:

.. das-doc: file arr_macro_mod.das
.. code-block:: das

    module arr_macro_mod public

    require daslib/ast
    require daslib/ast_boost
    require strings

    [reader_macro(name="arr")]
    class ArrayReader : AstReaderMacro {
        def override accept ( prog:ProgramPtr; mod:Module?; var expr:ExprReader?; ch:int; info:LineInfo ) : bool {
            append(expr.sequence,ch)
            if ( ends_with(expr.sequence,"%%") ) {
                let len = length(expr.sequence)
                resize(expr.sequence,len-2)
                return false
            } else {
                return true
            }
        }
        def override visit ( prog:ProgramPtr; mod:Module?; expr:ExprReader? ) : ExpressionPtr {
            let seqStr = string(expr.sequence)
            var arrT = make_fixed_array_type(length(seqStr), new TypeDecl(baseType=Type.tInt))
            var mkArr = new ExprMakeArray(at = expr.at, makeType = arrT)
            for ( x in seqStr ) {
                var mkC = new ExprConstInt(at=expr.at, value=x)
                push(mkArr.values,mkC)
            }
            return mkArr
        }
    }

The ``accept`` function macro collects symbols in the sequence.
Once the sequence ends with the terminator sequence %%, ``accept`` returns false to indicate the end of the sequence.

In ``visit``, the collected sequence is converted into a make array ``[ch1,ch2,..]`` expression.

More complex examples include the JsonReader macro in :ref:`daslib/json_boost <stdlib_json_boost>` or RegexReader in :ref:`daslib/regex_boost <stdlib_regex_boost>`.

``suffix`` is an alternative to ``visit`` — it is called immediately after ``accept`` during parsing,
before the AST is built.  Instead of returning an AST node, it returns a **string** of daScript source
code that the parser re-parses.  This is useful for generating top-level declarations (functions,
structs) from custom syntax.  When used at module level the ``ExprReader`` node is discarded,
and the suffix text is the only output.  ``SpoofInstanceReader`` in ``daslib/spoof.das`` is an example.

The ``outLine`` and ``outFile`` parameters allow remapping line information for error reporting in the
injected code.

Reader macros are normally invoked with the ``~`` separator (``%name~ ... %%``). There is a second,
**inline** form that uses a ``!`` separator (``%name! ... %%``) and runs ``suffix`` **in expression
position**:

.. das-doc: fragment
.. code-block:: das

    var total = %sum! 1, 2, 3 %%   // rewrites to ( 1 + 2 + 3 ), re-parsed in place

With ``%name!`` the collected body is handed to ``suffix`` and the returned source is spliced back
exactly where the macro appears, so the macro is itself an expression. The rewrite must be a single,
complete (parenthesized) expression. This is the basis for source-to-source DSLs embedded in
expressions. The ``~`` form's ``suffix`` only works at module level — used inline its discarded
``ExprReader`` node would strand the host statement, which is why the ``!`` form exists.

.. seealso::

   :ref:`Tutorial: Reader Macros <tutorial_macro_reader_macro>` — step-by-step example
   of both visit and suffix patterns.

------------
AstCallMacro
------------

``AstCallMacro`` operates on expressions which have function call syntax or something similar.
It occurs during the infer pass.

``add_new_call_macro`` adds a call macro to a module.
The ``[call_macro]`` annotation automates the same thing:

.. das-doc: signatures
.. code-block:: das

    class AstCallMacro {
        def abstract preVisit ( prog:ProgramPtr; mod:Module?; expr:ExprCallMacro? ) : void
        def abstract visit ( prog:ProgramPtr; mod:Module?; expr:ExprCallMacro? ) : ExpressionPtr
        def abstract canVisitArgument ( expr:ExprCallMacro?; argIndex:int ) : bool
        def abstract canFoldReturnResult ( expr:ExprCallMacro? ) : bool
    }

``apply`` from :ref:`daslib/apply <stdlib_apply>` is an example of such a macro:

.. das-doc: fragment
.. code-block:: das

    [call_macro(name="apply")]  // apply(value, block)
    class ApplyMacro : AstCallMacro {
        def override visit ( prog:ProgramPtr; mod:Module?; var expr:ExprCallMacro? ) : ExpressionPtr {
            ...
        }
    }

Note how the name is provided in the ``[call_macro]`` annotation.

``preVisit`` is called before the arguments are visited.

``visit`` is called after the arguments are visited.

``canVisitArgument`` is called per-argument to determine if the macro should visit that argument.

``canFoldReturnResult`` controls whether the compiler can finalize the return type while the macro is unexpanded.

------------
AstPassMacro
------------

``AstPassMacro`` is one macro to rule them all. It gets the entire program as
input and can be invoked at numerous passes:

.. das-doc: signatures
.. code-block:: das

    class AstPassMacro {
        def abstract apply(prog : ProgramPtr; mod : Module?) : bool
    }

Seven annotations control when a pass macro runs:

- ``[infer_macro]`` — after clean type inference.  Returning ``true`` re-infers.
- ``[dirty_infer_macro]`` — during each dirty inference pass.
- ``[pre_infer_macro]`` — before each inference pass, on the not-yet-inferred tree.
  Override ``canVisitPass(prog, mod, index)`` and return ``false`` to skip a pass;
  ``index`` is the pass number within the current inference run, 0 after every
  (re)start.
- ``[post_infer_macro]`` — once inference is finished, before the tree is consumed
  (access flags, lint, each optimisation round).
- ``[lint_macro]`` — after successful compilation (lint phase, read-only).
- ``[global_lint_macro]`` — same as ``[lint_macro]`` but for all modules.
- ``[optimization_macro]`` — during the optimisation loop.

``make_pass_macro`` registers a class as a pass macro.

Typically, such macros create an ``AstVisitor`` which performs the necessary
transformations via ``visit(prog, adapter)``.

.. seealso::

   :ref:`tutorial_macro_pass_macro` — step-by-step tutorial with lint and
   infer macro examples.

------------
AstTypeMacro
------------

``AstTypeMacro`` lets you define custom type expressions resolved during
type inference.  It has a single method:

.. das-doc: signatures
.. code-block:: das

    class AstTypeMacro {
        def abstract visit ( prog:ProgramPtr; mod:Module?; td:TypeDeclPtr; passT:TypeDeclPtr ) : TypeDeclPtr
    }

``add_new_type_macro`` adds a type macro to a module.
The ``[type_macro(name="…")]`` annotation automates registration.

The compiler parses invocations like ``name(type<T>, N)`` in type position
into a ``TypeDecl`` with ``baseType = Type.typeMacro``.  The arguments are
stored in ``td.typeMacroExpr``:

- ``typeMacroExpr[0]`` — ``ExprConstString`` with the macro name
- ``typeMacroExpr[1..]`` — user arguments (``ExprTypeDecl`` for types,
  ``ExprConstInt`` for integers, etc.)

``visit()`` is called in two contexts:

- **Concrete** — all types are inferred; ``passT`` is null;
  ``typeMacroExpr[i]._type`` is the resolved type.
- **Generic** — type parameters like ``auto(TT)`` are unresolved;
  ``passT`` carries the actual argument type for matching;
  ``typeMacroExpr[i]._type`` is null.

.. seealso::

   :ref:`tutorial_macro_type_macro` — step-by-step tutorial showing
   concrete and generic type-macro usage.

----------------
AstTypeInfoMacro
----------------

``AstTypeInfoMacro`` is designed to implement custom type information inside a typeinfo expression:

.. das-doc: signatures
.. code-block:: das

    class AstTypeInfoMacro {
        def abstract getAstChange ( expr:ExprTypeInfo?; var errors:das_string ) : ExpressionPtr
        def abstract getAstType ( var lib:ModuleLibrary; expr:ExprTypeInfo?; var errors:das_string ) : TypeDeclPtr
    }

``add_new_typeinfo_macro`` adds a typeinfo macro to a module.
There is additionally the ``[typeinfo_macro]`` annotation, which essentially automates the same thing.

The ``typeinfo`` expression uses gen2 syntax with the trait name **outside** the
parentheses::

    typeinfo trait_name(type<T>)                // basic
    typeinfo trait_name<subtrait>(type<T>)      // with subtrait
    typeinfo trait_name<sub;extra>(type<T>)     // with subtrait and extratrait

``getAstChange`` returns a newly generated AST node for the typeinfo expression.
Alternatively, it returns null if no changes are required, or if there is an error.
In case of error, the errors string must be filled.

``getAstType`` returns the type of the new typeinfo expression.

.. seealso::

   Tutorial: :ref:`tutorial_macro_typeinfo_macro` — step-by-step guide with three
   ``getAstChange`` examples (struct description, enum names, method check).

---------------
AstForLoopMacro
---------------

``AstForLoopMacro`` is designed to implement custom processing of for loop expressions:

.. das-doc: signatures
.. code-block:: das

    class AstForLoopMacro {
        def abstract visitExprFor ( prog:ProgramPtr; mod:Module?; expr:ExprFor? ) : ExpressionPtr
    }

``add_new_for_loop_macro`` adds a for-loop macro to a module.
There is additionally the ``[for_loop_macro]`` annotation, which essentially automates the same thing.

``visitExprFor`` is similar to that of ``AstVisitor``. It returns a new expression, or null if no changes are required.

---------------
AstCaptureMacro
---------------

``AstCaptureMacro`` is designed to implement custom capturing and finalization of lambda expressions:

.. das-doc: signatures
.. code-block:: das

    class AstCaptureMacro {
        def abstract captureExpression ( prog:Program?; mod:Module?; expr:ExpressionPtr; etype:TypeDeclPtr ) : ExpressionPtr
        def abstract captureFunction ( prog:Program?; mod:Module?; var lcs:Structure?; var fun:FunctionPtr ) : void
        def abstract releaseFunction ( prog:Program?; mod:Module?; var lcs:Structure?; var fun:FunctionPtr ) : void
    }

``add_new_capture_macro`` adds a capture macro to a module.
There is additionally the ``[capture_macro]`` annotation, which essentially automates the same thing.

``captureExpression`` is called per captured variable when the lambda struct is being built.
It returns a replacement expression to wrap the capture, or null if no changes are required.

``captureFunction`` is called once after the lambda function is generated.
Use this to inspect captured fields (``lcs``) and append code to ``(fun.body as ExprBlock).finalList`` —
which runs **after each invocation** (per-call finally), not on destruction.

``releaseFunction`` is called once when the lambda **finalizer** is generated.
``fun`` is the finalizer function (not the lambda call function).
Code appended to ``(fun.body as ExprBlock).list`` runs on **destruction** —
after the user-written ``finally {}`` block but before the compiler-generated
field cleanup (``delete *__this``).

.. seealso::

   :ref:`Tutorial: Capture Macros <tutorial_macro_capture_macro>` — step-by-step example
   using all three hooks with an ``[audited]`` tag annotation.

----------------
AstCommentReader
----------------

``AstCommentReader`` is designed to implement custom processing of comment expressions:

.. das-doc: signatures
.. code-block:: das

    class AstCommentReader {
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
        def abstract beforeVariantEntries ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterVariantEntry ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterVariantEntries ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterVariant ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract beforeTuple ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract beforeTupleEntries ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterTupleEntry ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterTupleEntries ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterTuple ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract beforeBitfield ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract beforeBitfieldEntries ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterBitfieldEntry ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterBitfieldEntries ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterBitfield ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract beforeEnumeration ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract beforeEnumerationEntries ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterEnumerationEntry ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterEnumerationEntries ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterEnumeration ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract beforeAlias ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        def abstract afterAlias ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
    }

``add_new_comment_reader`` adds a comment reader to a module.
There is additionally the ``[comment_reader]`` annotation, which essentially automates the same thing.

``open`` occurs when the parsing of a comment starts.

``accept`` occurs for every character of the comment.

``close`` occurs when a comment is over.

``beforeStructure`` and ``afterStructure`` occur before and after each structure or class declaration, regardless of if it has comments.

``beforeStructureFields`` and ``afterStructureFields`` occur before and after each structure or class field, regardless of if it has comments.

``afterStructureField`` occurs after each field declaration.

``beforeFunction`` and ``afterFunction`` occur before and after each function declaration, regardless of if it has comments.

``beforeGlobalVariables`` and ``afterGlobalVariables`` occur before and after each global variable declaration, regardless of if it has comments.

``afterGlobalVariable`` occurs after each individual global variable declaration.

``beforeVariant`` and ``afterVariant`` occur before and after each variant declaration, regardless of if it has comments.
The same pair exists for tuples (``beforeTuple`` / ``afterTuple``) and bitfields (``beforeBitfield`` / ``afterBitfield``).

``beforeEnumeration`` and ``afterEnumeration`` occur before and after each enumeration declaration, regardless of if it has comments.

Variants, tuples, bitfields and enumerations additionally report their bodies: ``before<Kind>Entries``
and ``after<Kind>Entries`` bracket the entry list, and ``after<Kind>Entry`` occurs after each single
entry, carrying its name.

``beforeAlias`` and ``afterAlias`` occur before and after each alias type declaration, regardless or if it has comments.

----------------
AstSimulateMacro
----------------

``AstSimulateMacro`` is designed to customize the simulation of the program:

.. das-doc: signatures
.. code-block:: das

    class AstSimulateMacro {
        def abstract preSimulate ( prog:Program?; ctx:Context? ) : bool
        def abstract simulate ( prog:Program?; ctx:Context? ) : bool
    }

``preSimulate`` occurs after the context has been simulated, but before all the structure and function annotation simulations.

``simulate`` occurs after all the structure and function annotation simulations.

----------
AstVisitor
----------

``AstVisitor`` implements the visitor pattern for the Daslang expression tree.
It contains a callback for every single expression in prefix and postfix form, as well as some additional callbacks:

.. das-doc: signatures
.. code-block:: das

    class AstVisitor {
        ...
        // find
            def abstract preVisitExprFind(expr:ExprFind?) : void          // prefix
            def abstract visitExprFind(expr:ExprFind?) : ExpressionPtr    // postfix
        ...
    }

Postfix callbacks can return expressions to replace the ones passed to the callback.

``PrintVisitor`` from ``daslib/ast_print`` implements the printing of every single expression in Daslang syntax.

``make_visitor`` creates a visitor adapter from the class, derived from ``AstVisitor``.
The adapter then can be applied to a program via the ``visit`` function:

.. code-block:: das

    require daslib/ast_print

    var astVisitor = new PrintVisitor()
    make_visitor(*astVisitor) $ (astVisitorAdapter) {
        visit(this_program(), astVisitorAdapter)
    }

If an expression needs to be visited, and can potentially be fully substituted, the
``visit_expression`` function from ``daslib/templates_boost`` should be used. It takes the
expression by reference and replaces it in place, so there is nothing to assign back:

.. das-doc: given var expr : ExpressionPtr
.. code-block:: das

    require daslib/templates_boost

    make_visitor(*astVisitor) $ (astVisitorAdapter) {
        visit_expression(expr,astVisitorAdapter)
    }

---------------------
Debugging a macro
---------------------

A macro that produces slightly-wrong AST does not fail where the mistake is. It
crashes the compiler several passes later, inside type inference or code
generation, with no line number and no indication of which macro was responsible.

The ``daslib/ast_verify`` module turns that into a diagnostic at the offending
node. Reach for it first, not last::

    daslang --ast-verify program.das

That form re-checks before every inference pass, so a break is caught on the
pass right after it happens, and after inference it sweeps every module on each
firing. ``--ast-verify-batch`` is the CI gate form for many files: no pre-infer
checks, and after inference only the module being compiled is checked - the
tree handed to lint, folding and codegen is valid, at a fraction of the cost.

Without it, a macro that leaves a variable untyped crashes:

.. code-block:: text

    CRASH: SIGSEGV (Segmentation fault) (signal 11) at address 0x30

With it, the same program reports the node, repairs it so the scan can go on and
every finding is printed, and fails the compile:

.. code-block:: text

    AST verify: let variable 'i' has no type (Variable._type is null) at program.das:6:9

It checks the shapes the compiler dereferences without a null check: missing
required children, null elements inside child lists, statements in positions
where a value is required, malformed types such as an array with no element type,
and declaration types on function results, arguments and structure fields.

The command-line flag checks every module before each inference pass. When
writing a macro, the inline forms see more:

``verify_module(prog, mod)``
    at the end of ``apply()``, once the tree is installed in the module.

``verify_expression(expr)``
    when the macro builds and returns a subtree. For-loop, call, variant and
    reader macros run inside inference, where the module-level form cannot yet
    see the result.

``verify_function(fn)``
    when the macro builds a function. ``add_function`` computes the mangled name
    immediately, so a malformed result or argument type fails there, before any
    later pass could report it.

Each returns the number of violations and repairs what it reports, substituting a
placeholder so the scan completes and reports every problem instead of stopping
at the first one.

Two limits are worth knowing. The check set covers shapes that crash the
compiler, so silence does not prove a tree is correct. And a macro that breaks
the same node on every pass will not converge: the verifier repairs it, the macro
breaks it again, and compilation ends with ``error[30507]: type inference
exceeded maximum allowed number of passes``. Apply such a change once.

.. seealso::

    :ref:`Annotations <annotations>` for annotation-based macro registration,
    :ref:`Reification <reification>` for AST reification used in macros,
    :ref:`Program structure <program_structure>` for the compilation lifecycle,
    :ref:`Generic programming <generic_programming>` for ``typeinfo`` macros.
