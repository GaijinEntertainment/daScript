options gen2
require dastest/testing_boost public
require crypto
require daslib/base64

def private decoded(value : string) : array<uint8> {
    var result : array<uint8>
    let count = base64_decode(value, result)
    resize(result, count)
    return <- result
}

[test]
def crypto_hmac_known_vectors(t : T?) {
    let key <- decoded("CwsLCwsLCwsLCwsLCwsLCwsLCws=")
    let message <- decoded("SGkgVGhlcmU=")
    var digest : array<uint8>
    t |> success(crypto_hmac_sha256(key, message, digest))
    t |> equal(base64_encode(digest), "sDRMYdjbOFNcqK/OrwvxK4gdwgDJgz2nJuk3bC4yz/c=")
    t |> success(crypto_hmac_sha1(key, message, digest))
    t |> equal(base64_encode(digest), "thcxhlUFcmTii8C2+zeMjvFGvgA=")
}

[test]
def crypto_random_and_constant_time_equality(t : T?) {
    var bytes : array<uint8>
    t |> success(crypto_random_bytes(32, bytes))
    t |> equal(length(bytes), 32)
    var copy <- clone(bytes)
    t |> success(crypto_equal(bytes, copy))
    copy[31] = uint8(uint(copy[31]) ^ 1u)
    t |> success(!crypto_equal(bytes, copy))
    resize(copy, 31)
    t |> success(!crypto_equal(bytes, copy))
    t |> success(!crypto_random_bytes(-1, bytes))
    t |> equal(length(bytes), 0)
    t |> success(!crypto_random_bytes(4097, bytes))
    t |> success(crypto_random_bytes(0, bytes))
    t |> success(crypto_equal(bytes, bytes))
}

[test]
def crypto_authenticated_encryption_rejects_tampering(t : T?) {
    var key : array<uint8>; resize(key, 32)
    var nonce : array<uint8>; resize(nonce, 12)
    let plain <- decoded("VW5pY29kZTog0J/RgNC40LLQtdGC")
    let aad <- decoded("Y29udGV4dA==")
    var ciphertext, restored : array<uint8>
    t |> success(crypto_aes256_gcm_seal(key, nonce, plain, aad, ciphertext))
    t |> equal(length(ciphertext), length(plain) + 16)
    t |> success(crypto_aes256_gcm_open(key, nonce, ciphertext, aad, restored))
    t |> success(crypto_equal(plain, restored))
    ciphertext[length(ciphertext) - 1] = uint8(uint(ciphertext[length(ciphertext) - 1]) ^ 1u)
    t |> success(!crypto_aes256_gcm_open(key, nonce, ciphertext, aad, restored))
    t |> equal(length(restored), 0, "unauthenticated plaintext is never returned")
    resize(key, 31)
    t |> success(!crypto_aes256_gcm_seal(key, nonce, plain, aad, ciphertext))
    t |> equal(length(ciphertext), 0)
}

[test]
def crypto_rsa_sha256_public_verification(t : T?) {
    var n <- decoded("vJiiP9ba2Hd9J+hcryDEWM0ZrtHTaFADicLQ3WHBjWbaHWvnte5ScqDoHF49i0UWinvRwlIoLvuU7UB+bUIb872hXyf3tGAgGPVVmaZPOnWn4n9fSvWckeyQ+9M9NrEPlAK2VNZy8XHLdsA27HifbLXTdR83Vogrtcx+ZdM/G6jZH3aOZkN22x/XZHkxryZcl/XT8num6t58A3LfZqkGecySeAiYHb+rYYivV76Uih9ICF2bMUTkJlhNcBvTJ/Yw5nFNVC8Qn1+85coNstBG49FtX6vuH4b8J/XMKJBAGwaD5rMs7f4GKTbOSNJVL3AkT0Q28QKrs25COO3IDm2wUQ==")
    let e <- decoded("AQAB")
    var message <- decoded("UlNBIFNIQTI1NiB2ZXJpZmljYXRpb24gZml4dHVyZQo=")
    var signature <- decoded("Bb9AYOgE64QvggdH7/Zcrzad5TCMozAWtvU3GhFLc5iu/E/YaSigrm9crG+NSa/UF+pY79sxG7ndM+WJ3cxAdnrkp7trpkDbQK+u96MIPAEgUMdlDV4Cwz+jkvTnWlInVnwtW9tKN5kYDPde5AuxY/88V4jRSDEy0C363iOjlSDOud+bHYVqRKMcL+29iEbGYZR/+m/QGLWMZBnH2v5Ux/g9U5Hfz6HoHt3XpcluYQFWGgwWhFn3gfJVimO+q5QeJXOgte2bSY+Y73eKLKvLpYq/BrIiZPTDFsGkQbpFDqn6K4vZfql8x/pspBSXuDJrstYxQzxKESAHTVQNP/xRUw==")
    t |> success(crypto_verify_rsa_sha256(n, e, message, signature))
    message[0] = uint8(uint(message[0]) ^ 1u)
    t |> success(!crypto_verify_rsa_sha256(n, e, message, signature))
    message[0] = uint8(uint(message[0]) ^ 1u)
    signature[0] = uint8(uint(signature[0]) ^ 1u)
    t |> success(!crypto_verify_rsa_sha256(n, e, message, signature))
    signature[0] = uint8(uint(signature[0]) ^ 1u)
    n[0] = uint8(uint(n[0]) ^ 2u)
    t |> success(!crypto_verify_rsa_sha256(n, e, message, signature))
    resize(n, 128)
    t |> success(!crypto_verify_rsa_sha256(n, e, message, signature))
    let empty : array<uint8>
    t |> success(!crypto_verify_rsa_sha256(empty, empty, empty, empty))
}

