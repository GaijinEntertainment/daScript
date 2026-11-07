# dasLLAMA Architecture - the tower rail the discrete homes' chains dispatch through

Companion to `ARCHITECTURE_GPU_TOWER.md`; a section is cited by its anchor. This document carries the rail a tower chain
body runs on when the Vulkan and the CUDA home serve it from one text: the plane roles, the rail records and their GEMM
site table, the stamp overload each dispatch lens generates over a rail, the primitives a chain cannot spell through a
stamp alone, the audio seats' install by home (`#rail-audio-installs`), and the family chains on the rail - the spectral
fronts, gemma4a, qwen3a's front and the VAD (`#g4a-chains`), the FastConformer trio and its decoders (`#fc-chains`), the
whisper class (`#whisper-chains`) and the whisper decoder (`#whisper-decoder`).

### The tower rail {#tower-rail}

A tower chain's dispatch sequence - the block loop, the stem, the tail - is the same graph on the Vulkan and the CUDA home:
the same shared templates stamped under the same dispatch names, the same args structs, the same workgroup counts. What
differs is how a dispatch is spelled: Vulkan binds a descriptor set over the planes, declares each plane's hazard bit,
pushes the args and records into a one-shot command buffer under the hazard rail; CUDA launches over the planes' addresses
on one stream. The rail is that difference, and nothing else, as a record: `VtRail` (`dasllama_vulkan_common`) holds the
command buffer, the hazard record and the lane the chain rides; `CutRail` (`dasllama_cuda_common`) the stream and the lane -
one field, `lane : RailLane` (`q8`, `f32`, `f16`; `dasllama_gpu_rail`), which the generic `rail_begin(rl, lane)` sets at an
encode's start. Both hold the chain's planes by role - `TowerBuf` names every plane a tower chain reads or writes (the
residual rows, the feeds, the attention panels, the FFN rows, the stems' planes, the tails' rows, the weight planes), and a
`RailBuf` seat carries the plane's handle, the byte range a set binds and the Vulkan hazard bit (0 on a weight plane and on
every CUDA plane) - so a chain body names a plane once, by role, and the home it runs on binds it its own way
(`dasllama_gpu_rail`, with `rail_buf`, the seat at a role on either rail).

Each dispatch lens generates the rail's stamp overload beside the raw one: `[vk_dispatch]` and
`[cuda_dispatch]` both emit `enc_<name>(rl, roles, pc, wgs)`, where `roles` is the stamp's
bindings as `TowerBuf` roles in binding order; the signature is one builder both lenses call
(`lens_rail_overload`, `dasllama_dispatch_scan`), and each lens hands it its own body. On Vulkan the overload builds the set over the
rail's seats at those roles through the global cached-set table (`vkd_cached_set`, keyed by the
plane tuple, so a set is built once a residency and never kept on a resident), pushes the args and
dispatches under the hazard rail (`vkd_rail_dispatch`); on CUDA it launches over the seats'
addresses on the rail's stream (`cud_rail_launch`). The roles list is a fixed array whose length
the generic reads at compile time, so no per-stamp helper exists on either home: a chain body
calling `enc_tower_ln_cls(rl, fixed_array(TowerBuf.x, TowerBuf.xb, TowerBuf.norms), pc, npos)`
compiles against either rail, and a stamp one home lacks is a compile error at the body, not a
decline at run time.

