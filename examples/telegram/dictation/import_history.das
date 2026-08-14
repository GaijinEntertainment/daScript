options gen2
options rtti

require daslib/clargs
require daslib/fio
require daslib/jobque_boost
require daslib/strings_boost
require daslib/strings_convert
require sqlite/sqlite_migrate
require math
require cadmus_history
require cadmus_rules                  // permanent-vs-transient ASR failure classification
require telegram_history_import

[CommandLineArgs]
struct ImportHistoryArgs {
    @clarg_required
    @clarg_doc = "Telegram Desktop HTML export directory"
    root : string

    @clarg_doc = "Cadmus SQLite database path"
    database : string = "cadmus.db"

    @clarg_required
    @clarg_doc = "Destination Telegram chat ID"
    chat_id : string

    @clarg_doc = "Write parsed messages and media checkpoints; default is dry-run"
    apply : bool

    @clarg_doc = "Raw-transcribe queued audio into Content and RawTranscript (no LLM cleanup)"
    transcribe : bool

    @clarg_doc = "Retry media previously marked failed"
    retry_failed : bool

    @clarg_doc = "Maximum queued audio files to transcribe; 0 means all"
    max_audio : int

    @clarg_doc = "OpenAI-like ASR server base URL"
    server : string = "http://127.0.0.1:8080"

    @clarg_doc = "ASR language hint"
    language : string = "auto"

    @clarg_doc = "ffmpeg executable path"
    ffmpeg : string = "ffmpeg"

    @clarg_doc = "curl executable path (used for long-running multipart ASR requests)"
    curl : string = "curl"

    @clarg_doc = "Parallel ASR request threads (default 2; match the server's --asr-workers)"
    workers : int = 2

    @clarg_doc = "Also process the newest export page (normally skipped while Telegram is exporting)"
    include_last_page : bool

