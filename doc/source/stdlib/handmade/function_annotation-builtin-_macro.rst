Indicates that the function will be called during the macro pass, similar to `[init]`.
Here is an example from the parse_macro implementation::

    [_macro]
    def private setup   // this is called every time during initialization of any macro module
        if is_compiling_macros_in_module("parse_macro")
            compiling_module() |> add_keyword("parse", false)

