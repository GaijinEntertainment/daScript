options gen2

module cli_repl shared

//! The `dasllama-cli chat` loop's logic with no model and no terminal in it: a line classifies
//! as a slash command or a message, a `"""` line opens and closes a multi-line block, and a
//! conversation saves to and loads from JSON. cli.das drives the engine off these.

require daslib/json_boost
require daslib/strings_boost   // join
require daslib/fio             // /read, /save, /load
require strings

enum ReplOp {
    nothing   // a blank line, or a line swallowed into an open block
    say       // a message for the model
    regen     // regenerate the last reply
    clear     // drop the conversation
    system    // replace the system prompt (an empty argument prints it)
    image     // attach an image to the next message
    audio     // attach an audio clip to the next message
    read      // paste a file's text as the next message
    save      // write the conversation to a JSON file
    load      // replace the conversation from a JSON file
    stats     // the last reply's counters
    speak     // on | off: route replies through the speech model
    help
    exit
    unknown
}

struct ReplLine {
    op : ReplOp
    arg : string
}

struct ReplTurn {
    user : string
    assistant : string
}

//! What the loop keeps between lines: the conversation as the user typed it, the attachments
//! queued for the next message, the speak toggle, and the multi-line block being collected.
struct ReplState {
    system : string
    turns : array<ReplTurn>
    images : array<string>
    audio : array<string>
    speak : bool
    in_block : bool
    lines : array<string>
}

let BLOCK_MARK = "\"\"\""

let REPL_HELP_LINES : array<string> <- [
    "/help              this list",
    "/exit              quit (an end of input at the prompt quits too)",
    "/clear             forget the conversation",
    "/regen             answer the last message again",
    "/system <text>     replace the system prompt (no text: show it)",
    "/image <path>      attach an image to the next message (needs --image-mmproj)",
    "/audio <path>      attach an audio clip to the next message (needs --audio-mmproj)",
    "/read <path>       send a file's text as the next message",
    "/save <path>       write the conversation as JSON",
    "/load <path>       replace the conversation from a JSON file",
    "/stats             the last reply's token counts and rates",
    "/speak on|off      speak every reply through the --tts model",
    "\"\"\"                open or close a multi-line message"
]

def repl_help() : string => "{join(REPL_HELP_LINES, "\n")}\n"

def private op_of(name : string) : ReplOp {
    if (name == "/help" || name == "/?") return ReplOp.help
    if (name == "/exit" || name == "/quit" || name == "/q") return ReplOp.exit
    if (name == "/clear") return ReplOp.clear
    if (name == "/regen") return ReplOp.regen
    if (name == "/system") return ReplOp.system
    if (name == "/image") return ReplOp.image
    if (name == "/audio") return ReplOp.audio
    if (name == "/read") return ReplOp.read
    if (name == "/save") return ReplOp.save
    if (name == "/load") return ReplOp.load
    if (name == "/stats") return ReplOp.stats
    if (name == "/speak") return ReplOp.speak
    return ReplOp.unknown
}

//! One line classified on its own: a slash command with its argument, a message, or nothing.
//! A `/` the user meant literally is not a command shape this loop knows, so it reads as
//! `unknown` and the driver says so rather than sending it.
def parse_repl_line(raw : string) : ReplLine {
    let line = strip(raw)
    if (empty(line)) {
        return ReplLine(op = ReplOp.nothing)
    }
    if (!starts_with(line, "/")) {
        return ReplLine(op = ReplOp.say, arg = line)
    }
    let sp = find(line, " ")
    let name = sp < 0 ? line : slice(line, 0, sp)
    let arg = sp < 0 ? "" : strip(slice(line, sp + 1))
    return ReplLine(op = op_of(name), arg = arg)
}

