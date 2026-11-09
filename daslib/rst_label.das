options gen2
options indenting = 4
options no_aot = true
options strict_smart_pointers = true

module rst_label shared private

//! RST label and short type names.
//!
//! The one place that spells a function's RST label and the short
//! type names the label hashes, shared by ``rst`` and the
//! ``rst_comment`` reader. It requires only builtin modules: the
//! reader is a parser macro, and every module parsed before it loses
//! its comments, so nothing this module requires may carry ``//!``
//! documentation.

require ast_core
require rtti_core
require strings

let private SAFE_NAME_REPLACEMENTS = [
    (text = " ", replacement = "_"),
    (text = "$", replacement = "_builtin_"),
    (text = "#", replacement = "_hh_"),
    (text = ":", replacement = "_c_"),
    (text = "@", replacement = "_at_"),
    (text = "<", replacement = "_ls_"),
    (text = ">", replacement = "_gr_"),
    (text = "`", replacement = "_rq_"),
    (text = "!", replacement = "_ex_"),
    (text = "=", replacement = "_eq_"),
    (text = "*", replacement = "_st_"),
    (text = ".", replacement = "_dot_"),
    (text = "?", replacement = "_q_"),
    (text = "&", replacement = "_ref_"),
    (text = "(", replacement = ""),
    (text = ")", replacement = ""),
    (text = "[", replacement = "_lb_"),
    (text = "]", replacement = "_rb_"),
    (text = "|", replacement = "")
]

def public safe_function_name(name : string) : string {
    //! Replaces special characters in a function name to produce a safe RST label identifier.
    var res = name
    for (r in SAFE_NAME_REPLACEMENTS) {
        res = replace(res, r.text, r.replacement)
    }
    return res
}

def public function_label_file(value : Function?; drop_args : int = 0) {
    //! Generates a unique RST label for a function based on its name and argument types.
    let mn = build_string() $(var w) {
        w |> write(value.name)
        var drop = drop_args
        for (arg in value.arguments) {
            if (drop-- > 0) continue
            let prefix = arg._type.flags.removeConstant ? "var " : ""
            if (arg._type.baseType == Type.autoinfer && arg.init != null) {
                let rtti = "{arg.init.__rtti}"
                if (rtti == "ExprConstDouble") {
                    w |> write("{prefix}double")
                } elif (rtti == "ExprConstFloat") {
                    w |> write("{prefix}float")
                } elif (rtti == "ExprConstString") {
                    w |> write("{prefix}string")
                } elif (rtti == "ExprConstBool") {
                    w |> write("{prefix}bool")
                } elif (rtti == "ExprConstInt") {
                    w |> write("{prefix}int")
                } elif (rtti == "ExprConstUInt") {
                    w |> write("{prefix}uint")
                } elif (rtti == "ExprConstInt64") {
                    w |> write("{prefix}int64")
                } elif (rtti == "ExprConstUInt64") {
                    w |> write("{prefix}uint64")
                } elif (rtti == "ExprConstInt8") {
                    w |> write("{prefix}int8")
                } elif (rtti == "ExprConstUInt8") {
                    w |> write("{prefix}uint8")
                } elif (rtti == "ExprConstInt16") {
                    w |> write("{prefix}int16")
                } elif (rtti == "ExprConstUInt16") {
                    w |> write("{prefix}uint16")
                } else {
                    w |> write("{prefix}auto")
                }
            } else {
                w |> write("{prefix}{describe_type_short(arg._type)}")
            }
        }
    }
    return "{safe_function_name(string(value.name))}-{hash(mn)}"
}

def public function_label_file(name; value : TypeDecl?&; drop_args : int = 0) {
    //! Generates a unique RST label for a function type based on its name and argument types.
    let mn = build_string() $(var w) {
        w |> write(name)
        var drop = drop_args
        for (arg in value.argTypes) {
            if (drop-- > 0) continue
            let prefix = arg.flags.removeConstant ? "var " : ""
            w |> write("{prefix}{describe_type_short(arg)}")
        }
    }
    return "{safe_function_name(string(name))}-{hash(mn)}"
}