What a stamp overload cannot spell is a rail primitive, one name with one argument shape on both homes. A primitive whose
body is the same text on both homes is written once, generic over the rail (`dasllama_tower_rail`): the f32 tile
(`rail_mm32`, `#rail-primitives`), a split pick's reduce of the `sk` partial planes into the site's rows (`rail_split_sum`),
a GEMM input's feed off its f32 rows (`rail_site_feed`: the clamp and halfword store into the site's f16 feed on the cm2
arm, the clamp and Q8_0 requant into its Q8_0 feed on the mul_mm arm; `rail_feed` the block sites' form), the q8 GEMV
(`rail_gemv_q8`), and the block GEMM site's compositions over them - the Q8_0 feed with its mark on the mul_mm arm
(`rail_rq_feed`), the site on the chain's lane (`rail_lane_mm` switches on `rl.lane`: the f32 tile on `RailLane.f32`, the
f16 GEMM over the halfword plane `wb` off the site's f16 feed on `RailLane.f16`, the site's tile on `RailLane.q8`) and the
bias pass (`rail_bias_pass`); the site's k split (`rail_site_nsplit`, the pick's split, 1 where it runs whole) is one
generic beside `rail_site` in `dasllama_gpu_rail`. A primitive whose body is the home's plumbing has one body a home
(`dasllama_vulkan_rail`, `dasllama_cuda_rail`): the tile over a site's schedule record (`rail_mm_kq`, `#rail-sites`; where
the site's pick splits k the tile writes the `sk` planes), the site's rows and workgroups (`rail_site_rows`,
`rail_site_groups` - Vulkan's 256-row column rounds its rows up, CUDA's records the encode's), the f16 GEMM (`rail_mm16`:
the KHR tile on the rail's `gemm_khr` on Vulkan, else the cm2 stamp at the column the home's pick gives the rows), the
GEMV's grid and N-column leaf (`rail_gemv_wgs`, `rail_q8_gemv_n`), the bidirectional flash tile over a head count
(`rail_fa`, its workgroups the home's), the plane copy and zero (`rail_copy`, `rail_zero`), the profile mark (`rail_pt`: the
Vulkan ledger's timestamp, nothing on CUDA), and the arm question `rail_mulmm` - true on a Vulkan rail off the f16 feed,
false on CUDA - under which a chain body keeps the Q8_0-feed arm the mul_mm tiles need. A chain body carries that arm whole,
so the one text serves a card without cooperative matrices too, and a Vulkan seat may put its chain on the mul_mm tiles on
any card; every stamp the arm names compiles on both homes, the requant feed's (`TowerClampRq`) included, though a CUDA rail
never takes the arm. The lane reads off the rail as well, so one body serves every weight lane.

The home keeps everything before and after the dispatch sequence: the gate and the arm, the decline by reason (the reasons
one enum both homes count under, `TowerDecline`, and their words one text, `tower_decline_words`, each home naming its own
knob, arm and failed allocation), the resident key and the plane uploads, the scratch grown to the widest encode, the schedule staging, the
command buffer, the copy-out and the served count. A seat sets its rail's lane through its home's begin (`rail_begin` on
CUDA; on Vulkan `vt_rail_begin(rl, lane, cm2, khr, gemm_khr)` beside `VtRail`, which calls it and sets the home's arms), its
sites' picks and binds, walks its schedule records through the shared body's record function into the rail's maps
(`tower_sched_walk` over `TowerSchedRec`, `dasllama_gpu_math`), fills every seat from the resident's planes once every
allocation of the encode is done, and calls the shared body inside one command buffer whose hazard record rides the rail
(`vt_rail_one_shot`, Vulkan) or straight on the stream (CUDA). The body is the family's dispatch sequence once, generic over
the rail type, in `dasllama_tower_chains`; a generic resolves calls in its own module's scope, so the shared primitive
module requires both home rail modules (each optional, since either home may be absent from a build) and re-exports them,
the chain module requires it alone, and every body instantiates for the rails the build carries.

### The plane roles and the site table {#rail-sites}

`TowerBuf` holds every plane a tower chain on either home binds, named by what the plane holds
rather than by the family that first bound it, so one role serves every chain where its plane plays
the same part: `planes` is a stem's input - the canvas's pixels, a front's samples or mel rows, the
VAD's chunks; `col` a stem's or a front stage's im2col rows; `rows_out` an encoder's output rows - the
stem's rows the blocks read, a front's, the rows a decoder reads; `tail_out`, `tail_proj`, `tail_hid`
and `tail_gate` a projector tail's planes; `hg` an FFN's hidden rows. Beside the block, attention,
stem and weight roles stand the fronts' stages (`spec`, `frames`, `mel`, `stage1` to `stage5`, `feat`,
`feat16`, and the `fw16`, `fnorm`, `twiddle` and `filters` slabs), the rel-position tables and
canary's rel plane (`rel_*`), gigaam's halfword `taps`, the kq lane's member planes (`kq_q0` to
`kq_q3` with their scales, a pair a K-quant member of the carrier), the VAD's gates, state and
probabilities, the whisper decoder's caches and partials (`kc`, `vc`, `kxr`, `vxr`, `cq`, `pms`,
`py`, `dec_in`), the encoder rows' feeds and projection (`enc_h`, `enc_q`, `enc_s`, `enc_proj`), the
transducer's weights and state, and a head's weights, logits and picks. The plane table is
`TOWER_BUFS` wide, the enum's length, so a role the enum gains widens it.

