#!/usr/bin/env python3
# Publish the dasLLAMA text-to-speech set to a Hugging Face model repository: the three GGUFs,
# the two front-end packs, their .LICENSE sidecars, the licence texts and the model card
# (harness/tts_model_card.md). Stages a copy, verifies every sha256 against the card's table,
# then uploads one commit and prints the commit-pinned resolve URLs the serving catalog rows
# take (performance/model_specs.das, serve_tts_set).
#
#   python publish_tts_hf.py --repo <user>/dasllama-tts [--models ~/Work/llama.cpp/models] [--dry-run]
#
# Auth: HF_TOKEN in the environment, or `huggingface-cli login` beforehand. Runs in the
# experiment venv (huggingface_hub importable).
import argparse, hashlib, os, re, shutil, sys, tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
MODULE = os.path.dirname(HERE)
CARD = os.path.join(HERE, "tts_model_card.md")
FILES = ["kitten-nano.gguf", "kitten-mini.gguf", "kokoro-82m.gguf", "tts_g2p.bin", "tts_postag.bin"]
LICENCES = ["LICENSE.APACHE-2.0", "LICENSE.CMUDICT", "LICENSE.UD_EWT", "LICENSE.SPACY", "LICENSE.STYLETTS2"]


def sha256_of(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def card_hashes(card_text):
    # the Files table: | `name` | what | bytes | `sha` |
    out = {}
    for m in re.finditer(r"^\| `([^`]+)` \| [^|]* \| (\d+) \| `([0-9a-f]{64})` \|", card_text, re.M):
        out[m.group(1)] = (int(m.group(2)), m.group(3))
    return out


def stage(models_dir, work):
    card_text = open(CARD, encoding="utf8").read()
    expected = card_hashes(card_text)
    missing = [f for f in FILES if f not in expected]
    if missing:
        sys.exit(f"the model card's Files table lacks: {missing}")
    for name in FILES:
        src = os.path.join(models_dir, name)
        if not os.path.exists(src):
            sys.exit(f"missing {src}")
        size, sha = os.path.getsize(src), sha256_of(src)
        if (size, sha) != expected[name]:
            sys.exit(f"{name}: the file is {size} bytes / {sha}, the card says {expected[name]} - rebuild the card's table or the file")
        shutil.copy2(src, os.path.join(work, name))
        sidecar = src + ".LICENSE"
        if not os.path.exists(sidecar):
            sys.exit(f"missing {sidecar} - every published file ships its licence sidecar")
        shutil.copy2(sidecar, os.path.join(work, name + ".LICENSE"))
    for lic in LICENCES:
        shutil.copy2(os.path.join(MODULE, lic), os.path.join(work, lic))
    shutil.copy2(CARD, os.path.join(work, "README.md"))
    return sorted(os.listdir(work))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--repo", required=True, help="<user>/<name> on huggingface.co")
    ap.add_argument("--models", default=os.path.expanduser("~/Work/llama.cpp/models"))
    ap.add_argument("--dry-run", action="store_true", help="stage and verify, upload nothing")
    ap.add_argument("--message", default="dasLLAMA text-to-speech set")
    a = ap.parse_args()
    work = tempfile.mkdtemp(prefix="dasllama_tts_hf_")
    try:
        staged = stage(a.models, work)
        print("staged:", ", ".join(staged))
        if a.dry_run:
            return
        from huggingface_hub import HfApi
        api = HfApi()
        api.create_repo(a.repo, repo_type="model", exist_ok=True)
        info = api.upload_folder(folder_path=work, repo_id=a.repo, repo_type="model", commit_message=a.message)
        sha = getattr(info, "oid", None) or getattr(info, "commit_sha", None) or str(info)
        print(f"commit {sha}")
        for name in FILES:
            print(f"https://huggingface.co/{a.repo}/resolve/{sha}/{name}")
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    main()
