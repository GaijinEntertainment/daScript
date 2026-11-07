# dasLLAMA image rail Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_IMAGE.md`. Planned work: `followup_general.md`.

A mint is the step that builds a `.dlim` image from a source file; a flavor is the
backend-and-layout variant an image is baked for, one part of its identity.

**A diff that moves or re-means the `.dlim` header's first two words - the magic number and the
version - or changes their byte layout, updates `examples/dasLLAMA/wasm/mint_models.py` in the
same change.** That script reads both words at their offsets to gate a browser deploy, and no
compile catches it when they move; a bump of the version's value needs nothing there.

**A transform on the go-live path - repacking, quantizing, folding, permuting - is a defect;
it belongs to the mint.** Going live is `parse_image` pointing a live carrier's planes into the
mapped `.dlim`.

**A diff that adds a weight carrier - anything that reads weights from a file and serves
inference from them - serves it from its `.dlim`, minted first where it is missing, or records it
in `ARCHITECTURE_IMAGE.md` as a carrier served without an image, in the same change.**

**A weight carrier the image rail serves becomes live only through `build_image` and
`parse_image` in `dasllama/dasllama_image.das`, and releases its image backing only through
`image_backing_release` in the carrier's `finalize`.**

**A second mint path - per family, per format, or per backend - is a defect even where its
output is identical - add the family, format or backend to the existing mint instead.**

**A decoder mint never materializes the whole image in memory: it sizes the image before
writing the first byte and writes each plane as it is produced.** A decoder mint is the mint of
an LLM decoder model, not of a tower or embedder carrier. A mint that is slower in exchange for
a lower peak is correct.

**A decoder mint never holds two copies of the carrier: the planar model it streamed from is
released before the written image is mapped, and a declined save serves the streamed build
from memory instead of reloading.**

**A staged carrier mint (`cache_via_image_staged`) meeting a source file at or past 1 GiB
either refuses it or streams it the way a decoder mint does.** A refusal names that file. The
staged form holds source and image at once, and this rule caps that doubled peak.

**A path that reinterprets a mismatched image, or widens an identity so that an image matches a
load whose plane bytes would differ from the image's, is a defect.** Anything that changes what
an image contains - the box profile, the knobs, the flavor, a head that rides the load - goes into
`image_identity`, the string the header bakes,
not only into the image path; a mismatch declines loudly.

**An image save at the `image_path_for` path deletes its own lane's images whose identity no longer
matches, and any image
the verdicts prove garbage in any lane - BROKEN, version-stale, or a stale layout of a family
this process registered.** A lane is the last dot-separated segment of the file name before
`.dlim`; a hash-named legacy image's lane is its identity's (quant, tag) pair.

