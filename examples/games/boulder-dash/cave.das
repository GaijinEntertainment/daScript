options gen2
options persistent_heap

module cave public

require math
require daslib/random
require daslib/strings_boost

// C64 cave size. Tests may use smaller grids.
let CAVE_COLS = 40
let CAVE_ROWS = 22
let CAVE_HZ = 8.0
let AMOEBA_MAX = 200
let DEFAULT_MAGIC_TIME = 20.0
let DEFAULT_AMOEBA_GROW = 0.03
let DEFAULT_SLIME_PERM = 0.2

// Legend (char ↔ Tile, enum order):
//   . empty   : dirt     o boulder   ! boulder_fall
//   * diamond $ diamond_fall  # brick  X steel
//   M magic   m magic_active  = expand  ~ slime
//   nesw firefly NESW   ABCD butterfly NESW
//   a amoeba  R player  E exit_closed  O exit_open
//   123 empty-explosion   456 diamond-explosion
enum Tile : int {
    empty = 0
    dirt
    boulder
    boulder_fall
    diamond
    diamond_fall
    brick
    steel
    magic
    magic_active
    expand
    slime
    firefly_u
    firefly_r
    firefly_d
    firefly_l
    butterfly_u
    butterfly_r
    butterfly_d
    butterfly_l
    amoeba
    player
    exit_closed
    exit_open
    explode0
    explode1
    explode2
    dexplode0
    dexplode1
    dexplode2
}

let TILE_N = 30
let TILE_CH = fixed_array(
    '.', ':', 'o', '!', '*', '$', '#', 'X',
    'M', 'm', '=', '~',
    'n', 'e', 's', 'w',
    'A', 'B', 'C', 'D',
    'a', 'R', 'E', 'O',
    '1', '2', '3', '4', '5', '6'
)

let DIR_X = fixed_array(0, 1, 0, -1)
let DIR_Y = fixed_array(-1, 0, 1, 0)
let FLY_TURN = fixed_array(3, 0, 1, 2)
let BUTTER_TURN = fixed_array(1, 0, 3, 2)

struct Mover {
    from_x : int
    from_y : int
    to_x : int
    to_y : int
    tile : Tile
}

struct Cave {
    cols : int = CAVE_COLS
    rows : int = CAVE_ROWS
    cells : array<Tile>
    scanned : array<bool>
    movers : array<Mover>
    rockford : int2 = int2(-1, -1)
    wish_dir : int2 = int2(0, 0)
    grab : bool = false
    input_locked : bool = false
    next_dir : int2 = int2(0, 0)
    next_grab : bool = false
    push_held : int2 = int2(0, 0)
    diamonds_got : int = 0
    diamonds_need : int = 0
    time_left : float = 9999.0
    magic_timer : float = 0.0
    magic_time_max : float = DEFAULT_MAGIC_TIME
    magic_spent : bool = false
    amoeba_grow : float = DEFAULT_AMOEBA_GROW
    amoeba_count : int = 0
    slime_perm : float = DEFAULT_SLIME_PERM
    rng : int4
    alive : bool = true
    god : bool = false
    cleared : bool = false
    dug : bool = false
    collected : int = 0
    landed_boulder : bool = false
    landed_diamond : bool = false
    fell_boulder : bool = false
    fell_diamond : bool = false
    exploded : bool = false
    died : bool = false
    exit_opened : bool = false
    timed_out : bool = false
    pushed : bool = false
    magic_on : bool = false
    magic_convert : bool = false
    amoeba_grew : bool = false
    amoeba_gems : bool = false
    amoeba_rocks : bool = false
    wall_grew : bool = false
}

def tile_to_char(t : Tile) : int {
    let i = int(t)
    if (i < 0 || i >= TILE_N) {
        return '?'
    }
    return TILE_CH[i]
}

def tile_from_int(i : int) : Tile {
    return unsafe(reinterpret<Tile>(i))
}

def char_to_tile(ch : int) : Tile {
    for (i in range(TILE_N)) {
        if (TILE_CH[i] == ch) {
            return tile_from_int(i)
        }
    }
    return Tile.steel
}

def in_bounds(c : Cave; x, y : int) : bool {
    return x >= 0 && y >= 0 && x < c.cols && y < c.rows
}

def cave_idx(c : Cave; x, y : int) : int {
    return y * c.cols + x
}

def cave_at(c : Cave; x, y : int) : Tile {
    if (!in_bounds(c, x, y)) {
        return Tile.steel
    }
    return c.cells[cave_idx(c, x, y)]
}

