options rtti=true

require fio
require rtti
require ast
require math
require uriparser
require daslib/ast_boost
require daslib/strings
require daslib/functional
require daslib/json
require daslib/json_boost

var
    doc_file : FILE const?

def output(text:string)
    fwrite(doc_file,text)

def describe_type_short(td:TypeDeclPtr)
    if !empty(td.alias) & !td.isAuto & !td.isAlias
        return string(td.alias)
    let tp = describe(td,false,false,false)
    return replace(tp,"$::","")

def describe_type(td:TypeDeclPtr)
    return build_string() <| $ (var writer)
        if !empty(td.alias) & !td.isAuto & !td.isAlias
            write(writer," :ref:`{td.alias} <{alias_label_name(td)}>` ")
            return
        let baseType = td.baseType
        if baseType==Type alias
            if !empty(td.alias)
                write(writer,td.alias)
            else
                write(writer,"alias")
        elif baseType==Type autoinfer
            write(writer,"auto")
            if !empty(td.alias)
                write(writer,"({td.alias})")
        elif baseType==Type tHandle
            write(writer," :ref:`{module_name(td.annotation._module)}::{td.annotation.name} <{handle_label_name(td.annotation)}>` ")
        elif baseType==Type tStructure
            write(writer," :ref:`{module_name(td.structType._module)}::{td.structType.name} <{struct_label_name(td.structType)}>` ")
        elif baseType==Type tEnumeration | baseType==Type tEnumeration8 | baseType==Type tEnumeration16
            write(writer," :ref:`{module_name(td.enumType._module)}::{td.enumType.name} <{enum_label_name(td.enumType)}>` ")
        elif baseType==Type tPointer
            if td.flags.smartPtr
                write(writer,"smart_ptr<")
            if td.firstType!=null
                write(writer,describe_type(td.firstType))
            else
                write(writer,"void")
            write(writer,td.flags.smartPtr ? ">" : "?")
        elif baseType==Type tArray
            write(writer,"array")
            if td.firstType!=null
                write(writer,"<")
                write(writer,describe_type(td.firstType))
                write(writer,">")
        elif baseType==Type tTable
            write(writer,"table")
            if td.firstType!=null
                write(writer,"<")
                write(writer,describe_type(td.firstType))
                write(writer,";")
                write(writer,describe_type(td.secondType))
                write(writer,">")
        elif baseType==Type tIterator
            write(writer,"iterator")
            if td.firstType!=null
                write(writer,"<")
                write(writer,describe_type(td.firstType))
                write(writer,">")
        elif baseType==Type tBlock | baseType==Type tFunction | baseType==Type tLambda
            write(writer,das_to_string(baseType))
            write(writer,"<")
            if td.argTypes.length!=0
                write(writer,"(")
                if td.argNames.length!=0
                    var first = true
                    for at,an in td.argTypes,td.argNames
                        if first
                            first = false
                        else
                            write(writer,";")
                        write(writer,an)
                        write(writer,":")
                        write(writer,describe_type(at))
                else
                    var first = true
                    for at in td.argTypes
                        if first
                            first = false
                        else
                            write(writer,";")
                        write(writer,describe_type(at))
                write(writer,")")
                if td.firstType!=null
                    write(writer,":")
                    write(writer,describe_type(td.firstType))
            write(writer,">")
        elif baseType==Type tTuple | baseType==Type tVariant
            write(writer,das_to_string(baseType))
            write(writer,"<")
            if td.argTypes.length!=0
                if td.argNames.length!=0
                    var first = true
                    for at,an in td.argTypes,td.argNames
                        if first
                            first = false
                        else
                            write(writer,";")
                        write(writer,an)
                        write(writer,":")
                        write(writer,describe_type(at))
                else
                    var first = true
                    for at in td.argTypes
                        if first
                            first = false
                        else
                            write(writer,";")
                        write(writer,describe_type(at))
            write(writer,">")
        elif baseType==Type tBitfield
            write(writer,das_to_string(baseType))
            write(writer,"<")
            var first = true
            for an in td.argNames
                if first
                    first = false
                else
                    write(writer,";")
                write(writer,an)
            write(writer,">")
        else
            write(writer,das_to_string(baseType))
        if td.flags.constant
            write(writer," const")
        for d in td.dim
            write(writer,"[")
            write(writer,d)
            write(writer,"]")
        if td.flags.ref
            write(writer,"&")
        if td.flags.temporary
            write(writer,"#")
        if td.flags._implicit
            write(writer," implicit")
        if td.flags.explicitConst
            write(writer,"!")

