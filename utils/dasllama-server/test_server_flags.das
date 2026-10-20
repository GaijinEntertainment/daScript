options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true   // the K/V room seam the ctx-clamp cell doubles is the engine's, not the facade's

require dastest/testing_boost public
require main                                            // the program root: cli_has_flag, mtp_setting, mtp_default_resolved (sibling by bare name)
require openai_server                                   // mtp_auto_arms, clamp_ctx_to_device_room, and serving_knobs' Metal-mode pair through it
require dasllama/dasllama                               // MetalMode through the facade
require dasllama/dasllama_gpu_tier                      // the K/V room seam a cell doubles
require daslib/clargs                                   // get_user_args / flag_present: the match cli_has_flag rests on
require strings
require daslib/fio                                      // the roster cell writes its config
require math
require _server_rig
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_scheduler

// Model-free unit coverage for the flag-presence test behind the config surface's provenance:
// a flag the process was given reads present, in both spellings, and an absent one reads absent;
// and for the self-speculation default's rule, as the slot and the config surface read it.
// Every cell but the speculating slot's runs in CI with no GGUF and no JIT; that one needs a NextN-headed
// model and an armed Vulkan tier, and skips without them.
//! the process-argument cell skips when the test process was given no `--` flag

[test]
def test_flag_match(t : T?) {
    t |> run("a flag matches bare and as flag=value, never as a prefix of another flag") @(t : T?) {
        let args <- ["-m", "model.gguf", "--flat", "--mtp=false", "--streams", "1"]
        t |> success(flag_present(args, "--flat"), "a bare flag")
        t |> success(flag_present(args, "--mtp"), "the flag=value spelling")
        t |> success(flag_present(args, "--streams"), "a flag whose value is the next argument")
        t |> success(flag_present(args, "-m"), "a short flag")
        t |> success(!flag_present(args, "--fla"), "a prefix of a given flag is absent")
        t |> success(!flag_present(args, "--mtp=true"), "a flag=value the list does not hold is absent")
        t |> success(!flag_present(args, "--gpu-dn"), "a flag the list does not hold is absent")
        let none : array<string>
        t |> success(!flag_present(none, "--flat"), "an empty list holds no flag")
    }
}

[test]
def test_cli_has_flag(t : T?) {
    t |> run("a flag this process was given reads present, one it was not given reads absent") @(t : T?) {
        var given = ""
        var args <- get_user_args()
        for (a in args) {
            if (empty(given) && a |> starts_with("--") && find(a, "=") < 0) {
                given = a
            }
        }
        delete args
        if (empty(given)) {
            t |> skip("the test process was given no -- flag")
            return
        }
        t |> success(cli_has_flag(given), "{given} is present")
        t |> success(!cli_has_flag("{given}-never-given"), "{given}-never-given is absent")
    }
}

[test]
def test_mtp_accept_default(t : T?) {
    t |> run("the acceptance a process starts with is rejection sampling - the lossless walk over the head's whole distribution; this file never sets it") @(t : T?) {
        t |> equal(int(get_mtp_accept()), int(MtpAccept.rejection), "the default acceptance ({get_mtp_accept()})")
    }
}

