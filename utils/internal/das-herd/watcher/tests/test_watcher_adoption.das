options gen2
options rtti

require dastest/testing_boost public
require ../watcher_core.das
require ../herd_sessions.das
require dashv/dashv_boost
require daslib/fio
require daslib/strings_boost
require strings

def private test_compiler : string {
    let root = get_das_root()
    var compiler = path_join(root, "bin/Release/daslang.exe")
    if (!fexist(compiler)) {
        let args <- get_command_line_arguments()
        compiler = empty(args) ? "daslang.exe" : args[0]
    }
    return compiler
}

def private pump_until(timeout_ms : int64; blk : block<() : bool>) : bool {
    let started = ref_time_ticks()
    while (get_time_nsec(started) / 1000000l < timeout_ms) {
        watcher_tick()
        herd_tick()
        if (invoke(blk)) return true
        sleep(5u)
    }
    return false
}

// Just enough of the host stamp to reach a host from a test.
struct private ProbeStamp {
    port : int
    token : string
}

// Connects to a host with its stamp credentials and orders it to die -
// simulating a host crash underneath a live watcher.
class private HostKiller : HvWebSocketClient {
    token : string
    shutdown_sent : bool
    def override onOpen {
        send("\{\"op\":\"hello\",\"token\":{sprint_json(token, false)},\"protocol\":1\}")
    }
    def override onClose {}
    def override onMessage(msg : string#) {
        if (find(msg, "\"type\":\"stat\"") >= 0 && !shutdown_sent) {
            shutdown_sent = true
            send("\{\"op\":\"shutdown\"\}")
        }
    }
}

class private PortBlocker : HvWebServer {
    def override onInit {}
    def override onTick {}
}

[test]
def test_watcher_host_adoption(t : T?) {
    if (get_platform_name() != "windows") return
    adopt_survives_watcher_restart(t)
    adopt_dead_host_folds_to_exited(t)
    adopt_host_death_fails_one_session(t)
    adopt_bind_collision_retries(t)
}

