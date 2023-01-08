options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module debug_expression shared private

require daslib/ast_boost
require daslib/templates_boost
require daslib/strings
require daslib/rtti

def debug_expression_impl ( var writer:StringBuilderWriter; expr:ExpressionPtr; tabs:int=0 )
    writer |> write("({expr.__rtti}")
    if string(expr.__rtti) |> starts_with("ExprConst")
        writer |> write(" {describe(expr)})")
        return
    var ann = unsafe(reinterpret<BasicStructureAnnotation?> get_expression_annotation(get_ptr(expr)))
    if ann==null
        writer |> write(")")
        return
    for_each_field(*ann) <| $ ( name,cppName,xtype,offset)
        if offset != -1u
            let tstr = describe(xtype)
            var p8 : int8?
            unsafe
                p8 = (reinterpret<int8?> expr) + int(offset)
            if tstr=="smart_ptr<ast::Expression>"
                let pv = unsafe(reinterpret<ExpressionPtr?> p8)
                let ts = repeat("  ",tabs+2)
                writer |> write("\n{ts}({name} ")
                debug_expression_impl(writer, *pv,tabs+2)
                writer |> write(")")
            elif tstr=="$::das_string"
                let pv = unsafe(reinterpret<das_string?> p8)
                writer |> write(" {name}=\"{*pv}\"")
            elif tstr=="int"
                let pv = unsafe(reinterpret<int?> p8)
                writer |> write(" {name}={*pv}")
            elif tstr=="uint"
                let pv = unsafe(reinterpret<uint?> p8)
                writer |> write(" {name}={*pv}")
            elif tstr=="int64"
                let pv = unsafe(reinterpret<int64?> p8)
                writer |> write(" {name}={*pv}")
            elif tstr=="uint64"
                let pv = unsafe(reinterpret<uint64?> p8)
                writer |> write(" {name}={*pv}")
            elif tstr=="bool"
                let pv = unsafe(reinterpret<bool?> p8)
                writer |> write(" {name}={*pv}")
            elif tstr=="$::dasvector`smart_ptr`Expression"
                pass
            else
                pass
                // writer |> write(" {name}={describe(xtype)}")
    for_each_field(*ann) <| $ ( name,cppName,xtype,offset)
        if offset != -1u
            let tstr = describe(xtype)
            var p8 : int8?
            unsafe
                p8 = (reinterpret<int8?> expr) + int(offset)
            if tstr=="$::dasvector`smart_ptr`Expression"
                let pv = unsafe(reinterpret<dasvector`smart_ptr`Expression?> p8)
                if length(*pv)!=0
                    let ts = repeat("  ",tabs+2)
                    writer |> write("\n{ts}[{name}\n")
                    for l,i in *pv,range(100500)
                        writer |> write("{ts}  ")
                        debug_expression_impl(writer, l,tabs+2)
                        if i != length(*pv)-1
                            writer |> write("\n")
                    writer |> write("]")
            elif tstr=="$::dasvector`smart_ptr`Variable"
                let pv = unsafe(reinterpret<dasvector`smart_ptr`Variable?> p8)
                if length(*pv)!=0
                    let ts = repeat("  ",tabs+2)
                    writer |> write("\n{ts}[{name}\n")
                    for l,i in *pv,range(100500)
                        writer |> write("{ts}  ")
                        writer |> write("{l.name}:{describe(l._type)}")
                        if l.init!=null
                            writer |> write(" = ")
                            debug_expression_impl(writer, l.init,tabs+2)
                        if i != length(*pv)-1
                            writer |> write("\n")
                    writer |> write("]")
    writer |> write(")")

def public debug_expression ( expr:ExpressionPtr )
    return build_string <| $ ( writer )
        debug_expression_impl(writer, expr)

