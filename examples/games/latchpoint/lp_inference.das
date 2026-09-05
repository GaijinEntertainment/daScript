options gen2
options persistent_heap

require openai/openai_common
require openai/openai_audio
require daslib/fio
require daslib/jobque_boost
require live/live_vars public
require live_host

[safe_when_uninitialized]
struct InferenceResponse {
    code : int = 0
    text : string
    audio : array<uint8>
    error : string
}

var @live inference_stream = 0ul
var @live inference_done = 0ul

struct InferenceJob {
    stream : uint64
    done : uint64
}

def inference_job_ready(job : InferenceJob) : bool {
    return job.done != 0ul && unsafe(reinterpret<JobStatus?>(job.done)).isReady
}

def inference_timeout(route : string) : int {
    return route == "/chat/completions" ? 90 : 25
}

def inference_busy() : bool {
    return inference_done != 0ul
}

def launch_inference(route, payload : string; binary : bool = false) {
    return if (inference_busy())
    let job = launch_inference_job(route, payload, binary, inference_timeout(route))
    inference_stream = job.stream
    inference_done = job.done
}

def launch_inference_job(route, payload : string; binary : bool; timeout : int) : InferenceJob {
    let delete_upload = route == "/audio/transcriptions" && normalize(payload) == normalize(path_join(get_das_root(), "logs/latchpoint-input/microphone.wav"))
    var output = unsafe(stream_create())
    var done = job_status_create()
    done |> append(1)
    let job = InferenceJob(stream = unsafe(reinterpret<uint64>(output)), done = unsafe(reinterpret<uint64>(done)))
    new_thread() <| @capture(= output, = done, = route, = payload, = binary, = timeout, = delete_upload) {
        var result = InferenceResponse()
        try {
            let client = OpenAIClient(base_url = "http://127.0.0.1:18082/v1", timeout_seconds = timeout)
            if (route == "/audio/transcriptions") {
                let response = transcribe(client, "parakeet", payload)
                result.code = response.ok ? 200 : response.error.status
                result.text = clone_string(response.text)
                result.error = clone_string(response.error.message)
            } elif (binary) {
                var response <- post_for_bytes(client, route, payload)
                result.code = response.status
                result.audio <- response.bytes
                result.error = clone_string(response.error.message)
            } else {
                let response = post_json(client, route, payload)
                result.code = response.status
                result.text = clone_string(response.body)
                result.error = clone_string(response.error.message)
            }
        } recover {
            result.error = "Inference request failed."
        }
        if (delete_upload) { remove(payload) }
        output |> push_archive(result)
        output |> release()
        done |> notify_and_release()
    }
    return job
}

//! Collect only after inference_job_ready; the returned packet owns its decoded payload.
def collect_inference_job(job : InferenceJob) : InferenceResponse {
    var done = unsafe(reinterpret<JobStatus?>(job.done))
    var output = unsafe(reinterpret<Stream?>(job.stream))
    var result = InferenceResponse()
    output |> try_pop_archive() $(var response : InferenceResponse) { result := response }
    unsafe { stream_remove(output); job_status_remove(done) }
    return <- result
}

def poll_inference(blk : block<(var response : InferenceResponse) : void>) : bool {
    return false if (!inference_busy())
    var done = unsafe(reinterpret<JobStatus?>(inference_done))
    return false if (!done.isReady)
    var output = unsafe(reinterpret<Stream?>(inference_stream))
    inference_done = 0ul
    inference_stream = 0ul
    output |> try_pop_archive() $(var response : InferenceResponse) { invoke(blk, response) }
    unsafe {
        stream_remove(output)
        job_status_remove(done)
    }
    return true
}
