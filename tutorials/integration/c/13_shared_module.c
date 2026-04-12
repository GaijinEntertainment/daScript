// Tutorial: Shared Modules — Global Module Registry
//
// This tutorial demonstrates the `module X shared` mechanism and how it
// solves a common integration problem: making a daslang module available
// across independent compilations without requiring every FileAccess to
// carry the module's source file.
//
// The problem:
//   When you introduce a module file via das_fileaccess_introduce_file(),
//   it is only visible to compilations that use THAT FileAccess.  A second
//   FileAccess (or a script in a different directory) cannot find the file
//   and fails with "missing prerequisite; file not found".
//
// The solution:
//   Declare the module as `module my_helpers shared`.  When it is first
//   compiled as a dependency, the compiler promotes it to the global module
//   registry.  Subsequent compilations find it there — no file needed.
//
// Build: link against libDaScript (C++ linker required).
// Run:   no external files needed; all script source is embedded.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "daScript/daScriptC.h"

// -----------------------------------------------------------------------
// Inline script sources
// -----------------------------------------------------------------------

// A helper module WITHOUT the `shared` keyword.
static const char * MODULE_NOT_SHARED =
    "options gen2\n"
    "module my_helpers\n"
    "\n"
    "[export]\n"
    "def double_it(x : int) : int {\n"
    "    return x * 2\n"
    "}\n"
    "\n"
    "[export]\n"
    "def greet(name : string) : string {\n"
    "    return \"Hello from my_helpers, {name}!\"\n"
    "}\n"
;

// The same module WITH the `shared` keyword.
// When compiled as a dependency, the compiler promotes it to the
// global module registry so it persists across FileAccess instances.
static const char * MODULE_SHARED =
    "options gen2\n"
    "module my_helpers shared\n"
    "\n"
    "[export]\n"
    "def double_it(x : int) : int {\n"
    "    return x * 2\n"
    "}\n"
    "\n"
    "[export]\n"
    "def greet(name : string) : string {\n"
    "    return \"Hello from my_helpers, {name}!\"\n"
    "}\n"
;

// A minimal loader script.  Its only purpose is to trigger compilation
// of the shared module as a dependency.  The `require` causes the
// compiler to parse my_helpers.das, see `module my_helpers shared`,
// and promote it to the global registry.
static const char * LOADER_SCRIPT =
    "options gen2\n"
    "require my_helpers\n"
;

// A user script that uses the module.
static const char * USER_SCRIPT =
    "options gen2\n"
    "require my_helpers\n"
    "\n"
    "[export]\n"
    "def test() : int {\n"
    "    let val = double_it(21)\n"
    "    print(\"double_it(21) = {val}\\n\")\n"
    "    let msg = greet(\"tutorial 13\")\n"
    "    print(\"{msg}\\n\")\n"
    "    return val\n"
    "}\n"
;

// -----------------------------------------------------------------------
// Part 1: The Problem — non-shared module across FileAccess boundaries
//
// Demonstrates that a module introduced into one FileAccess is NOT
// automatically available to compilations using a different FileAccess.
// -----------------------------------------------------------------------
static void part1_problem(void) {
    printf("=== Part 1: The Problem -- non-shared module across FileAccess boundaries ===\n\n");

    // --- 1a: Both files in the same FileAccess — works fine ---
    {
        das_text_writer  * tout    = das_text_make_printer();
        das_module_group * libgrp  = das_modulegroup_make();
        das_file_access  * fa      = das_fileaccess_make_default();

        das_fileaccess_introduce_file(fa, "my_helpers.das", MODULE_NOT_SHARED, 0);
        das_fileaccess_introduce_file(fa, "user_script.das", USER_SCRIPT, 0);

        das_program * program = das_program_compile("user_script.das", fa, tout, libgrp);
        int err_count = das_program_err_count(program);
        printf("Part 1a: same FileAccess -- compilation %s (%d error%s)\n\n",
               err_count ? "FAILED" : "succeeded", err_count, err_count == 1 ? "" : "s");

        das_program_release(program);
        das_fileaccess_release(fa);
        das_modulegroup_release(libgrp);
        das_text_release(tout);
    }

    // --- 1b: Module NOT in this FileAccess — fails ---
    {
        das_text_writer  * tout    = das_text_make_writer();
        das_module_group * libgrp  = das_modulegroup_make();
        das_file_access  * fa      = das_fileaccess_make_default();

        // Only introduce the user script — NOT the module.
        das_fileaccess_introduce_file(fa, "user_script.das", USER_SCRIPT, 0);

        das_program * program = das_program_compile("user_script.das", fa, tout, libgrp);
        int err_count = das_program_err_count(program);
        printf("Part 1b: different FileAccess, module file missing -- compilation failed (%d error%s):\n",
               err_count, err_count == 1 ? "" : "s");
        for (int i = 0; i < err_count; i++) {
            das_error * error = das_program_get_error(program, i);
            char buf[1024];
            das_error_report(error, buf, sizeof(buf));
            printf("  error %d: %s\n", i, buf);
        }
        printf("\n");

        das_program_release(program);
        das_fileaccess_release(fa);
        das_modulegroup_release(libgrp);
        das_text_release(tout);
    }
}

