options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module json_boost shared public

require rtti public
require ast
require strings
require daslib/json public
require daslib/ast_boost
require daslib/contracts
require daslib/templates
require daslib/strings_boost
require daslib/apply
require daslib/macro_boost
require daslib/static_let

def operator ?[] (a : json::JsonValue const? ==const; key : string) : JsonValue? const {
    //! Returns the value of the key in the JSON object, if it exists.
    return a != null && a.value is _object ? (a.value as _object)?[key] ?? JVNull() : JVNull()
}

def operator ?[] (var a : json::JsonValue? ==const; key : string) : JsonValue? {
    //! Returns the value of the key in the JSON object, if it exists.
    return a != null && a.value is _object ? (a.value as _object)?[key] ?? JVNull() : JVNull()
}

def operator ?.(a : json::JsonValue const? ==const; key : string) : JsonValue? const {
    //! Returns the value of the key in the JSON object, if it exists.
    return a != null && a.value is _object ? (a.value as _object)?[key] ?? JVNull() : JVNull()
}

def operator ?.(var a : json::JsonValue? ==const; key : string) : JsonValue? {
    //! Returns the value of the key in the JSON object, if it exists.
    return a != null && a.value is _object ? (a.value as _object)?[key] ?? JVNull() : JVNull()
}

def operator ?[] (a : json::JsonValue const? ==const; idx : int) : JsonValue? const {
    //! Returns the value of the index in the JSON array, if it exists.
    return a != null && a.value is _array ? (a.value as _array)?[idx] ?? JVNull() : JVNull()
}

def operator ?[] (var a : json::JsonValue? ==const; idx : int) : JsonValue? {
    //! Returns the value of the index in the JSON array, if it exists.
    return a != null && a.value is _array ? (a.value as _array)?[idx] ?? JVNull() : JVNull()
}

def private null_coalescing(a : json::JsonValue const?; val : auto(TT)) : TT {
    //! Returns the value of the JSON object, if it exists, otherwise returns the default value.
    if (a != null) {
        if (a.value is _number) {
            return TT(a.value as _number)
        } elif (a.value is _longint) {
            return TT(a.value as _longint)
        }
    }
    return val
}

def private null_coalescing_with_string(a : json::JsonValue const?; val : auto(TT)) : TT {
    //! Returns the value of the JSON object, if it exists, otherwise returns the default value.
    if (a != null) {
        if (a.value is _number) {
            return TT(a.value as _number)
        } elif (a.value is _longint) {
            return TT(a.value as _longint)
        } elif (a.value is _string) {
            return TT(a.value as _string)
        }
    }
    return val
}

def operator ??(a : json::JsonValue const?; val : double) : double {
    //! Returns the value of the JSON object, if it exists, otherwise returns the default value.
    return null_coalescing(a, val)
}

def operator ??(a : json::JsonValue const?; val : float) : float {
    //! Returns the value of the JSON object, if it exists, otherwise returns the default value.
    return null_coalescing(a, val)
}

def operator ??(a : json::JsonValue const?; val : int8) : int8 {
    //! Returns the value of the JSON object, if it exists, otherwise returns the default value.
    return null_coalescing(a, val)
}

def operator ??(a : json::JsonValue const?; val : int16) : int16 {
    //! Returns the value of the JSON object, if it exists, otherwise returns the default value.
    return null_coalescing(a, val)
}

def operator ??(a : json::JsonValue const?; val : int) : int {
    //! Returns the value of the JSON object, if it exists, otherwise returns the default value.
    return null_coalescing(a, val)
}

def operator ??(a : json::JsonValue const?; val : int64) : int64 {
    //! Returns the value of the JSON object, if it exists, otherwise returns the default value.
    return null_coalescing_with_string(a, val)
}

def operator ??(a : json::JsonValue const?; val : uint8) : uint8 {
    //! Returns the value of the JSON object, if it exists, otherwise returns the default value.
    return null_coalescing(a, val)
}

def operator ??(a : json::JsonValue const?; val : uint16) : uint16 {
    //! Returns the value of the JSON object, if it exists, otherwise returns the default value.
    return null_coalescing(a, val)
}

