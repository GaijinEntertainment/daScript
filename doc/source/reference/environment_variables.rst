.. _environment_variables:

.. This page has NO content of its own. The single source is the markdown skill
   skills/internal/environment_variables.md, which is repo-side only (it documents
   the repo's C++ env_cfg accessors), so this page is how the variable list reaches
   users. Edit the .md.
.. The page label is declared HERE rather than in the .md: a MyST target of the
   form (label)= does NOT register in Sphinx's std domain when the markdown is
   pulled in through `.. include:: ... :parser:`, so :ref: from other pages would
   silently fail to resolve.

.. include:: ../../../skills/internal/environment_variables.md
   :parser: myst_parser.sphinx_