[test]
def test_mtp_setting_tri_state(t : T?) {
    t |> run("a key the TOML or --mtp set pins 0 / 1; unset is -1 and each slot decides") @(t : T?) {
        t |> equal(mtp_setting(true, true), 1, "mtp = true")
        t |> equal(mtp_setting(true, false), 0, "mtp = false")
        t |> equal(mtp_setting(false, false), -1, "unset: the slot decides")
        t |> equal(mtp_setting(false, true), -1, "an unsourced true is still unset")
    }
    t |> run("the unset default resolves on for one stream on a GPU the model's catalog row lists, off otherwise") @(t : T?) {
        t |> success(mtp_default_resolved(1, "metal", "", "metal"), "one stream on Metal, the row lists metal")
        t |> success(mtp_default_resolved(1, "metal-required", "", "metal") && mtp_default_resolved(1, "vulkan", "", "vulkan"), "one stream on required Metal; one on a Vulkan device whose row lists vulkan")
        t |> success(mtp_default_resolved(1, "vulkan", "", "metal, vulkan"), "a row listing both platforms")
        t |> success(mtp_default_resolved(1, "", "auto", "metal"), "one stream on the legacy --metal auto")
        t |> success(!mtp_default_resolved(1, "metal", ""), "one stream on Metal, no catalog row: plain")
        t |> success(!mtp_default_resolved(1, "metal", "", "vulkan"), "one stream on Metal, the row lists vulkan alone: plain")
        t |> success(!mtp_default_resolved(1, "vulkan", "", "metal"), "one stream on a Vulkan device, the row lists metal alone: plain")
        t |> success(!mtp_default_resolved(1, "", "", "metal"), "one stream, no device")
        t |> success(!mtp_default_resolved(1, "off", "required", "metal"), "one stream on the CPU, whatever --metal says")
        t |> success(!mtp_default_resolved(4, "metal", "", "metal"), "four streams on Metal")
        t |> success(!mtp_default_resolved(4, "vulkan", "", "vulkan"), "four streams on a Vulkan device")
    }
    t |> run("an unset chunk resolves to one window where a GPU prefills, the short chunk on the CPU") @(t : T?) {
        t |> equal(chunk_default_resolved("metal", ""), 512, "Metal")
        t |> equal(chunk_default_resolved("vulkan", ""), 512, "a Vulkan device")
        t |> equal(chunk_default_resolved("", "auto"), 512, "the legacy --metal auto")
        t |> equal(chunk_default_resolved("", ""), 64, "no device")
        t |> equal(chunk_default_resolved("off", "required"), 64, "the CPU, whatever --metal says")
    }
    t |> run("a slot's default chunk reads its own arm outcome, not the box's GPU tier") @(t : T?) {
        t |> success(prefills_on_gpu(true, "cpu"), "the Metal flavor - its tier word reads cpu")
        t |> success(prefills_on_gpu(false, "gpu:resident"), "the whole model resident on a Vulkan device")
        t |> success(prefills_on_gpu(false, "gpu:rails"), "a model the device serves in part still prefills there")
        t |> success(!prefills_on_gpu(false, "cpu"), "a CPU slot beside another slot's armed model")
    }
}

[test]
def test_slot_chunk_defaults(t : T?) {
    t |> run("a slot's chunk defaults read its own arm outcome: the CPU's short chunk and one-window idle chunk, a GPU's one-window chunk and the idle floor, the resident driver's idle chunk at its prompt window or wider") @(t : T?) {
        let cpu = slot_chunk_defaults(false, "cpu")
        t |> equal(cpu.chunk, 64l, "a CPU slot's chunk is the short one")
        t |> equal(cpu.idle, DEFAULT_CHUNK_TOKENS, "a CPU slot's idle chunk is one window")
        let metal = slot_chunk_defaults(true, "cpu")
        t |> equal(metal.chunk, DEFAULT_CHUNK_TOKENS, "the Metal flavor's chunk is one window")
        t |> equal(metal.idle, GPU_IDLE_CHUNK_TOKENS, "the Metal flavor's idle chunk is the GPU floor")
        let resident = slot_chunk_defaults(false, "gpu:resident")
        t |> equal(resident.chunk, DEFAULT_CHUNK_TOKENS, "the resident driver's chunk is one window")
        t |> success(resident.idle >= GPU_IDLE_CHUNK_TOKENS, "the resident driver's idle chunk is the floor or the driver's wider prompt window ({resident.idle})")
    }
}

