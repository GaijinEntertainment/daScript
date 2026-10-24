# The reference package's wall for the served bench's one sentence, the third-party row beside
# dasllama-server's: one untimed request, then 8 timed, per config and thread count; prints the
# median ms, the seconds of speech, the real-time factor and the spread. Runs under the pocket-tts
# venv the converter's docstring names (ARCHITECTURE_MEASUREMENT_AUDIO.md#the-tts-reference-instrument).
#   python3 pocket_ref_time.py english_2026-04,english_18m_2026-10 1 cpu
#   python3 pocket_ref_time.py english_18m_2026-10 0 cuda
import sys, time, statistics, torch
from pocket_tts import TTSModel
from pocket_tts.utils.utils import get_predefined_voice, download_if_necessary
from pocket_tts.models.model_state import _import_model_state

TEXT = "The lighthouse keeper climbed the stairs once more, counting each step as the storm rolled in."
langs = sys.argv[1].split(",") if len(sys.argv) > 1 else ["english_2026-04", "english_18m_2026-10"]
threads = [int(t) for t in sys.argv[2].split(",")] if len(sys.argv) > 2 else [0, 1]
device = sys.argv[3] if len(sys.argv) > 3 else "cpu"
sync = (lambda: torch.cuda.synchronize()) if device == "cuda" else ((lambda: torch.mps.synchronize()) if device == "mps" else (lambda: None))
print("torch", torch.__version__, "cpu threads default", torch.get_num_threads(), flush=True)
for lang in langs:
    model = TTSModel.load_model(language=lang)
    if device != "cpu":
        model.to(device)
    state = _import_model_state(download_if_necessary(get_predefined_voice(lang, "alba")), model.device)
    for th in threads:
        if th > 0:
            torch.set_num_threads(th)
        model.generate_audio(state, TEXT, copy_state=True)
        sync()
        walls, secs = [], []
        for _ in range(8):
            t0 = time.perf_counter()
            wave = model.generate_audio(state, TEXT, copy_state=True)
            sync()
            walls.append((time.perf_counter() - t0) * 1000.0)
            secs.append(wave.numel() / model.sample_rate)
        med = statistics.median(walls)
        cv = statistics.pstdev(walls) / statistics.mean(walls) * 100
        print(f"REF {lang} {device} threads={torch.get_num_threads()}: {med:.1f} ms for {statistics.median(secs):.2f} s of speech, "
              f"rtf {med / 1000 / statistics.median(secs):.3f} (cv {cv:.1f}%) over 8 reps", flush=True)
