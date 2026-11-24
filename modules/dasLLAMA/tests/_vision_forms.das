// The vision towers' forms: what a form's name says (`tower_form`), the shared halves of the families' form loaders - the
// image rail's load under a lane pin, the staging of a form minted in memory - the check that a form is the lane, depth
// and image flavor it names, what the box lacks for the forms a device serves, and a tower block's cell. Each family's
// form loader sits in its test file, beside its block.

options gen2
options _dasllama_internal = true

module _vision_forms shared public

require dastest/testing_boost public
require dasllama/dasllama_vision_embedder public
require _tower_twin public   // the twin's kept canvases, the seats, the GPU knobs
require _vision_oracle   // make_planes: the exact lane's canvas
require dasllama/dasllama_lever   // with_put_back: a form's pin and knobs put back however its load leaves
require _model_form   // either_why, metal_build_why: the seats' reasons
require _model_tier   // models_dir: an oracle dump's root
require daslib/fio   // path_join: an oracle dump's path

//! a vision tower's form (`tower_form_of`), its lane `f32` or `q8`
def tower_form(form : string) : TowerForm => tower_form_of(form, ["f32", "q8"], "vision")

//! whether the form `f` is a copy the serving home's device chain reads (`@dev`, `@seat`), not the CPU chain's
def tower_form_on_device(f : TowerForm) : bool => f.dev || f.seat

//! the image flavor the minted form `f` asks: the serving home's under `@dev`, else the CPU image
def tower_form_dev(f : TowerForm) : TowerDev => tower_form_dev_of(f, TowerFamily.vision)

var private g_policy_q8 = false   //! the lane the family's policy named as the last unpinned image form's load started

//! An image-rail form `f` of a lane family's tower file, as the carrier holds it: `load_vision_embedder` on `path` under
//! the family's lane pin - unset for `image`, whose lane the policy `serves_q8` names as the load starts - with every GPU
//! tower knob off for the q8 form's load; the pin unset and the knobs put back however the load leaves, a panic included.
def image_tower_form(path : string; f : TowerForm; pin : function<(on : bool) : void>; unpin : function<() : void>;
                     serves_q8 : function<() : bool>) : VisionEmbedder {
    let knobs = gpu_knobs()
    invoke(unpin)
    if (empty(f.lane)) {
        g_policy_q8 = invoke(serves_q8)
    } else {
        invoke(pin, f.lane == "q8")
    }
    if (f.lane == "q8") {
        set_every_gpu_tower(false)
    }
    var inscope e = VisionEmbedder()
    with_put_back($ {
        invoke(unpin)
        set_gpu_knobs(knobs)
    }) {
        delete e
        e <- load_vision_embedder(path)
    }
    return <- e
}

//! the staged read a lane family's tower form is minted from (`tower_stage_key_of`)
def tower_stage_key(form : string) : string => tower_stage_key_of(tower_form(form))

//! the staging of a lane family's tower file the key `key` names (`tower_stage_key`): `staged_twin_tower` on its lane and flavor
def staged_tower_form(path, key : string; pin : function<(on : bool) : void>; unpin : function<() : void>;
                      stage : function<(path : string) : auto(STG)>) : STG {
    let f = tower_form(key)
    return <- staged_twin_tower(path, f.lane == "q8", tower_form_dev(f), pin, unpin, stage)
}

//! The form is the lane, depth and image flavor it names: on `tw`, a lane family's tower loaded as the form `form` of
//! `file`, the tower's own lane flag - on `image` the lane the policy named as the load started - and, on a minted form, its
//! block count where the form cuts it and its image flavor. The block a poisoned form zeroes is not read back.
def tower_form_is(t : T?; file, form : string; tw) {
    let f = tower_form(form)
    let q8 = f.image && empty(f.lane) ? g_policy_q8 : f.lane == "q8"
    t |> equal(tw.q8, q8, "{file}: the {form} form serves the lane it names")
    if (!f.image) {
        t |> success(f.nl == 0l || tw.n_layer == f.nl, "{file}: the {form} form holds the blocks it names ({tw.n_layer})")
        t |> equal(tw.dev_backend, tower_form_dev(f), "{file}: the {form} form is the image flavor it names")
    }
}

//! what the box lacks for a form only a vision tower's discrete twin reads, "" where the twin's seat stands: a build with
//! dasVulkan, and the serving discrete home's seat (`discrete_twin_seat_why`)
[unused_argument(form)]
def twin_form_why(form : string) : string {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return discrete_twin_seat_why()
    } else {
        return "dasVulkan not present"
    }
}

//! what the box lacks for a form the Metal tower's cells read, "" on a das_metal build
[unused_argument(form)]
def metal_form_why(form : string) : string => metal_build_why()

//! what the box lacks for a form read by the Metal tower's cells and by the discrete twin's: "" on a das_metal build, else
//! the twin's reason (`twin_form_why`)
def metal_or_twin_form_why(form : string) : string => either_why(metal_form_why(form), twin_form_why(form))

//! what the box lacks for a form the serving discrete home's device rung reads, "" where the rung runs: a dasVulkan build
//! with no das_metal, whose serving discrete home's device stands up
[unused_argument(form)]
def discrete_rung_form_why(form : string) : string {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return "das_metal on this build owns the tower hooks - the discrete device rung has no seat"
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return serving_tower_device_up(TowerFamily.vision) ? "" : "no device stands up for the serving discrete home's tower ({serving_tower_dev(TowerFamily.vision)})"
        } else {
            return "dasVulkan not present"
        }
    }
}

//! what the box lacks for a form read by the Metal tower's cells and by the discrete device rung's: "" on a das_metal build,
//! else the rung's reason (`discrete_rung_form_why`)
def device_rung_form_why(form : string) : string => either_why(metal_form_why(form), discrete_rung_form_why(form))