[test]
def test_slot_arm_rules(t : T?) {
    t |> run("a slot arms on one stream whose decode runs on a GPU the model's row lists") @(t : T?) {
        t |> success(mtp_auto_arms(1l, true, "cpu", "metal"), "the Metal flavor - its tier word reads cpu")
        t |> success(mtp_auto_arms(1l, false, "gpu:resident", "vulkan"), "the whole-model driver on a Vulkan device - the experts on the card or summed on the host alike, the round wins at the NextN depth on both")
        t |> success(!mtp_auto_arms(1l, true, "cpu") && !mtp_auto_arms(1l, false, "gpu:resident", "metal"), "a row listing no platform, or Metal alone on a Vulkan device: plain")
        t |> success(!mtp_auto_arms(1l, false, "gpu:rails", "vulkan") && !mtp_auto_arms(1l, false, "cpu", "metal, vulkan"), "a model the device serves in part; the CPU")
        t |> success(!mtp_auto_arms(4l, true, "cpu", "metal") && !mtp_auto_arms(4l, false, "gpu:resident", "vulkan"), "four streams on Metal or on a Vulkan device")
    }
    t |> run("an unset ASR dispatch joins the team when every LLM slot decodes on a GPU") @(t : T?) {
        t |> success(decodes_on_gpu(true, "cpu"), "the Metal flavor - its tier word reads cpu")
        t |> success(decodes_on_gpu(false, "gpu:resident"), "the whole model resident on a Vulkan device")
        t |> success(!decodes_on_gpu(false, "gpu:resident", true), "... but not where its routed experts sum on the host")
        t |> success(!decodes_on_gpu(false, "gpu:rails"), "a model the device serves in part decodes on the team")
        t |> success(!decodes_on_gpu(false, "cpu"), "the CPU")
        t |> success(asr_auto_team(1, 1), "one slot, on a GPU")
        t |> success(asr_auto_team(2, 2), "two slots, both on a GPU")
        t |> success(!asr_auto_team(2, 1), "one slot of two decodes on the CPU: the team is its")
        t |> success(!asr_auto_team(1, 0), "one slot on the CPU")
        t |> success(asr_auto_team(0, 0), "a speech-only server: nothing else publishes to the team")
    }
    t |> run("the dispatch mode decides which speech workers publish to the team") @(t : T?) {
        let hybrid_gpu = speech_team_dispatch(true, -1, 1, 1)
        t |> success(hybrid_gpu.asr && !hybrid_gpu.tts, "hybrid, every slot on a GPU: ASR on the team, TTS inline")
        let hybrid_cpu = speech_team_dispatch(true, -1, 1, 0)
        t |> success(!hybrid_cpu.asr && !hybrid_cpu.tts, "hybrid beside a CPU slot: both inline")
        let team = speech_team_dispatch(true, 1, 1, 0)
        t |> success(team.asr && team.tts, "team: both publish, whatever the slots decode on")
        let asr_inline = speech_team_dispatch(true, 0, 1, 1)
        t |> success(!asr_inline.asr && !asr_inline.tts, "ASR pinned inline beside GPU slots")
        let inline_mode = speech_team_dispatch(false, 1, 1, 1)
        t |> success(!inline_mode.asr && !inline_mode.tts, "inline: nobody publishes")
    }
    t |> run("the Metal mode: the --gpu pick when there is one, else the legacy --metal mode") @(t : T?) {
        t |> success(resolved_metal_mode(MetalMode.when_available, "vulkan") == MetalMode.off, "a --gpu pick wins over --metal")
        t |> success(resolved_metal_mode(MetalMode.required, "off") == MetalMode.off, "gpu = off keeps Metal out")
        t |> success(resolved_metal_mode(MetalMode.off, "metal") == MetalMode.when_available, "gpu = metal")
        t |> success(resolved_metal_mode(MetalMode.off, "metal-required") == MetalMode.required, "gpu = metal-required")
        t |> success(resolved_metal_mode(MetalMode.required, "") == MetalMode.required, "no --gpu pick: the legacy mode stands")
    }
    t |> run("a --metal spelling names its mode in any case, and an unknown one names none") @(t : T?) {
        t |> success(metal_mode_of("Required").mode == MetalMode.required && metal_mode_of("Required").known)
        t |> success(metal_mode_of("auto").mode == MetalMode.when_available && metal_mode_of("when_available").known)
        t |> success(metal_mode_of("").mode == MetalMode.off && metal_mode_of("").known && metal_mode_of("off").known)
        t |> success(metal_mode_of("fast").mode == MetalMode.off && !metal_mode_of("fast").known)
    }
}

def private room_of_a_megabyte(row_bytes : int64) : int64 => 1000000l / row_bytes

