options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require daslib/fio   // stat, fwrite: the stub file a block's loop reads
require _model_form   // ModelForm and its pure parts: the load-once forms' knobs, names and pairing; served_forms
require dasllama/dasllama_common   // MetalMode: a Metal form's mode
require daslib/strings_boost   // join: a block's loads and forms as one line

// The load-once forms and blocks (`_model_form.das`): a form's knobs, name and pairing, a block's account of its
// forms, and the block loops over fake loaders - no model file is read.

[test(description = "The load-once forms: a device form defaults to the driver's default form, drops_device no load knob, the context asked whole, the window-fault seat and a NextN head pin load knobs - the last two loading off no device lane; the uncached loader and a head pin each a CPU load of its own; a cells form loads q8 cached on every box with no pin; a backend pin or a K-quant knob - the Q4_0 native one too - makes a load of its own that names its knobs and answers no device form; the knob-free cells form answers the default form on its armed codec; a cells load stands alone on its first codec's pass only; a Metal form defaults to the blob twin under when_available at the call's context, a served image always loads under required, each names its flavor and mode, and neither is the driver's default form, a lane form or a side of the default pair; a block's Metal pass loads a Metal form where the build serves the home, skips it where the build lacks it and passes any other form by")]
def test_model_form_knobs(t : T?) {
    let dev = device_form()
    t |> success(dev.on_gpu && device_default(dev) && dev.quant == QuantMode.q8 && dev.fuse_bisect == -1l && !dev.kv_asked,
        "a knob-free device form is the driver's default form on the environment's codec")
    t |> success(!device_default(device_form(regions = 2l)) && !device_default(device_form() |> asking(KVDtype.q8_0)),
        "the control: a region count or an asked codec is a load knob")
    t |> success(!device_default(device_form(strict_ctx = true)), "the context asked whole is a load knob")
    t |> success(!device_default(device_form(cuda_window_fault = true)), "the window-fault seat is a load knob")
    t |> success(!device_default(device_form(mtp_head = "x.gguf")), "a NextN head pin is a load knob")
    t |> success(is_lane_form(dev), "the default form loads off the device lane where the rail is on")
    t |> success(!is_lane_form(device_form(cuda_window_fault = true)), "the window-fault seat's load streams the file: no lane")
    t |> success(!is_lane_form(device_form(mtp_head = "x.gguf")), "a NextN head pin's load streams the file: no lane")
    let drops = device_form(drops_device = true)
    t |> success(device_default(drops), "drops_device is no load knob: the marked form is the driver's default form")
    let plain = cpu_cells_form()
    t |> success(plain.alone && !plain.on_gpu && !plain.uncached && empty(plain.backend) && empty(plain.mtp_head)
        && plain.kq_native == KqNative.kept && plain.quant == QuantMode.q8, "a cells form defaults to the q8 cached load, alone, no pin")
    t |> success(!cpu_quant_form(QuantMode.q8).alone, "the control: the CPU form answering the device forms is not alone")
    let pinned = cpu_cells_form(backend = "portable")
    let kq_off = cpu_cells_form(kq_native = KqNative.off)
    let both = cpu_cells_form(backend = "portable", kq_native = KqNative.on)
    t |> success(same_cpu_load(plain, cpu_quant_form(QuantMode.q8)), "the knob-free cells form is the q8 load the default pair's CPU side takes")
    t |> success(!same_cpu_load(plain, pinned) && !same_cpu_load(plain, kq_off) && !same_cpu_load(pinned, both) && !same_cpu_load(kq_off, both),
        "each knob makes a load of its own")
    t |> success(!same_cpu_load(plain, cpu_cells_form(uncached = true)), "the uncached loader makes a load of its own")
    t |> success(!same_cpu_load(plain, cpu_cells_form(mtp_head = "x.gguf")), "a NextN head pin makes a load of its own")
    t |> equal(form_name(pinned), "the CPU form (f16 sessions, the portable backend pinned)", "the pinned form names its backend")
    t |> equal(form_name(both), "the CPU form (f16 sessions, the portable backend pinned, K-quant native on)", "the form names both knobs")
    t |> equal(form_name(cpu_cells_form(QuantMode.fp32, kq_native = KqNative.off)), "the fp32 CPU form, K-quant native off", "an f32 form names its knob")
    let q40 = cpu_cells_form(kq_native = KqNative.on_q40)
    t |> success(!same_cpu_load(q40, cpu_cells_form(kq_native = KqNative.on)) && !same_cpu_load(q40, plain), "the Q4_0 native knob makes a load of its own")
    t |> equal(form_name(q40), "the CPU form (f16 sessions, K-quant native on_q40)", "the Q4_0 native form names its knob")
    var p : MainPair
    t |> equal(main_side(p, plain), MainSide.neither, "before the default form ran the cells form answers nothing")
    t |> equal(main_side(p, drops), MainSide.device, "the marked default form is the pair's device side")
    t |> equal(main_side(p, plain), MainSide.cpu, "the knob-free cells form answers it on its codec")
    t |> equal(main_side(p, pinned), MainSide.neither, "the pinned form answers no device form")
    t |> equal(main_side(p, kq_off), MainSide.neither, "the K-quant form answers no device form")
    let first = codec_pass(plain, KVDtype.q8_0, true)
    let later = codec_pass(plain, KVDtype.tq4, false)
    t |> success(first.alone && first.kv == KVDtype.q8_0, "a cells load's first codec pass stands alone on that codec")
    t |> success(!later.alone && later.kv == KVDtype.tq4, "a later codec's pass of the same load does not: its cells ran once")
    t |> equal(main_side(p, later), MainSide.neither, "the later pass answers the default form only on the codec it armed")
    t |> success(!codec_pass(cpu_quant_form(QuantMode.q8), KVDtype.f16, true).alone, "the control: a load no form asks alone never stands alone")
    let twin = metal_form()
    t |> success(twin.on_metal && twin.on_gpu && !twin.metal_image && !twin.metal_required && twin.ctx == 0l,
        "a knob-free Metal form is the blob twin, no CPU load, at the call's context")
    t |> equal(metal_form_mode(twin), MetalMode.when_available, "the blob twin loads under when_available")
    t |> equal(metal_form_mode(metal_form(required = true)), MetalMode.required, "the control: the mode asked required")
    let image = metal_form(image = true, ctx = 512l)
    t |> success(image.metal_image && image.ctx == 512l, "a served image form keeps its flavor and its context")
    t |> equal(metal_form_mode(image), MetalMode.required, "a served image always loads under required: the mode is in its identity")
    t |> equal(form_name(twin), "the Metal form (the blob twin, when_available)", "the twin names its flavor and mode")
    t |> equal(form_name(metal_form(required = true)), "the Metal form (the blob twin, required)", "the required twin names its mode")
    t |> equal(form_name(image), "the Metal form (the served image, required)", "the image names its flavor")
    t |> success(!device_default(twin) && !is_lane_form(twin) && !device_default(image) && !is_lane_form(image),
        "a Metal form is no form of the whole-model driver: not its default form, and off no device lane")
    var q : MainPair
    t |> equal(main_side(q, twin), MainSide.neither, "a Metal form is no side of the default pair")
    t |> success(!q.ran, "and marks no default form as run: it arms no CPU answer")
    t |> equal(metal_pass(twin, ""), MetalPass.load, "a block's Metal pass loads a Metal form the build serves")
    t |> equal(metal_pass(image, "das_metal not present"), MetalPass.skip, "where the build lacks the home the form skips")
    t |> equal(metal_pass(dev, ""), MetalPass.other, "the control: a device form is passed by")
    t |> equal(metal_pass(plain, "das_metal not present"), MetalPass.other, "the control: a CPU form is passed by, whatever the build")
}

