# dasLLAMA Code Review Checklist - placement

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: the `ARCHITECTURE_*.md` set beside this file - sec.1 in each is the per-file charters.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with
it.**

**A per-file inventory restated in this checklist is a defect of the checklist.** The sec.1
charters own the per-file list; a rule naming what KIND of code lands in which file is the
checklist's own.

**A function whose KIND the file's sec.1 charter line seats in another file lands in that
file, or the charter line changes in the same diff.**

**A HOST-side tensor format conversion lands in `dasllama/dasllama_convert.das`; a kernel-side
decode helper rides its backend's kernel home.**

**A disk-order -> compute-order transform lands by its consumer: a transform into the layout
a CPU row core reads in `dasllama/dasllama_repack.das`, a transform into the layout a GPU plane
or gather reads in `dasllama/dasllama_layout.das`.**

**A CPU KV-cache store, read, score dot, or V-accumulate OVER CACHE BYTES - a codec primitive
that knows the K/V element format - lands in `dasllama/dasllama_kv_codec.das`, its format
family kept whole.** A dot over an already-decoded f32 row is not a codec primitive. GPU twins
land in their backend kernel file.

**A pre-tokenizer split lands in `dasllama/dasllama_pretok.das`; a merge algorithm in its
backend file (`dasllama/dasllama_spm.das` / `dasllama/dasllama_bpe.das`).**

**A kernel body - the arithmetic loop itself, the one a `[tune]` family or a dispatch class (a
class a `[metal_dispatch]` or `[vk_dispatch]` declares) picks one variant of - lands in its
owner's backend file.** A GPU kernel body lands in its backend's kernel home -
`dasllama/dasllama_metal_kernels.das`, `dasllama/dasllama_vulkan_classes.das` - never in a
driver, seat or math file. A CPU-tier kernel body lands in that tier's
`dasllama/dasllama_math_<tier>.das`, never in `dasllama/dasllama_math.das`. A class stamped from
a template declared elsewhere is not a kernel body: it compiles its own PSO where it is stamped.

**A quirk of one family - one model architecture's file, or one backend driver's - lands in that
file, never sideways into a sibling.**

**A piece two files both execute lands in their nearest shared file (its own file when none
exists) - never a second copy.** A predicate, a constant, or a helper spelled once in each of
two files drifts on the first edit to one copy; an enum-and-int twin of one predicate inside one
file is the tier's idiom, not a copy, and a test's CPU oracle that restates the arithmetic is a
witness, not a copy. A piece two folders outside each other both need lands in the folder that
owns the concern; one landing under `dasllama/` that code outside `modules/dasLLAMA/` drives
lands as a public entry module - one `dasllama/dasllama_lint.das` licenses a consumer to
require directly.

**A family gaining an arm for a media kind adds that kind's span markers to that family's chat
template, never to a second renderer.** Span markers are the template text that opens and
closes the media rows. A family whose template or vocab lacks them has no arm for that media
kind.

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

**No engine file (`dasllama/`) other than `dasllama/dasllama_audio_io.das` requires `audio`
(the miniaudio decode module).**

**No engine file (`dasllama/`) other than `dasllama/dasllama_vision_io.das` requires
`stbimage`.** Benchmarks, harnesses, and tests decode their own fixtures.

**Engine, HTTP, or response-writing logic never lands in `dasllama/dasllama_scheduler.das`** -
engine logic in engine files; HTTP, and the code that turns a step's output into the wire text
a client reads, in `utils/dasllama-server` (repo root).

**An `[init]`-only side-effect require in an engine file (`dasllama/`) lives in
`dasllama/dasllama_transformer.das`** - arch registrations, GPU tiers, every module requiring
the engine back. It lives in `dasllama/dasllama_common.das` instead when code there depends on
the registration having run and the registered module does not require the engine back; when
it does, `dasllama/dasllama_common.das` panics on the unset hook with a message naming the
module to require. A program root (test, harness, benchmark, tool) requires the registration
module it needs directly.

**A `dasllama/` module whose `[init]` registers a hook the engine dispatches through gets its
side-effect require in the same change that adds it - in `dasllama/dasllama_transformer.das`,
or in `dasllama/dasllama_common.das` where the rule above seats it there** - a registration
neither file reaches never fires for a consumer of the `dasllama.das` facade.

**An architecture file (`dasllama/dasllama_arch_*.das`) that changes a forward loop, or tests a
family name on a shared path, is a defect - it carries declarative registration only.**

**Platform-specific code in an engine file (`dasllama/`) lands only in that platform's backend
file.**

**A diff that adds to `dasllama/dasllama_common.das` a module global (`let` or `var`, private or
not) whose concern the file's charter line in `ARCHITECTURE_ENGINE.md` sec.1 does not name is a
defect - give the concern its own file, or extend the charter line in the same change.**