//! One raw line through the block state: a `"""` line opens a block, the next closes it and
//! the joined lines come back as one `say`; inside a block every line is swallowed as
//! `nothing`. Outside a block this is `parse_repl_line`.
def repl_feed(var st : ReplState; raw : string) : ReplLine {
    let line = strip(raw)
    if (line == BLOCK_MARK) {
        if (!st.in_block) {
            st.in_block = true
            st.lines |> clear()
            return ReplLine(op = ReplOp.nothing)
        }
        st.in_block = false
        let text = join(st.lines, "\n")
        st.lines |> clear()
        return empty(strip(text)) ? ReplLine(op = ReplOp.nothing) : ReplLine(op = ReplOp.say, arg = text)
    }
    if (st.in_block) {
        st.lines |> push(raw)
        return ReplLine(op = ReplOp.nothing)
    }
    return parse_repl_line(raw)
}

//! What a streamed piece prints: the reasoning when it is shown, one blank line at the turn from
//! reasoning to answer, the answer always. `thought` carries the state across pieces.
def think_print(show_thinking : bool; rsn, cnt : string; var thought : bool&) : string {
    var out = ""
    if (show_thinking && !empty(rsn)) {
        out = rsn
        thought = true
    }
    if (!empty(cnt)) {
        if (thought) {
            out = "{out}\n\n"
            thought = false
        }
        out = "{out}{cnt}"
    }
    return out
}

//! The conversation as JSON: `{"system": ..., "turns": [{"user": ..., "assistant": ...}, ...]}`.
def repl_save_json(st : ReplState) : string {
    var turns <- [for (tr in st.turns); JV((user = tr.user, assistant = tr.assistant))]
    var doc = JV((system = st.system, turns = turns))
    let text = write_json(doc)
    unsafe {
        delete doc
    }
    return text
}

//! Replace the conversation from `repl_save_json`'s shape; false with the reason when the text
//! is not that shape. The attachments and the block state are dropped either way on success.
def repl_load_json(text : string; var st : ReplState; var err : string&) : bool {
    var root = read_json(text, err)
    if (root == null) {
        return false
    }
    if (!(root.value is _object) || !key_exists(root.value as _object, "turns")) {
        err = "not a saved conversation - no `turns` array"
        unsafe {
            delete root
        }
        return false
    }
    let tv = root?["turns"]
    if (tv == null || !(tv.value is _array)) {
        err = "not a saved conversation - `turns` is not an array"
        unsafe {
            delete root
        }
        return false
    }
    st.system = root?["system"] ?? ""
    st.turns |> clear()
    st.images |> clear()
    st.audio |> clear()
    st.in_block = false
    st.lines |> clear()
    st.turns |> reserve(length(tv.value as _array))
    for (e in tv.value as _array) {
        st.turns |> push(ReplTurn(user = e?["user"] ?? "", assistant = e?["assistant"] ?? ""))
    }
    unsafe {
        delete root
    }
    return true
}

// ===== the loop =====

//! What the loop drives and never owns: where lines come from, where text goes, and the model.
//! cli.das implements it over a loaded model and the terminal; a test implements it over arrays.
class ReplEngine {
    //! The next input line; false at the end of input.
    def abstract read_line(var line : string&) : bool
    //! Text for the user.
    def abstract emit(text : string) : void
    //! One message through the model: the attachments queued on `st` first (consumed), then
    //! `text`; the turn lands on `st.turns` and is spoken when `st.speak` holds.
    def abstract say(var st : ReplState; text : string) : void
    //! A fresh session under `st.system`, `st.turns` replayed as known exchanges.
    def abstract reopen(var st : ReplState) : void
    //! The last reply's counters; "" before any reply.
    def abstract last_stats : string
    //! Whether a speech model is armed for /speak on.
    def abstract can_speak : bool
}

