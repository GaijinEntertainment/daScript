# dasLLAMA image rail Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_IMAGE.md`.

**Routed from `REVIEW.md`: a diff touching `dasllama/dasllama_image.das`, or a `.dlim` mint,
load, identity, or flavor concern wherever the diff puts it, applies this list together with
`REVIEW.md`.**

**A transform on the go-live path - repacking, quantizing, folding, permuting - is a defect;
it belongs to the mint.** Going live is `parse_image` pointing a live carrier's planes into the
mapped `.dlim`.

**A missing `.dlim` is minted first, and the model is served from what was minted.**

**A weight carrier becomes live only through `build_image` and `parse_image` in
`dasllama/dasllama_image.das`: reading weights into a live carrier, or releasing an image
backing, anywhere else is a defect - and a second mint path, per family, per format, or per
backend, is a defect even where its output is identical.**

**A decoder mint never holds the whole model.** A decoder mint is the mint of an LLM decoder
model, not of a tower or embedder carrier. It sizes the image before writing the first byte and
writes each plane as it is produced. A mint that is slower in exchange for a lower peak is
correct.

**A staged carrier mint (`cache_via_image_staged`) meeting a source file at or past 1 GiB
either refuses it or streams it the way a decoder mint does.** A refusal names that file. The
staged form holds source and image at once, and this rule caps that doubled peak.

**A path that reinterprets a mismatched image, or widens an identity so that more files match,
is a defect.** `image_identity` names the box profile, the knobs, and the flavor a file was
baked for, and a mismatch declines loudly.

**An image save drops AT MOST its own lane's dead siblings plus BROKEN/version-stale images in
any lane.** A lane is an identity's (quant, tag) pair.

**Reaping an image whose identity the code cannot recompute - another flavor's, another
family's - is a defect.**

**A plane split that follows the source FILE rather than a runtime knob takes ONE image tag**,
with the meta flags describing the layout - a per-tensor type split is not a second flavor.

**A plane the target platform or config never reads is not written into the image - the mint
decides that, not the load.**

**A flavor takes its image file through `image_path_for` and its tag through
`register_image_family_tag`.**

**A diff that changes the `.dlim` layout or serialization, or changes which tensors the gguf
loader puts into an image, bumps `IMAGE_VERSION` (`dasllama/dasllama_image.das`) in the same
change.**
Without the bump a stale image stays structurally valid and silently serves a different model.

**Weakening the `serialize_image_meta` field-count tripwire (`IMAGE_META_FIELDS`,
`dasllama/dasllama_image.das`) is a defect** - raising the constant without adding the field to
`serialize_image_meta` leaves that field out of every image.

**A filesystem or chunk-allocation decline while saving an image
(`dasllama/dasllama_image.das`) never fails the load - warn, and serve what is still whole:
the image already built in memory, or the carrier as loaded.**

**A bounds check on an image section or the meta blob in `dasllama/dasllama_image.das` is
written `bytes > msize || off > msize - bytes`, never `off + bytes > msize`, which wraps.**

**On the lane that serves the file's own planes, a weight plane's element type follows its
SOURCE tensors, per weight region - the set of source tensors a carrier stores in one plane
(a block stack, a merger/projector).**

**A weight region - the set of source tensors a carrier stores in one plane - whose source
tensors disagree on element type is refused in a message naming the offending tensor and
both element types.**

**A lane that PERSISTS a converted form of the file's planes is a separate flavor under its
own image identity.** A persisted form is one an image could carry. The load that picks such a
lane prints which lane it picked. A conversion made and dropped inside one forward pass
persists nothing and is not such a lane.

**Never regroup or refactor the float products in a mint-side dequant mirror
(`devw_dequant_q8_core` / `devw_dequant_k45_core` / `devw_dequant_k6_core`,
`dasllama/dasllama_convert.das`) - keep the multiply grouping and the subtraction order the code
already has.** The GPU kernel's factored form decides the sign of a zero result, so a
regrouping makes a baked panel differ from the runtime one.
