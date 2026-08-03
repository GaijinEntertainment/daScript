options gen2
options indenting = 4

module dasllama_vulkan_seams shared public

require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes

// The single-op resident-driver seams (vk_add_rms / vk_rope_kv_store / vk_decode_attn) on the
// CLASS rail: generated per-class sets + push-constant params — no meta buffers, no vk_set6.
// Scratch buffers live on g_gpu (dasllama_vulkan_common); this module owns their class sets.

// residual-stream scratch for the add+rms kernel (lazy — only a resident-driver run creates it)
def private ensure_ar_state {
    if (g_gpu.ar_ready) {
        return
    }
    verify(ensure_cls_ar(), "vulkan seams: the ar class rail must engage on a live device")
    let bytes = AR_MAX_DIM * 4l
    g_gpu.ar_x_dev = make_device_buf(bytes)
    g_gpu.ar_a_dev = make_device_buf(bytes)
    g_gpu.ar_w_dev = make_device_buf(bytes)
    g_gpu.ar_y_dev = make_device_buf(bytes)
    g_gpu.ar_host = make_host_buf(bytes, false, [cached = true])   // x and y readback
    g_gpu.ar_set = set_cls_ar(fixed_array(g_gpu.ar_x_dev, g_gpu.ar_a_dev, g_gpu.ar_w_dev, g_gpu.ar_y_dev),
        fixed_array(bytes, bytes, bytes, bytes),
        fixed_array(VHG_X, 0u, 0u, VHG_YO))   // a/w fill pre-cmd (fence-ordered)
    g_gpu.ar_ready = true
}

//! Resident-driver seam: x = (x + a) * ascale then y = rms(x) * w, in ONE dispatch. `add` false
//! runs the plain norm (the stack prologue). x is updated in place — it IS the residual stream.
def vk_add_rms(var x : array<float>; a : array<float>; w : array<float>; var y : array<float>;
               dim : int64; eps, ascale : float; add : bool) {
    verify(vk_moe_init())   // assert() refuses side-effecting expressions; the probe must run
    assert(dim <= AR_MAX_DIM, "dasLLAMA vulkan: add_rms dim exceeds AR_MAX_DIM")
    ensure_ar_state()
    let bytes = dim * 4l
    unsafe {
        upload_region_at(g_gpu.ar_x_dev, 0l, addr<uint8 const?>(x[0]), bytes)
        upload_region_at(g_gpu.ar_w_dev, 0l, addr<uint8 const?>(w[0]), bytes)
        if (add) {
            upload_region_at(g_gpu.ar_a_dev, 0l, addr<uint8 const?>(a[0]), bytes)
        }
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        var pc = ArArgs(dim = uint(dim), add_on = add ? 1u : 0u, woff = 0u, eps = eps, ascale = ascale)
        enc_cls_ar(raw, h, g_gpu.ar_set, pc, 1l)   // one workgroup owns the whole row (it reduces)
        vhz_dep(raw, h, VHG_YO, 0u, true)   // x reads back post-fence in the second cmd
        cmd_copy_whole(raw, g_gpu.ar_y_dev, g_gpu.ar_host.buf, bytes)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(y[0]), g_gpu.ar_host.mapped, bytes)
        // x is the residual stream: read the updated row back too
        var raw2 = alloc_cmd()
        vk_check(vkBeginCommandBuffer(raw2, begin), null)
        cmd_copy_whole(raw2, g_gpu.ar_x_dev, g_gpu.ar_host.buf, bytes)
        vk_check(vkEndCommandBuffer(raw2), null)
        submit_wait(raw2)
        memcpy(addr<void?>(x[0]), g_gpu.ar_host.mapped, bytes)
    }
}

def private ensure_rks_state {
    if (g_gpu.rks_ready) {
        return
    }
    verify(ensure_rope_kv_cls(), "vulkan seams: the rope class rail must engage on a live device")
    g_gpu.rks_qkv_dev = make_device_buf((RKS_MAX_QD + 2l * RKS_MAX_KVD) * 4l)
    g_gpu.rks_q_dev = make_device_buf(RKS_MAX_QD * 4l)
    g_gpu.rks_k_dev = make_device_buf(RKS_MAX_CTX * RKS_MAX_KVD * 4l)
    g_gpu.rks_v_dev = make_device_buf(RKS_MAX_CTX * RKS_MAX_KVD * 4l)
    g_gpu.rks_scal = make_host_buf(XS_BYTES, true)   // the cos+sin row (data, not params)
    g_gpu.rks_q_host = make_host_buf(RKS_MAX_QD * 4l, false, [cached = true])
    g_gpu.rks_kv_host = make_host_buf(2l * RKS_MAX_KVD * 4l, false, [cached = true])
    g_gpu.rks_set = set_rope_kv_cls(
        fixed_array(g_gpu.rks_q_dev, g_gpu.rks_qkv_dev, g_gpu.rks_k_dev, g_gpu.rks_v_dev, g_gpu.rks_scal.buf),
        fixed_array(RKS_MAX_QD * 4l, (RKS_MAX_QD + 2l * RKS_MAX_KVD) * 4l,
            RKS_MAX_CTX * RKS_MAX_KVD * 4l, RKS_MAX_CTX * RKS_MAX_KVD * 4l, XS_BYTES),
        fixed_array(VHG_Y1, 0u, VHG_Y2, VHG_Y3, 0u))   // roped q / K mirror / V mirror
    g_gpu.rks_ready = true
}

