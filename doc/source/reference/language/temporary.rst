.. _temporary:

===============
Temporary types
===============

Temporary types are designed to address lifetime issues of data, which is exposed to daScript directly from C++.

Lets review the following C++ example::

    void peek_das_string(const string & str, const TBlock<void,TTemporary<const char *>> & block, Context * context) {
        vec4f args[1];
        args[0] = cast<const char *>::from(str.c_str());
        context->invoke(block, args, nullptr);
    }

C++ function here exposes pointer to c-string, internal to std::string.
From daScript perspective declaration of the function looks like this::

    def peek ( str : das_string; blk : block<(arg:string#):void> )

Where string# is a temporary version of a daScript string type.

Main idea behind temporary type is that it can't `escape` outside of the scope of the block, its passed to.

The way its accomplished is that temporary values follow certain rules.

Temporary values can't be copied or moved::

    def sample ( var t : das_string )
        var s : string
        peek(t) <| $ ( boo : string# )
            s = boo // error, can't copy temporary value

Temporary values can't be returned, or passed to functions, which require regular values::

    def accept_string(s:string)
        print("s={s}\n")

    def sample ( var t : das_string )
        peek(t) <| $ ( boo : string# )
            accept_string(boo) // error

Causes the following error::

    30304: no matching functions or generics accept_string ( string const&# )
    candidate function:
            accept_string ( s : string const ) : void
                    invalid argument s. expecting string const, passing string const&#

Value needs to be marked as ``implicit`` to accept both temporary and regular values.
However such functions implicitly promise, that the data will not be cached (copied, moved) in any form::

    def accept_any_string(s:string implicit)
        print("s={s}\n")

    def sample ( var t : das_string )
        peek(t) <| $ ( boo : string# )
            accept_any_string(boo)

Temporary values can and are intended to be cloned::

    def sample ( var t : das_string )
        peek(t) <| $ ( boo : string# )
            var boo_clone : string := boo
            accept_string(boo_clone)

Returning a temporary value is unsafe operation.
