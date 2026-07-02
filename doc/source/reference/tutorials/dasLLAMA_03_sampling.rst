.. _tutorial_dasLLAMA_sampling:

======================
dasLLAMA-03 — Sampling
======================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Sampling
    single: Tutorial; Temperature

How the next token gets picked. ``SamplingParams`` has four knobs, and their
defaults are greedy::

   struct SamplingParams {
       temp : float = 0.0             // <= 0 => greedy argmax; otherwise softmax temperature
       top_k : int64 = 0l             // <= 0 or >= vocab => no top-k cutoff
       penalty : float = 1.0          // repetition penalty over recent tokens (1.0 = none)
       penalty_last_n : int64 = 64l   // repetition-penalty window
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
phrase. Still fully deterministic — no randomness involved::

   greedy + penalty 1.3:, I was a young man with dreams of becoming an
   engineer. My parents were both engineers and they encouraged my passion...

Temperature, top-k, and seeds
=============================

``temp > 0`` samples from the softmax distribution (higher = more
adventurous); ``top_k > 0`` first cuts it to the k most likely tokens.
Sampling draws from the *session's* RNG, so variety comes from the seed — and
``set_seed`` makes any run exactly reproducible:

.. code-block:: das

   def run_once(m : Model; prompt : array<int64>; params : SamplingParams; seed : int) : string {
       var s = create_session(m)   // fresh session, so runs compare cleanly
       set_seed(s, seed)
       var parts : array<string>
       generate(m, s, prompt, params, 40l) $(_id, piece) {
           parts |> push(piece)
           return true
       }
       return join(parts, "")
   }

   let params = SamplingParams(temp = 0.8, top_k = 40l, penalty = 1.1)
   let s7 = run_once(m, prompt, params, 7)        // a story about Alex
   let s8 = run_once(m, prompt, params, 8)        // a story about Kanaq
   let s7again = run_once(m, prompt, params, 7)   // s7 again, token for token

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/03_sampling.das <../../../../tutorials/dasLLAMA/03_sampling.das>`

   Next tutorial: :ref:`tutorial_dasLLAMA_sessions_and_memory`
