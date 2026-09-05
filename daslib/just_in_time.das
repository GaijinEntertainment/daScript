options gen2
options indenting = 4

module just_in_time shared private

//! JIT compilation configuration.
//!
//! Bootstraps the Just-In-Time compiler by requiring the platform-appropriate
//! LLVM-based JIT backend module - through the `llvm` witness guard, so a program
//! requiring this module still compiles, and runs interpreted, in a build without dasLLVM.

require ?llvm llvm/daslib/llvm_macro   // nolint:STYLE030,LINT019 — a side-effect require: the module's presence is what arms the JIT
