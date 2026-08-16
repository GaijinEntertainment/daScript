options gen2

// pr-babysit - the GitHub mechanics of the babysit loop (skills/babysit.md keeps the policy).
// Exit codes: 0 ready to merge, 1 request/api failure, 2 CI red, 3 review to triage,
// 5 watch timeout. Thread resolution is GraphQL-only - REST has none. Bot reviewers never
// appear in requested_reviewers or reviewRequests and Copilot converts a request into a
// review within seconds, so the only trustworthy signal is the review arriving in
// /pulls/N/reviews with commit_id == the requested tip.

require strings
require math
require daslib/fio
require daslib/clargs
require daslib/strings_boost
require daslib/strings_convert
require watch_verdict

[CommandLineArgs]
struct Config {
    @clarg_required
    @clarg_doc = "Pull request number"
    pr : int

    @clarg_doc = "GitHub repository, owner/name"
    repo : string = "GaijinEntertainment/daScript"

    @clarg_doc = "Request a Copilot review of the current tip"
    request_review : bool

    @clarg_doc = "Block until action is needed: CI red (2), review to triage (3), ready to merge (0), timeout (5)"
    watch : bool

    @clarg_doc = "List unresolved review threads, GraphQL id and path per line"
    list_unresolved : bool

    @clarg_doc = "Resolve a review thread by GraphQL id (repeatable)"
    resolve : array<string>

    @clarg_doc = "Watch poll interval, seconds"
    interval_sec : int = 30

    @clarg_doc = "Watch timeout, minutes"
    timeout_min : int = 45
}

def private gh(args : array<string>; var out : string&) : int {
    var argv <- ["gh"]
    argv |> push_from(args)
    return run_and_capture(argv, out, 120.0)
}

def private gh_line(args : array<string>) : string {
    var out : string
    return gh(args, out) == 0 ? trim(out) : ""
}

def private head_sha(cfg : Config) : string {
    return gh_line(["api", "repos/{cfg.repo}/pulls/{cfg.pr}", "--jq", ".head.sha"])
}

// non-empty output lines; ok=false on spawn/api failure so a caller never reads an error page as data
def private gh_lines(args : array<string>; var ok : bool&) : array<string> {
    var out : string
    ok = gh(args, out) == 0
    var lines : array<string>
    if (ok) {
        lines <- [for (l in split(out, "\n")); trim(l); where !empty(trim(l))]
    }
    return <- lines
}

// "<bucket> <name>" per check
def private check_lines(cfg : Config; var ok : bool&) : array<string> {
    return <- gh_lines(["pr", "checks", "{cfg.pr}", "--repo", cfg.repo,
        "--json", "name,bucket", "--jq", ".[] | .bucket + \" \" + .name"], ok)
}

def private bot_review_shas(cfg : Config; var ok : bool&) : array<string> {
    return <- gh_lines(["api", "repos/{cfg.repo}/pulls/{cfg.pr}/reviews", "--paginate",
        "--jq", ".[] | select(.user.type==\"Bot\") | .commit_id"], ok)
}

// "<id> <path>" per unresolved thread; paginated - an undercounted page must never read as zero
def private unresolved_threads(cfg : Config; var ok : bool&) : array<string> {
    let slash = find(cfg.repo, "/")
    let owner = slice(cfg.repo, 0, slash)
    let name = slice(cfg.repo, slash + 1)
    var threads : array<string>
    var cursor = ""
    ok = true
    while (true) {
        let after = empty(cursor) ? "" : ", after: \"{cursor}\""
        let query = "query \{ repository(owner: \"{owner}\", name: \"{name}\") \{ pullRequest(number: {cfg.pr}) \{ reviewThreads(first: 100{after}) \{ pageInfo \{ hasNextPage endCursor \} nodes \{ id isResolved path \} \} \} \} \}"
        var page_ok = false
        let lines <- gh_lines(["api", "graphql", "-f", "query={query}", "--jq",
            "(.data.repository.pullRequest.reviewThreads.pageInfo | (.hasNextPage|tostring) + \" \" + (.endCursor // \"\")), (.data.repository.pullRequest.reviewThreads.nodes[] | select(.isResolved==false) | .id + \" \" + (.path // \"\"))"], page_ok)
        if (!page_ok || empty(lines)) {
            ok = false
            break
        }
        threads |> reserve(length(threads) + length(lines) - 1)
        for (i in range(1, length(lines))) {
            threads |> push(lines[i])
        }
        break if (!(lines[0] |> starts_with("true ")))
        cursor = slice(lines[0], find(lines[0], " ") + 1)
    }
    return <- threads
}

def private unresolved_count(cfg : Config) : int {
    var ok = false
    let threads <- unresolved_threads(cfg, ok)
    return ok ? length(threads) : -1
}

def private resolve_thread(thread_id : string) : bool {
    let mutation = "mutation \{ resolveReviewThread(input: \{ threadId: \"{thread_id}\" \}) \{ thread \{ id \} \} \}"
    var out : string
    return gh(["api", "graphql", "-f", "query={mutation}"], out) == 0
}

