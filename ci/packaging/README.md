# Packaging — release artifacts and package-manager manifests

The canonical distribution source is the GitHub release: four stable-named bundles
(`daslang-bundle-{linux-x86_64,linux-arm64,darwin26-arm64,windows-x86_64}.zip`, unix
modes preserved), each with a sibling `.zip.sha256`, plus a `.deb` for Debian/Ubuntu —
all produced by `release.yml` at prerelease-cut time. Every package manager below is a
pointer at those assets.

## The per-release ritual

1. Cut the prerelease tag; `release.yml` uploads bundles + `.sha256` files + the `.deb`.
2. **Homebrew tap** (repo `homebrew-daslang`, formula `Formula/daslang.rb` from
   `homebrew-daslang.rb.template`): fill `@TAG@`/`@VERSION@` and the three `@SHA_*@`
   values from the `.sha256` assets, push to the tap.
   Users: `brew install <org>/daslang/daslang`.
3. **Scoop bucket** (repo `scoop-daslang`, `bucket/daslang.json` from
   `scoop-daslang.json.template`): fill version/tag/hash, push.
   Users: `scoop bucket add daslang <repo-url>; scoop install daslang`.
4. **winget** (real releases ONLY, never an RC): render the manifest trio from
   `winget-daslang.yaml.template` and PR it to microsoft/winget-pkgs.
5. **apt**: the `.deb` on the release page installs with
   `sudo apt install ./daslang_<version>_amd64.deb` (binaries land in `/opt/daslang`,
   `daslang`/`daslang-live`/`gen1_to_gen2` symlinked into `/usr/bin`). A hosted apt
   repo is a later tier.

RC dry-runs point the tap and bucket at the RC tag to validate install paths end to
end; retargeting to the real tag is a hash+tag bump. Templates here are the copies of
record — edit them first, then propagate to the tap/bucket repos.
