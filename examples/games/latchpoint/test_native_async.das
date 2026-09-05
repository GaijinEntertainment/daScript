options gen2
options persistent_heap
options rtti
require dastest/testing_boost public
require lp_dialogue
require daslib/jobque_boost
require main
require lp_microphone
require lp_chatter

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
    suite |> run("silent capture is rejected without starting recognition") @(t : T?) {
        t |> success(!recording_has_signal([0.0, 0.0, 0.0]))
        t |> success(recording_has_signal([0.0, 0.05, -0.02]))
    }
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

[test]
def test_empty_recognition(t : T?) {
    var output = unsafe(stream_create())
    var done = job_status_create()
    inference_stream = unsafe(reinterpret<uint64>(output))
    inference_done = unsafe(reinterpret<uint64>(done))
    var response = InferenceResponse(code = 200, text = "   ")
    output |> push_archive(response)
    conversation_stage = "asr"
    conversation_resident = 1
    pending_job = "native"
    discard_inference = false
    editing = false
    poll_dialogue()
    t |> success(empty(pending_job) && !editing)
    t |> equal(conversation_stage, "failed")
}

[test]
def test_finished_job_can_be_released_at_shutdown(t : T?) {
    var output = unsafe(stream_create())
    var done = job_status_create()
    var response = InferenceResponse(code = 200, text = "finished")
    output |> push_archive(response)
    var job = InferenceJob(stream = unsafe(reinterpret<uint64>(output)), done = unsafe(reinterpret<uint64>(done)))
    release_inference_job(job)
    t |> equal(job.stream, 0ul)
    t |> equal(job.done, 0ul)
    t |> success(!framebuffer_drawable(1280, 800, 0, 0), "a minimized framebuffer cannot render")
    t |> success(framebuffer_drawable(1280, 800, 2560, 1600), "a visible framebuffer can render")
}

[test]
def test_pending_job_is_handed_to_nonblocking_reaper(t : T?) {
    var output = unsafe(stream_create())
    var done = job_status_create()
    done |> append(1)
    output |> add_ref
    done |> add_ref
    var job = InferenceJob(stream = unsafe(reinterpret<uint64>(output)), done = unsafe(reinterpret<uint64>(done)))
    release_inference_job(job)
    t |> equal(job.stream, 0ul, "shutdown returns ownership immediately")
    t |> equal(job.done, 0ul, "shutdown does not join the request timeout")
    var response = InferenceResponse(code = 200, text = "late")
    output |> push_archive(response)
    output |> release()
    done |> notify_and_release()
}

[test]
def test_minimized_frame_drains_without_launching_ambient_followup(t : T?) {
    chatter_stage = "generating"
    chatter_topic = "test"
    chatter_world = chatter_world_key()
    chatter_epoch = player_turn_epoch
    chatter_started = 0.0
    chatter_resident = 2
    chatter_job = InferenceJob()
    var packet = InferenceResponse(code = 200,
        text = "\{\"choices\":[\{\"finish_reason\":\"stop\",\"message\":\{\"content\":\"A quiet test line.\"\}\}]\}")
    finish_chatter_packet(packet, 1.0, false)
    t |> equal(chatter_stage, "")
    t |> equal(chatter_job.done, 0ul, "a minimized frame does not start speech synthesis")
}
