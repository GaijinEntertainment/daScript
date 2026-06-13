.. _tutorial_dasAudio_playback_status:

==================================
AUDIO-09 — Playback Status
==================================

.. index::
    single: Tutorial; Playback Status
    single: Tutorial; AudioChannelStatus
    single: Tutorial; set_status_update
    single: Tutorial; dasAudio

This tutorial covers monitoring a playing sound. Audio runs on its own thread,
so status is published through a ``LockBox``: the audio thread writes a snapshot
each mix, and the main thread reads it on demand. This is how you detect when a
one-shot sound has finished, or how full a streaming queue is.

Registering a Status Box
========================

Create a ``LockBox`` with ``lock_box_create`` and attach it to a sound with
``set_status_update``. The call seeds the box with state ``starting``; the audio
thread updates it from then on:

.. code-block:: das

   require audio/audio_boost
   require daslib/jobque_boost

   let sid = play_sound_from_pcm(MA_SAMPLE_RATE, 1, tone)
   var status_box <- lock_box_create()
   set_status_update(sid, status_box)

Reading the Snapshot
====================

Read the current ``AudioChannelStatus`` with ``box |> get()`` — a read-only peek
that keeps the box alive across reads (do not use ``grab``, which is a one-shot
consume). The snapshot carries the playback state, frame positions, and the
streaming queue depth:

.. code-block:: das

   status_box |> get() $(s : AudioChannelStatus#) {
       // s.state              : AudioChannelState
       // s.playback_position  : frames played
       // s.consumed_position  : frames of real audio consumed
       // s.stream_que_length  : pending PCM chunks (for streaming sounds)
   }

``AudioChannelState`` is ``starting`` → ``playing`` → ``stopped`` (with
``stopping`` during a fade-out and ``paused`` while paused). Polling the box lets
you watch a one-shot sound run to completion:

.. code-block:: das

   var done = false
   for (i in range(100)) {
       status_box |> get() $(s : AudioChannelStatus#) {
           done = s.state == AudioChannelState.stopped
       }
       break if (done)
       sleep(20u)
   }

Releasing the Box
=================

Order matters — stop the updates, clear the stored value, join, then remove the
box, or the ``LockBox`` leaks:

.. code-block:: das

   unset_status_update(sid)
   clear_status(status_box)
   status_box |> join()
   unsafe(lock_box_remove(status_box))

.. seealso::

   Full source: :download:`tutorials/dasAudio/09_playback_status.das <../../../../tutorials/dasAudio/09_playback_status.das>`

   Previous tutorial: :ref:`tutorial_dasAudio_midi`

   Next tutorial: :ref:`tutorial_dasAudio_global_controls`
