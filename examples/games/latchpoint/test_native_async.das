options gen2
options persistent_heap
options rtti
require dastest/testing_boost public
require lp_dialogue
require daslib/jobque_boost
require main

[test]
def test_cancelled_response(suite : T?) {
    suite |> run("cancelled response is drained without applying its proposed action") @(t : T?) {
        var output = unsafe(stream_create())
        var done = job_status_create()
        inference_stream = unsafe(reinterpret<uint64>(output))
        inference_done = unsafe(reinterpret<uint64>(done))
        var response = InferenceResponse(code = 200, text = "\{\"intent\":\"loan_actuator\"\}")
        output |> push_archive(response)
        pending_job = "native"
        conversation_stage = "thinking"
        conversation_resident = 0
        discard_inference = true
        world.brack_actuator_lent = false
        poll_dialogue()
        t |> success(!inference_busy())
        t |> success(empty(pending_job))
        t |> success(!world.brack_actuator_lent)
    }
}

[test]
def test_busy_input(suite : T?) {
    suite |> run("a retry while the old request drains does not erase typed text") @(t : T?) {
        var output = unsafe(stream_create())
        var done = job_status_create()
        inference_stream = unsafe(reinterpret<uint64>(output))
        inference_done = unsafe(reinterpret<uint64>(done))
        pending_job = ""
        editing = true
        input_text = "Please follow me."
        conversation_resident = 1
        save_valid = true
        send_typed()
        t |> equal(input_text, "Please follow me.")
        t |> success(editing)
        inference_stream = 0ul
        inference_done = 0ul
        unsafe { stream_remove(output); job_status_remove(done) }
    }
}
