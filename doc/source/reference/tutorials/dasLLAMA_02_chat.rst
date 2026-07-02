.. _tutorial_dasLLAMA_chat:

================================
dasLLAMA-02 — Chat and Templates
================================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Chat
    single: Tutorial; Chat Templates

Chat is plain generation over a templated prompt. This tutorial holds a real
multi-turn conversation with a chat-tuned model, inspects the transcript, and
then lifts the hood on how turns become tokens.

Run it like tutorial 01 (SmolLM2-135M-Instruct works well)::

   daslang.exe -jit tutorials/dasLLAMA/02_chat.das -- path/to/model.gguf

A conversation in three calls
=============================

``create_chat`` resolves the model's chat template — sniffed from the GGUF's
embedded template, falling back to the arch registry — and creates the
session. ``add_user`` queues a message; ``respond`` renders the turn, prefills
it, and streams the reply until a stop token or the ``max_new`` budget.

.. code-block:: das

   var chat = create_chat(m, "You are a helpful, friendly assistant.")
   add_user(chat, "What is the capital of France?")
   respond(m, chat, SamplingParams()) $(piece) {
       fprint(fstdout(), piece)
       fflush(fstdout())
       return true
   }
   // -> The capital of France is Paris.

Multi-turn memory
=================

A follow-up like *"And of Italy?"* only makes sense with the first turn in
context. Nothing is re-prefilled: every turn so far is already in the
session's KV cache, so each ``respond`` only evaluates the new tokens.

.. code-block:: das

   add_user(chat, "And of Italy?")
   // respond(...) -> The capital of Italy is Rome.

The transcript
==============

``chat.history`` records both sides of the conversation (for Qwen3-style
thinking models, with the ``<think>`` block stripped per protocol):

.. code-block:: das

   for (msg in chat.history) {
       print("  {msg.role}: {msg.content}\n")
   }

Under the hood: render_turn
===========================

``render_turn`` shows the exact prefill the next ``respond`` would evaluate —
without running the model — so you can inspect the wrapping and budget tokens:

.. code-block:: das

   add_user(chat, "Thanks!")
   let turn <- render_turn(m, chat)   // read-only: the message stays queued
   print("next turn prefill: {length(turn)} tokens: {turn}\n")
   print("decoded text: \"{decode(m, turn)}\"\n")

On a ChatML-family model the ids come out as
``[ 1, 4093, 198, 16937, 17, 2, 198, 1, 520, 9531, 198]`` — and the decoded
text is just ``user\nThanks!\nassistant\n``. The template's
``<|im_start|>`` / ``<|im_end|>`` wrapping is special *tokens*: atomic ids the
model sees, which ``decode`` renders invisibly. Only the text between them
survives a round-trip.

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/02_chat.das <../../../../tutorials/dasLLAMA/02_chat.das>`

   Next tutorial: :ref:`tutorial_dasLLAMA_sampling`

   The interactive chat REPL: ``examples/dasLLAMA/chat.das``