    @clarg_doc = "Maximum pages to process; 0 means all stable pages"
    max_pages : int

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

struct private ImportAsrJob {
    index : int
    item : CadmusImportMedia
    stop : bool
}

enum private ImportAsrEventKind {
    completed
    stopped
}

struct private ImportAsrEvent {
    kind : ImportAsrEventKind
    index : int
    item : CadmusImportMedia
    success : bool
    transcript : string
    error : string
}

def private import_asr_worker(
                              worker_index : int;
                              cfg : TelegramRawAsrConfig;
                              root : string;
                              var jobs : Channel?&;
                              var events : Channel?&
                              ) {
    this_context().name := "cadmus_import_asr_{worker_index}"
    var stopping = false
    while (!stopping) {
        jobs |> pop_with_timeout_clone(250) $(job : ImportAsrJob#) {
            if (job.stop) {
                stopping = true
            } else {
                var item : CadmusImportMedia
                item := job.item
                var transcript = ""
                var error = ""
                let success = raw_transcribe_cadmus_import_media(
                    cfg, root, item, transcript, error)
                events |> push_clone(ImportAsrEvent(
                    kind = ImportAsrEventKind.completed, index = job.index,
                    item = item, success = success, transcript = transcript, error = error))
                delete item
            }
        }
    }
    events |> push_clone(ImportAsrEvent(kind = ImportAsrEventKind.stopped))
    jobs |> release
    events |> release
}

def private transcribe_import_queue(
                                    args : ImportHistoryArgs;
                                    asr : TelegramRawAsrConfig;
                                    queue : array<CadmusImportMedia>
                                    ) : int {
    if (empty(queue)) {
        return 0
    }
    let worker_count = clamp(args.workers, 1, min(length(queue), 8))
    var jobs = unsafe(channel_create())
    var events = unsafe(channel_create())
    for (worker_index in range(worker_count)) {
        let index_capture = worker_index
        let cfg_capture = asr
        let root_capture = args.root
        var jobs_capture = jobs
        var events_capture = events
        new_thread() <| @capture(= index_capture, = cfg_capture, = root_capture,
                                = jobs_capture, = events_capture) {
            import_asr_worker(
                index_capture, cfg_capture, root_capture, jobs_capture, events_capture)
        }
    }
    for (index in range(length(queue))) {
        jobs |> push_clone(ImportAsrJob(index = index, item = queue[index]))
    }
    for (_worker in range(worker_count)) {
        jobs |> push_clone(ImportAsrJob(stop = true))
    }
    var completed = 0
    var stopped = 0
    var failures = 0
    while (completed < length(queue) || stopped < worker_count) {
        events |> pop_with_timeout_clone(1000) $(event : ImportAsrEvent#) {
            if (event.kind == ImportAsrEventKind.stopped) {
                stopped++
            } else {
                var result : ImportAsrEvent
                result := event
                completed++
                print("ASR {completed}/{length(queue)} message {result.item.TelegramMessageId}... ")
                if (result.success) {
                    finish_cadmus_import_transcription(args.database, result.item, result.transcript)
                    print("done ({length(result.transcript)} bytes; source #{result.index + 1})\n")
                } else {
                    failures++
                    let permanent = is_permanent_asr_failure(result.error)
                    fail_cadmus_import_transcription(
                        args.database, result.item, result.error,
                        permanent ? IMPORT_MAX_ATTEMPTS : result.item.Attempts + 1)
                    let verdict = permanent ? "FAILED permanently" : "FAILED"
                    print("{verdict}: {result.error} (source #{result.index + 1})\n")
                }
                delete result
            }
        }
    }
    unsafe {
        jobs |> channel_remove
        events |> channel_remove
    }
    return failures
}

def private export_page_number(path : string) : int {
    let name = stem(base_name(path))
    if (name == "messages") {
        return 1
    }
    if (!starts_with(name, "messages")) {
        return 0
    }
    let parsed = try_to_int(trim_prefix(name, "messages"))
    return parsed |> is_ok ? parsed |> unwrap : 0
}

def private collect_export_pages(root : string) : array<string> {
    var pages : array<string>
    dir(root) $(name) {
        if (name == "." || name == "..") {
            return
        }
        let path = path_join(root, name)
        if (extension(path) == ".html" && export_page_number(path) > 0) {
            pages |> push(path)
        }
    }
    pages |> sort() $(a, b) => export_page_number(a) < export_page_number(b)
    return <- pages
}

def private read_complete_page(path : string; var html : string&; var error : string&) : bool {
    html = fread(path)
    if (empty(html)) {
        error = "could not read {path}"
        return false
    }
    if (!ends_with(strip(html), "</html>")) {
        error = "page is still being written: {path}"
        return false
    }
    return true
}

[export]
def main() : int {   // nolint:STYLE037,STYLE038 - flat CLI-argument validation followed by one linear import pipeline
    var parsed_args <- parse_args(type<ImportHistoryArgs>)
    if (parsed_args |> is_err) {
        print("error: {parsed_args |> unwrap_err}\n\n")
        print_help(get_command_info(type<ImportHistoryArgs>), "import_history")
        return 2
    }
    let args <- parsed_args |> move_unwrap
    if (args.help) {
        print_help(get_command_info(type<ImportHistoryArgs>), "import_history")
        return 0
    }
    let parsed_chat_id = try_to_int64(args.chat_id)
    if (parsed_chat_id |> is_err) {
        print("error: invalid --chat-id '{args.chat_id}'\n")
        return 2
    }
    let chat_id = parsed_chat_id |> unwrap
    if (!stat(args.root).is_dir) {
        print("error: export directory not found: {args.root}\n")
        return 2
    }
    var pages <- collect_export_pages(args.root)
    if (empty(pages)) {
        print("error: no messages*.html pages found under {args.root}\n")
        return 2
    }
    var skipped_newest = ""
    if (!args.include_last_page) {
        skipped_newest = pages[length(pages) - 1]
        pages |> resize(length(pages) - 1)
    }
    if (args.max_pages > 0 && length(pages) > args.max_pages) {
        pages |> resize(args.max_pages)
    }
    if (args.apply || args.transcribe) {
        ensure_cadmus_database(args.database)
    }

    var total_messages = 0
    var total_media = 0
    var missing_sender = 0
    var invalid_dates = 0
    var failed_pages = 0
    var current_sender = ""
    for (page in pages) {
        var html = ""
        var error = ""
        if (!read_complete_page(page, html, error)) {
            failed_pages++
            print("SKIP {base_name(page)}: {error}\n")
            continue
        }
        let messages <- parse_telegram_export_page(html, current_sender, error)
        if (!empty(error)) {
            failed_pages++
            print("SKIP {base_name(page)}: {error}\n")
            continue
        }
        var page_media = 0
        var page_missing_sender = 0
        var page_invalid_dates = 0
        for (message in messages) {
            page_media += empty(message.MediaPath) ? 0 : 1
            page_missing_sender += empty(message.SenderName) ? 1 : 0
            page_invalid_dates += message.MessageDate == 0l ? 1 : 0
        }
        if (args.apply) {
            with_cadmus_database(args.database) $(db) {
                let report = import_telegram_export_page(db, args.root, chat_id, messages)
                page_media = report.Media
                page_missing_sender = report.MissingSender
                page_invalid_dates = report.InvalidDate
            }
        }
        total_messages += length(messages)
        total_media += page_media
        missing_sender += page_missing_sender
        invalid_dates += page_invalid_dates
        print("{args.apply ? "IMPORTED" : "DRY-RUN"} {base_name(page)}: " +
              "{length(messages)} messages, {page_media} media\n")
    }
    print("\n{args.apply ? "Import complete" : "Dry-run complete"}: " +
          "{total_messages} messages, {total_media} media, {length(pages)} pages\n")
    if (!empty(skipped_newest)) {
        print("Newest page skipped for safety: {base_name(skipped_newest)} " +
              "(use --include-last-page after export finishes)\n")
    }
    if (missing_sender > 0 || invalid_dates > 0 || failed_pages > 0) {
        print("Warnings: {missing_sender} missing senders, {invalid_dates} invalid dates, " +
              "{failed_pages} skipped pages\n")
    }
    if (!args.apply && !args.transcribe) {
        print("No database changes were made. Re-run with --apply when the counts look right.\n")
    }
    var transcription_failures = 0
    if (args.transcribe) {
        let queue <- load_cadmus_import_media(
            args.database, chat_id, args.retry_failed, args.max_audio)
        let asr = TelegramRawAsrConfig(
            Server = trim_suffix(args.server, "/"),
            Language = args.language,
            Ffmpeg = args.ffmpeg,
            Curl = args.curl)
        let worker_count = empty(queue) ? 0 : clamp(args.workers, 1, min(length(queue), 8))
        print("\nRaw ASR queue: {length(queue)} audio files, {worker_count} request thread(s) " +
              "(no LLM postprocess)\n")
        transcription_failures = transcribe_import_queue(args, asr, queue)
        print("Raw ASR complete: {length(queue) - transcription_failures} done, " +
              "{transcription_failures} failed\n")
    }
    return (failed_pages == 0 && transcription_failures == 0) ? 0 : 1
}
