#!/usr/bin/env python3
# Publish the dasLLAMA text-to-speech set to a Hugging Face model repository: the GGUFs,
# the front-end packs, their .LICENSE sidecars, the licence texts and the model card
# (harness/tts_model_card.md). Stages a copy, verifies every sha256 against the card's table,
# then uploads one commit and prints the commit-pinned resolve URLs the serving catalog rows
# take (performance/model_specs.das, serve_tts_set).
#
#   python publish_tts_hf.py --repo <user>/dasllama-tts [--models ~/Work/llama.cpp/models] [--dry-run]
#                            [--only kokoro-ru.gguf,tts_g2p_ru.bin] [--revision <branch>]
#
# `--only` publishes the named files alone (each still verified against the card), with the card,
# the licence texts and the sidecars - the commit for a set that grew by a few files; the files it
# leaves out stay in the repository as they are.
# `--revision` commits onto a branch and leaves main alone: the printed URLs are commit-pinned and
# resolve either way, so the catalog and the examples' models.json can pin the new set before main
# moves; merging the branch on the Hub moves main afterwards.
#
# Auth: HF_TOKEN in the environment, or `huggingface-cli login` beforehand. Runs in the
# experiment venv (huggingface_hub importable).
import argparse, hashlib, os, re, shutil, sys, tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
MODULE = os.path.dirname(HERE)
CARD = os.path.join(HERE, "tts_model_card.md")
FILES = ["kitten-nano.gguf", "kitten-mini.gguf", "kokoro-82m.gguf", "kokoro-ru.gguf", "kokoro-ru-dima.gguf",
         "kitten-nano-q8.gguf", "kitten-nano-kq.gguf", "kitten-mini-q8.gguf", "kitten-mini-kq.gguf", "kokoro-82m-q8.gguf",
         "kokoro-82m-kq.gguf", "kokoro-ru-q8.gguf", "kokoro-ru-kq.gguf", "kokoro-ru-dima-q8.gguf", "kokoro-ru-dima-kq.gguf",
         "tts_g2p.bin", "tts_g2p_en_us.bin", "tts_postag.bin", "tts_g2p_ru.bin",
         "pocket-tts-en-q8.gguf", "pocket-tts-de-q8.gguf", "pocket-tts-es-q8.gguf", "pocket-tts-it-q8.gguf",
         "pocket-tts-pt-q8.gguf", "pocket-tts-fr-q8.gguf", "pocket-tts-de-kq.gguf", "pocket-tts-es-kq.gguf",
         "pocket-tts-it-kq.gguf", "pocket-tts-pt-kq.gguf", "pocket-tts-fr-kq.gguf", "pocket-tts-en-kq.gguf", "pocket-tts-en-stuart-kq.gguf",
         "pocket-tts-en-18m-q8.gguf", "pocket-tts-en-18m-kq.gguf", "pocket-tts-en-18m-stuart-kq.gguf", "pocket-tts-en-7m-q8.gguf",
         "pocket-tts-en-2m.gguf", "pocket-tts-en-18m-refl-q8.gguf", "pocket-tts-en-7m-refl-q8.gguf", "pocket-tts-en-2m-refl.gguf"]
LICENCES = ["LICENSE.APACHE-2.0", "LICENSE.CMUDICT", "LICENSE.UD_EWT", "LICENSE.SPACY", "LICENSE.STYLETTS2", "LICENSE.CC-BY-4.0",
            "LICENSE.POCKET_TTS"]


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


def stage(models_dir, work, files):
    card_text = open(CARD, encoding="utf8").read()
    expected = card_hashes(card_text)
    missing = [f for f in files if f not in expected]
    if missing:
        sys.exit(f"the model card's Files table lacks: {missing}")
    for name in files:
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


def upload(api, repo, work, message, revision):
    """one commit of the staged folder: onto `revision`, a branch made from main where absent, or onto main with none; its sha"""
    api.create_repo(repo, repo_type="model", exist_ok=True)
    if revision:
        api.create_branch(repo, repo_type="model", branch=revision, exist_ok=True)
    info = api.upload_folder(folder_path=work, repo_id=repo, repo_type="model", commit_message=message, revision=revision)
    return getattr(info, "oid", None) or getattr(info, "commit_sha", None) or str(info)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--repo", required=True, help="<user>/<name> on huggingface.co")
    ap.add_argument("--models", default=os.path.expanduser("~/Work/llama.cpp/models"))
    ap.add_argument("--dry-run", action="store_true", help="stage and verify, upload nothing")
    ap.add_argument("--message", default="dasLLAMA text-to-speech set")
    ap.add_argument("--revision", help="commit onto this branch of the repository, created from main where absent, and leave main "
                    "as it is: the set a tree pins by commit before the files a deploy reads from main are replaced")
    ap.add_argument("--only", help="comma-separated names from the set: publish these files alone, with the card and licences")
    a = ap.parse_args()
    files = FILES
    if a.only:
        files = [f.strip() for f in a.only.split(",") if f.strip()]
        unknown = [f for f in files if f not in FILES]
        if unknown:
            sys.exit(f"--only names files outside the set: {unknown}")
    work = tempfile.mkdtemp(prefix="dasllama_tts_hf_")
    try:
        staged = stage(a.models, work, files)
        print("staged:", ", ".join(staged))
        if a.dry_run:
            return
        from huggingface_hub import HfApi
        sha = upload(HfApi(), a.repo, work, a.message, a.revision)
        print(f"commit {sha}")
        for name in files:
            print(f"https://huggingface.co/{a.repo}/resolve/{sha}/{name}")
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    main()