[test(description = "A load-once block's account: a form that reached its cells or skipped loudly is owed nothing; a pending form asked alone or on the device is a failure, a device form ran or not; a pending CPU form not asked alone skips loudly where no device form ran and is a failure where one did; a load's pass stands alone only off the device, asked alone and on that load; a pending Metal form is a failure and no CPU load's pass")]
def test_model_form_account(t : T?) {
    let plain = cpu_cells_form()
    let answer = f32_form()
    for (mark in fixed_array(FormMark.reached, FormMark.skipped)) {
        t |> equal(form_owed(plain, mark, false), FormOwed.nothing, "a form at {mark} is owed nothing")
        t |> equal(form_owed(answer, mark, true), FormOwed.nothing, "an answering form at {mark} is owed nothing")
    }
    t |> equal(form_owed(plain, FormMark.pending, false), FormOwed.failure, "a form asked alone runs on every box: never a skip")
    t |> equal(form_owed(device_form(), FormMark.pending, false), FormOwed.failure, "a device form never reached or skipped is lost")
    t |> equal(form_owed(answer, FormMark.pending, false), FormOwed.skip, "a form not asked alone, no device form run: a loud skip")
    t |> equal(form_owed(answer, FormMark.pending, true), FormOwed.failure, "the same form a device form's run asked for is lost")
    t |> success(alone_pass_of(plain, cpu_quant_form(QuantMode.q8)), "the cells form's pass is the q8 load's pass that stands alone")
    t |> success(!alone_pass_of(cpu_quant_form(QuantMode.q8), plain), "the control: a pass not asked alone does not stand alone")
    t |> success(!alone_pass_of(plain, cpu_cells_form(kq_native = KqNative.off)), "the control: another load's pass is not this load's")
    var dev_alone = device_form()
    dev_alone.alone = true
    t |> success(!alone_pass_of(dev_alone, plain), "the control: a device pass is no CPU load's")
    t |> equal(form_owed(metal_form(), FormMark.pending, false), FormOwed.failure, "a Metal form never reached or skipped is lost")
    var metal_alone = metal_form()
    metal_alone.alone = true
    t |> success(!alone_pass_of(metal_alone, plain), "a Metal pass is no CPU load's: the CPU pass and the parity row pass it by")
}