//! Resident-driver seam (standalone-kernel form): rope the q row in place, rope the k row into the
//! device K mirror at row `pos`, copy v raw into the V mirror at `pos`; then read q + that row's
//! K/V back for parity. `neox` picks llama's adjacent-pair vs qwen/gemma's half-offset rope.
def vk_rope_kv_store(var qkv : array<float>; var qout : array<float>; var krow : array<float>; var vrow : array<float>;
                     cossin : array<float>; qd, kv_dim, head_size, pos : int64; neox : bool) {
    verify(vk_moe_init())
    let half = int(head_size / 2l)
    assert(qd <= RKS_MAX_QD && kv_dim <= RKS_MAX_KVD && pos < RKS_MAX_CTX, "dasLLAMA vulkan: rope_kv geometry over scratch")
    ensure_rks_state()
    let kpair0 = int(qd / 2l)
    let npairs = kpair0 + int(kv_dim / 2l)
    unsafe {
        memcpy(g_gpu.rks_scal.mapped, addr<void?>(cossin[0]), head_size * 4l)   // cos[half] + sin[half]
        upload_region_at(g_gpu.rks_qkv_dev, 0l, addr<uint8 const?>(qkv[0]), (qd + 2l * kv_dim) * 4l)
        // q ropes in place in its own plane — seed it with the raw q rows
        upload_region_at(g_gpu.rks_q_dev, 0l, addr<uint8 const?>(qkv[0]), qd * 4l)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        var pc = RopeKvArgs(qd = uint(qd), kvd = uint(kv_dim), hs = uint(head_size),
            half = uint(half), neox = neox ? 1u : 0u, kpair0 = uint(kpair0),
            npairs = uint(npairs), krow = uint(pos * kv_dim), vrow = uint(pos * kv_dim))
        enc_rope_kv_cls(raw, h, g_gpu.rks_set, pc, (int64(npairs) + int64(WG_X) - 1l) / int64(WG_X))
        vhz_dep(raw, h, VHG_Y1 | VHG_Y2 | VHG_Y3, 0u, true)   // one declaration for the q/k/v readback group
        cmd_copy_whole(raw, g_gpu.rks_q_dev, g_gpu.rks_q_host.buf, qd * 4l)
        cmd_copy_range(raw, g_gpu.rks_k_dev, pos * kv_dim * 4l, g_gpu.rks_kv_host.buf, 0l, kv_dim * 4l)
        cmd_copy_range(raw, g_gpu.rks_v_dev, pos * kv_dim * 4l, g_gpu.rks_kv_host.buf, kv_dim * 4l, kv_dim * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(qout[0]), g_gpu.rks_q_host.mapped, qd * 4l)
        memcpy(addr<void?>(krow[0]), g_gpu.rks_kv_host.mapped, kv_dim * 4l)
        memcpy(addr<void?>(vrow[0]), reinterpret<float?>(g_gpu.rks_kv_host.mapped) + kv_dim, kv_dim * 4l)
    }
}

def private ensure_da_state {
    if (g_gpu.da_ready) {
        return
    }
    ensure_rks_state()   // shares the K/V mirror planes
    verify(ensure_da_attn_cls(), "vulkan seams: the decode-attn class rail must engage on a live device")
    g_gpu.da_q_dev = make_device_buf(RKS_MAX_QD * 4l)
    g_gpu.da_out_dev = make_device_buf(RKS_MAX_QD * 4l)
    g_gpu.da_out_host = make_host_buf(RKS_MAX_QD * 4l, false, [cached = true])
    g_gpu.da_set = set_da_attn_cls(
        fixed_array(g_gpu.da_q_dev, g_gpu.rks_k_dev, g_gpu.rks_v_dev, g_gpu.da_out_dev),
        fixed_array(RKS_MAX_QD * 4l, RKS_MAX_CTX * RKS_MAX_KVD * 4l,
            RKS_MAX_CTX * RKS_MAX_KVD * 4l, RKS_MAX_QD * 4l),
        fixed_array(0u, 0u, 0u, VHG_YO))   // q/mirrors fill pre-cmd (fence-ordered)
    g_gpu.da_ready = true
}

//! Resident-driver seam: decode attention of the roped q row against the `cnt` cached positions in
//! the device K/V mirror (the same planes vk_rope_kv_store wrote), GQA per kv_mul, into out[0..qd).
def vk_decode_attn(var q : array<float>; var out : array<float>; cnt, qd, kv_dim, head_size, kv_mul : int64; scale : float) {
    verify(vk_moe_init())
    assert(head_size <= 256l && qd <= RKS_MAX_QD, "dasLLAMA vulkan: decode-attn geometry over scratch")
    ensure_da_state()
    let n_heads = int(qd / head_size)
    unsafe {
        upload_region_at(g_gpu.da_q_dev, 0l, addr<uint8 const?>(q[0]), qd * 4l)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        var pc = DaAttnArgs(cnt = uint(cnt), kvd = uint(kv_dim), hs = uint(head_size),
            kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u, scale = scale)
        enc_da_attn_cls(raw, h, g_gpu.da_set, pc, int64(n_heads))
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, g_gpu.da_out_dev, g_gpu.da_out_host.buf, qd * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(out[0]), g_gpu.da_out_host.mapped, qd * 4l)
    }
}
