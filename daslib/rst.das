module rst

require math
require rtti
require ast
require daslib/ast_boost
require daslib/strings

def module_name(mod)
    let mn = string(mod.name)
    return mn=="$" ? "builtin" : mn

def alias_label_name(td:TypeDeclPtr)
    return "alias-{td.alias}"

def enum_label_name(value:Enumeration?)
    return "enum-{module_name(value._module)}-{value.name}"

[unsafe]
def enum_label_name(value:EnumerationPtr)
    return enum_label_name(reinterpret<Enumeration?> value)

def struct_label_name(value:Structure?)
    return "struct-{module_name(value._module)}-{value.name}"

[unsafe]
def struct_label_name(value:StructurePtr)
    return struct_label_name(reinterpret<Structure?> value)

def handle_label_name(value)
    return "handle-{module_name(value._module)}-{value.name}"

[unsafe]
def describe_type_short(td:TypeDecl?)
    return describe_type_short(reinterpret<TypeDeclPtr> td)

def describe_type_short(td:TypeDeclPtr)
    if !empty(td.alias) & !td.isAuto & !td.isAlias
        return string(td.alias)
    let tp = describe(td,false,false,false)
    return replace(tp,"$::","")

[unsafe]
def describe_type(td:TypeDecl?)
    return describe_type(reinterpret<TypeDeclPtr> td)

def describe_type(td:TypeDeclPtr)
    return build_string() <| $ (var writer)
        if !empty(td.alias) & !td.isAuto & !td.isAlias
            write(writer,make_ref(alias_label_name(td),string(td.alias)))
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
            write(writer,make_ref(handle_label_name(td.annotation),"{module_name(td.annotation._module)}::{td.annotation.name}"))
        elif baseType==Type tStructure
            write(writer,make_ref(struct_label_name(td.structType),"{module_name(td.structType._module)}::{td.structType.name}"))
        elif baseType==Type tEnumeration | baseType==Type tEnumeration8 | baseType==Type tEnumeration16
            write(writer,make_ref(enum_label_name(td.enumType),"{module_name(td.enumType._module)}::{td.enumType.name}"))
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

def make_domain(attr:string)
    return ".. {attr}:: "

def make_label(name:string)
    return ".. _{name}:\n\n"

def make_ref(name,text:string)
    return " :ref:`{text} <{name}>` "

def make_group(name:string; plus:string="+")
    let len = length(name)
    return "{builtin`::repeat(plus,len)}\n{name}\n{builtin`::repeat(plus,len)}\n\n"

def make_header(name,lab:string)
    let wide = length(name)
    return ".. _stdlib_{lab}:\n\n{builtin`::repeat("=",wide)}\n{name}\n{builtin`::repeat("=",wide)}\n\n"

def write_table_separator(var writer:StringBuilderWriter; wid:array<int>)
    for x in wid
        write(writer,"+{builtin`::repeat("-",x)}")
    write(writer,"+\n")

def write_table_line(var writer:StringBuilderWriter;line:array<string>;wid:array<int>)
    for x,l in wid,line
        write(writer,"+{wide(l,x)}")
    write(writer,"+\n")

def make_table(tab:array<array<string>>)
    return build_string() <| $(writer)
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
            write_table_separator(writer,wid)
            write_table_line(writer,ccc,wid)
        write_table_separator(writer,wid)
        write(writer,"\n\n")