//! the two CPU towers of one cut depth of a lane family's twin, as their forms' cells keep them: each one's lane flag and
//! block count
struct TwinDepth {
    nl : int64
    q8 : bool
    exact_q8 : bool
    n_q8 : int64
    n_exact : int64
}

//! A lane family's discrete twin as the forms of its tower file keep it: the canvases three ways (`TwinKept`), the cut
//! depths' towers, and the exact lane served on the device over the `exact_kind` canvas at `exact_w` x `exact_h`. `form` is
//! the form whose legs run.
struct VisionTwin {
    form : string
    canvases : array<TwinKept>
    depths : array<TwinDepth>
    exact : ExactLaneKept
    exact_kind : string
    exact_w : int
    exact_h : int
}

//! The legs the form `twin.form` of a lane family's tower `tw` adds to its file's twin, each on the form's one home: a cut
//! depth's CPU tower noted (`TwinDepth`), the canvases of the form's depth (`twin_keep_legs`), and of the exact lane served
//! on the device its CPU leg on the whole exact form, its device leg on that form's `@seat` copy and its poison leg on the
//! zeroed `@seat` copy. `encode` is the family's encode of a canvas's planes.
def twin_form_legs(tt : T?; var twin : VisionTwin; tw : auto(TW); var st : auto(ST);
                   encode : block<(t : TW; var s : ST; planes : array<float>; w, h : int; var out : array<float>) : int64>) {
    let f = tower_form(twin.form)
    let q8 = f.lane == "q8"
    for (d in twin.depths) {
        continue if (d.nl != f.nl || tower_form_on_device(f) || f.zero_block >= 0l)
        if (q8) {
            d.q8 = tw.q8
            d.n_q8 = tw.n_layer
        } else {
            d.exact_q8 = tw.q8
            d.n_exact = tw.n_layer
        }
    }
    if (!q8 && f.nl == 0l) {
        let planes <- make_planes(twin.exact_kind, twin.exact_w, twin.exact_h)
        let w = twin.exact_w
        let h = twin.exact_h
        if (f.zero_block >= 0l) {
            exact_lane_poison_leg(tt, twin.exact, tw, st, $(t; var s; var out) => invoke(encode, t, s, planes, w, h, out))
            return
        }
        if (f.seat) {
            exact_lane_dev_leg(twin.exact, tw, st, $(t; var s; var out) => invoke(encode, t, s, planes, w, h, out))
            return
        }
        exact_lane_cpu_leg(twin.exact, tw, st, $(t; var s; var out) => invoke(encode, t, s, planes, w, h, out))
    }
    if (f.dev) {
        let home = serving_tower_dev(TowerFamily.vision)
        tt |> equal(tw.dev_backend, home, "{twin.form}: the device leg's tower is the serving home's image ({home})")
    }
    twin_keep_legs(tt, twin.canvases, f.nl, tw.n_layer, q8, f.dev, f.zero_block, tw, st, encode)
}

//! a lane family's twin held, after its file's block: each cut depth's towers the lanes and depth their forms name, each
//! canvas (`twin_kept_held`, the device leg's rows the `patch` grid's), the exact lane served on the device and its poison
def twin_held(tt : T?; family, log_suffix : string; patch : int64; twin : VisionTwin) {
    for (d in twin.depths) {
        tt |> success(d.q8 && !d.exact_q8 && d.n_q8 == d.nl && d.n_exact == d.nl, "the {d.nl}-block towers, q8 and exact")
    }
    for (k in twin.canvases) {
        twin_kept_held(tt, family, log_suffix, patch, k)
    }
    let kept = !empty(twin.exact.cpu) && !empty(twin.exact.dev) && !empty(twin.exact.poisoned)
    tt |> success(kept, "{twin.exact.tag}: the exact lane kept its clean legs and its poison leg")
    return if (!kept)
    exact_lane_held(tt, twin.exact)
    exact_lane_poison_held(tt, twin.exact)
}

//! what the box lacks for a form the dump-free device twin of a tower either discrete home seats reads, "" where its seat
//! stands: a build with dasVulkan or dasCUDA, and the serving discrete home's seat (`discrete_twin_seat_why`)
[unused_argument(form)]
def either_home_twin_form_why(form : string) : string {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        return discrete_twin_seat_why()
    } else {
        return "no discrete GPU home (dasVulkan or dasCUDA) in this build"
    }
}

//! the exact lane's carrier stamp, on a vision file's `image_f32` form
def carrier_stamps_exact(tt : T?; via_carrier_exact : VisionEmbedder) {
    tt |> equal(vision_exec_fmt(via_carrier_exact), "f32", "the carrier stamps the exact lane")
}

//! a vision tower's discrete twin with no legs yet: its exact lane served on the device over the `kind` canvas at `w` x
//! `h`, `npos` positions, held at `TWIN_EXACT_REL_L2_VK` under the tag `tag`
def vision_twin(kind : string; w, h : int; tag : string; npos : int64) : VisionTwin {
    var twin = VisionTwin(exact_kind = kind, exact_w = w, exact_h = h)
    twin.exact.tag = tag
    twin.exact.fam = TowerFamily.vision
    twin.exact.bar = TWIN_EXACT_REL_L2_VK
    twin.exact.npos = npos
    return <- twin
}

//! `logname`'s mtmd dump under `oracle_dir` beside the models, or the loud skip naming `mint_hint`; false on the skip
def oracle_dump(tt : T?; oracle_dir, logname, mint_hint : string; var o : OracleEncode) : bool {
    if (!load_oracle_encode(path_join(path_join(models_dir(), oracle_dir), logname), o)) {
        tt |> skip("oracle dump {logname} not present/parseable - mint via {mint_hint}")
        return false
    }
    return true
}