def cave_set(var c : Cave; x, y : int; t : Tile) {
    return if (!in_bounds(c, x, y))
    c.cells[cave_idx(c, x, y)] = t
}

def cave_mark_scanned(var c : Cave; x, y : int) {
    return if (!in_bounds(c, x, y))
    c.scanned[cave_idx(c, x, y)] = true
}

def cave_is_scanned(c : Cave; x, y : int) : bool {
    if (!in_bounds(c, x, y)) {
        return true
    }
    return c.scanned[cave_idx(c, x, y)]
}

def is_rounded(t : Tile) : bool {
    return t == Tile.boulder || t == Tile.boulder_fall || t == Tile.diamond || t == Tile.diamond_fall || t == Tile.brick
}

def is_fallable(t : Tile) : bool {
    return t == Tile.boulder || t == Tile.boulder_fall || t == Tile.diamond || t == Tile.diamond_fall
}

def is_firefly(t : Tile) : bool {
    let i = int(t)
    return i >= int(Tile.firefly_u) && i <= int(Tile.firefly_l)
}

def is_butterfly(t : Tile) : bool {
    let i = int(t)
    return i >= int(Tile.butterfly_u) && i <= int(Tile.butterfly_l)
}

def is_insect(t : Tile) : bool {
    return is_firefly(t) || is_butterfly(t)
}

def is_explode(t : Tile) : bool {
    let i = int(t)
    return i >= int(Tile.explode0) && i <= int(Tile.dexplode2)
}

def is_gem_tile(t : Tile) : bool {
    return t == Tile.diamond || t == Tile.diamond_fall
}

def insect_dir(t : Tile) : int {
    if (is_firefly(t)) {
        return int(t) - int(Tile.firefly_u)
    }
    return int(t) - int(Tile.butterfly_u)
}

def insect_tile(fly : bool; dir : int) : Tile {
    let d = ((dir % 4) + 4) % 4
    if (fly) {
        return tile_from_int(int(Tile.firefly_u) + d)
    }
    return tile_from_int(int(Tile.butterfly_u) + d)
}

def falling_variant(gem : bool) : Tile {
    return gem ? Tile.diamond_fall : Tile.boulder_fall
}

def settled_variant(gem : bool) : Tile {
    return gem ? Tile.diamond : Tile.boulder
}

def make_cave(cols, rows : int) : Cave {
    var c <- Cave(cols = cols, rows = rows)
    c.cells |> resize(cols * rows)
    c.scanned |> resize(cols * rows)
    c.rng = random_seed(1)
    return <- c
}

def adopt_cave(var dst : Cave; var src : Cave) {
    delete dst.cells
    delete dst.scanned
    delete dst.movers
    dst <- src
}

def find_rockford(var c : Cave) {
    c.rockford = int2(-1, -1)
    for (y in range(c.rows)) {
        for (x in range(c.cols)) {
            if (cave_at(c, x, y) == Tile.player) {
                c.rockford = int2(x, y)
                return
            }
        }
    }
}

def cave_from_ascii(text : string) : Cave {
    let normalized = replace(replace(text, "\r\n", "\n"), "\r", "\n")
    var lines <- split(normalized, "\n")
    var rows <- [for (ln in lines); ln; where !empty(ln)]
    let h = length(rows)
    var w = 0
    for (ln in rows) {
        w = max(w, length(ln))
    }
    if (w < 1 || h < 1) {
        return <- make_cave(1, 1)
    }
    var c <- make_cave(w, h)
    for (y in range(h)) {
        peek_data(rows[y]) $(d) {
            for (x in range(w)) {
                let ch = x < length(d) ? int(d[x]) : 'X'
                cave_set(c, x, y, char_to_tile(ch))
            }
        }
    }
    find_rockford(c)
    return <- c
}

def cave_to_ascii(c : Cave) : string {
    return build_string() $(var writer) {
        for (y in range(c.rows)) {
            for (x in range(c.cols)) {
                writer |> write_char(tile_to_char(cave_at(c, x, y)))
            }
            if (y + 1 < c.rows) {
                writer |> write("\n")
            }
        }
    }
}

def cave_count(c : Cave; t : Tile) : int {
    var n = 0
    for (cell in c.cells) {
        if (cell == t) {
            n++
        }
    }
    return n
}

def cave_count_diamonds(c : Cave) : int {
    var n = 0
    for (cell in c.cells) {
        if (is_gem_tile(cell)) {
            n++
        }
    }
    return n
}

