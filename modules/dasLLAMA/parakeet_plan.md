# Parakeet-TDT 0.6b-v2 - implementation spec (session D)

Oracle = whisper.cpp's in-tree `parakeet-cli` (built; `-ng -ps` dumps per-token
id/frame/dur_idx/dur_val/p/plog/t0/t1 - the token-for-token gate format). Model =
`~/Work/whisper.cpp/models/ggml-parakeet-tdt-0.6b-v2-f32.bin` (self-converted from the
.nemo with `--use-f32` so the oracle computes fp32). Reference source:
`~/Work/whisper.cpp/src/parakeet.cpp` (line refs below are that file).

## Container (parakeet_model_load, :983)

magic 0x67676d6c, then 15 i32 hparams IN ORDER: n_vocab(1024) n_audio_ctx n_audio_state(1024)
n_audio_head(8) n_audio_layer(24) n_mels(128) ftype n_fft(512) subsampling_factor(8)
n_subsampling_channels(256) n_conv_kernel(9) n_pred_dim(640) n_pred_layers(2)
n_tdt_durations(5) n_max_tokens(10). Then: filters (i32 n_mel, i32 n_fb, f32 data
[n_mel][n_fb]) * window (i32 n, f32 data - the STFT window comes FROM THE FILE) *
tdt_durations (u32 x n_tdt_durations = [0,1,2,3,4]) * vocab (i32 n, then per token u32 len +
bytes; SPM pieces with _ prefix) * tensors (whisper-bin style records: n_dims, name_len,
ttype, dims, name, UNPADDED raw data - converter REVERSES dims to ggml order). Blank token
id = n_vocab (1024). Joint output = n_vocab+1+n_durations = 1030.

## Mel (log_mel_spectrogram :2747 - FRAME-major `data[i*n_mel+j]`!)

1. preemph 0.97 BACKWARD in-place: for i=n-1..1: x[i] -= 0.97*x[i-1].
2. center CONSTANT (zero) pad n_fft/2=256 both sides.
3. frames: n_len = (padded - 512)/160 + 1 (frame_step 160 - confirm from pcm_to_mel :3212);
   window from the file (size may be < 512 -> centered with window_pad_left, zeros around);
   window applied as w[j]*samples[offset + pad_left + j].
4. FFT 512 (pow2) -> power (no magnitude), mel dot = float4-group/double-acc (whisper shape),
   NATURAL log(sum + eps), eps ADDED not floored, eps = 2^-^2^4.
5. per-feature normalize: mean/std over the first n_samples/160 VALID frames only (Bessel
   n-1), std+1e-5, applied to ALL frames.

## Conv subsampling (build_graph_encode :1498; mel input ne0=freq=128, ne1=time)

conv0 regular conv2d 1->256ch k3 s2 p1 -> ReLU -> conv2 DEPTHWISE k3 s2 p1 -> conv3 regular
1x1 s1 p0 -> ReLU -> conv5 DEPTHWISE k3 s2 p1 -> conv6 regular 1x1 s1 p0 -> ReLU.
freq 128->64->32->16, time T->T/2->T/4->T/8 (ceil at odd). Then permute [freq,time,chan] ->
[freq,chan,time], reshape rows of freq*chan = 16*256 = 4096, pre_out linear 4096->1024 + b.
Feature order per row: freq fastest then chan (f + 16*c ordering after the permute - ne0
was freq). VERIFY vs a stage probe.

## Rel-pos attention (global path :1771 - local_attn only past 8192 enc frames ~ 10.9 min;
v1 = global only, panic beyond with a clear message)

