options gen2

require dastest/testing_boost public

require cave
require cave_gen
require daslib/random

//! Cave simulation: physics, creatures, input, and couplings.

def ascii_eq(t : T?; c : Cave; want : string; msg : string) {
    let got = cave_to_ascii(c)
    t |> equal(got, want, msg)
}

def tick_n(var c : Cave; n : int) {
    for (_ in range(n)) {
        cave_tick(c)
    }
}

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

[test(description = "An ASCII cave parses to the right size and Rockford position and prints back to the same text.")]
def test_ascii_roundtrip(t : T?) {
    let src = "XoR*\nX:.E"
    let c <- cave_from_ascii(src)
    t |> equal(c.cols, 4)
    t |> equal(c.rows, 2)
    t |> equal(c.rockford, int2(2, 0))
    ascii_eq(t, c, src, "roundtrip")
}

[test(description = "A boulder over an empty cell moves down one cell as a falling boulder, then settles on the next tick.")]
def test_boulder_falls_into_empty(t : T?) {
    var c <- cave_from_ascii("XoX\nX.X\nXXX")
    cave_tick(c)
    ascii_eq(t, c, "X.X\nX!X\nXXX", "one cell down as falling")
    cave_tick(c)
    ascii_eq(t, c, "X.X\nXoX\nXXX", "settles")
}

[test(description = "A stack of boulders falls one cell per tick: the scan bit stops the lower boulder from falling twice in one tick.")]
def test_stacked_boulders_one_cell_per_tick(t : T?) {
    var c <- cave_from_ascii("#####\n#o###\n#o###\n#.###\n#.###\n#####")
    cave_tick(c)
    t |> equal(cave_at(c, 1, 1), Tile.boulder, "top stays")
    t |> equal(cave_at(c, 1, 2), Tile.empty, "middle vacated")
    t |> equal(cave_at(c, 1, 3), Tile.boulder_fall, "lower fell one")
    t |> equal(cave_at(c, 1, 4), Tile.empty, "scan bit blocked a double fall")
}

[test(description = "Dirt is not empty space: a boulder on dirt does not move.")]
def test_boulder_rests_on_dirt(t : T?) {
    var c <- cave_from_ascii("XoX\nX:X\nXXX")
    cave_tick(c)
    ascii_eq(t, c, "XoX\nX:X\nXXX", "dirt is not empty")
}

[test(description = "A boulder resting on another boulder rolls off diagonally to the left when the left side is free.")]
def test_boulder_rolls_left(t : T?) {
    var c <- cave_from_ascii("#####\n#.o.#\n#.o.#\n#...#\n#####")
    cave_tick(c)
    t |> equal(cave_at(c, 2, 1), Tile.empty, "rolled off")
    t |> equal(cave_at(c, 1, 2), Tile.boulder_fall, "diagonal left")
}

[test(description = "When both sides are free, a rolling boulder goes left and not right.")]
def test_boulder_prefers_left_over_right(t : T?) {
    var c <- cave_from_ascii("#####\n#.o.#\n#.o.#\n#...#\n#####")
    cave_tick(c)
    t |> success(cave_at(c, 3, 2) != Tile.boulder_fall, "did not roll right")
    t |> success(cave_at(c, 1, 2) == Tile.boulder_fall, "rolled left")
}

[test(description = "A falling boulder that lands on Rockford kills him and sets the died event.")]
def test_falling_crushes_rockford(t : T?) {
    var c <- cave_from_ascii("XoX\nXRX\nXXX")
    cave_set(c, 1, 0, Tile.boulder_fall)
    cave_tick(c)
    t |> success(!c.alive, "crushed")
    t |> success(c.died)
    t |> success(cave_at(c, 1, 1) == Tile.boulder)
}

[test(description = "A boulder already resting on Rockford's head does not kill him; both stay in place.")]
def test_stationary_boulder_does_not_crush(t : T?) {
    var c <- cave_from_ascii("XoX\nXRX\nXXX")
    cave_tick(c)
    t |> success(c.alive, "stationary on player is blocked, not crushing")
    t |> equal(cave_at(c, 1, 0), Tile.boulder)
    t |> equal(cave_at(c, 1, 1), Tile.player)
}

[test(description = "Walking onto a diamond collects it, and a grab clears dirt or collects a diamond while Rockford stays in place.")]
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

[test(description = "A diamond fires the fell event when it starts falling and the landed event, not before, when it comes to rest.")]
def test_diamond_fall_events(t : T?) {
    var c <- cave_from_ascii("X*X\nX.X\nXXX")
    cave_tick(c)
    t |> success(c.fell_diamond, "diamond started falling")
    t |> success(!c.landed_diamond)
    cave_tick(c)
    t |> success(c.landed_diamond, "diamond landed")
    t |> equal(cave_at(c, 1, 1), Tile.diamond)
}

[test(description = "The step preview predicts a move into dirt, and predicts no move into steel or into a boulder.")]
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

[test(description = "The input latch keeps a tapped direction through a later idle latch, lets the last tap win, holds a locked direction, and queues a changed tap for the tick after consume.")]
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