def make_header(name,lab:string)
    let wide = length(name)
    output(".. _stdlib_{lab}:\n\n")
    output("{builtin`::repeat("=",wide)}\n")
    output("{name}\n")
    output("{builtin`::repeat("=",wide)}\n\n")

def output_table_separator(wid:array<int>)
    for x in wid
        output("+{builtin`::repeat("-",x)}")
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

def alias_label_name(td:TypeDeclPtr)
    return "alias_{td.alias}"

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
        output(".. _{alias_label_name(value)}:\n\n")
        output(".. das:attribute:: {name}")
        if value.baseType==Type tBitfield
            output(" is a bitfield\n\n")
            var tab : array<array<string>>
            for an,bit in value.argNames,range(0,32)
                var line : array<string>
                push(line,string(an))
                push(line,string(bit))
                push(line,string(1 << bit))
                emplace(tab,line)
            output_table(tab)
            output("\n")
        elif value.baseType==Type tVariant
            output("is a variant type\n\n")
            var tab : array<array<string>>
            for an,at in value.argNames,value.argTypes
                var line : array<string>
                push(line,string(an))
                push(line,describe_type(at))
                emplace(tab,line)
            output_table(tab)
            output("\n")
        else
            output(" = {describe_type_short(value)}\n")
            output("\n")
        output("to be documented\n")
        output("\n")

def enum_label_name(value)
    return "enum_{module_name(value._module)}_{value.name}"

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
        output(".. _{enum_label_name(value)}:\n\n")
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

def argument_needs_documenting ( tt : TypeDeclPtr )
    if tt.baseType == Type fakeContext
        return false
    if tt.baseType == Type fakeLineInfo
        return false
    return true

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
        output(".. _{struct_label_name(value)}:\n\n")
        output(".. das:class:: {value.name}")
        if value.parent!=null
            output(" : {value.parent.name}")
        output("\n\n")
        output("to be documented\n\n")
        var w  = 0
        for fld in value.fields
            if !is_class_method(value,fld._type) & !(fld.flags.generated) & !(fld.flags.parentType)
                w = max(w,length(fld.name))
        if w != 0
            output("it defines as follows\n\n")
            for fld in value.fields
                if !is_class_method(value,fld._type) & !(fld.flags.generated) & !(fld.flags.parentType)
                    output("  {wide(string(fld.name),w)} : {describe_type(fld._type)}\n")
            output("\n")
        for fld in value.fields
            if is_class_method(value,fld._type) & !(fld.flags.generated) & !(fld.flags.parentType)
                output(".. das:method:: {value.name}.{fld.name}")
                output(" ( ")
                var first = true
                for an,at in fld._type.argNames,fld._type.argTypes
                    if argument_needs_documenting(at)
                        if first
                            first = false
                        else
                            output("; ")
                        output("{an}:{describe_type_short(at)}")
                output(" ) ")
                if fld._type.firstType!=null & !fld._type.firstType.isVoid
                    output( " : {describe_type_short(fld._type.firstType)}")
                output(" \n\n")
                if fld._type.firstType != null & !fld._type.firstType.isVoid
                    output("{fld.name} returns {describe_type(fld._type.firstType)}\n\n")
                if fld._type.argTypes.length > 1
                    var tab : array<array<string>>
                    for an,at in fld._type.argNames,fld._type.argTypes
                        if argument_needs_documenting(at)
                            var line : array<string>
                            push(line,string(an))
                            push(line,describe_type(at))
                            emplace(tab,line)
                    output("method arguments are\n\n")
                    output_table(tab)
                    output("\n")
                output("to be documented\n\n")
                output("\n")
        output("\n")

def struct_label_name(value)
    return "struct-{module_name(value._module)}-{value.name}"

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
        output(".. _{struct_label_name(value)}:\n\n")
        output(".. das:attribute:: {value.name}")
        if value.parent!=null
            output(" : {value.parent.name}")
        output("\n\n")
        var tab : array<array<string>>
        for fld in value.fields
            var line : array<string>
            push(line,string(fld.name))
            push(line,describe_type(fld._type))
            emplace(tab,line)
        output("{value.name} fields are\n\n")
        output_table(tab)
        output("\n")
        output("to be documented\n\n")
        output("\n")

def module_name(mod)
    let mn = string(mod.name)
    return mn=="$" ? "builtin" : mn

def handle_label_name(value)
    return "handle_{module_name(value._module)}_{value.name}"

