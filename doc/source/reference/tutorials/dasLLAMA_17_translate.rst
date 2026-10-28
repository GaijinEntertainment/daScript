.. _tutorial_dasLLAMA_translate:

==========================
dasLLAMA-17 — Translation
==========================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Translation

dasLLAMA translates with the small models Firefox ships for offline translation.
Each model goes one way: ru-en reads Russian and writes English, en-ru reads
English and writes Russian. Firefox publishes them as a Marian weight file
(``model.<pair>.intgemm.alphas.bin``, Marian is the C++ toolkit that trained
them) and a SentencePiece vocabulary (``vocab.<pair>.spm``) - the tokenizer
that cuts text into pieces the model knows.

Run it with the model file; its vocabulary sits beside it::

   daslang.exe -jit tutorials/dasLLAMA/17_translate.das -- ru-en/model.ruen.intgemm.alphas.bin
   daslang.exe -jit ... -- en-ru/model.enru.intgemm.alphas.bin "Where is the nearest pharmacy?"
   daslang.exe -jit ... -- en-ru/model.enru.intgemm.alphas.bin --exact

Use ``-jit``: the model runs the same tuned kernels the language models run.

Load a model
============

``load_translator`` takes the path of the Marian file. The first load reads the
file, converts the weights and writes a ``.dlim`` image beside it; every later
load maps that image and starts at once. ``translator_langs`` reads the two
languages off the model's own pair name.

.. code-block:: das

   var inscope tr <- load_translator("ru-en/model.ruen.intgemm.alphas.bin")
   let langs = translator_langs(tr)
   print("model reads '{langs.src}', writes '{langs.trg}'\n")

The file stores its weights as 8-bit integers. By default dasLLAMA serves them
as Q8_0 blocks - 32 integers and one scale each - which is the fast lane on the
CPU and on the GPU. ``load_translator(path, true)`` widens every weight to a
32-bit float instead: the reference lane, a little slower, and closest to the
float math the model was trained in. The two lanes agree on most sentences;
where two next words score almost the same, they can pick differently.

A translation LLM
=================

Firefox's models are small and fast, and they go one way. The other kind is a
language model trained to translate: MiLMMT-46 is Gemma 3 1B trained further on
46 languages. The file is a GGUF of about 1 GB at Q4_K_M - thirty times the
Firefox model - and a sentence takes tens of milliseconds instead of a few. It
translates noticeably better, and one file covers every direction among its
languages, so the program picks the direction:

.. code-block:: das

   var inscope tr <- load_translator("MiLMMT-46-1B-v0.1.Q4_K_M.gguf")
   print("{join(translator_languages(tr), " ")}\n")   // ar az bg ... zh zh-Hant
   if (!set_translator_langs(tr, "ru", "en")) {
       print("not a pair this model covers\n")
   }

``translator_languages`` lists the language codes the model knows,
``translator_language_name`` reads a code as its English name (``ru`` is
``Russian``), ``translator_serves`` asks whether a pair is covered without
changing anything, and
``set_translator_langs`` answers false for a pair it does not cover. A Firefox
model accepts only its own pair. Under the hood the LLM reads a short prompt -
"Translate this from Russian to English:", the sentence, and "English:" - and
writes the next line, greedy, until its end token. A general instruct model -
Qwen, Llama, Gemma - loads the same way: a GGUF that carries a chat template
gets an instruction through that template, with thinking off, and the reply is
the translation.

Translate
=========

.. code-block:: das

   for (ln in lines) {
       print("{ln}\n  -> {translate(tr, ln)}\n")
   }

``translate`` takes one sentence. An encoder reads the whole sentence at once,
then a decoder writes the translation one piece at a time, each time the most
likely next piece, until it writes the end token or reaches twice the length of
the source. The model was trained on single sentences, so split a paragraph
into sentences first. Each call reuses the buffers the translator holds: a long
text is a loop of short calls, and a sentence takes a few milliseconds on a
laptop CPU. Where a GPU driver serves the model, ``translate`` runs the whole
sentence on the device; elsewhere it runs on the CPU, with the same result.

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/17_translate.das <../../../../tutorials/dasLLAMA/17_translate.das>`

   Previous tutorial: :ref:`tutorial_dasLLAMA_text_to_speech`

   Speech in, speech out: :ref:`tutorial_dasLLAMA_speech_to_text` reads the source language, :ref:`tutorial_dasLLAMA_text_to_speech` speaks the translation
