options gen2
options indenting = 4
options no_aot

module testing_boost shared

require daslib/ast_boost
require testing public
require testing_support public

[macro_function]
def private validate_arguments(type_name : string, args) : bool {
    let len = length(args)
    if (len == 0) {
        return true
    }
    if (len == 1) {
        return (args[0]._type.isPointer && args[0]._type.firstType.isStructure
                && args[0]._type.firstType.structType.name == type_name
                && args[0]._type.firstType.structType._module != null
                && args[0]._type.firstType.structType._module.name == "testing")
    }
    return false
}

[function_macro(name="test")]
class TestFunctionAnnotation : AstFunctionAnnotation {
    [unused_argument(group)] def override apply(var func : FunctionPtr; var group : ModuleGroup;
                                                var args : AnnotationArgumentList;
        var errors : das_string) : bool {
        if (!validate_arguments("T", func.arguments)) {
            errors := "Invalid arguments for test macro\nexpected function header: def {func.name}() or def {func.name}(t: testing::T?)"
            return false
        }
        for (arg in args) {
            if (arg.name == "name" || arg.name == "description" || arg.name == "standalone") {
                if (arg.basicType != Type.tString) {
                    errors := "[test({arg.name} = ...)] takes a string"
                    return false
                }
            } elif (arg.name == "timeout") {
                if (arg.basicType != Type.tFloat && arg.basicType != Type.tInt) {
                    errors := "[test(timeout = ...)] takes seconds as an int or float"
                    return false
                }
            } else {
                errors := ("[test] does not take an argument named '{arg.name}'; it takes " +
                    "name, description, timeout, and standalone")
                return false
            }
        }
        func.flags.exports = true
        return true
    }
}

[function_macro(name="benchmark")]
class BenchmarkFunctionAnnotation : AstFunctionAnnotation {
    [unused_argument(group, args, errors)] def override apply(var func : FunctionPtr;
                                                              var group : ModuleGroup;
        var args : AnnotationArgumentList; var errors : das_string) : bool {
        if (!validate_arguments("B", func.arguments)) {
            errors := "Invalid arguments for benchmark macro\nexpected function header: def {func.name}() or def {func.name}(b: testing::B?)"
            return false
        }
        func.flags.exports = true
        return true
    }
}