def private adopt_survives_watcher_restart(t : T?) {  // nolint:STYLE038 — one linear launch/restart/adopt/teardown script; the stages share the launched session
    t |> run("host-backed session survives a watcher restart and resurrects as running") <| @(t : T?) {
        let temp = create_temp_directory_result("dasherd_adopt_")
        if (!(temp is value)) {
            t |> failure("could not create temporary test directory")
            return
        }
        let root = temp as value
        let sessions_root = path_join(root, "sessions")
        let ptyhost_root = path_join(root, "ptyhosts")
        let registry_path = path_join(root, "_registry.json")
        let profiles_path = path_join(root, "profiles.json")
        var inscope host_prefix <- [test_compiler(),
            path_join(get_das_root(), "utils/internal/das-herd/ptyhost/main.das"), "--"]

        t |> equal(watcher_init(sessions_root), "")
        t |> equal(watcher_configure_ptyhosts(ptyhost_root, host_prefix), "")
        t |> equal(herd_init(registry_path, profiles_path), "")
        var herd_error : string
        let herd_id = herd_create(HerdCreateRequest(name = "adopt probe",
            profile_id = "shell", worktree_path = root), herd_error)
        t |> success(!empty(herd_id), herd_error)

        var inscope request = WatcherLaunchRequest(cwd = root,
            display_name = "adoption probe", kind = "shell",
            herd_session_id = herd_id, columns = 100, rows = 30)
        request.argv <- ["cmd.exe", "/q"]
        let launched = watcher_launch(request)
        t |> success(launched.ok, launched.error)
        let session_id = launched.session_id
        t |> equal(herd_link_pty(herd_id, session_id), "")
        var info = WatcherSessionInfo()
        t |> success(watcher_get_session_info(session_id, info))
        t |> equal(info.state, "running")
        t |> success(info.pid != 0, "host reported the child pid")
        t |> success(fexist(path_join(ptyhost_root, "{session_id}.host.json")),
            "launch left a host discovery stamp")

        t |> equal(watcher_input(session_id, "echo ADOPT-A\r\n"), "")
        t |> success(pump_until(15000l)
                $ => find(watcher_output_text(session_id), "ADOPT-A") >= 0,
            "child echoed through the detached host")
        t |> success(fexist(path_join(ptyhost_root, "{session_id}.lock")),
            "host holds its liveness lock")
        var before_restart = WatcherSessionInfo()
        t |> success(watcher_get_session_info(session_id, before_restart))

        // The "restart": tear the watcher-side state down WITHOUT terminating.
        // The detached host (and cmd.exe inside it) must keep running.
        herd_shutdown()
        watcher_shutdown()
        t |> equal(watcher_init(sessions_root), "")
        t |> equal(watcher_configure_ptyhosts(ptyhost_root, host_prefix), "")
        t |> equal(watcher_adopt_hosts(), 1)
        t |> equal(herd_init(registry_path, profiles_path), "")

        var adopted = WatcherSessionInfo()
        t |> success(watcher_get_session_info(session_id, adopted),
            "adopted session keeps its identity")
        t |> equal(adopted.state, "running")
        var inscope record = HerdSession()
        t |> success(herd_get(herd_id, record))
        t |> equal(record.state, "running")
        t |> equal(record.reason, "", "resurrected record carries no watcher_restart scar")

        t |> success(pump_until(15000l)
                $ => find(watcher_output_text(session_id), "ADOPT-A") >= 0,
            "journal replay restored pre-restart output")
        var replayed = WatcherSessionInfo()
        t |> success(watcher_get_session_info(session_id, replayed))
        t |> success(replayed.output_bytes >= before_restart.output_bytes,
            "byte offsets continue the journal numbering")
        t |> equal(watcher_input(session_id, "echo ADOPT-B\r\n"), "")
        t |> success(pump_until(15000l)
                $ => find(watcher_output_text(session_id), "ADOPT-B") >= 0,
            "adopted session still accepts input")

        var pty_to_terminate : string
        t |> equal(herd_close(herd_id, pty_to_terminate), "")
        t |> equal(pty_to_terminate, session_id)
        t |> equal(watcher_terminate(session_id, "test_cleanup"), "")
        t |> success(pump_until(15000l) $ {
                var closing = WatcherSessionInfo()
                return (watcher_get_session_info(session_id, closing)
                    && closing.state == "exited" && closing.drained)
            }, "terminate folds back through the host")
        t |> success(pump_until(15000l)
                $ => fexist(path_join(path_join(ptyhost_root, "archive"),
                    "{session_id}.host.json")),
            "released host archived its discovery stamp")
        t |> success(fexist(path_join(path_join(ptyhost_root, "archive"),
                "{session_id}.log")),
            "host log archived beside the journal")
        t |> success(!fexist(path_join(ptyhost_root, "{session_id}.lock")),
            "liveness lock left no debris")
        watcher_shutdown()
        herd_shutdown()
    }
}

