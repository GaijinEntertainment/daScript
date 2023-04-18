module sound_names

require files
require app

enum SoundName
/*
[I] sound 0 offset 0x116a00 of size 4096
[I] sound 1 offset 0x117a00 of size 1900
*/
    DOGBARK =                2
// [I] sound 2 offset 0x118200 of size 3793
    OPENDOORSND =            3
/*
[I] sound 3 offset 0x119200 of size 4096
[I] sound 4 offset 0x11a200 of size 4096
[I] sound 5 offset 0x11b200 of size 4096
[I] sound 6 offset 0x11c200 of size 4096
[I] sound 7 offset 0x11d200 of size 3611
*/
    CLOSEDOORSND =           8
/*
[I] sound 8 offset 0x11e200 of size 4096
[I] sound 9 offset 0x11f200 of size 4096
[I] sound 10 offset 0x120200 of size 1830
*/
// untested
    ATKMACHINEGUNSND =       11
// [I] sound 11 offset 0x120a00 of size 3600
    ATKPISTOLSND =           12
// [I] sound 12 offset 0x121a00 of size 3605
    ATKGATLINGSND =          13
// [I] sound 13 offset 0x122a00 of size 4065
	SCHUTZADSND =            14
/*
[I] sound 14 offset 0x123a00 of size 4096
[I] sound 15 offset 0x124a00 of size 399
*/
	GUTENTAGSND =            16
/*
[I] sound 16 offset 0x124c00 of size 4096
[I] sound 17 offset 0x125c00 of size 2238
*/
	MUTTISND =               18
/*
[I] sound 18 offset 0x126600 of size 4096
[I] sound 19 offset 0x127600 of size 1685
*/
	BOSSFIRESND =            20                 // note: this sound keeps going, even though its 4096 bytes long
/*
[I] sound 20 offset 0x127e00 of size 4096
[I] sound 21 offset 0x128e00 of size 4096       // this sounds like gun shot
[I] sound 22 offset 0x129e00 of size 2524
*/
	SSFIRESND =              23
/*
[I] sound 23 offset 0x12a800 of size 4096
[I] sound 24 offset 0x12b800 of size 1314
*/
    DEATHSCREAM1SND =        25
// [I] sound 25 offset 0x12be00 of size 2850
    DEATHSCREAM2SND =        26
//[I] sound 26 offset 0x12ca00 of size 3750
    DEATHSCREAM3SND =        27
// [I] sound 27 offset 0x12da00 of size 2215
    TAKEDAMAGESND =          28
/*
[I] sound 28 offset 0x12e400 of size 4096
[I] sound 29 offset 0x12f400 of size 4096
[I] sound 30 offset 0x130400 of size 4096
[I] sound 31 offset 0x131400 of size 4096
[I] sound 32 offset 0x132400 of size 4096
[I] sound 33 offset 0x133400 of size 4096
[I] sound 34 offset 0x134400 of size 187
*/
    PUSHWALLSND =            35         // test this?
/*
[I] sound 35 offset 0x134600 of size 3057
*/
    AHHHGSND =               36
/*
[I] sound 36 offset 0x135200 of size 4096
[I] sound 37 offset 0x136200 of size 1536
*/
    DIESND =                 38
/*
[I] sound 38 offset 0x136800 of size 4096
[I] sound 39 offset 0x137800 of size 4096
[I] sound 40 offset 0x138800 of size 2757
*/
    EVASND =                 41
/*
[I] sound 41 offset 0x139400 of size 4096
[I] sound 42 offset 0x13a400 of size 4096
[I] sound 43 offset 0x13b400 of size 4096
[I] sound 44 offset 0x13c400 of size 1578
*/
    LEBENSND =               45
/*
[I] sound 45 offset 0x13cc00 of size 4096
[I] sound 46 offset 0x13dc00 of size 1778
*/
    NAZIFIRESND =            47
/*
[I] sound 47 offset 0x13e400 of size 4096
[I] sound 48 offset 0x13f400 of size 4096
[I] sound 49 offset 0x140400 of size 623
*/
    SLURPIESND =             50
/*
[I] sound 50 offset 0x140800 of size 4096
[I] sound 51 offset 0x141800 of size 4096
[I] sound 52 offset 0x142800 of size 903
*/
    TOT_HUNDSND =            53
/*
[I] sound 53 offset 0x142c00 of size 4096
[I] sound 54 offset 0x143c00 of size 1593
*/
    MEINGOTTSND =            55
/*
[I] sound 55 offset 0x144400 of size 4096       // this sounds like vomit, its 4096 bytes long
[I] sound 56 offset 0x145400 of size 4096
[I] sound 57 offset 0x146400 of size 4096
[I] sound 58 offset 0x147400 of size 4096
[I] sound 59 offset 0x148400 of size 4096
[I] sound 60 offset 0x149400 of size 4096
[I] sound 61 offset 0x14a400 of size 1
*/
    SCHABBSHASND =           62
/*
[I] sound 62 offset 0x14a600 of size 4096
[I] sound 63 offset 0x14b600 of size 4096
[I] sound 64 offset 0x14c600 of size 4096
[I] sound 65 offset 0x14d600 of size 918
*/
    HITLERHASND =            66
