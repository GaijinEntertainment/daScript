# The reference package's pace on three texts at alba: total seconds, the lead and tail silence,
# the pauses (runs of five or more quiet 20 ms windows) and the words a second of speech - the
# third-party side of `pocket_pace.das`'s reading. Runs under the pocket-tts venv the converter's
# docstring names; writes each clip as ref_<config>_<n>.wav beside it.
#   python3 pocket_ref_pace.py
import numpy as np, soundfile as sf, torch
from pocket_tts import TTSModel
from pocket_tts.utils.utils import get_predefined_voice, download_if_necessary
from pocket_tts.models.model_state import _import_model_state

TEXTS = [
    "Whose woods these are I think I know, his house is in the village though, he will not see me stopping here, to watch his woods fill up with snow.",
    "Whose woods these are I think I know. His house is in the village though. He will not see me stopping here. To watch his woods fill up with snow.",
    "The quick brown fox jumps over the lazy dog, and the small model speaks it in the browser's own host.",
]


def analyze(pcm, rate):
    win = rate // 50
    n = len(pcm) // win
    rms = np.sqrt((pcm[:n * win].reshape(n, win) ** 2).mean(1))
    thr = rms.max() * 0.01
    loud = np.where(rms > thr)[0]
    first, last = loud[0], loud[-1]
    pauses = pause_win = run = 0
    for i in range(first, last + 1):
        if rms[i] <= thr:
            run += 1
        else:
            if run >= 5:
                pauses += 1
                pause_win += run
            run = 0
    speech = (last - first + 1 - pause_win) * 0.02
    return len(pcm) / rate, first * 0.02, (n - 1 - last) * 0.02, pauses, pause_win * 0.02, speech


if __name__ == "__main__":
    torch.manual_seed(0)
    for lang in ["english_2026-04", "english_18m_2026-10"]:
        model = TTSModel.load_model(language=lang)
        state = _import_model_state(download_if_necessary(get_predefined_voice(lang, "alba")), model.device)
        for ti, text in enumerate(TEXTS):
            wave = model.generate_audio(state, text, copy_state=True).numpy().astype(np.float32).reshape(-1)
            total, lead, tail, pauses, pause_s, speech = analyze(wave, model.sample_rate)
            words = len(text.split())
            print(f"REF {lang} text{ti}: total {total:.2f} s, lead {lead:.2f}, tail {tail:.2f}, pauses {pauses} ({pause_s:.2f} s), "
                  f"speech {speech:.2f} s, {words / speech:.2f} words/s", flush=True)
            sf.write(f"ref_{lang}_{ti}.wav", wave, model.sample_rate)
