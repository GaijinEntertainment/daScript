# Stop on the loop owner: off-thread EventLoop::stop clears loop_ while
# connection teardown on the worker may still post/cancel events through it.
das_hv_patch_begin("http/server/HttpServer.cpp")
das_hv_hunk([=[
    // stop all loops
    for (auto& loop : privdata->loops) {
        loop->stop();
    }
]=] [=[
    // Stop each loop on its owner before joining its worker. Retain the wrapper
    // until the queued stop runs; connection teardown also reads loop state.
    for (auto& loop : privdata->loops) {
        loop->queueInLoop([loop]() { loop->stop(); });
    }
]=])
das_hv_patch_end()

# Async clients own EventLoopThread rather than HttpServer workers; use the
# same owner-thread ordering when their destructor requests shutdown.
das_hv_patch_begin("evpp/EventLoopThread.h")
das_hv_hunk([=[
        long loop_tid = loop_->tid();
        loop_->stop();
]=] [=[
        long loop_tid = loop_->tid();
        auto loop = loop_;
        loop->queueInLoop([loop]() { loop->stop(); });
]=])
das_hv_patch_end()