var private g_once_live = 0   //! the fake models a load made that no drop has taken back
var private g_once_loads : array<string>   //! each load's `<path>|<form>`, in order

//! a fake model: the form it was loaded as, alive until its finalizer runs
struct private OnceModel {
    form : string
    live : bool
}

def private finalize(var m : OnceModel) {
    if (m.live) {
        g_once_live--
        m.live = false
    }
}

def private once_form(path, form : string) : OnceModel {
    g_once_live++
    g_once_loads |> push("{path}|{form}")
    return OnceModel(form = form, live = true)
}

var private g_once_stages : array<string>   //! each staged read's `<path>|<key>`, in order

//! a fake staged read: the marks the forms minted from it wrote into it
struct private OnceStaging {
    marks : array<string>
}

//! the staged read a form of the fake file mints from: none for `img`, `k2` for `c`, `k1` for the rest
def private once_stage_key(form : string) : string => form == "img" ? "" : (form == "c" ? "k2" : "k1")

def private once_stage(path, key : string) : OnceStaging {
    g_once_stages |> push("{path}|{key}")
    return OnceStaging(marks <- ["read"])
}

//! a fake model minted from its own staging, which it marks: `form` carries every mark the staging held
def private once_staged_form(path, form : string; var stg : OnceStaging) : OnceModel {
    g_once_live++
    stg.marks |> push(form)
    g_once_loads |> push("{path}|{join(stg.marks, "+")}")
    return OnceModel(form = form, live = true)
}

//! a family's account of its forms: `gone` is one this box cannot serve
def private once_form_why(form : string) : string => form == "gone" ? "no device for it" : ""

//! a block's gate over a file of the test's own: a loud skip on the handle where the file is absent
def private once_gate(t : T?; path : string) : bool {
    return true if (stat(path).is_valid)
    t |> skip("{path} not present")
    return false
}

[test(description = "A staged block's loop (`staged_once_at`) reads a file once a key - again where another key's form came between - hands each form its own copy of that read, so one form's edit reaches no other's, and hands a form with no key no staging; `either_why` and `metal_build_why` answer a seat's reason")]
def test_staged_once_block(t : T?) {
    let root = test_temp_dir()
    g_once_live = 0
    g_once_loads |> clear()
    g_once_stages |> clear()
    let path = path_join(root, "once_stub.model")
    t |> success(fwrite(path, "a stub, never read"), "the stub file is written")
    var alive : array<int>
    let ran = staged_once_at(t, "infra", "once_stub.model", path, ["a", "b", "img", "c", "d"], @@once_gate, @@once_stage_key, @@once_stage, @@once_staged_form) $(var m; form) {
        alive |> push(g_once_live + (m.form == form ? 0 : 100))
    }
    t |> success(ran, "the call answers true")
    t |> equal(join(g_once_stages, " "), "{path}|k1 {path}|k2 {path}|k1", "one staged read a key, again where another key's form came between")
    t |> equal(join(g_once_loads, " "), "{path}|read+a {path}|read+b {path}|img {path}|read+c {path}|read+d", "each form's staging is the read plus its own mark: no form's edit reaches another's, and a form with no key gets none")
    t |> equal(join(alive, " "), "1 1 1 1 1", "each cell sees its form's model, one alive at a time")
    t |> equal(g_once_live, 0, "no model is alive after the call")
    t |> equal(either_why("", "the second seat's reason"), "", "a form the first seat serves lacks nothing")
    t |> equal(either_why("no first seat", "the second seat's reason"), "the second seat's reason", "else it lacks what the second seat does")
    static_if (typeinfo builtin_module_exists(das_metal)) {
        t |> equal(metal_build_why(), "", "a das_metal build lacks nothing for the Metal home's cells")
    } else {
        t |> equal(metal_build_why(), "das_metal not present", "a build without das_metal says so")
    }
    delete alive
}

