options gen2
options rtti

require dastest/testing_boost public
require ../watcher_core.das
require daslib/fio
require strings

[test]
def test_watcher_machine_output(t : T?) {
    t |> run("empty transport target defaults to local") <| @(t : T?) {
        t |> equal(watcher_target_id_or_local(""), "local")
        t |> equal(watcher_target_id_or_local("ssh-build"), "ssh-build")
    }
    t |> run("watcher GC reacts to allocation pressure as well as fragmentation") <| @(t : T?) {
        t |> equal(watcher_gc_reason(64ul * 1024ul * 1024ul,
            65ul * 1024ul * 1024ul, 1ul, 1ul), "string_pressure")
        t |> equal(watcher_gc_reason(1ul, 1ul,
            128ul * 1024ul * 1024ul, 129ul * 1024ul * 1024ul), "heap_pressure")
        t |> equal(watcher_gc_reason(10ul, 100ul, 10ul, 10ul),
            "string_fragmentation")
        t |> equal(watcher_gc_reason(10ul, 10ul, 10ul, 100ul),
            "heap_fragmentation")
        t |> equal(watcher_gc_reason(10ul, 10ul, 10ul, 10ul), "")
    }
    t |> run("ConPTY wrapper controls are removed without changing task text") <| @(t : T?) {
        let esc = to_char(27)
        let bel = to_char(7)
        let wrapped = "{esc}[?9001h{esc}[2J{esc}[Hfirst\r\nsecond\tvalue{esc}]0;git.exe{bel}{esc}[?25h"
        t |> equal(watcher_strip_terminal_controls(wrapped), "first\r\nsecond\tvalue")
        let expanded_tabs = "1{esc}[7C2{esc}[6Cutils/dasHerd/watcher_core.das"
        t |> equal(watcher_strip_terminal_controls(expanded_tabs),
            "1       2      utils/dasHerd/watcher_core.das")
        t |> equal(watcher_strip_terminal_controls("abc\n{esc}[5Gx"),
            "abc\n    x", "normalized lone LF resets the synthesized text column")
        let worktrees = "worktree D:/main\r\nHEAD A\r\nbranch refs/heads/main{esc}[5;1Hworktree D:/linked\r\nHEAD B\r\nbranch refs/heads/topic"
        t |> equal(watcher_strip_terminal_controls(worktrees),
            "worktree D:/main\r\nHEAD A\r\nbranch refs/heads/main\n\nworktree D:/linked\r\nHEAD B\r\nbranch refs/heads/topic")
        let unicode_wrapped = "{esc}[31mΚαλημέρα, мир, こんにちは, 🐑{esc}[0m"
        t |> equal(watcher_strip_terminal_controls(unicode_wrapped),
            "Καλημέρα, мир, こんにちは, 🐑")
        let e_acute : string = to_char(int(0xc3)) + to_char(int(0xa9))
        t |> equal(watcher_strip_terminal_controls(e_acute + esc + "[5Gx"),
            e_acute + "   x", "UTF-8 continuation bytes do not consume terminal columns")
        let wide_cjk : string = to_char(int(0xe7)) + to_char(int(0x95)) + to_char(int(0x8c))
        t |> equal(watcher_strip_terminal_controls(wide_cjk + esc + "[5Gx"),
            wide_cjk + "  x", "wide Unicode codepoints consume two terminal columns")
        let combining_acute : string = to_char(int(0xcc)) + to_char(int(0x81))
        t |> equal(watcher_strip_terminal_controls("e" + combining_acute + esc + "[5Gx"),
            "e" + combining_acute + "   x", "combining codepoints do not consume a terminal column")
    }
    t |> run("machine output reports truncation through its out parameter") <| @(t : T?) {
        var truncated = true
        t |> equal(watcher_machine_output_text("missing-session", truncated), "")
        t |> success(!truncated)
    }
}

def private wait_for(session_id, state : string; timeout_ms : int64) : bool {
    let started = ref_time_ticks()
    while (get_time_nsec(started) / 1000000l < timeout_ms) {
        watcher_tick()
        let snapshot = watcher_snapshot_json(session_id)
        if (find(snapshot, "\"state\":\"{state}\"") >= 0 &&
                find(snapshot, "\"drained\":true") >= 0) return true
        sleep(1u)
    }
    return false
}

