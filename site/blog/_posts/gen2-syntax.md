---
title: options gen2
date: 2025-05-04 20:57:24
tags:   daScript
---

Its very easy to ignore reality. Ignoring effects of ignoring reality - now that's tricky.

<!-- more -->

First things first. How do we go from gen1 (or gen1.5) to gen2? Fear not::

    daslang -run-fmt -i -v2 hello_world.das

It will convert in place (hence -i).

Now, Boris, what on earth are you talking about? Why it's generation 2 syntax for daslang.
Its now enabled by default in the CodeOfPolicies. Its also -v2syntax (or -v1syntax) as a command line argument.
And of cause its an option::

    option gen2 // that is

Now, to the very first obvious question. Why new syntax?

It's simple, really. We use AI more and more. And AI likes to write code a certain way. It tries to write daslang code,
but there were missing or misleading bits. New syntax fixes a lot of that.

People tend to write code they are used to, as well. Daslang integrates into C++. Some cross-pollination is unavoidable;
turns out a tad more is needed.

There are also issues with ambiguity, as well as misc parser problems. New syntax addresses a lot of those too.

To the next obvious question. Why keep two?

As we discovered daslang really has two target audiences. One likes to write script-like code in python-like language.
Another writes foundational systems from the C++ inspired background. Both gravitate towards the DSL's, but get there from the different perspective.
Code looks very different. Syntax now reflects it.

---------------------------------------------
So, lets give it some overview. What changed?
---------------------------------------------

Most obvious one is blocks, and constructions. Blocks now require explicit { }, and constructions now require ( ) around conditions::

    for ( x in 0..5 ) {         // notice () and {}
        print("x = {x}\n" )     // notice no semicolon
    }
    if ( true ) {
        bar()
    } elif ( false ) {
        foo()
    } else {
        par()
    }
    while ( something ) {
        break
    }
    with ( someexpr ) {
        pass
    }
    unsafe {
        that_thing()
    }

Semicolon still separate expressions, but it is not required for the last one in the line.

In type, function, and other declarations, we now use column almost exclusively::

    def add ( a:int, b:float ) {    // notice , though ; still works
        return float(a) + b
    }

Field annotations are now using @ syntax::

    struct Foo {
        @do_not_delete a : Foo?
    }

All sorts of calls and member access can now be written via dot::

    let a = SomeEnum.value  // was SomeEnum value
    foo.bar(args)           // was foo->bar(args), can still use ->
    someblock(args)         // was invoke(block,args), can still use invoke

All sorts of shortcuts for data initialization, including das figuring out move vs copy::

    let a = [1,2,3,4,5]     // was let a <- [[auto 1;2;3;4;5]]
    var b = (1,2.,"3")      // was var b =  [[auto 1,2.,"3"]]
    var c = {1=>"1",2=>"2"} // was var c <- {{1=>"1"; 2=>"2"}}
    var d = Foo(a=1,b=2.)   // was [[Foo() a=1,b=2.]]
    let e = default<FooBar> // was [[FooBar]]

We now have tuple variable expansion::

    var (a,b) = some_tuple_function()
    print("a={a} b={b}, ab={a`b}\n")            // can access fields, as well as combined tuples
    for ( (a,b) in some_tuple_container() ) {   // works the same in the for loop
        print("a={a} b={b}, ab={a`b}\n")
    }

Missing comprehensions::

    var a = {for (a in 0..20); a=>"{a}"; where is_odd(a) }  // table comprehension

Capture is now spelled explicitly (old way still works with &, -> and := symbols)::

    var a <- @ capture(move(a),clone(b)) ( x:int ) {
        b.set(x)
        return a[x]
    }

Blocks (local functions, lambdas) no longer need piping on initialization. Some blocks require nothing at all::

    var a <- $ ( a,b ) { a + b; }   // local block
    foo() {                         // same as foo(${print("bar\n);})
        print("bar\n");
    }
    foo() $ ( a, b ) {              // same as foo($(a,b){print("a={a} b={b}\n")})
        print("a={a} b={b}\n")
    }

String builder now supports format::

    var f = 13.; f ++
    print("f = {f:3.2f}\n")         // same as std::fmt in C++

Typeinfo command is now outside of ( ), and < > now requires type::

    typeinfo sizeof(type<int>)      // typeinfo(sizeof type<int>)
    typeinfo has_field<foo>(bar)    // typeinfo(has_field<foo> type<bar>)

There are bunch of other small things, and I did my best documenting them.

The best part is most of advanced syntax works with gen1 as well. Only [[ ]] and similar constructions are gone,
and even those are just off via CodeOfPolicies. You can convert to version 1.5 - which will fix obsolete syntax,
and keep the 'script' portion there. Or you can go straight to version 2. Its really down to culture,
and task at hand.
