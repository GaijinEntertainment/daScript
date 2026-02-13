The INTERFACES module implements interface-based polymorphism for daScript.
It provides the ``[interface]`` annotation for defining abstract interfaces
with virtual method tables, supporting multiple implementations and dynamic
dispatch without class inheritance.

All functions and symbols are in "interfaces" module, use require to get access to it. ::

    require daslib/interfaces

Example: ::

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