A rail carries one schedule plane, `meta`. Every record a chain dispatches - a block site's GEMM, a
front stage's, the cross site's, a GEMV - sits in it at the offset the seat's walk gave it
(`maps[rid]`, in words) past the byte its site counts from (the site bind's `meta_off`), so no role is
one site's schedule. A GEMM tile reads a record at its index from the bound plane's first word, so a
GEMM site counts from byte 0 (`rail_kq_map` refuses a bind past it); the GEMV site alone binds past
the start, its records viewed out of the plane one at a time.

`RailSite` names the GEMM sites: the block sites (`d`, `ff`, `qkv`, and `fc2`, whose pick may split k
over the `sk` planes), the conv module's `d2`, the rel table's projection `rel`, a projector tail's
`tail1` and `tail2`, the stem's `conv2`, a front's `front1` to `front3`, a decoder's `cross` over the
encoder rows, and the q8 GEMV records' `gemv`. Each home's rail holds a pick a site - `sites :
VtTile[RAIL_SITES]` on Vulkan (the mul_mm variant or the cm2 token column, and the k split), `sites :
CutPick[RAIL_SITES]` on CUDA - and every rail holds a `RailSiteBind` a site (`binds`): the planes the
site's GEMM binds by role (the weight quants and scales, the schedule and its `meta_off`, the f16 feed
`xh`, the Q8_0 feed `xq` and `xs`) and the weight format its tile decodes (`fmt`). A seat's rail begin
puts every site on the block sites' planes and the q8 format (`rail_binds_reset`); the seat then
picks the sites its chain dispatches and binds a site whose planes differ - a front's GEMM over its
own feed, the cross site over the encoder rows' feed, a kq site over its member's planes. The tile
(`rail_mm_kq`) reads both: the site's pick decoding the bound format, over the bound planes, at
record `rid`'s map; a lane whose format differs record to record (gigaam's kq lane) binds the site
again before the record. The site's split, rows and workgroups and the split's reduce read the pick
of the site named.

### The GEMM, GEMV, copy and zero primitives {#rail-primitives}

