---
slug: cross-platform-version-probes-from-dascript
title: Cross-platform pattern in daslang for probing external tool versions and capturing stdout/stderr for things like compiler, Lua/LuaJIT, Mono, dotnet, and daslang version strings from daScript code?
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

Use daslib/fio run_and_capture(args, output) for version probes instead of shell-string popen. Build argv arrays so the call bypasses the shell on Unix and Windows, then take the first non-empty line of merged stdout/stderr. In dasProfile, runtime probes work for Lua (-v), LuaJIT (-v), Mono (--version), dotnet (--version), QuickJS (-h), and Quirrel (-v); daslang should use get_das_version() directly instead of spawning the binary. For values that are not reliably discoverable at runtime, generate a small text file from CMake and read it from daScript. The current dasProfile pattern is build/profile_build_info.txt copied to examples/profile/profile_build_info.txt, carrying CMAKE_CXX_COMPILER_ID/CMAKE_CXX_COMPILER_VERSION and configured fallback versions like Luau.

## Questions
- Cross-platform pattern in daslang for probing external tool versions and capturing stdout/stderr for things like compiler, Lua/LuaJIT, Mono, dotnet, and daslang version strings from daScript code?
