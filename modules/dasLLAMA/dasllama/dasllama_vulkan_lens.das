options gen2
options indenting = 4

module dasllama_vulkan_lens shared private

require daslib/ast
require daslib/ast_boost
require daslib/macro_boost
require daslib/rtti
require strings
require daslib/strings_boost
require dasllama/dasllama_kernel_access

// The dasLLAMA Vulkan kernel-access lens ([[dasllama_metal_lens]]'s sibling). At compile time it
// walks every [compute_shader] kernel in the requiring module, derives which SSBO BINDINGS the
// body reads and writes (via dasllama_kernel_access over the @ssbo globals, aliased views
// unioned per @binding), and splices the result into the module as a spec string:
//     "kernel=readmask,writemask;..."   (decimal, 6-bit binding masks, name-sorted)
// The runtime pairs those masks with per-descriptor-set region bits to derive each dispatch's
// hazard masks — see dasllama_vulkan_common's hz_dispatch.
//
// Escape hatch: [vk_access(reads="vk_a,vk_b", writes="vk_y")] on a kernel overrides derivation.
// Without force=true the override must MATCH the derived masks (a drift guard); force=true makes
// the deliberate lie win — including when the body is unanalyzable (the ratchet error).

[function_macro(name = vk_access)]
class VkAccessAnnotation : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        return true   // marker — vk_kernel_access_spec reads the arguments during its module scan
    }
}

def private names_to_mask(spec : string; bindings : table<string; int>; var bad : string&) : uint {
    var mask = 0u
    for (tok in split(spec, ",")) {
        let t = strip(tok)
        if (t == "") {
            continue
        }
        if (key_exists(bindings, t)) {
            mask |= 1u << uint(bindings?[t] ?? 0)
        } else {
            bad = t
        }
    }
    return mask
}

def private set_to_mask(names : table<string>; bindings : table<string; int>) : uint {
    var mask = 0u
    for (n in keys(names)) {
        if (key_exists(bindings, n)) {
            mask |= 1u << uint(bindings?[n] ?? 0)
        }
    }
    return mask
}

[call_macro(name = "vk_kernel_access_spec")]
class VkKernelAccessSpec : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : ExpressionPtr {
        macro_verify(empty(call.arguments), prog, call.at, "vk_kernel_access_spec takes no arguments")
        // the tracked set: every @ssbo global with a @binding (aliased views union onto one bit)
        var bindings : table<string; int>
        mod |> for_each_global() $(v) {
            if (v.annotation |> find_arg("ssbo") is tBool) {
                let b = v.annotation |> find_arg("binding") ?as tInt ?? -1
                if (b >= 0) {
                    bindings |> insert(string(v.name), b)
                }
            }
        }
        macro_verify(!empty(bindings), prog, call.at, "vk_kernel_access_spec: no @ssbo @binding globals in this module")
        var tracked : table<string>
        for (n in keys(bindings)) {
            tracked |> insert(n)
        }
        var names : array<string>
        var rmasks : table<string; uint>
        var wmasks : table<string; uint>
        var fail = ""
        mod |> for_each_function("") $(fn) {
            var is_kernel = false
            var ovr_reads = ""
            var ovr_writes = ""
            var has_ovr = false
            var ovr_force = false
            for (ann in fn.annotations) {
                if (ann != null && ann.annotation.name == "compute_shader") {
                    is_kernel = true
                } elif (ann != null && ann.annotation.name == "vk_access") {
                    has_ovr = true
                    ovr_reads = ann.arguments |> find_arg("reads") ?as tString ?? ""
                    ovr_writes = ann.arguments |> find_arg("writes") ?as tString ?? ""
                    ovr_force = ann.arguments |> find_arg("force") ?as tBool ?? false
                }
            }
            if (is_kernel) {
                let name = string(fn.name)
                var res <- classify_global_access(fn, mod, tracked)
                var r = set_to_mask(res.reads, bindings)
                var w = set_to_mask(res.writes, bindings)
                if (has_ovr) {
                    var bad = ""
                    let or_ = names_to_mask(ovr_reads, bindings, bad)
                    let ow = names_to_mask(ovr_writes, bindings, bad)
                    if (bad != "" && fail == "") {
                        fail = "{name}: [vk_access] names unknown global '{bad}'"
                    }
                    if (!ovr_force) {
                        if (!empty(res.err) && fail == "") {
                            fail = "{name}: body unanalyzable ({res.err}) — [vk_access] needs force=true"
                        }
                        if ((or_ != r || ow != w) && fail == "") {
                            fail = "{name}: [vk_access] (r={int(or_)},w={int(ow)}) does not match derived (r={int(r)},w={int(w)}) — fix it or add force=true"
                        }
                    }
                    r = or_
                    w = ow
                } elif (!empty(res.err) && fail == "") {
                    fail = res.err
                }
                delete res
                names |> push(name)
                rmasks |> insert(name, r)
                wmasks |> insert(name, w)
            }
        }
        macro_verify(fail == "", prog, call.at, "vk_kernel_access_spec: {fail}")
        macro_verify(!empty(names), prog, call.at, "vk_kernel_access_spec: no [compute_shader] kernels in this module")
        sort(names)
        let spec = build_string() $(writer) {
            for (n in names) {
                writer |> write(n) |> write("=") |> write(int(rmasks[n])) |> write(",") |> write(int(wmasks[n])) |> write(";")
            }
        }
        return new ExprConstString(at = call.at, value := spec)
    }
}
