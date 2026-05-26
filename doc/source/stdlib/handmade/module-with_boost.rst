The WITH_BOOST module provides the ``with_`` call macro: bind one or more
array / table element references inside a block, with an automatic
container lock around the body so push / erase / resize / clear inside
the body panic at runtime instead of silently dangling. The macro
emits the lock / invoke / unlock sequence fully inline, so any arity
and any mix of array / table args work uniformly with a single
``require``.

All functions and symbols are in "with_boost" module, use require to get access to it.

.. code-block:: das

    require daslib/with_boost

Example:

.. code-block:: das

    require daslib/with_boost

    struct A {
        f1 : int
        f2 : int
    }

    [export]
    def main {
        var arr = [A(f1=1, f2=2), A(f1=3, f2=4)]

        // single-arg, default `_` binding
        with_(arr[0]) {
            _.f1 = 99
        }

        // multi-arg positional, struct + workhorse
        var ints = [10, 20, 30]
        with_(arr[1], ints[0]) $(s, n) {
            s.f1 = n + 100
        }

        print("arr[0]={arr[0].f1}, arr[1]={arr[1].f1}\n")
        // output: arr[0]=99, arr[1]=110
    }
