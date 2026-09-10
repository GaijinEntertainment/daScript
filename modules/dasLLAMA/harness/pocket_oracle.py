"""The Pocket TTS oracle: Kyutai's reference package run with its per-frame noise captured and
the stage tensors dumped for the das port to match.

    ~/Work/tts-ab/pocket/.venv-pocket/bin/python pocket_oracle.py --language english_2026-04 \\
        --hub ~/Work/tts-ab/pocket/hub --out ~/Work/llama.cpp/models/tts_oracle \\
        [--voices alba,caro_davy] [--count 12] [--stage-cases 4]

Same corpus, container and manifest shape as kitten_oracle.py / kokoro_oracle.py (the das rig
reads all three), one `<sentence-id>_<voice>.bin` per case under `<out>/pocket_<language>/`:
the prepared text's token ids, the voice clip the encoder read (24 kHz mono f32) with its
encoder latents, then per generated frame the backbone's conditioning vector, the EOS logit,
the noise x0 the flow head started from, its output x1 and the denormalized latent the
decoder read, and the waveform. The bring-up set adds the projected voice conditioning and
the KV cache after the voice prompt. Every voice goes through the ENCODER (the clip path),
never the package's precomputed state, and the `alba` case checks that the two agree.

The script also checks the one-shot decode claim the das port rests on: the whole latent
sequence through `decode_from_latent` in one call equals the frame-by-frame streamed output.
"""
import argparse
import json
import os
import sys
import types

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from tts_oracle_io import pick_sentences, write_dump  # noqa: E402
from convert_pocket import ENGLISH_VOICES, LANGUAGE_VOICES, load_config  # noqa: E402

FIXTURE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "tests", "_tts_fixtures", "g2p_corpus.json")


def voice_clip_path(hub, language, name):
    roster = ENGLISH_VOICES if language.startswith("english") else LANGUAGE_VOICES[language]
    rel = roster[name][0]
    if rel.startswith("tts-voices:"):
        return os.path.join(hub, "tts-voices", rel[len("tts-voices:"):])
    if language.startswith("english"):
        return os.path.join(hub, "tts-voices", rel)
    return os.path.join(hub, "pocket-tts", rel)


class FrameTrace:
    """FlowLMModel.forward re-run with every intermediate kept: one entry per call, prompts
    (an empty latent sequence) marked so the per-frame records skip them."""

    def __init__(self):
        self.calls = []

    def install(self, flow_lm):
        import torch
        from pocket_tts.models.flow_lm import lsd_decode, ot_decode
        from functools import partial
        trace = self

        def forward(self, sequence, text_embeddings, model_state, sampler_decode_steps, temp, noise_clamp, eos_threshold):
            is_prompt = sequence.shape[1] == 0
            sequence = torch.where(torch.isnan(sequence), self.bos_emb, sequence)
            input_ = self.input_linear(sequence)
            transformer_out = self.backbone(input_, text_embeddings, sequence, model_state=model_state).to(torch.float32)
            transformer_out = transformer_out[:, -1]
            eos_logit = self.out_eos(transformer_out)
            out_eos = eos_logit > eos_threshold
            noise_shape = transformer_out.shape[:-1] + (self.ldim,)
            std = temp ** 0.5
            noise = torch.empty(noise_shape, dtype=transformer_out.dtype, device=transformer_out.device)
            if noise_clamp is None:
                torch.nn.init.normal_(noise, mean=0.0, std=std)
            else:
                torch.nn.init.trunc_normal_(noise, mean=0.0, std=std, a=-noise_clamp, b=noise_clamp)
            conditioned_flow = partial(self.flow_net, transformer_out)
            decode = ot_decode if self.flow_type == "flow_matching" else lsd_decode
            x1 = decode(conditioned_flow, noise, sampler_decode_steps)
            trace.calls.append(dict(prompt=is_prompt, cond=transformer_out[0].numpy().copy(), eos=float(eos_logit[0, 0]),
                                    x0=noise[0].numpy().copy(), x1=x1[0].numpy().copy()))
            return x1, out_eos

        flow_lm.forward = types.MethodType(forward, flow_lm)