//! The context clamp a blob-only model gets: its `seq_len` cut to the rows the device's KV room holds, a planar model and a
//! context under the room left alone.
[test]
def test_ctx_clamp_to_device_room(t : T?) {
    var m = Model()
    m.config.n_layers = 3l
    m.config.head_size = 128l
    m.config.n_kv_heads = 2l
    m.kv_src <- [0l, 1l, 2l]
    let prior = gpu_kv_room_hook()
    set_gpu_kv_room_hook(@@room_of_a_megabyte)   // f16 K and V over three layers of 256: 3072 bytes a row, 325 rows in the megabyte
    m.config.seq_len = 1000l
    clamp_ctx_to_device_room(m, "planar", KVDtype.f16)
    t |> equal(1000l, m.config.seq_len, "a planar model keeps its context: it falls to the CPU stack")
    m.metal_blob = true
    clamp_ctx_to_device_room(m, "blob", KVDtype.f16)
    t |> equal(325l, m.config.seq_len, "a blob-only model is cut to the device's rows")
    clamp_ctx_to_device_room(m, "blob", KVDtype.f16)
    t |> equal(325l, m.config.seq_len, "a context already under the room stands")
    set_gpu_kv_room_hook(prior)
    delete m
}

//! The interface the front binds: unset reads loopback (this machine alone), a configured one stands as given.
[test]
def test_bind_host_defaults_to_loopback(t : T?) {
    t |> equal("127.0.0.1", DEFAULT_BIND_HOST, "the default is loopback")
    t |> equal(DEFAULT_BIND_HOST, bind_host(""), "an unset host binds loopback")
    t |> equal("0.0.0.0", bind_host("0.0.0.0"), "a configured host binds as given")
    t |> equal("192.168.1.20", bind_host("192.168.1.20"), "... whatever address it names")
}

//! The K/V codec a slot runs on, which the live-load clamp reads too: a slot that asked for none inherits the server default
//! (never SlotOpts' f16 seed), one that asked keeps its own.
[test]
def test_slot_kv_dtype(t : T?) {
    let plain = SlotOpts()
    t |> success(slot_kv_dtype(plain) == sched_default_kv_dtype(), "an unset slot inherits the server default")
    let asked = SlotOpts(kv_dtype = KVDtype.q8_0, kv_dtype_set = true)
    t |> success(slot_kv_dtype(asked) == KVDtype.q8_0, "a slot that asked keeps its own codec")
    let seeded = SlotOpts(kv_dtype = KVDtype.q8_0)   // the seed alone, not asked: the server default still rules
    t |> success(slot_kv_dtype(seeded) == sched_default_kv_dtype(), "a codec written without the ask is the seed, not a choice")
}

//! An [[asr]] / [[tts]] roster in the config: every table is a served model in file order, the flat keys read as the
//! first one, and a one-path `asr` / `tts` key is a roster of one.
[test]
def test_speech_roster_from_toml(t : T?) {
    let path = "_test_speech_roster.toml"
    t |> run("the tables are the roster, the first is the default") @(t : T?) {
        fopen(path, "wb") $(f) {
            f |> fwrite("tts = \"k.gguf\"\ntts_lane = \"f32\"\n\n[[asr]]\npath = \"a.gguf\"\nmmproj = \"mm.gguf\"\nworkers = 2\n\n[[asr]]\nid = \"big\"\npath = \"b.gguf\"\n\n[[asr]]\nid = \"lost\"\n")
        }
        var cfg = ServerArgs(config = path)
        t |> success(apply_config(cfg), "the config reads")
        t |> equal(cfg.asr, "a.gguf", "the flat key reads as the first table")
        t |> equal(cfg.mmproj, "mm.gguf")
        t |> equal(cfg.asr_workers, 2)
        t |> equal(cfg.tts, "k.gguf", "a one-path key stays a path")
        var asr <- speech_roster_ids(false)
        t |> success(length(asr) == 2 && asr[0] == "a.gguf" && asr[1] == "big", "a table with no path is skipped: {asr}")
        delete asr
    }
    t |> run("the [[tts]] tables are a roster too, and a flag overrides the first table of a config that is not authoritative") @(t : T?) {
        fopen(path, "wb") $(f) {
            f |> fwrite("[[asr]]\npath = \"a.gguf\"\nworkers = 2\n\n[[tts]]\npath = \"k.gguf\"\nlane = \"f32\"\n\n[[tts]]\nid = \"second\"\npath = \"p.gguf\"\n")
        }
        var cfg = ServerArgs(config = path, asr_workers = 4)
        set_cli_source("asr_workers", true)
        t |> success(apply_config(cfg), "the config reads")
        set_cli_source("asr_workers", false)
        t |> equal(cfg.asr_workers, 4, "--asr-workers is the first ASR table's, over the table's own 2")
        t |> equal(cfg.tts, "k.gguf")
        t |> equal(cfg.tts_lane, "f32", "a flag not given leaves the first table's lane")
        var tts <- speech_roster_ids(true)
        t |> success(length(tts) == 2 && tts[0] == "k.gguf" && tts[1] == "second", "both TTS tables, in file order: {tts}")
        delete tts
        fopen(path, "wb") $(f) {
            f |> fwrite("authoritative = true\n\n[[asr]]\npath = \"a.gguf\"\nworkers = 2\n")
        }
        var saved = ServerArgs(config = path, asr_workers = 4)
        set_cli_source("asr_workers", true)
        t |> success(apply_config(saved), "the authoritative config reads")
        set_cli_source("asr_workers", false)
        t |> equal(saved.asr_workers, 2, "an authoritative config wins over the flag")
    }
    remove(path)
}

