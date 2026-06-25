09 - Multisample Anti-Aliasing
==============================

The second **GL-idiom-substitution** rung. dasVulkan's MSAA tutorial draws into a 4x
multi-sample colour attachment and names a 1x "resolve attachment" in the render pass;
the GPU averages the samples down at the end. WebGL2 has no resolve attachments, so this
rung uses the canonical GL substitute: render into a multisample framebuffer, then
**resolve with a blit** (``glBlitFramebuffer`` from a multisample read framebuffer
averages the N samples per pixel into one).

MSAA's edge smoothing is nearly invisible on a cube (12 long edges) and dramatic on a
dense thin silhouette -- so the test mesh is a **spiky ball**: a smooth UV-sphere core
studded with 150 flat-shaded pyramidal spikes in Fibonacci-distributed directions, a
sea-urchin whose every spike is a high-frequency silhouette edge. And to make "is MSAA
on?" unmistakable, the same ball is rendered twice -- once 1x, once 4x -- and the screen
shows the **left half of the 1x image beside the right half of the resolved 4x image**,
split by a bright divider. Left of the line the spikes stair-step and crawl as the ball
spins; right of it they are smooth.

.. literalinclude:: ../../../../tutorials/opengl/09_msaa/09_msaa.das
   :language: das
   :start-at: GL objects

Two render targets
------------------

``setup_targets`` builds two full-size framebuffers, both with **renderbuffer**
attachments (never sampled, only blitted out):

* the **1x** target -- ``glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, w, h)`` colour
  plus a depth renderbuffer;
* the **MSAA** target -- ``glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples,
  GL_RGBA8, w, h)`` colour plus a multisample depth renderbuffer.

The sample count is ``min(4, GL_MAX_SAMPLES)``. With N samples per pixel each triangle
edge is rasterised against N sub-pixel sample points, so coverage along the edge is
graded instead of binary -- that graded coverage becomes a smooth edge after the resolve.

The resolve blit, and the comparison
------------------------------------

The same ball is drawn into both targets, then composed onto the screen with two blits::

    let half = target_w / 2
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0u)
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_1x)
    glBlitFramebuffer(0, 0, half, target_h, 0, 0, half, target_h, GL_COLOR_BUFFER_BIT, GL_NEAREST)
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_msaa)
    glBlitFramebuffer(half, 0, target_w, target_h, half, 0, target_w, target_h, GL_COLOR_BUFFER_BIT, GL_NEAREST)

Blitting *from* the multisample read framebuffer averages its N colour samples per pixel
into one -- that blit **is** the resolve. A multisample-resolve blit requires the source
and destination rectangles to be the same size, so both blits are 1:1 and the targets
track the live display size (recreated on resize). A scissored clear then paints the
divider over the seam.

MSAA is invisible to the shader
-------------------------------

The vertex and fragment programs are byte-for-byte tutorial 04's. Multisampling happens
entirely at the framebuffer level -- the fragment shader still runs once per pixel (not
once per sample), and the hardware handles the per-edge coverage. Nothing about the spiky
ball's draw call knows whether it is going into the 1x or the 4x target.

Run it
------

Locally, in a window::

    daslang tutorials/opengl/09_msaa/09_msaa.das

In the browser, it runs live in the daslang playground -- the same ``.das``, lowered to
WebGL2: a spinning spiky ball split down the middle, its silhouette stair-stepped on the
1x left and smooth on the 4x-MSAA right.
