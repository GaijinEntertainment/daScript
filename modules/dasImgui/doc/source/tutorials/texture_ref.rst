.. _tutorial_texture_ref:

#######################
Texture references
#######################

Dear ImGui 1.92 reworked texture handling: the raw ``ImTextureID`` you used to
hand to ``image()`` is now an ``ImTextureRef``. This tutorial shows the full
path for displaying **your own** image — decode a PNG, upload it to a GL
texture, and pass that texture through an ``ImTextureRef``:

.. code-block:: das

   // 1. decode a PNG to RGBA pixels (dasStbImage)
   var img : Image
   img->load(path, 4)
   // 2. upload to a GL texture
   glGenTextures(1, safe_addr(g_tex))
   glBindTexture(GL_TEXTURE_2D, g_tex)
   glTexImage2D(GL_TEXTURE_2D, 0, int(GL_RGBA), img.width, img.height, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, unsafe(addr(img.bytes[0])))
   // 3. wrap the GL texture name in an ImTextureRef
   var ref : ImTextureRef
   ref._TexID = uint64(g_tex)
   // 4. draw it
   image(TR_PIC, (user_texture_id = ref, size = float2(w, h), ...))

``_TexID`` is the **user slot** of ``ImTextureRef`` (an ``ImTextureID`` == the GL
texture name here). With ``_TexData`` left null, the backend treats it as a
user-managed texture and binds the GL id directly — that is how you pass your
own texture in 1.92.

The demo loads one of the repo's icon PNGs as a stand-in for an application
texture; point ``PICTURE_PATH`` at any image of your own.

Source: ``examples/tutorial/texture_ref.das``.

************
Walkthrough
************

.. video:: texture_ref.mp4

.. literalinclude:: ../../../examples/tutorial/texture_ref.das
   :language: das
   :linenos:

Requires
========

``stbimage/stbimage_boost`` for the PNG decode (the ``Image`` type and
``Image::load``), ``opengl/opengl_boost`` for the GL upload (already pulled in
by the live backend), and ``daslib/safe_addr`` for the texture-name address.

Behaviour
=========

The picture is decoded + uploaded **once** in ``init`` (the GL context is live
after ``live_imgui_init``), and the texture name is kept in a module global.
Each frame ``image()`` draws it twice through a freshly built ``ImTextureRef`` —
once plain, once tinted + bordered — to show the same texture reused while
``tint_col`` / ``border_col`` vary per call. The texture is freed with
``glDeleteTextures`` on shutdown.

Migration note
==============

Pre-1.92 code passed an ``ImTextureID`` straight to ``image()``. In 1.92 the
argument is an ``ImTextureRef``; put your texture handle in ``ref._TexID`` (the
user slot) and leave ``ref._TexData`` null. The font atlas moved the same way:
``io.Fonts.TexID`` → ``io.Fonts.TexRef`` (an ``ImTextureRef``), gated on
``io.Fonts.TexData != null``.

.. seealso::

   Full source: :download:`examples/tutorial/texture_ref.das <../../../examples/tutorial/texture_ref.das>`

   Related: :ref:`tutorial_tree_image_misc` — ``image()`` against the font
   atlas, alongside ``tree_node_ex``.
