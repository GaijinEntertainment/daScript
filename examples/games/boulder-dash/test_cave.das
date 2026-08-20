options gen2
require dastest/testing_boost public
require cave

def ascii_eq(t : T?; c : Cave; want : string; msg : string) {
    let got = cave_to_ascii(c)
    t |> equal(got, want, msg)
}

def tick_n(var c : Cave; n : int) {
    for (_ in range(n)) {
        cave_tick(c)
    }
}

[test]
def test_ascii_roundtrip(t : T?) {
    let src = "XoR*\nX:.E"
    let c <- cave_from_ascii(src)
    t |> equal(c.cols, 4)
    t |> equal(c.rows, 2)
    t |> equal(c.rockford, int2(2, 0))
    ascii_eq(t, c, src, "roundtrip")
}

[test]
def test_boulder_falls_into_empty(t : T?) {
    var c <- cave_from_ascii("XoX\nX.X\nXXX")
    cave_tick(c)
    ascii_eq(t, c, "X.X\nX!X\nXXX", "one cell down as falling")
    cave_tick(c)
    ascii_eq(t, c, "X.X\nXoX\nXXX", "settles")
}

[test]
def test_stacked_boulders_one_cell_per_tick(t : T?) {
    var c <- cave_from_ascii("#####\n#o###\n#o###\n#.###\n#.###\n#####")
    cave_tick(c)
    t |> equal(cave_at(c, 1, 1), Tile.boulder, "top stays")
    t |> equal(cave_at(c, 1, 2), Tile.empty, "middle vacated")
    t |> equal(cave_at(c, 1, 3), Tile.boulder_fall, "lower fell one")
    t |> equal(cave_at(c, 1, 4), Tile.empty, "scan bit blocked a double fall")
}

[test]
def test_boulder_rests_on_dirt(t : T?) {
    var c <- cave_from_ascii("XoX\nX:X\nXXX")
    cave_tick(c)
    ascii_eq(t, c, "XoX\nX:X\nXXX", "dirt is not empty")
}

[test]
def test_boulder_rolls_left(t : T?) {
    var c <- cave_from_ascii("#####\n#.o.#\n#.o.#\n#...#\n#####")
    cave_tick(c)
    t |> equal(cave_at(c, 2, 1), Tile.empty, "rolled off")
    t |> equal(cave_at(c, 1, 2), Tile.boulder_fall, "diagonal left")
}

[test]
def test_boulder_prefers_left_over_right(t : T?) {
    var c <- cave_from_ascii("#####\n#.o.#\n#.o.#\n#...#\n#####")
    cave_tick(c)
    t |> success(cave_at(c, 3, 2) != Tile.boulder_fall, "did not roll right")
    t |> success(cave_at(c, 1, 2) == Tile.boulder_fall, "rolled left")
}

[test]
def test_falling_crushes_rockford(t : T?) {
    var c <- cave_from_ascii("XoX\nXRX\nXXX")
    cave_set(c, 1, 0, Tile.boulder_fall)
    cave_tick(c)
    t |> success(!c.alive, "crushed")
    t |> success(c.died)
    t |> success(cave_at(c, 1, 1) == Tile.boulder)
}

[test]
def test_stationary_boulder_does_not_crush(t : T?) {
    var c <- cave_from_ascii("XoX\nXRX\nXXX")
    cave_tick(c)
    t |> success(c.alive, "stationary on player is blocked, not crushing")
    t |> equal(cave_at(c, 1, 0), Tile.boulder)
    t |> equal(cave_at(c, 1, 1), Tile.player)
}

[test]
def test_collect_diamond_and_grab(t : T?) {
    t |> run("walk onto diamond") <| @(t : T?) {
        var c <- cave_from_ascii("R*..\nXXXX")
        cave_set_input(c, int2(1, 0), false)
        cave_tick(c)
        t |> equal(c.diamonds_got, 1)
        t |> equal(c.rockford, int2(1, 0))
        t |> equal(cave_at(c, 1, 0), Tile.player)
    }
    t |> run("grab dirt without moving") <| @(t : T?) {
        var c <- cave_from_ascii("R:..\nXXXX")
        cave_set_input(c, int2(1, 0), true)
        cave_tick(c)
        t |> equal(c.rockford, int2(0, 0), "stayed")
        t |> equal(cave_at(c, 1, 0), Tile.empty)
        t |> success(c.dug)
    }
    t |> run("grab diamond without moving") <| @(t : T?) {
        var c <- cave_from_ascii("R*..\nXXXX")
        cave_set_input(c, int2(1, 0), true)
        cave_tick(c)
        t |> equal(c.rockford, int2(0, 0))
        t |> equal(c.diamonds_got, 1)
        t |> equal(cave_at(c, 1, 0), Tile.empty)
    }
}

