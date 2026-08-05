options gen2
options indenting = 4

module buildd_service public

require dashv/dashv_boost public
require daslib/json_boost
require daslib/logger
require daslib/fio
require buildd_config public
require buildd_core public
require buildd_client public

//! Lifecycle and orchestration: the loopback health/shutdown server plus the
//! poll → build → upload loop. One job at a time, on the tick thread — a
//! running build blocks /healthz, which the watchdog only logs (it restarts
//! on exit, never on health), and the build timeout bounds the dark window.

// Lifecycle-owned state. Module-global: the service may run on its own
// context (tests), and JIT heap collection cannot see stack locals as GC roots.
var private g_server : BuilddHealthServer?
var private g_cfg = BuilddArgs()
var private g_toolchain = ""
var private g_announced = false
var private g_stop_requested = false
var private g_server_started = false
var private g_next_poll = 0l
var private g_job_counter = 0

def is_loopback_peer(peer : string) : bool {
    //! Exact-match only, same rule as the playground's operator gate.
    return peer == "127.0.0.1" || peer == "::1" || peer == "::ffff:127.0.0.1"
}

def private is_loopback_caller(var req : HttpRequest?) : bool {
    return is_loopback_peer(client_ip(req))
}

class BuilddHealthServer : HvWebServer {
    def override onInit {
        GET("/healthz") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("ok")
        }
        POST("/shutdown") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!is_loopback_caller(req)) {
                logger_warning("buildd", "shutdown refused from non-loopback peer")
                return resp |> TEXT_PLAIN("forbidden", http_status.FORBIDDEN)
            }
            logger_info("buildd", "shutdown requested")
            g_stop_requested = true
            return resp |> TEXT_PLAIN("bye")
        }
    }
}

def init_service(cfg : BuilddArgs) : bool {
    //! Validate config, resolve the toolchain identity, bind loopback, start.
    g_cfg = cfg
    g_stop_requested = false
    g_next_poll = 0l
    if (empty(cfg.token)) {
        logger_error("buildd", "no token configured — refusing to start")
        return false
    }
    if (empty(cfg.build_cmd)) {
        logger_error("buildd", "no build_cmd configured — refusing to start")
        return false
    }
    g_toolchain = !empty(cfg.toolchain) ? cfg.toolchain : derive_toolchain(cfg.worktree)
    if (empty(g_toolchain)) {
        logger_error("buildd", "cannot resolve a toolchain id (set --toolchain or --worktree)")
        return false
    }
    // announce=false means the operator handles the go-live; treat as done
    g_announced = !cfg.announce
    g_server = new BuilddHealthServer()
    g_server->init(cfg.port)
    if (!g_server->set_bind_host("127.0.0.1")) {
        logger_error("buildd", "could not bind loopback")
        shutdown_service()
        return false
    }
    let rc = g_server->start()
    if (rc != 0) {
        logger_error("buildd", "could not start on port {cfg.port} (rc {rc}; port in use?)")
        shutdown_service()
        return false
    }
    g_server_started = true
    logger_info("buildd", "listening on http://127.0.0.1:{cfg.port}",
        JV((toolchain = g_toolchain, server_url = cfg.server_url)))
    return true
}

def private scratch_root : string {
    if (!empty(g_cfg.scratch_dir)) {
        return g_cfg.scratch_dir
    }
    var err = ""
    return temp_directory(err)
}