// -----------------------------------------------------------------------
// Part 2: The Solution — `module my_helpers shared`
//
// Compile a loader script that `require`s the shared module.  The
// module is promoted to the global registry as a dependency.  After
// that, any compilation — even with a completely fresh FileAccess —
// can `require my_helpers` because it lives in the global registry.
// -----------------------------------------------------------------------
static void part2_solution(void) {
    printf("=== Part 2: The Solution -- module my_helpers shared ===\n\n");

    // --- 2a: Compile the loader to promote my_helpers to the global registry ---
    //
    // Promotion happens when a shared module is compiled as a DEPENDENCY
    // (via `require`).  The loader script exists solely to trigger this.
    {
        das_text_writer  * tout    = das_text_make_printer();
        das_module_group * libgrp  = das_modulegroup_make();
        das_file_access  * fa      = das_fileaccess_make_default();

        das_fileaccess_introduce_file(fa, "my_helpers.das", MODULE_SHARED, 0);
        das_fileaccess_introduce_file(fa, "loader.das", LOADER_SCRIPT, 0);

        das_program * program = das_program_compile("loader.das", fa, tout, libgrp);
        int err_count = das_program_err_count(program);
        if (err_count) {
            printf("Part 2a: loader compilation FAILED (%d errors) -- unexpected\n", err_count);
            for (int i = 0; i < err_count; i++) {
                das_error * error = das_program_get_error(program, i);
                char buf[1024];
                das_error_report(error, buf, sizeof(buf));
                printf("  error %d: %s\n", i, buf);
            }
            das_program_release(program);
            das_fileaccess_release(fa);
            das_modulegroup_release(libgrp);
            das_text_release(tout);
            return;
        }

        printf("Part 2a: loader compiled -- 'my_helpers' promoted to global module registry\n\n");

        // Release everything.  The promoted module survives because
        // Module::promoteToBuiltin() transferred ownership to the
        // global linked list (builtIn=true skips deletion).
        das_program_release(program);
        das_fileaccess_release(fa);
        das_modulegroup_release(libgrp);
        das_text_release(tout);
    }

    // --- 2b: Fresh FileAccess, no module file — compilation succeeds ---
    {
        das_text_writer  * tout    = das_text_make_printer();
        das_module_group * libgrp  = das_modulegroup_make();
        das_file_access  * fa      = das_fileaccess_make_default();

        // Only introduce the user script — NOT the module.
        // The compiler finds my_helpers in the global registry.
        das_fileaccess_introduce_file(fa, "user_script.das", USER_SCRIPT, 0);

        das_program * program = das_program_compile("user_script.das", fa, tout, libgrp);
        int err_count = das_program_err_count(program);
        if (err_count) {
            printf("Part 2b: compilation FAILED (%d errors) -- unexpected\n", err_count);
            for (int i = 0; i < err_count; i++) {
                das_error * error = das_program_get_error(program, i);
                char buf[1024];
                das_error_report(error, buf, sizeof(buf));
                printf("  error %d: %s\n", i, buf);
            }
            das_program_release(program);
            das_fileaccess_release(fa);
            das_modulegroup_release(libgrp);
            das_text_release(tout);
            return;
        }

        printf("Part 2b: different FileAccess, no module file -- compilation succeeded!\n");

        // Simulate and call test() to prove the shared module works.
        das_context * ctx = das_context_make(das_program_context_stack_size(program));
        if (!das_program_simulate(program, ctx, tout)) {
            printf("Simulation failed\n");
            das_context_release(ctx);
            das_program_release(program);
            das_fileaccess_release(fa);
            das_modulegroup_release(libgrp);
            das_text_release(tout);
            return;
        }

        das_function * fn_test = das_context_find_function(ctx, "test");
        if (fn_test) {
            vec4f ret = das_context_eval_with_catch(ctx, fn_test, NULL);
            char * ex = das_context_get_exception(ctx);
            if (ex) {
                printf("Exception: %s\n", ex);
            } else {
                int val = das_argument_int(ret);
                printf("  test() returned: %d\n", val);
            }
        } else {
            printf("  test() function not found\n");
        }

        das_context_release(ctx);
        das_program_release(program);
        das_fileaccess_release(fa);
        das_modulegroup_release(libgrp);
        das_text_release(tout);
    }
}

// -----------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------
int main(int argc, char ** argv) {
    (void)argc; (void)argv;

    das_initialize();

    part1_problem();
    part2_solution();

    // das_shutdown() calls Module::ClearSharedModules(), freeing the
    // promoted my_helpers module.
    das_shutdown();
    return 0;
}