//! The audio tower's two spellings: `--audio-mmproj` on the command line and `audio_mmproj` in the config, the flag
//! winning over a config that is not authoritative.
[test]
def test_audio_mmproj_flag_and_key(t : T?) {
    t |> run("--audio-mmproj parses onto the server's arguments") @(t : T?) {
        let r <- parse_args(type<ServerArgs>, ["--audio-mmproj", "au.gguf", "--image-mmproj", "v.gguf"])
        t |> success(r |> is_ok, "the flags parse")
        if (r |> is_ok) {
            let cfg = r |> unwrap
            t |> equal(cfg.audio_mmproj, "au.gguf")
            t |> equal(cfg.image_mmproj, "v.gguf", "and stays apart from the vision flag")
        }
    }
    let path = path_join(test_temp_dir(), "audio_mmproj.toml")
    t |> run("the config's audio_mmproj lands, under a flag only when the config is authoritative") @(t : T?) {
        t |> success(fwrite(path, "audio_mmproj = \"toml.gguf\"\n"), "wrote the config")
        var plain = ServerArgs(config = path)
        t |> success(apply_config(plain), "the config reads")
        t |> equal(plain.audio_mmproj, "toml.gguf", "no flag: the key is the arm")
        var flagged = ServerArgs(config = path, audio_mmproj = "cli.gguf")
        set_cli_source("audio_mmproj", true)
        t |> success(apply_config(flagged), "the config reads")
        set_cli_source("audio_mmproj", false)
        t |> equal(flagged.audio_mmproj, "cli.gguf", "the flag wins over a config that is not authoritative")
        t |> success(fwrite(path, "authoritative = true\naudio_mmproj = \"toml.gguf\"\n"), "wrote the authoritative config")
        var saved = ServerArgs(config = path, audio_mmproj = "cli.gguf")
        set_cli_source("audio_mmproj", true)
        t |> success(apply_config(saved), "the authoritative config reads")
        set_cli_source("audio_mmproj", false)
        t |> equal(saved.audio_mmproj, "toml.gguf", "an authoritative config wins over the flag")
    }
    remove(path)
}

//! The roster a kind's flags make: the flat key's model first, with its own keys, then every other path named; a
//! repeated flag's first value is the default, whatever clargs kept.
[test]
def test_roster_from_flags(t : T?) {
    t |> run("one flag is a roster of one, several keep their order, and a config's flat key takes no flag's paths") @(t : T?) {
        var none : array<string>
        var one <- ["a.gguf"]
        var two <- ["a.gguf", "b.gguf"]
        var empty_roster <- roster_from_flags(SpeechEntry(), none, false)
        t |> success(empty(empty_roster), "no model, no roster")
        var single <- roster_from_flags(SpeechEntry(path = "a.gguf", mmproj = "mm.gguf", workers = 2), one, true)
        t |> success(length(single) == 1 && single[0].mmproj == "mm.gguf" && single[0].workers == 2, "the one model keeps the flat keys")
        var both <- roster_from_flags(SpeechEntry(path = "b.gguf", mmproj = "mm.gguf"), two, true)   // clargs kept the LAST value
        t |> success(length(both) == 2 && both[0].path == "a.gguf" && both[0].mmproj == "mm.gguf" && both[1].path == "b.gguf" && empty(both[1].mmproj),
            "the first path named is the default and carries the flat keys: {[for (e in both); e.path]}")
        var from_toml <- roster_from_flags(SpeechEntry(path = "t.gguf"), two, false)
        t |> success(length(from_toml) == 1 && from_toml[0].path == "t.gguf", "a key the config set is a roster of one: the flags' paths are not its")
        delete none
        delete one
        delete two
        delete empty_roster
        delete single
        delete both
        delete from_toml
    }
}

