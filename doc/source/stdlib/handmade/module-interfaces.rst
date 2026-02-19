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
