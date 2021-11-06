# DaScript testing framework

## Introduction

Inspired by [Golang testing framework](https://pkg.go.dev/testing), this framework provides a simple way to write unit tests for your scripts.

## Usage

- `dascript dastest.das -- --test folderWithScriptsOrSingleScript [--test anotherPath]`

### dastest.das arguments
- `--test`: Path to the folder with scripts or single script to test

#### Internal arguments
- `--run`: Path to the single script file to run tests in multiprocessor mode
- `--one-process`: Run tests in one process, useful for debugging of the framework
