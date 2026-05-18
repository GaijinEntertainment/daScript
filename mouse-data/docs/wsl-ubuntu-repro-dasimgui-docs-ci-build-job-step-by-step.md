---
slug: wsl-ubuntu-repro-dasimgui-docs-ci-build-job-step-by-step
title: How do I reproduce the dasImgui docs CI build job locally in WSL Ubuntu without sudo?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

Mirror the GitHub Actions `ubuntu-latest` docs job step-for-step inside WSL Ubuntu 24.04. Useful when sphinx-build fails in CI but local Windows builds pass — and you suspect Linux-specific behavior.

```bash
# 1. Setup (one-time). Bypass sudo password — WSL `-u root` has no password.
wsl.exe -d Ubuntu-24.04 -u root -- apt-get install -y python3-pip python3-venv build-essential libcurl4-openssl-dev libssl-dev clang flex bison libglu1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev mesa-common-dev libwayland-dev libxkbcommon-dev

# 2. Sphinx in a venv (system Python is PEP-668 externally-managed).
wsl.exe -d Ubuntu-24.04 -- bash -c "python3 -m venv ~/sphinx-venv && ~/sphinx-venv/bin/pip install --quiet sphinx==7.2.6 sphinx-rtd-theme==2.0.0"

# 3. Clone daslang master + the dasImgui PR branch as siblings.
wsl.exe -d Ubuntu-24.04 -- bash -c "
  mkdir -p ~/ci-repro && cd ~/ci-repro
  git clone --depth=1 -b master https://github.com/GaijinEntertainment/daScript daslang-src
  git clone --depth=1 -b <pr-branch> https://github.com/borisbat/dasImgui dasImgui
"

# 4. Build daslang.
wsl.exe -d Ubuntu-24.04 -- bash -c "
  cd ~/ci-repro/daslang-src
  cmake --no-warn-unused-cli -B./build -DCMAKE_BUILD_TYPE:STRING=Release -DDAS_HV_DISABLED=OFF -DDAS_PUGIXML_DISABLED=OFF -DDAS_LLVM_DISABLED=ON -DDAS_GLFW_DISABLED=OFF -G Ninja
  cmake --build ./build --parallel --target daslang
"

# 5. daspkg install dasImgui globally (CI step: 'daspkg install --global').
wsl.exe -d Ubuntu-24.04 -- bash -c "
  ~/ci-repro/daslang-src/bin/daslang ~/ci-repro/daslang-src/utils/daspkg/main.das -- install ~/ci-repro/dasImgui --global
"

# 6. Run imgui2rst then sphinx-build, as CI does.
wsl.exe -d Ubuntu-24.04 -- bash -c "
  cd ~/ci-repro/daslang-src/modules/dasImgui
  ~/ci-repro/daslang-src/bin/daslang utils/imgui2rst.das -- --detail_output doc/source/stdlib/generated
  ~/sphinx-venv/bin/sphinx-build -W --keep-going -b html -d build/sphinx-cache doc/source build/site
"
```

**Caveat — PR merge ref**: this repro builds the BRANCH HEAD, not the merge ref. If CI's failure is caused by collisions with master changes that landed since you forked (e.g., duplicate RST anchors auto-merged into one file), this repro will silently miss it. Add a `cd ~/ci-repro/dasImgui && git fetch origin master && git merge origin/master` step before running imgui2rst when you suspect a merge-ref bug. See `github-actions-pr-merge-ref-sphinx-duplicate-target-local-passes-ci-fails`.

**Cost**: first run ~15 min (daslang Release build). Subsequent runs just iterate steps 5-6 (~30s for imgui2rst + sphinx).

Verified 2026-05-18 against daslang ced9f5175 + dasImgui PR #48; WSL reproduced CI's pipeline cleanly (modulo the merge-ref caveat above).

## Questions
- How do I reproduce the dasImgui docs CI build job locally in WSL Ubuntu without sudo?
