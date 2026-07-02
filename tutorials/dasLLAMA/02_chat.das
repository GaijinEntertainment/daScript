options gen2

require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio
require strings

// Tutorial dasLLAMA-02: Chat and Templates
//
// This tutorial covers:
//   - Holding a conversation: create_chat / add_user / respond
//   - Multi-turn memory — the whole conversation lives in the session's KV cache
//   - The transcript: chat.history
//   - Under the hood: chat templates and render_turn
//
// Prerequisites: tutorial 01; a chat-tuned GGUF (SmolLM2-135M-Instruct works).
//
// Run: daslang.exe -jit tutorials/dasLLAMA/02_chat.das -- <path/to/model.gguf>

let SYSTEM = "You are a helpful, friendly assistant. Keep answers concise."

// Resolve the model path: last command-line argument ending in .gguf, else $DASLLAMA_MODEL.
def model_path() : string {
    let cmd <- get_command_line_arguments()
    if (!empty(cmd) && ends_with(cmd[length(cmd) - 1], ".gguf")) {
        return cmd[length(cmd) - 1]
    }
    return get_env_variable("DASLLAMA_MODEL")
}

// One turn: queue the user message, stream the reply to stdout, return it.
def ask(m : Model; var chat : ChatSession; text : string) : string {
    add_user(chat, text)
    print("You: {text}\nBot: ")
    let reply = respond(m, chat, SamplingParams()) $(piece) {
        fprint(fstdout(), piece)
        fflush(fstdout())
        return true
    }
    print("\n")
    return reply
}

[export]
def main() {
    let path = model_path()
    if (empty(path)) {
        print("usage: daslang -jit tutorials/dasLLAMA/02_chat.das -- <model.gguf>\n")
        return
    }
    var m <- load_model(path, QuantMode.q8)

    with_job_que() {
        set_jobque_fork_pool(true, true)

        // ──────────────────────────────────────────────────────────────────────
        // Section 1 — A conversation in three calls
        // ──────────────────────────────────────────────────────────────────────
        //
        // create_chat resolves the model's chat template (sniffed from the GGUF's
        // embedded template, falling back to the arch registry) and creates the
        // session. add_user queues a message; respond renders the turn, prefills it,
        // and streams the reply until a stop token or the max_new budget.
        //
        // Greedy sampling keeps this tutorial deterministic — real chat usually wants
        // temp > 0 (see chat.das in examples/dasLLAMA, and tutorial 03).

        var chat = create_chat(m, SYSTEM)
        ask(m, chat, "What is the capital of France?")

        // ──────────────────────────────────────────────────────────────────────
        // Section 2 — Multi-turn memory
        // ──────────────────────────────────────────────────────────────────────
        //
        // The follow-up only makes sense with the first turn in context. Nothing is
        // re-prefilled: every turn so far is already in the session's KV cache, so
        // each respond() only evaluates the new tokens.

        ask(m, chat, "And of Italy?")

        // ──────────────────────────────────────────────────────────────────────
        // Section 3 — The transcript
        // ──────────────────────────────────────────────────────────────────────
        //
        // chat.history records both sides of the conversation (for Qwen3-style
        // thinking models, with the <think> block stripped per protocol).

        print("\ntranscript ({length(chat.history)} messages):\n")
        for (msg in chat.history) {
            print("  {msg.role}: {msg.content}\n")
        }

        // ──────────────────────────────────────────────────────────────────────
        // Section 4 — Under the hood: the template and render_turn
        // ──────────────────────────────────────────────────────────────────────
        //
        // Chat is plain generation over a templated prompt. render_turn shows the
        // exact prefill the next respond() would evaluate — WITHOUT running the
        // model — so you can inspect the wrapping and budget tokens.

        if (!empty(m.chat_template_str)) {
            print("\nthis GGUF embeds a chat template ({length(m.chat_template_str)} chars) — create_chat sniffed it\n")
        }
        add_user(chat, "Thanks!")
        let turn <- render_turn(m, chat)   // read-only: the message stays queued
        print("next turn prefill: {length(turn)} tokens: {turn}\n")
        // The template's <|im_start|>/<|im_end|> wrapping is special TOKENS — atomic ids the
        // model sees but decode() renders invisibly. Only the text between them survives:
        print("decoded text: \"{decode(m, turn)}\"\n")
        print("Bot: ")
        respond(m, chat, SamplingParams()) $(piece) {   // now actually answer it
            fprint(fstdout(), piece)
            fflush(fstdout())
            return true
        }
        print("\n")
    }
}
