# Silero-VAD (v6, 16 kHz) - implementation spec

Native daslang reimplementation of Silero-VAD. Oracle = onnxruntime running the **official**
`silero_vad_16k_op15.onnx` (silero-vad 6.2.1, MIT). Weights source = the sibling
`silero_vad_16k.safetensors` (15 named fp32 tensors). Both ship inside the `silero-vad` pip
package (`silero_vad/data/`).

**Proven before any daslang was written** (scratch `validate2.py`): op15-flat driven with the
hand framing below reproduces the official `OnnxWrapper` (`utils_vad.py`) **bit-exactly**
(max|Delta| = 0.0 across all 343 jfk.wav windows). So onnxruntime-op15 per-chunk prob IS the gate,
and the framing here is not an approximation.

## I/O contract & state

- Input: mono 16 kHz f32, consumed in **512-sample chunks** (32 ms).
- Streaming state carried across chunks: **`context[64]`** (last 64 samples of the previous
  chunk; zeros before the first) + LSTM **`h[128]`, `c[128]`** (zeros at reset).
- Output: one speech probability  in  [0,1] per 512-chunk.
- (The official onnx packs LSTM state as `state[2,1,128]` = `[h;c]` and takes an `sr` input;
  the flat compute is identical - we keep h/c separate.)

## Per-chunk forward (seq_len = 1)

1. **Frame**: `win576 = context[64] ++ chunk[512]`.
2. **Reflect pad -> 640**: left=0, right=64. `padded[0..575]=win576`; `padded[576+k]=win576[574-k]`
   for k=0..63 (numpy 'reflect', edge excluded). MEASURED, gate-critical.
3. **STFT** (`stft_conv.weight [258,1,256]` -> matrix `[258,256]`): 4 hops, hop t in {0..3} covers
   `padded[t*128 : t*128+256]`. `stft[c,t] = Sum_j basis[c,j]*padded[t*128+j]`, c=0..257.
   The basis already bakes in the analysis window - no separate window multiply.
   -> `matmul(basis[258x256], frame[256]) = [258]` per hop.
4. **Magnitude** -> `mag[129,4]`: `re=stft[0..128]`, `im=stft[129..257]`; `mag[f,t]=sqrt(re^2+im^2)`.
5. **Encoder** - 4x conv1d, kernel 3, pad 1 (SAME), + ReLU. `pytorch [cout,cin,3]`:
   - `conv1 [128,129,3]` s1 -> `[128,4]`
   - `conv2 [64,128,3]`  s2 -> `[64,2]`
   - `conv3 [64,64,3]`   s2 -> `[64,1]`
   - `conv4 [128,64,3]`  s1 -> `[128,1]`
   -> feature vector `f[128]`. (conv, stride-2: `out[j]=Sum_k w[k]*in_pad[j*2+k]`, in_pad has one
   leading zero from pad-left.)
6. **LSTM cell** (pytorch `LSTMCell`, hidden 128, gate order **i,f,g,o**):
   `g = weight_ih[512,128]*f + weight_hh[512,128]*h + bias`   (bias = bias_ih+bias_hh, pre-folded
   in the converter). Split into i,f,g,o (128 each):
   `i=sigma(gi); f=sigma(gf); g=tanh(gg); o=sigma(go); c'=f*c + i*g; h'=o*tanh(c')`.
7. **Decoder**: `relu(h')` -> `final_conv.weight[1,128,1]->[128]`:
   `p = sigmoid( Sum final_conv[k]*relu_h'[k] + final_conv.bias )`.

Carry forward: `context <- chunk[512][-64:]`, `h <- h'`, `c <- c'`.

## Weights (from `silero_vad_16k_op15.onnx` initializers, all fp32)

**Source is the op15 graph's initializers, NOT the sibling `silero_vad_16k.safetensors`** - the
safetensors holds the *non-reparameterized* training weights (only `stft` matches op15; conv/
final differ by 4-18). The op15 `reparam_conv` weights (+ the LSTM, which lives in an `If`
subgraph) are the deployed ones. onnx names -> short bin names:
`model.stft.forward_basis_buffer`->`stft_conv`, `model.encoder.{0..3}.reparam_conv.{weight,bias}`
->`conv{1..4}.{w,b}`, `model.decoder.rnn.{weight,bias}_{ih,hh}`->`lstm.{w_ih,w_hh}`+folded `lstm.b`,
`model.decoder.decoder.2.{weight,bias}`->`final.{w,b}`.


| name | shape | role |
|---|---|---|
| `stft_conv.weight` | `[258,1,256]` | STFT basis (-> `[258,256]`) |
| `conv1.weight/bias` | `[128,129,3]`/`[128]` | encoder 0 (s1) |
| `conv2.weight/bias` | `[64,128,3]`/`[64]` | encoder 1 (s2) |
| `conv3.weight/bias` | `[64,64,3]`/`[64]` | encoder 2 (s2) |
| `conv4.weight/bias` | `[128,64,3]`/`[128]` | encoder 3 (s1) |
| `lstm_cell.weight_ih` | `[512,128]` | LSTM input weights (i,f,g,o) |
| `lstm_cell.weight_hh` | `[512,128]` | LSTM recurrent weights |
| `lstm_cell.bias_ih` + `bias_hh` | `[512]`x2 | folded -> one `[512]` bias |
| `final_conv.weight/bias` | `[1,128,1]`/`[1]` | decoder projection (-> `[128]`) |

## Container (`silero_vad.bin`, parakeet-style, `harness/convert_silero_vad.py`)

magic `0x67676d6c`; i32 header `[version, sr=16000, n_fft=256, hop=128, context=64,
win=576, pad_right=64, stft_ch=258, n_mag=129, lstm_hidden=128]`; then tensor records
(`i32 n_dims, i32 name_len, i32 ttype=0(F32), i32 dims[n_dims] (pytorch order), name bytes,
raw f32, unpadded`). daslang `sv_read(name, dst, n)` looks up by name + flat-reads (dims are
informational - mirrors parakeet `pk_read`). Converter folds `bias_ih+bias_hh` and flattens
`stft_conv`/`final_conv` to matrix/vector.

## Segmenter (`get_speech_timestamps`, `utils_vad.py` - transcribe exactly)

Per-chunk probs -> hysteresis. Defaults (16 kHz): `threshold 0.5`, `neg_threshold
max(0.5-0.15,0.01)=0.35`, `min_speech 4000`, `min_silence 1600`, `speech_pad 480` samples,
window 512. With default `max_speech_duration_s=inf` the max-speech branch is dormant -> v1
implements the plain onset/offset+padding machine:
- `prob>=thr & !triggered` -> start = i*512, triggered=true.
- `prob<neg & triggered` -> temp_end=i*512 (first time); if `i*512-temp_end >= min_silence`
  close: end=temp_end, emit if `end-start>min_speech`, reset.
- `prob>=thr & temp_end` -> temp_end=0 (dip recovered).
- tail: if open and `audio_len-start>min_speech` -> end=audio_len.
- padding pass: pad each segment +/-480, splitting shared silence between neighbours
  (`utils_vad.py:428-441`).

## Gate

`harness/silero_parity_probe.das` dumps `PROB<TAB>frame<TAB>prob`; `harness/silero_vad_oracle.py`
dumps the same from op15. Diff on jfk / gb1 / hp0 corpus wavs, fp32, target max|Delta| <= 1e-4.
Stage tensors (`stft`, `mag`, encoder relus, `h/c`) are exposable from op15 (`stage_probe.py`)
for stage-wise bisection if the final prob diverges.
