#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "dasCrypto.h"
#include <openssl/bn.h>
#include <openssl/core_names.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/param_build.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <memory>
#include <vector>

namespace das {
namespace {
    constexpr uint32_t max_payload_bytes = 16u * 1024u * 1024u;
    const uint8_t empty_byte = 0;
    const uint8_t * bytes(const TArray<uint8_t> & value) {
        return value.size ? reinterpret_cast<const uint8_t *>(value.data) : &empty_byte;
    }
    bool input_fits(const TArray<uint8_t> & value) { return value.size <= max_payload_bytes; }
    void clear_output(TArray<uint8_t> & output, Context * context, LineInfoArg * at) {
        if (output.size) OPENSSL_cleanse(output.data, output.size);
        builtin_array_resize(output, 0, sizeof(uint8_t), context, at);
    }
    bool publish(TArray<uint8_t> & output, const uint8_t * data, uint32_t size, Context * context, LineInfoArg * at) {
        clear_output(output, context, at);
        builtin_array_resize(output, size, sizeof(uint8_t), context, at);
        if (size) memcpy(output.data, data, size);
        return true;
    }
    struct SecretBytes {
        std::vector<uint8_t> data;
        explicit SecretBytes(size_t size) : data(size) {}
        ~SecretBytes() { if (!data.empty()) OPENSSL_cleanse(data.data(), data.size()); }
    };
    bool hmac(const char * digest, const TArray<uint8_t> & key, const TArray<uint8_t> & message, TArray<uint8_t> & output, Context * context, LineInfoArg * at) {
        if (!input_fits(key) || !input_fits(message)) { clear_output(output, context, at); return false; }
        uint8_t buffer[EVP_MAX_MD_SIZE];
        size_t size = 0;
        const bool ok = EVP_Q_mac(nullptr, "HMAC", nullptr, digest, nullptr, bytes(key), key.size,
            bytes(message), message.size, buffer, sizeof(buffer), &size) != nullptr;
        if (ok) publish(output, buffer, uint32_t(size), context, at);
        else clear_output(output, context, at);
        OPENSSL_cleanse(buffer, sizeof(buffer));
        return ok;
    }
    using Cipher = std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)>;
}

bool crypto_random_bytes(int count, TArray<uint8_t> & output, Context * context, LineInfoArg * at) {
    if (count < 0 || count > 4096) { clear_output(output, context, at); return false; }
    SecretBytes result{size_t(count)};
    if (count && RAND_bytes(result.data.data(), count) != 1) { clear_output(output, context, at); return false; }
    return publish(output, result.data.data(), uint32_t(count), context, at);
}
bool crypto_equal(const TArray<uint8_t> & a, const TArray<uint8_t> & b) {
    return input_fits(a) && input_fits(b) && a.size == b.size && (!a.size || CRYPTO_memcmp(a.data, b.data, a.size) == 0);
}
bool crypto_hmac_sha256(const TArray<uint8_t> & key, const TArray<uint8_t> & message, TArray<uint8_t> & output, Context * context, LineInfoArg * at) {
    return hmac("SHA256", key, message, output, context, at);
}
bool crypto_hmac_sha1(const TArray<uint8_t> & key, const TArray<uint8_t> & message, TArray<uint8_t> & output, Context * context, LineInfoArg * at) {
    return hmac("SHA1", key, message, output, context, at);
}
bool crypto_aes256_gcm_seal(const TArray<uint8_t> & key, const TArray<uint8_t> & nonce, const TArray<uint8_t> & plain, const TArray<uint8_t> & aad, TArray<uint8_t> & output, Context * context, LineInfoArg * at) {
    if (key.size != 32 || nonce.size != 12 || !input_fits(plain) || !input_fits(aad)) { clear_output(output, context, at); return false; }
    Cipher cipher(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);
    SecretBytes result(size_t(plain.size) + 16);
    int size = 0, final_size = 0;
    const bool ok = cipher && EVP_EncryptInit_ex(cipher.get(), EVP_aes_256_gcm(), nullptr, bytes(key), bytes(nonce)) == 1
        && EVP_EncryptUpdate(cipher.get(), nullptr, &size, bytes(aad), int(aad.size)) == 1
        && EVP_EncryptUpdate(cipher.get(), result.data.data(), &size, bytes(plain), int(plain.size)) == 1
        && EVP_EncryptFinal_ex(cipher.get(), result.data.data() + size, &final_size) == 1
        && uint32_t(size + final_size) == plain.size
        && EVP_CIPHER_CTX_ctrl(cipher.get(), EVP_CTRL_GCM_GET_TAG, 16, result.data.data() + plain.size) == 1;
    if (!ok) { clear_output(output, context, at); return false; }
    return publish(output, result.data.data(), uint32_t(result.data.size()), context, at);
}
bool crypto_aes256_gcm_open(const TArray<uint8_t> & key, const TArray<uint8_t> & nonce, const TArray<uint8_t> & ciphertext, const TArray<uint8_t> & aad, TArray<uint8_t> & output, Context * context, LineInfoArg * at) {
    if (key.size != 32 || nonce.size != 12 || ciphertext.size < 16 || ciphertext.size > max_payload_bytes + 16 || !input_fits(aad)) { clear_output(output, context, at); return false; }
    Cipher cipher(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);
    const uint32_t plain_size = ciphertext.size - 16;
    SecretBytes result(size_t(plain_size) + 16);
    int size = 0, final_size = 0;
    const bool ok = cipher && EVP_DecryptInit_ex(cipher.get(), EVP_aes_256_gcm(), nullptr, bytes(key), bytes(nonce)) == 1
        && EVP_DecryptUpdate(cipher.get(), nullptr, &size, bytes(aad), int(aad.size)) == 1
        && EVP_DecryptUpdate(cipher.get(), result.data.data(), &size, bytes(ciphertext), int(plain_size)) == 1
        && EVP_CIPHER_CTX_ctrl(cipher.get(), EVP_CTRL_GCM_SET_TAG, 16, const_cast<uint8_t *>(bytes(ciphertext) + plain_size)) == 1
        && EVP_DecryptFinal_ex(cipher.get(), result.data.data() + size, &final_size) == 1
        && uint32_t(size + final_size) == plain_size;
    if (!ok) { clear_output(output, context, at); return false; }
    return publish(output, result.data.data(), plain_size, context, at);
}
bool crypto_verify_rsa_sha256(const TArray<uint8_t> & modulus, const TArray<uint8_t> & exponent, const TArray<uint8_t> & message, const TArray<uint8_t> & signature) {
    if (modulus.size < 256 || modulus.size > 1024 || exponent.size == 0 || exponent.size > 8 || signature.size != modulus.size || !input_fits(message)) return false;
    using Big = std::unique_ptr<BIGNUM, decltype(&BN_free)>;
    using Builder = std::unique_ptr<OSSL_PARAM_BLD, decltype(&OSSL_PARAM_BLD_free)>;
    using Params = std::unique_ptr<OSSL_PARAM, decltype(&OSSL_PARAM_free)>;
    using KeyContext = std::unique_ptr<EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free)>;
    using Key = std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)>;
    using Digest = std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)>;
    Big n(BN_bin2bn(bytes(modulus), int(modulus.size), nullptr), BN_free);
    Big e(BN_bin2bn(bytes(exponent), int(exponent.size), nullptr), BN_free);
    if (!n || !e || BN_num_bits(n.get()) < 2048 || !BN_is_odd(n.get()) || !BN_is_odd(e.get()) || BN_is_one(e.get())) return false;
    Builder builder(OSSL_PARAM_BLD_new(), OSSL_PARAM_BLD_free);
    if (!builder || !OSSL_PARAM_BLD_push_BN(builder.get(), OSSL_PKEY_PARAM_RSA_N, n.get()) || !OSSL_PARAM_BLD_push_BN(builder.get(), OSSL_PKEY_PARAM_RSA_E, e.get())) return false;
    Params params(OSSL_PARAM_BLD_to_param(builder.get()), OSSL_PARAM_free);
    KeyContext key_context(EVP_PKEY_CTX_new_from_name(nullptr, "RSA", nullptr), EVP_PKEY_CTX_free);
    EVP_PKEY * raw_key = nullptr;
    if (!params || !key_context || EVP_PKEY_fromdata_init(key_context.get()) != 1) return false;
    const int imported = EVP_PKEY_fromdata(key_context.get(), &raw_key, EVP_PKEY_PUBLIC_KEY, params.get());
    Key key(raw_key, EVP_PKEY_free);
    if (imported != 1 || !key) return false;
    Digest digest(EVP_MD_CTX_new(), EVP_MD_CTX_free);
    EVP_PKEY_CTX * verify = nullptr;
    return digest && EVP_DigestVerifyInit(digest.get(), &verify, EVP_sha256(), nullptr, key.get()) == 1
        && EVP_PKEY_CTX_set_rsa_padding(verify, RSA_PKCS1_PADDING) == 1
        && EVP_DigestVerify(digest.get(), bytes(signature), signature.size, bytes(message), message.size) == 1;
}