def private run_one_job(job : ClaimedBuild) {
    //! One claimed job, start to finish: materialize, build, upload, clean up.
    //! Every outcome resolves the job on the server — a claim this function
    //! saw must never be left to the stale-requeue sweep.
    g_job_counter ++
    let t0 = ref_time_ticks()
    // Unpredictable, freshly created, and ours: a name derived from the job
    // would be guessable by anyone with a local account on this box, who could
    // then pre-create the output directory or plant a symlink in it before the
    // build runs.
    var mk_err = ""
    let root = create_temp_directory("dasweb-build-", mk_err)
    if (empty(root)) {
        logger_error("buildd", "cannot create scratch dir: {mk_err}", JV((hash = job.hash)))
        upload_failure(g_cfg.server_url, g_cfg.token, job.hash, job.toolchain,
            "builder could not allocate scratch space")
        return
    }
    let src_dir = path_join(root, "src")
    let out_dir = path_join(root, "out")
    mkdir_rec(src_dir)
    mkdir_rec(out_dir)
    logger_info("buildd", "job start", JV((hash = job.hash, mode = job.mode, toolchain = job.toolchain)))
    let entry = unpack_sources(src_dir, job.source)
    if (empty(entry)) {
        logger_warning("buildd", "job failed: malformed source document",
            JV((hash = job.hash, duration_ms = get_time_usec(t0) / 1000)))
        upload_failure(g_cfg.server_url, g_cfg.token, job.hash, job.toolchain,
            "malformed source document")
    } else {
        let r = run_build_command(g_cfg.build_cmd, src_dir, out_dir, job.mode, entry,
            float(g_cfg.build_timeout_seconds))
        if (!r.ok) {
            logger_warning("buildd", "build failed",
                JV((hash = job.hash, exit_code = r.exit_code, duration_ms = get_time_usec(t0) / 1000)))
            upload_failure(g_cfg.server_url, g_cfg.token, job.hash, job.toolchain,
                empty(r.output) ? "build failed (exit code {r.exit_code})" : r.output)
        } else {
            let files <- collect_artifacts(out_dir, job.mode)
            if (empty(files)) {
                logger_warning("buildd", "job failed: no artifacts collected",
                    JV((hash = job.hash, mode = job.mode, duration_ms = get_time_usec(t0) / 1000)))
                upload_failure(g_cfg.server_url, g_cfg.token, job.hash, job.toolchain,
                    "build produced no artifacts\n{r.output}")
            } elif (upload_success(g_cfg.server_url, g_cfg.token, job.hash, job.toolchain, files)) {
                logger_info("buildd", "job done",
                    JV((hash = job.hash, files = length(files), duration_ms = get_time_usec(t0) / 1000)))
            }
        }
    }
    rmdir_rec(root)
}

def update_service : bool {
    //! One tick: pump the health server, and when the poll timer fires,
    //! announce (until it sticks), claim, build. A drained claim polls again
    //! immediately; an idle queue waits poll_interval_seconds.
    return false if (g_server == null)
    g_server->tick()
    if (g_stop_requested) {
        return false
    }
    let now = int64(get_clock())
    if (now >= g_next_poll) {
        if (!g_announced) {
            g_announced = announce_toolchain(g_cfg.server_url, g_cfg.token, g_toolchain)
            if (g_announced) {
                logger_info("buildd", "toolchain announced", JV((toolchain = g_toolchain)))
            }
        }
        var claimed = false
        if (g_announced) {
            let job = claim_build(g_cfg.server_url, g_cfg.token, g_toolchain)
            claimed = job.ok
            if (job.ok) {
                run_one_job(job)
            }
        }
        g_next_poll = claimed ? now : now + int64(g_cfg.poll_interval_seconds)
    }
    sleep(100u)
    return true
}

def shutdown_service {
    //! Release in dependency order; safe after a partially failed init.
    if (g_server != null) {
        if (g_server_started) {
            g_server->stop()
        }
        g_server->cleanup()
        unsafe {
            delete g_server   // class-instance delete requires unsafe; sole owner, tick loop already stopped
        }
        g_server = null
        g_server_started = false
    }
    logger_info("buildd", "service stopped")
}

def run_service(cfg : BuilddArgs) {
    //! Blocking convenience wrapper (tests): init, tick until /shutdown, teardown.
    if (init_service(cfg)) {
        while (update_service()) {
        }
    }
    shutdown_service()
}
