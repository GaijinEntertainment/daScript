options gen2
options indenting = 4
options _dasllama_internal = true

//! A tower family's GPU flavor seat written once: `[tower_flavor(family = "<family>", model = "<Model>")]` on the family's flavor
//! record declares its fields, the private global holding the registered driver's, and the driver's registrant
//! (`ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block`); `tower_dev_flavor` and `tower_flavor_registered` read the record.
module dasllama_tower_flavor shared public

require dasllama/dasllama_lint public
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost
require dasllama/dasllama_kernel_access   // mk_alias_t

def private flavor_arg(args : AnnotationArgumentList; name, dflt : string) : string {
    for (a in args) {
        if (a.name == name && a.basicType == Type.tString) {
            return string(a.sValue)
        }
    }
    return dflt
}

//! `[tower_flavor(family = "<family>", model = "<Model>", lane = "bool" | "model")]` on a struct: the fields `backend : TowerDev`,
//! `serves` - whether the driver's chain serves a load's lane, the q8 flag (`lane = "bool"`, the default) or the model's own
//! meta record (`lane = "model"`) - and `bake` - the model into the driver's `TowerDevBuild` - beside any the struct declares; the
//! private global `g_<family>_flavor`; and `register_<family>_flavor(backend, serves, bake)`, which replaces the whole record
[structure_macro(name = tower_flavor), arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
class TowerFlavorSeat : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        let family = flavor_arg(args, "family", "")
        let model = flavor_arg(args, "model", "")
        let lane = flavor_arg(args, "lane", "bool")
        if (empty(family) || empty(model) || (lane != "bool" && lane != "model")) {
            errors := "[tower_flavor] {st.name}: needs family = and model = (strings), and lane = \"bool\" or \"model\" where given"
            return false
        }
        let at = st.at
        var model_t = mk_alias_t(at, model, true)
        var lane_t = lane == "bool" ? new TypeDecl(at = at, baseType = Type.tBool) : mk_alias_t(at, model, false)
        lane_t.flags.constant = true
        var serves_t = qmacro_type(type<function<(lane : $t(lane_t)) : bool>>)
        var bake_t = qmacro_type(type<function<(m : $t(model_t); var b : TowerDevBuild) : bool>>)
        st |> add_structure_field("backend", mk_alias_t(at, "TowerDev", false), default<ExpressionPtr>)
        st |> add_structure_field("serves", clone_type(serves_t), default<ExpressionPtr>)
        st |> add_structure_field("bake", clone_type(bake_t), default<ExpressionPtr>)
        let gname = "g_{family}_flavor"
        var rec_t = mk_alias_t(at, string(st.name), false)
        if (!add_global_var(compiling_module(), gname, clone_type(rec_t), at, true)) {
            errors := "[tower_flavor] {st.name}: cannot add the global {gname}"
            return false
        }
        var fn = qmacro_function("register_{family}_flavor") $(backend : TowerDev; serves : $t(serves_t); bake : $t(bake_t)) {
            var rec : $t(rec_t)
            rec.backend = backend
            rec.serves = serves
            rec.bake = bake
            $i(gname) = rec
        }
        fn.flags.generated = true
        if (!(compiling_module() |> add_function(fn))) {
            errors := "[tower_flavor] {st.name}: cannot add register_{family}_flavor"
            return false
        }
        return true
    }
}
