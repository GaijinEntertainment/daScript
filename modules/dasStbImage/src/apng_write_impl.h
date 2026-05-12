// Streaming Animated PNG (APNG) writer for daslang.
//
// This header MUST be #included from a translation unit that has
//   #define STB_IMAGE_WRITE_IMPLEMENTATION
// already in effect, because we reuse stb's internal helpers
// (stbiw__crc32) and externally-linkable stbi_zlib_compress, plus
// the stbi_write_png_compression_level global.
//
// Public C API (also forward-declared in dasStbImage.cpp for binding):
//   void * stbi_apng_begin(const char *filename, int w, int h, int channels);
//   int    stbi_apng_frame(void *writer, const void *pixels, int stride_bytes, int delay_ms);
//   int    stbi_apng_end(void *writer);
//   int    stbi_apng_dropped(void *writer);
//
// Pixel data submitted to stbi_apng_frame is expected to be bottom-up
// (glReadPixels output). The worker thread flips rows top-down before encoding.

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>

#if defined(_WIN32)
#  define stbi__apng_fseeko(fp, off, whence) _fseeki64((fp), (__int64)(off), (whence))
#  define stbi__apng_ftello(fp)              _ftelli64(fp)
typedef long long stbi__apng_off_t;
#else
#  include <sys/types.h>
#  define stbi__apng_fseeko(fp, off, whence) fseeko((fp), (off_t)(off), (whence))
#  define stbi__apng_ftello(fp)              ftello(fp)
typedef off_t stbi__apng_off_t;
#endif

namespace stbi_apng_detail {

struct Frame {
    std::vector<uint8_t> pixels;
    int                  delay_ms;
};

static inline void put_be_u32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)((v >> 24) & 0xFFu);
    p[1] = (uint8_t)((v >> 16) & 0xFFu);
    p[2] = (uint8_t)((v >> 8)  & 0xFFu);
    p[3] = (uint8_t)( v        & 0xFFu);
}
static inline void put_be_u16(uint8_t *p, uint16_t v) {
    p[0] = (uint8_t)((v >> 8) & 0xFFu);
    p[1] = (uint8_t)( v       & 0xFFu);
}

// Write one PNG chunk: [length:u32][type:4][body:length][crc:u32].
// CRC covers type+body. Returns 1 on success.
static int write_chunk(FILE *fp, const char *type, const uint8_t *body, uint32_t body_len) {
    uint8_t hdr[8];
    put_be_u32(hdr, body_len);
    memcpy(hdr + 4, type, 4);
    if (fwrite(hdr, 1, 8, fp) != 8) return 0;
    if (body_len && fwrite(body, 1, body_len, fp) != body_len) return 0;
    std::vector<uint8_t> crcbuf((size_t)4 + (size_t)body_len);
    memcpy(crcbuf.data(), type, 4);
    if (body_len) memcpy(crcbuf.data() + 4, body, body_len);
    uint32_t crc = stbiw__crc32(crcbuf.data(), 4 + (int)body_len);
    uint8_t crcbe[4];
    put_be_u32(crcbe, crc);
    if (fwrite(crcbe, 1, 4, fp) != 4) return 0;
    return 1;
}

class ApngWriter {
public:
    bool begin(const char *filename, int W, int H, int CH);
    bool enqueue(const void *pixels, int stride_bytes, int delay_ms);
    bool end();
    int  dropped();

private:
    void worker_loop();
    bool emit_frame(Frame &f);

    FILE *fp = nullptr;
    int   w  = 0, h = 0, channels = 0;
    stbi__apng_off_t acTL_body_offset = -1;

    std::mutex              mu;
    std::condition_variable cv;
    std::deque<Frame>       queue;
    int                     max_queue      = 4;
    int                     drop_count     = 0;
    bool                    errored        = false;
    bool                    stop_requested = false;

    std::thread worker;
    int         frames_written = 0;  // worker-thread-only after begin() returns
};