/*
[I] sound 66 offset 0x14da00 of size 4096
[I] sound 67 offset 0x14ea00 of size 4096
[I] sound 68 offset 0x14fa00 of size 4096
[I] sound 69 offset 0x150a00 of size 2561
*/
    SPIONSND =               70
/*
[I] sound 70 offset 0x151600 of size 4096
[I] sound 71 offset 0x152600 of size 1041
*/
    NEINSOVASSND =           72
/*
[I] sound 72 offset 0x152c00 of size 4096
[I] sound 73 offset 0x153c00 of size 4096
[I] sound 74 offset 0x154c00 of size 154
*/
    DOGATTACKSND =           75
/*
[I] sound 75 offset 0x154e00 of size 4096
[I] sound 76 offset 0x155e00 of size 341
*/
    LEVELDONESND =           77
/*
[I] sound 77 offset 0x156000 of size 4096
[I] sound 78 offset 0x157000 of size 472
*/
    MECHSTEPSND =            79
/*
[I] sound 79 offset 0x157200 of size 4096
[I] sound 80 offset 0x158200 of size 1253
*/
    YEAHSND =               81
/*
[I] sound 81 offset 0x158800 of size 4096
[I] sound 82 offset 0x159800 of size 2906
*/
    SCHEISTSND =             83
/*
[I] sound 83 offset 0x15a400 of size 4096
[I] sound 84 offset 0x15b400 of size 2928
*/
    DEATHSCREAM4SND =        85
/*
[I] sound 85 offset 0x15c000 of size 4096
[I] sound 86 offset 0x15d000 of size 3508
*/
    DEATHSCREAM5SND =        87
/*
[I] sound 87 offset 0x15de00 of size 4096
[I] sound 88 offset 0x15ee00 of size 248
*/
    DONNERSND =              89                     // EPISODE 4 BOSS DIE
/*
[I] sound 89 offset 0x15f000 of size 4096
[I] sound 90 offset 0x160000 of size 4096
[I] sound 91 offset 0x161000 of size 1352
*/
    EINESND =                92                     // EPISODE 4 BOSS SIGHTING
/*
[I] sound 92 offset 0x161600 of size 4096
[I] sound 93 offset 0x162600 of size 4096
[I] sound 94 offset 0x163600 of size 4096
[I] sound 95 offset 0x164600 of size 4096
[I] sound 96 offset 0x165600 of size 1064
*/
    ERLAUBENSND =            97                     // EPISODE 6 BOSS SIGHTING
/*
[I] sound 97 offset 0x165c00 of size 4096
[I] sound 98 offset 0x166c00 of size 4096
[I] sound 99 offset 0x167c00 of size 4096
[I] sound 100 offset 0x168c00 of size 4096
[I] sound 101 offset 0x169c00 of size 4096
[I] sound 102 offset 0x16ac00 of size 3892
*/
    DEATHSCREAM6SND =        103                    // FART
//[I] sound 103 offset 0x16bc00 of size 3030
    DEATHSCREAM7SND =        104                    // GASP
/*
[I] sound 104 offset 0x16c800 of size 4096
[I] sound 105 offset 0x16d800 of size 2204
*/
    DEATHSCREAM8SND =        106                    // GUH-BOY
/*
[I] sound 106 offset 0x16e200 of size 4096
[I] sound 107 offset 0x16f200 of size 1148
*/
	DEATHSCREAM9SND =        108                    // AH GEEZ!
/*
[I] sound 108 offset 0x16f800 of size 4096
[I] sound 109 offset 0x170800 of size 2713
*/
    KEINSND =                110                    // EPISODE 5 BOSS SIGHTING
/*
[I] sound 110 offset 0x171400 of size 4096
[I] sound 111 offset 0x172400 of size 4096
[I] sound 112 offset 0x173400 of size 370
*/
    MEINSND =                113                    // EPISODE 6 BOSS DIE
/*
[I] sound 113 offset 0x173600 of size 4096
[I] sound 114 offset 0x174600 of size 4096
[I] sound 115 offset 0x175600 of size 2856
*/
    ROSESND =                116                    // EPISODE 5 BOSS DIE
/*
[I] sound 116 offset 0x176200 of size 4096
[I] sound 117 offset 0x177200 of size 4096
[I] sound 118 offset 0x178200 of size 4096
[I] sound 119 offset 0x179200 of size 211
*/
/*
[I] sound 120 offset 0x179400 of size 184
*/

def get_sound_data ( name:SoundName; max_seq:int=INT_MAX )
    var index = int(name)
    var data : array<float>
    while index < length(Sounds)
        let next = Sounds[index].data |> length
        let cur = data |> length
        data |> reserve ( data|>length + next )
        for i in Sounds[index].data
            data |> push(float(i)/128.0-1.0)
        if next != 4096
            break
        index ++
    return <- data

def public play_game_sound_2d ( name:SoundName; xy:float2; r:float; rate:int=SOUND_SAMPLE_RATE )
    play_app_sound_2d(xy, r, get_sound_data(name), rate)

def public play_game_sound ( name:SoundName; rate:int=SOUND_SAMPLE_RATE )
    play_app_sound(get_sound_data(name), rate)