Every primitive names its planes by role, so one text serves every chain. The f32 tile (`rail_mm32`)
reads the weight plane the call names - the norms plane's f32 rows, the halfword plane's bf16 rows
through the bf16 arm (`rail_vis_w` picks between the two for a vision site), any other f32 plane, a
head's or the transducer's projection - at an offset the rail rebases on the norms plane alone, by
its `norms_base`: a norms offset is the model's, so the Vulkan home's partial upload and the CUDA
home's whole one read alike, and any other plane's offset is the plane's own. The bias row, where
one is named (`TOWER_NO_BIAS` naming none), is the norms plane's. The f16 GEMM (`rail_mm16`) reads the
halfword plane and the f16 feed the call names, its output rows `ystride` apart (0: packed at the
width), on the s (32 positions) or m (128) column of the f16 wave form (`f16_cm2_col` through `vt_f16_tc` and
`cud_f16_tc`, `ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-cm2-column`; the columns' outputs bit-identical); a lane GEMM writes y whole, so a
site's split-k reduce (`rail_split_sum`) runs on the q8 lane alone. A feed lands where its site's GEMM reads (`rail_site_feed`).

The q8 GEMV (`rail_gemv_q8`) is one body over the homes' GEMV stamps: `q8_gemv_cls` at one column,
the N-column leaves past it in `GEMV_COL_TILE` tiles (`rail_q8_gemv_n`, each home's ladder - the
column-count stamps on both, the row-pair twins where Vulkan's lever arms them) at the home's grid
(`rail_gemv_wgs`). A GEMV record's region pairs sit in the schedule at a `RAIL_META_ALIGN` multiple,
the largest storage-offset alignment the Vulkan limits allow a device to ask (a device asking more
declines the seat as `align`, `vk_rail_view_aligned`; `vk_ext_roster` names `minStorageBufferOffsetAlignment`), and the primitive narrows the schedule's
seat to that record's `RAIL_GEMV_META_BYTES` for its dispatches (`rail_view`): a seat's `off` is the
byte its binding starts at, the descriptor's offset on Vulkan (`vkd_cached_set_at`, the set cache keyed
on the offsets beside the planes) and the address's on CUDA (`rail_dev`). A plane copy (`rail_copy`)
and a zeroed range (`rail_zero`) are the home's transfer commands - behind the hazard rail's transfer
barrier on Vulkan, ordered on the stream on CUDA - so an encoder-row handoff stays on the rail (no chain
zeroes a range; the CUDA rail cell holds `rail_zero`).

Independent dispatches overlap on both homes. The Vulkan hazard rail puts no barrier between dispatches with no hazard
between them, so the device runs them together; one CUDA stream runs its launches in order, so a body marks where its
launches are independent: `rail_fork`, then `rail_branch(rl, b)` before each branch's launches (0 the stream the fork left,
1 and 2 the side streams, each waiting on the fork), then `rail_join`, after which the stream waits on every branch. The
three marks are nothing on Vulkan and keep the body's dispatch order. The pre-LN chain forks its q, k and v GEMMs, where
the d site runs whole in k (a split site's partial planes are one plane all three would write): three grids of a partial
wave each fill the card together.

### The audio seats follow the GPU home {#rail-audio-installs}

Every audio seat installs through the GPU home, as the vision seats do: each home registers its
installers with `register_gpu_home` and runs them at `[init]` where it holds the tier, and a home
switch runs the named home's. The Vulkan home installs an audio family at a time - the tower's hooks
and flavor (`vulkan_tower_install_audio_of`) and the decoders' (`vulkan_asr_dec_install_of`) - over
`TowerAudioFamily`: the whisper class with whisper's decoder and device mel, qwen3a's front and
projector (its core rides the whisper class's blocks), gemma4a, canary, parakeet, gigaam with its CTC
head, the transducer parakeet and gigaam share with the FastConformer rows' handoff, and the VAD.
Under the CUDA ask the CUDA home's audio installer (`cuda_tower_install_audio`,
`dasllama_cuda_tower_audio`) seats each family from the home its row of `CUDA_AUDIO_HOMES` names: the
Vulkan home's seats until the family's chains ride the rail on CUDA, its own after, each such family an
arm of `cuda_tower_install_audio_family`. The CUDA tower requires that module, so every program holding
the CUDA tower seats its audio families under the ask. Every CUDA audio flavor's pick arms its lane first (`cut_lane_arms`:
the q8 lane's cm2 tile at every column, the f16 lane the knob and the device), then its chain's classes. The tests route a
family's cells by their own row (`CUDA_AUDIO_FAMILIES` in `tests/_gpu_knobs.das`).

### The spectral fronts, gemma4a, qwen3a's front and the VAD on the rail {#g4a-chains}

`dasllama_tower_chains_g4a` holds five bodies both homes run: the log-mel (`tower_mel` - the spectrum, then the log
filterbank under a law the caller names, `TowerMelSpec`: magnitude or power, the floor, the scale, the rows' order;
`tower_whisper_mel_spec` is the whisper preprocessors' law, whisper's and qwen3a's device mel), gemma4a's subsample front
(`tower_g4a_front`: the log-mel, both convs as im2col + the f16 GEMM over the slab + the LayerNorm-ReLU rows, the feature
rows' f16 feed, the input projection at the `front1` site into the plane the caller names), gemma4a's Conformer blocks with
the projector tail (`tower_g4a_blocks`), qwen3a's conv front (`tower_q3a_front`) and MLP projector (`tower_q3a_proj`), and the
VAD's two dispatches (`tower_vad_chunks`). Beside them sits the geometry both homes' seats and bakes read, so neither home
spells it: gemma4a's GEMM regions in record order (`gemma4a_block_regions`, `gemma4a_all_regions` - the family module's, which
Metal's bake reads too), its schedule records
(`tower_g4a_rec` - the rel record keeps the table's raw rows on every column, its planes being sized to them), its shape gates,
the front's grid and plane bytes (`tower_g4a_front_geom`, `tower_g4a_front_bytes`), qwen3a's conv stages (`TowerConvStage`,
`tower_q3a_stages`), its norms span and regions, and both families' conv slabs as halfwords (`tower_g4a_front16`,
`tower_q3a_front16`), and the bakes that store them beside the families' q8 regions under the home's piece prefix
(`tower_g4a_bake`, `tower_q3a_bake`, `vk.*` and `cu.*`).

Every clamped GEMM site of the gemma4a blocks is `tower_g4a_mm`: the site's feed with the input bounds, the GEMM on the
rail's lane (`rail_lane_mm`, the seats setting the lane from `Gemma4aEncoder.gemm_f16`), the output bounds where the
calibration record is live. The q8 lane's GEMM is the tile over the site's schedule record; the exact lane is the f16 GEMM over the halfword
plane seated as `wb` - the GEMM set the f16 staging converted into `wblob`, the offset fields indexing it - with f32
accumulation, off the same feed. The rel table's projection is the `rel` site over the table's own feed: a seat binds the
site's `xh` / `xq` / `xs` to `rel_tabh` / `rel_tabq` / `rel_tabs`, and the body feeds `rel_tab` through it once an encode.
The front's input projection reads its own f16 feed the same way (`front1`, bound to `feat16`) on both lanes. qwen3a's
projector binds `tail1` / `tail2` to `feat16` too, and on the discrete homes' f16 lane (`Qwen3aTower.cf16`) its three GEMMs
are the f16 GEMM over the trio's halfwords (`wcblob`, seated as `wb`) off those same feeds.

gemma4a's projector tail runs at two widths. The out projection lands the encoder's own width (`gemma4a_mid_dim`) in
`tail_out` with its bias, where the weightless rms reads the norms plane's ones row at that width. The embedder then
writes the decoder's width (`proj_dim`) to `tail_proj`. The two are equal on E2B, and E4B's embedder widens 1536 to 2560.
So the regions, the records' groups, the sites' tiles and the two planes are sized apart, each at its own width.

The seats differ where the planes live. The Vulkan home keeps each front in its own command buffer over the front's scratch and
its own schedule plane (`fmeta` seated as `meta`); its seats build the rail from the resident's planes (`vt_g4a_front_rail`,
`vt_q3a_rail`, `vt_mel_seats`, `vt_vad_seats`). The CUDA home (`dasllama_cuda_tower_g4a`) runs gemma4a's front and blocks back
to back on one stream, the front's record the schedule's last behind the blocks'. On both homes gemma4a's front lands its input
projection in the blocks' residual plane `x` - the Vulkan seat arms the blocks' scratch before the front (`vt_g4a_blocks_arm`) -
so a chunk reads nothing back but the soft tokens. Its residents are the shared `CutResident` with the family's other planes by
role beside it (`CutRolePlanes`, `dasllama_cuda_tower_seat`, its verbs `cp_grow` / `cp_up` / `cp_dev` / `cp_release` /
`cp_seats` over a roles list), released together; the module's four seats share the side planes' helpers (`cut_side_*` over
`CUT_SIDE_ROLES`). qwen3a's front and projector meet the whisper-class blocks seat through the hook's host rows on both homes;
the device mel serves whisper and qwen3a alike and installs in the whisper family's arm (`cuda_device_mel_install`).

### The FastConformer trio and its decoders on the rail {#fc-chains}

`dasllama_tower_chains_fc` holds the FastConformer family descriptor both homes read (`TowerFc`, built by `tower_fc_of_cn`,
`tower_fc_of_pk`, `tower_fc_of_ga` over the seat's offsets record) and the bodies both homes run: the block loop
(`tower_fc_blocks` - the rel-position form canary and parakeet serve, the rotary form GigaAM serves through the padded route
`tower_attn_padded`; `ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains`, `ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-rotary`),
the canary / parakeet subsample front (`tower_fc_front`), GigaAM's device mel (`tower_ga_mel`) and conv front
(`tower_ga_front`), canary's projector (`tower_fc_proj`), and the decoders behind the encoders - a command buffer of the
transducer's greedy rounds with the first one's encoder projection (`asr_dec_rnnt_rounds`) and GigaAM's CTC head
(`asr_dec_ctc`). Beside them sit what both homes' seats and bakes read, so neither spells it: the block GEMM records'
sites, rows and groups (`tower_fc_site`, `tower_fc_rec`), the front's three records (`tower_fc_front_rec` - raw rows on every
column, the stage planes being sized to them), the shape gate (`tower_fc_shape_ok`, a scalar function in `dasllama_gpu_math`), the subsample geometry
(`tower_fc_geom`), the fronts' plane bytes at a frame capacity (`tower_fc_front_bytes`, `tower_ga_front_bytes`), and the
device-block bakes (`tower_fc_bake`, `tower_cn_front_bake`, `tower_ga_front_bake`): the same bytes
on both homes under the home's piece prefix (`tower_fc_piece`, `vk.fc.*` and `cu.fc.*`); and the decoders' decline reasons
and their words (`RnntDevDecline`, `CtcDevDecline`), each home naming its own knob and device.

A record's lane reads off the descriptor: the exact lane (`TowerFc.f16w`) runs every GEMM - the blocks', and on canary and parakeet the
rel table's projection, the front's three and canary's projector - on the f16 GEMM over the halfword plane seated as `wb`
(`tower_fc_lane_mm`, off the site's f16 feed, no schedule) at the GEMM's offset in the image's `wblob`: GigaAM's carrier's own
halfwords, canary's and parakeet's the file's f32 saturated to f16 at staging under a discrete flavor (`gemm_f16`,
`tower_dev_halfword`), their f32 blob holding no GEMM, so the bake adds no GEMM piece; the q8 lane runs the site's tile, and the kq lane binds the site's planes to the
record's seat (`TowerFcSeat`: the format, the first block, the member pair `kq_q<m>` / `kq_s<m>` in format-slot order,
`tower_fc_member`) before its tile - every member's tile built at the site's column. The rel table's projection is the `rel`
site over the table's own feed (`rel_tabh`, or `rel_tabq` / `rel_tabs` on the mul_mm arm); the front's three GEMMs are
`front1` to `front3` over `feat16`, its schedule the seat's own plane seated as `meta`.

The seats keep their planes. The Vulkan home seats the resident's planes on the rail (`vt_fc_rail_seats`,
`vt_cn_front_seats`, `vt_ga_front_seats`, the decoders' `rv_rail_seats`), each chain one command buffer; the front reads its
rows back and the blocks upload them. The CUDA home (`dasllama_cuda_tower_fc`, `dasllama_cuda_asr_dec_fc`) keeps the shared
`CutResident` with the family's own planes - the rel tables, the taps, the fronts' stages - in a `CutRolePlanes` by role
beside it, the kq member pairs and the front's schedule apart (`CufResident`); the transducer's `CurResident` holds its
planes the same way. A CUDA image's pieces upload as stored. Where one seat runs the front and the blocks of the same encode (parakeet, GigaAM), the
CUDA front leaves its rows on the device and the blocks copy them in place of the upload: a declined block chain fails
that encode whole and the CPU recomputes it from the mel, so the host rows are never read. Both homes leave the block chain's residual rows on the device under a generation the
session records - one `TowerRowsHandoff` a home (`dasllama_gpu_rail`, read through `rows_handoff_gen` / `rows_handoff_dev`;
`vulkan_tower_fc_rows` / `cuda_tower_fc_rows`, through `register_tower_rows_gen`) - and the transducer's
projection and the CTC head read them device to device where the generation matches, else upload the host rows.

### The whisper class on the rail {#whisper-chains}

`dasllama_tower_chains_whisper` holds the bodies of the towers built on the whisper encoder - whisper's, qwen2audio's,
Qwen2.5-Omni's, ultravox's, voxtral's and qwen3a's core: the conv stem (`tower_aud_stem` - conv1 as the halfword im2col of the
mel block in `planes` into `col` and the f16 GEMM over the `fw16` slab into `stage1`, conv2 as the im2col into the block
feed `xh` and the cm2 tile at the `conv2` site into `x`, each with its bias and GELU off the `fnorm` slab, the positions
added), the blocks (`tower_aud_blocks` - the pre-LN loop `tower_ln_chain` with the tower's post-norm folded into its last
seam where the caller asks for it, the chat projector tail behind it where the encode carries one) and that tail
(`tower_chat_tail`: ultravox's post-norm, rms, two GEMMs at `tail1` / `tail2` with the swapped SwiGLU and the mid rms
between them; qwen2a's and voxtral's pair pool, post-norm, first GEMM, then qwen2a's bias or the erf GELU and the second
GEMM, the soft tokens in `tail_proj`). Beside them sits what both homes' seats and bakes read: the tail's record
(`TowerChatTail`, `tower_chat_tail_of` - the widths and the model's offsets; a seat sets `on` once its classes built), its
shape and plane gates, its schedule records (`tower_ct_rec`), the stem's record, slab widths and halfword slab
(`tower_aud_stem_rec`, `tower_aud_stem_kdp`, `tower_aud_stem16`), and the device block's bake (`tower_aud_bake`: the q8
regions `tower_aud_regions` gathers and the stem's slab) - the same bytes under each home's piece prefix (`vk.aud`, `cu.aud`,
`tower_family_piece`).

The discrete homes' second lane (`AudioTower.gemm_f16`, the rail's `RailLane.f16`) is an arm of the same bodies: the f16 staging lays the q8
lane's compact layout with every GEMM region the q8 lane quantizes stored as halfwords dense in `wblob`, at the offsets the q8
lane gives it (`ARCHITECTURE_MEDIA.md#tower-weight-lane`), and the blocks' GEMMs, conv2's and the tail's two go through `rail_lane_mm` - the f16 GEMM
over `wblob` seated as `wb`, f32 accumulation, off the same f16 feeds the q8 lane's cm2 tile reads. conv1 is the halfword slab
GEMM on both lanes. A seat on the f16 lane uploads the compact f32 blob as the norms plane and the halfword plane as stored, picks
no site and walks no schedule, and its bake carries the stem's slab alone.

The stem and the blocks are two hooks, so the stem's rows stay on the device between them on both homes: the stem's seat
marks them pending against the host array they are owed to, the blocks' seat takes them in place of the upload, and a
blocks seat that declines first lands them in that array, since the CPU block loop reads it. The stem's seat grows the
blocks' scratch at the blocks' own shape, so the blocks' seat regrows nothing the rows sit in. A served post-normed encode
leaves its `xb` rows on the device under the host array's address and length - one `TowerEncOut` a home
(`dasllama_gpu_rail`, read through `enc_out_dev`; `vulkan_tower_enc_out`, `cuda_tower_enc_out`); every audio hook entry clears that handoff, so it names the encode in flight alone. The CUDA seats
(`dasllama_cuda_tower_whisper`) keep the shared `CutResident`, the Vulkan seats `VtResident`'s stem and tail planes, each
seated on the rail by role.

### The whisper decoder on the rail {#whisper-decoder}

`dasllama_asr_dec_chains` holds the whisper decoder's two bodies: the window's cross K / V (`asr_dec_whisper_cross_kv` - the
encoder rows taken off the tower's `xb` plane by `rail_copy` where it landed them, their feed at the `cross` site, two tiles a
layer into `enc_proj` each stored into the halfword planes `kxr` / `vxr`) and the decode step (`asr_dec_whisper_step` - per
layer the fused row passes that land the next GEMV's Q8_0 feed in the same dispatch, the q8 GEMV sites, the self K / V
stores, the chunked attention over self and cross keys, then the tied-embedding logits of the batch's last row). Every GEMV
is a record at the `gemv` site: one schedule plane carries the cross records from word 0 and each GEMV record's region pairs
at its own `RAIL_META_ALIGN` multiple past the GEMV site's offset (`wdec_meta`), so `rail_gemv_q8` views the plane at that
record alone. The step rewrites one word of it - the logits record's activation block, the last row's - before each batch.

Both homes upload the same f32 plane, the layers' rows from the first layer's with the final norm's behind them
(`wdec_norm_rows`), so the norms plane's base is the first layer's row and the final norm sits at its plane offsets
(`WdecGeom.lnf_w`, `lnf_b`); the geometry, the buffer sizes and the bake's regions (`wdec_geom`, `wdec_sizes`, `wdec_regions`)
are one text. Shared too: the decline reasons (`WdecDecline`; `align` a device whose storage-offset alignment does not divide
`RAIL_META_ALIGN`, which Vulkan alone can ask), the window gate and the step's live-window and batch-cap check (`wdec_gate`,
`wdec_step_admits`), the say-once lines (`wdec_miss_say`, `wdec_announce`; the FC chain module's `rnnt_dev_miss_say` /
`rnnt_dev_announce` / `ctc_dev_miss_say` / `ctc_dev_announce`, each home naming itself) and a decoder state's identity
(`wdec_uid`). The live window - one decoder state at a time, claimed by a served cross K / V - is each home's. The decoder keeps
its own rail on each home (`WdResident` with `g_wd_rl`; `CudWdResident`, the shared `CutResident` of `family = "wdec"` grown
through `cut_grow`, with `g_cud_wd_rl`), its seats, picks and schedule maps written once a model.
