---
title: Templates? Templates?? Type macros!
date: 2026-01-19 09:58:46
tags:
    - daScript
    - daslang
---

This is going to be a good one. It's an iceberg.

<!-- more -->

And since it's an iceberg, you can stop going down the template rabbit hole at just about any level.

Examples are under examples/typeMacro. I suspect there will be many more, given the magnitude of the disaster.
Think C++ templates on steroids.

Here it goes. First macro1.das

    module macro1 public

    require daslib/typemacro_boost

    [template_structure(CounterType, initialValue=1)]
    struct template TemplateCounter {
        private counter : CounterType
        def TemplateCounter {
            counter = CounterType($v(initialValue))
        }
        def next {
            counter ++
        }
        def const value {
            return counter
        }
    }

And this is how we use it in example1.das

    require macro1

    def print_counter(c : $TemplateCounter < auto(CType) >) {
        print("Counter value: {c.value()}\n")
    }

    [export]
    def main {
        var ct = default < $TemplateCounter < int > >
        ct.next()
        print_counter(ct)
        print("Counter value: {ct.value()}\n")
    }

If no default value specified its a type (like CounterType), otherwise its a constant (like initialValue).
All good, case closed. Unless it brings more questions than answers, that is.

Just don't forget it's a macro, so it needs to be in the separate file.

----------
It's macro
----------

Why the separate file? Well, because its a macro - not a template. So it needs to compile for it to run for it to work in the next modules.

In fact it's a collection of macros, which write another macros, which in turn write more macros. But I'm getting ahead of myself.

Whats with the 'template' keyword? Class, structure, or function can be marked as 'template.
Then is just hangs there. Infer does not find it. It does not affect compilation. But the ast is kept.
And can be accessed by other macros.

This explains $v(initialValue) but does that mean we can now use reification on the templates? Why, yes.

    qmacro_template_class("InstanceOfTheCounter",type<TemplateCounter>) // is how example above works
    qmacro_template_function(@@some_function) // is how to do the same with the function

If that's a macro with reification, how come its CounterType and not a $(t(CounterType))?
Thats because template_structure macro adds typedef CounterType = $t(CounterType) equivalent
into the instance of the TemplateCounter.

    // did you know this is valid?
    struct Foo {
        typedef Bar = int       // its a local (to structure) typedef
        def far ( a : Bar ) ... // we can use it in all fields and methods
    }

All good, case closed. Unless it brings more questions than answers. Didn't I say it already?

---------------------------
It's a macro within a macro
---------------------------

If it's a macro, how do I write one? I want some custom stuff. I'd like to generate types and add a few related functions.

Here it goes. First macro2.das

    module macro2 public

    require daslib/typemacro_boost

    struct template TemplateCounter {
        counter : CounterType
        def TemplateCounter {
            counter = CounterType($v(initialValue))
        }
        def next {
            counter ++
        }
        def value {
            return counter
        }
    }

    def template print_counter(c : $t(resType)) {
        print("Counter value: {c.value()}\n")
    }

    [typemacro_function]
    def TemplateCounter(macroArgument, passArgument : TypeDeclPtr; counter_type : TypeDeclPtr; initialValue : int = 123) : TypeDeclPtr {
            var inscope template_type <- typeinfo ast_typedecl(type<TemplateCounter>)
            var inscope template_arguments <- [
                TypeMacroTemplateArgument(name="CounterType",  argument_type <- clone_type(counter_type))
            ]
            var extra_template_arguments <- [
                ("initialValue", "{initialValue}")
            ]
            if (passArgument != null) {
                return <- TypeDeclPtr() if (!is_typemacro_template_instance(passArgument, template_type, extra_template_arguments))
                return <- TypeDeclPtr() if (!infer_struct_aliases(passArgument.structType, template_arguments))
                return <- infer_template_types(passArgument, template_arguments)
            } else {
                return <- TypeDeclPtr() if (!verify_arguments(template_arguments))
                var struct_name = template_structure_name(template_type.structType, template_arguments, extra_template_arguments)
                var existing_struct = compiling_program().find_unique_structure(struct_name)
                return <- new TypeDecl(baseType = Type.tStructure, structType = existing_struct, at = template_type.at) if (existing_struct != null)
                var inscope resType <- qmacro_template_class(struct_name, type<TemplateCounter>)
                make_typemacro_template_instance(resType.structType, template_type.structType, extra_template_arguments)
                add_structure_aliases(resType.structType, template_arguments)

                // HERE! LOOK! CUSTOM STUFF. WE ADD print_counter FUNCTION!!!!
                var inscope print_fn <- qmacro_template_function(@@print_counter)
                add_function(compiling_module(), print_fn)

                return <- resType
            }
    }

