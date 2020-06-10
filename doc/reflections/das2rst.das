options rtti=true

require fio
require rtti
require ast
require math
require daslib/ast_boost
require daslib/strings

var
    doc_file : FILE const?

// todo: write to appropriate file
def output(text:string)
    fwrite(doc_file,text)

def make_header(name,lab:string)
    let wide = length(name)
    output(".. _stdlib_{lab}:\n\n")
    output("{repeat("=",wide)}\n")
    output("{name}\n")
    output("{repeat("=",wide)}\n\n")

def output_table_separator(wid:array<int>)
    for x in wid
        output("+{repeat("-",x)}")
    output("+\n")

def output_table_line(line:array<string>;wid:array<int>)
    for x,l in wid,line
        output("+{wide(l,x)}")
    output("+\n")

def output_table(tab:array<array<string>>)
    let rows = length(tab)
    var cols = 0
    for c in tab
        cols = max(cols,length(c))
    var wid : array<int>
    resize(wid,cols)
    for cc in tab
        for x in range(0,cols)
            wid[x] = max(wid[x], length(cc[x]))
    for ccc in tab
        output_table_separator(wid)
        output_table_line(ccc,wid)
    output_table_separator(wid)
    output("\n")

def document_typedefs(mod:Module?)
    var any = false
    for_each_typedef(mod) <| $(name, value)
        any = true
    if !any
        return
    output("++++++++++++\n")
    output("Type aliases\n")
    output("++++++++++++\n\n")
    for_each_typedef(mod) <| $(name, value)
        output(".. das:attribute:: {name}")
        if value.baseType==Type tBitfield
            output(" is a bitfield\n\n")
            for bit in value.argNames
                output("- {bit}\n")
            output("\n")
        elif value.baseType==Type tVariant
            output(" is a variant type\n\n")
            output("it defines as follows ::\n\n")
            var w  = 0
            for nn in value.argNames
                w = max(w,length(nn))
            for n,t in value.argNames,value.argTypes
                output(" {wide(string(n),w)} : {describe(t,false)}\n")
            output("\n")
        else
            output(" = {describe(value,false)}\n")
            output("\n")
        output("to be documented\n")
        output("\n")

def document_enumerations(mod:Module?)
    var any = false
    for_each_enumeration(mod) <| $(value)
        any = true
    if !any
        return
    output("++++++++++++\n")
    output("Enumerations\n")
    output("++++++++++++\n\n")
    for_each_enumeration(mod) <| $(value)
        output(".. das:attribute:: {value.name}\n\n")
        output("to be documented\n\n")
        var tab : array<array<string>>
        for en in value.list
            var line : array<string>
            push(line,string(en._0))
            push(line,describe(en._1))
            emplace(tab,line)
        output_table(tab)
        output("\n")

def document_classes(mod:Module?)
    var any = false
    for_each_structure(mod) <| $(value)
        if value.flags.isClass
            any = true
    if !any
        return
    output("+++++++\n")
    output("Classes\n")
    output("+++++++\n\n")
    for_each_structure(mod) <| $(value)
        if !value.flags.isClass
            return
        output(".. das:class:: {value.name}\n\n")
        output("to be documented\n\n")
        var w  = 0
        for fld in value.fields
            if !is_class_method(value,fld._type) & !(fld.flags.generated) & !(fld.flags.parentType)
                w = max(w,length(fld.name))
        if w != 0
            output("it defines as follows ::\n\n")
            for fld in value.fields
                if !is_class_method(value,fld._type) & !(fld.flags.generated) & !(fld.flags.parentType)
                    output("  {wide(string(fld.name),w)} : {describe(fld._type,false)}\n")
            output("\n")
        for fld in value.fields
            if is_class_method(value,fld._type) & !(fld.flags.generated) & !(fld.flags.parentType)
                output(".. das:function:: {value.name}->{fld.name}")
                if fld._type.argTypes.length != 0
                    output(" (")
                    output(join(fld._type.argNames, ";"))
                    output(")")
                output(" \n\n")
                if fld._type.firstType != null & !fld._type.firstType.isVoid
                    output("{fld.name} returns ::\n\n")
                    output(" {describe(fld._type.firstType,false)}")
                    output("\n\n")
                if fld._type.argTypes.length > 1
                    var tab : array<array<string>>
                    for an,at in fld._type.argNames,fld._type.argTypes
                        if string(an)!="self"
                            var line : array<string>
                            push(line,string(an))
                            push(line,describe(at,false))
                            emplace(tab,line)
                    output("function arguments are\n\n")
                    output_table(tab)
                    output("\n")
                output("to be documented\n\n")
                output("\n")
        output("\n")

