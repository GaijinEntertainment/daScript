The DEFER module implements the ``defer`` pattern — the ability to schedule cleanup
code to run at scope exit, similar to Go's ``defer``. The deferred block is moved
to the ``finally`` section of the enclosing scope at compile time.

All functions and symbols are in "defer" module, use require to get access to it.

.. code-block:: das

    require daslib/defer

Example:

.. code-block:: das

    require daslib/defer

    [export]
    def main() {
        print("start\n")
        defer() {
            print("cleanup runs last\n")
        }
        print("middle\n")
    }
    // output:
    // start
    // middle
    // cleanup runs last

Two placements are compile errors, because the enclosing ``finally`` would not run
once per scope exit: directly in a loop body (the loop's ``finally`` runs once, after
the loop), and at the top level of a lambda or generator body (that ``finally`` is the
lambda's finalizer and runs on ``delete``, not per call). Enclose the ``defer`` in a
bare ``{ }`` block to give it a per-iteration or per-call scope; to run code on
``delete``, write the lambda's ``finally`` explicitly.
