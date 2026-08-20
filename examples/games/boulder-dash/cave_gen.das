options gen2
options persistent_heap

module cave_gen public

require math
require daslib/random
require cave

def rand_span(var rng : int4&; lo, hi : int) : int {
    if (hi <= lo) {
        return lo
    }
    return lo + (random_int(rng) % (hi - lo))
}

def chance(var rng : int4&; p : float) : bool {
    return random_float(rng) < p
}

def fill_rect(var c : Cave; x, y, w, h : int; t : Tile) {
    let x1 = max(x, 0)
    let y1 = max(y, 0)
    let x2 = min(x + w, c.cols)
    let y2 = min(y + h, c.rows)
    for (yy in range(y1, y2)) {
        for (xx in range(x1, x2)) {
            cave_set(c, xx, yy, t)
        }
    }
}

def steel_border(var c : Cave) {
    for (x in range(c.cols)) {
        cave_set(c, x, 0, Tile.steel)
        cave_set(c, x, c.rows - 1, Tile.steel)
    }
    for (y in range(c.rows)) {
        cave_set(c, 0, y, Tile.steel)
        cave_set(c, c.cols - 1, y, Tile.steel)
    }
}

def fill_dirt(var c : Cave) {
    for (i in range(length(c.cells))) {
        c.cells[i] = Tile.dirt
    }
}

def scatter_rocks(var c : Cave; var rng : int4&; p_bould, p_gem : float) {
    for (y in range(1, c.rows - 1)) {
        for (x in range(1, c.cols - 1)) {
            if (chance(rng, p_bould)) {
                cave_set(c, x, y, Tile.boulder)
            } elif (chance(rng, p_gem)) {
                cave_set(c, x, y, Tile.diamond)
            }
        }
    }
}

def carve_room(var c : Cave; var rng : int4&) {
    let w = rand_span(rng, 3, 8)
    let h = rand_span(rng, 2, 5)
    let x = rand_span(rng, 1, c.cols - w - 1)
    let y = rand_span(rng, 1, c.rows - h - 1)
    fill_rect(c, x, y, w, h, Tile.empty)
}

def place_exit_box(var c : Cave; var rng : int4&) {
    let w = 5
    let h = 4
    let x = rand_span(rng, c.cols / 2, c.cols - w - 1)
    let y = rand_span(rng, 1, c.rows - h - 1)
    fill_rect(c, x, y, w, h, Tile.steel)
    fill_rect(c, x + 1, y + 1, w - 2, h - 2, Tile.empty)
    cave_set(c, x + 2, y + 1, Tile.exit_closed)
    cave_set(c, x, y + h - 2, Tile.dirt)
}

def place_player_pocket(var c : Cave; var rng : int4&) {
    let x = rand_span(rng, 2, min(12, c.cols - 4))
    let y = rand_span(rng, c.rows - 8, c.rows - 3)
    fill_rect(c, x - 1, y - 1, 3, 3, Tile.empty)
    cave_set(c, x, y, Tile.player)
}

def place_insect_pocket(var c : Cave; var rng : int4&; fly : bool) {
    let x = rand_span(rng, 2, c.cols - 3)
    let y = rand_span(rng, 2, c.rows - 3)
    fill_rect(c, x, y, 2, 2, Tile.empty)
    let t = fly ? Tile.firefly_u : Tile.butterfly_u
    cave_set(c, x, y, t)
}

def place_amoeba_blob(var c : Cave; var rng : int4&) {
    let n = rand_span(rng, 5, 13)
    let x0 = rand_span(rng, 3, c.cols - 5)
    let y0 = rand_span(rng, 3, c.rows - 5)
    for (_ in range(n)) {
        let x = clamp(x0 + rand_span(rng, 0, 3), 1, c.cols - 2)
        let y = clamp(y0 + rand_span(rng, 0, 3), 1, c.rows - 2)
        cave_set(c, x, y, Tile.amoeba)
    }
}

def place_magic_strip(var c : Cave; var rng : int4&) {
    let w = rand_span(rng, 4, 9)
    let x = rand_span(rng, 2, c.cols - w - 2)
    let y = rand_span(rng, 4, c.rows - 4)
    fill_rect(c, x, y - 1, w, 1, Tile.empty)
    fill_rect(c, x, y, w, 1, Tile.magic)
    fill_rect(c, x, y + 1, w, 1, Tile.empty)
}

def place_expand_seg(var c : Cave; var rng : int4&) {
    let x = rand_span(rng, 3, c.cols - 4)
    let y = rand_span(rng, 2, c.rows - 3)
    cave_set(c, x, y, Tile.expand)
    cave_set(c, x - 1, y, Tile.empty)
    cave_set(c, x + 1, y, Tile.empty)
}

def place_slime_patch(var c : Cave; var rng : int4&) {
    let x = rand_span(rng, 2, c.cols - 5)
    let y = rand_span(rng, 3, c.rows - 4)
    fill_rect(c, x, y, 3, 2, Tile.slime)
}

