// Tutorial: Passing Arrays (and Tables) from C to daslang
//
// This tutorial demonstrates two ownership flavors for `array<T>` arguments,
// plus a `table<K;V>` example:
//
//   1. Borrowed array — backing data lives in a C-side buffer; daslang reads
//      it without taking ownership and is forbidden from resizing it.
//
//   2. Context-owned array — daslang allocates and grows the backing storage
//      on the context heap; C reads the result back through the das_array
//      struct and frees with das_array_clear.
//
//   3. Context-owned table — same pattern as (2), with key/value dispatch.
//
// All three rely on three pieces of C API added alongside this tutorial:
//   - das_context_allocate / _reallocate / _free  (raw context heap)
//   - das_array_init / _init_borrowed / _resize / _clear / _at
//   - das_table_init / _reserve / _find / _insert / _erase / _clear
//
// Build: link against libDaScript.
// Run:   from the project root so that the .das file path resolves correctly.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "daScript/daScriptC.h"

#define SCRIPT_NAME "/tutorials/integration/c/14_passing_arrays.das"

// Compile + simulate, returning the context (or NULL on failure).
static das_context * compile_and_simulate(
    const char * script_path,
    das_file_access * file_access,
    das_text_writer * tout,
    das_module_group * module_group,
    das_program ** out_program
) {
    *out_program = das_program_compile((char*)script_path, file_access, tout, module_group);
    int err_count = das_program_err_count(*out_program);
    if (err_count) {
        printf("Compilation failed with %d error(s)\n", err_count);
        for (int i = 0; i < err_count; i++)
            das_error_output(das_program_get_error(*out_program, i), tout);
        return NULL;
    }
    das_context * ctx = das_context_make(das_program_context_stack_size(*out_program));
    if (!das_program_simulate(*out_program, ctx, tout)) {
        printf("Simulation failed\n");
        das_context_release(ctx);
        return NULL;
    }
    return ctx;
}

int main(int argc, char ** argv) {
    (void)argc; (void)argv;

    das_initialize();

    das_text_writer  * tout         = das_text_make_printer();
    das_module_group * module_group = das_modulegroup_make();
    das_file_access  * file_access  = das_fileaccess_make_default();

    char script_path[512];
    das_get_root(script_path, sizeof(script_path));
    int chars_left = ((int)sizeof(script_path)) - ((int)strlen(script_path)) - 1;
    assert(chars_left >= 0);
    strncat(script_path, SCRIPT_NAME, (size_t)chars_left);

    das_program * program = NULL;
    das_context * ctx = compile_and_simulate(script_path, file_access, tout, module_group, &program);
    if (!ctx) goto cleanup;

    // -----------------------------------------------------------------------
    // 1. Borrowed array — daslang reads C-owned data, never resizes
    //
    // `int data[8]` lives in this stack frame. das_array_init_borrowed wraps
    // it as a non-owned view: lock=1, magic set, so any daslang attempt to
    // grow or delete the array panics. sum_array only reads, which is fine.
    // -----------------------------------------------------------------------
    {
        das_function * fn_sum = das_context_find_function(ctx, "sum_array");
        if (!fn_sum) { printf("'sum_array' not found\n"); goto done; }

        int data[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        das_array borrowed;
        das_array_init_borrowed(&borrowed, data, 8, 8);

        vec4f args[1];
        args[0] = das_result_array(&borrowed);

        vec4f ret = das_context_eval_with_catch(ctx, fn_sum, args);
        if (das_context_get_exception(ctx)) { printf("exception in sum_array\n"); goto done; }

        printf("sum_array([1..8]) = %d\n", das_argument_int(ret));
        // No cleanup: daslang never owned `data`.
    }

    // -----------------------------------------------------------------------
    // 2. Context-owned array — daslang grows the backing storage from empty
    //
    // We hand fill_squares an empty array; it calls `arr |> resize(n)`, which
    // routes through context.allocate / reallocate (the same heap path now
    // exposed via das_context_allocate). After the call, das_array.data points
    // into the context heap; we read it through das_array_at, then release
    // with das_array_clear.
    // -----------------------------------------------------------------------
    {
        das_function * fn_fill = das_context_find_function(ctx, "fill_squares");
        if (!fn_fill) { printf("'fill_squares' not found\n"); goto done; }

        das_array owned;
        das_array_init(&owned);

        vec4f args[2];
        args[0] = das_result_array(&owned);
        args[1] = das_result_int(6);

        das_context_eval_with_catch(ctx, fn_fill, args);
        if (das_context_get_exception(ctx)) { printf("exception in fill_squares\n"); goto done; }

        printf("fill_squares(_, 6) -> size=%u, contents=", owned.size);
        for (uint32_t i = 0; i < owned.size; i++) {
            int v = *(int*)das_array_at(&owned, i, sizeof(int));
            printf("%s%d", i ? "," : "", v);
        }
        printf("\n");

        das_array_clear(ctx, &owned, sizeof(int));
    }

    // -----------------------------------------------------------------------
    // 3. Context-owned table — count occurrences via daslang
    //
    // Same lifecycle as (2): init, hand to daslang, read back, clear.
    // The C side uses das_table_find for spot lookups; das_table_clear must
    // be told the key base type and value size so it can free the contiguous
    // (values | keys | hashes) heap block daslang allocated.
    // -----------------------------------------------------------------------
    {
        das_function * fn_count = das_context_find_function(ctx, "count_values");
        if (!fn_count) { printf("'count_values' not found\n"); goto done; }

        // Input data: a borrowed array of ints to count.
        int input[10] = { 7, 3, 7, 1, 3, 7, 1, 1, 1, 3 };
        das_array data_view;
        das_array_init_borrowed(&data_view, input, 10, 10);

        // Output table: empty, daslang fills it.
        das_table counts;
        das_table_init(&counts);

        vec4f args[2];
        args[0] = das_result_table(&counts);
        args[1] = das_result_array(&data_view);

        das_context_eval_with_catch(ctx, fn_count, args);
        if (das_context_get_exception(ctx)) { printf("exception in count_values\n"); goto done; }

        // Spot-check a few keys via das_table_find. Each find returns either
        // a pointer into the value slot or NULL on miss.
        int keys_to_check[] = { 1, 3, 7, 99 };
        for (int i = 0; i < 4; i++) {
            int k = keys_to_check[i];
            int * pv = (int*)das_table_find(ctx, &counts, DAS_TYPE_INT, &k, sizeof(int));
            if (pv) printf("counts[%d] = %d\n", k, *pv);
            else    printf("counts[%d] = (missing)\n", k);
        }

        das_table_clear(ctx, &counts, DAS_TYPE_INT, sizeof(int));
        // data_view borrows `input`; nothing to free.
    }

done:
    das_context_release(ctx);

cleanup:
    das_program_release(program);
    das_fileaccess_release(file_access);
    das_modulegroup_release(module_group);
    das_text_release(tout);

    das_shutdown();
    return 0;
}