def kv_after_prompt(model_state):
    """The backbone's KV cache rows the prompt filled, per layer: [2, T, H, D]."""
    out = {}
    for name, st in model_state.items():
        if "cache" not in st:
            continue
        layer = int(name.split(".layers.")[1].split(".")[0])
        t = int(st["offset"].view(-1)[0].item())
        out[f"kv_voice_L{layer}"] = st["cache"][:, 0, :t].numpy().astype(np.float32).copy()
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--language", default="english_2026-04")
    ap.add_argument("--hub", default=os.path.expanduser("~/Work/tts-ab/pocket/hub"))
    ap.add_argument("--out", required=True)
    ap.add_argument("--voices", default="alba,caro_davy")
    ap.add_argument("--count", type=int, default=12)
    ap.add_argument("--seed-base", type=int, default=20260909)
    ap.add_argument("--stage-cases", type=int, default=4)
    ap.add_argument("--max-tokens", type=int, default=50, help="a sentence over this is chunked by the package; skipped here")
    ap.add_argument("--tokens-fixture", default=None,
                    help="also write the whole corpus's prepared text and token ids as JSON (the tokenizer-parity fixture)")
    ap.add_argument("--tokens-only", action="store_true", help="write the tokens fixture and stop")
    ap.add_argument("--texts", default=None,
                    help="a JSON list of {id, text} rows to use instead of the English corpus (a language's own sentences)")
    a = ap.parse_args()
    import torch
    from pocket_tts import TTSModel
    from pocket_tts.data.audio import audio_read
    from pocket_tts.data.audio_utils import convert_audio
    from pocket_tts.models.text_chunking import prepare_text_prompt
    from pocket_tts.modules.stateful_module import init_states
    from pocket_tts.utils.utils import get_predefined_voice, download_if_necessary
    from pocket_tts.models.model_state import _import_model_state

    cfg = load_config(a.language)
    revision = cfg["weights_path"].split("@")[1]
    model = TTSModel.load_model(language=a.language)
    model.max_decoder_frames_per_call = 1   # frame-by-frame streaming: the strictest form of the one-shot claim
    trace = FrameTrace()
    trace.install(model.flow_lm)
    rows = json.load(open(FIXTURE, encoding="utf8"))
    if a.texts:
        texts = json.load(open(a.texts, encoding="utf8"))
        if isinstance(texts, dict):   # tests/_tts_fixtures/pocket_sentences.json: one list per language config
            texts = texts[a.language]
        rows = [dict(r, norm=r["text"]) for r in texts]
    if a.tokens_fixture:
        # every corpus sentence through the package's prepare rule and tokenizer, plus the chunk
        # list of a few multi-sentence texts (the chunker-parity rows) - the byte-fallback probes
        # carry an emoji and a Cyrillic word the 4000-piece vocabulary cannot spell
        from pocket_tts.models.text_chunking import split_into_best_sentences
        tok = model.flow_lm.conditioner.tokenizer
        fx = {"language": a.language, "revision": revision, "rows": [], "chunks": [], "probes": []}
        for r in rows:
            prepared, guess = prepare_text_prompt(r["text"], model.pad_with_spaces_for_short_inputs, model.remove_semicolons,
                                                  model.append_terminal_punctuation)
            fx["rows"].append({"id": r["id"], "prepared": prepared, "frames_after_eos": guess + 2,
                               "ids": [int(i) for i in tok(prepared)[0].tolist()]})
        long_texts = [" ".join(r["text"] for r in rows[i:i + 6]) for i in (0, 40, 100, 150) if rows[i:i + 6]]
        if not a.texts:
            long_texts.append("Dr. Smith paid $12.50 at 3.14 p.m.; then he left! Really? Yes... quite so. " * 3)
        for text in long_texts:
            fx["chunks"].append({"text": text, "chunks": split_into_best_sentences(tok, text, a.max_tokens, model.pad_with_spaces_for_short_inputs,
                                                                                    model.remove_semicolons, model.append_terminal_punctuation)})
        for text in ["Hello world, this is a test of 3.14 kg.", "The café served crème brûlée \U0001F600 to Москва.",
                     "a", "  spaces   everywhere  ", "ALL CAPS SHOUTING", "tabs\tand\nnewlines"]:
            fx["probes"].append({"text": text, "ids": [int(i) for i in tok(text)[0].tolist()]})
        with open(a.tokens_fixture, "w", encoding="utf8") as f:
            json.dump(fx, f, indent=1, ensure_ascii=False)
        print(f"tokens fixture: {len(fx['rows'])} rows, {len(fx['chunks'])} chunk texts, {len(fx['probes'])} probes -> {a.tokens_fixture}", flush=True)
        if a.tokens_only:
            return
    picked = rows if a.texts else pick_sentences(rows, a.count)
    out_dir = os.path.join(a.out, f"pocket_{a.language}")
    os.makedirs(out_dir, exist_ok=True)
    manifest = {"size": a.language, "revision": revision, "seed_base": a.seed_base, "temp": model.temp,
                "frames_after_eos": model.model_recommended_frames_after_eos, "cases": []}
    std = model.flow_lm.emb_std.numpy().astype(np.float32)
    mean = model.flow_lm.emb_mean.numpy().astype(np.float32)

    voices = {}
    for vname in a.voices.split(","):
        path = voice_clip_path(a.hub, a.language, vname)
        audio, sr = audio_read(path)
        pcm = convert_audio(audio, sr, model.sample_rate, 1)
        with torch.no_grad():
            latents = model.mimi.encode_to_latent(pcm.unsqueeze(0))[0].numpy().astype(np.float32)
            cond = torch.nn.functional.linear(torch.from_numpy(latents), model.flow_lm.speaker_proj_weight).numpy().astype(np.float32)
        trace.calls.clear()
        state = model.get_state_for_audio_prompt(pcm)
        voices[vname] = dict(pcm=pcm[0].numpy().astype(np.float32), latents=latents, cond=cond, state=state, kv=kv_after_prompt(state))
        print(f"voice {vname}: {pcm.shape[-1]} samples -> {latents.shape[0]} latent frames, prompt offset "
              f"{int(next(iter(state.values()))['offset'].view(-1)[0])}", flush=True)
        try:   # the package's precomputed state for this voice, where it has one, must match the clip path
            pre = _import_model_state(download_if_necessary(get_predefined_voice(a.language, vname)), model.device)
            d = max(float(np.abs(v - pre[k]["cache"][:, 0, :v.shape[1]].numpy()).max()) for k, v in
                    ((n, s["cache"][:, 0, :int(s["offset"].view(-1)[0])].numpy()) for n, s in state.items() if "cache" in s)
                    if k in pre)
            print(f"  precomputed state vs clip path: max |diff| {d:.3e}", flush=True)
        except Exception as e:   # noqa: BLE001
            print(f"  no precomputed state to compare ({type(e).__name__})", flush=True)

    for si, r in enumerate(picked):
        prepared, guess = prepare_text_prompt(r["text"], model.pad_with_spaces_for_short_inputs, model.remove_semicolons,
                                              model.append_terminal_punctuation)
        ids = model.flow_lm.conditioner.prepare(prepared)[0].numpy().astype(np.int32)
        if len(ids) > a.max_tokens:
            print(f"skip {r['id']}: {len(ids)} tokens > {a.max_tokens}", flush=True)
            continue
        for vi, (vname, v) in enumerate(voices.items()):
            seed = a.seed_base + 1000 * si + vi
            torch.manual_seed(seed)
            trace.calls.clear()
            with torch.no_grad():
                wave = model.generate_audio(v["state"], r["text"], copy_state=True).numpy().astype(np.float32)
            frames = [c for c in trace.calls if not c["prompt"]]
            n_frames = wave.shape[0] // 1920
            assert wave.shape[0] == n_frames * 1920, wave.shape
            assert n_frames <= len(frames), (n_frames, len(frames))
            frames = frames[:n_frames]   # the last call(s) past frames_after_eos never reach the decoder
            eos_frame = next((i for i, c in enumerate(frames) if c["eos"] > model.eos_threshold), -1)
            x1 = np.stack([c["x1"] for c in frames])
            latent = x1 * std + mean
            with torch.no_grad():   # the one-shot claim
                mimi_state = init_states(model.mimi, batch_size=1, sequence_length=n_frames * 16)
                oneshot = model.mimi.decode_from_latent(torch.from_numpy(latent)[None], mimi_state)[0, 0].numpy().astype(np.float32)
            oneshot_diff = float(np.abs(oneshot - wave).max()) if oneshot.shape == wave.shape else float("inf")
            records = {
                "text_ids": ids,
                "voice_pcm": v["pcm"],
                "voice_latents": v["latents"],
                "cond": np.stack([c["cond"] for c in frames]),
                "eos_logit": np.array([c["eos"] for c in frames], dtype=np.float32),
                "x0": np.stack([c["x0"] for c in frames]),
                "x1": x1,
                "latent": latent.astype(np.float32),
                "waveform": wave,
            }
            if si < a.stage_cases:
                records["voice_cond"] = v["cond"]
                records.update(v["kv"])
                records["waveform_oneshot"] = oneshot
            fname = f"{r['id']}_{vname}.bin"
            write_dump(os.path.join(out_dir, fname), records)
            manifest["cases"].append({"file": fname, "id": r["id"], "text": r["text"], "norm": r["norm"], "prepared": prepared,
                                      "voice": vname, "speed": 1.0, "seed": seed, "n_tokens": int(len(ids)), "n_frames": n_frames,
                                      "eos_frame": eos_frame, "n_samples": int(wave.shape[0]), "voice_frames": int(v["latents"].shape[0]),
                                      "stages": si < a.stage_cases})
            print(f"pocket {r['id']} {vname}: {len(ids)} tokens, {n_frames} frames (eos at {eos_frame}), {wave.shape[0]} samples; "
                  f"one-shot decode vs streamed max |diff| {oneshot_diff:.3e}", flush=True)
    with open(os.path.join(out_dir, "manifest.json"), "w", encoding="utf8") as f:
        json.dump(manifest, f, indent=1, ensure_ascii=False)
    with open(os.path.join(out_dir, "LICENSE"), "w", encoding="utf8") as f:
        f.write(f"pocket tts oracle dumps - derived from kyutai/pocket-tts languages/{a.language} @ {revision} (CC BY 4.0, Kyutai) "
                "run through PyTorch, with voice clips from kyutai/tts-voices; see the dasLLAMA THIRD_PARTY_NOTICES.md\n")
    print(f"pocket: {len(manifest['cases'])} cases -> {out_dir}")


if __name__ == "__main__":
    main()