def private adopt_dead_host_folds_to_exited(t : T?) {
    t |> run("a dead host's exit stamp folds into an exited, replayable session") <| @(t : T?) {
        let temp = create_temp_directory_result("dasherd_deadfold_")
        if (!(temp is value)) {
            t |> failure("could not create temporary test directory")
            return
        }
        let root = temp as value
        let sessions_root = path_join(root, "sessions")
        let ptyhost_root = path_join(root, "ptyhosts")
        var inscope host_prefix <- [test_compiler(),
            path_join(get_das_root(), "utils/internal/das-herd/ptyhost/main.das"), "--"]
        t |> equal(watcher_init(sessions_root), "")
        t |> equal(watcher_configure_ptyhosts(ptyhost_root, host_prefix), "")

        let session_id = "s999000111_7"
        let session_dir = path_join(sessions_root, session_id)
        var mkdir_error : string
        t |> success(mkdir_rec(session_dir, mkdir_error), mkdir_error)
        var inscope persisted = WatcherLaunchRequest(cwd = root,
            display_name = "dead probe", kind = "shell",
            herd_session_id = "h_dead", columns = 100, rows = 30)
        persisted.argv <- ["cmd.exe", "/q"]
        t |> success(fwrite(path_join(session_dir, "session.json"),
            sprint_json(persisted, true)))
        // Mailbox history: hf_3 posted then acknowledged, hf_5 posted once —
        // interleaved with a truncated-write line and plain corruption, which
        // restore must skip without losing the valid records around them.
        let mailbox_lines = ("\{\"id\":\"hf_3\",\"session_id\":\"{session_id}\",\"direction\":\"outbox\",\"sender\":\"agent_session:{session_id}\",\"kind\":\"note\",\"subject\":\"first\",\"status\":\"new\",\"revision\":4\}\n" +
            "\{\"id\":\"hf_4\",\"session_id\":\"{session_id}\",\"direc\n" +
            "\{\"id\":\"hf_3\",\"session_id\":\"{session_id}\",\"direction\":\"outbox\",\"sender\":\"agent_session:{session_id}\",\"kind\":\"note\",\"subject\":\"first\",\"status\":\"acknowledged\",\"revision\":6\}\n" +
            "not json at all\n" +
            "\{\"id\":\"hf_5\",\"session_id\":\"{session_id}\",\"direction\":\"inbox\",\"sender\":\"human:local\",\"kind\":\"note\",\"subject\":\"second\",\"status\":\"new\",\"revision\":7\}\n")
        t |> success(fwrite(path_join(session_dir, "mailbox.jsonl"), mailbox_lines))
        let journal_text = "dead-host-journal-content\r\n"
        t |> success(fwrite(path_join(ptyhost_root, "{session_id}.raw"), journal_text))
        let stamp_json = ("\{\"version\":1,\"session\":\"{session_id}\",\"port\":9," +
            "\"token\":\"t\",\"child_pid\":1234,\"journal_bytes\":{length(journal_text)}," +
            "\"exited\":true,\"exit_code\":42,\"drained\":true,\"error\":\"\"\}")
        t |> success(fwrite(path_join(ptyhost_root, "{session_id}.host.json"), stamp_json))

        t |> equal(watcher_adopt_hosts(), 1)
        var info = WatcherSessionInfo()
        t |> success(watcher_get_session_info(session_id, info),
            "dead-host session materialized")
        t |> equal(info.state, "exited")
        t |> equal(info.exit_code, 42l)
        t |> success(info.drained, "folded session is drained")
        t |> equal(info.pid, 1234)
        t |> success(find(watcher_output_text(session_id), "dead-host-journal-content") >= 0,
            "replay ring seeded from the journal tail")
        t |> success(fexist(path_join(path_join(ptyhost_root, "archive"),
                "{session_id}.host.json")),
            "dead stamp archived after the fold")
        t |> success(!fexist(path_join(ptyhost_root, "{session_id}.host.json")),
            "dead stamp left the discovery directory")

        let mailbox = watcher_mailbox_json(session_id)
        t |> success(find(mailbox, "\"id\":\"hf_3\"") >= 0
                && find(mailbox, "\"status\":\"acknowledged\"") >= 0,
            "restored mailbox keeps the newest state per message")
        t |> success(find(mailbox, "\"id\":\"hf_5\"") >= 0,
            "restored mailbox keeps every message")
        let posted = watcher_mailbox_post(WatcherMailboxPostRequest(
            session_id = session_id, direction = "outbox", subject = "fresh"))
        t |> success(posted.ok, posted.error)
        t |> success(posted.message_id != "hf_3" && posted.message_id != "hf_5",
            "restored id counters prevent duplicate mailbox ids")
        watcher_shutdown()
    }
}

