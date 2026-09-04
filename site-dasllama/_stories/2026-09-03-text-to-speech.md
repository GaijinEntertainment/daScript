---
date: 2026-09-03
tag: engine
title: The engine learned to talk.
lede: Three speech models, a text front end that is nothing but data, and an afternoon spent hunting a hiss that turned out to be a comma.
---

<!-- figures: plans/dasllama-tts.md receipts, all on our M1 Max (64 GB, 8 worker lanes), das at the
     merge b124bef01 (2026-09-03) unless named: the speed ladder and the final speed receipt are dated
     2026-09-02 (one two-sentence input, warm: nano 383 ms for 15 s of audio, RTF 0.026 = 38x real time;
     kokoro 651-671 ms for 9.1 s, RTF 0.071-0.073 = 14x), the references were taken the same day on the
     same box at 8 threads, best of 5 (onnxruntime 1.29 on the KittenTTS nano ONNX, RTF 0.035; kokoro
     0.9.4 on torch 2.13 CPU, RTF 0.097; the community ONNX export of Kokoro on onnxruntime, RTF 0.138);
     the quality receipt is the rig's final run, 2026-09-02, q8 lane, 200 sentences, WER by
     parakeet-tdt-0.6b-v2 on the synthesized audio and UTMOS22-strong, the reference arms' own audio
     re-scored on the same forms and scorer; the front-end parity counts are the test suite's own lines
     at das 850f0ba8a (2026-09-03): 171 of 200 American sentences, 149 of 157 British, 32 of 38
     heteronyms against the reference front end's 24 (the American count reads 170 once the
     fallback consults the lexicon before CMUdict, 2026-09-04: "PyTorch" moved from the
     reference's spelling to the local addition's, the other rows unchanged); the pack sizes are the pack builder's own print at
     850f0ba8a (v1 12,719,239 bytes, v2 14,011,554; the tagger pack 12,566,510); the phoneme strings for
     Dr. Chen, the two Polish names and the hunt are the served front end's own answers on 2026-09-03 at
     das 2efa6dde1 (POST /v1/audio/phonemes, and the tutorial's live run for Dr. Chen); the hunt compared
     our waveform with the reference pipeline's for the same line and voice in 20 ms frames, RMS and the
     share of energy above 4 kHz, at das c08ae9bcf; the arc opened 2026-09-01 (42595f347) and merged
     2026-09-03 (PR #3929); the models are kitten-nano 0.8, kitten-mini 0.8 and Kokoro-82M v1.0 converted
     to GGUF; a re-run on 2026-09-03 at das 2efa6dde1 with the serving server and a browser live on the
     same box read nano 0.028-0.030 and kokoro 0.072-0.076 over four warm runs of a 15 s input (a 4%
     spread, so not a quiet-box measurement; the 2026-09-02 receipts stand as the published figures);
     the audio clips under files/tts/ are the served engine's own answers to POST /v1/audio/speech on
     2026-09-03: dr-chen, brzezinski, grzegorz and hello-kitten by kitten-nano in the voice
     expr-voice-5-f at das 2efa6dde1; hello-emma and hello-bare-emma by kokoro-82m in bf_emma at
     das c08ae9bcf and 220e56bf3 (the two builds the hunt's last two paragraphs describe); the bare
     line's tail breath: ours 0.012-0.014 RMS at 74-95% above 4 kHz over 1320-1380 ms against the
     reference pipeline's 0.014-0.017 at 84-98% over 1340-1380 ms, versus 0.041 after the question mark
     (2026-09-03, das 220e56bf3); the model sizes (59, 296, 353 MB) and Kokoro's 54 voice packs are the
     model card's byte counts at the Hugging Face commit c5247896 (2026-09-03); "the same eighteen token
     ids, the same 49,800 samples" is the reference-pipeline probe of 2026-09-03 at das c08ae9bcf; "19 MB"
     is the pack builder's estimate for two more plain tables (2.7 + 3.6 MB packed, 2026-09-03) on top of
     the v1 pack; "44,000 words" is the builder's own print at 850f0ba8a (cmudict 79,062 kept, 44,393
     dropped) -->
dasLLAMA exists to run inside games, and games are where the GPU is never yours. The frame
owns it. What a game has to spare is a few CPU cores, sometimes, and an engine that lives on
those cores gets to do the things a game would like an engine to do: hear the player
(speech-to-text), think about what was said (correct the dictation, play a character, run the
announcer), and now, answer out loud.

Text-to-speech is the third leg, and the one with the most obvious uses: a typed line becomes
a voice, a generated dialogue becomes a scene, a dynamic announcer reads a score that did not
exist a second ago. So we went on a journey. It opened on the 1st of September and merged on
the 3rd, and it was fun.

## The part before the model

Text is not phonemes. Before anything can say "Dr. Chen read 3.5 pages and paid $12." it has
to become

```
Doctor Chen read three point five pages and paid twelve dollars.
```

and then every word has to become its sounds:

```
dˈɑktəɹ ʧˈɛn ɹˈɛd θɹˈi pˈYnt fˈIv pˈAʤᵻz ænd pˈAd twˈɛlv dˈɑləɹz.
```

<audio controls preload="none" src="/files/tts/dr-chen.wav"></audio>

Note "read" coming out as `ɹˈɛd`, the past tense, because the tagger looked at the sentence
and said so. The reference pipelines do all this with a stack of Python packages and a
phonemizer process, which is fine for a notebook. A game ships an executable and its data, so
the front end here is data: a normalizer that reads numbers, dates, money, units and
abbreviations the way a reader does, a part-of-speech tagger, and a pronunciation table with a
spelling model behind it for the words the table does not know.

It is remarkable how much has to go right for one sentence to come out. Every number has a
reading, every abbreviation a word, every "read" a tense, and every word that is spelled like
another word a rule that tells them apart. We got a lot of it right: of the 38 heteronyms in
our corpus 32 come out the way a reader would say them, where the reference front end manages
24. "Bass player" and "the pipes lead out" took an afternoon each.

Names the table does not know go to the spelling model, which has opinions. It knows Zbigniew
Brzezinski:

```
zbˈɪɡnjɛf bɹəzˈɪnski.
```

<audio controls preload="none" src="/files/tts/brzezinski.wav"></audio>

Asked for Grzegorz Brzęczyszczykiewicz, it answers

```
ɡˈɜɹzəɡɹˌɑs bɹˈɪzʧɪzkˈIkəbəlz.
```

<audio controls preload="none" src="/files/tts/grzegorz.wav"></audio>

## Small kittens, big Kokoro

Kitten nano is 59 MB, eight voices, and the served default. Kitten mini is 296 MB. Kokoro is
353 MB with 54 voice packs, of which we drive the 28 English ones: twenty American and, since
this week, eight British. All three speak at 24 kHz, and all three are the same machine
underneath, one assembly in the engine with a small family file for each one's quirks.

We made the kittens say two sentences and timed them on the M1 Max. Kokoro too.

| | first speech | the rows form | the 8-bit lane | today |
|---|---|---|---|---|
| kitten-nano, RTF | 0.22 | 0.051 | 0.040 | 0.026 |
| kokoro-82m, RTF | 0.66 | 0.168 | 0.116 | 0.072 |

RTF is the real-time factor, seconds of compute per second of audio, so 0.026 means fifteen
seconds of speech in 383 milliseconds. The two big moves were the ones the language models
already had: 8-bit weights and the worker lanes. Nothing clever, and it still went from 0.66
to 0.07. The reference engines on the same input, same box:

| RTF, M1 Max | dasLLAMA, q8 lane | reference engine |
|---|---|---|
| kitten-nano | 0.026 | 0.035 (onnxruntime) |
| kokoro-82m | 0.072 | 0.097 (torch CPU) |

## How we measured quality

Speed is easy to measure and easy to fake: a model that speaks fast and says the wrong thing
is a bug with a good RTF, and memset(0) even sounds smart. So the rig synthesizes the whole
200-sentence corpus with each model, transcribes the audio back with a speech-to-text model
that has never heard of us, and scores the word error rate of the round trip beside a
naturalness score. The reference engines' own audio for the same sentences goes through the
same transcriber and the same scorer, so both columns share a yardstick.

Then we found the yardstick was bent: the "expected" text the rig scored against was the
reference normalizer's output, defects included, so a correct "kilograms" counted as an error
and "kay gee" did not.

| 200 sentences, WER % / UTMOS | dasLLAMA, q8 lane | the reference engine's own audio |
|---|---|---|
| kitten-nano | 3.23 / 3.971 | 4.50 / 4.035 |
| kitten-mini | 2.77 / 4.335 | 4.09 / 3.996 |
| kokoro-82m | 2.73 / 4.500 | 3.32 / 4.499 |

## The hunt for "hellows"

We picked a British voice on the speech studio, typed "Hello, how are you", and Emma said
"hellows, how are yous". That is the kind of bug that ruins an afternoon in the best way.

First suspect: the dialect. Kokoro picks its phonemizer by the first letter of the voice's
name, `b` for British, and our front end only spoke American. We had been handing an
Englishwoman American phonetics and wondering why she sounded odd. So we taught it British,
which turned out to be American with the r's removed and the vowels moved half a step, plus a
table of the words that disagree, until "Hello, how are you?" came out

```
həlˈQ, hˌW ɑː juː?
```

byte for byte what the reference pipeline produces, the same eighteen token ids, the same
49,800 samples of audio.

Emma still said "hellows".

<audio controls preload="none" src="/files/tts/hello-emma.wav"></audio>

Second suspect: us. Odd that it was not the first. We put our waveform next to the reference
pipeline's for the same line and the same voice and looked at where the energy above 4 kHz
went; a hiss is nothing else.

| 20 ms frame | reference (RMS, share above 4 kHz) | ours |
|---|---|---|
| 660 ms, right after "Hello," | 0.034, 91% | 0.035, 93% |
| 680 ms | 0.025, 84% | 0.023, 82% |
| 1380 ms, right after "you?" | 0.032, 100% | 0.034, 100% |
| 1400 ms | 0.041, 100% | 0.041, 100% |

Frame for frame, within two percent, both lanes. It was like looking in the mirror. The model
renders a British voice's comma as a breath and a full stop as a longer one, and the
reference's own voice table grades those packs accordingly. The engine was innocent.

Then we removed every punctuation mark and still heard one at the end. That one was ours. We
knew we should have started there. The sentence chunker appended a comma to any piece the
split left bare, which is what the KittenTTS driver does and what Kokoro's does not. Now each
family gets its own driver's rule, and "hello how are u" goes to the model as

```
həlˈQ hˌW ɑː jˈuː
```

with nothing after it. Emma still gives a breath at the end. How very British of her.

<audio controls preload="none" src="/files/tts/hello-bare-emma.wav"></audio>

Kitten is the served default; in the same test its voices did not breathe at the comma.

<audio controls preload="none" src="/files/tts/hello-kitten.wav"></audio>

## British, for 1.3 megabytes

Two more tables would have made the pronunciation pack 19 MB. It turns out the British word
for an American key is "key", so both dialects share one, the spelling model went to 16-bit,
and the fallback dictionary dropped the 44,000 words both dialects already carry:

| pronunciation pack | American only | American + British |
|---|---|---|
| bytes | 12,719,239 | 14,011,554 |

On the British corpus, 149 of the 157 sentences the reference answers from its table alone
come out identical. Eight differ: two are tag calls that could go either way, and six are ours
by rule: the lead pipe, the bass player, this minute detail, the does are watching, get the
lead out, and my resume. Never rhyme retain and Britain.

## Where it ends

So the server speaks now, the page shows what the model heard before it spoke, and every chat
answer can read itself out. British Emma still does the talking around here: she narrated our
tutorial videos through a Kokoro server we had to keep running next to the recorder, and the
engine that generated the script now answers that same request itself, at fourteen times real
time on a laptop. Never again the other way. Kitten nano does it at thirty-eight times and leaves
the rest of the core to the frame, which is the number a game reads first, and the one we set
out to get.
