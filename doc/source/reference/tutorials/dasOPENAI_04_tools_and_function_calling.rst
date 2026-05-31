.. _tutorial_dasOPENAI_tools:

===========================================
OPENAI-04 — Tools and Function Calling
===========================================

.. index::
    single: Tutorial; OpenAI
    single: Tutorial; dasOPENAI
    single: Tutorial; Function Calling
    single: Tutorial; Tools

Function calling lets the model ask your program to run a function. The model
does not run anything itself — it returns a ``tool_calls`` request, your program
executes it, and you send the result back so the model can use it.

Declaring a tool
================

A ``Tool`` wraps a ``FunctionDef``. ``parameters`` is a raw JSON-schema string
describing the arguments the model should fill in:

.. code-block:: das

   def weather_tool() : Tool {
       let params = ("\{\"type\":\"object\"," +
           "\"properties\":\{\"location\":\{\"type\":\"string\"\}\}," +
           "\"required\":[\"location\"]\}")
       return Tool(_type = "function",
           _function = FunctionDef(name = "get_weather",
               description = "Get the current weather for a location",
               parameters = params))
   }

(In daslang string literals ``{`` and ``}`` start interpolation, so literal JSON
braces are escaped as ``\{`` / ``\}``.)

Reading the tool call
=====================

Send the tools with the request. When the model wants a tool, ``content`` is
empty (some servers send ``content:null`` — ``sscan_json`` decodes that fine)
and ``tool_calls`` is populated:

.. code-block:: das

   let req = ChatCompletionRequest(model = "gpt-4o-mini",
       messages <- [ChatMessage(role = "user", content = "What's the weather in Paris?")],
       tools <- [weather_tool()])
   let res = chat(client, req)

   let tcall = res.response.choices[0].message.tool_calls[0]
   print("name={tcall._function.name} args={tcall._function.arguments}\n")

Completing the round trip
=========================

Execute the function yourself, then send the result back as a message with role
``tool``, tagged with the ``tool_call_id`` the model gave you. The conversation
includes the user turn, the assistant's tool-call turn, and your tool result:

.. code-block:: das

   let req2 = ChatCompletionRequest(model = "gpt-4o-mini", messages <- [
       ChatMessage(role = "user", content = "What's the weather in Paris?"),
       ChatMessage(role = "assistant", content = "",
           tool_calls <- [ToolCall(id = tcall.id, _type = "function",
               _function = ToolCallFunction(name = tcall._function.name,
                   arguments = tcall._function.arguments))]),
       ChatMessage(role = "tool", tool_call_id = tcall.id,
           content = "\{\"temp_c\":18,\"sky\":\"clear\"\}")])
   let res2 = chat(client, req2)
   // res2 now contains the model's natural-language answer using the tool result

Quick Reference
===============

.. list-table::
   :header-rows: 1
   :widths: 45 55

   * - Type / field
     - Description
   * - ``Tool`` / ``FunctionDef``
     - A declarable function (``parameters`` = JSON schema)
   * - ``req.tools <- [ … ]``
     - Offer tools on a request
   * - ``message.tool_calls``
     - The tool calls the model requested
   * - ``ToolCall._function.name`` / ``.arguments``
     - Function name + JSON argument string
   * - ``role = "tool"`` + ``tool_call_id``
     - Return a tool result to the model

.. seealso::

   Full source: :download:`tutorials/dasOPENAI/04_tools_and_function_calling.das <../../../../tutorials/dasOPENAI/04_tools_and_function_calling.das>`

   Next tutorial: :ref:`tutorial_dasOPENAI_embeddings_models`