inline bool ApngWriter::begin(const char *filename, int W, int H, int CH) {
    if (W <= 0 || H <= 0) return false;
    if (CH != 3 && CH != 4) return false;
    w = W; h = H; channels = CH;
    fp = fopen(filename, "wb");
    if (!fp) return false;

    static const uint8_t sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    if (fwrite(sig, 1, 8, fp) != 8) goto fail;

    {
        uint8_t ihdr[13];
        put_be_u32(ihdr + 0, (uint32_t)w);
        put_be_u32(ihdr + 4, (uint32_t)h);
        ihdr[8]  = 8;                          // bit_depth
        ihdr[9]  = (channels == 4) ? 6 : 2;    // color_type: 6=RGBA, 2=RGB
        ihdr[10] = 0;                          // compression = deflate
        ihdr[11] = 0;                          // filter      = adaptive (we use 0/None per-row)
        ihdr[12] = 0;                          // interlace   = none
        if (!write_chunk(fp, "IHDR", ihdr, 13)) goto fail;
    }

    {
        // Remember acTL body offset for backpatch (skip 4-byte length + 4-byte type).
        stbi__apng_off_t before_acTL = stbi__apng_ftello(fp);
        if (before_acTL < 0) goto fail;
        uint8_t actl[8];
        put_be_u32(actl + 0, 0);   // num_frames — backpatched on close
        put_be_u32(actl + 4, 0);   // num_plays  — 0 means loop forever
        if (!write_chunk(fp, "acTL", actl, 8)) goto fail;
        acTL_body_offset = before_acTL + 8;
    }

    {
        std::lock_guard<std::mutex> lk(mu);
        drop_count     = 0;
        errored        = false;
        stop_requested = false;
        frames_written = 0;
    }
    worker = std::thread(&ApngWriter::worker_loop, this);
    return true;

fail:
    if (fp) { fclose(fp); fp = nullptr; }
    return false;
}

inline bool ApngWriter::enqueue(const void *pixels, int stride_bytes, int delay_ms) {
    std::unique_lock<std::mutex> lk(mu);
    if (errored) return false;
    if ((int)queue.size() >= max_queue) {
        queue.pop_front();
        drop_count++;
    }
    Frame f;
    size_t row_bytes = (size_t)w * (size_t)channels;
    f.pixels.resize(row_bytes * (size_t)h);
    const uint8_t *src = (const uint8_t *)pixels;
    if ((size_t)stride_bytes == row_bytes) {
        memcpy(f.pixels.data(), src, f.pixels.size());
    } else {
        for (int y = 0; y < h; y++) {
            memcpy(f.pixels.data() + (size_t)y * row_bytes,
                   src             + (size_t)y * (size_t)stride_bytes,
                   row_bytes);
        }
    }
    f.delay_ms = delay_ms;
    queue.emplace_back(std::move(f));
    lk.unlock();
    cv.notify_one();
    return true;
}

inline void ApngWriter::worker_loop() {
    while (true) {
        Frame f;
        {
            std::unique_lock<std::mutex> lk(mu);
            cv.wait(lk, [this]{ return !queue.empty() || stop_requested; });
            if (queue.empty() && stop_requested) return;
            f = std::move(queue.front());
            queue.pop_front();
        }
        if (!emit_frame(f)) {
            std::lock_guard<std::mutex> lk(mu);
            errored = true;
            queue.clear();
            return;
        }
    }
}

inline bool ApngWriter::emit_frame(Frame &f) {
    size_t row_bytes = (size_t)w * (size_t)channels;

    // Flip rows in place: glReadPixels emits bottom-up; PNG is top-down.
    {
        std::vector<uint8_t> tmp(row_bytes);
        for (int y = 0; y < h / 2; y++) {
            uint8_t *top = f.pixels.data() + (size_t)y           * row_bytes;
            uint8_t *bot = f.pixels.data() + (size_t)(h - 1 - y) * row_bytes;
            memcpy(tmp.data(), top, row_bytes);
            memcpy(top, bot, row_bytes);
            memcpy(bot, tmp.data(), row_bytes);
        }
    }

    // Build deflate input: [filter=0x00 || row] per scanline.
    std::vector<uint8_t> in((row_bytes + 1) * (size_t)h);
    {
        uint8_t *p = in.data();
        for (int y = 0; y < h; y++) {
            *p++ = 0;
            memcpy(p, f.pixels.data() + (size_t)y * row_bytes, row_bytes);
            p += row_bytes;
        }
    }

    int            zlib_len = 0;
    unsigned char *zlib_buf = stbi_zlib_compress(in.data(), (int)in.size(),
                                                 &zlib_len, stbi_write_png_compression_level);
    if (!zlib_buf) return false;

    // fcTL — frame N≥1 gets seq=2N-1, frame 0 gets seq=0.
    {
        uint8_t fctl[26];
        uint32_t seq = (frames_written == 0) ? 0u : (uint32_t)(2 * frames_written - 1);
        put_be_u32(fctl +  0, seq);
        put_be_u32(fctl +  4, (uint32_t)w);
        put_be_u32(fctl +  8, (uint32_t)h);
        put_be_u32(fctl + 12, 0);    // x_offset
        put_be_u32(fctl + 16, 0);    // y_offset
        int dms = f.delay_ms;
        if (dms < 0)     dms = 0;
        if (dms > 65535) dms = 65535;
        put_be_u16(fctl + 20, (uint16_t)dms);   // delay_num — ms
        put_be_u16(fctl + 22, 1000);            // delay_den — 1/1000 sec base
        fctl[24] = 0;                            // dispose_op = APNG_DISPOSE_OP_NONE
        fctl[25] = 0;                            // blend_op   = APNG_BLEND_OP_SOURCE
        if (!write_chunk(fp, "fcTL", fctl, 26)) {
            STBIW_FREE(zlib_buf);
            return false;
        }
    }

    // IDAT (frame 0) or fdAT (frame N≥1, body = u32 seq || zlib stream).
    bool ok;
    if (frames_written == 0) {
        ok = write_chunk(fp, "IDAT", zlib_buf, (uint32_t)zlib_len) != 0;
    } else {
        uint32_t              fdat_seq = (uint32_t)(2 * frames_written);
        std::vector<uint8_t>  fdat_body(4 + (size_t)zlib_len);
        put_be_u32(fdat_body.data(), fdat_seq);
        memcpy(fdat_body.data() + 4, zlib_buf, (size_t)zlib_len);
        ok = write_chunk(fp, "fdAT", fdat_body.data(), (uint32_t)fdat_body.size()) != 0;
    }
    STBIW_FREE(zlib_buf);
    if (!ok) return false;

    frames_written++;
    return true;
}