[test(description = "A load-once block's loop (`model_once_at`, which `with_model_once_at` wires to the models' gate): behind a gate that finds no file it skips on the handle, loads nothing, runs no cell and answers false; over a present file and three forms it loads once a form, in the list's order, from the file's path, hands the cells each form's own model, holds exactly one model alive inside every cells call and none after the call, and answers true. A block's form list through `served_forms`: a form the family's account gives a reason for is one skipped cell of its own, named the tag, the form and the reason, and is left out of the list, the others kept in order; a list the box serves whole comes back whole with no cell")]
def test_model_once_block(t : T?) {
    let root = test_temp_dir()
    t |> run("an absent file: the gate's skip, no load, no cell, false") @(t : T?) {
        g_once_live = 0
        g_once_loads |> clear()
        var skips = 0
        var cells = 0
        var p = new T("probe", ref_time_ticks(), false)
        unsafe {
            p.onSkipNow <- @capture(& skips)() {
                skips++
            }
        }
        let ran = model_once_at(p, "infra", "once_absent.model", path_join(root, "once_absent.model"), ["a", "b"], @@once_gate, @@once_form) $(var m; form) {
            cells += m.form == form ? 1 : 2
        }
        unsafe {
            delete p
        }
        t |> success(!ran, "the call answers false")
        t |> equal(skips, 1, "the gate skipped on the handle, once")
        t |> equal(length(g_once_loads), 0, "the loader is never invoked")
        t |> equal(cells, 0, "no cell runs")
    }
    t |> run("a present file, three forms: a load a form in order, each form's own model, one alive at a time, none after") @(t : T?) {
        g_once_live = 0
        g_once_loads |> clear()
        let path = path_join(root, "once_stub.model")
        t |> success(fwrite(path, "a stub, never read"), "the stub file is written")
        var seen : array<string>
        var alive : array<int>
        let ran = model_once_at(t, "infra", "once_stub.model", path, ["a", "b", "c"], @@once_gate, @@once_form) $(var m; form) {
            seen |> push("{form}={m.form}")
            alive |> push(g_once_live)
        }
        t |> success(ran, "the call answers true")
        t |> equal(join(g_once_loads, " "), "{path}|a {path}|b {path}|c", "one load a form, in the list's order, from the file's path")
        t |> equal(join(seen, " "), "a=a b=b c=c", "the cells see each form's own model")
        t |> equal(join(alive, " "), "1 1 1", "exactly one model is alive inside every cells call")
        t |> equal(g_once_live, 0, "no model is alive after the call")
        delete seen
        delete alive
    }
    t |> run("a form the box cannot serve: a skipped cell of its own, left out of the list") @(t : T?) {
        var cells : array<string>
        var skips = 0
        var p = new T("probe", ref_time_ticks(), false)
        unsafe {
            p.onRun <- @capture(& cells, & skips)(name : string; cell : RunT) {
                cells |> push(name)
                var sub = new T(name, ref_time_ticks(), false)
                sub.onSkipNow <- @capture(& skips)() {
                    skips++
                }
                invoke(cell as lmd1, sub)
                delete sub
            }
        }
        var inscope served <- served_forms(p, "once.model", ["a", "gone", "b"], @@once_form_why)
        t |> equal(join(served, " "), "a b", "the forms the box serves, in the list's order")
        t |> equal(join(cells, " | "), "once.model, the gone form: no device for it", "the form it cannot serve is one cell, named the tag, the form and the reason")
        t |> equal(skips, 1, "and that cell skipped")
        cells |> clear()
        var inscope whole <- served_forms(p, "once.model", ["a", "b"], @@once_form_why)
        t |> equal(join(whole, " "), "a b", "the control: a list the box serves whole comes back whole")
        t |> equal(length(cells) + skips, 1, "with no cell and no further skip")
        unsafe {
            delete p
        }
        delete cells
    }
}