def reset_tick_events(var c : Cave) {
    c.dug = false
    c.collected = 0
    c.landed_boulder = false
    c.landed_diamond = false
    c.fell_boulder = false
    c.fell_diamond = false
    c.exploded = false
    c.died = false
    c.exit_opened = false
    c.timed_out = false
    c.pushed = false
    c.magic_on = false
    c.magic_convert = false
    c.amoeba_grew = false
    c.amoeba_gems = false
    c.amoeba_rocks = false
    c.wall_grew = false
}

def add_mover(var c : Cave; fx, fy, tx, ty : int; t : Tile) {
    c.movers |> push <| Mover(
        from_x = fx, from_y = fy,
        to_x = tx, to_y = ty,
        tile = t
    )
}

def moved_later(fx, fy, tx, ty : int) : bool {
    return ty > fy || (ty == fy && tx > fx)
}

def move_tile(var c : Cave; x, y, nx, ny : int; result : Tile) {
    cave_set(c, x, y, Tile.empty)
    cave_set(c, nx, ny, result)
    if (moved_later(x, y, nx, ny)) {
        cave_mark_scanned(c, nx, ny)
    }
    add_mover(c, x, y, nx, ny, result)
}

def kill_rockford(var c : Cave) {
    return if (c.god || !c.alive)
    c.alive = false
    c.died = true
    let p = c.rockford
    if (in_bounds(c, p.x, p.y) && cave_at(c, p.x, p.y) == Tile.player) {
        cave_set(c, p.x, p.y, Tile.empty)
    }
}

def collect_diamond(var c : Cave) {
    c.diamonds_got++
    c.collected++
}

def explode_cell(var c : Cave; x, y : int; gems : bool) {
    return if (!in_bounds(c, x, y))
    let t = cave_at(c, x, y)
    if (t == Tile.steel || t == Tile.exit_closed || t == Tile.exit_open) {
        return
    }
    if (t == Tile.player) {
        if (c.god) {
            return
        }
        kill_rockford(c)
    }
    cave_set(c, x, y, gems ? Tile.dexplode0 : Tile.explode0)
    cave_mark_scanned(c, x, y)
}

def explode_at(var c : Cave; cx, cy : int; gems : bool) {
    c.exploded = true
    for (dy in range(-1, 2)) {
        for (dx in range(-1, 2)) {
            explode_cell(c, cx + dx, cy + dy, gems)
        }
    }
}

def settle_at(var c : Cave; x, y : int; gem : bool) {
    cave_set(c, x, y, settled_variant(gem))
    if (gem) {
        c.landed_diamond = true
    } else {
        c.landed_boulder = true
    }
}

def note_fell(var c : Cave; gem : bool) {
    if (gem) {
        c.fell_diamond = true
    } else {
        c.fell_boulder = true
    }
}

def try_crush(var c : Cave; x, y : int; below : Tile; falling, gem : bool) : bool {
    if (!(falling && below == Tile.player)) {
        return false
    }
    if (c.god) {
        settle_at(c, x, y, gem)
        return true
    }
    kill_rockford(c)
    move_tile(c, x, y, x, y + 1, settled_variant(gem))
    return true
}

def try_fall_empty(var c : Cave; x, y : int; below : Tile; gem : bool) : bool {
    if (below != Tile.empty) {
        return false
    }
    move_tile(c, x, y, x, y + 1, falling_variant(gem))
    note_fell(c, gem)
    return true
}

def try_magic_fall(var c : Cave; x, y : int; below : Tile; gem : bool) : bool {
    if (below != Tile.magic && below != Tile.magic_active) {
        return false
    }
    if (c.magic_spent || cave_at(c, x, y + 2) != Tile.empty) {
        let cur = cave_at(c, x, y)
        if (cur == Tile.boulder_fall || cur == Tile.diamond_fall) {
            settle_at(c, x, y, gem)
        }
        return true
    }
    if (!c.magic_spent && c.magic_timer <= 0.0) {
        c.magic_timer = c.magic_time_max
        c.magic_on = true
        for (i in range(length(c.cells))) {
            if (c.cells[i] == Tile.magic) {
                c.cells[i] = Tile.magic_active
            }
        }
    }
    move_tile(c, x, y, x, y + 2, falling_variant(!gem))
    c.magic_convert = true
    note_fell(c, !gem)
    return true
}