def private request_review(cfg : Config) : int {
    let head = head_sha(cfg)
    if (empty(head)) {
        to_log(LOG_ERROR, "pr-babysit: cannot read {cfg.repo}#{cfg.pr} - is gh authenticated?\n")
        return 1
    }
    var out : string
    let rc = gh(["api", "-X", "POST", "repos/{cfg.repo}/pulls/{cfg.pr}/requested_reviewers",
        "-f", "reviewers[]=copilot-pull-request-reviewer[bot]"], out)
    if (rc != 0) {
        to_log(LOG_ERROR, "pr-babysit: review request refused (rc={rc})\n{out}\n")
        return 1
    }
    to_log(LOG_INFO, "pr-babysit: copilot review requested on {slice(head, 0, 9)}\n")
    return 0
}

// re-request rail for a PR Copilot has already reviewed - REST accepts but may not re-trigger
// there; GraphQL requestReviews(botIds, union) is the reported-reliable path
def private request_review_graphql(cfg : Config) : bool {
    let pr_id = gh_line(["api", "repos/{cfg.repo}/pulls/{cfg.pr}", "--jq", ".node_id"])
    let bot_id = gh_line(["api", "repos/{cfg.repo}/pulls/{cfg.pr}/reviews",
        "--jq", "[.[] | select(.user.type==\"Bot\") | .user.node_id] | last"])
    return false if (empty(pr_id) || empty(bot_id) || bot_id == "null")
    let mutation = "mutation \{ requestReviews(input: \{ pullRequestId: \"{pr_id}\", botIds: [\"{bot_id}\"], union: true \}) \{ clientMutationId \} \}"
    var out : string
    return gh(["api", "graphql", "-f", "query={mutation}"], out) == 0
}

def private watch(cfg : Config) : int {
    let polls = max(1, (cfg.timeout_min * 60) / max(1, cfg.interval_sec))
    var last_state = ""
    var escalated = false
    for (poll in range(polls)) {
        if (poll != 0) {
            sleep(uint(cfg.interval_sec * 1000))
        }
        let head = head_sha(cfg)
        var checks_ok = false
        var reviews_ok = false
        let checks <- check_lines(cfg, checks_ok)
        let reviews <- bot_review_shas(cfg, reviews_ok)
        let unresolved = unresolved_count(cfg)
        if (empty(head) || !checks_ok || !reviews_ok || unresolved < 0) {
            to_log(LOG_WARNING, "pr-babysit: api read failed, retrying\n")
            continue
        }
        var fails : array<string>
        var pending = 0
        for (c in checks) {
            if (c |> starts_with("fail ")) {
                fails |> push(slice(c, 5))
            } elif (c |> starts_with("pending ")) {
                pending++
            }
        }
        let reviewed = reviews |> has_value(head)
        let state = "tip {slice(head, 0, 9)}: {length(fails)} failed, {pending} pending, reviewed={reviewed}, unresolved={unresolved}"
        if (state != last_state) {
            to_log(LOG_INFO, "pr-babysit: {state}\n")
            last_state = state
        }
        let verdict = watch_verdict(length(fails), pending, reviewed, unresolved)
        if (verdict == WatchVerdict.ci_red) {
            to_log(LOG_ERROR, "pr-babysit: CI red - {join(fails, ", ")}\n")
            return 2
        }
        return 3 if (verdict == WatchVerdict.review_to_triage)
        return 0 if (verdict == WatchVerdict.ready_to_merge)
        // REST accepted but never re-triggered
        if (cfg.request_review && !escalated && !reviewed && poll >= polls / 2) {
            escalated = true
            let sent = request_review_graphql(cfg)
            to_log(LOG_WARNING, "pr-babysit: no review at half timeout - GraphQL re-request {sent ? "sent" : "unavailable"}\n")
        }
    }
    to_log(LOG_ERROR, "pr-babysit: nothing actionable within {cfg.timeout_min} min\n")
    return 5
}

[export]
def main() : int {
    var cfg = Config()
    let rc = parse_args_with_help(cfg, "pr-babysit")
    return rc if (rc >= 0)
    if (!cfg.request_review && !cfg.watch && !cfg.list_unresolved && empty(cfg.resolve)) {
        to_log(LOG_ERROR, "pr-babysit: pick --request-review, --watch, --list-unresolved, and/or --resolve\n")
        return 1
    }
    for (tid in cfg.resolve) {
        if (!resolve_thread(tid)) {
            to_log(LOG_ERROR, "pr-babysit: failed to resolve thread {tid}\n")
            return 1
        }
        to_log(LOG_INFO, "pr-babysit: resolved {tid}\n")
    }
    if (cfg.list_unresolved) {
        var ok = false
        let threads <- unresolved_threads(cfg, ok)
        if (!ok) {
            to_log(LOG_ERROR, "pr-babysit: cannot read review threads\n")
            return 1
        }
        for (t in threads) {
            to_log(LOG_INFO, "{t}\n")
        }
        to_log(LOG_INFO, "pr-babysit: {length(threads)} unresolved\n")
    }
    if (cfg.request_review) {
        let request_rc = request_review(cfg)
        return request_rc if (request_rc != 0)
    }
    return cfg.watch ? watch(cfg) : 0
}
