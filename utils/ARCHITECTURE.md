# utils - architecture

`utils/` holds the repo's tools: the programs that lint, format, test, serve and package daslang
code. This document defines the terms `REVIEW.md` (beside this file) binds a diff with. Contract:
`ARCHITECTURE_COMMON.md` (repo root).

## What a tool is {#tools}

A tool is a directory holding a program and the files only that program uses. A directory holds
a program when the program's entry file sits in it, or in a child folder of it when the directory
has a `README.md`.

A directory anywhere in the repo is a tool when `CMakeLists.txt` (beside this file) or the root
`CMakeLists.txt` builds or ships a program it holds, or when `CMakeLists.txt` (beside this file)
runs a `DAS_UTILS_TO_TEST` suite against that program. A directory under this folder is also a
tool when its `.das_package` declares a program with `release_main` or `release_program`, or
when a CMake target or a `.github/workflows/` step (repo root) starts a program it holds as the
program of its command.

## A tool's sources {#tool-sources}

A tool's sources are the files it answers for, so a change to any of them is a change to the
tool. They are three sets:

- every file under the tool's directory;
- every module under `common/` (beside this file) the tool requires - one `common/` module is a
  source of every tool that requires it;
- every module named in the `RULE_MODULES` list of the tool's `REVIEW.das`.

A `RULE_MODULES` list names the modules whose rule ids the tool gates (`lint/REVIEW.das` carries
one). A daslib module a tool merely requires is not one of its sources: most tools require
dozens, and the module's own folder reviews it.

## Arms {#arms}

An arm is one test case that a test runner executes and reports pass or fail for on its own, in
any language: a `dastest` `[test]` function and each `t |> run(...)` case inside it, a compile-fail
fixture carrying an `expect` line (dastest compares its error counts and reports the file as one
result), and each `test(...)` case of a JavaScript runner (`node:test`, Playwright).

An arm's load-bearing assertions are the ones that prove the change. An assertion that runs only
when the arm skips is not one of them.

## CI rows and pull-request lanes {#ci-rows}

A CI row is a workflow step whose command runs the arm, directly or through a process it spawns.
A pull-request lane is a CI job that runs on every pull request.

The utils suites run through one chain. `.github/workflows/extended_checks.yml` (repo root) runs
on every pull request and builds `run_extended_<role>`. `ci/CMakeLists.txt` (repo root) puts
`run_utils_tests` into `DAS_EXTENDED_CORE`, the group the `core` role builds. `run_utils_tests`
(`CMakeLists.txt`, beside this file) runs `dastest --test` on every `DAS_UTILS_TO_TEST` line and
`dastest --compile-only --test` on every `DAS_UTILS_TO_COMPILE` line. A compile-only line checks
that the file compiles; it executes no assertion.

An assertion no CI row can run needs something no pull-request lane's runner image provides: a
GPU, a host tool the image does not carry, a network service, a credential, or a model or data
file that neither the repo checkout nor the build provides. That list is closed, and whether a
row already runs the arm does not decide it. An arm that skips unless something the build can
produce is present - a target, an exe, a generated file - has assertions a CI row can run. An
arm that skips unless a host tool is present has assertions a CI row can run when a
pull-request lane's runner image carries that tool. One arm can hold assertions of both kinds.
