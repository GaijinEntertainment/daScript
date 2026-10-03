# dasLLAMA Architecture - serving and prefix-cache charters

Companion to `ARCHITECTURE.md` beside `ARCHITECTURE_ENGINE.md`; a section is cited by its
anchor.

### Serving {#scheduler-step}

- **`dasllama_scheduler.das`** - the continuous-batching scheduler, the serving layer over the
  facade (its one engine require is `dasllama/dasllama`). One synchronous thread: each
  `scheduler_step` admits queued requests, runs one `eval_batch` decode step over every
  decoding stream (a self-speculative scheduler instead ticks every stream's round through
  `mtp_spec_eval_batch`, one joint verify where a driver seats one, and counts the tick as a
  batched step only when every stream's rows rode it), then at most one prefill chunk FCFS - `chunk_tokens` while
  a stream decodes, since the chunk is the stall every decoding stream waits out, and at least `idle_chunk_tokens`
  while none does, since a prefill window's cost is mostly fixed and nothing waits on it; `chunk_defaults` names
  both sizes per serving backend (the chunk 512 where a GPU prefills and 64 on the CPU, whose chunk costs its token count, so the stall follows it down; idle 512,
  2048 on Metal, where four windows in one call run within a few percent of the whole prompt's rate); paged serving donates finished streams'
  KV pages to the prefix cache, device mode parks their regions. Results flow out as `SchedEvent`s - no HTTP here.
  `utils/dasllama-server` owns the writers; `tutorials/dasLLAMA/13_serving.das` is the
  teaching consumer; `tests/test_scheduler.das` gates it against `generate()` references.
  The step clears its gather arrays (`batch_rows`, `batch_toks`, `batch_idx`) before it reaps
  finished streams: `batch_rows` holds borrowed pointers into the sessions the reap deletes,
  and a validating heap collect between steps walks every pointer the array still holds.

### The prefix cache's partial page {#prefix-tail-page}

The page cache shares the whole pages a prompt matches and COPIES the one page after them for the
leading rows the prompt shares with it. `prefix_insert_` enters a donation's pages whole and its last
page partial (keyed by the hash of the history each closes), and links each entry to the page that
followed it in the first donation to continue past it (`PrefixEntry.next`). `prefix_attach_` walks
the whole pages by hash, then follows the last hit's link and counts the rows that page shares with
the prompt, short of the prompt's last token; the taker gets `kv_pool_copy_group` of it and writes
the page's remaining rows itself, the donor's page staying the cache's hold alone. A conversation's
next turn therefore attaches all of the history it repeats, not its whole pages - the served turn's
shape is a reply that filled the page the prompt ends in: with 64-row pages a turn prefilled 57 to 76
tokens through a 128-expert MoE where the reference prefilled the turn's own 19, and that was the
one served row it led (`PERF_LEDGER.md`, the qwen ladder). The link is weak: an evicted page's key
misses and the attach stops at the whole pages; a page reaching nothing cached past it attaches
nothing partial. The link hangs off a matched page, so a prompt that shares less than one whole
page with the cache attaches nothing - the rows at stake are under a page.

### The prefix cache on a recurrent model {#prefix-recurrent-checkpoints}

A deltanet hybrid's K/V pages continue nothing alone: its recurrent state exists at one position only, so `dasllama_prefix.das` caches a CHECKPOINT (`PrefixState`) - the evaled tokens, their page groups, and a `DnSnapshot` (`dn_snapshot_take`: the state brought to the host through `dn_state_to_host`, where a device driver registers its mirror's copy-down; the n-gram ring; the draft head's carry). A prompt attaches the deepest checkpoint whose every token opens it: the pages below the last row's are shared, the last row's page is copied even when whole (the draft head rewrites row n - 1 for the token that follows), the snapshot is restored. Because the position is exact, the scheduler STOPS a prefill there (`prefix_checkpoint_at`): at the caller's stable opening (`PendingReq.stable_at`, the server's `render_turn_marked` - every earlier turn plus the first turn's system block), else at the longest opening an earlier checkpoint shares; a checkpoint within a chunk and a half ends the chunk, so the stop adds no window. A finished turn leaves one more past its close tokens, which the conversation's next turn attaches. `max_states` (4) bounds them, the one used longest ago dropped first.

