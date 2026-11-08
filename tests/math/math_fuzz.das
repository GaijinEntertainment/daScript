options gen2

require dastest/testing_boost
require daslib/faker
require daslib/fuzzer
require math

[test(description = "matrix multiply, equality and inequality survive fuzzed inputs")]
def test_matrix_op(t : T?) {
    t |> run("fuzz *") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let m1 = fake |> random_float3x3
            let m2 = fake |> random_float3x3
            t |> accept(m1 * m2)
        }
        fuzz() {
            let m1 = fake |> random_float3x4
            let m2 = fake |> random_float3x4
            t |> accept(m1 * m2)
        }
        fuzz() {
            let m1 = fake |> random_float4x4
            let m2 = fake |> random_float4x4
            t |> accept(m1 * m2)
        }
    }

    t |> run("fuzz ==") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let m1 = fake |> random_float3x3
            let m2 = fake |> random_float3x3
            t |> accept(m1 == m2)
        }
        fuzz() {
            let m1 = fake |> random_float3x4
            let m2 = fake |> random_float3x4
            t |> accept(m1 == m2)
        }
        fuzz() {
            let m1 = fake |> random_float4x4
            let m2 = fake |> random_float4x4
            t |> accept(m1 == m2)
        }
    }

    t |> run("fuzz !=") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let m1 = fake |> random_float3x3
            let m2 = fake |> random_float3x3
            t |> accept(m1 != m2)
        }
        fuzz() {
            let m1 = fake |> random_float3x4
            let m2 = fake |> random_float3x4
            t |> accept(m1 != m2)
        }
        fuzz() {
            let m1 = fake |> random_float4x4
            let m2 = fake |> random_float4x4
            t |> accept(m1 != m2)
        }
    }
}

[test(description = "matrix translation, transpose, projection, look_at, compose, decompose, inverse and determinant survive fuzzed inputs")]
def test_matrix_manip(t : T?) { // nolint:STYLE038 one flat fuzz arm per math entry point under test
    t |> run("fuzz translation") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(translation(fake |> random_float3))
        }
    }

    t |> run("fuzz transpose") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(transpose(fake |> random_float4x4))
        }
    }

    t |> run("fuzz persp_forward") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(persp_forward(fake |> random_float, fake |> random_float, fake |> random_float, fake |> random_float))
        }
    }

    t |> run("fuzz persp_reverse") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(persp_reverse(fake |> random_float, fake |> random_float, fake |> random_float, fake |> random_float))
        }
    }

    t |> run("fuzz look_at") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(look_at(fake |> random_float3, fake |> random_float3, fake |> random_float3))
        }
    }

    t |> run("fuzz compose") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(compose(fake |> random_float3, fake |> random_float4, fake |> random_float3))
        }
    }

    t |> run("fuzz decompose") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            var pos, scale : float3
            var rot : float4
            decompose(fake |> random_float4x4, pos, rot, scale)
        }
    }

    t |> run("fuzz inverse") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(inverse(fake |> random_float3x4))
        }
        fuzz() {
            t |> accept(inverse(fake |> random_float4x4))
        }
    }

    t |> run("fuzz determinant") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(determinant(fake |> random_float3x3))
        }
        fuzz() {
            t |> accept(determinant(fake |> random_float3x4))
        }
        fuzz() {
            t |> accept(determinant(fake |> random_float4x4))
        }
    }

    t |> run("fuzz orthonormal_inverse") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(orthonormal_inverse(fake |> random_float3x3))
        }
        fuzz() {
            t |> accept(orthonormal_inverse(fake |> random_float3x4))
        }
    }
}

[test(description = "pack_float_to_byte and unpack_byte_to_float survive fuzzed inputs")]
def test_pack_unpack(t : T?) {
    t |> run("fuzz pack_float_to_byte") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(pack_float_to_byte(fake |> random_float4))
        }
        fuzz() {
            t |> accept(unpack_byte_to_float(fake |> random_uint))
        }
    }
}

[test(description = "the quaternion constructors and operations survive fuzzed inputs")]
def test_quaternions(t : T?) {
    t |> run("fuzz quat_from_unit_arc") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(quat_from_unit_arc(fake |> random_float3, fake |> random_float3))
        }
    }

    t |> run("fuzz quat_from_unit_vec_ang") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(quat_from_unit_vec_ang(fake |> random_float3, fake |> random_float))
        }
    }

    t |> run("fuzz quat_from_float3x4") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(quat(fake |> random_float3x4))
        }
    }

    t |> run("fuzz quat_mul") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(quat_mul(fake |> random_float4, fake |> random_float4))
        }
    }

    t |> run("fuzz quat_mul_vec") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(quat_mul_vec(fake |> random_float4, fake |> random_float3))
        }
    }

    t |> run("fuzz quat_conjugate") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            t |> accept(quat_conjugate(fake |> random_float4))
        }
    }
}
