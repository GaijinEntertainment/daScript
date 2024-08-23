Specifies that class and its inherited children are used as a macro interfaces, and would not be exported by default::

    [macro_interface]
    class AstBlockAnnotation    // this class is not exported to regular modules, and only stays in the macro modules
        def abstract apply ( var blk:smart_ptr<ExprBlock>; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract finish ( var blk:smart_ptr<ExprBlock>; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool

