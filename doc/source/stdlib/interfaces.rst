
.. _stdlib_interfaces:

==========
Interfaces
==========

.. das:module:: interfaces

The INTERFACES module implements interface-based polymorphism for daslang.
It provides the ``[interface]`` annotation for defining abstract interfaces
with virtual method tables, supporting multiple implementations and dynamic
dispatch without class inheritance.

All functions and symbols are in "interfaces" module, use require to get access to it.

.. code-block:: das

    require daslib/interfaces

Example:

.. code-block:: das

    require daslib/interfaces

        [interface]
        class IGreeter {
            def abstract greet(name : string) : string
        }

        class MyGreeter {
            def greet(name : string) : string {
                return "Hello, {name}!"
            }
        }

        [export]
        def main() {
            var obj = new MyGreeter()
            print("{obj->greet("world")}\n")
            unsafe {
                delete obj
            }
        }
        // output:
        // Hello, world!



++++++++++++++++
Structure macros
++++++++++++++++

.. _handle-interfaces-interface:

.. das:attribute:: interface

Verifies that the annotated class is a valid interface — it may only contain
function-typed fields (no data members). Applied via ``[interface]`` annotation.


.. _handle-interfaces-implements:

.. das:attribute:: implements

Generates interface bindings for a struct. Creates a proxy class that delegates
interface method calls to the struct's own methods, and adds a ``get`InterfaceName``
method that lazily constructs the proxy. Applied via ``[implements(InterfaceName)]``.