def operator ??(a : json::JsonValue const?; val : uint) : uint {
    //! Returns the value of the JSON object, if it exists, otherwise returns the default value.
    return null_coalescing(a, val)
}

def operator ??(a : json::JsonValue const?; val : uint64) : uint64 {
    //! Returns the value of the JSON object, if it exists, otherwise returns the default value.
    return null_coalescing_with_string(a, val)
}

def operator ??(a : json::JsonValue const?; val : bool) : bool {
    //! Returns the value of the JSON object, if it exists, otherwise returns the default value.
    return a != null && a.value is _bool ? a.value as _bool : val
}

def operator ??(a : json::JsonValue const?; val : string) : string {
    //! Returns the value of the JSON object, if it exists, otherwise returns the default value.
    return a != null && a.value is _string ? a.value as _string : val
}

def operator ?. value(a : JsonValue const? ==const) : JsValue const? {
    //! Returns the value of the JSON object, if it exists.
    return a . ?. value
}

def operator ?. value(var a : JsonValue? ==const) : JsValue? {
    //! Returns the value of the JSON object, if it exists.
    return a . ?. value
}

[macro_function]
def private is_json_ptr_value(td : TypeDeclPtr) {
    if (td.baseType != Type.tPointer) {
        return false
    }
    if (td.firstType == null) {
        return false
    }
    if (td.firstType.baseType != Type.tStructure) {
        return false
    }
    let st = td.firstType.structType
    if (st.name != "JsonValue" && st._module.name != "json") {
        return false
    }
    return true
}


// replacing ExprIsVariant(value,name) => ExprIsVariant(value.value,name)
// replacing ExprAsVariant(value,name) => ExprAsVariant(value.value,name)
// if value is json::JsonValue?
[variant_macro(name="better_json")]
class private BetterJsonMacro : AstVariantMacro {
    //! This macro is used to implement `is json_value` and `as json_value` runtime checks.
    //! It essencially substitutes `value as name` with `value.value as name` and `value is name` with `value.value is name`.
    def override visitExprIsVariant(prog : ProgramPtr; mod : Module?; expr : smart_ptr<ExprIsVariant>) : ExpressionPtr {
        if (is_json_ptr_value(expr.value._type)) {
            var inscope vdr <- new ExprField(at = expr.at, name := "value", value <- clone_expression(expr.value))
            var inscope isv <- new ExprIsVariant(at = expr.at, name := expr.name, value <- vdr)
            isv.genFlags = expr.genFlags
            return <- isv
        }
        return <- default<ExpressionPtr>
    }
    def override visitExprAsVariant(prog : ProgramPtr; mod : Module?; expr : smart_ptr<ExprAsVariant>) : ExpressionPtr {
        if (is_json_ptr_value(expr.value._type)) {
            var inscope vdr <- new ExprField(at = expr.at, name := "value", value <- clone_expression(expr.value))
            var inscope isv <- new ExprAsVariant(at = expr.at, name := expr.name, value <- vdr)
            isv.genFlags = expr.genFlags
            return <- isv
        }
        return <- default<ExpressionPtr>
    }
    def override visitExprSafeAsVariant(prog : ProgramPtr; mod : Module?; expr : smart_ptr<ExprSafeAsVariant>) : ExpressionPtr {
        if (is_json_ptr_value(expr.value._type)) {
            var inscope vdr <- new ExprField(at = expr.at, name := "value", value <- clone_expression(expr.value))
            var inscope isv <- new ExprSafeAsVariant(at = expr.at, name := expr.name, value <- vdr)
            isv.genFlags = expr.genFlags
            return <- isv
        }
        return <- default<ExpressionPtr>
    }
}