[test]
def test_diamond_fall_events(t : T?) {
    var c <- cave_from_ascii("X*X\nX.X\nXXX")
    cave_tick(c)
    t |> success(c.fell_diamond, "diamond started falling")
    t |> success(!c.landed_diamond)
    cave_tick(c)
    t |> success(c.landed_diamond, "diamond landed")
    t |> equal(cave_at(c, 1, 1), Tile.diamond)
}

[test]
def test_peek_step(t : T?) {
    t |> run("walk into dirt") <| @(t : T?) {
        let c <- cave_from_ascii("R:.\nXXX")
        t |> equal(cave_peek_step(c, int2(1, 0)), int2(1, 0))
    }
    t |> run("blocked by steel") <| @(t : T?) {
        let c <- cave_from_ascii("XR.\nXXX")
        t |> equal(cave_peek_step(c, int2(-1, 0)), int2(1, 0))
    }
    t |> run("no preview into boulder") <| @(t : T?) {
        let c <- cave_from_ascii("Ro.\nXXX")
        t |> equal(cave_peek_step(c, int2(1, 0)), int2(0, 0))
    }
}

[test]
def test_input_latch(t : T?) {
    t |> run("tap dir survives a later idle latch") <| @(t : T?) {
        var c <- cave_from_ascii("R:..\nXXXX")
        cave_latch_input(c, int2(1, 0), false)
        cave_latch_input(c, int2(0, 0), false)
        cave_tick(c)
        t |> equal(c.rockford, int2(1, 0), "queued step applied")
    }
    t |> run("later tap replaces earlier") <| @(t : T?) {
        var c <- cave_from_ascii(".R.\nXXX")
        cave_latch_input(c, int2(-1, 0), false)
        cave_latch_input(c, int2(1, 0), false)
        cave_tick(c)
        t |> equal(c.rockford, int2(2, 0), "last tap wins")
    }
    t |> run("lock keeps committed dir") <| @(t : T?) {
        var c <- cave_from_ascii("R:.\n:..\nXXX")
        cave_latch_input(c, int2(1, 0), false)
        cave_lock_input(c)
        cave_latch_input(c, int2(0, 1), false)
        cave_tick(c)
        t |> equal(c.rockford, int2(1, 0), "did not cancel")
    }
    t |> run("lock later tap applies after consume") <| @(t : T?) {
        var c <- cave_from_ascii("R:.\n:..\nXXX")
        cave_latch_input(c, int2(1, 0), false)
        cave_lock_input(c)
        cave_latch_input(c, int2(0, 1), false)
        cave_tick(c)
        cave_consume_input(c)
        cave_tick(c)
        t |> equal(c.rockford, int2(1, 1), "pending dir next tick")
    }
    t |> run("same dir while locked does not queue extra step") <| @(t : T?) {
        var c <- cave_from_ascii("R:.\nXXX")
        cave_latch_input(c, int2(1, 0), false)
        cave_lock_input(c)
        cave_latch_input(c, int2(1, 0), false)
        cave_tick(c)
        cave_consume_input(c)
        cave_tick(c)
        t |> equal(c.rockford, int2(1, 0), "one cell")
    }
    t |> run("grab while locked does not cancel walk") <| @(t : T?) {
        var c <- cave_from_ascii("R:.\nXXX")
        cave_latch_input(c, int2(1, 0), false)
        cave_lock_input(c)
        cave_latch_input(c, int2(1, 0), true)
        cave_tick(c)
        t |> equal(c.rockford, int2(1, 0), "walked")
        t |> equal(cave_at(c, 1, 0), Tile.player)
    }
    t |> run("consume without pending clears wish") <| @(t : T?) {
        var c <- cave_from_ascii("R:.\nXXX")
        cave_latch_input(c, int2(1, 0), false)
        cave_tick(c)
        cave_consume_input(c)
        cave_tick(c)
        t |> equal(c.rockford, int2(1, 0), "stopped")
    }
    t |> run("set_input unlocks so a later latch applies") <| @(t : T?) {
        var c <- cave_from_ascii("R:.\nXXX")
        cave_latch_input(c, int2(1, 0), false)
        cave_lock_input(c)
        cave_set_input(c, int2(0, 0), false)
        cave_latch_input(c, int2(1, 0), false)
        cave_tick(c)
        t |> equal(c.rockford, int2(1, 0), "unlocked latch walked")
    }
    t |> run("grab tap survives idle latch") <| @(t : T?) {
        var c <- cave_from_ascii("R:..\nXXXX")
        cave_latch_input(c, int2(1, 0), true)
        cave_latch_input(c, int2(0, 0), false)
        cave_tick(c)
        t |> equal(c.rockford, int2(0, 0), "did not walk")
        t |> success(c.dug, "grab still fired")
        t |> equal(cave_at(c, 1, 0), Tile.empty)
    }
}

