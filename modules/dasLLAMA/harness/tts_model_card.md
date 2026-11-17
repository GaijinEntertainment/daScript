---
license: other
license_name: per-file
license_link: https://github.com/GaijinEntertainment/daScript/blob/master/modules/dasLLAMA/THIRD_PARTY_NOTICES.md
language:
  - en
  - ru
  - de
  - es
  - it
  - pt
  - fr
language_bcp47:
  - en-US
  - en-GB
pipeline_tag: text-to-speech
library_name: dasllama
tags:
  - daslang
  - dasllama
  - gguf
  - kittentts
  - kokoro
  - styletts2
  - pocket-tts
  - voice-cloning
---

# dasLLAMA text-to-speech models

The speech models dasLLAMA serves, converted to GGUF, and the front-end packs the phoneme
families load. dasLLAMA is the daslang inference engine
(https://github.com/GaijinEntertainment/daScript, `modules/dasLLAMA`); the models are the
StyleTTS2 lineage - KittenTTS nano and mini, Kokoro-82M, and kokoro-ru, the Russian fine-tune of
Kokoro-82M - behind one facade and one das-native text front end (an English normalizer, a
part-of-speech tagger and a grapheme-to-phoneme pass; a Russian normalizer, a stress pass over
the stress pack and a letter-to-sound pass), and Kyutai's Pocket TTS, a continuous-audio
language model that reads text and clones a voice from a clip. No espeak-ng, no phonemizer: the
front end is data, and the data is in the packs.

## Files

| file | what | bytes | sha256 |
|---|---|---|---|
| `kitten-nano.gguf` | KittenTTS nano 0.8, f32 | 59331456 | `4556948c36a29e4be5ad521e597a20ae817059c404fea3c5d935afa73506d9da` |
| `kitten-nano-q8.gguf` | KittenTTS nano 0.8, the fast form: the weights the Q8_0 lane serves, stored as that lane mints them - no mint at load | 16646016 | `ed1b412ccc10499d2f4bc4fe4f4c78c7308dfb772c311782989f41ed8aee9dbe` |
| `kitten-nano-kq.gguf` | KittenTTS nano 0.8, the small form: the recipe lane's weights stored the same way - Q4_K and Q6_K convs beside the Q8_0 groups | 14785920 | `d92014bdfa7cc7bafb3093f3e755c0fdc76a974e03c363b1b6c42b610860326e` |
| `kitten-mini.gguf` | KittenTTS mini 0.8, f32 | 295975008 | `e127a95c6ffa390eccc5956e7156ff055e9f30078cf16361cbcd0eea6c21fdb5` |
| `kitten-mini-q8.gguf` | KittenTTS mini 0.8, the fast form: the weights the Q8_0 lane serves, stored as that lane mints them - no mint at load | 80495296 | `1e1d094ee91aaf28a33ec4a1617a1acdd7d3a35fba7a681d2d99e022532701ed` |
| `kitten-mini-kq.gguf` | KittenTTS mini 0.8, the small form: the recipe lane's weights stored the same way - Q4_K and Q6_K convs beside the Q8_0 groups | 59725504 | `f90e4f5aaac96cbd887fce4b9eeb3d05bd39a34a9f80c188a32a8902e6475d46` |
| `kokoro-82m.gguf` | Kokoro-82M v1.0 with its 54 voice packs, f32 | 352965024 | `e0d6584b5b650730bd62e7b105a2e9502384bbe3fbebf3030113560355e6232f` |
| `kokoro-82m-q8.gguf` | Kokoro-82M v1.0 with its 54 voice packs, the fast form: the weights the Q8_0 lane serves, stored as that lane mints them - no mint at load | 95792544 | `cb1ade7573957ed00bffe4968f1cc0ddec4222dd28d654636b93b5ea406a6d20` |
| `kokoro-82m-kq.gguf` | Kokoro-82M v1.0 with its 54 voice packs, the small form: the recipe lane's weights stored the same way - Q4_K and Q6_K convs beside the Q8_0 groups | 71391648 | `4b5102ab80b568ed6d9dc23417d7864fb23435b6f744729146a8fd1977b5bc4f` |
| `kokoro-ru.gguf` | kokoro-ru v2, the Russian fine-tune of Kokoro-82M, the base checkpoint with its two voice packs `sveta` and `masha`, f32 | 325804992 | `635a43debfcbff5cf1fd0380a4ffc55dfacff083ae16d057bae6d7e7c9a29c9f` |
| `kokoro-ru-q8.gguf` | kokoro-ru v2 with `sveta` and `masha`, the fast form: the weights the Q8_0 lane serves, stored as that lane mints them - no mint at load | 88575552 | `1961e4ca0cfde51c6e57244e19c45163c3c7e67808d94387cee942849789e5ff` |
| `kokoro-ru-kq.gguf` | kokoro-ru v2 with `sveta` and `masha`, the small form: the recipe lane's weights stored the same way - Q4_K and Q6_K convs beside the Q8_0 groups | 64174656 | `b3a238b377514b11fa778ba7b8003f83c460f3a427a5fea54525c2064d725bdb` |
| `kokoro-ru-dima.gguf` | kokoro-ru v2, the second checkpoint with its one male voice pack `dima`, f32 | 325282688 | `7720e1087c0c9720577e7e0b383dd31f75b7f6ab45bcd88df1f28df3c08c56a6` |
| `kokoro-ru-dima-q8.gguf` | kokoro-ru v2 with `dima`, the fast form: the weights the Q8_0 lane serves, stored as that lane mints them - no mint at load | 88436768 | `17ae7fb6698d66011facffc7e9a3e4b7db84d17542d4b9523967417545fa5d2a` |
| `kokoro-ru-dima-kq.gguf` | kokoro-ru v2 with `dima`, the small form: the recipe lane's weights stored the same way - Q4_K and Q6_K convs beside the Q8_0 groups | 64035872 | `b396a6c2dfe4461b7127eb89aade5007654f39606e31cb56a31fa489870d978c` |
| `tts_g2p_ru.bin` | the Russian stress pack: every inflected form of the OpenRussian dictionary, the forms and readings the English and Russian Wiktionary add and the names Russian Wikipedia marks, each with its stressed vowel (a form with two readings carrying both, the more frequent one first), the ё spellings a text written without ё needs, and the rules that pick a reading from the words beside it - a stress (`з+амок` / `зам+ок`) or a spelling (`все` / `всё`) | 15387767 | `74b8f3c4d3a3e140b7c7937d15da95a37328e7e3347392088514e47e68e5265d` |
| `ru-stress-rubert-base-q4.gguf` | the Russian stress-context model: reads a sentence and picks the reading of each word whose stress (`зам+ок` / `з+амок`) or ё spelling (`все` / `всё`) depends on the context; ruBert-base fine-tuned on a private dataset, Q4_K with a Q8_0 embedding table. Optional: copy it beside a Russian voice as `tts_ctx_ru.gguf` | 120522080 | `28692fe9fbea9cb7b06f6c39e18a78e4c1e30b37173f2040808865a1d7730624` |
| `ru-stress-ruelectra-small-q4.gguf` | the same model in the small form, ruElectra-small fine-tuned on a private dataset; used the same way | 38428800 | `43475b1535fdcd3d142bb04ac845909f2557bee32b524af5e19eb1528bbc1ae3` |
| `tts_g2p.bin` | the grapheme-to-phoneme pack: misaki's gold and silver lexicons in both English dialects (one merged table per tier), CMUdict 0.7a rendered into the American inventory, the g2p_en GRU spelling model | 14011554 | `d7b6afea7a0901a877d10531054d3f967845c58f415f0ea937decd8a3081ccd5` |
| `tts_g2p_en_us.bin` | the same pack with the American tier alone - no British values, CMUdict pruned of every word the American lexicon carries; the web serving set's pack, British voices are not offered on it | 10257455 | `6f69d2e74565bd7d876b8d1f4042bf8c1c5b615387fa26ff45215cf447932154` |
| `tts_postag.bin` | the tokenizer exception table and the averaged-perceptron PTB tagger | 12566510 | `38c2e85f7fef3e57d561d2aa0af25fccda4276376ba1993c3dbc2ae0ebfa57b4` |
| `pocket-tts-en-q8.gguf` | Kyutai Pocket TTS, English (english_2026-04): the served GEMM weights as Q8_0, the rest f16, its unigram tokenizer and 19 voices as latent frames inside; reads text, needs no pack | 136812032 | `ff41c88d0099375e096c1a0bb9db43c10e8b53b8bc9d3874b03f658f94a5487e` |
| `pocket-tts-de-q8.gguf` | Pocket TTS German (6 layers), the same form, one voice (`juergen`) | 136519648 | `1762eb997ab113247a5083d55dfd22bf5ee83d8256016fce13a67ba4f3fb6fd9` |
| `pocket-tts-es-q8.gguf` | Pocket TTS Spanish (6 layers), one voice (`lola`) | 136518688 | `65ab208f0ecc976e752889bd3a1c1f61b51a95515e1159fde8468177ba083128` |
| `pocket-tts-it-q8.gguf` | Pocket TTS Italian (6 layers), one voice (`giovanni`) | 136515072 | `65507c2ff6c87836bca5ea50e66e55a12ecec78390556efae75eb35b4c44f754` |
| `pocket-tts-pt-q8.gguf` | Pocket TTS Portuguese (6 layers), one voice (`rafael`) | 136521024 | `80ae1a859658149843de6701fc4b86868c34925b55490c21429c497d333201b6` |
| `pocket-tts-fr-q8.gguf` | Pocket TTS French (24 layers, the only French model Kyutai ships), one voice (`estelle`) | 377328544 | `edb510a0c441f50817b8aed8cfec961b77bd9ee12691c9db9f91427a966cb738` |
| `pocket-tts-de-kq.gguf` | Pocket TTS German in the small form (the backbone and the codec transformers Q4_K, the flow head Q8_0), one voice (`juergen`) | 77466176 | `29d3088f3db5aa9958b589e60da023b6c89a80a341986eb43cbe7aa29bbd7432` |
| `pocket-tts-es-kq.gguf` | Pocket TTS Spanish in the small form (the backbone and the codec transformers Q4_K, the flow head Q8_0), one voice (`lola`) | 77465216 | `23b3a52cc77d4efaf291f46507a5e0412896f87e1f71fd0d4afa3c4d2e9d0008` |
| `pocket-tts-it-kq.gguf` | Pocket TTS Italian in the small form (the backbone and the codec transformers Q4_K, the flow head Q8_0), one voice (`giovanni`) | 77461600 | `dfe62eccdf774c660a8c11dc7da1778a3e1f986c245f4d37b3bedd4c7f21c309` |
| `pocket-tts-pt-kq.gguf` | Pocket TTS Portuguese in the small form (the backbone and the codec transformers Q4_K, the flow head Q8_0), one voice (`rafael`) | 77467552 | `0c25e851662d09fb782f1eac11caf38e7b00f1f19f307577700c0b8c2014decd` |
| `pocket-tts-fr-kq.gguf` | Pocket TTS French in the small form (the backbone and the codec transformers Q4_K, the flow head Q8_0), one voice (`estelle`) | 205028864 | `f56ac84a47687d517a0a4e1776532096503567d09250b090c50c9a74d0c55cb8` |
| `pocket-tts-en-kq.gguf` | Pocket TTS English in the small form: the backbone and the codec transformers as Q4_K, the flow head as Q8_0, the codec convolutions f16, the embedding table Q4_K; its tokenizer, the codec encoder (so it clones) and the 19 voices as latent frames | 77758560 | `9b549549e44601435ec5df3f48b5ef977cf1449bcf92e6ae49db4b07da20e757` |
| `pocket-tts-en-stuart-kq.gguf` | the same small form with one voice (`stuart_bell`) as latent frames and no codec encoder: reads text in that voice, cannot clone | 67005056 | `e9e69e52254f48e544509a0dff3b594779f737eac5b372d61adda8b696a5af46` |
| `pocket-tts-en-18m-q8.gguf` | Kyutai Pocket TTS English 18.2M (`english_18m_2026-10`: an 8-layer 512-wide backbone, a 128-wide decoder behind the 100M model's encoder), the served GEMMs Q8_0, the rest f16, the 19 voices and the encoder inside | 36712480 | `d5b138be971d7907eb7a568b827510d85775d7ce933fad4dba0245d2d0536fa2` |
| `pocket-tts-en-18m-kq.gguf` | the 18.2M model in the small form: the backbone and the text table Q4_K, the flow head and the decoder transformer Q8_0 | 23547232 | `ed8db0741c377ea21a3603101d9215b538e2c143b1916f459e1b599ce096716b` |
| `pocket-tts-en-18m-stuart-kq.gguf` | the 18.2M small form with one voice (`stuart_bell`) as latent frames and no codec encoder: reads text in that voice, cannot clone | 12793728 | `3678b27db407fa0715feb7c0863c63c1cb00a28f9387fa76f8e4922e30c8f124` |
| `pocket-tts-en-7m-q8.gguf` | Pocket TTS English 6.9M (`english_7m_2026-10`: a 224-wide grouped-query backbone), the served GEMMs Q8_0 | 22991840 | `b3d99bc412613c4323f023572c782b980a375fed83d88239b7e5d68de2621c4c` |
| `pocket-tts-en-2m.gguf` | Pocket TTS English 1.9M (`english_2m_2026-10`: a 112-wide backbone, a 4-layer decoder), every tensor f16 - its widths sit off the quant lattices | 23758400 | `612a583ce4811f8904200295fa9293423074cffe4eadbcc3a5232aec1be5f68d` |
| `pocket-tts-en-18m-refl-q8.gguf` | the 18.2M model after Kyutai's reward fine-tuning (`english_18m_refl_2026-10`), the q8 form | 36712480 | `3460c9cff8102e997369ab3ee7e32e1d020a8f1cffc4a454dc1ecb059f51d4dc` |
| `pocket-tts-en-7m-refl-q8.gguf` | the 6.9M model after reward fine-tuning (`english_7m_refl_2026-10`), the q8 form | 22991840 | `80ca90b4a60716693722ed19a91945d326f4611f8573defe27f722d6f4bd50a2` |
| `pocket-tts-en-2m-refl.gguf` | the 1.9M model after reward fine-tuning (`english_2m_refl_2026-10`), f16 | 23758432 | `54dead7cb04aa87971b32f4a25288d566c95c739290581df91742155fcc61230` |

The packs sit beside whichever GGUF you load; the loader reads them from the model's
directory - `tts_g2p.bin` when it is there, else `tts_g2p_en_us.bin`. The GGUFs carry f32 weights: dasLLAMA quantizes the served layouts to Q8_0 at first
load and keeps the result beside the file as a prepared image, so the f32 file is also the
reference lane.

## Use

```
dasllama-server --tts kitten-nano.gguf          # POST /v1/audio/speech, the OpenAI shape
daslang -jit utils/dasllama-server/cli.das -- speak --tts kitten-nano.gguf --voice expr-voice-2-f -o hello.wav "Hello."
```

Pocket TTS English is the cloning model: 137 MB, 19 voices (`alba` the default, `bill_boerst`,
`caro_davy`, `anna`, `george`, ...), and any voice from a few seconds of 24 kHz audio through
`tts_register_voice`. It reads text, so it needs neither pack; the English normalizer runs in
front of it. On the 200-sentence rig at `alba` this file reads WER 3.36 / UTMOS 4.395 on an
Apple M5 Max (measured 2026-11-16 with the module's `harness/tts_rig.py`, the engine under the
JIT tier on the Metal seats, the kernels untuned - `DASLLAMA_ALLOW_UNTUNED=1`). The five other
languages are the same form, one file each with Kyutai's default clip for that language as its
only voice (German `juergen`, Spanish `lola`, Italian `giovanni`, Portuguese `rafael`, French
`estelle`); the German, Spanish, Italian and Portuguese files are the six-layer models, French
exists only as the 24-layer one, and each has its small form beside it
(`pocket-tts-<code>-kq.gguf`: 77 MB for the six-layer models, 205 MB for French). A voice cloned
from any clip speaks the file's language with the clip's accent. Text in those languages is read
as it is, since the normalizer is English.
`pocket-tts-en-kq.gguf` is the English model in the small form, 78 MB: the backbone and the
codec transformers as Q4_K, the flow head as Q8_0, the codec convolutions f16, the embedding
table Q4_K, the encoder and the 19 voices inside (the rig row at `alba`: WER 3.45 / UTMOS 4.384,
the same box, rig and date); it is the file the browser examples on
dasllama.io fetch. `pocket-tts-en-stuart-kq.gguf` is that form with one voice, `stuart_bell`,
and no codec encoder, 67 MB (WER 3.36 / UTMOS 4.106 at that voice): it reads text in that voice
and cannot clone. The 18.2M model's
twins of these two files - `pocket-tts-en-18m-kq.gguf` (24 MB) and
`pocket-tts-en-18m-stuart-kq.gguf` (13 MB) - are the same forms at half the bytes and twice the
browser speed; the pages keep the 100M model, whose pace reads more naturally.

The 2026-10 small English models are Kyutai's distilled Pocket TTS at 18.2M, 6.9M and 1.9M
parameters (`english_18m_2026-10`, `english_7m_2026-10`, `english_2m_2026-10`; the `-refl`
files are the same models after Kyutai's reward fine-tuning), each with the same 19 voices
and the 100M model's codec encoder inside, so each clones. Every Pocket file here is served the
same way: the voice clip is trimmed to end on a pause, the end-of-speech logit is ignored on a
chunk's first six frames, each chunk fades in over 5 ms, and a trailing comma or dash becomes
a period. On the 200-sentence rig at `alba` on an Apple M5 Max (measured 2026-10-23,
`harness/tts_rig.py`, the JIT tier, the CPU chain - the GPU seats decline these shapes):
`pocket-tts-en-18m-q8.gguf` reads WER 3.09 / UTMOS 4.341, `pocket-tts-en-18m-refl-q8.gguf`
3.04 / 4.369, `pocket-tts-en-18m-kq.gguf` 3.18 / 4.232, `pocket-tts-en-7m-q8.gguf` 3.41 /
4.357, `pocket-tts-en-7m-refl-q8.gguf` 3.86 / 4.377, `pocket-tts-en-2m.gguf` 5.32 / 3.777 and
`pocket-tts-en-2m-refl.gguf` 4.68 / 3.823; the reference package on the same sentences reads
5.41 / 4.364 (18m), 5.59 / 4.378 (7m) and 9.18 / 3.817 (2m) - the WER gap is the English
normalizer in front of our files (the reference reads "1995" as digits), the UTMOS gap the q8
lane's. On that box, served
through `dasllama-server` on the Metal seats (one sentence, eight requests), the 100M q8 file
answers in 50 ms for 4.6 s of speech (a real-time factor of 0.011), the 18m files in 28-30 ms
for 4.4-4.6 s (0.006), the 7m in 28 ms for 5.0 s (0.006) and the 2m in 25 ms for 4.7 s
(0.005) - every stage of every file on the GPU seats. The reference package on the same box and
sentence, one CPU thread as it pins, takes 325 ms on the 100M and 156 ms on the 18m. Every figure of
this paragraph is the Pocket small-model leg's entry of `PERF_LEDGER.md`, with its instruments and
commands.
In the browser (the parrot page's wasm64 build under Chrome on the same box, seven workers)
`pocket-tts-en-18m-kq.gguf` runs at 0.079 against the 100M kq file's 0.161 - twice as fast,
twelve times faster than real time - and clones a 6.5 s voice in 0.47 s against 0.87.

Kitten nano is the phoneme families' served default: 59 MB, eight voices, a real-time factor of 0.03 on an Apple
M1 Max (measured 2026-09-02 with the same rig). Its voices are `expr-voice-2-m` through
`expr-voice-5-f`, and the upstream's alias names
(`Bella`, `Jasper`, ...) are accepted for them. Kokoro ships 54 packs, of which the front end
drives the 28 English ones: the 20 American (`af_*`, `am_*` - `af_heart`, `am_adam`, ...) and
the 8 British (`bf_*`, `bm_*` - `bf_emma`, `bm_george`, ...), each phonemized in its own
dialect. The other 26 packs speak languages the front end does not phonemize yet, so they are
not listed and are refused. kokoro-ru carries the Russian voices - `sveta` and `masha` in
`kokoro-ru.gguf`, `dima` in `kokoro-ru-dima.gguf` - and needs `tts_g2p_ru.bin` beside it; with
the English packs beside it too, a Latin word inside Russian text is read in English. Russian
stress comes from the pack, or from the text itself: `+` before the stressed vowel (`зам+ок`) or
U+0301 after it, which always wins, so a homograph the pack picks the other way can be fixed
in the text. With `tts_ctx_ru.gguf` beside the voice - a copy of either `ru-stress-*-q4.gguf` -
a word with two readings is stressed by its sentence; without it the pack's rules and its more
frequent reading decide. A text written without ё gets the letter back: always where the е
spelling is no word (`еще`, `объем`), and where both spellings are words (`все` / `всё`) by the
sentence with `tts_ctx_ru.gguf`, by the pack's rules without it. `dasllama-server --tts <directory holding the packs>` serves the front
end with no voice: `POST /v1/audio/phonemes` answers the text with `+` before each stressed
vowel and its phonemes, for a synthesis that runs elsewhere. Some Kokoro packs, the British ones most audibly, render a comma or
a full stop as a breath; the reference pipeline does the same, so it is the pack, not the
engine. Every model speaks at 24 kHz. `GET /v1/stats` on the server lists the served model's
voices.

Each of the five phoneme-family files also ships in two forms that load with no mint - the engine
reads the quantized weights as the file stores them. `<name>-q8.gguf` is the fast form: the weights
the default Q8_0 lane serves. `<name>-kq.gguf` is the small form: the decoder, the generator and the
text encoder's convolutions as Q4_K and the upsamplers as Q6_K beside the Q8_0 groups. Kokoro-82M is
96 MB and 71 MB against the f32 file's 353; kokoro-ru 89 and 64 against 326; kokoro-ru-dima 88 and
64 against 325; Kitten mini 80 and 60 against 296; Kitten nano 17 and 15 against 59. On the
200-sentence rigs (measured 2026-11-16 on an Apple M5 Max with `harness/tts_rig.py`, the JIT tier,
the engine's CPU chain - `DASLLAMA_METAL_TOWER=0` - with the kernels untuned
(`DASLLAMA_ALLOW_UNTUNED=1`), the Russian set under `--lang ru`) the files read, fast then small,
WER % / UTMOS:
kokoro-ru `sveta` 0.70 / 3.835 and 0.70 / 3.838, `masha` 0.65 / 3.715 and 0.65 / 3.714,
kokoro-ru-dima `dima` 0.70 / 3.739 and 0.74 / 3.743, Kokoro-82M `af_heart` 2.82 / 4.500 on both,
Kitten nano `Bella` 3.13 / 3.986 and 3.41 / 3.956, Kitten mini `Bella` 2.86 / 4.341 and 3.13 /
4.323. The f32 files on the same lanes, minted at load (the same rig and box the same day, the
kernels tuned), read 0.74 / 3.844 and 0.65 / 3.846
(`sveta`), 0.65 / 3.716 on both (`masha`), 0.74 / 3.740 and 0.78 / 3.743 (`dima`), 2.68 / 4.500 on
both (Kokoro-82M), 3.41 / 3.966 and 2.91 / 3.941 (Kitten nano), 2.86 / 4.339 and 3.00 / 4.325
(Kitten mini): one word of either set is 0.04 points, and no form leads. The f32 files stay the
reference - they serve the same lanes after a mint at first load, and take a lane pin, which a
file that stores its lane does not.

## Provenance

- `kitten-nano.gguf` / `kitten-mini.gguf`: converted by `modules/dasLLAMA/harness/convert_kitten.py`
  from `KittenML/kitten-tts-nano-0.8` at `7a1db645b1f3ab9420761d87428e042b9cec3f26` and
  `KittenML/kitten-tts-mini-0.8` at `c02725660cea441db4c383af69f1f26f5cd00947` (the ONNX weights,
  the voice table, the config).
- `kokoro-82m.gguf`: converted by `modules/dasLLAMA/harness/convert_kokoro.py` from
  `hexgrad/Kokoro-82M` at `f3ff3571791e39611d31c381e3a41a3af07b4987` (`kokoro-v1_0.pth` and the
  voice packs).
- `kokoro-ru.gguf` / `kokoro-ru-dima.gguf`: converted by `modules/dasLLAMA/harness/convert_kokoro.py`
  (`--checkpoint`, `--config`, `--voices`, `--lang ru`) from `zaakirio/kokoro-ru` at
  `d649c57b239b18c4c384378127cbf01dba039bc1` (`kokoro-ru-v2-base.pth` with `voices/sveta.bin` and
  `voices/masha.bin`; `kokoro-ru-v2-dima.pth` with `voices/dima.bin`).
- `<name>-q8.gguf` / `<name>-kq.gguf` of the five files above: written from the f32 file of the same name
  by `utils/dasllama-convert --model <name>.gguf --gguf <name>-q8.gguf` (and `--recipe` for `-kq`) - the
  same weights, each quantized group stored as the engine's Q8_0 or recipe lane mints it.
- `tts_g2p_ru.bin`: `modules/dasLLAMA/harness/build_g2p_ru_data.py` over the OpenRussian dictionary
  data (`Badestrand/russian-dictionary`: `nouns.csv`, `adjectives.csv`, `verbs.csv`, `others.csv`),
  the forms and readings of the English and Russian Wiktionary (the machine-readable extracts of
  kaikki.org) and the stress Russian Wikipedia's lead sentences mark, the pronoun inflections
  the dictionary leaves out added by the builder, the homograph defaults ordered by a frequency
  table.
- `ru-stress-rubert-base-q4.gguf` / `ru-stress-ruelectra-small-q4.gguf`: converted by
  `modules/dasLLAMA/harness/convert_ru_stress.py` from `ai-forever/ruBert-base` and
  `ai-forever/ruElectra-small`, each fine-tuned on a private dataset; the fine-tuned checkpoints
  are not distributed.
- `tts_g2p.bin`: `modules/dasLLAMA/harness/build_g2p_data.py` over misaki 0.9.4 (`us_gold.json`,
  `us_silver.json`, `gb_gold.json`, `gb_silver.json`), CMUdict 0.7a (the NLTK distribution) and
  g2p_en 2.1.0 (`checkpoint20.npz`), plus `harness/g2p_local_additions.json`.
- `tts_postag.bin`: `modules/dasLLAMA/harness/train_postag.py` over UD English-EWT and Project
  Gutenberg prose tagged by spaCy's `en_core_web_sm`; the tokenizer exception table is spaCy's.
- `pocket-tts-en-q8.gguf`: converted by `modules/dasLLAMA/harness/convert_pocket.py --q8` under
  pocket-tts 4.0.1 from the files its `english_2026-04` config pins: `kyutai/pocket-tts`
  `languages/english_2026-04/model.safetensors` at `19f95fe2df36e79fbd9f10008595cc4c977a0fcc` and
  `kyutai/pocket-tts-without-voice-cloning` `languages/english_2026-04/tokenizer.json` at
  `00eac05ed3d16bdc3f6b5d598874019c34a89214`; the
  voice clips from `kyutai/tts-voices` at `323332d33f997de8394f24a193e1a76df720e01a`
  (`voice-zero/`, `voice-donations/`, `vctk/`, `alba-mackenna/casual.wav`), each trimmed to end
  on a pause and stored as the codec encoder's latent frames.
- `pocket-tts-en-kq.gguf` / `pocket-tts-en-stuart-kq.gguf`, and the 18.2M model's
  `pocket-tts-en-18m-kq.gguf` / `pocket-tts-en-18m-stuart-kq.gguf`: the same sources through
  `modules/dasLLAMA/harness/convert_pocket.py --kq` (the one-voice files with `--voices stuart_bell
  --no-cloning`); the K-quant blocks are ggml's own quantizer, and each voice is stored as the
  latent frames of its clip through the model's codec encoder.
- `pocket-tts-<code>-q8.gguf` and `pocket-tts-<code>-kq.gguf` for German, Spanish, Italian,
  Portuguese and French: the same converter (`--q8`, `--kq`) under pocket-tts 4.0.1 from the files
  each language's config pins (`german`, `spanish`, `italian`, `portuguese`, `french_24l`):
  `kyutai/pocket-tts` `languages/<config>/model.safetensors` at
  `3e82814a68665eec246ff649b14c71331f955c06` and `kyutai/pocket-tts-without-voice-cloning`
  `languages/<config>/tokenizer.json` at `4e1e0a3e611c51c0b4ed8174fc10f32a54644303` (French at
  `8843db76457a91db32077edf8dfcd1c0e3e755fd`); the language's default clip from `kyutai/pocket-tts`
  (`estelle` from `kyutai/tts-voices`), stored the same way.
- `pocket-tts-en-18m-q8.gguf`, `pocket-tts-en-7m-q8.gguf`, `pocket-tts-en-18m-refl-q8.gguf`,
  `pocket-tts-en-7m-refl-q8.gguf` (`--q8`), `pocket-tts-en-2m.gguf`, `pocket-tts-en-2m-refl.gguf`
  (f16) and `pocket-tts-en-18m-kq.gguf` (`--kq`): converted by `modules/dasLLAMA/harness/convert_pocket.py`
  under pocket-tts 4.0.1 from `kyutai/pocket-tts` `languages/<config>/model.safetensors` at
  `db04f0e653473e5c7333c69686ac92b7460b0c1d` (the configs `english_18m_2026-10`,
  `english_7m_2026-10`, `english_2m_2026-10` and their `_refl_` twins) and each config's
  `tokenizer.model` of `kyutai/pocket-tts-without-voice-cloning` at
  `08ffc58775e62aad9409511b9591c9af626270f3`; the
  same 19 clips, each trimmed to end on a pause as the package's prompt path trims it, then
  through the codec encoder (the 100M model's, which these bundles carry unchanged).

The whole set is rebuilt by `modules/dasLLAMA/performance/build_tts_data.das`. Parity against
the reference implementations (block by block, and the front end sentence by sentence on a
200-sentence corpus) is the test suite under `modules/dasLLAMA/tests/test_tts_*.das`.

## Licences, per file

| file | licence | terms |
|---|---|---|
| `kitten-nano.gguf`, `kitten-mini.gguf` and their `-q8` / `-kq` forms | Apache-2.0 | KittenML's weights, converted; `LICENSE.APACHE-2.0`; the architecture is StyleTTS2 (MIT, `LICENSE.STYLETTS2`) |
| `kokoro-82m.gguf` and its `-q8` / `-kq` forms | Apache-2.0 | hexgrad's weights and voices, converted; the architecture is StyleTTS2 (MIT, `LICENSE.STYLETTS2`) |
| `kokoro-ru.gguf`, `kokoro-ru-dima.gguf` and their `-q8` / `-kq` forms | OpenRAIL | the kokoro-ru weights and voices, converted - the source repository tags them `openrail` and carries no licence text of its own; the voices come from the Dialogs corpus, which its card calls OpenRAIL; the architecture is StyleTTS2 (MIT, `LICENSE.STYLETTS2`) |
| `tts_g2p_ru.bin` | CC BY-SA 4.0 | the OpenRussian dictionary data (openrussian.org), the English and Russian Wiktionary and Russian Wikipedia (`LICENSE.UD_EWT` carries the licence text); `tts_g2p_ru.bin.LICENSE` names each source |
| `ru-stress-rubert-base-q4.gguf` | Apache-2.0 | fine-tuned from ai-forever's ruBert-base, converted (`LICENSE.APACHE-2.0`) |
| `ru-stress-ruelectra-small-q4.gguf` | MIT | fine-tuned from ai-forever's ruElectra-small, converted; MIT as its model card states |
| `tts_g2p.bin` | Apache-2.0 and BSD-2-Clause | misaki and g2p_en (Apache-2.0), CMUdict (`LICENSE.CMUDICT`, Carnegie Mellon University) |
| `tts_postag.bin` | CC BY-SA 4.0 | the tagger weights are trained on UD English-EWT (`LICENSE.UD_EWT`); the exception table and the silver tags come from spaCy (MIT, `LICENSE.SPACY`); Gutenberg prose is public domain |
| `pocket-tts-*.gguf` (every Pocket file, the `-kq` ones included) | CC BY 4.0 | Kyutai's weights and tokenizer, converted (`LICENSE.CC-BY-4.0`); the reference implementation is MIT (`LICENSE.POCKET_TTS`) and not included; the voice clips: `voice-zero` and `voice-donations` CC0, VCTK (CSTR, University of Edinburgh) and Alba Mackenna CC BY 4.0 - the sidecar lists each |

Each `.LICENSE` sidecar beside a file names its sources; the full texts are in this repository.
The engine that reads these files is under the daslang licence in its own repository.
