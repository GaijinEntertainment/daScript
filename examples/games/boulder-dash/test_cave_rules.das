options gen2

require dastest/testing_boost public

require cave
require cave_gen
require daslib/random

def private load(t : T?; ascii : string) : Cave {
    var c <- cave_from_ascii(ascii)
    t |> success(c.cols > 1 && c.rows > 1, "fixture parsed")
    return <- c
}

def private probe_cave(probe : Tile; start, step_dir : int2) : Cave {
    var c <- make_cave(7, 5)
    for (y in range(c.rows)) {
        for (x in range(c.cols)) {
            cave_set(c, x, y, Tile.dirt)
        }
    }
    cave_set(c, start.x, start.y, Tile.player)
    find_rockford(c)
    cave_set(c, start.x + step_dir.x, start.y + step_dir.y, probe)
    cave_set_input(c, step_dir, false)
    return <- c
}

[test]
def test_magic_wall_falling_gate(t : T?) {
    t |> run("a boulder at rest on a magic wall stays put") @(tt : T?) {
        var c <- load(tt, ".....\n..o..\n..M..\n.....\n.....")
        cave_tick(c)
        tt |> equal(int(cave_at(c, 2, 1)), int(Tile.boulder), "resting boulder survives the tick")
        tt |> equal(int(cave_at(c, 2, 3)), int(Tile.empty), "nothing emerged below the wall")
        tt |> success(!c.magic_convert, "no conversion event fired")
        delete c
    }
    t |> run("a FALLING boulder through a magic wall becomes a diamond") @(tt : T?) {
        var c <- load(tt, ".....\n..o..\n.....\n..M..\n.....\n.....")
        for (_ in range(4)) {
            cave_tick(c)
        }
        tt |> equal(int(cave_at(c, 2, 5)), int(Tile.diamond), "converted object settled below the wall")
        tt |> equal(int(cave_at(c, 2, 1)), int(Tile.empty), "source cell vacated")
        delete c
    }
    t |> run("a falling boulder onto a SPENT magic wall settles on top") @(tt : T?) {
        var c <- load(tt, ".....\n..o..\n.....\n..M..\n.....\n.....")
        c.magic_spent = true
        for (_ in range(4)) {
            cave_tick(c)
        }
        tt |> equal(int(cave_at(c, 2, 2)), int(Tile.boulder), "settled on the spent wall")
        tt |> equal(int(cave_at(c, 2, 4)), int(Tile.empty), "nothing emerged")
        delete c
    }
}

[test]
def test_tile_enum_coupling(t : T?) {
    t |> run("TILE_CH covers the enum one glyph per member") @(tt : T?) {
        tt |> equal(typeinfo dim(TILE_CH), TILE_N, "TILE_CH entry count matches TILE_N")
        for (i in range(TILE_N)) {
            let tl = tile_from_int(i)
            tt |> equal(int(char_to_tile(tile_to_char(tl))), i, "glyph round-trip for member {i}")
        }
    }
    t |> run("family range tests match exactly their members") @(tt : T?) {
        var flies = 0
        var butters = 0
        var explodes = 0
        for (i in range(TILE_N)) {
            let tl = tile_from_int(i)
            if (is_firefly(tl)) {
                flies++
            }
            if (is_butterfly(tl)) {
                butters++
            }
            if (is_explode(tl)) {
                explodes++
            }
        }
        tt |> equal(flies, 4, "four firefly headings")
        tt |> equal(butters, 4, "four butterfly headings")
        tt |> equal(explodes, 6, "six explosion stages")
        tt |> success(is_firefly(Tile.firefly_u) && is_firefly(Tile.firefly_l), "firefly run endpoints")
        tt |> success(is_butterfly(Tile.butterfly_u) && is_butterfly(Tile.butterfly_l), "butterfly run endpoints")
        tt |> success(is_explode(Tile.explode0) && is_explode(Tile.dexplode2), "explosion run endpoints")
    }
    t |> run("insect tiles encode heading in DIR order") @(tt : T?) {
        for (d in range(4)) {
            tt |> equal(insect_dir(insect_tile(true, d)), d, "firefly heading {d} round-trips")
            tt |> equal(insect_dir(insect_tile(false, d)), d, "butterfly heading {d} round-trips")
        }
        tt |> equal(int(insect_tile(true, 0)), int(Tile.firefly_u), "heading 0 is up")
        tt |> equal(int(insect_tile(true, 1)), int(Tile.firefly_r), "heading 1 is right")
        tt |> equal(int(insect_tile(true, 2)), int(Tile.firefly_d), "heading 2 is down")
        tt |> equal(int(insect_tile(true, 3)), int(Tile.firefly_l), "heading 3 is left")
        tt |> equal(DIR_X[0] * 10 + DIR_Y[0], -1, "up is (0,-1)")
        tt |> equal(DIR_X[1] * 10 + DIR_Y[1], 10, "right is (1,0)")
        tt |> equal(DIR_X[2] * 10 + DIR_Y[2], 1, "down is (0,1)")
        tt |> equal(DIR_X[3] * 10 + DIR_Y[3], -10, "left is (-1,0)")
    }
}

[test]
def test_peek_step_parity(t : T?) {
    t |> run("a predicted step always happens — over-prediction is the snap-back defect") @(tt : T?) {
        let start = int2(3, 2)
        for (i in range(TILE_N)) {
            let probe = tile_from_int(i)
            if (probe == Tile.player) {
                continue
            }
            var c <- probe_cave(probe, start, int2(-1, 0))
            let predicted = cave_peek_step(c, c.wish_dir)
            cave_tick(c)
            if (predicted != start) {
                tt |> equal(c.rockford, predicted, "tile {i}: a promised step must land where promised")
            }
            delete c
        }
    }
    t |> run("under-prediction is limited to tiles the tick itself transforms") @(tt : T?) {
        let start = int2(3, 2)
        for (i in range(TILE_N)) {
            let probe = tile_from_int(i)
            if (probe == Tile.player || probe == Tile.explode2 || probe == Tile.dexplode2) {
                continue
            }
            var c <- probe_cave(probe, start, int2(-1, 0))
            let predicted = cave_peek_step(c, c.wish_dir)
            cave_tick(c)
            if (predicted == start) {
                tt |> equal(c.rockford, start, "tile {i}: an unpromised step must not happen")
            }
            delete c
        }
    }
}

[test]
def test_ascii_cave_exit_contract(t : T?) {
    t |> run("a raw ascii cave can never open its exit — why the loader must finish it") @(tt : T?) {
        var c <- load(tt, "R.*.*\n:::::\n....E")
        tt |> equal(c.diamonds_need, 0, "raw load leaves no quota")
        c.diamonds_got = 5
        cave_tick(c)
        tt |> equal(int(cave_at(c, 4, 2)), int(Tile.exit_closed), "exit stays closed without a quota")
        delete c
    }
    t |> run("finish_cave on an ascii cave arms the quota and the exit opens") @(tt : T?) {
        var c <- load(tt, "R.*.*\n:::::\n....E")
        finish_cave(c, random_seed(1), 0)
        tt |> success(c.diamonds_need >= 1, "quota derived from the gems present")
        c.diamonds_got = c.diamonds_need
        cave_tick(c)
        tt |> equal(int(cave_at(c, 4, 2)), int(Tile.exit_open), "exit opened at quota")
        tt |> success(c.exit_opened, "exit_opened event fired")
        delete c
    }
}