let private SLOT_PAGE_ROWS = 64l   //! the server's default paged sessions (`--flat` off): the slot's scheduler as `main.das` builds it

//! the slot's auto-arm over a modeled catalog row that lists vulkan:
//! Qwen3.5-0.8B-MTP has no catalog row of its own
def private slot_mtp_device_home(t : T?; file : string) {
    let path = path_join(models_dir(), file)
    return if (model_missing(t, path))
    if (!moe_gpu_tier_installed() || !gpu_want_auto()) {
        t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver")
        return
    }
    if (resident_mirror_dtype() != KVDtype.f16) {
        t |> skip("the slot's sessions are f16; under DASLLAMA_VK_KV32=1 the mirror is f32 and every call passes as codec")
        return
    }
    with_dasllama_jobque() {
        var s = ModelSlot()
        s.model <- load_model_(path, QuantMode.q8)
        s.model.config.seq_len = min(s.model.config.seq_len, 512l)
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the NextN-headed model (the load log names any decline)")
        s.experts_host = gpu_resident_experts_host()
        if (s.experts_host) {
            s.backend_effective = gpu_slot_capture(s.gpu_marks)
            t |> equal(s.backend_effective, "gpu:resident", "the capture names the resident driver")
            t |> success(!gpu_resident_experts_host(), "with the slot's marks parked the live predicate reads false - the trap the slot's field exists for")
            t |> success(mtp_auto_arms(1l, false, s.backend_effective, "vulkan"), "the auto-arm arms the round where the routed experts sum on the host: the NextN depth wins there")
            t |> success(!decodes_on_gpu(false, s.backend_effective, s.experts_host), "the worker team stays with a slot whose experts sum on the host")
            moe_gpu_model_marks_restore(s.gpu_marks)
            t |> success(gpu_resident_experts_host(), "the marks restored, the live predicate reads the host experts again")
            delete s.model
            return
        }
        s.backend_effective = "gpu:resident"
        s.gpu_regions = gpu_device_sessions()
        let spec = mtp_auto_arms(1l, false, s.backend_effective, "vulkan")
        t |> success(spec, "the unset mtp arms one stream whose decode the whole-model driver serves, on a catalog row listing vulkan")
        s.sched <- create_scheduler(s.model, 1l, 4l, 64l, SLOT_PAGE_ROWS, (s.model.config.seq_len + SLOT_PAGE_ROWS - 1l) / SLOT_PAGE_ROWS,
                                    KVDtype.f16, spec)
        t |> success(s.sched.mtp && s.sched.paged, "the slot speculates on the server's paged sessions")
        t |> success(slot_wants_device_kv(s), "the speculating slot homes its stream on the device: the draft head, its verify and its rollback run there")
        t |> success(set_device_kv(s.sched, slot_wants_device_kv(s)), "the scheduler admits device-home sessions")
        t |> equal(slot_served(s, s.sched.device_kv, true), "GPU: the whole model and every stream's cache on the card", "the model card names where the stream's cache lives")
        let prev = get_mtp_spec()
        set_mtp_spec(true)
        let drafts0 = moe_gpu_resident_drafts()
        let verifies0 = moe_gpu_resident_verifies()
        var req <- PendingReq(id = 1l, params = SamplingParams(), max_new = 24l)
        req.prompt <- encode(s.model, "Count from one to twenty: one, two, three,", true, false)
        t |> success(submit(s.model, s.sched, req), "the stream queued")
        var events : array<SchedEvent>
        var steps = 0
        while (scheduler_step(s.model, s.sched, events) && steps < 4096) {
            steps++
        }
        set_mtp_spec(prev)
        t |> success(s.sched.mtp_drafted > 0l, "the slot's ticks drafted ({s.sched.mtp_drafted} drafts, {s.sched.mtp_accepted} accepted)")
        t |> equal(moe_gpu_resident_drafts() - drafts0, s.sched.mtp_drafted, "every drafted step's draft ran on the device's NextN head")
        t |> equal(moe_gpu_resident_verifies() - verifies0, s.sched.mtp_drafted, "every drafted step's verify ran on the device")
        var passes <- gpu_cpu_passes_()
        t |> equal(length(passes), 0, "no call passed to the CPU rails: {passes}")
        delete passes
        set_device_kv(s.sched, false)
        delete events
        delete s.sched
        delete s.model
    }
}

