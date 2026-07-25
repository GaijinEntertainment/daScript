options gen2
options rtti

require dastest/testing_boost public
require ../herd_sessions.das
require ../watcher_core.das
require daslib/fio
require strings

def private wait_for_herd_state(herd_id, state : string; timeout_ms : int64) : bool {
    let started = ref_time_ticks()
    while (get_time_nsec(started) / 1000000l < timeout_ms) {
        watcher_tick()
        herd_tick()
        var inscope record = HerdSession()
        if (herd_get(herd_id, record) && record.state == state) return true
        sleep(1u)
    }
    return false
}

def private wait_for_pty(session_id, state : string; timeout_ms : int64) : bool {
    let started = ref_time_ticks()
    while (get_time_nsec(started) / 1000000l < timeout_ms) {
        watcher_tick()
        herd_tick()
        let snapshot = watcher_snapshot_json(session_id)
        if (find(snapshot, "\"state\":\"{state}\"") >= 0 &&
                find(snapshot, "\"drained\":true") >= 0) return true
        sleep(1u)
    }
    return false
}

[test]
def test_herd_registry(t : T?) {
    t |> run("registry persists durable sessions across watcher restarts") <| @(t : T?) {
        let temp = create_temp_directory_result("dasherd_herd_registry_")
        if (!(temp is value)) {
            t |> failure("could not create temporary test directory")
            return
        }
        let root = temp as value
        let registry_path = path_join(root, "_registry.json")
        let profiles_path = path_join(root, "profiles.json")
        t |> equal(herd_init(registry_path, profiles_path), "")
        t |> success(fexist(profiles_path),
            "shipped default profiles are written for user editing")
        let profiles = herd_profiles_json()
        t |> success(find(profiles, "\"shell\"") >= 0 && find(profiles, "\"claude\"") >= 0
                && find(profiles, "\"codex\"") >= 0,
            "default profiles ship shell, claude, and codex")

        var error : string
        t |> success(empty(herd_create(HerdCreateRequest(profile_id = "unknown",
            worktree_path = root), error)) && find(error, "unknown profile") >= 0,
            "creation rejects an unknown profile")
        t |> success(empty(herd_create(HerdCreateRequest(profile_id = "shell",
            worktree_path = path_join(root, "missing")), error))
                && find(error, "does not exist") >= 0,
            "creation rejects a missing worktree")
        t |> success(empty(herd_create(HerdCreateRequest(profile_id = "shell",
            host = "ssh:build", worktree_path = root), error))
                && find(error, "local host") >= 0,
            "creation rejects non-local hosts for now")

        let herd_id = herd_create(HerdCreateRequest(profile_id = "claude",
            workflow = "regular", repository_id = "test-repo",
            worktree_path = root, task_context = "Review the diff"), error)
        t |> success(!empty(herd_id), error)
        var inscope record = HerdSession()
        t |> success(herd_get(herd_id, record), "created session is addressable")
        t |> equal(record.name, base_name(root))
        t |> equal(record.state, "exited")
        t |> equal(record.reason, "never_launched")
        t |> equal(record.worktree_path, herd_display_worktree(root),
            "records store the canonical forward-slash spelling")

        t |> success(!empty(herd_rename(herd_id, " ")),
            "rename rejects a blank name")
        t |> equal(herd_rename(herd_id, "renamed session"), "")
        t |> equal(herd_add_satellite(herd_id, "other-repo", path_join(root, "other"), "agent"), "")
        t |> equal(herd_add_satellite(herd_id, "other-repo", path_join(root, "other"), "agent"), "")
        t |> equal(herd_add_satellite(herd_id, "test-repo", root, "agent"), "")
        t |> success(herd_get(herd_id, record), "renamed session is addressable")
        t |> equal(record.name, "renamed session")
        t |> equal(length(record.satellites), 1,
            "satellite adds deduplicate against satellites and the primary worktree")

        var inscope launch = WatcherLaunchRequest()
        t |> success(herd_build_launch(herd_id, true, "http://127.0.0.1:9191",
            "token", "skill.md", launch, error), error)
        t |> equal(launch.argv[0], "claude")
        t |> equal(launch.kind, "agent-claude")
        t |> equal(launch.cwd, herd_display_worktree(root))
        t |> equal(launch.herd_session_id, herd_id)
        t |> equal(launch.agent_url, "http://127.0.0.1:9191")
        t |> success(!launch.resume, "the first launch is not a resume")
        var inscope resume = WatcherLaunchRequest()
        t |> success(herd_build_launch(herd_id, false, "http://127.0.0.1:9191",
            "token", "skill.md", resume, error), error)
        t |> success(length(resume.argv) == 2 && resume.argv[1] == "--continue",
            "relaunch uses the profile resume command")
        t |> success(resume.resume, "relaunch is flagged as a resume")

        t |> equal(herd_link_pty(herd_id, "pty_fake"), "")
        t |> success(herd_get(herd_id, record) && record.state == "running",
            "linking a PTY marks the session running")
        herd_shutdown()
        t |> equal(herd_init(registry_path, profiles_path), "")
        t |> success(herd_get(herd_id, record), "sessions survive a watcher restart")
        t |> equal(record.state, "exited")
        t |> equal(record.reason, "watcher_restart")
        t |> equal(record.name, "renamed session")
        t |> equal(length(record.satellites), 1)

        t |> equal(herd_link_pty(herd_id, "pty_fake_2"), "")
        var pty_to_terminate : string
        t |> equal(herd_close(herd_id, pty_to_terminate), "")
        t |> equal(pty_to_terminate, "pty_fake_2")
        t |> success(herd_get(herd_id, record) && record.state == "closed",
            "closing is an explicit user act")

        let first_id = herd_create(HerdCreateRequest(name = "conflict a",
            profile_id = "shell", worktree_path = root), error)
        t |> success(!empty(first_id), error)
        herd_tick()
        t |> success(herd_get(first_id, record) && !record.conflicted,
            "a lone session is never conflicted")
        let second_id = herd_create(HerdCreateRequest(name = "conflict b",
            profile_id = "shell", worktree_path = root), error)
        t |> success(!empty(second_id), error)
        herd_tick()
        t |> success(herd_get(first_id, record) && record.conflicted,
            "two sessions on one worktree both flag the advisory conflict")
        t |> success(herd_get(second_id, record) && record.conflicted,
            "the second session is flagged too")
        var other_dir_error : string
        let other_dir = path_join(root, "elsewhere")
        t |> success(mkdir_rec(other_dir, other_dir_error), other_dir_error)
        let third_id = herd_create(HerdCreateRequest(name = "conflict c",
            profile_id = "shell", worktree_path = other_dir), error)
        t |> success(!empty(third_id), error)
        herd_tick()
        t |> success(herd_get(third_id, record) && !record.conflicted,
            "a different worktree stays unflagged")
        t |> equal(herd_add_satellite(third_id, "repo", root, "agent"), "")
        herd_tick()
        t |> success(herd_get(third_id, record) && record.conflicted,
            "a satellite on a contested worktree flags the session")
        t |> equal(herd_close(second_id, pty_to_terminate), "")
        t |> equal(herd_close(third_id, pty_to_terminate), "")
        herd_tick()
        t |> success(herd_get(first_id, record) && !record.conflicted,
            "closing the other attendees clears the advisory flag")
        herd_shutdown()
    }
    t |> run("profile intro lines ride the initial launch request") <| @(t : T?) {
        let temp = create_temp_directory_result("dasherd_herd_intro_")
        if (!(temp is value)) {
            t |> failure("could not create temporary test directory")
            return
        }
        let root = temp as value
        let profiles_path = path_join(root, "profiles.json")
        t |> success(fwrite(profiles_path, "\{\"version\":1,\"profiles\":[" +
            "\{\"id\":\"intro\",\"display_name\":\"Intro\"," +
            "\"launch_argv\":[\"claude\"]," +
            "\"resume_argv\":[\"claude\",\"--continue\"]," +
            "\"agent\":true,\"intro_lines\":[\"Line A\",\"Line B\"]\}]\}"),
            "custom intro profile written")
        t |> equal(herd_init(path_join(root, "_registry.json"), profiles_path), "")
        var error : string
        let herd_id = herd_create(HerdCreateRequest(name = "intro session",
            profile_id = "intro", worktree_path = root), error)
        t |> success(!empty(herd_id), error)
        var inscope launch = WatcherLaunchRequest()
        t |> success(herd_build_launch(herd_id, true, "", "", "", launch, error), error)
        t |> equal(launch.intro_text, "Line A\nLine B")
        t |> success(!launch.resume, "initial launch is not a resume")
        herd_shutdown()
    }
}

