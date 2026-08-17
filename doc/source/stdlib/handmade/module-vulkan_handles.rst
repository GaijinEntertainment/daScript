RAII ownership wrappers — one struct per Vulkan handle (``Instance``,
``Device``, ``Image``, ``CommandPool``, …). Each wrapper carries its raw
handle plus the parent it was created from, and its ``finalize`` calls the
matching ``vkDestroy*`` / ``vkFree*`` when the owner goes out of scope. Declare
owners with ``var inscope`` so they destroy in reverse order; use ``weak_copy``
from ``vulkan_runtime`` for an intentional non-owning alias.
