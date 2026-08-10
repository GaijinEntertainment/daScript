.. _tutorial_dasAudio_spatial_audio:

================================
AUDIO-04 --- 3D Spatial Audio
================================

.. index::
    single: Tutorial; 3D Audio
    single: Tutorial; HRTF
    single: Tutorial; Attenuation

This tutorial covers 3D positional audio: placing the listener and sound
sources in space, moving sources in real time, and choosing attenuation
models.  Best experienced with headphones.

Listener Setup
==============

``set_head_position(pos, dir, vel)`` defines the listener in 3D space:

- ``pos`` --- world position (``float3``)
- ``dir`` --- facing direction (``float3``, should be normalized)
- ``vel`` --- velocity (``float3``, used for Doppler calculations)

.. code-block:: das

   // Listener at origin, facing forward (-Y), stationary
   set_head_position(float3(0, 0, 0), float3(0, -1, 0), float3(0, 0, 0))

The coordinate system is left-handed: +X is right, -Y is forward, +Z is up.
Set the listener explicitly before placing sources --- the engine starts with a
head direction of +Y, which puts +X on the *left*.

Playing 3D Sound
================

``play_3d_sound_loop_from_pcm`` places a looping sound at a 3D position with
a specified attenuation model:

.. das-doc: given def generate_click_burst() : array<float> { return <- [for (x in range(4)); 0.0] }

.. code-block:: das

   var samples <- generate_click_burst()
   let sid = play_3d_sound_loop_from_pcm(
       float3(5, 0, 0),              // position: 5 meters to the right
       linear_attenuation(20.0),     // audible up to 20 meters
       MA_SAMPLE_RATE, 1,
       samples
   )

The attenuation model determines how volume falls off with distance (see
below).  Short impulsive sounds (clicks, noise bursts) are easier to
localize than pure tones.

Moving a Sound Source
=====================

``set_position(sid, pos, vel)`` updates the position and velocity of a 3D
sound.  Call it in a loop to animate:

.. code-block:: das

   let radius = 5.0
   let num_steps = 100
   for (i in range(num_steps)) {
       let angle = 2.0 * PI * float(i) / float(num_steps)
       let x = radius * cos(angle)
       let y = radius * sin(angle)
       let pos = float3(x, y, 0)

       // Tangential velocity enables Doppler effect
       let vx = -radius * sin(angle) * 2.0 * PI / 5.0
       let vy =  radius * cos(angle) * 2.0 * PI / 5.0
       let vel = float3(vx, vy, 0)

       set_position(sid, pos, vel)
       sleep(50u)
   }

The velocity vector does not move the source --- it only affects the Doppler
pitch shift.  You must update ``pos`` yourself each frame.

Attenuation Models
==================

Four built-in attenuation models control how volume decreases with distance.
``linear_attenuation`` and ``quadratic_attenuation`` take a ``max_distance``
parameter (distance at which volume reaches zero).
``inverse_distance_attenuation`` and ``inverse_square_attenuation`` take a
``dmin`` reference distance --- volume is 1.0 at the listener and 0.5 at
``dmin``, rolling off from there:

.. list-table::
   :header-rows: 1
   :widths: 32 38 30

   * - Constructor
     - Falloff
     - Character
   * - ``inverse_distance_attenuation``
     - *dmin* / (*d* + *dmin*)
     - Natural rolloff; half volume at *dmin*
   * - ``linear_attenuation``
     - 1 - *d* / *max*
     - Straight line to silence at *max*
   * - ``quadratic_attenuation``
     - 1 - (*d* / *max*)\ :sup:`2`
     - Louder than linear, then drops steeply near *max*
   * - ``inverse_square_attenuation``
     - *dmin*\ :sup:`2` / (*d*\ :sup:`2` + *dmin*\ :sup:`2`)
     - Half volume at *dmin*, then rapid falloff

Example --- comparing all four at different distances:

.. code-block:: das

   let max_dist = 30.0
   let pos = float3(2, 0, 0)

   // every play_*_from_pcm call MOVES its samples array, so each source
   // needs its own copy
   var samples_a <- generate_click_burst()
   var samples_b <- clone(samples_a)
   var samples_c <- clone(samples_a)
   var samples_d <- clone(samples_a)

   let sid_inv  = play_3d_sound_loop_from_pcm(pos,
       inverse_distance_attenuation(max_dist), MA_SAMPLE_RATE, 1, samples_a)
   let sid_lin  = play_3d_sound_loop_from_pcm(pos,
       linear_attenuation(max_dist), MA_SAMPLE_RATE, 1, samples_b)
   let sid_quad = play_3d_sound_loop_from_pcm(pos,
       quadratic_attenuation(max_dist), MA_SAMPLE_RATE, 1, samples_c)
   let sid_isq  = play_3d_sound_loop_from_pcm(pos,
       inverse_square_attenuation(max_dist), MA_SAMPLE_RATE, 1, samples_d)

HRTF
====

HRTF (Head-Related Transfer Function) is enabled by default in the audio
engine.  It applies binaural filtering so that sounds placed in 3D space are
perceived at their correct spatial position when listening through
headphones.  No additional setup is required --- every ``play_3d_sound_*``
call is a candidate for HRTF processing.

The convolution is budgeted, though: the 32 3D channels closest to the
listener get HRTF, and any beyond that fall back to simulated 3D ---
constant-power panning plus distance attenuation, no convolution.
``set_hrtf_budget(n)`` moves the line: 0 for all-simulated, a large number
(say 999) for all-HRTF.

Running the Tutorial
====================

::

   daslang.exe tutorials/dasAudio/04_spatial_audio.das

The tutorial places a clicking sound beside the listener, orbits it around
the listener over 5 seconds, then demonstrates the four attenuation models at
varying distances.  Use headphones for the best spatial experience.

.. seealso::

   Full source: :download:`tutorials/dasAudio/04_spatial_audio.das <../../../../tutorials/dasAudio/04_spatial_audio.das>`

   Next tutorial: :ref:`tutorial_dasAudio_reverb`