[test]
def test_herd_lifecycle(t : T?) {
    if (get_platform_name() != "windows") return
    t |> run("durable sessions launch, exit, relaunch, and close over real PTYs") <| @(t : T?) {
        let temp = create_temp_directory_result("dasherd_herd_lifecycle_")
        if (!(temp is value)) {
            t |> failure("could not create temporary test directory")
            return
        }
        let root = temp as value
        t |> equal(watcher_init(path_join(root, "sessions")), "")
        let profiles_path = path_join(root, "profiles.json")
        t |> success(fwrite(profiles_path, "\{\"version\":1,\"profiles\":[" +
            "\{\"id\":\"fakeshell\",\"display_name\":\"Fake\"," +
            "\"launch_argv\":[\"powershell.exe\",\"-NoLogo\",\"-NoProfile\",\"-Command\",\"exit 3\"]," +
            "\"resume_argv\":[\"powershell.exe\",\"-NoLogo\",\"-NoProfile\",\"-Command\",\"exit 5\"]," +
            "\"agent\":false\}," +
            "\{\"id\":\"sleeper\",\"display_name\":\"Sleeper\"," +
            "\"launch_argv\":[\"powershell.exe\",\"-NoLogo\",\"-NoProfile\",\"-Command\",\"Start-Sleep -Seconds 30\"]," +
            "\"resume_argv\":[],\"agent\":false\}]\}"),
            "test profiles file is written")
        t |> equal(herd_init(path_join(root, "_registry.json"), profiles_path), "")
        t |> success(herd_find_profile("fakeshell") >= 0 && herd_find_profile("claude") < 0,
            "the profiles file is authoritative over the shipped defaults")

        var error : string
        let herd_id = herd_create(HerdCreateRequest(name = "fake session",
            profile_id = "fakeshell", worktree_path = root), error)
        t |> success(!empty(herd_id), error)
        var inscope launch = WatcherLaunchRequest()
        t |> success(herd_build_launch(herd_id, true, "", "", "", launch, error), error)
        t |> equal(launch.kind, "shell")
        let first = watcher_launch(launch)
        t |> success(first.ok, first.error)
        t |> equal(herd_link_pty(herd_id, first.session_id), "")
        t |> success(wait_for_herd_state(herd_id, "exited", 10000l),
            "the herd session observes its PTY exit")
        var inscope record = HerdSession()
        t |> success(herd_get(herd_id, record), "exited session is addressable")
        t |> equal(record.exit_code, 3l)
        t |> equal(record.reason, "process_exited")
        t |> equal(record.launch_count, 1)
        let posted = watcher_mailbox_post(WatcherMailboxPostRequest(
            session_id = first.session_id, direction = "outbox",
            subject = "work is ready", notify = false))
        t |> success(posted.ok, posted.error)
        herd_tick()
        t |> success(herd_get(herd_id, record) && record.unread_outbox == 1,
            "unread agent mail is reflected on the durable session")

        var inscope relaunch = WatcherLaunchRequest()
        t |> success(herd_build_launch(herd_id, false, "", "", "", relaunch, error), error)
        let second = watcher_launch(relaunch)
        t |> success(second.ok, second.error)
        t |> equal(herd_link_pty(herd_id, second.session_id), "")
        t |> success(wait_for_herd_state(herd_id, "exited", 10000l),
            "the relaunched PTY exit is observed too")
        t |> success(herd_get(herd_id, record), "relaunched session is addressable")
        t |> equal(record.exit_code, 5l)
        t |> equal(record.launch_count, 2)

        let sleeper_id = herd_create(HerdCreateRequest(name = "sleeper",
            profile_id = "sleeper", worktree_path = root), error)
        t |> success(!empty(sleeper_id), error)
        var inscope sleeper_launch = WatcherLaunchRequest()
        t |> success(herd_build_launch(sleeper_id, true, "", "", "", sleeper_launch, error), error)
        let third = watcher_launch(sleeper_launch)
        t |> success(third.ok, third.error)
        t |> equal(herd_link_pty(sleeper_id, third.session_id), "")
        var pty_to_terminate : string
        t |> equal(herd_close(sleeper_id, pty_to_terminate), "")
        t |> equal(pty_to_terminate, third.session_id)
        t |> equal(watcher_terminate(pty_to_terminate, "test_cleanup"), "")
        t |> success(wait_for_pty(third.session_id, "exited", 10000l),
            "the closed session's PTY terminates")
        t |> success(herd_get(sleeper_id, record) && record.state == "closed",
            "close outlives the PTY termination")
        herd_shutdown()
        watcher_shutdown()
    }
}
