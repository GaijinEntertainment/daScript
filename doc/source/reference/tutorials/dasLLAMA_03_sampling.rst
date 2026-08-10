.. _tutorial_dasLLAMA_sampling:

======================
dasLLAMA-03 — Sampling
======================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Sampling
    single: Tutorial; Temperature

How the next token gets picked. ``SamplingParams`` carries the knobs, and its
defaults are greedy:

.. das-doc: signatures
.. code-block:: das

   struct SamplingParams {
       temp : float = 0.0               // <= 0 => greedy argmax; otherwise softmax temperature
       top_k : int64 = 0l               // <= 0 or >= vocab => no top-k cutoff
       top_p : float = 1.0              // nucleus: keep the smallest probability mass >= top_p (>= 1 = off)
       min_p : float = 0.0              // drop tokens under min_p * the top token's probability (<= 0 = off)
       penalty : float = 1.0            // multiplicative repetition penalty over recent tokens (1.0 = none)
       presence_penalty : float = 0.0   // subtracted from the logit of every distinct token in the window
       frequency_penalty : float = 0.0  // subtracted once per occurrence in the window (OpenAI semantics)
       penalty_last_n : int64 = 64l     // penalty window: the most recent N generated tokens
   }

Run it like tutorial 01::

   daslang.exe -jit tutorials/dasLLAMA/03_sampling.das -- path/to/model.gguf

Greedy: deterministic, and loop-prone
=====================================

Greedy argmax takes the single most likely token every step. Two runs are
identical — but on small models the most likely continuation of a repetitive
context is more repetition, so greedy text tends to loop. SmolLM2-135M
demonstrates on cue::

   greedy:, I was a young man, a young woman, and a young man again. I was a
   young man, a young woman, and a young man again. ...

The repetition penalty
======================

``penalty > 1`` scales down the logits of the last ``penalty_last_n``
generated tokens before picking, so the argmax can't keep choosing the same
phrase. ``presence_penalty`` and ``frequency_penalty`` do the same job by
subtraction instead of scaling — once for every distinct token in that window,
and once per occurrence — which is how the OpenAI API spells it. All three are
fully deterministic; no randomness involved::

   greedy + penalty 1.3:, I was a young man with dreams of becoming an
   engineer. My parents were both engineers and they encouraged my passion...

Temperature, top-k, and seeds
=============================

``temp > 0`` samples from the softmax distribution (higher = more
adventurous); ``top_k > 0`` first cuts it to the k most likely tokens.
``top_p`` and ``min_p`` cut by probability instead of by count — keep the
smallest set of tokens worth ``top_p`` of the mass, drop everything under
``min_p`` of the top token's probability. Sampling draws from the *session's*
RNG, so variety comes from the seed — and ``set_seed`` makes any run exactly
reproducible:

.. das-doc: given var m = Model()
.. das-doc: given var prompt : array<int64>
.. code-block:: das

   def run_once(m : Model; prompt : array<int64>; params : SamplingParams; seed : int) : string {
       var s = create_session(m)   // fresh session, so runs compare cleanly
       set_seed(s, seed)
       var parts : array<string>
       generate(m, s, prompt, params, 40l) $(_id, piece) {
           parts |> push(piece)
           return true
       }
       let out = join(parts, "")
       delete s                    // six sessions in one run — free each (tutorial 04)
       return out
   }

   let params = SamplingParams(temp = 0.8, top_k = 40l, penalty = 1.1)
   let s7 = run_once(m, prompt, params, 7)        // a story about Alex
   let s8 = run_once(m, prompt, params, 8)        // a story about Kanaq
   let s7again = run_once(m, prompt, params, 7)   // s7 again, token for token

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/03_sampling.das <../../../../tutorials/dasLLAMA/03_sampling.das>`

   Next tutorial: :ref:`tutorial_dasLLAMA_sessions_and_memory`
