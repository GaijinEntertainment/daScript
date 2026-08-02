#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/misc/env_cfg.h"
#include "aot_builtin_clipboard.h"
#include "clip.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <limits>
#include <string>
#include <vector>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#ifndef DAS_CLIPBOARD_IMAGE_SUPPORTED
#define DAS_CLIPBOARD_IMAGE_SUPPORTED 0
#endif

namespace das {

    namespace {

        constexpr int32_t desktopCapabilities =
            clipboard_read_text | clipboard_write_text |
            clipboard_clear | clipboard_synchronous |
            clipboard_read_rich_text | clipboard_write_rich_text
#if DAS_CLIPBOARD_IMAGE_SUPPORTED
            | clipboard_read_image | clipboard_write_image
#endif
            ;

        constexpr int32_t knownFormats =
            clipboard_plain_text | clipboard_markdown | clipboard_html |
            clipboard_uri_list | clipboard_image_rgba8;

        void ignoreClipError(clip::ErrorCode) {
            // The public API reports failures as ClipboardStatus.  The vendored
            // library's default handler throws, which would erase that contract.
        }

        bool nativeClipboardAvailable() {
#if !DAS_CLIPBOARD_NATIVE_SUPPORTED
            return false;
#elif defined(__linux__)
            // clip's Linux backend is XCB.  A Wayland session with XWayland has
            // DISPLAY and works; a native Wayland-only session is unavailable.
            const char * display = das_getenv("DISPLAY");
            return display && *display;
#else
            return true;
#endif
        }

        ClipboardStatus unavailableStatus() {
#if DAS_CLIPBOARD_NATIVE_SUPPORTED
            return ClipboardStatus::unavailable;
#else
            return ClipboardStatus::unsupported;
#endif
        }

        ClipboardStatus lockClipboard(clip::lock & lock) {
            return lock.locked() ? ClipboardStatus::ok : ClipboardStatus::busy;
        }

        int32_t statusCode(ClipboardStatus status) {
            return static_cast<int32_t>(status);
        }

        clip::format markdownFormat() {
            static const clip::format value = clip::register_format("text/markdown");
            return value;
        }

        clip::format htmlFormat() {
#if defined(_WIN32)
            static const clip::format value = clip::register_format("HTML Format");
#elif defined(__APPLE__)
            static const clip::format value = clip::register_format("public.html");
#else
            static const clip::format value = clip::register_format("text/html");
#endif
            return value;
        }

        clip::format uriListFormat() {
            static const clip::format value = clip::register_format("text/uri-list");
            return value;
        }

        bool validBytes(const char * data, int32_t size) {
            return size >= 0 && (data || size == 0);
        }

        bool setRegisteredData(clip::lock & lock, clip::format format,
                               const char * data, size_t size) {
#if defined(_WIN32)
            // clip's generic Windows custom-format rail prepends a private
            // uint64 length. Standards-visible formats must be raw HGLOBAL
            // data instead, with a trailing NUL for text consumers.
            if (size == std::numeric_limits<size_t>::max()) return false;
            HGLOBAL memory = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, size + 1);
            if (!memory) return false;
            void * dst = GlobalLock(memory);
            if (!dst) {
                GlobalFree(memory);
                return false;
            }
            if (size) std::memcpy(dst, data, size);
            GlobalUnlock(memory);
            if (!SetClipboardData(static_cast<UINT>(format), memory)) {
                GlobalFree(memory);
                return false;
            }
            return true;
#else
            return lock.set_data(format, data ? data : "", size);
#endif
        }

