# dasLLAMA Code Review Checklist - placement

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** The per-file
charters are `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_GPU.md` and `ARCHITECTURE_MEDIA.md` sec.1.

**Routed from `REVIEW.md`: a diff that adds a file under `dasllama/`, moves code between files,
or lands a kernel, codec, transform, tokenizer, tool-wire, media-IO or registration concern
in a new place applies this list together with `REVIEW.md`.**

**A per-file inventory restated in this checklist is a defect of the checklist.** The sec.1
charters - `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_GPU.md`, `ARCHITECTURE_MEDIA.md` - own the
per-file list. A rule naming what KIND of code lands in which file is the checklist's own.

**A tensor format conversion lands in `dasllama/dasllama_convert.das`.**

**A disk-order -> compute-order transform lands per scope: kernel-layout in
`dasllama/dasllama_repack.das`, load-scope in `dasllama/dasllama_layout.das`.**

**A CPU KV-cache store, read, score dot, or V-accumulate lands in `dasllama/dasllama_kv_codec.das`,
its format family kept whole.** GPU twins land in their backend kernel file.

**A pre-tokenizer split lands in `dasllama/dasllama_pretok.das`; a merge algorithm in its backend file
(`dasllama/dasllama_spm.das` / `dasllama/dasllama_bpe.das`).**

**A kernel body lands in its owner's backend file.** A GPU kernel body lands in the file where
its PSO is compiled and released. A CPU-tier kernel body lands in that tier's
`dasllama/dasllama_math_<tier>.das`. A kernel body never lands in
`dasllama/dasllama_math.das` or in a lens/dispatch macro file.

**A family quirk lands in the family file; a piece two families need moves UP into the
concern's shared file (its own file when none exists)** - never sideways into a sibling.

**A family gaining an arm for a media kind adds that kind's span markers to that family's chat
template, never to a second renderer.** Span markers are the template text that opens and
closes the media rows. A family whose template or vocab lacks them has no arm for that media
kind - `create_chat_` panics at create, not at render.

**No signature in `dasllama/dasllama_tower.das` takes a type that
`dasllama/dasllama_audio.das`, `dasllama/dasllama_vision.das`, or a family file declares.**
`dasllama/dasllama_tower.das` is the shared encoder-tower home. A doc comment naming the
family a helper was built for is fine.

**`dasllama/dasllama_tower.das` requires none of `dasllama/dasllama_audio.das`,
`dasllama/dasllama_vision.das`, or a family file - a diff adding such a require is a defect.**

**A `dasllama/dasllama_tower.das` helper with one calling family lands in that family's
file** - a single-caller helper sanctioned as tower-worthy is ledgered on
`ARCHITECTURE_MEDIA.md` sec.1.7's tower charter line, not argued in review.

**Tool wire text (the text of a model's tool/function call, built or parsed) is produced only
in `dasllama/dasllama_tools.das`.**

**No engine file (`dasllama/`) other than `dasllama/dasllama_audio_io.das` requires `audio` (the
miniaudio decode module).**

**No engine file (`dasllama/`) other than `dasllama/dasllama_vision_io.das` requires `stbimage`.**
Benchmarks, harnesses, and tests decode their own fixtures.

**Engine, HTTP, or writer logic never lands in `dasllama/dasllama_scheduler.das`** - engine logic in
engine files, HTTP in the server, writer logic in the writer's own file.

**An `[init]`-only side-effect require in an engine file (`dasllama/`) lives in
`dasllama/dasllama_transformer.das`** - arch registrations, GPU tiers, every module requiring
the engine back; it sits in `dasllama/dasllama_common.das` only if engine code needs it. A
program root (test, harness, benchmark, tool) requires the registration module it needs
directly.

**An architecture file (`dasllama/dasllama_arch_*.das`) that changes a forward loop, or tests a
family name on a shared path, is a defect - it carries declarative registration only.**

**Platform-specific code in an engine file (`dasllama/`) lands only in that platform's backend
file.**

**A diff that adds a new engine concern that is not `Model`/`Session`/`Config` state to
`dasllama/dasllama_common.das` is a defect - give the concern its own file.**