[test]
def test_push(t : T?) {
    t |> run("push takes two ticks") <| @(t : T?) {
        var c <- cave_from_ascii("Ro.\nXXX")
        cave_set_input(c, int2(1, 0), false)
        cave_tick(c)
        t |> equal(c.rockford, int2(0, 0), "wind-up")
        t |> equal(cave_at(c, 1, 0), Tile.boulder)
        cave_tick(c)
        t |> equal(c.rockford, int2(1, 0))
        t |> equal(cave_at(c, 2, 0), Tile.boulder)
        t |> equal(cave_at(c, 1, 0), Tile.player)
    }
    t |> run("no push into occupied") <| @(t : T?) {
        var c <- cave_from_ascii("Ro:\nXXX")
        cave_set_input(c, int2(1, 0), false)
        tick_n(c, 3)
        t |> equal(c.rockford, int2(0, 0))
        t |> equal(cave_at(c, 1, 0), Tile.boulder)
    }
    t |> run("no vertical push") <| @(t : T?) {
        var c <- cave_from_ascii("XoX\nXRX\nXXX")
        cave_set_input(c, int2(0, -1), false)
        cave_tick(c)
        t |> equal(cave_at(c, 1, 0), Tile.boulder)
        t |> equal(c.rockford, int2(1, 1))
    }
}

[test]
def test_firefly_left_hand(t : T?) {
    var c <- cave_from_ascii("XXXX\nXn.X\nX..X\nXXXX")
    cave_tick(c)
    t |> equal(cave_at(c, 2, 1), Tile.firefly_r, "left and up blocked, turns east")
    t |> equal(cave_at(c, 1, 1), Tile.empty)
}

[test]
def test_butterfly_mirror(t : T?) {
    var c <- cave_from_ascii("XXXX\nX.AX\nX..X\nXXXX")
    cave_tick(c)
    t |> equal(cave_at(c, 1, 1), Tile.butterfly_l, "right-hand: from up, try right (east) blocked, stay/turn west into empty")
}

[test]
def test_explosion_firefly_and_butterfly(t : T?) {
    t |> run("firefly + falling boulder -> empty blast") <| @(t : T?) {
        var c <- cave_from_ascii("XXXXX\nX...X\nX.!.X\nX.n.X\nXXXXX")
        t |> equal(cave_at(c, 2, 2), Tile.boulder_fall, "falling boulder")
        t |> equal(cave_at(c, 2, 3), Tile.firefly_u, "firefly")
        cave_tick(c)
        t |> success(c.exploded)
        t |> equal(cave_to_ascii(c), "XXXXX\nX...X\nX111X\nX111X\nXXXXX", "3x3 empty blast")
        t |> equal(cave_at(c, 0, 0), Tile.steel, "steel outside blast")
        tick_n(c, 3)
        t |> equal(cave_at(c, 2, 3), Tile.empty)
    }
    t |> run("butterfly + falling boulder -> diamond blast") <| @(t : T?) {
        var c <- cave_from_ascii("XXXXX\nX...X\nX.!.X\nX.A.X\nXXXXX")
        cave_tick(c)
        t |> equal(cave_at(c, 2, 3), Tile.dexplode0)
        tick_n(c, 3)
        t |> equal(cave_at(c, 2, 3), Tile.diamond)
        t |> equal(cave_at(c, 0, 0), Tile.steel)
    }
}