[reader_macro(name="json")]
class private JsonReader : AstReaderMacro {
    //! This macro implements embedding of the JSON object into the program::
    //!   var jsv = %json~
    //!   {
    //!     "name": "main_window",
    //!     "value": 500,
    //!     "size": [1,2,3]
    //!   } %%
    def override accept(prog : ProgramPtr; mod : Module?; var expr : ExprReader?; ch : int; info : LineInfo) : bool {
        append(expr.sequence, ch)
        if (ends_with(expr.sequence, "%%")) {
            let len = length(expr.sequence)
            resize(expr.sequence, len - 2)
            return false
        } else {
            return true
        }
    }
    def override visit(prog : ProgramPtr; mod : Module?; expr : smart_ptr<ExprReader>) : ExpressionPtr {
        let seqStr = string(expr.sequence)
        var error = ""
        var json = read_json(seqStr, error)
        macro_verify(json != null, prog, expr.at, "json did not parse ({error})")
        return <- convert_to_expression(json, expr.at)
    }
}

[expect_any_enum(ent), template(ent), unused_argument(ent)]
def from_JV(v : JsonValue const explicit?; ent : auto(EnumT); defV : EnumT = default<EnumT>) : EnumT {
    //! Parse a JSON value and return the corresponding native value.
    if (v == null || !((v.value is _string) || (v.value is _number) || (v.value is _longint))) {
        return defV
    }
    if (v.value is _string) {
        let name = v.value as _string
        var res : auto(EnumTT) = default<EnumT>
        var ti = typeinfo rtti_typeinfo(type<EnumT>)
        for (ef in *ti.enumType) {
            if (name == ef.name) {
                unsafe {
                    res = reinterpret<EnumTT>(ef.value)
                }
                return res
            }
        }
        panic("not a valid enumeration {name} in {typeinfo typename(type<EnumT>)}")
    } else {
        let intValue = (v.value is _number) ? int(v.value as _number) : int(v.value as _longint)
        for (it in each_enum(defV)) {
            if (intValue == int(it)) {
                return it
            }
        }
        panic("not a valid enumeration {intValue} in {typeinfo typename(type<EnumT>)}")
    }
    return defV
}

[generic, template(ent), unused_argument(ent)]
def from_JV(v : JsonValue const explicit?; ent : string; defV : string = "") {
    if (v == null || !(v.value is _string)) {
        return defV
    } else {
        return v.value as _string
    }
}

[generic, template(ent), unused_argument(ent)]
def from_JV(v : JsonValue const explicit?; ent : bool; defV : bool = false) {
    if (v == null || !(v.value is _bool)) {
        return defV
    } else {
        return v.value as _bool
    }
}