def try_slime_fall(var c : Cave; x, y : int; below : Tile; gem : bool) : bool {
    if (below != Tile.slime) {
        return false
    }
    if (cave_at(c, x, y + 2) == Tile.empty && random_float(c.rng) < c.slime_perm) {
        move_tile(c, x, y, x, y + 2, falling_variant(gem))
        note_fell(c, gem)
        return true
    }
    let t = cave_at(c, x, y)
    if (t == Tile.boulder_fall || t == Tile.diamond_fall) {
        settle_at(c, x, y, gem)
    }
    return true
}

def try_insect_hit(var c : Cave; x, y : int; below : Tile; falling : bool) : bool {
    if (!(falling && is_insect(below))) {
        return false
    }
    explode_at(c, x, y + 1, is_butterfly(below))
    return true
}

def try_roll_dir(var c : Cave; x, y, dx : int; gem : bool) : bool {
    if (cave_at(c, x + dx, y) != Tile.empty || cave_at(c, x + dx, y + 1) != Tile.empty) {
        return false
    }
    move_tile(c, x, y, x + dx, y + 1, falling_variant(gem))
    note_fell(c, gem)
    return true
}

def try_roll(var c : Cave; x, y : int; below : Tile; gem : bool) : bool {
    if (!is_rounded(below)) {
        return false
    }
    if (try_roll_dir(c, x, y, -1, gem)) {
        return true
    }
    return try_roll_dir(c, x, y, 1, gem)
}

def process_fallable(var c : Cave; x, y : int) {
    let t = cave_at(c, x, y)
    let falling = t == Tile.boulder_fall || t == Tile.diamond_fall
    let gem = is_gem_tile(t)
    let below = cave_at(c, x, y + 1)
    if (try_crush(c, x, y, below, falling, gem) || try_fall_empty(c, x, y, below, gem) || try_magic_fall(c, x, y, below, gem) || try_slime_fall(c, x, y, below, gem) || try_insect_hit(c, x, y, below, falling) || (!falling && try_roll(c, x, y, below, gem))) {
        return
    }
    if (falling) {
        settle_at(c, x, y, gem)
    }
}

def try_grab(var c : Cave; nx, ny : int) {
    let dest = cave_at(c, nx, ny)
    if (dest == Tile.dirt) {
        cave_set(c, nx, ny, Tile.empty)
        c.dug = true
    } elif (is_gem_tile(dest)) {
        cave_set(c, nx, ny, Tile.empty)
        collect_diamond(c)
    }
}

def player_enter(var c : Cave; x, y, nx, ny : int; dest : Tile) {
    if (dest == Tile.dirt) {
        c.dug = true
    } elif (is_gem_tile(dest)) {
        collect_diamond(c)
    } elif (dest == Tile.exit_open) {
        c.cleared = true
        cave_set(c, x, y, Tile.empty)
        c.rockford = int2(nx, ny)
        add_mover(c, x, y, nx, ny, Tile.player)
        return
    }
    move_tile(c, x, y, nx, ny, Tile.player)
    c.rockford = int2(nx, ny)
}

def try_push(var c : Cave; x, y, nx, ny : int; d : int2; dest : Tile) : bool {
    if (dest != Tile.boulder || d.y != 0 || d.x == 0) {
        return false
    }
    let bx = nx + d.x
    let by = ny
    if (cave_at(c, bx, by) != Tile.empty) {
        return false
    }
    if (c.push_held != d) {
        c.push_held = d
        return true
    }
    move_tile(c, nx, ny, bx, by, Tile.boulder)
    move_tile(c, x, y, nx, ny, Tile.player)
    c.rockford = int2(nx, ny)
    c.push_held = d
    c.pushed = true
    return true
}

def try_step(var c : Cave; x, y, nx, ny : int; d : int2) {
    let dest = cave_at(c, nx, ny)
    if (is_insect(dest) || dest == Tile.amoeba || is_explode(dest)) {
        if (is_insect(dest) || is_explode(dest)) {
            kill_rockford(c)
        }
        c.push_held = int2(0)
        return
    }
    if (try_push(c, x, y, nx, ny, d, dest)) {
        return
    }
    if (dest == Tile.empty || dest == Tile.dirt || is_gem_tile(dest) || dest == Tile.exit_open) {
        c.push_held = int2(0)
        player_enter(c, x, y, nx, ny, dest)
        return
    }
    if (dest == Tile.boulder) {
        c.push_held = d
        return
    }
    c.push_held = int2(0)
}