And this is how we use it in example2.das

    require macro2

    [export]
    def main {
        var ct = default < $TemplateCounter<int> >
        ct.next()
        print_counter(ct)
        print("Counter value: {ct.value()}\n")
    }

typemacro_function here handles all the necessary interactions. Lets look at some details.

When passArgument is not null, we are dealing with a generic function.

    def foo ( a : $TemplateCounter<auto(TT)> )  // when passing arguments to foo, infer happens

We check if its a template instance via 'is_typemacro_template_instance' - it basically uses 'typemacro_template' annotation for it.
We collect alias template types with an 'infer_struct_aliases' (like CounterType in the example).
We infer generic function`s arguments with 'infer_template_types'.
If any of it fails - its not a matching generic function, and will be ignored at inferring function call.

When passArgument is null, we are dealing with a regular type declaration.

    def foo ( a : $TempalteCoutner<int >)       // no infer, passArguments are null

So we first check, if we already instanced it.
If its not instanced - we use reification (qmacro_template_class) to instance it.
We add typemacro_template annotation, and structure aliases (like CounterType in the example).

We do all the other 'custom' stuff afterwards - like add additional functions, types, enums, etc etc.
Otherwise we can just call a type, which we would otherwise generate.

It's obviously way too much boilerplate. But fear not. We can piggy-back on all that existing code-generation.

First, lets look at macro3.das - all the relevant changes

    [typemacro_template_function(TemplateCounter)]
    def makeTemplateCounter(macroArgument, passArgument : TypeDeclPtr; CounterType : TypeDeclPtr; initialValue : int = 123) : TypeDeclPtr {
        return <- default<TypeDeclPtr>
    }

    [typemacro_function]
    def TemplateCounter(macroArgument, passArgument : TypeDeclPtr; CounterType : TypeDeclPtr; initialValue : int = 123) : TypeDeclPtr {
        var inscope resType <- makeTemplateCounter(macroArgument, passArgument, CounterType, initialValue)
        if (resType != null && passArgument == null) {
            if (!is_custom_work_done(resType.structType)) {
                mark_custom_work_done(resType.structType)

                // HERE! LOOK! CUSTOM STUFF. WE ADD print_counter FUNCTION!!!!
                var inscope print_fn <- qmacro_template_function(@@print_counter)
                add_function(compiling_module(), print_fn)
            }
        }
        return <- resType
    }

typmacro_template_function does all that generation, from the example above - minus the custom part.
We also add a check to see, if we need to add functions - so that we don't do it twice.

All good, case closed.... oh wait. It's like deja vu all over again.
What about typemacro_function macro?

-------------------------------------------
It's a macro within a macro, within a macro
-------------------------------------------

I'm glad you asked. We are almost at the bottom of the iceberg (or are we?)

Behold, typemacro interface from ast.das

    [macro_interface]
    class AstTypeMacro {
        def abstract visit(prog : ProgramPtr; mod : Module?; td, passT : TypeDeclPtr) : TypeDeclPtr
    }

This is where we get macroArgument (via td) and passArgument (via passT).

macroArgument are what we pass to typemacro. For example $TemplateCounter<int>(13) is in fact TypeDecl(baseType=Type.typeMacro, dimExpr=[type<int>,13]).

passArgument is literally what was passed to a generic function. From the example1 print_counter receives TypeDecl(baseTypeType.tStructure, structType=TemplateCounter<int,1>).
Yes, really. "TemplateCounter<int,1>". Thats how template_structure_name generates name. No, you can't type it as a valid DAS structure name - but you can alias.

At the end typemacro_function takes function declaration, and converts it into a class inherited from AstTypeMacro.
It generates visit function, which in turn converts macroArguments info appropriate typemacro_function arguments.
Default values are passed, when arguments are not available (too few??). Types are checked and errors are generated.
All that wonderful boiler-plate, which you can also bypass - if you want one of those typemacro with variable number of arguments and such.

daslib/typemacro_boost is an entertaining reading, as well as one of the best examples of macro magic -
together with daslib/templates_boost, daslib/match, peg/peg, and many more to come.

All good, case closed... zomg.

qmacro_template_class and qmacro_template_function are MACROS in daslib/templates_boost.
They sit on top QRulesVisitor and TemplateVisitor.

And that is is .... NOT. They are registered via call_macro MACRO, from daslib/ast_boost.
Its turtles all the way down.
