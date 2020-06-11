options rtti=true

require fio
require rtti
require ast
require math
require uriparser
require daslib/rst
require daslib/ast_boost
require daslib/strings
require daslib/functional
require daslib/json
require daslib/json_boost

def topic(category:string;mod:Module?;name:string)
    return "{category}|{mod.name}|{name}"

def document_topic(doc_file:file;topic:string)
    // todo: this is where we look for topic, and if found embed text
    fwrite(doc_file,"{topic} to be documented\n\n")

def document_bitfield(doc_file:file; value:TypeDeclPtr)
    var tab : array<array<string>>
    for an,bit in value.argNames,range(0,32)
        var line : array<string>
        push(line,string(an))
        push(line,string(bit))
        push(line,string(1 << bit))
        emplace(tab,line)
    fwrite(doc_file,make_table(tab))

def document_variant(doc_file:file; value:TypeDeclPtr)
    var tab : array<array<string>>
    for an,at in value.argNames,value.argTypes
        var line : array<string>
        push(line,string(an))
        push(line,describe_type(at))
        emplace(tab,line)
    fwrite(doc_file,make_table(tab))

def document_typedefs(doc_file:file;mod:Module?)
    var any = false
    for_each_typedef(mod) <| $(name, value)
        any = true
    if !any
        return
    fwrite(doc_file,make_group("TypeAliases"))
    for_each_typedef(mod) <| $(name, value)
        fwrite(doc_file,make_label(alias_label_name(value)))
        fwrite(doc_file,make_domain("das:attribute"))
        fwrite(doc_file,name)
        if value.baseType==Type tBitfield
            fwrite(doc_file," is a bitfield\n\n")
            document_bitfield(doc_file,value)
        elif value.baseType==Type tVariant
            fwrite(doc_file,"is a variant type\n\n")
            document_variant(doc_file,value)
        else
            fwrite(doc_file," = {describe_type_short(value)}\n\n")
        document_topic(doc_file,topic("typedef",mod,name))

def document_enumeration(doc_file:file;mod:Module?;value)
    fwrite(doc_file,make_label(enum_label_name(value)))
    fwrite(doc_file,make_domain("das:attribute"))
    fwrite(doc_file,"{value.name}\n\n")
    var tab : array<array<string>>
    for en in value.list
        var line : array<string>
        push(line,string(en._0))
        push(line,describe(en._1))
        emplace(tab,line)
    fwrite(doc_file,make_table(tab))
    document_topic(doc_file,topic("enumeration",mod,string(value.name)))

def document_enumerations(doc_file:file;mod:Module?)
    var any = false
    for_each_enumeration(mod) <| $(value)
        any = true
    if !any
        return
    fwrite(doc_file,make_group("Enumerations"))
    for_each_enumeration(mod) <| $(value)
        document_enumeration(doc_file,mod,value)

def argument_needs_documenting ( tt )
    if tt.baseType == Type fakeContext
        return false
    if tt.baseType == Type fakeLineInfo
        return false
    return true

def document_function_declaration(doc_file:file;domain,func_name:string;
        argNames:array<string>;argTypes:array<TypeDecl?>;resType:TypeDeclPtr)
    fwrite(doc_file,make_domain(domain))
    fwrite(doc_file,"{func_name} (")
    for an,at,i in argNames,argTypes,range(0,100500)
        if argument_needs_documenting(at)
            if i != 0
                fwrite(doc_file,"; ")
            fwrite(doc_file,"{an}:{describe_type_short(at)}")
    fwrite(doc_file," ) ")
    if resType!=null & !resType.isVoid
        fwrite(doc_file," : {describe_type_short(resType)}")
    fwrite(doc_file,"\n\n")

def document_function_arguments(doc_file:file;argNames:array<string>;argTypes:array<TypeDecl?>)
    var tab : array<array<string>>
    for an,at in argNames,argTypes
        if argument_needs_documenting(at)
            var line : array<string>
            push(line,an)
            push(line,describe_type(at))
            emplace(tab,line)
    fwrite(doc_file,"arguments are\n\n")
    fwrite(doc_file,make_table(tab))