def process_player(var c : Cave; x, y : int) {
    if (!c.alive || c.cleared) {
        return
    }
    c.rockford = int2(x, y)
    let d = c.wish_dir
    if (d.x == 0 && d.y == 0) {
        c.push_held = int2(0)
        return
    }
    let nx = x + d.x
    let ny = y + d.y
    if (!in_bounds(c, nx, ny)) {
        return
    }
    if (c.grab) {
        try_grab(c, nx, ny)
        c.push_held = int2(0)
        return
    }
    try_step(c, x, y, nx, ny, d)
}

def insect_hits_blast(c : Cave; x, y : int) : bool {
    for (d in range(4)) {
        let n = cave_at(c, x + DIR_X[d], y + DIR_Y[d])
        if (n == Tile.amoeba || n == Tile.boulder_fall || n == Tile.diamond_fall) {
            return true
        }
    }
    return false
}

def process_insect(var c : Cave; x, y : int) {
    let t = cave_at(c, x, y)
    if (insect_hits_blast(c, x, y)) {
        explode_at(c, x, y, is_butterfly(t))
        return
    }
    let fly = is_firefly(t)
    let heading = insect_dir(t)
    let turns = fly ? FLY_TURN : BUTTER_TURN
    for (k in range(4)) {
        let nd = (heading + turns[k]) % 4
        let nx = x + DIR_X[nd]
        let ny = y + DIR_Y[nd]
        let dest = cave_at(c, nx, ny)
        if (dest == Tile.player) {
            kill_rockford(c)
            if (c.god) {
                return
            }
            move_tile(c, x, y, nx, ny, insect_tile(fly, nd))
            return
        }
        if (dest == Tile.empty) {
            move_tile(c, x, y, nx, ny, insect_tile(fly, nd))
            return
        }
    }
}

def process_expand(var c : Cave; x, y : int) {
    if (cave_at(c, x - 1, y) == Tile.empty) {
        cave_set(c, x - 1, y, Tile.expand)
        add_mover(c, x, y, x - 1, y, Tile.expand)
        c.wall_grew = true
        return
    }
    if (cave_at(c, x + 1, y) == Tile.empty) {
        cave_set(c, x + 1, y, Tile.expand)
        cave_mark_scanned(c, x + 1, y)
        add_mover(c, x, y, x + 1, y, Tile.expand)
        c.wall_grew = true
    }
}

def process_explode(var c : Cave; x, y : int) {
    let t = cave_at(c, x, y)
    if (t == Tile.explode0) {
        cave_set(c, x, y, Tile.explode1)
    } elif (t == Tile.explode1) {
        cave_set(c, x, y, Tile.explode2)
    } elif (t == Tile.explode2) {
        cave_set(c, x, y, Tile.empty)
    } elif (t == Tile.dexplode0) {
        cave_set(c, x, y, Tile.dexplode1)
    } elif (t == Tile.dexplode1) {
        cave_set(c, x, y, Tile.dexplode2)
    } elif (t == Tile.dexplode2) {
        cave_set(c, x, y, Tile.diamond)
    }
}

def process_cell(var c : Cave; x, y : int) {
    let t = cave_at(c, x, y)
    if (is_fallable(t)) {
        process_fallable(c, x, y)
    } elif (t == Tile.player) {
        process_player(c, x, y)
    } elif (is_insect(t)) {
        process_insect(c, x, y)
    } elif (t == Tile.expand) {
        process_expand(c, x, y)
    } elif (is_explode(t)) {
        process_explode(c, x, y)
    }
}

def amoeba_can_grow_from(c : Cave; x, y : int) : bool {
    for (d in range(4)) {
        let t = cave_at(c, x + DIR_X[d], y + DIR_Y[d])
        if (t == Tile.empty || t == Tile.dirt || t == Tile.player) {
            return true
        }
    }
    return false
}

def replace_all(var c : Cave; from, to : Tile) {
    for (i in range(length(c.cells))) {
        if (c.cells[i] == from) {
            c.cells[i] = to
        }
    }
}

def grow_amoeba_at(var c : Cave; x, y : int) {
    var n = 0
    var ox : int[4]
    var oy : int[4]
    for (d in range(4)) {
        let nx = x + DIR_X[d]
        let ny = y + DIR_Y[d]
        let t = cave_at(c, nx, ny)
        if (t == Tile.empty || t == Tile.dirt || t == Tile.player) {
            ox[n] = nx
            oy[n] = ny
            n++
        }
    }
    return if (n == 0)
    let k = random_int(c.rng) % n
    let nx = ox[k]
    let ny = oy[k]
    if (cave_at(c, nx, ny) == Tile.player) {
        if (c.god) {
            return
        }
        kill_rockford(c)
    }
    cave_set(c, nx, ny, Tile.amoeba)
}

