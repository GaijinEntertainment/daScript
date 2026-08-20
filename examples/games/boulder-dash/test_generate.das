options gen2
require dastest/testing_boost public
require cave
require cave_gen

def border_is_steel(c : Cave) : bool {
    for (x in range(c.cols)) {
        if (cave_at(c, x, 0) != Tile.steel || cave_at(c, x, c.rows - 1) != Tile.steel) {
            return false
        }
    }
    for (y in range(c.rows)) {
        if (cave_at(c, 0, y) != Tile.steel || cave_at(c, c.cols - 1, y) != Tile.steel) {
            return false
        }
    }
    return true
}

[test]
def test_generated_cave_invariants(t : T?) {
    let c <- generate_cave(1984, 0)
    t |> equal(c.cols, CAVE_COLS)
    t |> equal(c.rows, CAVE_ROWS)
    t |> success(border_is_steel(c), "steel border")
    t |> equal(cave_count(c, Tile.player), 1, "one Rockford")
    let exits = cave_count(c, Tile.exit_closed) + cave_count(c, Tile.exit_open)
    t |> success(exits >= 1, "has exit")
    let gems = cave_count_diamonds(c)
    t |> success(gems >= 1, "has diamonds")
    t |> success(c.diamonds_need >= 1 && c.diamonds_need <= gems, "quota in range")
    t |> success(c.time_left >= 80.0)
    t |> success(c.rockford.x > 0)
    t |> success(cave_count(c, Tile.boulder) > 20, "scattered boulders")
}

[test]
def test_same_seed_identical(t : T?) {
    let a <- generate_cave(42, 3)
    let b <- generate_cave(42, 3)
    t |> equal(cave_to_ascii(a), cave_to_ascii(b), "deterministic")
    t |> equal(a.diamonds_need, b.diamonds_need)
}

[test]
def test_different_seeds_differ(t : T?) {
    let a <- generate_cave(1984, 0)
    let b <- generate_cave(2026, 0)
    let d <- generate_cave(1984, 1)
    t |> success(cave_to_ascii(a) != cave_to_ascii(b), "world seed changes layout")
    t |> success(cave_to_ascii(a) != cave_to_ascii(d), "cave index changes layout")
}

[test]
def test_fallback_playable(t : T?) {
    let c <- fallback_cave()
    t |> success(cave_is_sane(c))
    t |> success(border_is_steel(c))
    t |> equal(cave_count(c, Tile.player), 1)
}

[test]
def test_many_seeds_sane(t : T?) {
    for (s in range(20)) {
        let c <- generate_cave(1000 + s, s % 5)
        t |> success(cave_is_sane(c), "seed {s}")
        t |> success(c.diamonds_need <= cave_count_diamonds(c))
    }
}

def survives_idle(c0 : Cave; ticks : int) : bool {
    var c <- cave_from_ascii(cave_to_ascii(c0))
    find_rockford(c)
    for (_ in range(ticks)) {
        cave_tick(c)
        if (!c.alive) {
            return false
        }
    }
    return true
}

[test]
def test_spawn_not_sucker(t : T?) {
    t |> run("seed 1984 cave 3 survives idle") <| @(t : T?) {
        let c <- generate_cave(1984, 2)
        t |> success(survives_idle(c, 24), "no crush/chase in first 3s")
    }
    t |> run("early caves across seeds") <| @(t : T?) {
        for (s in range(12)) {
            for (idx in range(5)) {
                let c <- generate_cave(2000 + s * 17, idx)
                t |> success(survives_idle(c, 24), "seed {s} cave {idx + 1}")
            }
        }
    }
}