[test]
def test_slot_mtp_device_home(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP, a row listing vulkan modeled: speculates device-home") @(t : T?) {
        slot_mtp_device_home(t, "Qwen3.5-0.8B-MTP-Q8_0.gguf")
    }
}

[test]
def test_slot_mtp_device_home_moe(t : T?) {
    t |> run("the same over Qwen3.6-35B-A3B-MTP-IQ4_XS-pure: the routed NextN head drafts and verifies on the device") @(t : T?) {
        slot_mtp_device_home(t, "Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf")
    }
}

[test]
def test_mtp_catalog_platforms(t : T?) {
    t |> run("the catalog's serve_mtp is read off the model file's base name; a file with no row lists none") @(t : T?) {
        t |> equal(catalog_mtp_platforms("/models/Qwen3.8-27B-UD-Q4_K_M.gguf"), "metal, vulkan", "the Qwen3.8-27B row lists both platforms: whole on either, the round measured a win on both")
        t |> equal(catalog_mtp_platforms("Qwen3.6-35B-A3B-MTP-UD-Q4_K_M.gguf"), "metal, vulkan", "the Qwen3.6-35B MTP row lists both platforms")
        t |> equal(catalog_mtp_platforms("Qwen3.6-35B-A3B-UD-Q4_K_M.gguf"), "", "the head-less Qwen3.6-35B file has nothing to draft with")
        t |> equal(catalog_mtp_platforms("/models/no-such-model.gguf"), "", "no row, no platform")
        t |> success(mtp_platform_listed("metal, vulkan", "vulkan") && !mtp_platform_listed("metal", "vulkan") && !mtp_platform_listed("", "metal"), "the comma list reads its members and nothing else")
    }
}

[test]
def test_config_mtp_catalog(t : T?) {
    t |> run("GET /config's unset mtp reads the configured model's catalog row") @(t : T?) {
        let listed = "/models/Qwen3.6-35B-A3B-MTP-UD-Q4_K_M.gguf"
        let one = ServerArgs(model = listed, streams = 1)
        t |> success(config_mtp_resolved(one, "vulkan"), "one stream on the device the row lists")
        t |> success(config_mtp_resolved(one, "metal"), "one stream on the other device the row lists")
        let no_row = ServerArgs(model = "/models/no-such-model.gguf", streams = 1)
        t |> success(!config_mtp_resolved(no_row, "vulkan"), "no catalog row: plain")
        let wide = ServerArgs(model = listed)
        t |> success(!config_mtp_resolved(wide, "vulkan"), "the four-stream default: plain")
    }
}