[test]
def test_watcher_process_lifecycle(t : T?) {
    if (get_platform_name() != "windows") return
    t |> run("persistent core logs output, times out hangs, and arbitrates control") <| @(t : T?) {
        let temp = create_temp_directory_result("dasherd_watcher_test_")
        if (!(temp is value)) {
            t |> failure("could not create temporary test directory")
            return
        }
        let root = temp as value
        t |> equal(watcher_init(path_join(root, "sessions")), "")

        let oversized_display = watcher_launch(WatcherLaunchRequest(
            command_line = "cmd.exe /d /c exit 0", cwd = root,
            display_name = repeat("x", 32 * 1024 + 1)))
        t |> success(!oversized_display.ok
                && find(oversized_display.error, "display_name") >= 0,
            "oversized display metadata is rejected before session allocation")
        var oversized_argv <- ["cmd.exe", repeat("x", 32 * 1024)]
        let oversized_arguments = watcher_launch(WatcherLaunchRequest(
            argv <- oversized_argv, cwd = root))
        t |> success(!oversized_arguments.ok
                && find(oversized_arguments.error, "argv") >= 0,
            "oversized structured argv is rejected before display joining")

        let missing_command = path_join(root, "missing-watcher-command.exe")
        let failed = watcher_launch(WatcherLaunchRequest(
            command_line = "\"{missing_command}\"", cwd = root,
            columns = 1, rows = 999, timeout_ms = 5000l))
        t |> success(!failed.ok, "failed PTY launch is reported to the caller")
        t |> success(!empty(failed.session_id), "failed PTY launch retains its diagnostic session")
        let failed_snapshot = watcher_snapshot_json(failed.session_id)
        t |> success(find(failed_snapshot, "\"state\":\"failed\"") >= 0,
            "failed PTY launch is retained in watcher state")
        t |> success(find(failed_snapshot, "\"pid\":0") >= 0,
            "failed PTY launch retains a deterministic zero process id")
        t |> equal(base_name(watcher_session_artifact_path(
            failed.session_id, "staged-view.raw")), "staged-view.raw")
        t |> equal(watcher_session_artifact_path(
            failed.session_id, "../outside.raw"), "")
        t |> equal(watcher_session_artifact_path(
            failed.session_id, "C:outside.raw"), "")
        let failed_metadata = fread(path_join(path_join(path_join(root, "sessions"),
            failed.session_id), "session.json"))
        t |> success(find(failed_metadata, "\"columns\":20") >= 0
                && find(failed_metadata, "\"rows\":512") >= 0,
            "session metadata retains the clamped launch dimensions")
        watcher_tick()
        t |> success(find(watcher_snapshot_json(failed.session_id), "\"state\":\"failed\"") >= 0,
            "process exit observation preserves the failed state")

        let quick = watcher_launch(WatcherLaunchRequest(
            command_line = "powershell.exe -NoProfile -Command \"Write-Output watcher-core-ok\"",
            cwd = root, columns = 80, rows = 20, timeout_ms = 5000l))
        t |> success(quick.ok, quick.error)
        t |> equal(watcher_mailbox_session_revision("missing-session"), -1l,
            "unknown sessions do not expose a normal mailbox revision")
        t |> success(watcher_accepts_input(quick.session_id), "running session accepts input")
        t |> success(wait_for(quick.session_id, "exited", 5000l), "quick command exits and drains")
        t |> success(!watcher_accepts_input(quick.session_id), "exited session is replay-only")
        let quick_snapshot = watcher_snapshot_json(quick.session_id)
        t |> success(find(quick_snapshot, "watcher-core-ok") >= 0, "viewport retains command output")
        t |> success(find(quick_snapshot, "\"exit_code\":0") >= 0, "exit code is retained")
        let raw <- watcher_output_since(quick.session_id, 0l)
        t |> success(find(string(raw.bytes), "watcher-core-ok") >= 0, "raw PTY history can be replayed")
        let raw_tail <- watcher_output_since(quick.session_id, raw.next_offset)
        t |> equal(length(raw_tail.bytes), 0, "raw replay offset does not duplicate output")
        t |> success(fexist(path_join(path_join(path_join(root, "sessions"), quick.session_id), "output.raw")),
            "raw output log exists")

        var senderless_request = WatcherMailboxPostRequest()
        t |> success(sscan_json("\{\"session_id\":{sprint_json(quick.session_id, false)}," +
                "\"direction\":\"inbox\",\"subject\":\"No sender\"\}",
                senderless_request),
            "mailbox JSON accepts an omitted transport sender")
        t |> equal(senderless_request.sender, "",
            "omitted transport sender defaults to empty before server stamping")

        var focus_target = WatcherFocusTarget(
            file_path = "unicode-Привет.txt")
        focus_target.ranges |> push(WatcherFocusRange(
            start_byte = 2, end_byte = 9, caption = "important"))
        var mailbox_request = WatcherMailboxPostRequest(
            session_id = quick.session_id, direction = "outbox",
            sender = "agent_session:{quick.session_id}", kind = "focus_set",
            subject = "Ready for PR")
        mailbox_request.focus.repository_id = "repo"
        mailbox_request.focus.worktree_path = root
        mailbox_request.focus.targets |> emplace(focus_target)
        let posted = watcher_mailbox_post(mailbox_request)
        t |> success(posted.ok, posted.error)
        t |> success(starts_with(posted.message_id, "hf_"),
            "mailbox messages receive stable ids")
        let outbox = watcher_mailbox_json(quick.session_id, "outbox")
        t |> success(find(outbox, "unicode-Привет.txt") >= 0
                && find(outbox, "Ready for PR") >= 0
                && find(outbox, "agent_session:{quick.session_id}") >= 0,
            "focus set and server-assigned provenance survive JSON serialization")
        let notified = watcher_mailbox_set_notification(
            quick.session_id, posted.message_id)
        t |> success(notified.ok, notified.error)
        let fetched = watcher_mailbox_mark_fetched(
            quick.session_id, posted.message_id)
        t |> success(fetched.ok, fetched.error)
        let acknowledged = watcher_mailbox_set_state(WatcherMailboxStateRequest(
            session_id = quick.session_id, message_id = posted.message_id,
            status = "acknowledged"))
        t |> success(acknowledged.ok, acknowledged.error)
        t |> success(find(watcher_mailbox_json(quick.session_id, "", posted.message_id),
            "\"status\":\"acknowledged\"") >= 0
                && find(watcher_mailbox_json(quick.session_id, "", posted.message_id),
                    "\"notified\":true") >= 0
                && find(watcher_mailbox_json(quick.session_id, "", posted.message_id),
                    "\"fetched\":true") >= 0,
            "notification, fetch, and acknowledgement are observable")
        let mailbox_log = path_join(path_join(path_join(root, "sessions"),
            quick.session_id), "mailbox.jsonl")
        t |> success(fexist(mailbox_log)
                && find(fread(mailbox_log), "acknowledged") >= 0,
            "mailbox create and state records are durable JSONL")

        var first_participant = WatcherBundleParticipant(
            repository_id = "repo-main", worktree_path = root,
            role = "implementation", state = "modified")
        first_participant.files |> push(WatcherBundleFile(
            path = "src/main.das", role = "primary"))
        var second_participant = WatcherBundleParticipant(
            repository_id = "repo-render", worktree_path = path_join(root, "other-worktree"),
            role = "dependency", state = "modified")
        second_participant.files |> push(WatcherBundleFile(
            path = "src/render.das", role = "risk"))
        var first_bundle_target = WatcherFocusTarget(
            repository_id = "repo-main", worktree_path = root,
            comparison = "working", role = "primary", file_path = "src/main.das")
        first_bundle_target.ranges |> push(WatcherFocusRange(
            start_byte = 1, end_byte = 8, caption = "main loop"))
        var second_bundle_target = WatcherFocusTarget(
            repository_id = "repo-render", worktree_path = path_join(root, "other-worktree"),
            comparison = "working", role = "risk", file_path = "src/render.das")
        second_bundle_target.ranges |> push(WatcherFocusRange(
            start_byte = 4, end_byte = 12, caption = "bug was here"))
        var bundle = WatcherReviewBundle(id = "bundle-debug",
            title = "Cross-repository debug fix", kind = "debug", status = "ready",
            summary = "Two repositories participate in one review unit")
        bundle.participants |> emplace(first_participant)
        bundle.participants |> emplace(second_participant)
        bundle.focus.targets |> emplace(first_bundle_target)
        bundle.focus.targets |> emplace(second_bundle_target)
        let bundle_result = watcher_bundle_sync(WatcherBundleSyncRequest(
            session_id = quick.session_id, bundle := bundle))
        t |> success(bundle_result.ok, bundle_result.error)
        t |> equal(bundle_result.bundle_id, "bundle-debug")
        let bundles_json = watcher_bundles_json(quick.session_id)
        t |> success(find(bundles_json, "repo-main") >= 0
                && find(bundles_json, "repo-render") >= 0
                && find(bundles_json, "bug was here") >= 0,
            "one durable review bundle spans repositories and exact focus ranges")
        let bundle_attention = watcher_mailbox_json(quick.session_id, "outbox")
        t |> success(find(bundle_attention, "\"bundle_id\":\"bundle-debug\"") >= 0
                && find(bundle_attention, "\"kind\":\"bundle_focus\"") >= 0,
            "a ready bundle projects one non-modal Attention message")
        var updated_bundle := bundle
        updated_bundle.summary = "Updated complete snapshot"
        let updated_result = watcher_bundle_sync(WatcherBundleSyncRequest(
            session_id = quick.session_id, bundle := updated_bundle))
        t |> success(updated_result.ok && updated_result.revision > bundle_result.revision,
            "bundle sync replaces the same logical bundle with a newer snapshot")
        let updated_attention = watcher_mailbox_json(quick.session_id, "outbox")
        let first_bundle_marker = find(updated_attention,
            "\"bundle_id\":\"bundle-debug\"")
        t |> success(first_bundle_marker >= 0 && find(updated_attention,
                "\"bundle_id\":\"bundle-debug\"", first_bundle_marker + 1) < 0,
            "retry-safe bundle sync updates rather than duplicating Attention")
        let bundles_log = path_join(path_join(path_join(root, "sessions"),
            quick.session_id), "bundles.jsonl")
        t |> success(fexist(bundles_log)
                && find(fread(bundles_log), "Updated complete snapshot") >= 0,
            "bundle snapshots are durable JSONL")

        let spaced_dir = path_join(root, "argv space")
        var mkdir_error : string
        t |> success(mkdir_rec(spaced_dir, mkdir_error), mkdir_error)
        let echo_script = path_join(spaced_dir, "echo args.ps1")
        t |> success(fwrite(echo_script, "[Console]::Write(($args -join '|'))"),
            "argv echo script is written")
        let fake_codex_script = path_join(spaced_dir, "fake codex.ps1")
        t |> success(fwrite(fake_codex_script,
            "[Console]::Write($env:DASHERD_SESSION_KIND + '|' + $env:DASHERD_SESSION_ID + '|' + $args.Count)"),
            "fake Codex entry point is written")
        var fake_codex_argv <- ["powershell.exe", "-NoLogo", "-NoProfile",
            "-File", fake_codex_script]
        let agent = watcher_launch(WatcherLaunchRequest(argv <- fake_codex_argv,
            cwd = root, display_name = "Codex debug", kind = "agent-codex",
            purpose = "debug", task_context = "Find the renderer crash",
            agent_url = "http://127.0.0.1:9191", agent_token = "secret-not-persisted",
            agent_skill_path = path_join(root, "dasherder.md"),
            target_id = "local", repository_id = "test-repository",
            worktree_path = root, columns = 80, rows = 20, timeout_ms = 5000l))
        t |> success(agent.ok, agent.error)
        t |> success(wait_for(agent.session_id, "exited", 5000l),
            "agent bootstrap exits after the fake Codex process")
        let agent_output = watcher_output_text(agent.session_id)
        t |> success(find(agent_output, "debug|{agent.session_id}|1") >= 0,
            "agent process receives purpose, watcher-assigned session id, and context prompt")
        let agent_dir = path_join(path_join(root, "sessions"), agent.session_id)
        let agent_context = fread(path_join(agent_dir, "context.md"))
        let agent_metadata = fread(path_join(agent_dir, "session.json"))
        t |> success(find(agent_context, "Find the renderer crash") >= 0
                && find(agent_context, "immutable session origin") >= 0,
            "watcher-authored agent context explains the task and cooperation contract")
        t |> success(find(agent_metadata, "secret-not-persisted") < 0,
            "session metadata never persists the watcher routing token")
        var structured_argv <- ["powershell.exe", "-NoLogo", "-NoProfile", "-File", echo_script,
            "space value", "quote\"value", "trailing\\"]
        let structured = watcher_launch(WatcherLaunchRequest(argv <- structured_argv,
            cwd = root, display_name = "structured argv test", kind = "test",
            target_id = "local", repository_id = "test-repository", worktree_path = root,
            columns = 80, rows = 20, timeout_ms = 5000l))
        t |> success(structured.ok, structured.error)
        t |> success(wait_for(structured.session_id, "exited", 5000l),
            "structured argv command exits and drains")
        let structured_snapshot = watcher_snapshot_json(structured.session_id)
        t |> success(find(structured_snapshot, "space value|quote\\\"value|trailing\\\\") >= 0,
            "structured argv preserves spaces, quotes, and trailing backslashes")
        t |> success(find(structured_snapshot, "\"worktree_path\":") >= 0,
            "session snapshot retains launch worktree association")
        var before_compact = WatcherSessionInfo()
        t |> success(watcher_get_session_info(structured.session_id, before_compact),
            "completed structured session remains addressable")
        t |> success(watcher_compact_session(structured.session_id, 60, 10),
            "completed session can release its oversized terminal state")
        var after_compact = WatcherSessionInfo()
        t |> success(watcher_get_session_info(structured.session_id, after_compact),
            "compacted session remains addressable")
        t |> equal(after_compact.pid, before_compact.pid,
            "compaction retains the original process id")
        t |> equal(after_compact.exit_code, before_compact.exit_code,
            "compaction retains the exit code")
        t |> success(find(watcher_snapshot_json(structured.session_id), "space value") >= 0,
            "compaction replays retained raw output into an inspectable terminal")
        let structured_log = path_join(path_join(path_join(root, "sessions"), structured.session_id), "output.raw")
        t |> equal(watcher_prune_completed_sessions("test", "test-repository", root), 1,
            "completed observation session can leave the in-memory working set")
        t |> equal(watcher_find_session(structured.session_id), -1,
            "pruned observation session is removed from the live list")
        t |> success(fexist(structured_log), "pruning preserves its durable raw log")

        for (iteration in range(6)) {
            let close_race = watcher_launch(WatcherLaunchRequest(
                command_line = "cmd.exe /d /c exit 0", cwd = root,
                columns = 80, rows = 20, timeout_ms = 5000l))
            t |> success(close_race.ok, close_race.error)
            t |> success(wait_for(close_race.session_id, "exited", 5000l),
                "pipe closure still converges to process exit ({iteration})")
        }

        let hung = watcher_launch(WatcherLaunchRequest(
            command_line = "powershell.exe -NoProfile -Command \"Start-Sleep -Seconds 30\"",
            cwd = root, columns = 80, rows = 20, timeout_ms = 250l))
        t |> success(hung.ok, hung.error)
        t |> success(wait_for(hung.session_id, "exited", 4000l), "hung command is externally terminated")
        t |> success(find(watcher_snapshot_json(hung.session_id), "\"reason\":\"wall_timeout\"") >= 0,
            "timeout reason is retained")

        let controlled = watcher_launch(WatcherLaunchRequest(
            command_line = "powershell.exe -NoProfile -Command \"Start-Sleep -Seconds 5\"",
            cwd = root, columns = 80, rows = 20, timeout_ms = 10000l))
        t |> success(controlled.ok, controlled.error)
        let quick_mailbox_revision = watcher_mailbox_session_revision(quick.session_id)
        t |> equal(watcher_mailbox_session_revision(controlled.session_id), 0l,
            "a new session starts with an independent mailbox revision")
        t |> success(watcher_claim_controller(controlled.session_id, 11), "first controller claims lease")
        t |> success(!watcher_claim_controller(controlled.session_id, 12), "second controller is rejected")
        watcher_release_controller(controlled.session_id, 11, "test_detach")
        t |> success(watcher_claim_controller(controlled.session_id, 12), "released lease can be reclaimed")
        let queued_message = watcher_mailbox_post(WatcherMailboxPostRequest(
            session_id = controlled.session_id, direction = "inbox",
            sender = "system:dasHerd", subject = "Look at that"))
        t |> success(queued_message.ok, queued_message.error)
        t |> equal(watcher_mailbox_session_revision(quick.session_id),
            quick_mailbox_revision,
            "mailbox activity does not change another session's revision")
        t |> equal(watcher_input(controlled.session_id, "partial", 12), "")
        t |> equal(watcher_notify(controlled.session_id, "nudge\r",
            queued_message.message_id), "queued",
            "notification waits behind a partial terminal input line")
        t |> success(watcher_mailbox_session_revision(controlled.session_id)
                > queued_message.revision,
            "recording queued delivery advances the session mailbox revision")
        let missing_fetch = watcher_mailbox_mark_fetched(
            controlled.session_id, "missing-message")
        t |> success(!missing_fetch.ok && missing_fetch.error == "unknown mailbox message",
            "fetching an unknown mailbox message reports a routing error")
        let queued_json = watcher_mailbox_json(
            controlled.session_id, "inbox", queued_message.message_id)
        t |> success(find(queued_json, "\"sender\":\"human:local\"") >= 0
                && find(queued_json, "\"notification_error\":\"queued\"") >= 0,
            "queued notification and reserved system provenance cannot be spoofed")
        t |> equal(watcher_input(controlled.session_id, "\r", 12), "")
        let delivered_json = watcher_mailbox_json(
            controlled.session_id, "inbox", queued_message.message_id)
        t |> success(find(delivered_json, "\"notified\":true") >= 0
                && find(delivered_json, "\"notification_error\":\"\"") >= 0,
            "submitting the partial line atomically flushes its queued notification")
        t |> equal(watcher_input(controlled.session_id, "partial", 12), "")
        t |> equal(watcher_notify(controlled.session_id, "plain nudge\r"), "queued")
        t |> equal(watcher_input(controlled.session_id, "\r", 12), "")
        let controlled_events = fread(path_join(path_join(path_join(root, "sessions"),
            controlled.session_id), "events.jsonl"))
        t |> success(find(controlled_events, "\"sequence\":1") >= 0
                && find(controlled_events, "\"wall_time_seconds\":") >= 0
                && find(controlled_events, "\"state\":") >= 0
                && find(controlled_events, "\"controller_id\":") >= 0,
            "every event carries ordered time and reconstructible session/controller context")
        t |> success(find(controlled_events, "\"event\":\"session_created\"") >= 0
                && find(controlled_events, "\"event\":\"controller_claimed\"") >= 0
                && find(controlled_events, "\"event\":\"controller_claim_rejected\"") >= 0
                && find(controlled_events, "\"event\":\"controller_released\"") >= 0,
            "controller contention and release are durably auditable")
        t |> success(find(controlled_events, "delivery=deferred") >= 0,
            "a deferred mailbox nudge records its eventual delivery")
        t |> success(find(controlled_events, "terminal_notification_queued") >= 0,
            "a deferred non-mailbox nudge uses a terminal-specific queue event")
        t |> success(find(controlled_events,
                "\"event\":\"mailbox_notification_queued\",\"detail\":\"id=;") < 0,
            "a deferred non-mailbox nudge does not emit an empty mailbox queue event")
        t |> success(find(controlled_events, "\"detail\":\"id=; delivery=deferred\"") < 0,
            "a deferred non-mailbox nudge does not emit an empty mailbox event")
        t |> equal(watcher_terminate(controlled.session_id, "test_complete"), "")
        t |> success(wait_for(controlled.session_id, "exited", 4000l), "terminated command exits and drains")
        watcher_shutdown(true)
        rmdir_rec(root)
    }
}