def amoeba_pass(var c : Cave) {
    var count = 0
    var growable = 0
    for (y in range(c.rows)) {
        for (x in range(c.cols)) {
            if (cave_at(c, x, y) == Tile.amoeba) {
                count++
                if (amoeba_can_grow_from(c, x, y)) {
                    growable++
                }
            }
        }
    }
    c.amoeba_count = count
    return if (count == 0)
    if (count >= AMOEBA_MAX) {
        replace_all(c, Tile.amoeba, Tile.boulder)
        c.amoeba_rocks = true
        return
    }
    if (growable == 0) {
        replace_all(c, Tile.amoeba, Tile.diamond)
        c.amoeba_gems = true
        return
    }
    for (y in range(c.rows)) {
        for (x in range(c.cols)) {
            if (cave_at(c, x, y) == Tile.amoeba && amoeba_can_grow_from(c, x, y)) {
                if (random_float(c.rng) < c.amoeba_grow) {
                    grow_amoeba_at(c, x, y)
                    c.amoeba_grew = true
                }
            }
        }
    }
}

def magic_pass(var c : Cave) {
    return if (c.magic_timer <= 0.0)
    c.magic_timer -= 1.0 / CAVE_HZ
    if (c.magic_timer <= 0.0) {
        c.magic_timer = 0.0
        c.magic_spent = true
        replace_all(c, Tile.magic_active, Tile.brick)
        replace_all(c, Tile.magic, Tile.brick)
    }
}

def exit_pass(var c : Cave) {
    return if (c.diamonds_need <= 0 || c.diamonds_got < c.diamonds_need)
    var opened = false
    for (i in range(length(c.cells))) {
        if (c.cells[i] == Tile.exit_closed) {
            c.cells[i] = Tile.exit_open
            opened = true
        }
    }
    if (opened) {
        c.exit_opened = true
    }
}

def cave_tick(var c : Cave) {
    reset_tick_events(c)
    c.movers |> resize(0)
    let n = c.cols * c.rows
    if (length(c.scanned) != n) {
        c.scanned |> resize(n)
    }
    for (i in range(n)) {
        c.scanned[i] = false
    }
    for (y in range(c.rows)) {
        for (x in range(c.cols)) {
            if (!cave_is_scanned(c, x, y)) {
                process_cell(c, x, y)
            }
        }
    }
    amoeba_pass(c)
    magic_pass(c)
    exit_pass(c)
}

def cave_apply_time(var c : Cave; dt : float) {
    return if (!c.alive || c.cleared || c.god)
    c.time_left -= dt
    if (c.time_left <= 0.0) {
        c.time_left = 0.0
        c.timed_out = true
        kill_rockford(c)
    }
}

def cave_set_input(var c : Cave; dir : int2; grab : bool) {
    c.wish_dir = dir
    c.grab = grab
    c.input_locked = false
    c.next_dir = int2(0, 0)
    c.next_grab = false
}

def cave_lock_input(var c : Cave) {
    c.input_locked = true
}

def cave_consume_input(var c : Cave) {
    c.input_locked = false
    c.wish_dir = c.next_dir
    c.grab = c.next_grab
    c.next_dir = int2(0, 0)
    c.next_grab = false
}

def cave_latch_input(var c : Cave; dir : int2; grab : bool) {
    if (dir.x != 0 || dir.y != 0) {
        if (c.input_locked) {
            if (dir.x != c.wish_dir.x || dir.y != c.wish_dir.y) {
                c.next_dir = dir
            }
        } else {
            c.wish_dir = dir
        }
    }
    if (grab) {
        if (c.input_locked) {
            c.next_grab = true
        } else {
            c.grab = true
        }
    }
}

def cave_peek_step(c : Cave; dir : int2) : int2 {
    let p = c.rockford
    if (!c.alive || (dir.x == 0 && dir.y == 0)) {
        return p
    }
    let nx = p.x + dir.x
    let ny = p.y + dir.y
    if (!in_bounds(c, nx, ny)) {
        return p
    }
    let dest = cave_at(c, nx, ny)
    if (dest == Tile.empty || dest == Tile.dirt || is_gem_tile(dest) || dest == Tile.exit_open) {
        return int2(nx, ny)
    }
    return p
}