        bool getRegisteredData(const clip::lock & lock, clip::format format,
                               std::string & data) {
            data.clear();
            if (!lock.is_convertible(format)) return false;
#if defined(_WIN32)
            HGLOBAL memory = GetClipboardData(static_cast<UINT>(format));
            if (!memory) return false;
            const size_t capacity = GlobalSize(memory);
            const char * src = static_cast<const char *>(GlobalLock(memory));
            if (!src) return false;
            size_t size = 0;
            while (size < capacity && src[size] != '\0') ++size;
            data.assign(src, size);
            GlobalUnlock(memory);
            return true;
#else
            const size_t size = lock.get_data_length(format);
            if (size == 0) return true;
            std::vector<char> buffer(size);
            if (!lock.get_data(format, buffer.data(), size)) return false;
            size_t textSize = size;
            while (textSize > 0 && buffer[textSize - 1] == 0) --textSize;
            data.assign(buffer.data(), textSize);
            return true;
#endif
        }

#if defined(_WIN32)
        std::string makeCfHtml(const char * fragment, size_t fragmentSize) {
            constexpr const char * prefix =
                "<html><body>\r\n<!--StartFragment-->";
            constexpr const char * suffix =
                "<!--EndFragment-->\r\n</body></html>";
            constexpr const char * headerFormat =
                "Version:0.9\r\n"
                "StartHTML:%010llu\r\n"
                "EndHTML:%010llu\r\n"
                "StartFragment:%010llu\r\n"
                "EndFragment:%010llu\r\n";

            char probe[256];
            const int headerSize = std::snprintf(probe, sizeof(probe), headerFormat,
                0ull, 0ull, 0ull, 0ull);
            if (headerSize <= 0 || static_cast<size_t>(headerSize) >= sizeof(probe)) {
                return {};
            }
            const unsigned long long startHtml = static_cast<unsigned long long>(headerSize);
            const unsigned long long startFragment = startHtml + std::strlen(prefix);
            const unsigned long long endFragment = startFragment + fragmentSize;
            const unsigned long long endHtml = endFragment + std::strlen(suffix);
            if (endHtml >= 10000000000ull) return {};

            char header[256];
            const int written = std::snprintf(header, sizeof(header), headerFormat,
                startHtml, endHtml, startFragment, endFragment);
            if (written != headerSize) return {};
            std::string result(header, static_cast<size_t>(written));
            result += prefix;
            result.append(fragment, fragmentSize);
            result += suffix;
            return result;
        }

        bool parseCfHtmlOffset(const std::string & data, const char * key,
                               size_t & value) {
            const size_t keyAt = data.find(key);
            if (keyAt == std::string::npos) return false;
            size_t at = keyAt + std::strlen(key);
            if (at >= data.size() || data[at] != ':') return false;
            ++at;
            while (at < data.size() && std::isspace(static_cast<unsigned char>(data[at]))) ++at;
            if (at >= data.size() || !std::isdigit(static_cast<unsigned char>(data[at]))) return false;
            size_t parsed = 0;
            while (at < data.size() && std::isdigit(static_cast<unsigned char>(data[at]))) {
                const unsigned digit = static_cast<unsigned>(data[at] - '0');
                if (parsed > (std::numeric_limits<size_t>::max() - digit) / 10) return false;
                parsed = parsed * 10 + digit;
                ++at;
            }
            value = parsed;
            return true;
        }

