# dasCrypto

Optional native OpenSSL bindings for secure random bytes, HMAC, authenticated
AES encryption and RSA signature verification. Enable with
`-DDAS_CRYPTO_DISABLED=OFF`; OpenSSL 3.0 or newer is required. Emscripten is not
supported by this module. Import with `require crypto`.

All byte arguments use `array<uint8>`. Operations return `bool`; output-producing
operations clear their output on failure. Outputs may alias inputs: computation
finishes before publishing the output. Invalid lengths return false rather than
panicking. Allocation failures retain the runtime's fatal-error behavior.

| Function | Contract |
| --- | --- |
| `crypto_random_bytes(count, output)` | Cryptographic randomness; count 0 through 4096. |
| `crypto_equal(a, b)` | Constant-time byte comparison for equal-length inputs. Length is not hidden. |
| `crypto_hmac_sha256(key, message, output)` | 32-byte HMAC-SHA256 result. |
| `crypto_hmac_sha1(key, message, output)` | 20-byte HMAC-SHA1 result for protocols requiring it, such as TOTP. |
| `crypto_aes256_gcm_seal(key, nonce, plain, aad, output)` | 32-byte key, 12-byte nonce; output is ciphertext followed by a 16-byte authentication tag. |
| `crypto_aes256_gcm_open(key, nonce, ciphertext, aad, output)` | Same format. No unauthenticated plaintext is published. |
| `crypto_verify_rsa_sha256(modulus, exponent, message, signature)` | Unsigned big-endian RSA key components, 2048–8192-bit modulus, exponent up to 8 bytes. PKCS#1 v1.5 SHA-256 verification. Signature length equals modulus byte length. |

Byte inputs are limited to 16 MiB, except the documented fixed-size key/nonce,
RSA components and ciphertext's extra 16-byte tag. Applications should use much
smaller limits where appropriate and reject oversized network input before calling.

Never reuse an AES-GCM nonce with the same key. Callers own nonce generation,
key storage/rotation, protocol context in AAD and key lifetimes. These primitives
do not implement token parsing, identity verification, certificate validation or
a complete authentication protocol.

Run `bin/daslang dastest/dastest.das -- --test tests/crypto` with the module enabled.
The suite checks published HMAC/GCM vectors, RSA verification, tampering, bounds
and output behavior. The tests also join the enabled module's AOT suite.
