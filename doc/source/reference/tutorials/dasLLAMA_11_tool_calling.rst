.. _tutorial_dasLLAMA_tool_calling:

============================
dasLLAMA-11 — Tool Calling
============================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Tool Calling
    single: Tutorial; Function Calling

A tool-calling conversation is a loop: you declare functions, the model
replies with a *call* instead of an answer, you run the function and hand the
result back, and the model answers with it. This tutorial walks one full round
of that loop.

Run it with a tool-capable GGUF (Qwen3-0.6B-Q8_0 works)::

   daslang.exe -jit tutorials/dasLLAMA/11_tool_calling.das -- path/to/Qwen3-0.6B-Q8_0.gguf

Declare the tools
=================

``set_tools`` takes the conversation's tools — verbatim OpenAI ``tools[]``
JSON objects — before the first turn renders; the system turn then carries the
model family's tool block. A family with no tool format ignores the
declaration. ``render_turn`` (tutorial 02) shows the exact result:

.. das-doc: given var m = Model()
.. code-block:: das

   let WEATHER_TOOL = ("\{\"type\": \"function\", \"function\": \{\"name\": \"get_weather\", " +
       "\"description\": \"Get the current weather in a city\", " +
       "\"parameters\": \{\"type\": \"object\", \"properties\": \{\"city\": \{\"type\": \"string\"}}, " +
       "\"required\": [\"city\"]}}}")
   var chat = create_chat(m, "You are a helpful assistant.", 512l)
   var tools <- [WEATHER_TOOL]
   set_tools(chat, tools)
   add_user(chat, "What is the weather in Paris right now?")
   print("{decode(m, render_turn(m, chat))}\n")   // the system turn now carries the defs

The model calls the tool
========================

The reply to a tool-needing question is a call, spelled in the family's wire
format — hermes (Qwen) wraps a JSON object in ``<tool_call>`` markers.
``parse_calls`` splits any complete reply into reasoning, content, and calls
per that format; each call's ``args`` comes back normalized to JSON object
text:

.. code-block:: das

   let reply = respond(m, chat, SamplingParams()) $(_piece) => true
   var tr <- parse_calls(chat, reply)
   for (c in tr.calls) {
       print("call: {c.name}({c.args})\n")   // call: get_weather(\{"city": "Paris"})
   }

Answer the call
===============

Tool results are the reply *to the model*: queue them with
``add_tool_results`` in place of ``add_user`` — one result per call, in call
order — then ``respond`` again. The model reads the results and answers the
original question:

.. code-block:: das

   var results <- ["\{\"temp_c\": 21, \"sky\": \"clear\"}"]
   add_tool_results(chat, results)
   respond(m, chat, SamplingParams()) $(piece) {
       fprint(fstdout(), piece)
       fflush(fstdout())
       return true
   }
   // -> The current weather in Paris is clear with a temperature of 21°C.

Beyond one round
================

The loop generalizes: parse every reply, and while it carries calls, run them
and ``add_tool_results``; when it carries none, the content is the answer.
Hosts that key results by call id use the named overload —
``add_tool_results(chat, results, names)`` — so the name-spelling families
(harmony, gemma-4) pair results correctly even out of call order. A stateless
server replays a past call turn with ``render_assistant_calls``.

Five wire formats ride the same verbs: hermes (Qwen), Harmony (gpt-oss),
gemma-4's bracketed DSL, Mistral's ``[TOOL_CALLS]`` arrays, and Llama-3's
whole-reply JSON. The OpenAI-compatible server
(:ref:`utils/dasllama-server <utils_dasllama_server>`) serves ``tools[]``
requests over exactly this surface.

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/11_tool_calling.das <../../../../tutorials/dasLLAMA/11_tool_calling.das>`

   Thinking models and the reasoning split: :ref:`tutorial_dasLLAMA_thinking`

   Chat basics: :ref:`tutorial_dasLLAMA_chat`