[test(description = "Pushing a boulder sideways takes a wind-up tick and then moves both; a push into an occupied cell or upward does nothing.")]
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

[test(description = "A firefly follows the left-hand rule: with left and up blocked it turns east and moves.")]
def test_firefly_left_hand(t : T?) {
    var c <- cave_from_ascii("XXXX\nXn.X\nX..X\nXXXX")
    cave_tick(c)
    t |> equal(cave_at(c, 2, 1), Tile.firefly_r, "left and up blocked, turns east")
    t |> equal(cave_at(c, 1, 1), Tile.empty)
}

[test(description = "A butterfly follows the mirrored right-hand rule: with east blocked it turns west into the empty cell.")]
def test_butterfly_mirror(t : T?) {
    var c <- cave_from_ascii("XXXX\nX.AX\nX..X\nXXXX")
    cave_tick(c)
    t |> equal(cave_at(c, 1, 1), Tile.butterfly_l, "right-hand: from up, try right (east) blocked, stay/turn west into empty")
}

[test(description = "A falling boulder on a firefly leaves a 3x3 empty blast, while a butterfly hit by a boulder or touched by amoeba blasts into diamonds; steel survives.")]
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
    t |> run("butterfly walks into falling boulder -> diamonds") <| @(t : T?) {
        var c <- cave_from_ascii("XXXXX\nXA!#X\nXXXXX")
        cave_tick(c)
        t |> success(c.exploded, "walk-in blast")
        tick_n(c, 3)
        t |> success(cave_count_diamonds(c) >= 1, "left diamonds")
    }
    t |> run("amoeba touching butterfly -> diamonds") <| @(t : T?) {
        var c <- cave_from_ascii("XXXXX\nX...X\nX.AaX\nX...X\nXXXXX")
        cave_tick(c)
        t |> success(c.exploded)
        tick_n(c, 3)
        t |> success(cave_count_diamonds(c) >= 1)
    }
}

[test(description = "Enclosed amoeba turns into diamonds, amoeba grows into dirt, and an amoeba mass that fills the cave turns into boulders.")]
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

[test(description = "A falling boulder activates a magic wall and comes out below it as a falling diamond.")]
def test_magic_wall(t : T?) {
    // the wall converts only FALLING objects, so the fixture drops the boulder one cell first
    var c <- cave_from_ascii("XoX\nX.X\nXMX\nX.X\nXXX")
    tick_n(c, 2)
    t |> success(c.magic_timer > 0.0, "activated")
    t |> equal(cave_at(c, 1, 2), Tile.magic_active)
    t |> equal(cave_at(c, 1, 3), Tile.diamond_fall, "boulder became diamond below")
    t |> equal(cave_at(c, 1, 1), Tile.empty)
}

[test(description = "An active magic wall runs out after its time limit, sets the spent event, and turns into brick.")]
def test_magic_expires_to_brick(t : T?) {
    var c <- cave_from_ascii("XoX\nX.X\nXMX\nX.X\nXXX")
    c.magic_time_max = 2.0 / CAVE_HZ
    tick_n(c, 2)
    t |> success(c.magic_timer > 0.0)
    tick_n(c, 2)
    t |> success(c.magic_spent)
    t |> equal(cave_at(c, 1, 2), Tile.brick)
}

[test(description = "An expanding wall grows left first, one cell per tick, and keeps its origin cell.")]
def test_expanding_wall(t : T?) {
    var c <- cave_from_ascii(".....\n..=..\n.....")
    cave_tick(c)
    t |> equal(cave_at(c, 1, 1), Tile.expand, "grows left first")
    t |> equal(cave_at(c, 2, 1), Tile.expand, "origin stays")
    cave_tick(c)
    t |> equal(cave_at(c, 0, 1), Tile.expand)
}

[test(description = "Fully permeable slime lets a boulder pass through it and stays in place.")]
def test_slime_lets_boulder_through(t : T?) {
    var c <- cave_from_ascii("XoX\nX~X\nX.X\nXXX")
    c.slime_perm = 1.0
    cave_tick(c)
    t |> equal(cave_at(c, 1, 0), Tile.empty)
    t |> equal(cave_at(c, 1, 1), Tile.slime, "slime remains")
    t |> equal(cave_at(c, 1, 2), Tile.boulder_fall)
}

[test(description = "Reaching the diamond quota opens the exit and walking into it clears the cave; running out of time kills Rockford.")]
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

[test(description = "A magic wall converts only a falling boulder: a resting boulder stays on top, and a spent wall stops a falling one.")]
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

[test(description = "The Tile enum, its glyph table, its family range tests and the insect heading encoding agree member for member.")]
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

[test(description = "For every tile in Rockford's path, the step preview agrees with the tick: a predicted step always lands, and an unpredicted step never happens.")]
def test_peek_step_parity(t : T?) {
    t |> run("a predicted step always happens - over-prediction is the snap-back bug") @(tt : T?) {
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

[test(description = "A raw ASCII cave has no diamond quota, so its exit never opens; finish_cave sets the quota and the exit then opens.")]
def test_ascii_cave_exit_contract(t : T?) {
    t |> run("a raw ascii cave can never open its exit - the loader must finish it") @(tt : T?) {
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