[generic, template(ent), unused_argument(ent)]
def from_JV(v : JsonValue const explicit?; ent : float; defV : float = 0.) {
    return null_coalescing(v, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_JV(v : JsonValue const explicit?; ent : double; defV : double = 0.lf) {
    return null_coalescing(v, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_JV(v : JsonValue const explicit?; ent : int; defV : int = 0) {
    return null_coalescing(v, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_JV(v : JsonValue const explicit?; ent : uint; defV : uint = 0u) {
    return null_coalescing(v, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_JV(v : JsonValue const explicit?; ent : int64; defV : int64 = 0l) {
    return null_coalescing_with_string(v, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_JV(v : JsonValue const explicit?; ent : uint64; defV : uint64 = 0ul) {
    return null_coalescing_with_string(v, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_JV(v : JsonValue const explicit?; ent : int8; defV : int8 = int8(0)) {
    return null_coalescing(v, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_JV(v : JsonValue const explicit?; ent : uint8; defV : uint8 = uint8(0)) {
    return null_coalescing(v, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_JV(v : JsonValue const explicit?; ent : int16; defV : int16 = int16(0)) {
    return null_coalescing(v, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_JV(v : JsonValue const explicit?; ent : uint16; defV : uint16 = uint16(0)) {
    return null_coalescing(v, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_JV(v : JsonValue const explicit?; ent : bitfield; defV : bitfield = bitfield()) {
    return null_coalescing(v, defV)
}

[expect_any_vector_type(v)]
def JV(v : auto(VecT)) {
    //! Creates `JsonValue` out of value.
    static_if (typeinfo vector_dim(type<VecT>) == 2) {
        return JV({"x" => JV(v.x), "y" => JV(v.y)})
    } static_elif (typeinfo vector_dim(type<VecT>) == 3) {
        return JV({"x" => JV(v.x), "y" => JV(v.y), "z" => JV(v.z)})
    } static_elif (typeinfo vector_dim(type<VecT>) == 4) {
        return JV({"x" => JV(v.x), "y" => JV(v.y), "z" => JV(v.z), "w" => JV(v.w)})
    }
}

[expect_any_vector_type(ent), template(ent), unused_argument(ent)]
def from_JV(v : JsonValue const explicit?; ent : auto(VecT); defV : VecT = VecT()) {
    if (v == null) {
        return defV
    }
    let VecV : auto(VecTT) = VecT()
    assume vo = v
    static_if (typeinfo vector_dim(VecV) == 2) {
        return VecTT(vo?["x"] ?? defV.x, vo?["y"] ?? defV.y)
    } static_elif (typeinfo vector_dim(VecV) == 3) {
        return VecTT(vo?["x"] ?? defV.x, vo?["y"] ?? defV.y, vo?["z"] ?? defV.z)
    } static_elif (typeinfo vector_dim(VecV) == 4) {
        return VecTT(vo?["x"] ?? defV.x, vo?["y"] ?? defV.y, vo?["z"] ?? defV.z, vo?["w"] ?? defV.w)
    }
}

[template(anything), unused_argument(anything)]
def from_JV(v : JsonValue const explicit?; anything : table< auto(KT); auto(VT) >) {
    var ret : table<KT; VT>
    if (v == null || !(v.value is _object)) {
        return <- ret
    }
    unsafe {
        let tab & = v.value as _object
        for (k, vv in keys(tab), values(tab)) {
            move_to_ref(ret[k], _::from_JV(vv, type<VT -&>))
        }
    }
    return <- ret
}

struct public JsonFieldState {
    argName : string
    enumAsInt : bool
    unescape : bool
    embed : bool
    optional : bool
}

def public parse_json_annotation(name : string; annotation : array<tuple<name : string; data : RttiValue>>) : JsonFieldState {
    var fieldState : JsonFieldState
    fieldState.argName = name
    for (ann in annotation) {
        if (ann.name == "rename") {
            fieldState.argName = ann.data as tString
        } elif (ann.name == "enum_as_int" && ann.data is bool) {
            fieldState.enumAsInt = ann.data as tBool
        } elif (ann.name == "unescape" && ann.data is bool) {
            fieldState.unescape = ann.data as tBool
        } elif (ann.name == "embed" && ann.data is bool) {
            fieldState.embed = ann.data as tBool
        } elif (ann.name == "optional" && ann.data is bool) {
            fieldState.optional = ann.data as tBool
        }
    }
    return <- fieldState
}

[unused_argument(anything)]
def from_JV(v : JsonValue const explicit?; anything : auto(TT)) {
    static_if (typeinfo is_dim(anything)) {
        var ret : TT[typeinfo dim(anything)]
        if (v == null || !(v.value is _array)) {
            return <- ret
        }
        unsafe {
            let arr & = v.value as _array
            if (arr |> length != typeinfo dim(anything)) {
                return <- ret
            }
            for (i in 0..typeinfo dim(anything)) {
                move_to_ref(ret[i], _::from_JV(arr[i], decltype_noref(anything[0])))
            }
            return <- ret
        }
    } static_elif (typeinfo is_struct(anything)) {
        var ret <- default<TT -const>
        if (v == null || !(v.value is _object)) {
            return <- ret
        }
        apply(ret) <| $(name : string; var field; annotations) {
            static_let("{typeinfo typename(type<TT-const-&>)}`{name}") {
                var fieldState : JsonFieldState
                var fieldStateParsed : bool = false
            }
            if (!fieldStateParsed) {
                fieldState <- parse_json_annotation(name, annotations)
                fieldStateParsed = true
            }
            (v as _object) |> get(fieldState.argName) <| $(val) {
                move_to_ref(field, _::from_JV(val, decltype_noref(field)))
            }
        }
        return <- ret
    } static_elif (typeinfo is_tuple(anything)) {
        var ret <- default<TT -const>
        if (v == null || !(v.value is _object)) {
            return <- ret
        }
        apply(ret) <| $(name : string; var field) {
            (v as _object) |> get(name) <| $(val) {
                move_to_ref(field, _::from_JV(val, decltype_noref(field)))
            }
        }
        return <- ret
    } static_elif (typeinfo is_array(anything)) {
        var ret : TT
        if (v == null || !(v.value is _array)) {
            return <- ret
        }
        unsafe {
            let arr & = v.value as _array
            for (i in range(arr |> length)) {
                if (typeinfo can_copy(anything[0])) {
                    ret |> push_clone <| _::from_JV(arr[i], decltype_noref(anything[0]))
                } else {
                    ret |> emplace <| _::from_JV(arr[i], decltype_noref(anything[0]))
                }
            }
            return <- ret
        }
    } static_elif (typeinfo is_variant(anything)) {
        var ret : TT
        if (v == null || !(v.value is _object)) {
            return <- ret
        }
        let vv : JsonValue? = (v.value as _object)?["$variant"] ?? default<JsonValue?>
        if (vv == null || !((vv.value is _number) || (vv.value is _longint))) {
            return <- ret
        }
        let index = (vv.value is _number) ? int(vv.value as _number) : int(vv.value as _longint)
        unsafe {
            set_variant_index(ret, index)
        }
        apply(ret) <| $(name : string; var field) {
            (v as _object) |> get(name) <| $(val) {
                move_to_ref(field, _::from_JV(val, decltype_noref(field)))
            }
        }
        return <- ret
    } else {
        concept_assert(false, "unsupported json serialization type. missing 'fromJV")
    }
}

def JV(value : auto(TT)) : JsonValue? {
    static_if (typeinfo is_dim(value)) {
        var arr : array<JsonValue?>
        for (t in value) {
            arr |> push(JV(t))
        }
        return _::JV(arr)
    }
    static_if (typeinfo typename(value) == "json::JsonValue?" || typeinfo typename(value) == "json::JsonValue? const") {
        return value
    } static_elif (typeinfo is_pointer(value)) {
        return value == null ? default<JsonValue?> : _::JV(*value)
    } static_elif (typeinfo is_enum(value)) {
        return JV("{value}")
    } static_elif (typeinfo is_string(value)) {
        return _::JV(value)
    } static_elif (typeinfo is_struct(value)) {
        var map : table<string; JsonValue?>
        apply(value) <| $(name : string; field; annotations) {
            static_if (typeinfo is_function(field)) {
                pass // skip functions
            } static_elif (typeinfo is_void_pointer(field)) {
                pass // skip void?
            } static_elif (typeinfo is_pointer(field)) {
                if (field != null) {
                    static_let("{typeinfo typename(type<TT-const-&>)}`{name}") {
                        var fieldState : JsonFieldState
                        var fieldStateParsed : bool = false
                    }
                    if (!fieldStateParsed) {
                        fieldState <- parse_json_annotation(name, annotations)
                        fieldStateParsed = true
                    }
                    map |> insert(fieldState.argName, _::JV(field))
                }
            } else {
                static_let("{typeinfo typename(type<TT-const-&>)}`{name}") {
                    var fieldState : JsonFieldState
                    var fieldStateParsed : bool = false
                }
                if (!fieldStateParsed) {
                    fieldState <- parse_json_annotation(name, annotations)
                    fieldStateParsed = true
                }
                if (fieldState.enumAsInt) {     // we only need to handle it as we are writing out, reading handles it at the enum level
                    static_if (typeinfo is_enum(field)) {
                        let fieldStateEnumValue : int = int(field)
                        map |> insert(fieldState.argName, _::JV(fieldStateEnumValue))
                        return
                    }
                } elif (fieldState.unescape) {
                    static_if (typeinfo is_string(field)) {
                        map |> insert(fieldState.argName, _::JV(unescape(field)))
                        return
                    }
                } elif (fieldState.embed && typeinfo is_string(field)) {
                    static_if (typeinfo is_string(field)) {
                        var errors : string
                        var subexpr = read_json(field, errors)
                        if (subexpr == null) {
                            to_log(LOG_ERROR, "JSON: failed to parse embedded json field '{fieldState.argName}': {errors}\n")
                            panic("failed to parse embedded json field '{fieldState.argName}': {errors}")
                        }
                        map |> insert(fieldState.argName, subexpr)
                        return
                    }
                }
                map |> insert(fieldState.argName, _::JV(field))
            }
        }
        return _::JV(map)
    } static_elif (typeinfo is_tuple(value)) {
        var map : table<string; JsonValue?>
        apply(value) <| $(name : string; field) {
            static_if (typeinfo is_function(field)) {
                pass // skip functions
            } static_elif (typeinfo is_void_pointer(field)) {
                pass // skip void?
            } static_elif (typeinfo is_pointer(field)) {
                if (field != null) {
                    map |> insert(name, _::JV(field))
                }
            } else {
                map |> insert(name, _::JV(field))
            }
        }
        return _::JV(map)
    } static_elif (typeinfo is_table(value)) {
        var map : table<string; JsonValue?>
        for (k, v in keys(value), values(value)) {
            map |> insert(k, _::JV(v))
        }
        return _::JV(map)
    } static_elif (typeinfo is_iterable(value)) {
        var arr : array<JsonValue?>
        for (x in value) {
            arr |> push <| _::JV(x)
        }
        return _::JV(arr)
    } static_elif (typeinfo is_variant(value)) {
        var map : table<string; JsonValue?>
        map |> insert("$variant", _::JV(variant_index(value)))
        apply(value) <| $(name : string; field) {
            map |> insert(name, _::JV(field))
        }
        return _::JV(map)
    } else {
        to_log(LOG_ERROR, "JSON: unsupported type {typeinfo typename(value)}\n")
        concept_assert(false, "unknown JV()")
        return null
    }
}


def JV(val1; val2) : JsonValue? {
    return _::JV([ _::JV(val1), _::JV(val2)])
}

def JV(val1; val2; val3) : JsonValue? {
    return _::JV([ _::JV(val1), _::JV(val2), _::JV(val3)])
}

def JV(val1; val2; val3; val4) : JsonValue? {
    return _::JV([ _::JV(val1), _::JV(val2), _::JV(val3), _::JV(val4)])
}

def JV(val1; val2; val3; val4; val5) : JsonValue? {
    return _::JV([ _::JV(val1), _::JV(val2), _::JV(val3), _::JV(val4), _::JV(val5)])
}

def JV(val1; val2; val3; val4; val5; val6) : JsonValue? {
    return _::JV([ _::JV(val1), _::JV(val2), _::JV(val3), _::JV(val4), _::JV(val5), _::JV(val6)])
}

def JV(val1; val2; val3; val4; val5; val6; val7) : JsonValue? {
    return _::JV([ _::JV(val1), _::JV(val2), _::JV(val3), _::JV(val4), _::JV(val5), _::JV(val6), _::JV(val7)])
}

def JV(val1; val2; val3; val4; val5; val6; val7; val8) : JsonValue? {
    return _::JV([ _::JV(val1), _::JV(val2), _::JV(val3), _::JV(val4), _::JV(val5), _::JV(val6), _::JV(val7), _::JV(val8)])
}

def JV(val1; val2; val3; val4; val5; val6; val7; val8; val9) : JsonValue? {
    return _::JV([ _::JV(val1), _::JV(val2), _::JV(val3), _::JV(val4), _::JV(val5), _::JV(val6), _::JV(val7), _::JV(val8), _::JV(val9)])
}

def JV(val1; val2; val3; val4; val5; val6; val7; val8; val9; val10) : JsonValue? {
    return _::JV([ _::JV(val1), _::JV(val2), _::JV(val3), _::JV(val4), _::JV(val5), _::JV(val6), _::JV(val7), _::JV(val8), _::JV(val9), _::JV(val10)])
}