class Module_Crypto : public Module {
public:
    Module_Crypto() : Module("crypto") {
        ModuleLibrary lib; lib.addModule(this); lib.addBuiltInModule();
        addExtern<DAS_BIND_FUN(crypto_random_bytes)>(*this, lib, "crypto_random_bytes", SideEffects::modifyArgumentAndExternal, "crypto_random_bytes")->args({"count", "output", "", ""});
        addExtern<DAS_BIND_FUN(crypto_equal)>(*this, lib, "crypto_equal", SideEffects::none, "crypto_equal")->args({"a", "b"});
        addExtern<DAS_BIND_FUN(crypto_hmac_sha256)>(*this, lib, "crypto_hmac_sha256", SideEffects::modifyArgumentAndExternal, "crypto_hmac_sha256")->args({"key", "message", "output", "", ""});
        addExtern<DAS_BIND_FUN(crypto_hmac_sha1)>(*this, lib, "crypto_hmac_sha1", SideEffects::modifyArgumentAndExternal, "crypto_hmac_sha1")->args({"key", "message", "output", "", ""});
        addExtern<DAS_BIND_FUN(crypto_aes256_gcm_seal)>(*this, lib, "crypto_aes256_gcm_seal", SideEffects::modifyArgumentAndExternal, "crypto_aes256_gcm_seal")->args({"key", "nonce", "plain", "aad", "output", "", ""});
        addExtern<DAS_BIND_FUN(crypto_aes256_gcm_open)>(*this, lib, "crypto_aes256_gcm_open", SideEffects::modifyArgumentAndExternal, "crypto_aes256_gcm_open")->args({"key", "nonce", "ciphertext", "aad", "output", "", ""});
        addExtern<DAS_BIND_FUN(crypto_verify_rsa_sha256)>(*this, lib, "crypto_verify_rsa_sha256", SideEffects::modifyExternal, "crypto_verify_rsa_sha256")->args({"modulus", "exponent", "message", "signature"});
    }
    ModuleAotType aotRequire(TextWriter & writer) const override {
        writer << "#include \"../modules/dasCrypto/src/dasCrypto.h\"\n";
        return ModuleAotType::cpp;
    }
};
REGISTER_DYN_MODULE(Module_Crypto, Module_Crypto);
}
REGISTER_MODULE_IN_NAMESPACE(Module_Crypto, das);
