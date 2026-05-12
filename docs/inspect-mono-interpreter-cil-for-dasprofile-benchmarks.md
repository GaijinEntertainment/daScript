---
slug: inspect-mono-interpreter-cil-for-dasprofile-benchmarks
title: How can I inspect what Mono --interpreter is actually running for a suspiciously fast C# benchmark, and tell whether mcs optimized the hot path away?
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

For dasProfile C# benchmarks, start with the exact source and benchmark shape before assuming Mono interpreter magic. In tests/cs/sha256.cs, Main calls profile(1, "sha256", ...), so both Mono and .NET run only one timed iteration; that alone can make the result look suspicious. The Mono path in config.das is just: mcs -optimize+ <source> -out:test.exe, then mono --interpreter test.exe. To inspect what runs, compile the source yourself and disassemble the CIL with monodis. For sha256.cs, the optimized and non-optimized IL differ only trivially (e.g. string.Empty vs ldstr ""); the hash loop is still present, and ComputeHash still calls Encoding.GetBytes, LINQ Skip/Take/Reverse/ToArray, BitConverter.ToUInt32, the local Omega/Sigma helpers, and StringBuilder. That means mcs did not optimize the algorithm away. Mono interpreter does an internal transform pass over CIL into its own bytecode; mono --interpreter --stats reports transform, constant-fold, and inline counts, but release builds do not expose a nice stable CLI dump of the transformed interpreter IR. Practical workflow: 1) inspect source for benchmark count/workload, 2) monodis the assembly, 3) compare mcs -optimize+ vs -optimize- IL, 4) use mono --interpreter --stats for coarse transform info.

## Questions
- How can I inspect what Mono --interpreter is actually running for a suspiciously fast C# benchmark, and tell whether mcs optimized the hot path away?
