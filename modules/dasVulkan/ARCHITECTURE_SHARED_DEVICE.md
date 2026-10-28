# dasVulkan - the device a process's contexts share

**Read `ARCHITECTURE_COMMON.md` (repo root) first - its contract binds this document.** Companion to `ARCHITECTURE.md`;
a section is cited by its anchor.

## The shared device holder {#shared-device}

A daslang context keeps its module globals to itself - a worker thread's context included - so a handle a consumer keeps in
a global would give every context a `VkDevice` of its own, each its own loader table load and memory count. The holder in
`src/dasVULKAN.main.cpp` (bound by hand) is the one process-wide slot instead: `vk_shared_device_acquire` returns the device
and counts a reference, or - with no device yet - returns 0 holding the creation lock, and that caller makes the instance
and the device and hands them over with `vk_shared_device_publish` (or `vk_shared_device_abort` on failure), so two contexts
arming at once make one device. The slot keeps the instance, the physical device, the device, the compute and transfer
families with the queue count created on each, and a byte count consumers add to (`vk_shared_device_bytes`). A consumer
takes a queue index round robin (`vk_shared_queue_take`); past the count two consumers share a queue, and since
`vkQueueSubmit` and `vkQueueWaitIdle` need external synchronization on one queue, `vk_queue_submit_locked` and
`vk_queue_wait_idle_locked` take a per-queue mutex keyed by the queue handle - uncontended where a queue is one consumer's.
`vk_shared_device_release` drops a reference; the last one waits the device idle and destroys the device and the instance.
`set_device_queue_ask` (`daslib/vulkan_boost.das`) is how the creator asks several queues of each family in one
`create_device*` call; `device_queue_count` answers what a family gets. The dasLLAMA Vulkan tier is the consumer
(`modules/dasLLAMA/ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#vk-shared-device`).

## The loader table loads once {#volk-once}

volk's function pointers are process globals. `volkInitialize` runs once per process and answers its first result after;
`volkLoadDevice` loads the table for the first device it is handed and is a no-op for the same device again, so the table
every context dispatches through is the shared device's; `volkLoadInstance` after a device loaded loads the instance-level
entries alone (`volkLoadInstanceOnly`), leaving the device table intact for a probe instance a context makes. A mutex guards
the three and `volkFinalize`, which forgets the loaded device.
