# dasLLAMA Architecture - the Vulkan tier's model residency

Companion to `ARCHITECTURE_GPU_VULKAN.md`; section numbers are `ARCHITECTURE.md`'s. This
document carries sections 2.2n-2.2o: the residency plan that sizes a whole model before a byte
uploads, and the marks swap that lets one GPU slot serve many models. The kernels and encode
chains that run once a model is resident are `ARCHITECTURE_GPU_VULKAN.md` sections 2.2j-2.2m
and 2.2p-2.2q; the per-op tier's decode era is `ARCHITECTURE_GPU_VULKAN_DECODE.md` sections
2.2r-2.2v. The GPU backend role table these sections build on stays in `ARCHITECTURE_GPU.md`
sec.1.5.

### 2.2n The residency plan sizes a whole model before a byte uploads {#resident-plan}

The resident driver is all-or-nothing, so the plan IS the decision, and it is computed from
`Model` metadata alone. It sizes four numbers against the tier's weight budget: the dense weight
planes, the KV mirror at `seq_cap`, the driver's own device scratch, and the headroom the auto
arm leaves unfilled (zero when the user pins VRAM). KV is reserved BEFORE weights and never
grows: on a discrete card the two compete directly, and evicting weights to grow KV would mean
re-uploading gigabytes. A decline carries a reason, and where the numbers allow one it carries
the remedy that works - a shorter context, because the weights are fixed and the KV is not.

**The auto arm's headroom is the larger of 2 GiB and 27% of the tier's cap.** WDDM demotes a
process's buffers to system memory by how full the card is, not by a fixed leave-behind, and a
demoted plane reads at PCIe speed with no error: on the 16 GB reference card the 9B hybrid at a
12.5 GB plan (the fixed 2 GiB headroom under the 14.7 GB cap) decoded at 6.8 tok/s, at 12 GB
pinned 3.4, at 11 GB pinned 49.8. The share keeps a 16 GB card's plan near 10.7 GB and leaves
an 8 GB card's plan where the fixed term already put it. A pinned `VRAM_MB` takes no headroom:
the number is the user's contract, and the plan fills it.

An OPTIONAL plane rides only the room left under the budget at THIS context - what remains of
`budget_bytes - headroom_bytes` after weights, KV and scratch; the reserved headroom itself
stays unfilled. It never shrinks any of the three, and it reports zero bytes when it does not
fit - so the same model plans the plane in at a short context and out at a long one. The raw f32 embed
table is the one optional plane today.

### 2.2o One GPU slot, many models: the marks swap {#gpu-slot-marks}

A multi-model host runs one device tier under several loaded models, and the tier's per-model
state is offset-keyed - two models' marks installed together route one model's dispatches at
the other's planes. `GpuModelMarks` is that state WHOLE: the loader-contract marks plus every
resident-driver per-model global (the activation, the mirror count, the mirror cap, the mirror
codec, and the device-embed arm). The save moves the installed state out and leaves the globals
reading as no-model; the restore is its exact inverse. The whole-model drop clears the same set
and deselects the `"vulkan"` overrides, so a dropped model's prefill and decode take the plain
CPU path and a later re-arm passes `resident_upload`'s no-active-override gate. The three carry
the same set, which is why a model's device state never survives into the next. The upload
rail enforces it from its own side: a load that finds marks still installed - a model deleted
without the drop, the shape every test process and single-model tool takes - drops that
model's device state before uploading its own, carrying the load's MoE layer request across
the drop (the one mark the drop's reset would otherwise zero before the rail reads it).
Without that drop the second model's stacks
land beside the first's, and the offset-keyed stack lookup serves whichever model's plane
registered that offset first: the decode attention block asserts on the geometry change, and a
model whose geometry matches decodes the earlier model's weights.
