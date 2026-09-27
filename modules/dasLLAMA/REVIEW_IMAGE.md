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

**A weight carrier - anything that reads weights from a file and serves inference from them -
whose `.dlim` is missing mints it first and is served from what was minted.** A carrier the
image rail does not serve is listed as unserved in `ARCHITECTURE_IMAGE.md`.

**A weight carrier becomes live only through `build_image` and `parse_image` in
`dasllama/dasllama_image.das`: reading weights into a live carrier, or releasing an image
backing, anywhere else is a defect.**

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

**A path that reinterprets a mismatched image, or widens an identity so that more files match,
is a defect.** Anything that changes what an image contains - the box profile, the knobs, the
flavor, a head that rides the load - goes into `image_identity`, the string the header bakes,
not only into the image path; a mismatch declines loudly.

**An image save deletes its own lane's images whose identity no longer matches, and any image
the verdicts prove garbage in any lane - BROKEN, version-stale, or a stale layout of a family
this process registered; deleting any other image - a current image of another flavor or
another family, or one whose family this process cannot recompute - is a defect.** A lane is an
identity's (quant, tag) pair.

**A plane split that follows the source FILE rather than a runtime knob takes ONE image tag**,
with the meta flags describing the layout - a per-tensor type split is not a second flavor.

**A plane the target platform or config never reads is not written into the image - the mint
decides that, not the load.**

**A flavor takes its image file through `image_path_for` and its tag through
`register_image_family_tag`.**

**A family whose plane bytes no box property shapes - no backend pin, no tune state, no lane
width - registers its tag with `register_image_family_tag(tag, config_free = true)`; a family
whose bytes any such property shapes never does.** A config-bound identity on a property-free
family bakes one image per tune state and reaps the others on every switch; a config-free
identity on a property-shaped family serves the wrong bytes under a changed property.

**A diff that changes what an image at an UNCHANGED path contains without adding or dropping a
field of a struct the image meta serializes - a moved byte, a re-meaning of a serialized field, a
serializer body change - bumps `IMAGE_VERSION` (`dasllama/dasllama_image.das`) in the same
change.** An added or dropped field moves `layout_fingerprint()` and the load refuses that image;
any other change leaves a stale image valid and serving a different model.

**A diff that adds a function that places or sizes image bytes, or changes one so that it does,
adds it to `REVIEW.das`'s `LAYOUT_HELPERS` in the same change unless that gate's layout-stamp
closure already takes it in.** What the stamp covers is
`ARCHITECTURE_IMAGE.md#image-layout-stamp`.

**A diff that takes a function out of the layout-stamp closure names it in the PR body as one
that places no image bytes.**

**A diff that re-stamps the layout-stamp hash without bumping `IMAGE_VERSION`, moving the image
path, or adding or dropping a field of a struct the image meta serializes carries in the PR body
the proof that no byte moved: a byte compare of the `.dlim` minted before and after the change,
for each family and lane the changed functions mint.**

**Weakening a meta field-count tripwire - `IMAGE_META_FIELDS` and every `*_META_FIELDS`
constant in a `dasllama/` file - is a defect.** Raising the constant
without adding the field to the serializer leaves that field out of every image, and a
serializer without the tripwire reads a forgotten field back as zero on every load.

**A decline on any sink an image save writes through - a sink is the file or the memory buffer
the save writes its bytes into - wherever that code lives, never fails the load: warn, and serve
what is still whole - the image already built in memory, or the carrier as loaded.**

**A bounds check on an image section or the meta blob in `dasllama/dasllama_image.das` is
written `bytes > msize || off > msize - bytes`, never `off + bytes > msize`, which wraps.**

**On the lane that serves the file's own planes, a weight plane's element type follows its
SOURCE tensors, per weight region - the set of source tensors a carrier stores in one plane
(a block stack, a merger/projector).**

**A weight region whose source tensors disagree on element type is refused in a message naming
the offending tensor and both element types.**

**A lane that PERSISTS a converted form of the file's planes is a separate flavor under its
own image identity.** A persisted form is one an image could carry. The load that picks such a
lane prints which lane it picked. A conversion made and dropped inside one forward pass
persists nothing and is not such a lane.

**Never regroup or refactor the float products in a mint-side dequant mirror
(`devw_dequant_q8_core` / `devw_dequant_k45_core` / `devw_dequant_k6_core`,
`dasllama/dasllama_convert.das`) - keep the multiply grouping and the subtraction order the code
already has.** The GPU kernel's factored form decides the sign of a zero result, so a
regrouping makes a baked panel differ from the runtime one.