        std::string extractCfHtmlFragment(const std::string & data) {
            size_t begin = 0, end = 0;
            if (parseCfHtmlOffset(data, "StartFragment", begin) &&
                parseCfHtmlOffset(data, "EndFragment", end) &&
                begin <= end && end <= data.size()) {
                return data.substr(begin, end - begin);
            }
            // Some producers put raw HTML in the registered format. Keeping it
            // is more useful than discarding an otherwise valid representation.
            return data;
        }
#endif

#if DAS_CLIPBOARD_IMAGE_SUPPORTED
        uint8_t extractMaskedChannel(uint32_t pixel, uint32_t mask, uint8_t fallback) {
            if (!mask) return fallback;
            unsigned shift = 0;
            while (shift < 32u && ((mask >> shift) & 1u) == 0u) ++shift;
            if (shift == 32u) return fallback;
            const uint32_t shiftedMask = mask >> shift;
            const uint32_t value = (pixel & mask) >> shift;
            return static_cast<uint8_t>((value * 255u + shiftedMask / 2u) / shiftedMask);
        }
#endif

#if defined(_WIN32) && DAS_CLIPBOARD_IMAGE_SUPPORTED
        bool setWindowsImage(const TArray<uint8_t> & rgba8,
                             int32_t width, int32_t height, int32_t stride) {
            const size_t rowBytes = static_cast<size_t>(width) * 4;
            const size_t pixelBytes = rowBytes * static_cast<size_t>(height);
            if (pixelBytes > std::numeric_limits<DWORD>::max() ||
                pixelBytes > std::numeric_limits<size_t>::max() - sizeof(BITMAPV5HEADER)) {
                return false;
            }
            HGLOBAL memory = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,
                                         sizeof(BITMAPV5HEADER) + pixelBytes);
            if (!memory) return false;
            auto * header = static_cast<BITMAPV5HEADER *>(GlobalLock(memory));
            if (!header) {
                GlobalFree(memory);
                return false;
            }
            header->bV5Size = sizeof(BITMAPV5HEADER);
            header->bV5Width = width;
            header->bV5Height = -height; // canonical input is top-to-bottom
            header->bV5Planes = 1;
            header->bV5BitCount = 32;
            header->bV5Compression = BI_BITFIELDS;
            header->bV5SizeImage = static_cast<DWORD>(pixelBytes);
            header->bV5RedMask = 0x00ff0000;
            header->bV5GreenMask = 0x0000ff00;
            header->bV5BlueMask = 0x000000ff;
            header->bV5AlphaMask = 0xff000000;
            header->bV5CSType = LCS_sRGB;
            auto * dst = reinterpret_cast<uint8_t *>(header + 1);
            for (int32_t y = 0; y != height; ++y) {
                const uint8_t * src = reinterpret_cast<const uint8_t *>(rgba8.data) +
                                      static_cast<size_t>(y) * stride;
                uint8_t * row = dst + static_cast<size_t>(y) * rowBytes;
                for (int32_t x = 0; x != width; ++x) {
                    row[x * 4 + 0] = src[x * 4 + 2];
                    row[x * 4 + 1] = src[x * 4 + 1];
                    row[x * 4 + 2] = src[x * 4 + 0];
                    row[x * 4 + 3] = src[x * 4 + 3];
                }
            }
            GlobalUnlock(memory);
            if (!SetClipboardData(CF_DIBV5, memory)) {
                GlobalFree(memory);
                return false;
            }
            return true;
        }

        bool getWindowsImage(std::vector<uint8_t> & rgba8,
                             int32_t & width, int32_t & height, int32_t & stride) {
            const UINT format = IsClipboardFormatAvailable(CF_DIBV5) ? CF_DIBV5 :
                                (IsClipboardFormatAvailable(CF_DIB) ? CF_DIB : 0);
            if (!format) return false;
            HGLOBAL memory = GetClipboardData(format);
            if (!memory) return false;
            const size_t total = GlobalSize(memory);
            const uint8_t * base = static_cast<const uint8_t *>(GlobalLock(memory));
            if (!base) return false;
            if (total < sizeof(BITMAPINFOHEADER)) {
                GlobalUnlock(memory);
                return false;
            }
            const auto * info = reinterpret_cast<const BITMAPINFOHEADER *>(base);
            if (info->biSize < sizeof(BITMAPINFOHEADER) || info->biSize > total ||
                info->biWidth <= 0 || info->biHeight == 0 ||
                (info->biBitCount != 24 && info->biBitCount != 32) ||
                (info->biCompression != BI_RGB && info->biCompression != BI_BITFIELDS)) {
                GlobalUnlock(memory);
                return false;
            }
            const uint64_t absHeight = info->biHeight < 0
                ? static_cast<uint64_t>(-static_cast<int64_t>(info->biHeight))
                : static_cast<uint64_t>(info->biHeight);
            if (static_cast<uint64_t>(info->biWidth) > INT32_MAX || absHeight > INT32_MAX) {
                GlobalUnlock(memory);
                return false;
            }
            const uint64_t srcStride64 =
                ((static_cast<uint64_t>(info->biWidth) * info->biBitCount + 31u) / 32u) * 4u;
            size_t pixelOffset = info->biSize;
            uint32_t redMask = 0x00ff0000, greenMask = 0x0000ff00;
            uint32_t blueMask = 0x000000ff, alphaMask = 0;
            if (info->biCompression == BI_BITFIELDS) {
                if (info->biSize >= sizeof(BITMAPV4HEADER)) {
                    const auto * v4 = reinterpret_cast<const BITMAPV4HEADER *>(base);
                    redMask = v4->bV4RedMask;
                    greenMask = v4->bV4GreenMask;
                    blueMask = v4->bV4BlueMask;
                    alphaMask = v4->bV4AlphaMask;
                } else {
                    if (pixelOffset + 3 * sizeof(uint32_t) > total) {
                        GlobalUnlock(memory);
                        return false;
                    }
                    const auto * masks = reinterpret_cast<const uint32_t *>(base + pixelOffset);
                    redMask = masks[0]; greenMask = masks[1]; blueMask = masks[2];
                    pixelOffset += 3 * sizeof(uint32_t);
                }
            }
            const uint64_t sourceBytes = srcStride64 * absHeight;
            if (srcStride64 > SIZE_MAX || sourceBytes > SIZE_MAX ||
                pixelOffset > total || static_cast<size_t>(sourceBytes) > total - pixelOffset) {
                GlobalUnlock(memory);
                return false;
            }
            width = info->biWidth;
            height = static_cast<int32_t>(absHeight);
            stride = width * 4;
            rgba8.resize(static_cast<size_t>(stride) * height);
            const uint8_t * pixels = base + pixelOffset;
            for (int32_t y = 0; y != height; ++y) {
                const int32_t sourceY = info->biHeight < 0 ? y : height - 1 - y;
                const uint8_t * src = pixels + static_cast<size_t>(sourceY) * static_cast<size_t>(srcStride64);
                uint8_t * dst = rgba8.data() + static_cast<size_t>(y) * stride;
                for (int32_t x = 0; x != width; ++x) {
                    if (info->biBitCount == 24) {
                        dst[x * 4 + 0] = src[x * 3 + 2];
                        dst[x * 4 + 1] = src[x * 3 + 1];
                        dst[x * 4 + 2] = src[x * 3 + 0];
                        dst[x * 4 + 3] = 255;
                    } else {
                        uint32_t pixel = 0;
                        std::memcpy(&pixel, src + x * 4, sizeof(pixel));
                        dst[x * 4 + 0] = extractMaskedChannel(pixel, redMask, 0);
                        dst[x * 4 + 1] = extractMaskedChannel(pixel, greenMask, 0);
                        dst[x * 4 + 2] = extractMaskedChannel(pixel, blueMask, 0);
                        dst[x * 4 + 3] = extractMaskedChannel(pixel, alphaMask, 255);
                    }
                }
            }
            GlobalUnlock(memory);
            return true;
        }
