options gen2
options persistent_heap
options indenting = 4

module levels public

// The boards, drawn as they play: one character per brick cell, top row furthest from the
// paddle. `1`..`5` are the colour rows and their points, `#` takes two hits, `X` never breaks
// and never counts toward the clear, `?` always drops a capsule, `.` is air.

struct public LevelDef {
    name  : string
    speed : float           //!< ball speed multiplier for the board
    rows  : array<string>   //!< the stencil, at most nine cells wide
}

def private add_level(var levels : array<LevelDef>; name : string; speed : float; rows : array<string>) {
    var lv = LevelDef(name = name, speed = speed)
    lv.rows := rows
    levels |> emplace(lv)
}

def public level_defs() : array<LevelDef> {
    var levels : array<LevelDef>
    add_level(levels, "THE WALL", 1.0, [
        "111111111",
        "222222222",
        "333333333",
        "444444444",
        "555555555"])
    add_level(levels, "PYRAMID", 1.05, [
        "....1....",
        "...222...",
        "..33333..",
        ".4444444.",
        "555555555"])
    add_level(levels, "CHECKERS", 1.1, [
        "1.2.3.2.1",
        ".2.3.4.3.",
        "3.4.5.4.3",
        ".4.5.#.5.",
        "5.#.X.#.5"])
    add_level(levels, "DIAMOND", 1.15, [
        "....#....",
        "...111...",
        "..22X22..",
        ".333X333.",
        "..44X44..",
        "...555...",
        "....#...."])
    add_level(levels, "GATES", 1.2, [
        "11..X..11",
        "22..X..22",
        "33.###.33",
        "44.....44",
        "5555?5555"])
    add_level(levels, "INVADER", 1.25, [
        "..1...1..",
        "...1.1...",
        "..22222..",
        ".22.2.22.",
        "333333333",
        "3.33333.3",
        "4.4...4.4",
        "...#.#..."])
    add_level(levels, "SPIRAL", 1.3, [
        "333333333",
        "........3",
        "3333333.3",
        "3.....3.3",
        "3.444.3.3",
        "3.4?4.3.3",
        "3.4...3.3",
        "3.44444.3",
        "3.......3"])
    add_level(levels, "FORTRESS", 1.4, [
        "XXX...XXX",
        "X#######X",
        "X#55?55#X",
        "X#5###5#X",
        "X#5#?#5#X",
        "X#5###5#X",
        "X#55555#X",
        ".#######."])
    return <- levels
}
