RAII creators — the ``create_*`` / ``allocate_*`` functions that wrap
``vkCreate*`` / ``vkAllocate*`` and return an owning handle wrapper from
``vulkan_handles``. Each takes a boost ``*CreateInfo`` view (``sType`` filled,
arrays instead of count+pointer pairs) and an optional ``var result :
VkResult?`` — left null it panics on failure, passed non-null it forwards the
code for the caller to handle.