window W = 2*n_time - 1. Runtime inputs (fill code in parakeet_encode_internal :1929-2057 -
COPY EXACTLY): pos_freqs [d_half=512], rel_positions [W], attn_mask [n_time or W x n_time].
pos_emb[W]: theta = freqs*rel_pos; row = INTERLEAVED (sin0,cos0,sin1,cos1,...) via the
[1,d_half](+)[1,d_half] dim0-concat + reshape (ne0=2 fastest).
Per layer: pos = attn_pos_w * pos_emb -> per head [d_head, W].
content[q,k] = (Q+bias_u)[q]*K[k]; rel[q,w] = (Q+bias_v)[q]*pos[w] then the SHIFT
(pad+roll+reshape+view :1793-1824) aligns w -> key index; scalar equivalent: derive mapping
from the rel_positions fill (rel[q][k] uses w = (n_time-1) - q + k when rel_positions =
[T-1 ... -(T-1)] - VERIFY against the fill). scores = (content + rel)*1/sqrtd_head + mask;
softmax; out = P*V; attn_out_w; +residual. NO qkv biases (only u/v).

## Conformer block order (per layer, fc_factor=0.5)

FFN1: ln(+w,b) -> lin1 -> silu -> lin2 -> x0.5 -> +res
ATTN: ln -> (above) -> +res
CONV: ln -> pw1 1024->2048 -> GLU (first*sigma(second)) -> depthwise conv1d k9 pad4 (ssm_conv) ->
      BN inference: (x - mean)/sqrt(var) * w + b - NO eps visible: CHECK the converter
      (likely folds BN eps into running_var at conversion) -> silu -> pw2 -> +res
FFN2: same as FFN1 (x0.5) -> +res
then ln_out (per-layer post norm). Final output [1024, T/8] -> enc_out.

## Predictor / joint / TDT (:2099-2597 - TO READ)

LSTM 2 layers d=640, embed_w for token; ih_w/hh_w/b_h (ih bias folded into hh at
conversion). Joint: enc_w 1024->640 + b, pred_w 640->640 + b, combine -> relu? -> net_w
640->1030 + b. TDT greedy: argmax over 1025 token logits + argmax over 5 duration logits;
blank=1024; frame += dur (dur 0 -> same frame, cap n_max_tokens=10 per frame). EXACT loop
from parakeet_decode :2451.

## dasLLAMA plan

- `dasllama_parakeet.das`: bin loader (whisper-bin record style, own header), mel flavor,
  conv subsampling (reuse conv2d_s2-style im2col + a depthwise variant), conformer blocks
  (gemm_f32/matmul_batch + layernorm_batch - NOT the AudioTower loop: different block
  anatomy), LSTM step, joint, TDT greedy, SPM detok (_->space; is_first_piece rule :566).
- ASR route: load_asr_model(path) ggml-bin vocab<32768 -> parakeet. caps: timestamps word
  (per-token frames x80 ms), languages ["en"], translate false, streaming false (v1),
  prompt false. Segments: one per word? oracle emits tokens w/ word_start flag +
  refine_timestamps_tdt :2402 - v1 emit ONE segment per utterance + per-token data kept?
  TranscribeSegment carries tokens; timestamps t0/t1 = utterance bounds; finer granularity
  once the API grows word-level (caps already names it).
- Gates: token-for-token id/frame/duration sequence vs `parakeet-cli -ng -ps` on jfk.wav,
  then tests/parakeet-expected-gb1-output.txt text as fixture 2.

## Confirmed details (from encode_internal :1929-2055, converter, joint :2220-2368, dims log)

- hop = 160, sample rate 16k, n_fb = 257; the STFT window comes from the file (size may be
  < 512 -> centered, pad_left = (512-win)/2, zeros outside).
- Runtime fills: pos_freqs[k] = exp(-2k*ln(10000)/n_state), k < d_half=512.
  rel_positions[t] = (n_time-1) - t (global path) -> pos_emb column w <-> relative distance
  (T-1-w). Scalar rel gather: rel[q][k] = relmat[q][ (T-1) - q + k ].
  attn_mask[q][k] = k >= n_tokens_real ? -1e30 : 0, n_tokens_real = ceil(n_len_org/8).