def private adopt_host_death_fails_one_session(t : T?) {
    t |> run("a host dying under a live watcher fails that one session only") <| @(t : T?) {
        let temp = create_temp_directory_result("dasherd_hostdeath_")
        if (!(temp is value)) {
            t |> failure("could not create temporary test directory")
            return
        }
        let root = temp as value
        let ptyhost_root = path_join(root, "ptyhosts")
        var inscope host_prefix <- [test_compiler(),
            path_join(get_das_root(), "utils/internal/das-herd/ptyhost/main.das"), "--"]
        t |> equal(watcher_init(path_join(root, "sessions")), "")
        t |> equal(watcher_configure_ptyhosts(ptyhost_root, host_prefix), "")

        var inscope request = WatcherLaunchRequest(cwd = root,
            display_name = "doomed host", kind = "shell",
            herd_session_id = "h_doomed", columns = 100, rows = 30)
        request.argv <- ["cmd.exe", "/q"]
        let launched = watcher_launch(request)
        t |> success(launched.ok, launched.error)
        let session_id = launched.session_id

        var inscope stamp = ProbeStamp()
        t |> success(sscan_json(fread(path_join(ptyhost_root,
            "{session_id}.host.json")), stamp), "stamp is readable")
        var killer = new HostKiller(token = stamp.token)
        t |> equal(killer->init("ws://127.0.0.1:{stamp.port}/"), 0)
        t |> success(pump_until(15000l) $ {
                killer->process_event_que()
                var info = WatcherSessionInfo()
                return (watcher_get_session_info(session_id, info)
                    && info.state == "failed" && info.drained)
            }, "watcher marked the session failed after the host died")
        var info = WatcherSessionInfo()
        t |> success(watcher_get_session_info(session_id, info))
        t |> equal(info.reason, "host_disconnected")
        killer->close()
        killer->cleanup()
        unsafe {
            delete killer
        }
        watcher_shutdown()
    }
}

def private adopt_bind_collision_retries(t : T?) {
    t |> run("a bind collision retries onto the next port") <| @(t : T?) {
        let temp = create_temp_directory_result("dasherd_bindretry_")
        if (!(temp is value)) {
            t |> failure("could not create temporary test directory")
            return
        }
        let root = temp as value
        let ptyhost_root = path_join(root, "ptyhosts")
        var inscope host_prefix <- [test_compiler(),
            path_join(get_das_root(), "utils/internal/das-herd/ptyhost/main.das"), "--"]
        t |> equal(watcher_init(path_join(root, "sessions")), "")
        t |> equal(watcher_configure_ptyhosts(ptyhost_root, host_prefix), "")

        // Learn where the port cursor is by launching once, then squat on the
        // NEXT port so the following launch must retry past the blocker.
        var inscope first_request = WatcherLaunchRequest(cwd = root,
            display_name = "cursor probe", kind = "shell",
            herd_session_id = "h_probe", columns = 100, rows = 30)
        first_request.argv <- ["cmd.exe", "/q"]
        let first = watcher_launch(first_request)
        t |> success(first.ok, first.error)
        var inscope first_stamp = ProbeStamp()
        t |> success(sscan_json(fread(path_join(ptyhost_root,
            "{first.session_id}.host.json")), first_stamp))

        var blocker = new PortBlocker()
        blocker->init(first_stamp.port + 1)
        let _bind = blocker->set_bind_host("127.0.0.1")
        t |> equal(blocker->start(), 0, "test blocker occupies the next port")

        var inscope second_request = WatcherLaunchRequest(cwd = root,
            display_name = "collision victim", kind = "shell",
            herd_session_id = "h_victim", columns = 100, rows = 30)
        second_request.argv <- ["cmd.exe", "/q"]
        let second = watcher_launch(second_request)
        t |> success(second.ok, second.error)
        var inscope second_stamp = ProbeStamp()
        t |> success(sscan_json(fread(path_join(ptyhost_root,
            "{second.session_id}.host.json")), second_stamp))
        t |> equal(second_stamp.port, first_stamp.port + 2,
            "launch skipped the occupied port")
        var info = WatcherSessionInfo()
        t |> success(watcher_get_session_info(second.session_id, info))
        t |> equal(info.state, "running")

        blocker->stop()
        blocker->cleanup()
        unsafe {
            delete blocker
        }
        let _t1 = watcher_terminate(first.session_id, "test_cleanup")
        let _t2 = watcher_terminate(second.session_id, "test_cleanup")
        t |> success(pump_until(20000l) $ {
                var first_info = WatcherSessionInfo()
                var second_info = WatcherSessionInfo()
                return (watcher_get_session_info(first.session_id, first_info)
                    && watcher_get_session_info(second.session_id, second_info)
                    && first_info.drained && second_info.drained)
            }, "both hosts folded on terminate")
        watcher_shutdown()
    }
}
