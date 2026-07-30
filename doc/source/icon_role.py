# -*- coding: utf-8 -*-
#
# Local Sphinx role: ``:icon:`name``` renders a dasImgui glyph from
# ``_static/icons/<name>.png`` inline with the surrounding text.
#
# The PNGs are produced by ``utils/make_icon_doc.das`` (one per glyph in the
# imgui_icons catalog) and committed under ``_static/icons/``. Use the role in
# any ``//!`` docstring or handmade page to reference an icon by name, e.g.
# ``the :icon:`play` button``. See the icon catalog page for the full set.
#
# Kept separate from the vendored ``daslang.py`` domain (presentation local to
# the imgui docs), like ``tutorial_video.py``.

from docutils import nodes


def icon_role(name, rawtext, text, lineno, inliner, options={}, content=[]):
    icon_name = text.strip()
    # Leading slash = relative to the Sphinx source root, so the same markup
    # resolves from any page depth (tutorials/, stdlib/generated/, ...). A
    # missing PNG raises a build warning (caught by -W) — a useful typo guard.
    uri = '/_static/icons/%s.png' % icon_name
    node = nodes.image(rawtext, uri=uri, alt='%s icon' % icon_name,
                       classes=['dasimgui-icon'])
    return [node], []


def setup(app):
    app.add_role('icon', icon_role)
    return {
        'version': '1.0',
        'parallel_read_safe': True,
        'parallel_write_safe': True,
    }
