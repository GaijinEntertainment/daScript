Deprecated annotation is used to mark a function as deprecated.
It will generate a warning during compilation, and will not be callable from the final compiled context.
Here is an example from ast_boost::

    [deprecated]
    def find_arg(argn:string; args:AnnotationArgumentList) : RttiValue


