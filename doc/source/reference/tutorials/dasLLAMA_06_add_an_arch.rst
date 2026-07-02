.. _tutorial_dasLLAMA_add_an_arch:

=======================================
dasLLAMA-06 — The Architecture Registry
=======================================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Architecture Registry

How does one ``load_model`` run Llama, Qwen, Phi, Gemma, and gpt-oss? Through
the architecture registry. This tutorial needs no model file and no ``-jit`` —
it's about the registry, not inference::

   daslang.exe tutorials/dasLLAMA/06_add_an_arch.das

The registry
============

Requiring ``dasllama/dasllama`` registers every supported architecture (each
``dasllama_arch_*.das`` self-registers at ``[init]``). ``load_model`` reads
the GGUF's ``general.architecture`` string and looks it up — an unknown arch
panics with the registered list, better than feeding weights through the
wrong path.

.. code-block:: das

   let names <- arch_names()
   print("{length(names)} registered architectures: {join(names, ", ")}\n")
   // -> 9 registered architectures: qwen2, qwen2moe, llama, gemma2, gemma3, ...

Anatomy of an ArchDesc
======================

An architecture is data, not a fork of the engine:

``configure``
    A function setting ``Config`` flags — rope style, extra norms,
    sliding-window patterns, MoE shape. It runs before dim/vocab are known.
    The real qwen3 sets ``rope_neox`` + ``qk_norm`` and nothing else.

``blocks``
    The forward-pass kernels. ``std_blocks()`` is the standard attention +
    FFN pair; MoE (qwen2moe) and gpt-oss swap in their own.

``chat``
    The chat template as data: per-turn part lists
    (``chat_special`` / ``chat_text`` / ``chat_content``) plus stop tokens.

Most new model families are just ``Config`` flags over ``std_blocks`` — that
is the Tier-1/Tier-2 design the registry exists for.

Register your own
=================

Suppose a llama-shaped model ships with an architecture string the registry
doesn't know. Mapping it onto the existing machinery takes no engine change:

.. code-block:: das

   def private configure_my_llama(var c : Config) {
       c.rope_neox = true
   }

   var d = ArchDesc(name = "my_llama", configure = @@configure_my_llama)
   d.blocks = std_blocks()
   d.chat.add_bos = true
   d.chat.user.parts <- [chat_special("<|im_start|>"), chat_text("user\n"),
       chat_content(), chat_special("<|im_end|>"), chat_text("\n")]
   d.chat.assistant_open.parts <- [chat_special("<|im_start|>"), chat_text("assistant\n")]
   d.chat.stop <- ["<|im_end|>"]
   register_arch("my_llama", d)

From here, ``load_model`` on a GGUF whose ``general.architecture`` says
``my_llama`` would configure, load, and chat through this descriptor.

Adding a real architecture to the module
========================================

The in-tree recipe (see ``modules/dasLLAMA/API_REWORK.md``'s model-support
waves for worked examples, from Qwen3's two flags to gpt-oss's
MoE-with-sinks):

1. ``modules/dasLLAMA/dasllama/dasllama_arch_<name>.das`` — configure +
   blocks + chat template, self-registered in an ``[init]`` function.
2. ``require`` it from the ``dasllama`` umbrella; add it to the module's
   CMake AOT file lists and ``.das_module`` descriptor.
3. Validate token-for-token against llama.cpp on a real GGUF before trusting
   any output — greedy continuations must match id-for-id.
4. Registry-count tests (``tests/dasLLAMA/test_arch_registry.das``) hardcode
   the architecture count — bump them.

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/06_add_an_arch.das <../../../../tutorials/dasLLAMA/06_add_an_arch.das>`