[test]
def test_amoeba(t : T?) {
    t |> run("enclosed becomes diamonds") <| @(t : T?) {
        var c <- cave_from_ascii("###\n#a#\n###")
        cave_tick(c)
        t |> equal(cave_at(c, 1, 1), Tile.diamond)
    }
    t |> run("grows into dirt when forced") <| @(t : T?) {
        var c <- cave_from_ascii("#####\n#a:##\n#####")
        c.amoeba_grow = 1.0
        cave_tick(c)
        t |> equal(cave_at(c, 1, 1), Tile.amoeba)
        t |> equal(cave_at(c, 2, 1), Tile.amoeba)
    }
    t |> run("mass becomes boulders") <| @(t : T?) {
        var c <- make_cave(20, 12)
        for (y in range(c.rows)) {
            for (x in range(c.cols)) {
                cave_set(c, x, y, Tile.amoeba)
            }
        }
        cave_tick(c)
        t |> equal(cave_count(c, Tile.amoeba), 0)
        t |> equal(cave_count(c, Tile.boulder), 20 * 12)
    }
}

[test]
def test_magic_wall(t : T?) {
    var c <- cave_from_ascii("XoX\nXMX\nX.X\nXXX")
    cave_tick(c)
    t |> success(c.magic_timer > 0.0, "activated")
    t |> equal(cave_at(c, 1, 1), Tile.magic_active)
    t |> equal(cave_at(c, 1, 2), Tile.diamond_fall, "boulder became diamond below")
    t |> equal(cave_at(c, 1, 0), Tile.empty)
}

[test]
def test_magic_expires_to_brick(t : T?) {
    var c <- cave_from_ascii("XoX\nXMX\nX.X\nXXX")
    c.magic_time_max = 2.0 / CAVE_HZ
    cave_tick(c)
    t |> success(c.magic_timer > 0.0)
    tick_n(c, 2)
    t |> success(c.magic_spent)
    t |> equal(cave_at(c, 1, 1), Tile.brick)
}

[test]
def test_expanding_wall(t : T?) {
    var c <- cave_from_ascii(".....\n..=..\n.....")
    cave_tick(c)
    t |> equal(cave_at(c, 1, 1), Tile.expand, "grows left first")
    t |> equal(cave_at(c, 2, 1), Tile.expand, "origin stays")
    cave_tick(c)
    t |> equal(cave_at(c, 0, 1), Tile.expand)
}

[test]
def test_slime_lets_boulder_through(t : T?) {
    var c <- cave_from_ascii("XoX\nX~X\nX.X\nXXX")
    c.slime_perm = 1.0
    cave_tick(c)
    t |> equal(cave_at(c, 1, 0), Tile.empty)
    t |> equal(cave_at(c, 1, 1), Tile.slime, "slime remains")
    t |> equal(cave_at(c, 1, 2), Tile.boulder_fall)
}

[test]
def test_quota_exit_and_timeout(t : T?) {
    t |> run("quota opens exit") <| @(t : T?) {
        var c <- cave_from_ascii("R*E\nXXX")
        c.diamonds_need = 1
        cave_set_input(c, int2(1, 0), false)
        cave_tick(c)
        t |> equal(c.diamonds_got, 1)
        t |> equal(cave_at(c, 2, 0), Tile.exit_open)
        t |> success(c.exit_opened)
        cave_set_input(c, int2(1, 0), false)
        cave_tick(c)
        t |> success(c.cleared)
    }
    t |> run("timeout kills") <| @(t : T?) {
        var c <- cave_from_ascii("R.\nXX")
        c.time_left = 0.05
        cave_apply_time(c, 0.1)
        t |> success(!c.alive)
        t |> success(c.timed_out)
    }
}
