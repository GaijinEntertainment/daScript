options gen2

require dastest/testing_boost public
require daslib/sha_256
require strings

[test]
def test_nist_vectors(t : T?) {
    t |> run("empty string") @(t : T?) {
        t |> equal(sha256_hex(""), "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855")
    }
    t |> run("abc") @(t : T?) {
        t |> equal(sha256_hex("abc"), "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad")
    }
    t |> run("two-block 448-bit message") @(t : T?) {
        t |> equal(sha256_hex("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"),
            "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1")
    }
    t |> run("four-block 896-bit message") @(t : T?) {
        t |> equal(sha256_hex("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"),
            "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1")
    }
    t |> run("one million a") @(t : T?) {
        var st = sha256_init()
        for (_i in range(1000000)) {
            sha256_update(st, uint8('a'))
        }
        t |> equal(sha256_hex_digest(sha256_final(st)),
            "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0")
    }
}

[test]
def test_padding_boundaries(t : T?) {
    // 55/56/63/64 bytes straddle the padding split (0x80 + length must fit or spill)
    t |> run("55 56 63 64 byte inputs agree between overloads") @(t : T?) {
        for (n in fixed_array(55, 56, 63, 64, 65, 119, 120)) {
            var bytes : array<uint8>
            var str = ""
            for (_i in range(n)) {
                bytes |> push(uint8('x'))
                str += "x"
            }
            t |> equal(sha256_hex(bytes), sha256_hex(str))
            delete bytes
        }
    }
}

[test]
def test_streaming_equals_oneshot(t : T?) {
    t |> run("split updates match one-shot") @(t : T?) {
        let msg = "the quick brown fox jumps over the lazy dog"
        var st = sha256_init()
        peek_data(msg) $(bytes) {
            for (b in bytes) {
                sha256_update(st, b)
            }
        }
        t |> equal(sha256_hex_digest(sha256_final(st)), sha256_hex(msg))
    }
}
