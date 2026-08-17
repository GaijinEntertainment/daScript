Low-level runtime helpers shared by the rest of the boost layer: ``vk_check``
turns a ``VkResult`` into a panic (or forwards it to a caller-supplied
``var result``), ``array_addr`` bridges a daslang ``array<T>`` to the raw
pointer Vulkan expects, ``weak_copy`` produces a non-owning duplicate of a
handle wrapper, and ``make_api_version`` packs a version triple.
