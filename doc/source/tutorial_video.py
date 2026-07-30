# -*- coding: utf-8 -*-
#
# Local Sphinx directive: ``.. video:: scene_name.mp4`` embeds a tutorial
# recording from ``_static/tutorials/`` in a native HTML5 player.
#
# Centralizes the player markup so the chrome (controls, preload, sizing) is
# tuned in one place instead of being duplicated across every tutorial page.
# The player starts PAUSED with native controls (play/pause, scrubber, volume,
# fullscreen); audio is on; no autoplay, no loop — it behaves like a normal
# embedded video.
#
# The MP4s are NOT in git: they ship as assets on the rolling ``docs-assets``
# GitHub release and are staged into ``doc/source/_static/tutorials/`` by
# ``utils/docs_assets/fetch`` before sphinx runs. The directive verifies the
# staged file exists, so a missing recording fails a ``-W`` build loudly
# instead of shipping a broken player.
#
# Kept out of the ``daslang.py`` domain — this is presentation local to the
# tutorial pages.

import os

from docutils import nodes
from docutils.parsers.rst import Directive


# Raw HTML is not depth-rewritten by Sphinx, so compute the ../ prefix from the
# including document's depth (e.g. reference/tutorials/imgui/drawlist -> 3 up).
_VIDEO_HTML = (
    '<video controls preload="metadata" playsinline width="100%">\n'
    '  <source src="{prefix}_static/tutorials/{name}" type="video/mp4">\n'
    "  Your browser doesn't support HTML5 video. "
    '<a href="{prefix}_static/tutorials/{name}">Download the recording</a>.\n'
    '</video>'
)


class VideoDirective(Directive):
    """``.. video:: scene_name.mp4`` — embed a tutorial recording.

    One argument: the mp4 filename under ``doc/source/_static/tutorials/``
    (staged there from the docs-assets release by utils/docs_assets/fetch).
    """

    required_arguments = 1
    optional_arguments = 0
    final_argument_whitespace = False
    has_content = False

    def run(self):
        name = self.arguments[0].strip()
        env = self.state.document.settings.env
        staged = os.path.join(env.srcdir, '_static', 'tutorials', name)
        if not os.path.exists(staged):
            return [self.state.document.reporter.warning(
                'video: %s is not staged under _static/tutorials/ — run '
                'utils/docs_assets/fetch (or upload it to the docs-assets '
                'release)' % name, line=self.lineno)]
        prefix = '../' * env.docname.count('/')
        return [nodes.raw('', _VIDEO_HTML.format(name=name, prefix=prefix),
                          format='html')]


def setup(app):
    app.add_directive('video', VideoDirective)
    return {
        'version': '1.0',
        'parallel_read_safe': True,
        'parallel_write_safe': True,
    }