**A deletion an image save makes of any other image - a current image of another flavor or
another family, another lane's image whatever its identity, or one whose family this process
cannot recompute - is a defect.** The explicit GC (`dlim_clean_`, the converter's `--clean`, the
server's boot) deletes the same garbage plus a hash-named legacy image of another identity, and
never a lane-named image of another identity.

**A diff that adds a segment to `image_lane_name` or a field to the identity records in
`ARCHITECTURE_IMAGE.md#image-lane-name` whether the knob coexists (a lane segment AND an identity
element) or supersedes (the identity alone), and a lane segment lands only on the flavors whose
identity carries the same knob.** A lane segment for a superseding knob leaves dead images no
save reaps; an identity-only spelling of a coexisting knob re-mints on every switch; a segment on
a flavor whose identity lacks the knob mints the same bytes twice.

**A plane split that follows the source FILE rather than a runtime knob takes ONE image tag**,
with the meta flags describing the layout - a per-tensor type split is not a second flavor.

**A plane the target platform or config never reads is not written into the image - the mint
decides that, not the load.**

**The path a load looks up a flavor's image at comes only from `image_path_for`, and every flavor
tag is registered through `register_image_family_tag` or `register_image_flavor_tag`; building that
path any other way, or using an unregistered tag, is a defect.**

**A family whose plane bytes no box property shapes - no backend pin, no tune state, no lane
width - registers its tag config-free: `register_image_family_tag(tag, config_free = true)`, or
`register_image_flavor_tag(tag, lane)`, whose default is config-free; a family whose bytes any such
property shapes passes `config_free = false` to either.** A config-bound identity on a property-free
family bakes one image per tune state and reaps the others on every switch; a config-free
identity on a property-shaped family serves the wrong bytes under a changed property.

**A diff that changes what an image at an UNCHANGED path contains - a moved byte, a re-meaning of a
serialized field, a serializer body change - and adds or drops no field of the struct THAT image's
meta serializes, or of one that struct nests, bumps `IMAGE_VERSION` (`dasllama/dasllama_image.das`)
in the same change.** An added or dropped field moves `layout_fingerprint()` and the load
refuses that image; any other change leaves a stale image valid and serving a different model.

**A diff that adds or changes a function a closure member calls to set the order, offset or size
of a piece's or a plane's bytes - not their values - adds it to `REVIEW.das`'s `LAYOUT_HELPERS` in
the same change, unless the layout-stamp closure holds it.** The closure is every top-level
definition under `dasllama/` that is a `serialize*` taking an `Archive`, `build_image`,
`parse_image`, a `*_bake` or `*_bake_into`, a `*_prepare` of `dasllama/dasllama_tts_blocks.das`, a
`LAYOUT_HELPERS` entry, or a `LAYOUT_CONSTANTS` declaration line.

**A diff that takes a function out of the layout-stamp closure - deleting it, renaming it, or
changing it so it no longer qualifies - names it in the PR body with the closure member its bytes
moved to, or as one that places no image bytes; a function it names neither way owes, for each
family and lane it reached, the `IMAGE_VERSION` bump or the byte compare.**

**A diff that re-stamps `REVIEW.das`'s `IMAGE_LAYOUT_STAMP_HASH` without bumping `IMAGE_VERSION`
carries in the PR body a byte compare of the `.dlim` minted before and after the change for each
family and lane a changed closure member reaches at an unchanged path, where the struct that
family's image meta serializes, and every struct it nests, gains or loses no field.**

**A diff that adds a family or a flavor whose images live at new paths and re-stamps
`IMAGE_LAYOUT_STAMP_HASH` names in the PR body every closure member it adds.**

**Weakening a meta field-count tripwire - `IMAGE_META_FIELDS` and every `*_META_FIELDS`
constant in a `dasllama/` file - is a defect.** Raising the constant
without adding the field to the serializer leaves that field out of every image, and a
serializer without the tripwire reads a forgotten field back as zero on every load.

**A decline on any sink an image save writes through - a sink is the file or the memory buffer
the save writes its bytes into - wherever that code lives, never fails the load: warn, and serve
what is still whole - the image already built in memory, or the carrier as loaded.**

**A bounds check on an image section or the meta blob in `dasllama/dasllama_image.das` is
written `bytes > msize || off > msize - bytes`, never `off + bytes > msize`, which wraps.**

**A diff that stores a weight region of the form that serves the file's own planes unconverted in
an element type other than its SOURCE tensors' is a defect - keep the source tensors' type.** A weight
region is the set of source tensors a carrier stores in one plane (a block stack, a
merger/projector).

**A weight region whose source tensors disagree on element type is refused in a message naming
the offending tensor and both element types.**

**A diff that makes a serving form PERSIST a converted form of the file's planes - by adding the
form or by changing one - gives it its own flavor under its own image identity, and serving such a
form under another flavor's identity is a defect.** A persisted form is one an image could carry;
a conversion made and dropped inside one forward pass persists nothing and is not such a form.

**A load that picks a persisted converted form and does not print the picked form after the pick
is a defect.**

**Never regroup or refactor the float products in a mint-side dequant mirror
(`devw_dequant_q8_core` / `devw_dequant_k45_core` / `devw_dequant_k6_core`,
`dasllama/dasllama_convert.das`) - keep the multiply grouping and the subtraction order the code
already has.** The GPU kernel's factored form decides the sign of a zero result, so a
regrouping makes a baked panel differ from the runtime one.