def gen_seed(world_seed, cave_index : int) : int4 {
    return random_seed(world_seed * 73856093 + cave_index * 19349663 + 83492791)
}

def replace_all_player(var c : Cave) {
    for (i in range(length(c.cells))) {
        if (c.cells[i] == Tile.player) {
            c.cells[i] = Tile.empty
        }
    }
}

def force_player(var c : Cave) {
    replace_all_player(c)
    let x = 3
    let y = c.rows - 4
    fill_rect(c, x - 1, y - 1, 3, 3, Tile.empty)
    cave_set(c, x, y, Tile.player)
}

def force_exit(var c : Cave) {
    let x = c.cols - 6
    let y = 3
    fill_rect(c, x, y, 4, 3, Tile.steel)
    fill_rect(c, x + 1, y + 1, 2, 1, Tile.empty)
    cave_set(c, x + 1, y + 1, Tile.exit_closed)
    cave_set(c, x, y + 1, Tile.dirt)
}

def force_diamonds(var c : Cave) {
    return if (cave_count_diamonds(c) >= 1)
    cave_set(c, 5, c.rows - 4, Tile.diamond)
    cave_set(c, 6, c.rows - 4, Tile.diamond)
    cave_set(c, 7, c.rows - 4, Tile.diamond)
}

def paint_interior(var c : Cave; var rng : int4&; cave_index : int) {
    var p_bould = 0.14 + float(cave_index) * 0.008
    if (p_bould > 0.22) {
        p_bould = 0.22
    }
    var p_gem = 0.05 - float(cave_index) * 0.001
    if (p_gem < 0.025) {
        p_gem = 0.025
    }
    scatter_rocks(c, rng, p_bould, p_gem)
    let rooms = rand_span(rng, 2, 5)
    for (_ in range(rooms)) {
        carve_room(c, rng)
    }
    let flies = rand_span(rng, 2, 5 + min(cave_index, 3))
    for (_ in range(flies)) {
        place_insect_pocket(c, rng, true)
    }
    if (cave_index > 0 && chance(rng, 0.5)) {
        place_insect_pocket(c, rng, false)
    }
    if (chance(rng, 0.35)) {
        place_amoeba_blob(c, rng)
    }
    if (chance(rng, 0.4)) {
        place_magic_strip(c, rng)
    }
    if (chance(rng, 0.25)) {
        place_expand_seg(c, rng)
    }
    if (chance(rng, 0.2)) {
        place_slime_patch(c, rng)
    }
    place_exit_box(c, rng)
    place_player_pocket(c, rng)
    steel_border(c)
    if (cave_count(c, Tile.player) != 1) {
        force_player(c)
    }
    let exits = cave_count(c, Tile.exit_closed) + cave_count(c, Tile.exit_open)
    if (exits < 1) {
        force_exit(c)
    }
    force_diamonds(c)
    steel_border(c)
}

def cave_is_sane(c : Cave) : bool {
    if (cave_count(c, Tile.player) != 1) {
        return false
    }
    let exits = cave_count(c, Tile.exit_closed) + cave_count(c, Tile.exit_open)
    if (exits < 1) {
        return false
    }
    return cave_count_diamonds(c) >= 1
}

def finish_cave(var c : Cave; rng : int4; cave_index : int) {
    find_rockford(c)
    let gems = cave_count_diamonds(c)
    var need = int(ceil(float(gems) * 0.45))
    need = clamp(need, 1, max(gems, 1))
    c.diamonds_need = need
    c.diamonds_got = 0
    c.time_left = max(150.0 - 4.0 * float(min(cave_index, 20)), 80.0)
    c.rng = rng
    c.alive = true
    c.cleared = false
}

def fallback_cave() : Cave {
    var c <- make_cave(CAVE_COLS, CAVE_ROWS)
    fill_dirt(c)
    steel_border(c)
    fill_rect(c, 2, 2, 6, 4, Tile.empty)
    cave_set(c, 3, 3, Tile.player)
    cave_set(c, 5, 3, Tile.diamond)
    cave_set(c, 6, 3, Tile.diamond)
    fill_rect(c, 30, 8, 5, 4, Tile.steel)
    fill_rect(c, 31, 9, 3, 2, Tile.empty)
    cave_set(c, 32, 9, Tile.exit_closed)
    cave_set(c, 30, 10, Tile.dirt)
    let rng = random_seed(1)
    finish_cave(c, rng, 0)
    return <- c
}

def generate_cave(world_seed, cave_index : int) : Cave {
    for (attempt in range(16)) {
        var rng = gen_seed(world_seed + attempt, cave_index)
        var c <- make_cave(CAVE_COLS, CAVE_ROWS)
        fill_dirt(c)
        paint_interior(c, rng, cave_index)
        if (cave_is_sane(c)) {
            finish_cave(c, rng, cave_index)
            return <- c
        }
    }
    return <- fallback_cave()
}
