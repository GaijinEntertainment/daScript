# dasLLAMA Architecture - the Vulkan per-op tier's dense FFN rail and routed form

Companion to `ARCHITECTURE_GPU_VULKAN.md`; a section is cited by its anchor. This document carries the per-op tier's
dense FFN rail, the routed experts' form on its expert chain and the attention-quad chain's gemma decline
(`#perop-attn-gemma`). The tier's decode attention block and the whole-token decode span are in
`ARCHITECTURE_GPU_VULKAN_DECODE.md`, its streamed expert split in
`ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split`.

### The expert chain serves gpt-oss's routed form {#perop-routed-form}

The per-op expert walk places a layer's routed stacks when every plane is q8, a superblock format or a per-32 one (q51,
iq4nl32, and mx4 for a native-MXFP4 model, whose stacks carry no per-tensor tag - `expert_fmt`) and a biased layer's down
plane is no superblock format (`tier_expert_decline`). A gate and up of two activation forms walk with two images of the
rows: the gate's rides the call, the up's rides beside it (`set_moe_gpu_image2`) - the decode GEMV arm fills the up stack's
own activation buffer from it, the batch arm uploads it to a second device image - and the deltanet chain's qkv and z
planes take the same pair, the z plane's image on the window chain and in the decode step's own buffer. A biased model's gate and
up bias rows go up once, every layer's rows in layer order (`moe_gpu_upload_ffn_bias`). A call into the chain under a layer's
form (`moe_routed_form`, set around the call by `set_moe_gpu_ffn_form`) records the clamped swiglu's act code and, under the
bias, the biased act (`Q8ActRqB`): its row base in the bias planes, each chain row's expert written to the row -> expert words
from the regions (`ffn_rex_fill`), on the decode GEMV arm and the batch arm alike; the biased act stores Q8_0 alone, which is
why a biased layer's down plane is per-32. The resident driver's streamed window sets the same form with `bias_resident`
on - the bias planes the driver uploaded, the row base a routed layer's ordinal in them - and the f16 chain's act binds
those planes (`ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split`). The down biases join on the host - a decode row's under its own expert, a combined
prefill position's under each pick's weight. The heat pool admits the same stacks (`heat_advise_layer`): a decode row's
hits run under the layer's form with each slot row's expert named (`set_moe_gpu_ffn_experts`), the misses on the CPU, and
the shared expert's three GEMVs on the device after the hits. On gpt-oss the per-op tier serves no attention sink, so its
attention layers stay on the CPU, and the decode span, the tail fusion and the f16-fed chain stand aside; the decode's
per-layer chain and the prefill's Q8-fed chain with the device combine serve its routed blocks.

### A dense layer's FFN row runs on the device off the dense rail's triple {#perop-dense-ffn}

Under the per-op tier (the whole-model route off, or a model the driver declines) the dense rail's walk places each
dense layer's w1/w3/w2 triple beside the attention q/o pair when its FFN half is asked for (`gpu_want_dense_ffn`,
``DASLLAMA_GPU_DENSE_FFN``, on: the triples are most of a dense model's bytes, and the walk places them ahead of the
rails it reaches after them; ``=0`` keeps the FFN on the CPU, the arm that reads a dense model's served rate at 17 against the
rail's 29 tok/s on Llama-3.2-3B over the pod's card, `PERF_LEDGER.md`). The walk takes a layer whose planes are q8, a repacked superblock format or a per-32 format
(q51, iq4nl32, q4032, q5032), and whose FFN width is a block multiple of the down plane's format (`perop_dense_ffn_decline`); a routed
layer and gemma-4's dense shared expert keep their own rails. A decode step's FFN on such a layer (`dense_ffn_inplace`)
quantizes the normed row to the gate plane's form - Q8_K for a superblock format, else Q8_0 - and runs the routed decode
chain at one region of one row (`matmul_gpu_dense_ffn`): the gate and up GEMVs, the activation with its requant to the down
plane's form, the down GEMV, one submit; the fused CPU chain stands aside on that layer. A gate and up of two activation
forms run as three dense GEMMs instead (`ffn_gpu_gemms`), each fed its own image of the row - the per-32 planes on the
per-32 batch tile - the activation on the host between them. A prefill's FFN GEMMs over the same planes take the dense rail's batch GEMM where the CPU
batch path consults its marks (`moe_gpu_dense_on_gpu`). `moe_gpu_dense_ffn_rows` counts the rows the rail ran. The image's
identity spells the half only when it is on (`dlim_vulkan_tag`'s ` F`), so images minted without it keep their tag.

### The attention-quad chain serves no gemma {#perop-attn-gemma}

The per-op tier's attention-quad chain (`DASLLAMA_GPU_ATTN`: the prefill chain over a layer's q/k/v/o quad and the decode
block over its own mirrors) has no arm for a gemma generation's attention: gemma-2's logit softcap, the sandwich norms
every gemma carries and gemma-4's weightless v-norm (`attn_chain_unserved_features` names each; the walk logs the decline
and places no quad). Past the features its walk takes no sliding-window layer, no layer reading another's K/V and no head
wider than 256, so on gemma-4 its geometry covers neither the global heads of 512 nor the sharers. A gemma the whole-model
driver declines runs its attention on the CPU under the per-op tier; what keeps a gemma on the device is the whole-model
plan fitting, and its sliding layers' mirror rows ride a ring of their window to that end
(`ARCHITECTURE_GPU_KV_RING.md#kv-sliding-ring`).
