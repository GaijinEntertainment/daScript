options gen2
options indenting = 4

module _model_bench shared public

//! What the model bench (`model_bench.das`) and its runner (`model_bench_run.das`) share, with no engine module behind
//! it: the config file's shape, a model's settings resolved from it, and the run file the bench writes for the runner.

require daslib/fio

//! how a runner starts the server for a model
struct ServerPlan {
    @optional ctx : int                    //! the context a stream gets, in tokens
    @optional streams : int
    @optional flags : array<string>        //! more server arguments, one an element
}

//! how a speculative arm starts each server: more arguments, `\{drafter\}` in one standing for the model's drafter file
struct ArmPlan {
    @optional das : array<string>
    @optional llama : array<string>
}

//! what a model's cells run under: the config's defaults, a field of its own entry over each
struct Settings {
    @optional lengths : array<int>         //! prompt lengths in tokens, ascending; 0 is the task alone
    @optional widths : array<int>
    @optional wide_widths : array<int>
    @optional width_lengths : array<int>
    @optional reply_tokens : int
    @optional eyeball_reply_tokens : int   //! an eyeball turn's reply, where a model reasons ahead of every answer; 0 leaves the bench's own
    @optional arm_reply_tokens : int       //! the reply of a speculative arm's rows, and of the plain rows they are read against: long enough to be past the reply's opening
    @optional reps : int
    @optional cell_seconds : int
    @optional arms : array<string>         //! the arms a runner starts a server for, in order; `off` is the plain one and runs the whole ladder
    @optional drafter : string             //! the draft model of the arms that take one, a path under the models directory
    @optional drafters : table<string; string>   //! an arm's own draft model, where a model has more than one
    @optional cliff_share : float          //! a rate under this share of the rung before it is a cliff
    @optional stall_times : float          //! a gap between reply chunks past this many median gaps is a stall
    @optional unfair_times : float         //! a wave whose last first text comes past this many times its width times the first one's is unfair
    @optional server : ServerPlan
    @optional ultralong_server : ServerPlan   //! the ultralong pass's server: the model's whole context, one stream
    @optional wide_server : ServerPlan     //! the wide pass's server: the streams its widest wave takes, each with room for the longest width prompt
}

struct BenchConfig {
    defaults : Settings
    @optional arm_flags : table<string; ArmPlan>
    models : table<string; Settings>
}

//! one row as a ladder record's `tests` entry
struct TestRow {
    tok_s : double              //! the median over the reps
    stddev : double
    @optional prompt_tokens : int64
    @optional reps : int
    @optional ttft_ms : double  //! a decode row's median time to the first reply text
    @optional server_tok_s : double   //! the same rate on the server's own clock, where it reports one
    @optional stream_tok_s : double   //! a width row's median rate of one stream
    @optional done_ms : double        //! a width row's median time from the wave's send to its last stream's last text
    @optional draft_n : int64         //! a speculative row's drafted tokens over its reps
    @optional draft_n_accepted : int64
    @optional cached_tokens : int64   //! a behind row's prompt tokens read off the cache: its rate is over the rest
    @optional flags : string
}

//! a cell the run could not report
struct Refusal {
    cell : string
    why : string
}

//! what the run was, beside its rows
struct Provenance {
    cmd : string
    url : string
    entry : string
    reply_tokens : int
    reps : int
    cell_seconds : int
    corpus : string
    corpus_bytes : int
    chars_per_token : float
    base_tokens : int64         //! the prompt tokens of the task alone, the chat template's among them
    stopped : string            //! why the ladder ended short of its last length, "" when it did not
}

//! one reply of a run's eyeball: the mode it was asked in (`plain`, `@2`, `:dflash2`), the ask, the reply's opening, why it
//! reads wrong ("" when it reads right), and what a reader should know of a reply that reads right ("" for nothing)
struct EyeballReply {
    mode : string
    ask : string
    why : string
    note : string
    reply : string
}

//! the file a bench run writes (`--out`)
struct RunOut {
    bench : Provenance
    tests : table<string; TestRow>
    refused : array<Refusal>
    @optional eyeball : array<EyeballReply>
}

//! the draft model `arm` takes under `s`: the arm's own, else the model's one; "" where it names none
def drafter_of(s : Settings; arm : string) : string => s.drafters?[arm] ?? s.drafter

def default_config_path() : string => path_join(get_das_root(), "modules/dasLLAMA/harness/model_bench.json")

//! `over`'s set fields of a server plan laid over `base`
def private overlay_server(var base : ServerPlan; over : ServerPlan) {
    base.ctx = over.ctx > 0 ? over.ctx : base.ctx
    base.streams = over.streams > 0 ? over.streams : base.streams
    if (!empty(over.flags)) {
        base.flags := over.flags
    }
}

//! `over`'s set fields laid over `base`
def private overlay(var base : Settings; over : Settings) {
    if (!empty(over.lengths)) {
        base.lengths := over.lengths
    }
    if (!empty(over.widths)) {
        base.widths := over.widths
    }
    if (!empty(over.wide_widths)) {
        base.wide_widths := over.wide_widths
    }
    if (!empty(over.width_lengths)) {
        base.width_lengths := over.width_lengths
    }
    if (!empty(over.arms)) {
        base.arms := over.arms
    }
    base.reply_tokens = over.reply_tokens > 0 ? over.reply_tokens : base.reply_tokens
    base.arm_reply_tokens = over.arm_reply_tokens > 0 ? over.arm_reply_tokens : base.arm_reply_tokens
    base.eyeball_reply_tokens = over.eyeball_reply_tokens > 0 ? over.eyeball_reply_tokens : base.eyeball_reply_tokens
    base.reps = over.reps > 0 ? over.reps : base.reps
    base.cell_seconds = over.cell_seconds > 0 ? over.cell_seconds : base.cell_seconds
    base.cliff_share = over.cliff_share > 0.0 ? over.cliff_share : base.cliff_share
    base.stall_times = over.stall_times > 0.0 ? over.stall_times : base.stall_times
    base.unfair_times = over.unfair_times > 0.0 ? over.unfair_times : base.unfair_times
    overlay_server(base.server, over.server)
    base.drafter = !empty(over.drafter) ? over.drafter : base.drafter
    for (arm, path in keys(over.drafters), values(over.drafters)) {
        base.drafters |> insert(arm, path)
    }
    overlay_server(base.wide_server, over.wide_server)
    overlay_server(base.ultralong_server, over.ultralong_server)
}

//! The config at `path` read into `config`; false when the file does not read.
def read_config(path : string; var config : BenchConfig) : bool => sscan_json(fread(path), config)

//! The settings of `entry`: the defaults, with the entry's own fields over them where the config names it. `known` says whether it did.
def settings_of(var config : BenchConfig; entry : string) : tuple<s : Settings; known : bool> {
    var s := config.defaults
    let known = !empty(entry) && key_exists(config.models, entry)
    if (known) {
        overlay(s, unsafe(config.models[entry]))
    }
    return <- (s <- s, known = known)
}

//! `word` is one of the bench's row kinds, the words its `--only` takes
def is_bench_kind(word : string) : bool => word == "pp" || word == "tg" || word == "width" || word == "eyeball"
