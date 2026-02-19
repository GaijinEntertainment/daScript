Turns a static method into a class method by adding a ``self`` argument
of the class type as the first argument, and wrapping the function body
in ``with (self) { ... }``. Applied via ``[class_method]`` annotation.
