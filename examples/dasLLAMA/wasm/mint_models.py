#!/usr/bin/env python3
"""Stage a browser example's model set: fetch the GGUFs and packs its models.json names from
Hugging Face, mint each GGUF into a .dlim against the wasm64 build's DlimConfiguration, copy the
packs, and write models/manifest.json - the list the example's web shell reads, stamped with the
IMAGE_VERSION the images carry.

    mint_models.py --example examples/dasLLAMA/storywish --config wasm64.json \
                   --daslang bin/daslang --out _site/examples/storywish/models \
                   [--cache ~/.cache/dasllama-web-models] [--expect-image-version 35] [--stamp-page page.html]

The GGUFs are cached by sha256 (a download is verified before it is used, and never trusted from
the cache without its hash in the name); the images are minted on every run - it takes
milliseconds, and a mint for THIS build is the whole point. --expect-image-version is the deploy
gate: the version read from each minted image's header must be the tree's, or the run fails.
--stamp-page writes that version into the shell's `/* @image-version */ 0` slot, so the page can
refuse a set minted for another version before it fetches one.

Standard library only: the deploy runner has python3 and nothing else.
"""
import argparse, hashlib, json, os, shutil, struct, subprocess, sys, urllib.request

HF = "https://huggingface.co"
IMAGE_MAGIC = 0x314d4c44   # 'DLM1' little-endian, dasllama_image.das


def sha256_of(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def fetch(entry, cache):
    """the file at cache/<sha256>-<name>, downloaded from its repo when absent, verified either way"""
    os.makedirs(cache, exist_ok=True)
    target = os.path.join(cache, f"{entry['sha256']}-{entry['file']}")
    if not os.path.exists(target):
        url = f"{HF}/{entry['repo']}/resolve/main/{entry['file']}"
        print(f"fetching {url}")
        tmp = target + ".part"
        with urllib.request.urlopen(urllib.request.Request(url, headers={"User-Agent": "dasllama-web-models"})) as r, open(tmp, "wb") as f:
            shutil.copyfileobj(r, f, 1 << 20)
        os.replace(tmp, target)
    got = sha256_of(target)
    if got != entry["sha256"]:
        os.remove(target)
        raise SystemExit(f"{entry['file']}: sha256 {got}, models.json says {entry['sha256']} - the repo file changed; update models.json or pin the old one")
    return target


def image_version_of(path):
    with open(path, "rb") as f:
        magic, version = struct.unpack("<II", f.read(8))
    if magic != IMAGE_MAGIC:
        raise SystemExit(f"{path}: not a dasLLAMA image (magic {magic:#x})")
    return version


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--example", required=True, help="the example's directory (holds models.json)")
    ap.add_argument("--config", required=True, help="the wasm64 build's DlimConfiguration JSON (examples/dasLLAMA/wasm/dlim_config under node)")
    ap.add_argument("--daslang", required=True, help="the host daslang that runs utils/dasllama-convert")
    ap.add_argument("--out", required=True, help="the models/ directory to stage beside the page")
    ap.add_argument("--cache", default=os.path.expanduser("~/.cache/dasllama-web-models"))
    ap.add_argument("--expect-image-version", type=int, default=0, help="fail unless every minted image carries this IMAGE_VERSION")
    ap.add_argument("--stamp-page", default="", help="write the image version into this shell's `/* @image-version */ 0` slot")
    a = ap.parse_args()

    repo_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
    with open(os.path.join(a.example, "models.json")) as f:
        spec = json.load(f)
    os.makedirs(a.out, exist_ok=True)
    files, versions = [], set()

    for entry in spec.get("images", []):
        gguf = fetch(entry, a.cache)
        dlim = os.path.join(a.out, entry["dlim"])
        if os.path.exists(dlim):
            os.remove(dlim)
        env = dict(os.environ, DASLLAMA_IMAGE="0")   # the source GGUF's directory gets no sidecar of the host's own identity
        cmd = [a.daslang, "-jit", os.path.join(repo_root, "utils", "dasllama-convert", "main.das"), "--",
               "-m", gguf, "--config", a.config, "-o", dlim]
        r = subprocess.run(cmd, env=env, capture_output=True, text=True)
        if r.returncode != 0 or not os.path.exists(dlim):
            sys.stderr.write(r.stdout[-4000:] + r.stderr[-4000:])
            raise SystemExit(f"mint of {entry['file']} failed (exit {r.returncode})")
        version = image_version_of(dlim)
        versions.add(version)
        files.append({"name": entry["dlim"], "bytes": os.path.getsize(dlim), "sha256": sha256_of(dlim), "source": f"{entry['repo']}/{entry['file']}"})
        print(f"minted {entry['dlim']} ({os.path.getsize(dlim) >> 20} MB, IMAGE_VERSION {version}) from {entry['file']}")

    for entry in spec.get("packs", []):
        src = fetch(entry, a.cache)
        dst = os.path.join(a.out, entry["file"])
        shutil.copyfile(src, dst)
        files.append({"name": entry["file"], "bytes": os.path.getsize(dst), "sha256": entry["sha256"], "source": f"{entry['repo']}/{entry['file']}"})

    if len(versions) != 1:
        raise SystemExit(f"the minted images disagree on IMAGE_VERSION: {sorted(versions)}")
    version = versions.pop()
    if a.expect_image_version and version != a.expect_image_version:
        raise SystemExit(f"minted images carry IMAGE_VERSION {version}, the tree says {a.expect_image_version} - the converter that minted them is not this tree's")

    manifest = {"image_version": version, "files": files}
    with open(os.path.join(a.out, "manifest.json"), "w") as f:
        json.dump(manifest, f, indent=2)
        f.write("\n")
    print(f"manifest.json: IMAGE_VERSION {version}, {len(files)} files, {sum(f['bytes'] for f in files) >> 20} MB")

    if a.stamp_page:
        with open(a.stamp_page) as f:
            page = f.read()
        slot = "/* @image-version */ 0"
        if slot not in page:
            raise SystemExit(f"{a.stamp_page}: no `{slot}` slot to stamp")
        with open(a.stamp_page, "w") as f:
            f.write(page.replace(slot, f"/* @image-version */ {version}"))
        print(f"stamped {a.stamp_page} with IMAGE_VERSION {version}")


if __name__ == "__main__":
    main()
