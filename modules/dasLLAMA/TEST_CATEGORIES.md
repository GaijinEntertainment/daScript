# dasLLAMA test categories

A category is a set of model files no test run opens unless the run names it. A model sits in a
category when a smaller model of its family takes the same paths: the per-PR suites run the small
one, and the category holds the cells only the large one reaches. The table is
`tests/_model_categories.das`; `tests/REVIEW.das` holds this file to it.

Run a category when the work touches one of its topics - on this box first, on a pod when the
change reaches a device this box has none of:

```
./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --category <name>
```

A category run opens its categories' files and no other model: every cell on another model skips,
and so does every category cell in a run that does not name it.

## Categories

| Category | Model files: a name that starts with | Run it when the work touches |
|---|---|---|
| `gemma4-e4b` | `gemma-4-E4B`, `mmproj-gemma-4-E4B` | the gemma-4 audio embedder widening 1536 to a 2560 decoder, the gemma-4 vision projector at 2560 with no std bias |
| `gemma4-dense` | `gemma-4-12B`, `gemma-4-12b`, `gemma-3-4b` | the dense gemma-4 decoder: the global layers that take V off the K row at heads of 512, the checkpoint's suppress list, K-quant and Q8_0 planes at the 12B's shapes; the gemma-3 dense base at the 4B's wider geometry, and the image chat on it |
| `gemma4-31b` | `gemma-4-31B`, `mmproj-gemma-4-31B` | the Metal residual step past its 4096-float slab, on a decode of the 31B's 5376-wide rows |
| `gemma4-moe` | `gemma-4-26B`, `mtp-gemma-4-26B`, `mmproj-gemma-4-26B` | the routed gemma-4 decoder: the experts under GeGLU with a per-expert down scale, the expert stacks in MXFP4, IQ and K-quant formats, the NextN draft head, the DFlash block drafter's rounds on the routed trunk |