#endif

#if defined(__APPLE__) && DAS_CLIPBOARD_IMAGE_SUPPORTED
        bool setAppleImage(clip::lock & lock, const TArray<uint8_t> & rgba8,
                           int32_t width, int32_t height, int32_t stride) {
            clip::image_spec spec;
            spec.width = static_cast<unsigned long>(width);
            spec.height = static_cast<unsigned long>(height);
            spec.bits_per_pixel = 32;
            spec.bytes_per_row = static_cast<unsigned long>(stride);
            spec.red_mask = 0x000000ff;
            spec.green_mask = 0x0000ff00;
            spec.blue_mask = 0x00ff0000;
            spec.alpha_mask = 0xff000000;
            spec.red_shift = 0;
            spec.green_shift = 8;
            spec.blue_shift = 16;
            spec.alpha_shift = 24;
            clip::image image(rgba8.data, spec);
            return image.is_valid() && lock.set_image(image);
        }

        bool getAppleImage(const clip::lock & lock, std::vector<uint8_t> & rgba8,
                           int32_t & width, int32_t & height, int32_t & stride) {
            clip::image image;
            if (!lock.get_image(image) || !image.is_valid()) return false;
            const clip::image_spec & spec = image.spec();
            if (spec.width == 0 || spec.height == 0 ||
                spec.width > INT32_MAX || spec.height > INT32_MAX ||
                (spec.bits_per_pixel != 16 && spec.bits_per_pixel != 24 &&
                 spec.bits_per_pixel != 32)) return false;
            const size_t bytesPerPixel = (spec.bits_per_pixel + 7) / 8;
            if (spec.bytes_per_row < spec.width * bytesPerPixel) return false;
            width = static_cast<int32_t>(spec.width);
            height = static_cast<int32_t>(spec.height);
            stride = width * 4;
            rgba8.resize(static_cast<size_t>(stride) * height);
            for (int32_t y = 0; y != height; ++y) {
                const uint8_t * src = reinterpret_cast<const uint8_t *>(image.data()) +
                                      static_cast<size_t>(y) * spec.bytes_per_row;
                uint8_t * dst = rgba8.data() + static_cast<size_t>(y) * stride;
                for (int32_t x = 0; x != width; ++x) {
                    uint32_t pixel = 0;
                    std::memcpy(&pixel, src + static_cast<size_t>(x) * bytesPerPixel,
                                bytesPerPixel);
                    dst[x * 4 + 0] = extractMaskedChannel(pixel, spec.red_mask, 0);
                    dst[x * 4 + 1] = extractMaskedChannel(pixel, spec.green_mask, 0);
                    dst[x * 4 + 2] = extractMaskedChannel(pixel, spec.blue_mask, 0);
                    dst[x * 4 + 3] = extractMaskedChannel(pixel, spec.alpha_mask, 255);
                }
            }
            return true;
        }