def public cleanup_type_short(name : string implicit) {
    //! Drops the const, ref, implicit and temporary suffixes
    //! ``describe`` spells, keeping the ``#`` and ``[]`` marks.
    var res = name
    if (res |> ends_with(" const")) {
        res = res |> slice(0, -6)
    }
    if (res |> ends_with(" const&")) {
        res = res |> slice(0, -7)
    }
    if (res |> ends_with(" -const")) {
        res = res |> slice(0, -7)
    }
    if (res |> ends_with(" const#")) {
        res = "{res |> slice(0,-7)}#"
    }
    if (res |> ends_with(" const[]")) {
        res = "{res |> slice(0,-8)}[]"
    }
    if (res |> ends_with(" const[]#")) {
        res = "{res |> slice(0,-9)}[]#"
    }
    if (res |> ends_with(" const implicit")) {
        res = res |> slice(0, -15)
    }
    if (res |> ends_with(" const implicit#")) {
        res = "{res |> slice(0,-16)}#"
    }
    if (res |> ends_with(" implicit")) {
        res = res |> slice(0, -9)
    }
    if (res |> ends_with(" implicit#")) {
        res = "{res |> slice(0,-10)}#"
    }
    if (res |> ends_with(" const ==const")) {
        res = "{res |> slice(0,-14)} ==const"
    }
    return res
}

def public describe_type_short(td : TypeDecl?) { // nolint:STYLE038 - one arm per Type class
    //! Spells a type the way the reference manual does: aliases by
    //! name, containers and callables structurally, everything else
    //! through ``describe`` with its qualifiers dropped.
    if (td == null) {
        panic("describe_type_short: null type")
    }
    if (!empty(td.alias) && !td.isAuto && !td.isAlias) return string(td.alias)
    var res : string
    if (td.baseType == Type.tArray) {
        if (td.firstType == null) {
            res = "array<anything>"
        } else {
            res = "array<{describe_type_short(td.firstType)}>"
        }
    } elif (td.baseType == Type.tTable) {
        if (td.firstType == null || td.secondType == null) {
            res = "table<anything, anything>"
        } else {
            res = "table<{describe_type_short(td.firstType)}, {describe_type_short(td.secondType)}>"
        }
    } elif (td.baseType == Type.tFunction || td.baseType == Type.tBlock || td.baseType == Type.tLambda) {
        res = build_string() $(var w) {
            w |> write(td.baseType == Type.tBlock ? "block<" : td.baseType == Type.tFunction ? "function<" : "lambda<")
            if (!empty(td.argTypes)) {
                w |> write("(")
                if (!empty(td.argNames)) {
                    var first = true
                    for (at, an in td.argTypes, td.argNames) {
                        if (first) {
                            first = false
                        } else {
                            w |> write(";")
                        }
                        if (at.flags.removeConstant) {
                            w |> write("var ")
                        }
                        peek(an) $(an_str) {
                            w |> write(an_str)
                        }
                        w |> write(":")
                        w |> write(describe_type_short(at))
                    }
                } else {
                    var first = true
                    for (at in td.argTypes) {
                        if (first) {
                            first = false
                        } else {
                            w |> write(";")
                        }
                        w |> write(describe_type_short(at))
                    }
                }
                w |> write(")")
                if (td.firstType != null) {
                    w |> write(":")
                    w |> write(describe_type_short(td.firstType))
                } else {
                    w |> write(":void")
                }
            } else {
                w |> write("():void")
            }
            w |> write(">")
        }
    }
    if (!empty(res)) {
        if (td.flags.temporary) {
            res += "#"
        }
        return res
    }

    let tp = describe_typedecl(td, false, false, false)
    res = replace(tp, "$::", "")
    if (res |> find("|") != -1) {
        let joined = build_string() $(var w) {
            builtin_string_split(res, "|") $(parts : array<string>#) : void {
                for (p, i in parts, count()) {
                    if (i > 0) {
                        w |> write("|")
                    }
                    w |> write(cleanup_type_short(p))
                }
            }
        }
        return cleanup_type_short(joined)
    }

    return cleanup_type_short(res)
}