//! the slot options both load paths build carry the catalog row's
//! platforms, so an unset `mtp` arms a listed model's slot alike at
//! boot and at a runtime POST load
[test]
def test_slot_mtp_platforms(t : T?) {
    let listed = "/models/Qwen3.6-35B-A3B-MTP-UD-Q4_K_M.gguf"
    let no_row = "/models/no-such-model.gguf"
    t |> run("a [[models]] entry's boot options") @(t : T?) {
        let opts = boot_slot_opts(ModelEntry(path = listed))
        t |> equal(opts.mtp, -1, "the entry leaves mtp unset")
        t |> equal(opts.mtp_platforms, "metal, vulkan", "the catalog row's platforms")
        t |> success(slot_mtp_arms(opts, -1, 1l, false, "gpu:resident"),
            "one resident stream on the listed device arms")
        let plain = boot_slot_opts(ModelEntry(path = no_row))
        t |> success(!slot_mtp_arms(plain, -1, 1l, false, "gpu:resident"),
            "no catalog row: plain")
    }
    t |> run("a runtime POST load's options") @(t : T?) {
        let opts = runtime_slot_opts(listed, false)
        t |> equal(opts.backend, "auto")
        t |> equal(opts.mtp, -1, "the load leaves mtp unset")
        t |> equal(opts.mtp_platforms, "metal, vulkan", "the catalog row's platforms")
        t |> success(slot_mtp_arms(opts, -1, 1l, false, "gpu:resident"),
            "one resident stream on the listed device arms")
        t |> equal(runtime_slot_opts(listed, true).backend, "cpu")
        let plain = runtime_slot_opts(no_row, false)
        t |> success(!slot_mtp_arms(plain, -1, 1l, false, "gpu:resident"),
            "no catalog row: plain")
    }
    t |> run("the slot's own mtp, then the server's, outranks the catalog") @(t : T?) {
        var opts = runtime_slot_opts(listed, false)
        t |> success(!slot_mtp_arms(opts, 0, 1l, false, "gpu:resident"),
            "the server's mtp = false")
        opts.mtp = 0
        t |> success(!slot_mtp_arms(opts, 1, 1l, false, "gpu:resident"),
            "the slot's mtp = false over the server's true")
        let on = runtime_slot_opts(no_row, false)
        t |> success(slot_mtp_arms(on, 1, 4l, false, "cpu"), "an ask on arms anywhere")
    }
}

[test]
def test_prefix_state_budget(t : T?) {
    t |> run("the checkpoint budget: the MB asked, else a quarter of RAM, else 4 GB") @(t : T?) {
        t |> equal(16384l, prefix_state_budget_mb(0l, 64l), "a 64 GB box")
        t |> equal(4096l, prefix_state_budget_mb(0l, 16l), "a 16 GB box")
        t |> equal(4096l, prefix_state_budget_mb(0l, 0l), "RAM unknown")
        t |> equal(512l, prefix_state_budget_mb(512l, 64l), "an explicit budget stands")
    }
    t |> run("a cgroup limit file's byte count, no limit read as none") @(t : T?) {
        t |> equal(58, ram_gb_from_cgroup("61999996928\n"), "a 62 GB container limit (v2)")
        t |> equal(1, ram_gb_from_cgroup("268435456\n"), "256 MiB reads as one GiB, not no limit")
        t |> equal(0, ram_gb_from_cgroup("max\n"), "v2 with no limit")
        t |> equal(0, ram_gb_from_cgroup("9223372036854771712\n"), "v1 with no limit")
        t |> equal(0, ram_gb_from_cgroup(""), "no cgroup file")
        t |> equal(0, ram_gb_from_cgroup("+68719476736\n"), "a signed count")
        t |> equal(0, ram_gb_from_cgroup(".5e11\n"), "text opening on no digit")
        t |> equal(0, ram_gb_from_cgroup("nan\n"), "a float spelling")
    }
    t |> run("a Linux box's RAM: the v2 limit, else the v1 one, where under the total") @(t : T?) {
        let mem = "MemFree:  1024 kB\nMemTotal:       67108864 kB\n"
        let gb16 = "17179869184\n"
        let gb8 = "8589934592\n"
        t |> equal(64, linux_ram_gb(mem, "", ""), "no cgroup: the total")
        t |> equal(16, linux_ram_gb(mem, gb16, gb8), "the v2 limit, the v1 file unread")
        t |> equal(8, linux_ram_gb(mem, "max\n", gb8), "v2 with no limit: the v1 one")
        t |> equal(8, linux_ram_gb(mem, "", gb8), "no v2 file: the v1 one")
        t |> equal(64, linux_ram_gb(mem, "137438953472\n", ""), "a limit past the total")
        t |> equal(16, linux_ram_gb("", gb16, ""), "the total unread: the limit")
        t |> equal(0, linux_ram_gb("", "max\n", ""), "neither: unknown")
    }
}