[unsafe]
def document_class_method(doc_file:file;mod:Module?;value;fld)
    let argNames <- [{for arg in fld._type.argNames; string(arg)}]
    let argTypes <- [{for arg in fld._type.argTypes; reinterpret<TypeDecl?> arg}]
    document_function_declaration(doc_file,"das:method","{value.name}.{fld.name}",
        argNames,argTypes,fld._type.firstType)
    if fld._type.firstType != null & !fld._type.firstType.isVoid
        fwrite(doc_file,"{fld.name} returns {describe_type(fld._type.firstType)}\n\n")
    if length(argTypes) > 1
        document_function_arguments(doc_file,argNames,argTypes)
    document_topic(doc_file,topic("method",mod,"{value.name}.{fld.name}"))

def document_classes(doc_file:file;mod:Module?)
    var any = false
    for_each_structure(mod) <| $(value)
        if value.flags.isClass
            any = true
    if !any
        return
    fwrite(doc_file,make_group("Classes"))
    for_each_structure(mod) <| $(value)
        if !value.flags.isClass
            return
        fwrite(doc_file,make_label(struct_label_name(value)))
        fwrite(doc_file,make_domain("das:class"))
        fwrite(doc_file,string(value.name))
        if value.parent!=null
            fwrite(doc_file," : {value.parent.name}")
        fwrite(doc_file,"\n\n")
        document_topic(doc_file,topic("class",mod,string(value.name)))
        var w  = 0
        for fld in value.fields
            if !is_class_method(value,fld._type) & !(fld.flags.generated) & !(fld.flags.parentType)
                w = max(w,length(fld.name))
        if w != 0
            fwrite(doc_file,"it defines as follows\n\n")
            for fld in value.fields
                if !is_class_method(value,fld._type) & !(fld.flags.generated) & !(fld.flags.parentType)
                    fwrite(doc_file,"  {wide(string(fld.name),w)} : {describe_type(fld._type)}\n")
            fwrite(doc_file,"\n")
        for fld in value.fields
            if is_class_method(value,fld._type) & !(fld.flags.generated) & !(fld.flags.parentType)
                document_class_method(doc_file,mod,value,fld)

def document_structure(doc_file:file;mod:Module?;value:StructurePtr)
    fwrite(doc_file,make_label(struct_label_name(value)))
    fwrite(doc_file,make_domain("das:attribute"))
    fwrite(doc_file,"{value.name}\n\n")
    if value.parent!=null
        fwrite(doc_file," : {value.parent.name}")
    fwrite(doc_file,"\n\n")
    var tab : array<array<string>>
    for fld in value.fields
        var line : array<string>
        push(line,string(fld.name))
        push(line,describe_type(fld._type))
        emplace(tab,line)
    fwrite(doc_file,"{value.name} fields are\n\n")
    fwrite(doc_file,make_table(tab))
    document_topic(doc_file,topic("structure",mod,string(value.name)))

def document_structures(doc_file:file;mod:Module?)
    var any = false
    for_each_structure(mod) <| $(value)
        if value.flags.isClass | value.flags.generated
            return
        any = true
    if !any
        return
    make_group("Structures")
    for_each_structure(mod) <| $(value)
        if value.flags.isClass | value.flags.generated
            return
        document_structure(doc_file,mod,value)

[unsafe]
def document_structure_annotation(doc_file:file;mod:Module?;value)
    fwrite(doc_file,make_label(handle_label_name(value)))
    fwrite(doc_file,make_domain("das:attribute"))
    fwrite(doc_file,"{value.name}\n\n")
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
        fwrite(doc_file,"{value.name} fields are\n\n")
        fwrite(doc_file,make_table(tab))
    if any_props
        var tab : array<array<string>>
        for_each_field(*ann) <| $ ( name,cppName,xtype,offset)
            if offset == -1u
                var line : array<string>
                push(line,name)
                push(line,describe_type(xtype))
                emplace(tab,line)
        fwrite(doc_file,"{value.name} properties are\n\n")
        fwrite(doc_file,make_table(tab))
    document_topic(doc_file,topic("structure_annotation",mod,string(value.name)))

