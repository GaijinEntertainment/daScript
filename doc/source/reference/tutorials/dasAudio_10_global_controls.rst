.. _tutorial_dasAudio_global_controls:

==================================
AUDIO-10 — Global Controls
==================================

.. index::
    single: Tutorial; Global Volume
    single: Tutorial; set_global_volume
    single: Tutorial; set_global_pause
    single: Tutorial; dasAudio

This tutorial covers the master controls that affect **every** playing sound at
once, rather than a single channel. They are independent of each sound's own
``set_volume`` / ``set_pitch`` / ``set_pause`` (see :ref:`tutorial_dasAudio_sound_control`).

Master Volume
=============

``set_global_volume`` multiplies the final mix — it scales all sounds together.
Reach for it on focus loss to duck the whole game without touching individual
channels:

.. code-block:: das

   set_global_volume(0.3)   // everything quieter
   set_global_volume(1.0)   // restored

Master Pitch
============

``set_global_pitch`` multiplies the playback rate of every sound — a one-call
slow-motion (``0.5``) or fast-forward (``2.0``) effect:

.. code-block:: das

   set_global_pitch(0.5)    // an octave down, slow motion
   set_global_pitch(1.0)    // normal

Master Pause
============

``set_global_pause(true)`` freezes the entire mixer; ``(false)`` resumes it, and
each sound continues exactly where it left off — ideal for a pause menu:

.. code-block:: das

   set_global_pause(true)   // silence
   set_global_pause(false)  // resume

Because these act on the whole mixer, two looping tones started together both
respond to a single global call — no need to enumerate channels.

.. note::

   ``set_ignore_global_volume(sid, true)`` exempts one channel from the master
   volume — useful for an editor preview that should stay audible while the
   game's master volume is muted.

.. seealso::

   Full source: :download:`tutorials/dasAudio/10_global_controls.das <../../../../tutorials/dasAudio/10_global_controls.das>`

   Previous tutorial: :ref:`tutorial_dasAudio_playback_status`
