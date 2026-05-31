A pure-daslang client for OpenAI-compatible REST APIs — OpenAI itself, plus any
server that speaks the same surface (Ollama, OpenRouter, Kokoro, LM Studio,
vLLM, …). Built on ``dashv`` for HTTP and ``daslib/json_boost`` for
serialization.

The API is split into one module per section; ``require`` only what you use:

.. code-block:: das

    require openai/openai_chat          // chat completions, tools, JSON mode, streaming
    require openai/openai_embeddings    // text embeddings
    require openai/openai_models        // list / retrieve models
    require openai/openai_completions   // legacy text completions
    require openai/openai_audio         // text-to-speech and transcription
    require openai/openai_moderations   // content moderation
    require openai/openai_images        // image generation
    require openai/openai_vision        // image-input chat

Each section module re-exports ``openai_common`` (the client, the unified
``OpenAIError``, and the HTTP transport), so requiring a section is enough.

Every consumer root MUST set ``options rtti`` — otherwise the ``@optional`` /
``@rename`` field annotations are ignored and requests serialize every field
(e.g. ``"n":0``), which real servers reject:

.. code-block:: das

    options rtti
    require openai/openai_chat

    let client = openai_client("https://api.openai.com/v1", get_env("OPENAI_API_KEY"))
    print("{chat_text(client, "gpt-4o-mini", "Say hi in one sentence.")}\n")

See :ref:`tutorial_dasOPENAI_first_chat` for a hands-on tutorial, or the rest of
the :ref:`dasOPENAI tutorials <tutorials_dasopenai>`.