//! The commands that change the conversation itself: a message, a pasted file, a redo, a reset,
//! a new system prompt, a loaded transcript. False when `l` is none of them.
def private repl_conversation_op(var st : ReplState; var eng : ReplEngine?; l : ReplLine) : bool {
    if (l.op == ReplOp.say) {
        eng->say(st, l.arg)
    } elif (l.op == ReplOp.read) {
        let body = fread(l.arg)
        if (empty(body)) {
            eng->emit("could not read {l.arg}\n")
        } else {
            eng->say(st, body)
        }
    } elif (l.op == ReplOp.regen) {
        if (empty(st.turns)) {
            eng->emit("nothing to regenerate yet\n")
        } else {
            let again = st.turns[length(st.turns) - 1].user
            st.turns |> pop()
            eng->reopen(st)
            eng->say(st, again)
        }
    } elif (l.op == ReplOp.clear) {
        st.turns |> clear()
        eng->reopen(st)
        eng->emit("conversation cleared\n")
    } elif (l.op == ReplOp.system) {
        if (empty(l.arg)) {
            eng->emit(empty(st.system) ? "no system prompt\n" : "system: {st.system}\n")
        } else {
            st.system = l.arg
            eng->reopen(st)
            eng->emit("system prompt set\n")
        }
    } elif (l.op == ReplOp.load) {
        let body = fread(l.arg)
        var lerr = ""
        if (empty(body) || !repl_load_json(body, st, lerr)) {
            let because = empty(lerr) ? "" : ": {lerr}"
            eng->emit("could not load {l.arg}{because}\n")
        } else {
            eng->reopen(st)
            eng->emit("loaded {length(st.turns)} turn(s) from {l.arg}\n")
        }
    } else {
        return false
    }
    return true
}

//! The commands beside the conversation: attachments (one media kind per message - the later
//! attachment replaces the other kind), the transcript file, the toggles, help.
def private repl_side_op(var st : ReplState; var eng : ReplEngine?; l : ReplLine) {
    if (l.op == ReplOp.help) {
        eng->emit(repl_help())
    } elif (l.op == ReplOp.image) {
        if (!empty(st.audio)) {
            st.audio |> clear()
            eng->emit("the queued audio is dropped - one media kind per message\n")
        }
        if (!empty(st.images)) {   // one image per message (the turn takes one span); clips may stack
            st.images |> clear()
            eng->emit("this replaces the queued image - one image per message\n")
        }
        st.images |> push(l.arg)
        eng->emit("image queued for the next message\n")
    } elif (l.op == ReplOp.audio) {
        if (!empty(st.images)) {
            st.images |> clear()
            eng->emit("the queued image is dropped - one media kind per message\n")
        }
        st.audio |> push(l.arg)
        eng->emit("audio queued for the next message\n")
    } elif (l.op == ReplOp.save) {
        eng->emit(fwrite(l.arg, repl_save_json(st)) ? "saved {l.arg}\n" : "could not write {l.arg}\n")
    } elif (l.op == ReplOp.stats) {
        let stats = eng->last_stats()
        eng->emit(empty(stats) ? "no reply yet\n" : "{stats}\n")
    } elif (l.op == ReplOp.speak) {
        let on = l.arg == "on"
        if (on && !eng->can_speak()) {
            eng->emit("no speech model - start with --tts <gguf>\n")
        } else {
            st.speak = on
            eng->emit(on ? "replies will be spoken\n" : "replies will not be spoken\n")
        }
    } else {
        eng->emit("unknown command - /help lists them\n")
    }
}

//! One line through the loop's state and the engine; false on /exit.
def repl_step(var st : ReplState; var eng : ReplEngine?; raw : string) : bool {
    let l = repl_feed(st, raw)
    return true if (l.op == ReplOp.nothing)
    return false if (l.op == ReplOp.exit)
    if (!repl_conversation_op(st, eng, l)) {
        repl_side_op(st, eng, l)
    }
    return true
}

//! The loop: lines from the engine until the input ends or /exit.
def repl_run(var st : ReplState; var eng : ReplEngine?) {
    var line = ""
    while (eng->read_line(line)) {
        break if (!repl_step(st, eng, line))
    }
}
