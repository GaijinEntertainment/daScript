options gen2
options persistent_heap

require dastest/testing_boost public
require daslib/fio
require strings

// Two checks of modules/dasLLAMA/REVIEW.das over fixture files written under the test's temp dir - the HOW_TO check and
// the unlocked-queue-call check: one file a defect, each red on its own finding, and a clean one green. The gate runs as a
// child: REVIEW.das is a program root whose checks are private to it, and its `-- --howto <doc>...` and
// `-- --queue-calls <file>...` rails read only the named files.

struct private GateRun {
    rc : int
    output : string
}

def private run_gate(rail, file : string) : GateRun {
    let args <- get_command_line_arguments()
    let exe = empty(args) ? "" : args[0]
    var output = ""
    let rc = run_and_capture([exe, "-dasroot", get_das_root(), "{get_das_root()}/modules/dasLLAMA/REVIEW.das", "--", rail, file], output)
    return GateRun(rc = rc, output = output)
}

def private fixture(name, text : string) : string {
    let path = path_join(test_temp_dir(), name)
    fwrite(path, text)
    return path
}

let private CLEAN = "# Fixture\n\nThe preamble names no step.\n\n## 1. Read the charter\n\nOpen `ARCHITECTURE_MEDIA_ASR.md#gigaam-planes` and follow it.\n\n## Traps\n\nA trap section carries no number and needs no anchor.\n"

def private red_on(t : T?; name, text, fragment : string; rail : string = "--howto") {
    let run = run_gate(rail, fixture(name, text))
    t |> equal(run.rc, 1, "{name}: the gate's exit code\n{run.output}")
    t |> success(find(run.output, fragment) >= 0, "{name}: the finding names '{fragment}'\n{run.output}")
    t |> success(find(run.output, "1 finding(s)") >= 0, "{name}: the one defect is the one finding\n{run.output}")
}

[test]
def test_howto_gate(t : T?) {
    t |> run("a clean HOW_TO passes") @(t : T?) {
        let run = run_gate("--howto", fixture("clean.md", CLEAN))
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("a past-tense marker is a finding") @(t : T?) {
        red_on(t, "past.md", "{CLEAN}\nThe loader was slow.\n", "past-tense marker 'was'")
    }
    t |> run("a 20YY-MM date is a finding") @(t : T?) {
        red_on(t, "date.md", "{CLEAN}\nMeasured in 2026-10.\n", "a date - a HOW_TO is timeless")
    }
    t |> run("a #NNN number is a finding") @(t : T?) {
        red_on(t, "pr.md", "{CLEAN}\nSee #4370 for the reason.\n", "a PR or issue number")
    }
    t |> run("a numbered step that cites no architecture anchor is a finding") @(t : T?) {
        red_on(t, "uncited.md", "{CLEAN}\n## 2. Run the test\n\nRun it through the runner.\n", "a numbered step that cites no")
    }
}

//! the call's open paren, spliced into the fixtures' banned spellings so the folder walk over this file never reads a call
let private OPEN = "("

let private QUEUE_CLEAN = "def readback(raw : VkCommandBuffer) \{\n    cmd_one_shot() $(r; var _h) \{\n        copy_rows(r)\n    \}\n\}\n// run_cmd_sync{OPEN}device, pool, queue) in a comment is not a call\n"
[test]
def test_locked_queue_gate(t : T?) {
    t |> run("a source that submits through the locked one-shot passes") @(t : T?) {
        let run = run_gate("--queue-calls", fixture("locked.das", QUEUE_CLEAN))
        t |> equal(run.rc, 0, "the gate's exit code\n{run.output}")
    }
    t |> run("run_cmd_sync is a finding") @(t : T?) {
        red_on(t, "sync.das", "{QUEUE_CLEAN}run_cmd_sync{OPEN}g_gpu.device, g_gpu.pool, g_gpu.queue) $(cmd) \{\n\}\n", "without the shared device's queue lock", "--queue-calls")
    }
    t |> run("a raw vkQueueSubmit is a finding") @(t : T?) {
        red_on(t, "submit.das", "{QUEUE_CLEAN}vk_check(vkQueueSubmit{OPEN}q, 1u, si, fence), null)\n", "without the shared device's queue lock", "--queue-calls")
    }
    t |> run("a raw vkQueueWaitIdle is a finding") @(t : T?) {
        red_on(t, "wait.das", "{QUEUE_CLEAN}vkQueueWaitIdle{OPEN}q)\n", "without the shared device's queue lock", "--queue-calls")
    }
    t |> run("a raw vkDeviceWaitIdle is a finding") @(t : T?) {
        red_on(t, "devwait.das", "{QUEUE_CLEAN}vkDeviceWaitIdle{OPEN}d)\n", "without the shared device's queue lock", "--queue-calls")
    }
}
