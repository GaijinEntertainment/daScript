options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module interfaces shared private

require daslib/ast_boost
require daslib/templates_boost
require daslib/defer
require strings
require ast

[structure_macro(name="interface")]
class InterfaceMacro : AstStructureAnnotation
    def override finish ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        for fld in st.fields
            if fld.name=="__rtti"
                continue
            if !fld._type.isFunction
                errors := "interface can only define functions. {fld.name} is not a function\n{errors}"
                return false
        return true

[structure_macro(name="implements")]
class ImplementsMacro : AstStructureAnnotation

    def override apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool

        if length(args)!=1
            errors := "implements macro takes exactly one argument"
            return false
        var iface_name : string
        if args[0].basicType == Type tBool
            iface_name = string(args[0].name)
        elif args[0].basicType == Type tString && args[0].name == "name"
            iface_name = string(args[0].sValue)
        else
            errors := "expecting [implements(InteraceName)] or [implements(name=InteraceName)]"
            return false

        var parent <- compiling_program() |> find_unique_structure(iface_name)
        if parent == null
            errors := "{iface_name} not found"
            return false

        let iface_var_name = "_interface_{iface_name}"
        let iface_class_name = "_implementation_{st.name}_{iface_name}"
        let iface_get_name = "get_{iface_name}"
        let iface_get_name_func = "{st.name}`{iface_get_name}"

        var cls <- make_class(iface_class_name, parent, st._module)
        var iface_index = -1
        for ann,ii in cls.annotations,range(100500)
            if ann.annotation.name == "interface"
                iface_index = ii
                break
        if iface_index==-1
            errors := "can only implement interfaces, {iface_name} is not an [interface]"
            return false
        cls.annotations |> erase(iface_index)

        var iface_ptr <- qmacro_type(type<$t(StructurePtr(parent))?>); defer_delete(iface_ptr)

        // add {st}::_interface_{Foo} : Foo?
        st |> add_structure_field ( iface_var_name, clone_type(iface_ptr), [[ExpressionPtr]]);

        // add function {st}::get_{Foo} : Foo?
        var fn_get_i <- qmacro_method(iface_get_name_func,get_ptr(st)) <| $ ( var self: $t(st) ) : $t(iface_ptr)
            if self.$f(iface_var_name) == null
                self.$f(iface_var_name) = new $t(cls)(unsafe(addr(self)))
            return self.$f(iface_var_name)
        add_function(st._module, fn_get_i)

        var st_ptr <- qmacro_type(type<$t(st)?>); defer_delete(st_ptr)

        // add {cls}::_self : {ST}?
        let fi = cls |> add_structure_field ( "_self", clone_type(st_ptr), [[ExpressionPtr]]);
        cls.fields[fi].annotation |> add_annotation_argument("do_not_delete",true)
        cls.fields[fi].flags |= FieldDeclarationFlags doNotDelete

        // add {cls} ( var s : {ST}? )
        var ctor <- qmacro_function("{iface_class_name}`{iface_class_name}") <| $ ( s : $t(st_ptr) )
            _self = s
        var ctor_fun <- make_class_constructor(get_ptr(cls), get_ptr(ctor))
        modify_to_class_member(get_ptr(ctor), get_ptr(cls), false, false)
        add_function(st._module, ctor)
        add_function(st._module, ctor_fun)

        // add get_{Foo} : Foo?
        st |> add_structure_field (
            iface_get_name,
            qmacro_type(type<function<(self:$t(st)):$t(iface_ptr)>>),
            new [[ExprAddr() at = st.at, target := iface_get_name_func]]
        );

        // now, for the methods
        let skipl = length(iface_name) + 1
        for fld in st.fields
            if string(fld.name) |> starts_with("{iface_name}`")
                let method_name = string(fld.name) |> slice(skipl)
                var found = false
                for cm in cls.fields
                    if cm.name == method_name
                        found = true

                        let fmethod_name = "{iface_class_name}`{method_name}"

                        cm.init := null
                        cm.init <- new [[ExprCast() at = fld.at,
                            castType <- clone_type(cm._type),
                            subexpr <- new [[ExprAddr() at = fld.at, target := fmethod_name]]
                        ]]

                        var fmethod_args : array<VariablePtr>; defer_delete(fmethod_args)
                        var fcall_args : array<ExpressionPtr>; defer_delete(fcall_args)
                        for n,t in cm._type.argNames,cm._type.argTypes
                            if n != "self"
                                fcall_args |> emplace <| new [[ExprVar() at = fld.at, name := n]]
                                fmethod_args |> emplace <| new [[Variable() name:=n, _type<-clone_type(t)]]

                        var fmethod : FunctionPtr
                        unsafe
                            if cm._type.firstType.isVoid
                                fmethod <- qmacro_method(fmethod_name,get_ptr(cls)) <| $ ( self : $t(cls); $a(fmethod_args) ) : $t(cm._type.firstType)
                                    invoke ( self._self.$f(fld.name), *self._self, $a(fcall_args) ) // TODO: return_or_call
                            else
                                fmethod <- qmacro_method(fmethod_name,get_ptr(cls)) <| $ ( self : $t(cls); $a(fmethod_args) ) : $t(cm._type.firstType)
                                    return invoke ( self._self.$f(fld.name), *self._self, $a(fcall_args) ) // TODO: return_or_call
                        add_function(st._module, fmethod)
                        break
                if !found
                    errors := "unknown interface method {method_name} in {iface_class_name}"
                    return false

        add_structure(st._module, cls)

        return true