#endif

        ClipboardStatus getClipboardTextLocked(const clip::lock & lock,
                                                std::string & text) {
            text.clear();
            const clip::format format = clip::text_format();
            if (!lock.is_convertible(format)) return ClipboardStatus::no_text;
            const size_t bufferSize = lock.get_data_length(format);
            if (bufferSize == 0) return ClipboardStatus::ok;
            std::vector<char> buffer(bufferSize + 1, 0);
            if (!lock.get_data(format, buffer.data(), bufferSize)) {
                return ClipboardStatus::failed;
            }
            size_t textSize = bufferSize;
            while (textSize > 0 && buffer[textSize - 1] == 0) --textSize;
            text.assign(buffer.data(), textSize);
            return ClipboardStatus::ok;
        }

        bool setClipboardTextLocked(clip::lock & lock, const char * text,
                                    size_t textSize) {
#if defined(_WIN32)
            if (textSize == 0) {
                HGLOBAL memory = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,
                                             sizeof(wchar_t));
                if (!memory) return false;
                if (!SetClipboardData(CF_UNICODETEXT, memory)) {
                    GlobalFree(memory);
                    return false;
                }
                return true;
            }
#endif
            return lock.set_data(clip::text_format(), text ? text : "", textSize);
        }

        ClipboardStatus getClipboardText(std::string & text) {
            text.clear();
            if (!nativeClipboardAvailable()) return unavailableStatus();
            clip::lock lock;
            ClipboardStatus status = lockClipboard(lock);
            if (status != ClipboardStatus::ok) return status;
            return getClipboardTextLocked(lock, text);
        }

    }

    int32_t builtin_clipboard_capabilities() {
        return nativeClipboardAvailable() ? desktopCapabilities : 0;
    }

    int32_t builtin_clipboard_set_text(const char * text, int32_t text_size) {
        if (text_size < 0 || (!text && text_size != 0)) {
            return statusCode(ClipboardStatus::failed);
        }
        if (!nativeClipboardAvailable()) return statusCode(unavailableStatus());
        clip::lock lock;
        ClipboardStatus status = lockClipboard(lock);
        if (status != ClipboardStatus::ok) {
            return statusCode(status);
        }
        if (!lock.clear()) {
            return statusCode(ClipboardStatus::failed);
        }
        return statusCode(setClipboardTextLocked(lock, text,
                                                 static_cast<size_t>(text_size))
            ? ClipboardStatus::ok : ClipboardStatus::failed);
    }

    char * builtin_clipboard_get_text(int32_t & resultStatus, Context * context, LineInfoArg * at) {
        std::string text;
        ClipboardStatus status = getClipboardText(text);
        resultStatus = statusCode(status);
        return context->allocateString(text.data(), text.size(), at);
    }

    const char * builtin_clipboard_get_text_temporary(int32_t & resultStatus) {
        static thread_local std::string text;
        ClipboardStatus status = getClipboardText(text);
        resultStatus = statusCode(status);
        return status == ClipboardStatus::ok ? text.c_str() : nullptr;
    }

    int32_t builtin_clipboard_clear() {
        if (!nativeClipboardAvailable()) return statusCode(unavailableStatus());
        clip::lock lock;
        ClipboardStatus status = lockClipboard(lock);
        if (status != ClipboardStatus::ok) {
            return statusCode(status);
        }
        return statusCode(lock.clear() ? ClipboardStatus::ok
                                      : ClipboardStatus::failed);
    }

    int32_t builtin_clipboard_set_content(
        int32_t formats,
        const char * plainText, int32_t plainTextSize,
        const char * markdown, int32_t markdownSize,
        const char * html, int32_t htmlSize,
        const char * uriList, int32_t uriListSize,
        const TArray<uint8_t> & imageRgba8,
        int32_t imageWidth, int32_t imageHeight, int32_t imageStride) {
        if ((formats & ~knownFormats) != 0 ||
            !validBytes(plainText, plainTextSize) ||
            !validBytes(markdown, markdownSize) ||
            !validBytes(html, htmlSize) ||
            !validBytes(uriList, uriListSize)) {
            return statusCode(ClipboardStatus::failed);
        }
        if ((formats & clipboard_image_rgba8) != 0) {
#if !DAS_CLIPBOARD_IMAGE_SUPPORTED
            return statusCode(ClipboardStatus::unsupported);
#else
            if (imageWidth <= 0 || imageHeight <= 0 ||
                static_cast<int64_t>(imageWidth) * 4 > INT32_MAX ||
                static_cast<int64_t>(imageStride) < static_cast<int64_t>(imageWidth) * 4 ||
                static_cast<uint64_t>(imageStride) * static_cast<uint64_t>(imageHeight) != imageRgba8.size) {
                return statusCode(ClipboardStatus::invalid_image);
            }
#endif
        }
        if (!nativeClipboardAvailable()) return statusCode(unavailableStatus());
        clip::lock lock;
        ClipboardStatus status = lockClipboard(lock);
        if (status != ClipboardStatus::ok) return statusCode(status);
        if (!lock.clear()) return statusCode(ClipboardStatus::failed);

        bool success = true;
#if defined(_WIN32) && DAS_CLIPBOARD_IMAGE_SUPPORTED
        if ((formats & clipboard_image_rgba8) != 0) {
            success = setWindowsImage(imageRgba8, imageWidth, imageHeight, imageStride) && success;
        }
#elif defined(__APPLE__) && DAS_CLIPBOARD_IMAGE_SUPPORTED
        if ((formats & clipboard_image_rgba8) != 0) {
            success = setAppleImage(lock, imageRgba8, imageWidth, imageHeight, imageStride) && success;
        }
#endif
        if ((formats & clipboard_html) != 0) {
#if defined(_WIN32)
            const std::string encoded = makeCfHtml(html, static_cast<size_t>(htmlSize));
            success = !encoded.empty() &&
                      setRegisteredData(lock, htmlFormat(), encoded.data(), encoded.size()) && success;
#else
            success = setRegisteredData(lock, htmlFormat(), html, static_cast<size_t>(htmlSize)) && success;
#endif
        }
        if ((formats & clipboard_markdown) != 0) {
            success = setRegisteredData(lock, markdownFormat(), markdown,
                                        static_cast<size_t>(markdownSize)) && success;
        }
        if ((formats & clipboard_uri_list) != 0) {
            success = setRegisteredData(lock, uriListFormat(), uriList,
                                        static_cast<size_t>(uriListSize)) && success;
        }
        if ((formats & clipboard_plain_text) != 0) {
            success = setClipboardTextLocked(lock, plainText,
                                             static_cast<size_t>(plainTextSize)) && success;
        }
        return statusCode(success ? ClipboardStatus::ok : ClipboardStatus::failed);
    }

    int32_t builtin_clipboard_get_content(
        int32_t & formats,
        char * & plainText, char * & markdown, char * & html, char * & uriList,
        TArray<uint8_t> & imageRgba8,
        int32_t & imageWidth, int32_t & imageHeight, int32_t & imageStride,
        Context * context, LineInfoArg * at) {
        formats = 0;
        imageWidth = imageHeight = imageStride = 0;
        if (!nativeClipboardAvailable()) return statusCode(unavailableStatus());
        clip::lock lock;
        ClipboardStatus status = lockClipboard(lock);
        if (status != ClipboardStatus::ok) return statusCode(status);

        std::string plainValue, markdownValue, htmlValue, uriValue;
        std::vector<uint8_t> imageValue;
        if (getClipboardTextLocked(lock, plainValue) == ClipboardStatus::ok) {
            formats |= clipboard_plain_text;
        }
        if (getRegisteredData(lock, markdownFormat(), markdownValue)) {
            formats |= clipboard_markdown;
        }
        std::string nativeHtml;
        if (getRegisteredData(lock, htmlFormat(), nativeHtml)) {
#if defined(_WIN32)
            htmlValue = extractCfHtmlFragment(nativeHtml);
#else
            htmlValue = std::move(nativeHtml);
#endif
            formats |= clipboard_html;
        }
        if (getRegisteredData(lock, uriListFormat(), uriValue)) {
            formats |= clipboard_uri_list;
        }
#if defined(_WIN32) && DAS_CLIPBOARD_IMAGE_SUPPORTED
        if (getWindowsImage(imageValue, imageWidth, imageHeight, imageStride)) {
            formats |= clipboard_image_rgba8;
        }
#elif defined(__APPLE__) && DAS_CLIPBOARD_IMAGE_SUPPORTED
        if (getAppleImage(lock, imageValue, imageWidth, imageHeight, imageStride)) {
            formats |= clipboard_image_rgba8;
        }
#endif
        if (formats == 0) return statusCode(ClipboardStatus::no_content);

        plainText = context->allocateString(plainValue.data(), plainValue.size(), at);
        markdown = context->allocateString(markdownValue.data(), markdownValue.size(), at);
        html = context->allocateString(htmlValue.data(), htmlValue.size(), at);
        uriList = context->allocateString(uriValue.data(), uriValue.size(), at);
        array_resize(*context, imageRgba8, static_cast<uint32_t>(imageValue.size()),
                     sizeof(uint8_t), false, at);
        if (!imageValue.empty()) {
            std::memcpy(imageRgba8.data, imageValue.data(), imageValue.size());
        }
        return statusCode(ClipboardStatus::ok);
    }

    class Module_Clipboard final : public Module {
    public:
        Module_Clipboard() : Module("clipboard_core") {
            ModuleLibrary lib(this);
            lib.addBuiltInModule();
            addBuiltinDependency(lib, Module::require("strings"));

            clip::set_error_handler(ignoreClipError);

            addExtern<DAS_BIND_FUN(builtin_clipboard_capabilities)>(*this, lib,
                "_clipboard_capabilities", SideEffects::accessExternal,
                "builtin_clipboard_capabilities");
            addExtern<DAS_BIND_FUN(builtin_clipboard_set_text)>(*this, lib,
                "_clipboard_set_text", SideEffects::modifyExternal,
                "builtin_clipboard_set_text")
                    ->args({"text", "text_size"});
            addExtern<DAS_BIND_FUN(builtin_clipboard_get_text)>(*this, lib,
                "_clipboard_get_text", SideEffects::modifyArgumentAndExternal,
                "builtin_clipboard_get_text")
                    ->args({"status", "context", "at"});
            addExtern<DAS_BIND_FUN(builtin_clipboard_clear)>(*this, lib,
                "_clipboard_clear", SideEffects::modifyExternal,
                "builtin_clipboard_clear");
            addExtern<DAS_BIND_FUN(builtin_clipboard_set_content)>(*this, lib,
                "_clipboard_set_content", SideEffects::modifyExternal,
                "builtin_clipboard_set_content")
                    ->args({"formats",
                            "plain_text", "plain_text_size",
                            "markdown", "markdown_size",
                            "html", "html_size",
                            "uri_list", "uri_list_size",
                            "image_rgba8", "image_width", "image_height", "image_stride"});
            addExtern<DAS_BIND_FUN(builtin_clipboard_get_content)>(*this, lib,
                "_clipboard_get_content", SideEffects::modifyArgumentAndExternal,
                "builtin_clipboard_get_content")
                    ->args({"formats", "plain_text", "markdown", "html", "uri_list",
                            "image_rgba8", "image_width", "image_height", "image_stride",
                            "context", "at"});

            verifyBuiltinNames(uint32_t(VerifyBuiltinFlags::verifyAll));
            verifyAotReady();
        }

        ModuleAotType aotRequire(TextWriter & tw) const override {
            tw << "#include \"../modules/dasClipboard/src/aot_builtin_clipboard.h\"\n";
            return ModuleAotType::cpp;
        }
    };

    REGISTER_DYN_MODULE(Module_Clipboard, Module_Clipboard);

}

REGISTER_MODULE_IN_NAMESPACE(Module_Clipboard, das);