[test]
def crypto_gcm_known_vector_and_empty_plaintext(t : T?) {
    // AES-256-GCM zero-key/zero-IV vectors (GCM specification cases 13 and 14).
    var key, nonce, plain, aad, ciphertext, restored : array<uint8>
    resize(key, 32); resize(nonce, 12); resize(plain, 16)
    t |> success(crypto_aes256_gcm_seal(key, nonce, plain, aad, ciphertext))
    t |> equal(base64_encode(ciphertext), "zqdAPU1ga24HTsXTuvOdGNDRyKeZmWvwJluYtdSKuRk=")
    t |> success(crypto_aes256_gcm_open(key, nonce, ciphertext, aad, restored))
    t |> success(crypto_equal(plain, restored))
    clear(plain)
    t |> success(crypto_aes256_gcm_seal(key, nonce, plain, aad, ciphertext))
    t |> equal(base64_encode(ciphertext), "Uw+K+8dFNrmpY7TxxMtziw==")
    t |> success(crypto_aes256_gcm_open(key, nonce, ciphertext, aad, restored))
    t |> equal(length(restored), 0)
    aad |> push(1u8)
    t |> success(!crypto_aes256_gcm_open(key, nonce, ciphertext, aad, restored))
    t |> equal(length(restored), 0)
    clear(aad); resize(nonce, 11)
    t |> success(!crypto_aes256_gcm_open(key, nonce, ciphertext, aad, restored))
    resize(nonce, 12); resize(ciphertext, 15)
    t |> success(!crypto_aes256_gcm_open(key, nonce, ciphertext, aad, restored))
}

[test]
def crypto_outputs_may_alias_inputs(t : T?) {
    var key <- decoded("CwsLCwsLCwsLCwsLCwsLCwsLCws=")
    let message <- decoded("SGkgVGhlcmU=")
    t |> success(crypto_hmac_sha256(key, message, key))
    t |> equal(base64_encode(key), "sDRMYdjbOFNcqK/OrwvxK4gdwgDJgz2nJuk3bC4yz/c=")
    resize(key, 32)
    var nonce : array<uint8>; resize(nonce, 12)
    var content <- decoded("YWJj")
    let aad : array<uint8>
    t |> success(crypto_aes256_gcm_seal(key, nonce, content, aad, content))
    t |> equal(length(content), 19)
    t |> success(crypto_aes256_gcm_open(key, nonce, content, aad, content))
    t |> equal(base64_encode(content), "YWJj")
    t |> success(!crypto_aes256_gcm_open(key, nonce, content, aad, content))
    t |> equal(length(content), 0)
}

[test]
def crypto_byte_limits_fail_without_stale_output(t : T?) {
    var oversized : array<uint8>; resize(oversized, 16 * 1024 * 1024 + 1)
    let short <- decoded("YWJj")
    var output <- decoded("c3RhbGU=")
    t |> success(!crypto_hmac_sha256(short, oversized, output))
    t |> equal(length(output), 0)
    t |> success(!crypto_equal(oversized, oversized))
    var key : array<uint8>; resize(key, 32)
    var nonce : array<uint8>; resize(nonce, 12)
    t |> success(!crypto_aes256_gcm_seal(key, nonce, short, oversized, output))
    t |> equal(length(output), 0)
    delete oversized
}
