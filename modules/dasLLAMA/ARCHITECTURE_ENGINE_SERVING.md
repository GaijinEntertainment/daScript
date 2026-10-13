# dasLLAMA Architecture - serving and prefix-cache charters

Companion to `ARCHITECTURE.md` beside `ARCHITECTURE_ENGINE.md`; a section is cited by its
anchor.

### Serving {#scheduler-step}

- **`dasllama_scheduler.das`** - the continuous-batching scheduler, the serving layer over the
  facade (its one engine require is `dasllama/dasllama`). One synchronous thread: each
  `scheduler_step` reaps the streams that finished on an earlier tick (`reap_finished` - the
  finished turn's close tokens eval, its snapshot and its donation run only once the caller has
  drained the finished event, so they never sit between a reply's last token and its finish on the
  wire; the reap runs ahead of admission, so the slot a finished stream frees goes to a queued
  request on the same tick), admits queued requests while no admitted stream is prefilling with a
  SHARED checkpoint stop still ahead of it - the opening's head, the system opening, a stable
  opening that is no tail (a request admitted then would prefill the opening that stop is
  about to leave for it; admitted after the stop it attaches the checkpoint, the opening two
  requests of one minute share, instead of prefilling it again - and a prefilling stream past its
  shared stops, with only its own conversation's stops ahead, or with none, holds nothing back,
  since the newcomer's chunks ride the same windows; a
  queued request holds no session - its KV memory exists from admission on, and in paged mode
  admission attaches the longest prefix-cache hit, whose positions never prefill), runs one `eval_batch` decode step over every
  decoding stream (a self-speculative scheduler instead ticks every stream's round through
  `mtp_spec_eval_batch`, one joint verify where a driver seats one, and counts the tick as a
  batched step only when every stream's rows rode it; in device mode every seat of the round - the
  draft, its verify, a rejected round's rollback, the continuation's seam row - runs on the device
  over the stream's region, so the server homes a speculating slot's streams there only where the
  armed driver serves them all, `gpu_resident_mtp_device_home`), then - unless a stream is still on its prompt
  and the decode phase emitted an event, a token or a finish, which ends the tick there (the caller
  drains the event to the wire, and the chunk the tick owes runs first on the next tick, which
  decodes nothing: a token a stream produced never waits out another stream's prefill window) - ONE
  prefill call over every prefilling stream's next text chunk (`prefill_tick`: the segments of one
  window through `eval_segments`, FCFS inside the quantum - the first stream's chunk whole, the next
  streams' rows while the quantum leaves room - so two prompts' tails cost about one window where
  the GPU serves segments (`ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-segments`) and run one after
  the other elsewhere; a stream with a media span ahead takes no segment, and when the first
  prefilling stream is one, or the scheduler is device-home on a model the driver's segmented window
  declines (`gpu_device_prefill_segments`: a recurrent, per-layer-embedding, NextN, routed or
  hyper-connection model, or a block-codec mirror - each stream's chunk would cost its own window
  and the tick would cut the next stream's prompt short for nothing), the tick is that stream's own
  call, `prefill_one_chunk`) - the quantum `chunk_tokens` while
  a stream decodes, since the call is the stall every decoding stream waits out, and at least `idle_chunk_tokens`
  while none does, since a prefill window's cost is mostly fixed and nothing waits on it; `chunk_defaults` names
  both sizes per serving backend (the chunk 512 where a GPU prefills and 64 on the CPU, whose chunk costs its token count, so the stall follows it down; idle 512,
  2048 on Metal, where four windows in one call run within a few percent of the whole prompt's rate); paged serving donates finished streams'
  KV pages to the prefix cache, device mode parks their regions. The paged pool's blobs hold reserved address
  room for every stream's context plus the cache's retention, up to `KV_POOL_RESERVE_BYTES` (`kv_pool_reserve`):
  inside it a doubling moves and fills nothing, where a moved blob is a copy of every cached page inside one
  request's first token. A prompt is one position stream: a media
  span's rows stand in it as ids of the media's content (`media_position_id`), so the prefix cache matches,
  attaches and donates across an image or a clip as across text. A span evals as one body, and up to
  a chunk of the text either side of it rides that body (`prefill_media_body`) - a prefill call's cost is mostly
  fixed, so a turn's opening, its media and its closing text are one call where three would each pay the floor. On a
  gemma-4 E-series decoder the body carries each row's token id (`Session.embd_ids`, 0 for a media row): a text row
  takes its own token's per-layer input and only the media rows the padding token's.
  A span attaches
  whole - a hit ending inside one counts as no hit - and a hit past a grid-roped span restores the span's
  rope advance on the session. A request may leave a span's rows out where it counts on the cache to hold
  the span (`prefix_match_len` is the probe for that); if the cache has lost it by admission the stream
  finishes `media_lost` and its caller brings the rows. A request may carry a `TokenConstraint`
  (`PendingReq.constraint`, a `TokenConstraintBox`); the stream takes it over at admission, points
  its session at it, and frees it with the stream. A constrained stream never joins a speculative
  round and never asks the device for its pick: both would commit tokens the constraint has not
  seen. On a self-speculative scheduler the round skips such streams and they take one plain
  `eval_batch` step after it, in a batch of their own. A stream whose constraint admits no token
  finishes `constraint` with nothing emitted for that step. A request's stop strings (`PendingReq.stop_strs`)
  end the stream where one's text begins, finish `stop`, through a hold on the piece stream: a token's text
  that could still begin a stop string waits in `Stream.stop_hold` (`stop_scan`), rides the next piece event once
  it cannot, and flushes as a piece of its own when the stream finishes another way - so no emitted byte is ever
  taken back, and a stop string split across tokens still cuts. Results flow out as `SchedEvent`s - no HTTP here.
  `utils/dasllama-server` owns the writers; `tutorials/dasLLAMA/13_serving.das` is the
  teaching consumer; `tests/test_scheduler.das` gates it against `generate()` references.
  The step clears its gather arrays (`batch_rows`, `batch_toks`, `batch_idx`) at the end of its
  decode step, before the tick's prefill chunk and before the next tick's reap: `batch_rows` holds
  borrowed pointers into the sessions the reap deletes, and a validating heap collect between
  steps walks every pointer the array still holds. `prefill_tick` clears `seg_rows` before it
  returns for the same reason: it holds borrowed pointers into the streams' sessions, and
  admission and the reap move the streams.

### The prefix cache's partial page {#prefix-tail-page}

The page cache shares the whole pages a prompt matches and COPIES the one page after them for the
leading rows the prompt shares with it. `prefix_insert_` enters a donation's pages whole and its last
page partial (keyed by the hash of the history each closes), and links each entry to the pages that
followed it, a donation each (`PrefixEntry.nexts`). `prefix_attach_` walks the whole pages by hash,
then takes, of the pages linked past the last hit, the one sharing the most leading rows with the
prompt, short of the prompt's last token; the taker gets `kv_pool_copy_group` of it and writes
the page's remaining rows itself, the donor's page staying the cache's hold alone. A conversation's
next turn therefore attaches all of the history it repeats, not its whole pages - the served turn's
shape is a reply that filled the page the prompt ends in: with 64-row pages a turn prefilled 57 to 76
tokens through a 128-expert MoE where the reference prefilled the turn's own 19, and that was the
one served row it led (`PERF_LEDGER.md`, the qwen ladder). A link is weak: an evicted page's key
misses, and a page reaching nothing cached past it attaches nothing partial. A prompt no whole
page serves reads the same kind of list off the cache itself - the key of every donation's first
page (`PrefixCache.roots`) - and copies the first page it shares the most leading rows with. The
match is therefore the longest common prefix of the prompt and a cached history at any length - a
request that repeats an earlier one's opening and changes a word reuses the rows before the word,
under a page or past it.

### The reasoning budget {#think-budget}

A request's `thinking_budget` (`PendingReq.think_budget`, the server's field of that name; 0 = none)
is the number of reasoning tokens a thinking reply may spend before the scheduler closes the span
for it. The scheduler reads tokens, not text, so the span's bounds come with the request as token
marks (`ThinkBudgetMarks`, `think_budget_marks_`): the token SEQUENCE a reply writes to open the
span (the family's open marker encoded with its specials parsed - Qwen's `<think>` is one token,
gemma-4's `<|channel>thought` the channel mark and a word, harmony's the channel mark and
`analysis`, which the answer's `final` channel does not match), the one token the model writes to
leave it (`close_tok`: the close special, harmony's `<|end|>`), the tokens the budget forces, and
whether the generation prompt already opened it (Qwen3.5/3.6's opener ends inside the block;
gemma-4's continuation after a tool result re-opens the channel). A stream matches the open
sequence token by token (`think_budget_track`, `Stream.think_open_at`), counts every token it emits
inside the span, leaves on `close_tok` - never on a token of the forced close, whose first token a
symmetric family's reasoning writes freely - and, at the budget, queues the forced close as
`Stream.forced`: the steps that follow emit it in the sample's place, the sample still drawn so a
device pick landed for the step is consumed and the sampler's state advances as on every other
step (`sample_advance`). The length cap drawing within the forced close's length forces it early,
so the close lands inside `max_tokens`; a cap already shorter than the forced close keeps the
model's own close marker alone, so the span still ends. The forced close is the family's own - a symmetric family's
close special after the sentence the Qwen3 recipe inserts at a spent budget (a bare close
mid-thought leaves the model reasoning on in its content and closing again at the end), gemma-4's
`<channel|>`, harmony's `<|end|>` followed by the final channel's header, each followed by the
template's blank line - so the server's reply-side splitter reads it as the model's and the answer
begins as content. Once the forced close is out, the marks' `reopen` tokens arm the stream's
instruct-mode marker guard (`Stream.nothink`, its content already seen): a span the model re-opens
past the budget - gemma-4's channel markers, harmony's channel mark, a symmetric family's open
special - ends the turn as a
marker after content does in instruct mode, since a thought re-opened after the cut never reaches an
answer. The marks are looked up by name, not through the tokenizer's special parse, which does not
see an asymmetric bracket such as `<channel|>`. A stream with a forced close pending
leaves the speculative round for a plain step, as a constrained stream does: the round's drafts
would run past the close. A budget on a turn that does not think, or on a vocab without the
markers, has empty marks and acts on nothing.

### The prefix cache on a recurrent model {#prefix-recurrent-checkpoints}

A deltanet hybrid's K/V pages continue nothing alone: its recurrent state exists at one position only, so `dasllama_prefix.das` caches a CHECKPOINT (`PrefixState`) - the evaled tokens, their page groups, and a `DnSnapshot` (`dn_snapshot_take`: the state brought to the host through `dn_state_to_host`, where a device driver registers its mirror's copy-down; the n-gram ring; the draft head's carry). A prompt attaches the deepest checkpoint whose every token opens it: the pages below the last row's are shared, the last row's page is copied even when whole (the draft head rewrites row n - 1 for the token that follows), the snapshot is restored. Because the position is exact, the scheduler STOPS a prefill there (`prefix_checkpoint_at`): at the caller's stable opening (`PendingReq.stable_at`, the server's `render_turn_marked` - every earlier turn plus the last turn's opening, where the requests sharing that history part: on the house assistant's traffic the note ahead of the phrase, which every request of the minute carries), else at the longest opening an earlier checkpoint shares; then, a page or more past that, at the last message's close, ahead of the generation prompt (`PendingReq.messages_end_at`, the server's `RenderedTurn.messages_end`; `Stream.ckpt_end_at`; a caller naming none gets the prompt's end less one token): the same prompt asked again and the conversation's follow-up both match there, where a generation prompt the client replays otherwise - the think block a thinking-off turn opens - would end the match a few tokens short of a stop at the prompt's end, and a checkpoint needs its every token; and earlier at the opening every request on the same system prompt and tool set shares (`PendingReq.system_at`, the server's `ChatSession.first_opening`), and earlier still at the opening's shared head (`PendingReq.system_head_at`, `system_head_opening_`: the opening rendered without the tools and without the system text, each matched token by token against the whole, the longer match the head - the system text ahead of a Hermes tool block, the tool block ahead of a Qwen3.5/3.6 system text) - each stop a page or more past the hit and a page or more under the next - so a new conversation attaches the system text another conversation prefilled whatever tools it declares, or the tool block whatever system prompt it carries. A checkpoint within a chunk and a half ends the chunk, so the stop adds no window. A finished turn leaves one more past its close tokens, which the conversation's next turn attaches. The two kinds are kept apart (`PrefixStateKind`): a stop at a shared position - the system text, the system opening, an opening an earlier checkpoint shares - is an `opening`, which other requests share; the stop at the request's own stable opening (`Stream.ckpt_at_tail`) and the finished turn are `tail`s, which only that conversation's next turn extends; and a tail another request later stops at becomes an opening. Kinding the stable stop as an opening is the defect the budget then shows: every conversation's own history counts as shared, and the tails-first order protects nothing. `max_state_bytes` (`--prefix-state-mb`; the server's default is a quarter of the box's RAM, `prefix_state_budget_mb`; a serving box's prompt cache is budgeted this way elsewhere too) bounds their bytes - every snapshot's arrays plus the rows of every held page counted ONCE (`prefix_states_bytes_`, recomputed into `PrefixCache.state_bytes` at each insert and drop: the checkpoints of one conversation share every page under their fork, and `PrefixState.nbytes`, a checkpoint's own figure, would charge those pages once per checkpoint; the pool grows on demand, so the held pages are memory the budget alone bounds) - and `max_states` (`DEFAULT_PREFIX_STATES`, 16, for a library caller; the server's `SERVER_PREFIX_STATES`, 256, is `--prefix-states`'s default) caps their count; past either, the tail used longest ago goes first, and an opening only when no tail stands; the newest checkpoint stands even when it alone passes the byte budget.

### The host pool behind device-home streams {#device-home-pool}

A device-home stream's openings live on the card in two places only: the parked claim a region keeps for the finished stream,
and the region's `RDEC_DN_SNAPS` checkpoint rows (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`, the device-home checkpoints). Both
die when another stream takes the region or a newer checkpoint takes the row, so the card holds as many openings as it has
regions - four on a four-stream slot - where a host-cached slot's prefix cache holds what a quarter of the box's RAM holds.
A workload that keeps more openings alive than that (a shared system prompt per channel, a tool block, several conversations
a minute) loses them on the card and keeps them on the host, so the card's cache is the smaller one exactly when the card is
the faster tier. The pool closes that: a paged scheduler in device mode keeps the same `PrefixCache` over its `KVPool` that
its host-cached streams use, and a device-home stream's openings enter it as copies. Live streams stay device-home - nothing
crosses the bus per token; the pool holds the parked and the evicted state.

- **Down.** At each checkpoint stop (`note_prefilled`) the stream's rows up to the stop and its recurrent state there go into
  the cache as a checkpoint - an opening at or under the request's shared opening (`Stream.ckpt_shared_to`: the system text's
  head, the system opening), a tail past it - and at the park the finished turn goes in as a tail, its pages on a dense model
  (`pool_export`). The rows come down into a carrier session's pages (`Scheduler.pool_host`, a paged session of the pool that
  holds pages only while a move runs) through `gpu_device_kv_export`, the hydrate rail a run at a time; only the rows past the
  pages the cache already holds of the same tokens are read (`prefix_share_pages`: a checkpoint's whole pages under the longest
  opening it shares, or the page chain's), so a conversation's later stops cost their own rows and the shared system prompt's
  pages are held once. The state is the stream's own, flushed off its region's slots (`dn_snapshot_take`), and enters through
  `prefix_insert_snapshot`. A checkpoint the cache already holds is touched instead (`prefix_state_touch`), so nothing is read.
- **Up.** At admission (`device_attach`) the parked claim sharing the longest opening is weighed against the cache's longest
  (`prefix_state_match_len`, `prefix_match_len`); the device's is taken where it is as long - it costs no copy - and the pool's
  where it is longer: `prefix_attach_state_split` shares the checkpoint's pages into the carrier and restores its state into
  the fresh session's host arrays, and `gpu_device_kv_import` binds a region to the session and takes the rows up, the state
  following at the session's next prefill, which hands its host arrays to the region's slots (`rdec_dn_prefill_entry`). A
  dense model attaches pages the usual way (`prefix_attach`) into the carrier.
- **Budget.** The pool is the host cache, so its bounds are `--prefix-state-mb` and `--prefix-states` with their accounting -
  every snapshot plus each held page once - and their order: tails used longest ago first, openings only when no tail stands
  (`#prefix-recurrent-checkpoints`). A dense model's pages fall under `prefix_groups`.
- **Where it stands aside.** The pool serves a paged scheduler whose pool codec is the mirror's (the rows move as bytes), with
  no draft head (the head's rows ride no export) and no n-gram side input (its ring can sit on the device), on a model whose
  device prefill continues past zero (`device_pool_on`). `DASLLAMA_DEVICE_POOL=0` keeps the card's claims and rows alone - the
  A/B rail. `gpu_device_kv_pool_moves` counts the rows each direction moved and the wall it took, the upload cost's reading.

