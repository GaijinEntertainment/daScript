.. _temporary:

===============
Temporary types
===============

Temporary types are designed to address lifetime issues of data, which are exposed to Daslang directly from C++.

Let's review the following C++ example:

.. code-block:: cpp

    void peek_das_string(const string & str, const TBlock<void,TTemporary<const char *>> & block, Context * context, LineInfoArg * at) {
        vec4f args[1];
        args[0] = cast<const char *>::from(str.c_str());
        context->invoke(block, args, nullptr, at);
    }

The C++ function here exposes a pointer to a C-string, internal to std::string.
From Daslang's perspective, the declaration of the function looks like this:

.. das-doc: signatures
.. code-block:: das

    def peek ( src : das_string implicit; block : block<(arg0:string#):void> implicit ) : void

Where string# is a temporary version of a Daslang string type.

The key property of temporary types is that they cannot escape the scope of the block they are passed to.

Temporary values enforce this through the following rules.

Temporary values can't be copied or moved:

.. das-doc: expect error[30915]
.. code-block:: das

    def copy_temporary ( var t : das_string ) {
        var s : string
        peek(t) $ ( boo : string# ) {
            s = boo // error[30915]: can't copy temporary value
        }
    }

Temporary values can't be returned or passed to functions, which require regular values:

.. code-block:: das

    def accept_string(s:string) {
        print("s={s}\n")
    }

.. das-doc: expect error[30341]
.. code-block:: das

    def pass_temporary ( var t : das_string ) {
        peek(t) $ ( boo : string# ) {
            accept_string(boo) // error
        }
    }

This causes the following error:

.. code-block:: text

    error[30341]: no matching functions or generics: accept_string(string const&#)
    candidate function:
            accept_string(s: string const): void
                    invalid argument 's' (0). expecting 'string const', passing 'string const&#'

Values need to be marked as ``implicit`` to accept both temporary and regular values.
These functions implicitly promise that the data will not be cached (copied, moved) in any form:

.. code-block:: das

    def accept_any_string(s:string implicit) {
        print("s={s}\n")
    }

    def pass_implicit ( var t : das_string ) {
        peek(t) $ ( boo : string# ) {
            accept_any_string(boo)
        }
    }

Temporary values can and are intended to be cloned:

.. code-block:: das

    def clone_temporary ( var t : das_string ) {
        peek(t) $ ( boo : string# ) {
            var boo_clone : string := boo
            accept_string(boo_clone)
        }
    }

Returning a temporary value is an unsafe operation.

A pointer to the temporary value can be received for the corresponding scope via the ``safe_addr`` macro:

.. code-block:: das

    require daslib/safe_addr

    def foo {
        var a = 13
        ...
        var b = safe_addr(a)    // b is int?#, and this operation does not require unsafe
        ...
    }

.. seealso::

    :ref:`Unsafe <unsafe>` for ``implicit`` keyword usage and returning temporary values,
    :ref:`Clone <clone>` for cloning temporary values into regular ones,
    :ref:`Blocks <blocks>` for temporary scope and block lifetime,
    :ref:`Functions <functions>` for ``implicit`` in function signatures.