[unsafe]
def document_structure_annotations(mod:Module?)
    var any = false
    module_for_each_annotation(mod) <| $(value)
        if !value.isBasicStructureAnnotation
            return
        any = true
    if !any
        return
    output("++++++++++++++++++\n")
    output("Handled structures\n")
    output("++++++++++++++++++\n\n")
    module_for_each_annotation(mod) <| $(value)
        if !value.isBasicStructureAnnotation
            return
        output(".. _{handle_label_name(value)}:\n\n")
        output(".. das:attribute:: {value.name}\n\n")
        var any_fields = false
        var any_props = false
        var ann = reinterpret<BasicStructureAnnotation?> value
        for_each_field(*ann) <| $ ( name,cppName,xtype,offset)
            if offset != -1u
                any_fields = true
            else
                any_props = true
        if any_fields
            var tab : array<array<string>>
            for_each_field(*ann) <| $ ( name,cppName,xtype,offset)
                if offset != -1u
                    var line : array<string>
                    push(line,name)
                    push(line,describe_type(xtype))
                    emplace(tab,line)
            output("{value.name} fields are\n\n")
            output_table(tab)
            output("\n")
        if any_props
            var tab : array<array<string>>
            for_each_field(*ann) <| $ ( name,cppName,xtype,offset)
                if offset == -1u
                    var line : array<string>
                    push(line,name)
                    push(line,describe_type(xtype))
                    emplace(tab,line)
            output("{value.name} properties are\n\n")
            output_table(tab)
            output("\n")
        output("to be documented\n\n")
        output("\n")

[unsafe]
def document_annotations(mod:Module?)
    var any = false
    module_for_each_annotation(mod) <| $(value)
        if !value.isTypeAnnotation | value.isBasicStructureAnnotation
            return
        any = true
    if !any
        return
    output("+++++++++++++\n")
    output("Handled types\n")
    output("+++++++++++++\n\n")
    module_for_each_annotation(mod) <| $(value)
        if !value.isTypeAnnotation | value.isBasicStructureAnnotation
            return
        output(".. _{handle_label_name(value)}:\n\n")
        output(".. das:attribute:: {value.name}\n\n")
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
        output(" ( ")
        var first = true
        for arg in func.arguments
            if argument_needs_documenting(arg._type)
                if first
                    first = false
                else
                    output("; ")
                output("{arg.name}:{describe_type_short(arg._type)}")
        output(" ) ")
        if func.result!=null & !func.result.isVoid
            output( " : {describe_type_short(func.result)}")
    output("\n\n")
    if func.result != null & !func.result.isVoid
        output("{func.name} returns {describe_type(func.result)}\n\n")
    output("\n\n")
    if func.flags.unsafeOperation
        output(".. warning::\n\n")
        output(" This is unsafe operation.\n\n")
    if func.arguments.length !=0
        var tab : array<array<string>>
        for arg in func.arguments
            if argument_needs_documenting(arg._type)
                var line : array<string>
                push(line,string(arg.name))
                push(line,describe_type(arg._type))
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
    if mod==null
        panic("missing module {name}")
    doc_file = fopen(fname,"wb")
    if doc_file == null
        panic("can't open {fname}")
    output("\n")
    let mod_name = string(mod.name)
    make_header(name,mod_name)
    document_typedefs(mod)
    document_enumerations(mod)
    document_structures(mod)
    document_structure_annotations(mod)
    document_annotations(mod)
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
    document("Math library",
        get_module("math"),"{root}/math.rst")
    document("File input output library",
        get_module("fio"),"{root}/fio.rst")
    document("Random generator library",
        get_module("random"),"{root}/random.rst")
    document("Network socket library",
        get_module("network"),"{root}/network.rst")
    document("URI manipulation library based on UriParser",
        get_module("uriparser"),"{root}/uriparser.rst")
    document("Runtime type information library",
        get_module("rtti"),"{root}/rtti.rst")
    document("Compile time type  and AST information library",
        get_module("ast"),"{root}/ast.rst")
    document("Boost package for the AST",
        find_module("ast_boost"),"{root}/ast_boost.rst")
    document("String manipulation library",
        find_module("strings"),"{root}/strings.rst")
    document("Functional programming library",
        find_module("functional"),"{root}/functional.rst")
    document("JSON manipulation library",
        find_module("json"),"{root}/json.rst")
    document("Boost package for JSON",
        find_module("json_boost"),"{root}/json_boost.rst")
    return true