def document_structures(mod:Module?)
    var any = false
    for_each_structure(mod) <| $(value)
        if value.flags.isClass | value.flags.generated
            return
        any = true
    if !any
        return
    output("++++++++++\n")
    output("Structures\n")
    output("++++++++++\n\n")
    for_each_structure(mod) <| $(value)
        if value.flags.isClass | value.flags.generated
            return
        output(".. das:attribute:: {value.name}\n\n")
        output("it defines as follows ::\n\n")
        var w  = 0
        for fld in value.fields
            w = max(w,length(fld.name))
        for fld in value.fields
            output("  {wide(string(fld.name),w)} : {describe(fld._type,false)}\n")
        output("\n")
        output("to be documented\n\n")
        output("\n")

def function_needs_documenting(func:FunctionPtr)
    if func.flags.generated
        return false
    if func.flags.isClassMethod
        return false
    if func.flags.privateFunction
        return false
    if func.fromGeneric != null
        return false
    var isBuiltin = false
    peek(func._module.name) <| $(name)
        if name=="$"
            isBuiltin = true
    // skip builtin operators
    var isOperator = false
    if isBuiltin
        peek(func.name) <| $(name)
            let ch = character_at(name,0)
            if !is_alpha(ch) & !is_number(ch) & ch!='_'
                isOperator = true
    if isOperator
        return false
    // skip builtin type c'tors
    var isTypeConstructor = false
    if isBuiltin
        peek(func.name) <| $(name)
            if func.result!=null & func.result.isCtorType
                let tname = das_to_string(func.result.baseType)
                if name == tname
                    isTypeConstructor = true
    if isTypeConstructor
        return false
    // skip ones starting with _builtin or __builtin
    var isBuiltinName = false
    peek(func.name) <| $(name)
        if starts_with(name,"_builtin") | starts_with(name,"__builtin")
            isBuiltinName = true
    if isBuiltinName
        return false
    return true

def document_function(func:FunctionPtr)
    output(".. das:function:: {func.name}")
    if func.arguments.length != 0
        output("(")
        output(join([{for arg in func.arguments; string(arg.name)}],";"))
        output(")")
    output("\n\n")
    if func.result != null & !func.result.isVoid
        output("{func.name} returns ::\n\n")
        output(" {describe(func.result,false)}")
        output("\n\n")
    output("\n\n")
    if func.flags.unsafeOperation
        output(".. warning::\n\n")
        output(" This is unsafe operation.\n\n")
    if func.arguments.length !=0
        var tab : array<array<string>>
        for arg in func.arguments
            var line : array<string>
            push(line,string(arg.name))
            push(line,describe(arg._type,false))
            emplace(tab,line)
        output("function arguments are\n\n")
        output_table(tab)
        output("\n")
    output("\n")
    output("to be documented\n\n")
    output("\n")

def document_functions(mod:Module?)
    var any = false
    ast::for_each_function(mod, "") <| $(func)
        if function_needs_documenting(func)
            any = true
    if !any
        return
    output("+++++++++\n")
    output("Functions\n")
    output("+++++++++\n\n")
    ast::for_each_function(mod, "") <| $(func)
        if !function_needs_documenting(func)
            return
        document_function(func)

def document_generics(mod:Module?)
    var any = false
    ast::for_each_generic(mod) <| $(func)
        if function_needs_documenting(func)
            any = true
    if !any
        return
    output("++++++++\n")
    output("Generics\n")
    output("++++++++\n\n")
    ast::for_each_generic(mod) <| $(func)
        if !function_needs_documenting(func)
            return
        document_function(func)

def document(name:string;mod:Module?;fname:string)
    doc_file = fopen(fname,"wb")
    if doc_file == null
        panic("can't open {fname}")
    output("\n")
    let mod_name = string(mod.name)
    make_header(name,mod_name)
    document_typedefs(mod)
    document_enumerations(mod)
    document_structures(mod)
    document_classes(mod)
    document_generics(mod)
    document_functions(mod)
    output("\n")
    fclose(doc_file)

def find_module(name:string) : Module?
    var rm : Module?
    program_for_each_module(this_program()) <| $ ( mod )
        if string(mod.name)==name
            rm = mod
    verify(rm!=null,"module not found")
    return rm

[export]
def test
    let root = get_das_root()+"/doc/source/stdlib"  // todo: modify to output /temp so that we can merge changes
    document("Built-in runtime",
        get_module("$"),"{root}/builtin.rst")
    document("Runtime type information library",
        get_module("rtti"),"{root}/rtti.rst")
    document("Compile time type  and AST information library",
        get_module("ast"),"{root}/ast.rst")
    document("File input output library",
        get_module("fio"),"{root}/fio.rst")
    document("Boost package for the AST",
        find_module("ast_boost"),"{root}/ast_boost.rst")
    return true
