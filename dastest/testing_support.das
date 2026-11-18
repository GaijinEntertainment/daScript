options gen2
options indenting = 4
options no_aot

module testing_support shared

require daslib/rtti
require strings
require math
require daslib/fio

var private g_test_temp_dir : string

//! A directory only this context writes to - created on the first
//! call, removed with everything in it when the context ends. A
//! thread or job context gets its own.
def public test_temp_dir() : string {
    if (empty(g_test_temp_dir)) {
        let made = create_temp_directory_result("dastest_")
        if (!(made is value)) {
            panic("test_temp_dir: {unsafe(made.error)}")
        }
        g_test_temp_dir = unsafe(made.value)
    }
    return g_test_temp_dir
}

[finalize]
def private remove_test_temp_dir {
    rmdir_rec(g_test_temp_dir)
}

//! The panic a block raises, as text ("" when it returns): a refusal
//! cell reads the message it holds.
def public panic_text(blk : block<() : void>) : string {
    var why = ""
    try {
        invoke(blk)
    } recover {
        why = this_context().last_exception |> rtrim
    }
    return why
}

//! The relative-tolerance float compare of a fastmath (GPU,
//! flattened) result against its oracle.
def public fapprox(x, y : float) : bool => abs(x - y) <= 1e-4 * max(1.0, max(abs(x), abs(y)))

//! a Debug host runs a child several times slower, so its deadline
//! triples there, never past five minutes
def public child_deadline(seconds : float) : float =>
    shared_module_extension() == "_debug.shared_module" ? min(seconds * 3.0, 300.0) : seconds
