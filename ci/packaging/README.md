# Packaging - release artifacts and package-manager manifests

Two packages, each on its own release tags and each described once in `packages.py` (commands,
install prefix, dependencies, the tap and bucket files it publishes) and read by every builder
here:

- **daslang** - tags `vX.Y.Z` and `vX.Y.Z-RCn`, `release.yml`: four stable-named bundles
  (`daslang-bundle-{linux-x86_64,linux-arm64,darwin26-arm64,windows-x86_64}.zip`, unix modes
  preserved), a `.deb` (x86_64), an `.rpm` per linux arch, a pip wheel per bundle. The same
  tag cuts the docs (`doc.yml`) and the cpp-mcp archives (`cpp_mcp_release.yml`).
- **dasllama** - tags `dasllama-vX.Y.Z` and `dasllama-vX.Y.Z-RCn`,
  `dasllama_server_release.yml`: the fat server bundle as
  `dasllama-{linux-x86_64,linux-arm64}.tar.gz`, `dasllama-darwin-arm64.zip` (the `.app`) and
  `dasllama-windows-x64.zip`, a `.deb` and an `.rpm` per linux arch, a pip wheel per platform.
  The archives also go to the rolling `dasllama-server` release, the one dasllama.io links.

Every release workflow reads the tag's prefix and runs for its own product only: a daslang tag
cuts no dasllama, a dasllama tag cuts no SDK, docs or cpp-mcp. `packages.py`'s `bare_tag` drops
the prefix and the `v`, so every version spelling below (dpkg, rpm, PEP 440, the tap's) comes
from the version alone; the asset URLs keep the whole tag.

Every asset has a sibling `.sha256`, written by `checksum_assets.sh`. Every package manager below is a pointer at those assets,
except pip, which the workflows publish themselves.

## The per-release ritual

1. Cut the prerelease tag - `vX.Y.Z-RCn` for daslang, `dasllama-vX.Y.Z-RCn` for dasllama; the
   two are separate cuts on separate days or the same one. The product's workflow uploads the
   assets, publishes the wheels, and ends with `publish_manifests`, which renders the package's
   Homebrew and scoop manifests from the release's `.sha256` assets and pushes them to the tap
   (`homebrew-daslang`) and the bucket (`scoop-daslang`) with the `PACKAGING_TOKEN` secret.
   Check that the commit `<package> manifests @ <tag>` arrived.
   `publish_manifests.sh <package> <tag>` without `--push` prints the same diff locally.
2. **winget** (real releases ONLY, never an RC): render the manifest trio from
   `winget-daslang.yaml.template` and PR it to microsoft/winget-pkgs.
3. **site**: the install commands in `site/index.html`, `site/downloads.html` and
   `site-dasllama/index.html` name the advertised release's assets by file name, so a package
   form reaches the site with the release that first ships it.

RC cuts update the tap and bucket too, as they point at the newest cut; the templates here are
the copies of record - never edit the tap or bucket by hand.

## The package managers

| Manager | daslang | dasllama |
|---|---|---|
| Homebrew | `brew install borisbat/daslang/daslang` | `brew install borisbat/daslang/dasllama` (the commands; `brew services start dasllama` runs the watchdog at login), `brew install --cask borisbat/daslang/dasllama` (the tray app, macOS) - the two conflict |
| scoop | `scoop bucket add daslang https://github.com/borisbat/scoop-daslang; scoop install daslang` | same bucket, `scoop install dasllama` (a Start-menu shortcut to the watchdog) |
| apt | `sudo apt install ./daslang_<version>_amd64.deb` | `sudo apt install ./dasllama_<version>_<arch>.deb` (pulls `libssl3`, `curl`) |
| dnf | `sudo dnf install ./daslang-<version>-1.<arch>.rpm` | `sudo dnf install ./dasllama-<version>-1.<arch>.rpm` (pulls `openssl-libs`, `curl`) |
| pip | `pip install daslang` | `pip install dasllama` |

The linux packages install the bundle under `/opt/<package>` and link the commands into
`/usr/bin`; the dasllama links drop the bundle's `.exe` suffix and name the supervisor
`dasllama-watchdog` (the Debian `watchdog` package owns that name). A hosted apt or dnf repo is
a later tier. glibc floors: the daslang bundle is built on ubuntu-24.04 (`manylinux_2_38` on the
wheels), which every supported Fedora meets and RHEL 9 does not; the dasllama bundle on
ubuntu-22.04 (2.35), which admits Debian 12.

The installed dasllama programs never write beside themselves: config, tune sidecar and logs
live under `~/.dasllama` (`utils/dasllama-server/README.md`), so every install directory can
stay read-only.

## pip

`wheel_build.py` repacks a bundle into a platform wheel
(`<package>-<ver>-py3-none-{win_amd64,manylinux_2_NN_x86_64,manylinux_2_NN_aarch64,macosx_NN_0_arm64}.whl`)
and the `publish_pypi` jobs upload each set through trusted publishing - a plain `vX.Y.Z` tag to
PyPI, any other tag (RC, beta, ...) to TestPyPI. The daslang wheel is the toolchain minus the
C++ embedding payload and the media trees, to stay under PyPI's 100 MB per-file cap - exact set:
`EXCLUDE_*` in `wheel_build.py`; the dasllama wheel carries its bundle whole. The platform tag
is read off the binaries (highest GLIBC symbol / Mach-O minos), never assumed. Users:
`pip install daslang` (RC: `pip install -i https://test.pypi.org/simple/ daslang==<ver>rcN`),
then `daslang`, `dastest`, `lint`, `daspkg`, ... are on PATH and `python -m daslang file.das`
works; `pip install dasllama` puts `dasllama-server`, `dasllama-cli`, `dasllama-bench` and
`dasllama-watchdog` on PATH.

## Fixture tests

`python3 ci/test_wheel_build.py`, `ci/test_rpm_build.py`, `ci/test_deb_build.py`,
`ci/test_render_manifests.py` - the extended core runs them as `check_wheel_repack`,
`check_rpm_spec`, `check_deb_control` and `check_manifest_render`.
