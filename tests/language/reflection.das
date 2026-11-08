options gen2
options no_unused_function_arguments = false
require daslib/ast
require daslib/rtti
require strings
require daslib/strings_boost
require dastest/testing_boost public

// note: this is var because we want "sample" global variable to show up.
// The sample is gen2 and compiles under a DEFAULT CodeOfPolicies — this test
// is the in-tree witness that version_2_syntax defaults to true.
var {
    sample = "
require math
require UnitTest

let \{
    one_two = 12
\}

enum TestEnum \{
    foo
    bar
\}

[checkRange (foo=5)]
struct TestStruct \{
    a : int = 5
    b : string = \"something\"
    c : float[3][4]
    d : array<int>
    e : table<float; string>
    f : TestStruct?
    g : tuple<int; float; string>
    h : variant<i : int; f : float>
    fn : function<(a:int;b:float):string>
    lm : lambda<(a:int;b:float):string>
\}

[export]
def bar(blk : block<(a:int):float>) \{
    pass
\}

[export]
def main \{
    feint(\"hello world!\n\")
    return true
\}
"
}

def describeType(info) { // nolint:STYLE037 — one arm per reflected Type class; the ladder is the test
    var res : string
    if (info.basicType == Type.tHandle) {
        res = string(info.annotation.name)
    } elif (info.basicType == Type.tStructure) {
        res = info.structType?.name ?? "structure"
    } elif (info.basicType == Type.tEnumeration || info.basicType == Type.tEnumeration8 || info.basicType == Type.tEnumeration16 || info.basicType == Type.tEnumeration64) {
        res = info.enumType?.name ?? "enumeration"
    } elif (info.basicType == Type.tPointer) {
        res = info.firstType != null ? "{describeType(*(info.firstType))}?" : "void?"
    } elif (info.basicType == Type.tArray) {
        res = info.firstType != null ? "array<{describeType(*(info.firstType))}>" : "array"
    } elif (info.basicType == Type.tTable) {
        if (info.firstType != null && info.secondType != null) {
            res = "table<{describeType(deref(info.firstType))};{describeType(deref(info.secondType))}>"
        } else {
            res = "table"
        }
    } elif (info.basicType == Type.tTuple) {
        if (info.argTypes != null) {
            res = "tuple<" + join([for (argtype in arg_types(info)); describeType(argtype)], ";") + ">"
        } else {
            res += "tuple"
        }
    } elif (info.basicType == Type.tVariant) {
        if (info.argTypes != null && info.argNames != null) {
            res = "variant<" +  join([for (argname, argtype in arg_names(info), arg_types(info)); "{argname}:{describeType(argtype)}"], ";") + ">"
        } else {
            res += "variant"
        }
    } elif (info.basicType == Type.tFunction || info.basicType == Type.tLambda || info.basicType == Type.tBlock) {
        res = "{get_das_type_name(info.basicType)}<"
        if (info.argTypes != null) {
            res += "(" + join([for (argtype in arg_types(info)); describeType(argtype)], ";") + ")"
        }
        if (info.firstType != null) {
            if (info.argTypes != null) {
                res += ":"
            }
            res += "{describeType(*(info.firstType))}"
        }
        res += ">"
    } else {
        res = get_das_type_name(info.basicType)
    }
    res += join([for (d in each_dim(info)); d != -1 ? "[{d}]" : "[]"], "")
    if (info.isConst) {
        res += " const"
    }
    if (info.isRef) {
        res += " &"
    }
    if (info.isTemp) {
        res += "#"
    }
    if (info.isImplicit) {
        res += " implicit"
    }
    return res
}

def describeFunction(finfo) {
    feint("def {finfo.name} ")
    if (finfo.count != 0u) {
        let argT = join([for (arg in finfo); "{arg.name}:{describeType(arg)}"], "; ")
        feint("( {argT} ) ")
    }
    if (finfo.result != null) {
        feint(": {describeType(*(finfo.result))}")
    }
    feint("\n")
}

def describeValue(value) {
    if (value is nothing) {
        return ""
    } elif (value is tString) {
        return "=\"{value}\""
    } else {
        return "={value}"
    }
}

def describeVariable(glob; extra = "") {
    feint("{extra}{glob.name} : {describeType(glob)}{describeValue(get_variable_value(glob))}\n")
}

def describeStructure(sinfo) {
    var _anyAnn = false
    structure_for_each_annotation(sinfo) $(ann; annArgs) {
        let argT = join([for (arg in annArgs); "{arg.name}{describeValue(get_annotation_argument_value(arg))}"], ",")
        feint("[{ann.name}({argT})]\n")
    }
    feint("struct {sinfo.name}\n")
    for (sfield in sinfo) {
        describeVariable(sfield, "\t")
    }
}

def describeEnumeration(einfo) {
    feint("enum {einfo.name}\n")
    for (eitem in einfo) {
        feint("\t{eitem.name} = {eitem.value}\n")
    }
}

def describeAnnotation(ann) {
    if (ann.isBasicStructureAnnotation) {
        unsafe {
            let sann & = reinterpret<BasicStructureAnnotation&>(ann)
            feint("struct {sann.name} // {sann.fieldCount} {sann.cppName}\n")
            basic_struct_for_each_field(sann) $(name, cppName, xtype, offset) {
                feint("\t{name}:{describeType(xtype)} // {cppName} at {offset}\n")
            }
        }
    } elif (ann.isTypeAnnotation) {
        unsafe {
            let tann & = reinterpret<TypeAnnotation&>(ann)
            feint("{tann.name} // {tann.cppName}\n")
        }
    } else {
        feint("{ann.name}\n")
    }
}

def describeModule(mod) {
    module_for_each_enumeration(mod) $(einfo) {
        describeEnumeration(einfo)
    }
    module_for_each_structure(mod) $(sinfo) {
        describeStructure(sinfo)
    }
    module_for_each_annotation(mod) $(ann) {
        describeAnnotation(ann)
    }
    module_for_each_function(mod) $(finfo) {
        describeFunction(finfo)
    }
    module_for_each_generic(mod) $(finfo) {
        describeFunction(finfo)
    }
    module_for_each_global(mod) $(glob) {
        describeVariable(glob)
    }
}

// note: do_reflection iterates all registered modules including generics.
// module_for_each_generic crashes on dastest's suite module generics (dangling RTTI),
// so we skip full registered-module iteration and test safe subsets instead.

[test(description = "a gen2 sample program compiles under a default CodeOfPolicies and its module's enums, structs, annotations, functions and globals are walkable through reflection")]
def test_compile_sample(t : T?) {
    var compiled_ok = false
    compile("sample", sample, CodeOfPolicies()) $(ok; prog; errors) {
        if (ok) {
            compiled_ok = true
            let thisMod = get_this_module(prog)
            program_for_each_module(prog) $(mod) {
                if (mod == thisMod) {
                    describeModule(mod)
                }
            }
        } else {
            feint("failed to compile:\n{errors}\n")
        }
    }
    t |> success(compiled_ok, "sample program should compile")
}

[test(description = "walking every enum, struct, annotation, function, generic and global of the builtin $ module completes without a crash")]
def test_dollar_module(t : T?) {
    // describe the builtin "$" module — enums, structs, annotations, functions, globals
    describeModule(get_module("$"))
    t |> success(true)
}

[test(description = "the running context enumerates its own functions and global variables through debug info")]
def test_context_reflection(t : T?) {
    var fcount = 0
    context_for_each_function() $(finfo) {
        describeFunction(finfo)
        fcount++
    }
    var vcount = 0
    context_for_each_variable() $(vinfo) {
        describeVariable(vinfo)
        vcount++
    }
    t |> success(fcount > 0, "should have context functions")
    t |> success(vcount > 0, "should have context variables")
}

[test(description = "every registered module's enums, structs, annotations, functions and globals can be walked without a crash")]
def test_registered_modules_safe(t : T?) {
    // iterate registered modules — enums, structs, annotations, functions, globals
    // skip generics: module_for_each_generic can crash on some module RTTI
    var modCount = 0
    program_for_each_registered_module() $(mod) {
        module_for_each_enumeration(mod) $(einfo) {
            describeEnumeration(einfo)
        }
        module_for_each_structure(mod) $(sinfo) {
            describeStructure(sinfo)
        }
        module_for_each_annotation(mod) $(ann) {
            describeAnnotation(ann)
        }
        module_for_each_function(mod) $(finfo) {
            describeFunction(finfo)
        }
        module_for_each_global(mod) $(glob) {
            describeVariable(glob)
        }
        modCount++
    }
    t |> success(modCount > 0, "should iterate registered modules")
}

[comment(i=1, f=2.0, s="three", b=false)]
struct TiEvent {
    i : int
}

[test(description = "typeinfo sizeof, has_field and struct_get_annotation_argument fold to the declared size, field presence and annotation value")]
def test_typeinfo(t : T?) {
    let _arg : int
    let a = typeinfo sizeof(_arg)
    t |> equal(4, a)
    let b = typeinfo has_field<i>(type<TiEvent>)
    t |> equal(true, b)
    let c = typeinfo struct_get_annotation_argument<comment; s>(type<TiEvent>)
    t |> equal("three", c)
}

struct SimpleStruct {
    x : int
    y : float
}

class SimpleClass {
    value : int
}

[test(description = "typeinfo traits report int, float3 and string as local, copyable and container-placeable, and string as clonable")]
def test_typeinfo_traits_value_types(t : T?) {
    t |> run("int is local") @(t : T?) {
        t |> equal(typeinfo is_local(type<int>), true)
    }
    t |> run("int can copy") @(t : T?) {
        t |> equal(typeinfo can_copy(type<int>), true)
    }
    t |> run("int can be placed in container") @(t : T?) {
        t |> equal(typeinfo can_be_placed_in_container(type<int>), true)
    }
    t |> run("float3 is local") @(t : T?) {
        t |> equal(typeinfo is_local(type<float3>), true)
    }
    t |> run("string can copy") @(t : T?) {
        t |> equal(typeinfo can_copy(type<string>), true)
    }
    t |> run("string can clone") @(t : T?) {
        t |> equal(typeinfo can_clone(type<string>), true)
    }
}

[test(description = "typeinfo traits report a plain POD struct as local, copyable, movable and newable, with no nontrivial ctor or dtor")]
def test_typeinfo_traits_structs(t : T?) {
    t |> run("struct is local") @(t : T?) {
        t |> equal(typeinfo is_local(type<SimpleStruct>), true)
    }
    t |> run("struct can copy") @(t : T?) {
        t |> equal(typeinfo can_copy(type<SimpleStruct>), true)
    }
    t |> run("struct can move") @(t : T?) {
        t |> equal(typeinfo can_move(type<SimpleStruct>), true)
    }
    t |> run("struct can new") @(t : T?) {
        t |> equal(typeinfo can_new(type<SimpleStruct>), true)
    }
    t |> run("struct has no nontrivial ctor") @(t : T?) {
        t |> equal(typeinfo has_nontrivial_ctor(type<SimpleStruct>), false)
    }
    t |> run("struct has no nontrivial dtor") @(t : T?) {
        t |> equal(typeinfo has_nontrivial_dtor(type<SimpleStruct>), false)
    }
}

[test(description = "a pointer to a C++ handled type (FileAccess?) is local, copyable and movable but cannot be deleted")]
def test_typeinfo_traits_handled_types(t : T?) {
    t |> run("FileAccess? is local") @(t : T?) {
        t |> equal(typeinfo is_local(type<FileAccess?>), true)
    }
    t |> run("FileAccess? can copy") @(t : T?) {
        t |> equal(typeinfo can_copy(type<FileAccess?>), true)
    }
    t |> run("FileAccess? can move") @(t : T?) {
        t |> equal(typeinfo can_move(type<FileAccess?>), true)
    }
    t |> run("FileAccess? cannot delete ptr") @(t : T?) {
        t |> equal(typeinfo can_delete_ptr(type<FileAccess?>), false)
    }
}

[test(description = "typeinfo traits report arrays as local, movable and clonable, and tables as movable")]
def test_typeinfo_traits_containers(t : T?) {
    t |> run("array is local") @(t : T?) {
        t |> equal(typeinfo is_local(type<array<int>>), true)
    }
    t |> run("array can move") @(t : T?) {
        t |> equal(typeinfo can_move(type<array<int>>), true)
    }
    t |> run("array can clone") @(t : T?) {
        t |> equal(typeinfo can_clone(type<array<int>>), true)
    }
    t |> run("table can move") @(t : T?) {
        t |> equal(typeinfo can_move(type<table<string; int>>), true)
    }
}

struct TnFoo {
    bar : int
}

def tn_testInfT(var a : auto(numT)) => typeinfo typename(type<numT>)

def tn_testVarRefT(var a) => typeinfo typename(a)

def tn_testVarValT(var a) => typeinfo typename(a + 0)

def tn_arrType(var a : array<auto(arrT)>) => typeinfo typename(type<arrT>)

def tn_tabTypes(var a : table<auto(key); auto(value)>) =>
    typeinfo typename(type<key>) + ":" + typeinfo typename(type<value>)

[test(description = "typeinfo typename names auto-inferred, argument, expression, array element and table key and value types")]
def test_typename(t : T?) {
    t |> run("typeinfo typename for various types") @(t : T?) {
        var _x : auto(numT) = 3
        t |> equal(typeinfo typename(type<numT>), "int")
        t |> equal(tn_testInfT(3), "int")
        t |> equal(tn_testVarRefT(3), "int")
        t |> equal(tn_testVarValT(3), "int const")
        var y : table<string; int>
        t |> equal(tn_tabTypes(y), "string:int")
        var z : array<float>
        var zz : array<TnFoo>
        t |> equal(tn_arrType(z), "float")
        t |> equal(tn_arrType(zz), "TnFoo")
    }
}

var {
    ty_g_var_i : int = 1
    ty_g_var_f : float = 3.14
    ty_g_var_s : string = "blah"
}

def ty_test_global_var(t : T?) {
    t |> equal(ty_g_var_i, 1)
    ty_g_var_i = ty_g_var_i + 1
    t |> equal(ty_g_var_i, 2)
    t |> equal(ty_g_var_f, 3.14)
    ty_g_var_f = ty_g_var_f * 2.0
    t |> equal(ty_g_var_f, 6.28)
    t |> equal(ty_g_var_s, "blah")
    ty_g_var_s += "_blah"
    t |> equal(ty_g_var_s, "blah_blah")
}

def ty_test_local_var(t : T?) {
    var l_var_i : int = 1
    var l_var_f : float = 3.14
    var l_var_s : string = "blah"
    t |> equal(l_var_i, 1)
    l_var_i = l_var_i + 1
    t |> equal(l_var_i, 2)
    t |> equal(l_var_f, 3.14)
    l_var_f = l_var_f * 2.0
    t |> equal(l_var_f, 6.28)
    t |> equal(l_var_s, "blah")
    l_var_s += "_blah"
    t |> equal(l_var_s, "blah_blah")
}

def ty_test_arg(t : T?; var a_var_i : int = 1; var a_var_f : float = 3.14; var a_var_s : string = "blah") {
    t |> equal(a_var_i, 1)
    a_var_i = a_var_i + 1
    t |> equal(a_var_i, 2)
    t |> equal(a_var_f, 3.14)
    a_var_f = a_var_f * 2.0
    t |> equal(a_var_f, 6.28)
    t |> equal(a_var_s, "blah")
    a_var_s += "_blah"
    t |> equal(a_var_s, "blah_blah")
}

def ty_test_block_invoke(b : block<(var b_var_i : int; var b_var_f : float; var b_var_s : string) : void>) {
    invoke(b, 1, 3.14, "blah")
}

def ty_test_block_var(t : T?) {
    ty_test_block_invoke() $(var b_var_i : int; var b_var_f : float; var b_var_s : string) {
        t |> equal(b_var_i, 1)
        b_var_i = b_var_i + 1
        t |> equal(b_var_i, 2)
        t |> equal(b_var_f, 3.14)
        b_var_f = b_var_f * 2.0
        t |> equal(b_var_f, 6.28)
        t |> equal(b_var_s, "blah")
        b_var_s += "_blah"
        t |> equal(b_var_s, "blah_blah")
    }
}

[test(description = "int, float and string variables read and update the same way as globals, locals, defaulted arguments and block arguments")]
def test_types(t : T?) {
    ty_g_var_i = 1
    ty_g_var_f = 3.14
    ty_g_var_s = "blah"
    t |> run("global variables") @(t : T?) {
        ty_test_global_var(t)
    }
    t |> run("local variables") @(t : T?) {
        ty_test_local_var(t)
    }
    t |> run("argument variables") @(t : T?) {
        ty_test_arg(t)
    }
    t |> run("block variables") @(t : T?) {
        ty_test_block_var(t)
    }
}

struct StStorage {
    i8 : int8
    ui8 : uint8
    i16 : int16
    ui16 : uint16
}

[test(description = "a struct of int8, uint8, int16 and uint16 fields packs into 6 bytes and every field round-trips its value")]
def test_storage_types(t : T?) {
    t |> run("storage types sizeof and roundtrip") @(t : T?) {
        var st, sq : StStorage
        t |> equal(typeinfo sizeof(type<StStorage>), 6)
        st.i8 = int8(1)
        st.ui8 = uint8(2)
        st.i16 = int16(3)
        st.ui16 = uint16(4)
        t |> success(int(st.i8) == 1 && uint(st.ui8) == 2u && int(st.i16) == 3 && uint(st.ui16) == 4u)
        let i8 = st.i8
        sq.i8 = i8
        t |> equal(int(sq.i8), 1)
    }
}