- pos_emb row = INTERLEAVED (sin0, cos0, sin1, cos1, ...), theta = freqs[k]*rel_pos.
- Dims (0.6b-v2): FFN 1024->4096->1024 both halves x0.5; conv pw1 1024->2048 (GLU->1024), dw
  [9,1024], pw2 1024->1024; attn q/k/v/out/pos all 1024->1024, pos_bias u/v = [128, 8]
  (d_head=128, ggml file dims); pre convs: conv0 [3,3,1,256] regular in=1ch,
  conv2/conv5 [3,3,1,256] DEPTHWISE (per-channel 3x3), conv3/conv6 [1,1,256,256] pointwise;
  relu after conv0/conv3/conv6 only; pre_out 4096->1024+b. BN: divide by sqrt(running_var)
  with NO eps (mirrors the oracle; converter stores var raw).
- LSTM: converter reorders PyTorch [i,f,g,o] -> [i,f,o,g] and folds bias_ih into bias_hh
  (written as bias_h). Gates = ih_w*x + hh_w*h + b; sigma on i/f/o, tanh on g;
  c' = f*c + i*g; h' = o*tanh(c'). 2 layers d=640; embed rows = n_vocab+1 (blank has one).
- Joint: enc_w*enc[t]+b (precompute for all t) + pred (pred_w*lstm_h+b, per predict) ->
  relu -> net_w -> +b -> SOFTMAX OVER THE WHOLE 1030 ROW -> log. pstate.logits = the LOG-PROBS
  (graph's last node), so the -ps dump's plog = log_probs[tok] (normalizer includes the
  duration slots), p = exp(plog)/Sum exp(log_probs[0..1024]).
- TDT greedy (:2451): predict(blank) once to init LSTM; loop t<n_frames: joint(t); argmax
  tokens[0..1025), argmax durations[1025..1030) -> dur = tdt_durations[idx] ([0..4]);
  blank -> dur=max(dur,1), t+=dur, emitted=0, NO predict; else emit (frame=t,
  t0/t1 = frame*8 / (frame+dur)*8 in mel frames -> x10 ms), predict(token), dur>0 ->
  t+=dur, emitted=0; dur==0 -> emitted++, >=10 -> t+=1, emitted=0.
- SPM detok (:550-590): pieces with _ prefix; leading _ of the FIRST piece is dropped,
  others -> space (read sentencepiece_piece_to_text exactly at impl time).

## Findings

- **GATES GREEN: jfk.wav 33/33 and gb1 786/786 token-for-token** (ids, frames, dur_idx,
  dur_val) vs `parakeet-cli -ng -ps`, first try on jfk; gb1 needed the underflow fix below.
- **The duration argmax MUST mirror the oracle's fp32 softmax underflow.** The oracle
  argmaxes durations over log(softmax(full row)); duration relative mass sits ~e^-^1^1^0, so
  exp(logit - max) flushes to 0.0f wherever it drops under the fp32 denormal floor
  (~e^-^1^0^3), log() gives -inf, and the strict-> argmax over five -infs DEFAULTS TO INDEX 0.
  An analytic logit-space argmax picks the TRUE winner and diverged at 16/786 gb1 steps
  (always oracle=0 vs ours>0 - one-directional, the tell). Fix: argmax over
  float(exp(logit - row_max)) - denormals order correctly where they survive, ties at 0.0
  fall to index 0. Diagnosed with a PARAKEET_TRACE_DUR env-gated fprintf patched into the
  local whisper.cpp parakeet_decode (kept in the checkout, env-gated, harmless).
- The checked-in tests/parakeet-expected-gb1-output.txt does NOT match the v2 model's live
  output (the whisper.cpp test defaults to a v3 bin) - gate against the LIVE parakeet-cli
  run, not the fixture file.
- gb1.ogg fetched from the wikimedia URL in whisper.cpp's Makefile; converted to
  samples/gb1.wav (ffmpeg 16k mono s16).
- Deep AsrModel struct returns need `options stack = 65536` in user programs (same as
  session C's finding - transcribe.das carries it).
