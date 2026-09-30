#pragma once
#include "daScript/misc/platform.h"
#include "daScript/simulate/simulate.h"

namespace das {
    DAS_MOD_API bool crypto_random_bytes(int count, TArray<uint8_t> & output, Context * context, LineInfoArg * at);
    DAS_MOD_API bool crypto_equal(const TArray<uint8_t> & a, const TArray<uint8_t> & b);
    DAS_MOD_API bool crypto_hmac_sha256(const TArray<uint8_t> & key, const TArray<uint8_t> & message, TArray<uint8_t> & output, Context * context, LineInfoArg * at);
    DAS_MOD_API bool crypto_hmac_sha1(const TArray<uint8_t> & key, const TArray<uint8_t> & message, TArray<uint8_t> & output, Context * context, LineInfoArg * at);
    DAS_MOD_API bool crypto_aes256_gcm_seal(const TArray<uint8_t> & key, const TArray<uint8_t> & nonce, const TArray<uint8_t> & plain, const TArray<uint8_t> & aad, TArray<uint8_t> & output, Context * context, LineInfoArg * at);
    DAS_MOD_API bool crypto_aes256_gcm_open(const TArray<uint8_t> & key, const TArray<uint8_t> & nonce, const TArray<uint8_t> & ciphertext, const TArray<uint8_t> & aad, TArray<uint8_t> & output, Context * context, LineInfoArg * at);
    DAS_MOD_API bool crypto_verify_rsa_sha256(const TArray<uint8_t> & modulus, const TArray<uint8_t> & exponent, const TArray<uint8_t> & message, const TArray<uint8_t> & signature);
}
