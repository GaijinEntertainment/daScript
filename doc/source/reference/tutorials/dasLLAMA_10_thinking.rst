.. _tutorial_dasLLAMA_thinking:

=============================
dasLLAMA-10 — Thinking Models
=============================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Thinking
    single: Tutorial; Reasoning

A thinking model works through the question before answering, and the reply's
wire format carries that reasoning span: Qwen3 and GLM wrap it in
``<think>...</think>``, gpt-oss speaks Harmony channels, gemma-4 a thought
channel. This tutorial splits replies into reasoning and answer — buffered and
live-streamed — and then turns thinking off.

Run it with a thinking-family GGUF (Qwen3-0.6B-Q8_0 works)::

   daslang.exe -jit tutorials/dasLLAMA/10_thinking.das -- path/to/Qwen3-0.6B-Q8_0.gguf

The reply carries its reasoning
===============================

``respond`` streams the raw pieces — reasoning markers included — while
``chat.history`` stores the reply already reasoning-stripped (tutorial 02).
Thinking burns token budget, so raise ``max_new`` past its 256 default:

.. das-doc: given var m = Model()
.. code-block:: das

   var chat = create_chat(m, "You are a concise assistant.", 1024l)
   add_user(chat, "Which is larger: 17 * 24 or 20 * 20? One sentence.")
   let full = respond(m, chat, SamplingParams()) $(_piece) => true   // buffered: collect only
   // full starts "<think> ..." — the raw wire; history stores only the answer

split_reasoning: the buffered split
===================================

``split_reasoning`` splits a complete reply at the family's reasoning
boundary. Both halves come back stripped when a reasoning span was found; a
reply with no reasoning passes through untouched — so it is safe to run on
every reply, thinking model or not.

.. code-block:: das

   let sp = split_reasoning(chat, full)
   print("reasoning: {sp.reasoning}\n")   // the model's work: 17*24 = 408, 20*20 = 400 ...
   print("content:   {sp.content}\n")     // the answer: 17 * 24 is larger than 20 * 20.

Streaming the split: think_feed / think_finish
==============================================

A live UI wants the reasoning marked *while* it streams, not after. The
``ThinkStream`` splitter turns each streamed piece into (reasoning, content)
deltas, holding partial markers across piece boundaries. Create it for the
turn, feed every piece, and flush at the end — an unclosed reasoning span (the
token budget cut mid-thought) classifies as reasoning:

.. code-block:: das

   var chat2 = create_chat(m, "You are a concise assistant.", 1024l)
   add_user(chat2, "What is the capital of Australia? One sentence.")
   var ts = make_think_stream(chat2)
   respond(m, chat2, SamplingParams()) $(piece) {
       var r = ""
       var c = ""
       think_feed(ts, piece, r, c)   // overwritten per piece; either may be empty
       fprint(fstdout(), c)          // show the answer live; dim or hide r as you like
       fflush(fstdout())
       return true
   }
   var tail_r = ""
   var tail_c = ""
   think_finish(ts, tail_r, tail_c)

On a model with no reasoning format the stream is a pass-through: every piece
comes back as content. And a small model sometimes ends a *later* turn inside
its think block — the flush then returns the tail as reasoning, and the
content half stays empty; the tutorial file shows the pattern that handles
both endings.

When the reply is already whole — a captured ``respond`` return, a server's
non-streaming route — ``think_drain`` runs the same splitter in one call:
feed, finish, and the strip rule together:

.. code-block:: das

   var ts2 = make_think_stream(chat2)
   let full = respond(m, chat2, SamplingParams()) $(piece) {
       return true   // capture only
   }
   let sp = think_drain(ts2, full)   // ThinkSplit: .reasoning / .content

Turning thinking off
====================

Hybrid families answer directly when the turn opens with the template's empty
think block. ``set_thinking(false)`` renders exactly that (a no-op for models
with no think specials in the vocabulary); the default is on.
``effective_stop_ids`` is the merged view of every stop in force — the
template's stops plus the thinking-off extras while thinking is off:

.. code-block:: das

   set_thinking(chat, false)
   let stops <- effective_stop_ids(chat)
   add_user(chat, "And 12 * 12 vs 11 * 13? One sentence.")
   // respond(...) now answers directly — no <think> span on the wire

A loop that samples the tokens itself splits that list in two.
``turn_stop_ids`` are the template's own stops and end the turn outright;
``make_nothink_guard`` carries the thinking-off extras, and
``nothink_stop_here`` decides each token: a channel marker before the reply's
first content piece is a leading thought the reply matcher splits (gemma-4's
E-series opens a media turn with one even in instruct mode), a marker after
content ends the turn:

.. code-block:: das

   let turn_stops <- turn_stop_ids(chat)
   var guard <- make_nothink_guard(chat)
   let prompt <- render_turn(m, chat)
   generate(m, chat.session, prompt, SamplingParams(), 96l) $(id, piece) {
       return false if (find_index(turn_stops, id) >= 0)
       return false if (nothink_stop_here(guard, id, piece))
       print("{piece}")
       return true
   }

A reasoning budget
==================

A server caps how long a reply may think with the request's ``thinking_budget``:
once the reply has spent that many tokens inside its reasoning span, the
scheduler writes the family's own close for it and the answer follows. The
scheduler reads tokens, not text, so the span's bounds travel as token marks.
``think_budget_marks`` builds them for the family the chat is on: the sequence a
reply writes to open the span (Qwen3's ``<think>``; gemma-4's ``<|channel>``
followed by the word ``thought``; gpt-oss's channel mark followed by
``analysis``), the one token the model writes to leave it, and the tokens the
budget forces at the cut - a symmetric family's close special after the recipe's
"Considering the limited time..." sentence, each followed by the template's
blank line. A family with no reasoning span returns empty marks, and a budget is
ignored on it:

.. code-block:: das

   var marks <- think_budget_marks(m, chat)
   if (!empty(marks.close)) {
       print("open: {marks.open}  close token: {marks.close_tok}  forced: {decode(m, marks.close)}\n")
   }

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/10_thinking.das <../../../../tutorials/dasLLAMA/10_thinking.das>`

   Next tutorial: :ref:`tutorial_dasLLAMA_tool_calling`

   Chat basics and the transcript: :ref:`tutorial_dasLLAMA_chat`