[unsafe]
def document_structure_annotations(doc_file:file;mod:Module?)
    var any = false
    module_for_each_annotation(mod) <| $(value)
        if !value.isBasicStructureAnnotation
            return
        any = true
    if !any
        return
    fwrite(doc_file,make_group("Handled structures"))
    module_for_each_annotation(mod) <| $(value)
        if !value.isBasicStructureAnnotation
            return
        document_structure_annotation(doc_file,mod,value)

[unsafe]
def document_annotation(doc_file:file;mod:Module?;value)
    fwrite(doc_file,make_label(handle_label_name(value)))
    fwrite(doc_file,make_domain("das:attribute"))
    fwrite(doc_file,"{value.name}\n\n")
    document_topic(doc_file,topic("any_annotation",mod,string(value.name)))

[unsafe]
def document_annotations(doc_file:file;mod:Module?)
    var any = false
    module_for_each_annotation(mod) <| $(value)
        if !value.isTypeAnnotation | value.isBasicStructureAnnotation
            return
        any = true
    if !any
        return
    fwrite(doc_file,make_group("Handled types"))
    module_for_each_annotation(mod) <| $(value)
        if !value.isTypeAnnotation | value.isBasicStructureAnnotation
            return
        document_annotation(doc_file,mod,value)

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

[unsafe]
def document_function(doc_file:file;mod:Module?;func:FunctionPtr)
    let argNames <- [{for arg in func.arguments; string(arg.name)}]
    let argTypes <- [{for arg in func.arguments; reinterpret<TypeDecl?> arg._type}]
    document_function_declaration(doc_file,"das:function","{func.name}",
        argNames,argTypes,func.result)
    if func.result != null & !func.result.isVoid
        fwrite(doc_file,"{func.name} returns {describe_type(func.result)}\n\n")
    if func.flags.unsafeOperation
        fwrite(doc_file,make_domain("warning"))
        fwrite(doc_file,"\n  This is unsafe operation.\n\n")
    document_function_arguments(doc_file,argNames,argTypes)
    document_topic(doc_file,topic("function",mod,"{func.name}"))

def document_functions(doc_file:file;mod:Module?)
    var any = false
    ast::for_each_function(mod, "") <| $(func)
        if function_needs_documenting(func)
            any = true
    if !any
        return
    make_group("Functions")
    ast::for_each_function(mod, "") <| $(func)
        if !function_needs_documenting(func)
            return
        document_function(doc_file,mod,func)

def document_generics(doc_file:file;mod:Module?)
    var any = false
    ast::for_each_generic(mod) <| $(func)
        if function_needs_documenting(func)
            any = true
    if !any
        return
    make_group("Generics")
    ast::for_each_generic(mod) <| $(func)
        if !function_needs_documenting(func)
            return
        document_function(doc_file,mod,func)

def document(name:string;mod:Module?;fname:string)
    if mod==null
        panic("missing module {name}")
    var doc_file = fopen(fname,"wb")
    if doc_file == null
        panic("can't open {fname}")
    fwrite(doc_file,"\n")
    let mod_name = string(mod.name)
    fwrite(doc_file,make_header(name,mod_name))
    document_typedefs(doc_file,mod)
    document_enumerations(doc_file,mod)
    document_structures(doc_file,mod)
    document_structure_annotations(doc_file,mod)
    document_annotations(doc_file,mod)
    document_classes(doc_file,mod)
    document_generics(doc_file,mod)
    document_functions(doc_file,mod)
    fwrite(doc_file,"\n")
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