inline bool ApngWriter::end() {
    {
        std::lock_guard<std::mutex> lk(mu);
        stop_requested = true;
    }
    cv.notify_one();
    if (worker.joinable()) worker.join();

    if (!fp) return false;
    bool was_errored;
    {
        std::lock_guard<std::mutex> lk(mu);
        was_errored = errored;
    }
    bool result = !was_errored;

    // Backpatch acTL.num_frames + recomputed CRC. Even when errored, we still
    // emit a valid truncated APNG so the partial recording is salvageable.
    if (acTL_body_offset >= 0 && frames_written > 0) {
        uint8_t newbody[8];
        put_be_u32(newbody + 0, (uint32_t)frames_written);
        put_be_u32(newbody + 4, 0);  // num_plays unchanged
        uint8_t crcbuf[12];
        memcpy(crcbuf,     "acTL", 4);
        memcpy(crcbuf + 4, newbody, 8);
        uint32_t crc = stbiw__crc32(crcbuf, 12);
        uint8_t patch[12];
        memcpy(patch, newbody, 8);
        put_be_u32(patch + 8, crc);
        if (stbi__apng_fseeko(fp, acTL_body_offset, SEEK_SET) != 0) result = false;
        else if (fwrite(patch, 1, 12, fp) != 12) result = false;
        if (stbi__apng_fseeko(fp, 0, SEEK_END) != 0) result = false;
    }

    if (!write_chunk(fp, "IEND", nullptr, 0)) result = false;
    fflush(fp);
    fclose(fp);
    fp = nullptr;
    return result;
}

inline int ApngWriter::dropped() {
    std::lock_guard<std::mutex> lk(mu);
    return drop_count;
}

} // namespace stbi_apng_detail


// ============================================================================
// C-callable API — opaque void * handle for the daslang binding.
// ============================================================================

struct StbiApngWriterImpl {
    stbi_apng_detail::ApngWriter impl;
};

extern "C" {

void * stbi_apng_begin(const char *filename, int w, int h, int channels) {
    if (!filename) return nullptr;
    StbiApngWriterImpl *handle = new StbiApngWriterImpl();
    if (!handle->impl.begin(filename, w, h, channels)) {
        delete handle;
        return nullptr;
    }
    return handle;
}

int stbi_apng_frame(void *writer, const void *pixels, int stride_bytes, int delay_ms) {
    if (!writer || !pixels) return 0;
    StbiApngWriterImpl *handle = (StbiApngWriterImpl *)writer;
    return handle->impl.enqueue(pixels, stride_bytes, delay_ms) ? 1 : 0;
}

int stbi_apng_end(void *writer) {
    if (!writer) return 0;
    StbiApngWriterImpl *handle = (StbiApngWriterImpl *)writer;
    bool ok = handle->impl.end();
    delete handle;
    return ok ? 1 : 0;
}

int stbi_apng_dropped(void *writer) {
    if (!writer) return 0;
    StbiApngWriterImpl *handle = (StbiApngWriterImpl *)writer;
    return handle->impl.dropped();
}

} // extern "C"
