require app_opengl_glfw
require rast2d

require daslib/match
require daslib/safe_addr

require math
require fio
require stbimage
require strings

require llvm/llvm_jit

def set_debug_status_string(str)
    print("status: {str}\n")

var MapSize = 1723
var [[transient]] MapData = [[int[1723]
    412; 1; 15; 12; 49; 1; 3; 12; -1; 10; 5; 12; -1; 10; 5; 12; 49; 1; -2; 12; 142; 9; 0; -1; 142; 6; 12; 24; 1; 2; 156; 2; 159; 9; 1; 2; 2; -1; 1; 2; 2; -5; 3; 1; 2; 1; 10; 2; 0; -4; 132; 0; 178; 132; 2; 0; -1; 132; 2; 0; -1; 10; 5; 12; 24; 1; 2; 156; -2; 161;
    159; 7; 1; -1; 2; 10; 0; -1; 12; 11; 0; -1; 12; 2; 159; -1; 160; 2; 12; 24; 1; 2; 156; 2; 161; -1; 0; 7; 1; -2; 0; 144; 4; 0; -1; 144; 3; 0; -1; 90; 5; 0; -1; 134; 4; 0; -7; 178; 90; 0; 180; 159; 10; 12; 24; 1; 2; 156; -1; 161; 2; 0; 6; 1; -1; 2; 10; 0; -1;
    12; 11; 0; -1; 12; 3; 160; 2; 12; 24; 1; -8; 2; 6; 1; 2; 4; 2; 1; 6; 4; 1; 3; 0; 3; 1; -1; 3; 2; 1; -2; 2; 11; 11; 0; -1; 11; 5; 12; 23; 1; -8; 2; 0; 159; 160; 138; 0; 138; 160; 2; 0; -1; 2; 2; 1; 3; 0; 7; 1; -1; 12; 11; 0; 6; 12; 23; 1; -3; 3; 0; 133; 3;
    0; -7; 159; 0; 133; 178; 3; 1; 2; 3; 0; -2; 1; 2; 5; 1; 3; 12; -1; 10; 2; 12; -1; 91; 2; 12; -1; 10; 4; 12; 28; 1; 2; 0; -1; 184; 6; 0; -1; 2; 3; 0; -1; 182; 3; 0; 5; 1; 5; 12; 3; 0; 6; 12; 27; 1; -2; 2; 169; 3; 0; -3; 132; 0; 178; 2; 0; -1; 90; 3; 0; -1;
    144; 3; 0; 7; 1; 3; 12; -3; 0; 178; 0; 6; 12; 22; 1; 4; 2; 2; 1; 9; 0; -1; 1; 5; 0; -2; 182; 0; 7; 1; 2; 12; -6; 11; 0; 144; 0; 11; 12; 27; 1; -8; 2; 1; 2; 1; 3; 161; 133; 184; 4; 0; -9; 133; 0; 3; 1; 2; 1; 6; 1; 2; 2; 1; -1; 2; 5; 1; 3; 12; 3; 0; 2; 12;
    26; 1; -4; 2; 0; 155; 165; 2; 1; 2; 161; 6; 0; -1; 182; 11; 1; 7; 12; 3; 0; 4; 12; 25; 1; -7; 0; 144; 0; 2; 1; 2; 6; 2; 1; -1; 91; 2; 1; -2; 6; 2; 11; 1; 7; 12; 3; 0; 4; 12; 25; 1; 3; 0; 5; 1; 3; 0; -1; 2; 13; 1; 2; 12; 2; 0; -2; 10; 184; 2; 0; -1; 144; 2;
    0; -1; 146; 2; 12; 24; 1; -3; 2; 1; 91; 5; 1; -5; 3; 0; 144; 0; 3; 13; 1; 2; 12; -2; 156; 0; 3; 12; 3; 0; 4; 12; 25; 1; 3; 0; 2; 1; -1; 2; 2; 1; 3; 0; -1; 2; 13; 1; 2; 12; -2; 156; 155; 3; 12; 3; 0; 4; 12; 25; 1; -4; 0; 144; 0; 2; 2; 0; -2; 154; 1; 3; 0;
    14; 1; 2; 12; -2; 157; 155; 3; 12; 3; 0; 4; 12; 25; 1; 5; 0; -7; 144; 180; 3; 0; 144; 0; 3; 13; 1; 6; 12; -6; 10; 0; 144; 0; 10; 12; 27; 1; 3; 0; -1; 1; 2; 0; -5; 160; 2; 0; 182; 0; 14; 1; 7; 12; 3; 0; 2; 12; 14; 1; 12; 9; -1; 2; 3; 0; -4; 1; 2; 1; 2; 2; 1;
    -1; 91; 2; 1; -1; 2; 18; 1; -5; 12; 10; 91; 10; 12; 7; 2; 8; 1; -1; 9; 2; 8; -1; 9; 3; 8; 3; 9; -9; 8; 9; 3; 0; 144; 0; 3; 1; 184; 8; 0; 11; 1; -7; 2; 1; 2; 1; 3; 2; 141; 3; 0; -5; 141; 1; 3; 1; 2; 2; 1; -1; 2; 8; 1; -3; 9; 8; 154; 3; 0; -1; 130; 3; 165;
    -3; 8; 9; 2; 3; 0; -4; 1; 2; 0; 141; 5; 0; -3; 141; 0; 3; 11; 1; -3; 141; 0; 178; 11; 0; -1; 141; 2; 2; 9; 9; -1; 8; 7; 0; -1; 165; 2; 9; -1; 1; 3; 0; -2; 1; 2; 2; 0; -1; 144; 3; 0; -1; 144; 2; 0; 11; 1; -1; 6; 15; 0; -2; 6; 9; 3; 8; -1; 9; 3; 8; 2; 9; -1;
    8; 5; 0; -1; 180; 2; 0; -3; 8; 9; 1; 4; 0; -1; 1; 9; 0; -1; 2; 11; 1; 16; 0; -1; 9; 9; 0; -1; 8; 8; 0; 2; 9; -1; 1; 4; 0; -3; 90; 0; 180; 7; 0; -1; 3; 10; 1; -1; 2; 2; 0; -1; 134; 4; 0; -1; 134; 3; 0; -2; 180; 134; 3; 0; -1; 90; 9; 0; -1; 90; 8; 0; -3; 5;
    9; 1; 4; 0; -1; 1; 9; 0; 11; 1; -1; 2; 16; 0; -3; 8; 0; 229; 7; 0; -1; 9; 8; 0; 2; 9; -1; 1; 3; 0; 2; 1; 2; 0; -1; 144; 3; 0; -1; 144; 2; 0; 2; 2; 9; 1; -1; 6; 15; 0; -1; 6; 3; 8; 4; 0; 2; 9; 2; 8; -11; 227; 0; 143; 154; 0; 143; 0; 180; 8; 9; 2; 3; 0; 2; 1;
    -2; 0; 141; 5; 0; -3; 141; 0; 3; 10; 1; -2; 2; 141; 13; 0; -1; 141; 3; 2; 2; 8; 2; 0; 4; 8; -1; 9; 8; 0; 2; 9; -1; 1; 3; 0; 2; 1; 7; 0; -2; 182; 0; 13; 1; -4; 4; 1; 2; 1; 5; 0; -10; 1; 2; 4; 1; 2; 1; 2; 1; 8; 9; 2; 0; -1; 9; 2; 8; -2; 9; 8; 6; 0; -11; 154;
    0; 8; 9; 4; 0; 144; 0; 4; 1; 2; 2; 1; -2; 2; 91; 2; 1; 2; 2; 17; 1; 2; 8; -2; 91; 9; 2; 8; 6; 2; -3; 1; 8; 9; 2; 0; 2; 8; 4; 9; -7; 8; 166; 9; 8; 167; 9; 8; 2; 9; -1; 1; 3; 0; -1; 2; 4; 1; 3; 0; 19; 1; 2; 8; 3; 0; -2; 9; 8; 7; 1; 2; 8; 2; 0; -2; 9; 8; 3; 9;
    -1; 8; 9; 9; -1; 1; 3; 0; 4; 1; -1; 2; 3; 0; -1; 2; 18; 1; -7; 8; 9; 0; 144; 0; 9; 8; 7; 1; -2; 8; 9; 2; 0; 2; 8; 13; 9; -1; 2; 3; 0; -5; 1; 178; 0; 131; 1; 3; 0; 19; 1; 2; 8; 3; 0; -2; 9; 8; 7; 1; 2; 8; -3; 0; 182; 9; 12; 8; 3; 1; 5; 0; -7; 144; 0; 3; 0;
    144; 0; 3; 18; 1; -2; 8; 9; 3; 0; 2; 8; 7; 1; -2; 8; 9; 2; 0; 2; 8; -11; 9; 0; 8; 0; 9; 0; 9; 136; 8; 9; 8; 2; 1; -1; 2; 3; 0; -2; 2; 154; 2; 0; -1; 2; 3; 0; -1; 2; 18; 1; -2; 8; 9; 3; 0; -2; 9; 8; 7; 1; 2; 8; 3; 0; -1; 8; 5; 0; -1; 229; 3; 0; 2; 8; 2; 1;
    -4; 2; 0; 144; 0; 2; 1; -1; 2; 2; 1; 3; 0; -1; 2; 4; 1; -1; 2; 13; 1; 2; 8; -3; 0; 144; 0; 2; 8; 7; 1; 2; 8; 3; 0; -1; 90; 8; 0; -3; 229; 5; 8; 3; 1; -14; 0; 178; 0; 2; 1; 2; 1; 2; 0; 182; 0; 1; 2; 1; 2; 2; -8; 3; 1; 2; 1; 6; 2; 1; 2; 6; 1; -7; 8; 9; 0;
    178; 0; 8; 9; 7; 1; -6; 8; 9; 0; 182; 0; 8; 3; 0; -1; 233; 4; 0; -1; 153; 2; 8; 2; 1; -1; 2; 16; 0; -1; 2; 5; 0; -3; 180; 3; 21; 5; 1; 2; 8; 3; 0; -2; 9; 8; 7; 1; 2; 8; -1; 9; 3; 8; -11; 9; 0; 8; 0; 8; 0; 8; 0; 8; 9; 8; 2; 1; -1; 2; 5; 0; -1; 144; 7; 0; -1;
    144; 2; 0; -1; 90; 3; 0; -1; 180; 2; 0; -3; 100; 0; 21; 4; 1; -2; 8; 9; 3; 0; -2; 9; 8; 7; 1; 17; 8; 3; 1; 16; 0; -1; 1; 5; 0; -3; 180; 3; 21; 5; 1; -5; 8; 9; 0; 144; 0; 2; 8; 27; 1; 2; 2; -1; 1; 2; 2; -1; 1; 2; 2; -2; 1; 4; 3; 1; -1; 2; 4; 1; -5; 2; 1; 6;
    1; 2; 2; 1; -2; 2; 1; 5; 8; 3; 0; -1; 9; 6; 8; 31; 1; -2; 21; 3; 2; 0; -1; 1; 2; 0; -3; 1; 0; 155; 7; 1; 2; 8; -3; 9; 8; 9; 2; 8; -1; 91; 5; 8; -1; 9; 2; 8; 30; 1; -3; 21; 0; 100; 2; 0; -1; 1; 2; 0; -3; 1; 0; 2; 6; 1; -4; 8; 9; 149; 154; 2; 0; -1; 8; 3; 0;
    -1; 8; 4; 0; -2; 9; 8; 31; 1; -2; 21; 3; 5; 0; 9; 1; 2; 8; 4; 0; -7; 90; 0; 144; 0; 90; 0; 139; 2; 0; -2; 9; 8; 47; 1; 2; 8; 4; 0; -1; 8; 3; 0; -1; 8; 4; 0; -2; 9; 8; 34; 1; 2; 0; 11; 1; -2; 8; 9; 4; 0; -1; 9; 3; 0; -1; 9; 4; 0; -2; 9; 8; 34; 1; -2; 0; 163;
    11; 1; 3; 8; -1; 9; 2; 8; -1; 9; 3; 0; 7; 8; 47; 1; 2; 8; 4; 0; -1; 8; 3; 0; -1; 8; 4; 0; 2; 8; 47; 1; -2; 8; 9; 3; 0; -7; 223; 90; 0; 144; 0; 90; 154; 2; 0; -3; 149; 9; 8; 47; 1; 2; 8; 4; 0; -1; 8; 3; 0; -1; 8; 4; 0; 2; 8; 47; 1; 2; 8; 2; 9; 2; 8; -1; 9;
    3; 0; -1; 9; 2; 8; -1; 9; 3; 8; 47; 1; 2; 8; 13; 0; 2; 8; 47; 1; -2; 8; 9; 2; 0; -1; 144; 3; 0; -1; 144; 3; 0; -3; 144; 180; 156; 2; 8; 47; 1; 2; 8; -1; 184; 5; 0; -1; 154; 6; 0; 2; 8; 47; 1; 2; 8; -13; 5; 9; 8; 5; 8; 9; 7; 8; 9; 5; 9; 8; 5; 2; 8; 21; 1]]

var [[transient]] Obstacles = [[int[50] 0; 0; 0; 1; 1; 1; 0; 1; 0; 1; 1; 0; 1; 1; 1; 1; 0; 0; 1; 1; 1; 0; 0; 0; 1; 0; 0; 0; 0; 0; 0; 0; 0; 0; 0; 0; 0; 1; 1; 1; 0; 1; 0; 0; 0; 0; 0; 1; 1; 0]]

var [[transient]] NextFrames = [[int[366]
    0; 1; 2; 3; 4; 5; 6; 7; 16; 17; 18; 19; 20; 21;
    22; 23; 24; 25; 26; 27; 28; 29; 30; 31; 32; 33; 34; 35; 36; 37; 38; 39; 8; 9; 10; 11; 12; 13; 14; 15; 41; 42; 43; 45; 0; 45;
    47; 48; 46; 57; 58; 59; 60; 61; 62; 63; 64; 65; 66; 67; 68; 69; 70; 71; 72; 73; 74; 75; 76; 77; 78; 79; 80; 49; 50; 51; 52; 53;
    54; 55; 56; 49; 83; 84; 84; 86; 87; 85; 88; 89; 90; 91; 92; 93; 94; 95; 104; 105; 106; 107; 108; 109; 110; 111; 112; 113; 114; 115; 116; 117;
    118; 119; 120; 121; 122; 123; 124; 125; 126; 127; 96; 97; 98; 99; 100; 101; 102; 103; 129; 130; 131; 133; 88; 133; 135; 136; 135; 137; 138; 139; 140; 141;
    142; 143; 144; 153; 154; 155; 156; 157; 158; 159; 160; 161; 162; 163; 164; 165; 166; 167; 168; 169; 170; 171; 172; 173; 174; 175; 176; 145; 146; 147; 148; 149;
    150; 151; 152; 178; 179; 180; 182; 137; 183; 183; 185; 186; 187; 184; 188; 189; 190; 191; 192; 193; 194; 195; 204; 205; 206; 207; 208; 209; 210; 211; 212; 213;
    214; 215; 216; 217; 218; 219; 220; 221; 222; 223; 224; 225; 226; 227; 196; 197; 198; 199; 200; 201; 202; 203; 229; 230; 231; 233; 188; 234; 234; 236; 237; 235;
    239; 238; 241; 240; 243; 242; 245; 244; 247; 248; 249; 246; 251; 252; 251; 253; 255; 256; 253; 258; 259; 260; 257; 262; 261; 264; 265; 266; 266; 268; 269; 270;
    267; 272; 273; 274; 271; 275; 277; 276; 279; 280; 281; 282; 283; 283; 285; 286; 287; 284; 289; 290; 289; 291; 293; 294; 295; 296; 297; 298; 295; 300; 301; 300;
    302; 304; 305; 306; 307; 308; 309; 302; 311; 312; 313; 310; 315; 314; 317; 318; 319; 319; 320; 321; 322; 323; 324; 325; 326; 327; 328; 329; 330; 331; 332; 333;
    334; 336; 337; 338; 335; 340; 341; 340; 342; 344; 345; 342; 347; 348; 349; 346; 351; 352; 353; 350; 355; 356; 357; 357; 359; 360; 361; 362; 363; 364; 365; 365]]

struct STile
    ID : int
    Visible : bool
    DoorState : int
    DoorSlide : float

var MapWidth = 64
var MapHeight = 64
var [[transient]] Map : array<STile>

var [[transient]] DepthBuffer : array<float>

var [[transient]] Texture : Texture

let MASK_COLOR = 0xFF980088

struct SPlayer
    Position : float2
    Direction : float
    Size : float
    Health : float

var Player : SPlayer

struct SWeapon
    Type : int
    Shoot : bool
    Animation : float

var Weapon : SWeapon

struct SObject
    ID : int
    Position : float2
    Animation : float
    Enemy : int

var [[transient]] Objects : array<SObject>

enum EEnemyType
    Invalid

    Guard
    Dog
    SS
    Mutant
    Officer

    Blinky
    Pinky
    Clyde
    Inky

    Boss
    Schabbs
    FakeHitler
    MechaHitler
    Hitler
    Gift
    Gretel
    Fat

struct SEnemy
    Object : int
    Type : EEnemyType
    Health : float

var [[transient]] Enemies : array<SEnemy>

var DoorUse = false

var FlashState = false
var FlashPhase = 0.0
var FlashColor = 0x000000

def UncompressMap()
    clear(Map)

    var Tile : STile
    Tile.ID = 0
    Tile.Visible = false
    Tile.DoorState = 0
    Tile.DoorSlide = 0.0f

/*
    let level = 0
    for y in 0..64
        for x in 0..64
            Tile.ID = int(Levels[level].plane0[x + y*64])
            if Tile.ID >= 106 && Tile.ID <= 143
                Tile.ID = 0
            push(Map, Tile)
*/

    var X = 0
    var Y = 0
    var Index = 0

    while Index < MapSize
        var Count = MapData[Index]
        Index = Index + 1

        var Step = -1
        if Count < 0
            Step = 1

        if Count > 0
            Tile.ID = MapData[Index]
            Index = Index + 1

        while Count != 0
            if Count < 0
                Tile.ID = MapData[Index]
                Index = Index + 1

            push(Map, Tile)

            X = X + 1
            if X == MapWidth
                X = 0
                Y = Y + 1

            Count = Count + Step

def InitPlayer()
    Player.Position = float2(29.5f, 57.5f)
    Player.Direction = 0.0f
    Player.Size = 0.2f
    Player.Health = 100.0f

def InitWeapon()
    Weapon.Type = 1
    Weapon.Shoot = false
    Weapon.Animation = 0.0f

/*
000 - 063	Walls
090 - 091	Regular unlocked door (oriented)
092 - 093	Gold-locked door (oriented)
094 - 095	Silver-locked door (oriented)
100 - 101	Elevator door (oriented)
106 - 143	Walkable tile (room)
*/

def InitObjects()
    clear(Objects)
    clear(Enemies)

    var Index = 0
    for Y in range(0, MapHeight)
        for X in range(0, MapWidth)
            var Object : SObject
            Object.ID = 0
            Object.Position = float2(float(X) + 0.5f, float(Y) + 0.5f)
            Object.Animation = 0.0f

            var ID = Map[Y * MapWidth + X].ID
            if ID >= 128
                Map[Y * MapWidth + X].ID = 0
                Object.ID = ID

            /*
            var ID = int(Levels[0].plane1[Y*64+X])
            if ID == 0
                ID = int(Levels[0].plane1[Y*64+X])
                if ID >= 106 && ID <= 143
                    pass
                else
                    ID = 0
            */

            if ID >= 178 && ID <= 543
                var Enemy : SEnemy
                Enemy.Object = Y * MapWidth + X
                Enemy.Type = EEnemyType Invalid
                Enemy.Health = 100.0f

                if ID >= 178 && ID <= 226
                    Enemy.Type = EEnemyType Guard
                if ID >= 227 && ID <= 265
                    Enemy.Type = EEnemyType Dog
                if ID >= 266 && ID <= 314
                    Enemy.Type = EEnemyType SS
                if ID >= 315 && ID <= 365
                    Enemy.Type = EEnemyType Mutant
                if ID >= 366 && ID <= 415
                    Enemy.Type = EEnemyType Officer

                if ID >= 416 && ID <= 417
                    Enemy.Type = EEnemyType Blinky
                if ID >= 418 && ID <= 419
                    Enemy.Type = EEnemyType Pinky
                if ID >= 420 && ID <= 421
                    Enemy.Type = EEnemyType Clyde
                if ID >= 422 && ID <= 423
                    Enemy.Type = EEnemyType Inky

                if ID >= 424 && ID <= 434
                    Enemy.Type = EEnemyType Boss
                if ID >= 435 && ID <= 444
                    Enemy.Type = EEnemyType Schabbs
                if ID >= 449 && ID <= 461
                    Enemy.Type = EEnemyType FakeHitler
                if ID >= 462 && ID <= 472
                    Enemy.Type = EEnemyType MechaHitler
                if ID >= 473 && ID <= 487
                    Enemy.Type = EEnemyType Hitler
                if ID >= 488 && ID <= 497
                    Enemy.Type = EEnemyType Gift
                if ID >= 513 && ID <= 523
                    Enemy.Type = EEnemyType Gretel
                if ID >= 524 && ID <= 535
                    Enemy.Type = EEnemyType Fat

                if Enemy.Type != EEnemyType Invalid
                    Object.Enemy = Index
                    Index = Index + 1

                    push(Enemies, Enemy)

            push(Objects, Object)

def GetIdleFrame(Enemy : SEnemy) : int
    if Enemy.Type == EEnemyType Invalid
        return -1

    var Object = Objects[Enemy.Object]
    var ID = Object.ID

    if Enemy.Type == EEnemyType Guard
        return 178
    if Enemy.Type == EEnemyType Dog
        return 227
    if Enemy.Type == EEnemyType SS
        return 266
    if Enemy.Type == EEnemyType Mutant
        return 315
    if Enemy.Type == EEnemyType Officer
        return 366

    if Enemy.Type == EEnemyType Boss
        return 424
    if Enemy.Type == EEnemyType Schabbs
        return 435
    if Enemy.Type == EEnemyType FakeHitler
        return 449
    if Enemy.Type == EEnemyType MechaHitler
        return 462
    if Enemy.Type == EEnemyType Hitler
        return 473
    if Enemy.Type == EEnemyType Gift
        return 488
    if Enemy.Type == EEnemyType Gretel
        return 513
    if Enemy.Type == EEnemyType Fat
        return 524

    return ID

def GetAttackDistance(Enemy : SEnemy) : float
    if Enemy.Type == EEnemyType Invalid
        return -1.0f

    var Object = Objects[Enemy.Object]
    var ID = Object.ID

    if Enemy.Type == EEnemyType Guard
        return 5.0f
    if Enemy.Type == EEnemyType Dog
        return 1.0f
    if Enemy.Type == EEnemyType SS
        return 8.0f
    if Enemy.Type == EEnemyType Mutant
        return 4.0f
    if Enemy.Type == EEnemyType Officer
        return 6.0f

    if Enemy.Type == EEnemyType Boss
        return 10.0f
    if Enemy.Type == EEnemyType Schabbs
        return 10.0f
    if Enemy.Type == EEnemyType FakeHitler
        return 10.0f
    if Enemy.Type == EEnemyType MechaHitler
        return 10.0f
    if Enemy.Type == EEnemyType Hitler
        return 10.0f
    if Enemy.Type == EEnemyType Gift
        return 10.0f
    if Enemy.Type == EEnemyType Gretel
        return 10.0f
    if Enemy.Type == EEnemyType Fat
        return 10.0f

    return -1.0f

def GetAttackFrame(Enemy : SEnemy) : int
    if Enemy.Type == EEnemyType Invalid
        return -1

    var Object = Objects[Enemy.Object]
    var ID = Object.ID

    if Enemy.Type == EEnemyType Guard
        return 224
    if Enemy.Type == EEnemyType Dog
        return 263
    if Enemy.Type == EEnemyType SS
        return 312
    if Enemy.Type == EEnemyType Mutant
        return 362
    if Enemy.Type == EEnemyType Officer
        return 413

    if Enemy.Type == EEnemyType Boss
        return 428
    if Enemy.Type == EEnemyType Schabbs
        return 439
    if Enemy.Type == EEnemyType FakeHitler
        return 458
    if Enemy.Type == EEnemyType MechaHitler
        return 466
    if Enemy.Type == EEnemyType Hitler
        return 477
    if Enemy.Type == EEnemyType Gift
        return 492
    if Enemy.Type == EEnemyType Gretel
        return 517
    if Enemy.Type == EEnemyType Fat
        return 528

    return ID

def IsAttack(Enemy : SEnemy) : bool
    if Enemy.Type == EEnemyType Invalid
        return false

    var Object = Objects[Enemy.Object]
    var ID = Object.ID

    if Enemy.Type == EEnemyType Guard && ID >= 224 && ID <= 226
        return true
    if Enemy.Type == EEnemyType Dog && ID >= 263 && ID <= 265
        return true
    if Enemy.Type == EEnemyType SS && ID >= 312 && ID <= 314
        return true
    if Enemy.Type == EEnemyType Mutant && ID >= 362 && ID <= 365
        return true
    if Enemy.Type == EEnemyType Officer && ID >= 413 && ID <= 415
        return true

    if Enemy.Type == EEnemyType Boss && ID >= 428 && ID <= 430
        return true
    if Enemy.Type == EEnemyType Schabbs && ID >= 439 && ID <= 440
        return true
    if Enemy.Type == EEnemyType FakeHitler && ID >= 458 && ID <= 458
        return true
    if Enemy.Type == EEnemyType MechaHitler && ID >= 466 && ID <= 468
        return true
    if Enemy.Type == EEnemyType Hitler && ID >= 477 && ID <= 479
        return true
    if Enemy.Type == EEnemyType Gift && ID >= 492 && ID <= 493
        return true
    if Enemy.Type == EEnemyType Gretel && ID >= 517 && ID <= 519
        return true
    if Enemy.Type == EEnemyType Fat && ID >= 528 && ID <= 531
        return true

    return false

def GetDamageFrame(Enemy : SEnemy) : int
    if Enemy.Type == EEnemyType Invalid
        return -1

    var Object = Objects[Enemy.Object]
    var ID = Object.ID

    if Enemy.Type == EEnemyType Guard
        return 222
    if Enemy.Type == EEnemyType Dog
        return 259
    if Enemy.Type == EEnemyType SS
        return 310
    if Enemy.Type == EEnemyType Mutant
        return 359
    if Enemy.Type == EEnemyType Officer
        return 410

    return ID

def GetDyingFrame(Enemy : SEnemy) : int
    if Enemy.Type == EEnemyType Invalid
        return -1

    var Object = Objects[Enemy.Object]
    var ID = Object.ID

    if Enemy.Type == EEnemyType Guard
        return 218
    if Enemy.Type == EEnemyType Dog
        return 260
    if Enemy.Type == EEnemyType SS
        return 306
    if Enemy.Type == EEnemyType Mutant
        return 355
    if Enemy.Type == EEnemyType Officer
        return 406

    if Enemy.Type == EEnemyType Boss
        return 431
    if Enemy.Type == EEnemyType Schabbs
        return 441
    if Enemy.Type == EEnemyType FakeHitler
        return 456
    if Enemy.Type == EEnemyType MechaHitler
        return 469
    if Enemy.Type == EEnemyType Hitler
        return 480
    if Enemy.Type == EEnemyType Gift
        return 494
    if Enemy.Type == EEnemyType Gretel
        return 520
    if Enemy.Type == EEnemyType Fat
        return 532

    return ID

def IsDying(Enemy : SEnemy) : bool
    if Enemy.Type == EEnemyType Invalid
        return false

    var Object = Objects[Enemy.Object]
    var ID = Object.ID

    if Enemy.Type == EEnemyType Guard && ID >= 218 && ID <= 223
        return true
    if Enemy.Type == EEnemyType Dog && ID >= 259 && ID <= 262
        return true
    if Enemy.Type == EEnemyType SS && ID >= 306 && ID <= 311
        return true
    if Enemy.Type == EEnemyType Mutant && ID >= 355 && ID <= 361
        return true
    if Enemy.Type == EEnemyType Officer && ID >= 406 && ID <= 412
        return true

    if Enemy.Type == EEnemyType Boss && ID >= 431 && ID <= 434
        return true
    if Enemy.Type == EEnemyType Schabbs && ID >= 441 && ID <= 444
        return true
    if Enemy.Type == EEnemyType FakeHitler && ID >= 456 && ID <= 461
        return true
    if Enemy.Type == EEnemyType MechaHitler && ID >= 469 && ID <= 472
        return true
    if Enemy.Type == EEnemyType Hitler && ID >= 480 && ID <= 487
        return true
    if Enemy.Type == EEnemyType Gift && ID >= 494 && ID <= 497
        return true
    if Enemy.Type == EEnemyType Gretel && ID >= 520 && ID <= 523
        return true
    if Enemy.Type == EEnemyType Fat && ID >= 532 && ID <= 535
        return true

    return false

def GetFirstDirection(ID : int) : int
    if ID >= 178 && ID <= 217
        return 178
    if ID >= 227 && ID <= 258
        return 227
    if ID >= 266 && ID <= 305
        return 266
    if ID >= 315 && ID <= 354
        return 315
    if ID >= 366 && ID <= 405
        return 366

    return -1

def IsBlock(X : int; Y : int) : bool
    if X < 0 || Y < 0 || X >= MapWidth || Y >= MapHeight
        return false

    var Tile = Map[Y * MapWidth + X];
    var ID = Tile.ID
    if ID == 0
        ID = Objects[Y * MapWidth + X].ID
        if ID == 0
            return false

    if ID >= 90 && ID <= 101
        if Tile.DoorSlide >= 1.0f
            return false

    if ID >= 128
        if ID <= 177
            if Obstacles[ID - 128] != 0
                return true
            else
                return false
        else
            if ID == 223 || ID == 262 || ID == 311 || ID == 361 || ID == 412
                return false
            elif ID == 431 || ID == 444 || ID == 461 || ID == 469 || ID == 480 || ID == 487 || ID == 497 || ID == 520 || ID == 535
                return false
            else
                return true

    return true

def IsDoor(X : int; Y : int) : bool
    if X < 0 || Y < 0 || X >= MapWidth || Y >= MapHeight
        return false

    var ID = Map[Y * MapWidth + X].ID
    if ID >= 90 && ID <= 101
        return true

    return false

def GetNextFrame(ID : int) : int
    if ID >= 178 && ID <= 543
        return NextFrames[ID - 178] + 178
    return ID

def IsVisible(PositionA : float2; PositionB : float2) : bool
    var Ray = PositionB - PositionA

    var MapX = int(PositionA.x)
    var MapY = int(PositionA.y)

    var StepX : int
    if Ray.x > 0.0f
        StepX = 1
    elif Ray.x < 0.0f
        StepX = -1
    else
        StepX = 0

    var StepY : int
    if Ray.y > 0.0f
        StepY = 1
    elif Ray.y < 0.0f
        StepY = -1
    else
        StepY = 0

    var Fraction = float2(PositionA.x - float(MapX), PositionA.y - float(MapY))

    var Distance : float2
    if Ray.x > 0.0f
        Distance.x = 1.0f - Fraction.x
    elif Ray.x < 0.0f
        Distance.x = Fraction.x
    else
        Distance.x = 0.0f

    if Ray.y > 0.0f
        Distance.y = 1.0f - Fraction.y
    elif Ray.y < 0.0f
        Distance.y = Fraction.y
    else
        Distance.y = 0.0f

    var Sign = float2(StepX, StepY)
    var Abs = Sign * Ray

    var Ratio = float2(0.0f, 0.0f)
    if Abs.x != 0.0f
        Ratio.x = Abs.y / Abs.x
    if Abs.y != 0.0f
        Ratio.y = Abs.x / Abs.y

    var Axis = float2(1.0f, 1.0f)
    if Abs.x >= Abs.y
        Axis.y = Ratio.y
    else
        Axis.x = Ratio.x

    var Next = float2(Distance.x * Axis.x, Distance.y * Axis.y)

    if Abs.x == 0.0f
        Next = float2(1.0f, 0.0f)
        Axis = float2(0.0f, 0.0f)

    if Abs.y == 0.0f
        Next = float2(0.0f, 1.0f)
        Axis = float2(0.0f, 0.0f)

    Distance = Distance - float2(1.0f, 1.0f)

    var Side : int
    var Empty : bool

    Empty = true
    while Empty
        if Next.x < Next.y
            Next.x = Next.x + Axis.x
            Distance.x = Distance.x + 1.0f
            if Distance.x > Abs.x
                return true
            MapX = MapX + StepX
            Side = 0
        else
            Next.y = Next.y + Axis.y
            Distance.y = Distance.y + 1.0f
            if Distance.y > Abs.y
                return true
            MapY = MapY + StepY
            Side = 1

        var ID = Map[MapY * MapWidth + MapX].ID
        Empty = ID == 0 || ID >= 128

        if ID >= 90 && ID <= 101
            var Slide = Map[MapY * MapWidth + MapX].DoorSlide
            var Vec = float2(MapX, MapY) - PositionA
            if Side == 0
                Vec.x = (Vec.x + 0.5f) * Sign.x
                var Open = Vec.x * Ratio.x * Sign.y - Vec.y
                if Open >= Slide && Open <= 1.0f
                    Distance.x = Vec.x
                else
                    Empty = true
            else
                Vec.y = (Vec.y + 0.5f) * Sign.y
                var Open = Vec.y * Ratio.y * Sign.x - Vec.x
                if Open >= Slide && Open <= 1.0f
                    Distance.y = Vec.y
                else
                    Empty = true

    if Side == 0
        if Distance.x <= Abs.x
            return false
    else
        if Distance.y <= Abs.y
            return false

    return true

def GetPlayerAngle(Position : float2) : int
    var Direction = Position - Player.Position
    var PlayerAngle = floori((8.0f * atan2(Direction.y, Direction.x) / (2.0f * PI)) + 0.5f)
    if PlayerAngle < 0
        PlayerAngle = 8 - ((-PlayerAngle) % 8)
    PlayerAngle = PlayerAngle % 8
    return PlayerAngle

def UpdatePlayer(Time : float)
    if Player.Health <= 0.0f
        return

    var TurnSpeed = Time * 2.0f
    var MoveSpeed = Time * 5.0f

    var Direction = float2(cos(Player.Direction), sin(Player.Direction))
    var Forward = float2(Direction.x, Direction.y)
    var Leftward = float2(Direction.y, -Direction.x)

    var Velocity = float2(0.0f, 0.0f)

    var Run = false
    if is_key_pressed(VK_LSHIFT) || is_key_pressed(VK_RSHIFT)
        Run = true

    if Run
        MoveSpeed = MoveSpeed * 2.0f
        TurnSpeed = TurnSpeed * 2.0f

    var Strafe = false
    if is_key_pressed(VK_LALT) || is_key_pressed(VK_RALT)
        Strafe = true

    if Strafe
        if is_key_pressed(VK_LEFT)
            Velocity = Velocity + Leftward * MoveSpeed
        if is_key_pressed(VK_RIGHT)
            Velocity = Velocity - Leftward * MoveSpeed
    else
        if is_key_pressed(VK_LEFT)
            Player.Direction = Player.Direction - TurnSpeed
        if is_key_pressed(VK_RIGHT)
            Player.Direction = Player.Direction + TurnSpeed

    if is_key_pressed(VK_UP)
        Velocity = Velocity + Forward * MoveSpeed
    if is_key_pressed(VK_DOWN)
        Velocity = Velocity - Forward * MoveSpeed

    var Position = Player.Position
    var Size = float2(Player.Size, Player.Size)

    var Min = Position - Size
    var Max = Position + Size

    var X1 = floori(Min.x)
    var Y1 = floori(Min.y)
    var X2 = ceili(Max.x)
    var Y2 = ceili(Max.y)

    var FlagX = false
    var FlagY = false

    for Y in range(Y1, Y2)
        for X in range(X1, X2)
            if !IsBlock(X, Y)
                continue

            var Block = float2(X, Y)
            if Block.x >= Position.x + Size.x || Block.y >= Position.y + Size.y || Block.x + 1.0f <= Position.x - Size.x || Block.y + 1.0f <= Position.y - Size.y
                continue

            var Stop : float2
            if Block.x + 0.5f > Position.x
                Stop.x = Block.x - Size.x
            else
                Stop.x = Block.x + 1.0f + Size.x
            if Block.y + 0.5f > Position.y
                Stop.y = Block.y - Size.y
            else
                Stop.y = Block.y + 1.0f + Size.y

            if abs(Stop.x - Position.x) <= abs(Stop.y - Position.y)
                if !IsBlock(int(Stop.x), Y)
                    FlagX = true
                elif !IsBlock(X, int(Stop.y))
                    FlagY = true
                elif !IsBlock(int(Stop.x), int(Stop.y))
                    FlagX = true
                    FlagY = true
            else
                if !IsBlock(X, int(Stop.y))
                    FlagY = true
                elif !IsBlock(int(Stop.x), Y)
                    FlagX = true
                elif !IsBlock(int(Stop.x), int(Stop.y))
                    FlagX = true
                    FlagY = true


            if FlagX
                if Position.x > Stop.x == Velocity.x > 0.0f
                    Velocity.x = 0.0f
                Position.x = Stop.x

            if FlagY
                if Position.y > Stop.y == Velocity.y > 0.0f
                    Velocity.y = 0.0f
                Position.y = Stop.y

            if FlagX || FlagY
                break

        if FlagX || FlagY
            break

    for Step in range(0, 2)
        if Velocity.x == 0.0f && Velocity.y == 0.0f
            break

        var Stop = float2(0.0f, 0.0f)
        var Move = 1.0f
        var Side = 0

        Min = Position - Size
        Max = Position + Size

        if Velocity.x < 0.0f
            Min.x = Min.x + Velocity.x
        else
            Max.x = Max.x + Velocity.x

        if Velocity.y < 0.0f
            Min.y = Min.y + Velocity.y
        else
            Max.y = Max.y + Velocity.y

        X1 = floori(Min.x)
        Y1 = floori(Min.y)
        X2 = ceili(Max.x)
        Y2 = ceili(Max.y)

        if X1 < 0
            X1 = 0
        if Y1 < 0
            Y1 = 0
        if X2 > MapWidth
            X2 = MapWidth
        if Y2 > MapHeight
            Y2 = MapHeight

        var ClipLines : array<float3>
        push(ClipLines, float3(1.0f, 0.0f, -Min.x))
        push(ClipLines, float3(0.0f, 1.0f, -Min.y))
        push(ClipLines, float3(-1.0f, 0.0f, Max.x))
        push(ClipLines, float3(0.0f, -1.0f, Max.y))

        if Velocity.x != 0.0f && Velocity.y != 0.0f
            var Extent : float2
            if Velocity.y > 0.0f
                Extent.x = Size.x
            else
                Extent.x = -Size.x
            if Velocity.x > 0.0f
                Extent.y = Size.y
            else
                Extent.y = -Size.y

            var Center = Velocity.x * Position.y - Velocity.y * Position.x
            var Corner = Velocity.x * Extent.y + Velocity.y * Extent.x

            push(ClipLines, float3(Velocity.y, -Velocity.x, Corner + Center))
            push(ClipLines, float3(-Velocity.y, Velocity.x, Corner - Center))

        for Line in ClipLines
            Line.z = (Line.x + abs(Line.x)) * 0.5f + (Line.y + abs(Line.y)) * 0.5f + Line.z

        for Y in range(Y1, Y2)
            for X in range(X1, X2)
                if !IsBlock(X, Y)
                    continue

                var Block = float2(X, Y)

                var Inside = true
                for Line in ClipLines
                    if Line.x * Block.x + Line.y * Block.y + Line.z <= 0.0f
                        Inside = false
                        break

                if Inside
                    if Velocity.x != 0.0f
                        if Velocity.x > 0.0f
                            Stop.x = Block.x - Size.x
                        else
                            Stop.x = Block.x + 1.0f + Size.x
                        Stop.x = (Stop.x - Position.x) / Velocity.x

                    if Velocity.y != 0.0f
                        if Velocity.y > 0.0f
                            Stop.y = Block.y - Size.y
                        else
                            Stop.y = Block.y + 1.0f + Size.y
                        Stop.y = (Stop.y - Position.y) / Velocity.y

                    if Stop.y <= Stop.x && 0.0f <= Stop.x && Move >= Stop.x
                        Move = Stop.x
                        Side = 1
                    if Stop.x <= Stop.y && 0.0f <= Stop.y && Move >= Stop.y
                        Move = Stop.y
                        Side = 2

        Position = Position + Velocity * Move

        if Side == 1
            Velocity.y = Velocity.y * (1.0f - Move)
            Velocity.x = 0.0f
        if Side == 2
            Velocity.x = Velocity.x * (1.0f - Move)
            Velocity.y = 0.0f

        if Move == 1.0f
            break

    Player.Position = Position

def GetWeaponRange() : float
    if Weapon.Type == 0
        return 0.2f
    if Weapon.Type == 1
        return 0.3f
    if Weapon.Type == 2
        return 0.5f
    if Weapon.Type == 3
        return 1.0f

    return 0.0f

def GetWeaponDamage() : float
    if Weapon.Type == 0
        return 100.0f
    if Weapon.Type == 1
        return 200.0f
    if Weapon.Type == 2
        return 300.0f
    if Weapon.Type == 3
        return 500.0f

    return 0.0f

def UpdateWeapon(Time : float)
    if is_key_pressed(VK_1)
        Weapon.Type = 0
    if is_key_pressed(VK_2)
        Weapon.Type = 1
    if is_key_pressed(VK_3)
        Weapon.Type = 2
    if is_key_pressed(VK_4)
        Weapon.Type = 3

    var Shoot = false
    if is_key_pressed(VK_LCONTROL) || is_key_pressed(VK_RCONTROL)
        Shoot = true

    var ShootSpeed = Time * 10.0f
    if Weapon.Shoot
        Weapon.Animation = Weapon.Animation + ShootSpeed
        if Weapon.Type >= 2 && Shoot
            if Weapon.Animation >= 4.0f
                Weapon.Animation = 2.0f
                Weapon.Shoot = false
        else
            if Weapon.Animation >= 5.0f
                Weapon.Animation = 0.0f
                Weapon.Shoot = false

    elif Shoot
        Weapon.Shoot = true

        var WeaponRange = GetWeaponRange()
        var WeaponDamage = GetWeaponDamage()

        var Direction = float2(cos(Player.Direction), sin(Player.Direction))

        var NearestEnemy = -1
        var NearestDistance = 0.0f

        var Index = 0
        for Enemy in Enemies
            var Object = Objects[Enemy.Object]
            var X = int(Object.Position.x)
            var Y = int(Object.Position.y)
            if X >= 0 && Y >= 0 && X < MapWidth && Y < MapHeight
                if Map[Y * MapWidth + X].Visible && IsBlock(X, Y)
                    var Distance = Object.Position - Player.Position
                    var DistanceC = Distance.x * Distance.x + Distance.y * Distance.y
                    if Weapon.Type >= 1 || DistanceC <= 2.0f
                        var DistanceA = Distance.x * Direction.x + Distance.y * Direction.y
                        var DistanceB = DistanceC - DistanceA * DistanceA
                        if DistanceB <= WeaponRange * WeaponRange
                            if NearestDistance > DistanceC || NearestEnemy == -1
                                NearestDistance = DistanceC
                                NearestEnemy = Index
            Index = Index + 1

        if NearestEnemy != -1
            assume Enemy = Enemies[NearestEnemy]
            if !IsDying(Enemy)
                assume Object = Objects[Enemy.Object]

                var Damage = 1.0f
                if NearestDistance >= 1.0f
                    Damage = 1.0f / sqrt(NearestDistance)

                Enemy.Health = Enemy.Health - Damage * WeaponDamage

                if Enemy.Health <= 0.0f
                    Enemy.Health = 0.0f
                    Object.ID = GetDyingFrame(Enemy)
                else
                    Object.ID = GetDamageFrame(Enemy)

                Object.Animation = 0.0f

    elif Weapon.Animation > 0.0f
        Weapon.Animation = Weapon.Animation + ShootSpeed
        if Weapon.Animation >= 5.0f
            Weapon.Animation = 0.0f

def UpdateObjects(Time : float)
    for Object in Objects
        if Object.ID != 0
            Object.Animation = Object.Animation + Time
            if Object.Animation >= 0.2f
                Object.Animation = Object.Animation - 0.2f;
                if Object.Enemy != -1
                    var Enemy = Enemies[Object.Enemy]
                    if Object.ID == GetDamageFrame(Enemy)
                        Object.ID = GetIdleFrame(Enemy) + GetPlayerAngle(Object.Position)
                Object.ID = GetNextFrame(Object.ID)

            var ID = Object.ID
            if ID == 136 || ID == 150 || ID == 151 || (ID >= 154 && ID <= 163)
                var Distance = Object.Position - Player.Position
                var DistanceC = Distance.x * Distance.x + Distance.y * Distance.y
                if DistanceC <= 0.25f
                    Object.ID = 0

                    if ID == 136
                        Player.Health = Player.Health + 4.0f
                    if ID == 154
                        Player.Health = Player.Health + 10.0f
                    if ID == 155
                        Player.Health = Player.Health + 25.0f
                    if ID == 163
                        Player.Health = Player.Health + 100.0f

                    if Player.Health > 100.0f
                        Player.Health = 100.0f

                    FlashState = true
                    FlashPhase = 0.0f
                    FlashColor = 0xffff00

def UpdateEnemies(Time : float)
    var Shoot = Weapon.Shoot && Weapon.Type >= 1
    if !Shoot
        for Enemy in Enemies
            if IsAttack(Enemy)
                Shoot = true
                break

    for Enemy in Enemies
        var AttackDistance = GetAttackDistance(Enemy)
        var HearDistance = 1.2f

        assume Object = Objects[Enemy.Object]
        var Distance = Object.Position - Player.Position
        var DistanceA = Distance.x * Distance.x + Distance.y * Distance.y

        var DistanceB = 1.0f
        if !Shoot
            var FirstDirection = GetFirstDirection(Object.ID)
            if FirstDirection != -1
                var Index = Object.ID - FirstDirection
                var Angle = 2.0f * PI * float(Index % 8) / 8.0f
                var Direction = float2(cos(Angle), sin(Angle))
                DistanceB = Distance.x * Direction.x + Distance.y * Direction.y

        var SightRange = DistanceA <= AttackDistance * AttackDistance
        var ViewRange = DistanceA <= HearDistance * HearDistance || DistanceB >= 0.0f
        if ViewRange
            ViewRange = IsVisible(Object.Position, Player.Position)

        var Attack = SightRange && ViewRange

        if IsAttack(Enemy)
            if Attack
                var Damage = Time * 10.0f
                Player.Health = Player.Health - Damage
                if Player.Health < 0.0f
                    Player.Health = 0.0f

                if !FlashState && Object.ID == GetAttackFrame(Enemy)
                    FlashState = true
                    FlashPhase = 0.0f
                    FlashColor = 0xff0000
            else
                Object.ID = GetIdleFrame(Enemy) + GetPlayerAngle(Object.Position)
                Object.Animation = 0.0f

        elif !IsDying(Enemy)
            if Attack
                Object.ID = GetAttackFrame(Enemy)
                Object.Animation = 0.0f

            elif ViewRange
                Object.ID = GetIdleFrame(Enemy) + GetPlayerAngle(Object.Position)
                Object.Animation = 0.0f

def UpdateDoors(Time : float)
    var Direction = float2(cos(Player.Direction), sin(Player.Direction))

    if is_key_pressed(VK_SPACE)
        if !DoorUse
            var X = int(Player.Position.x + Direction.x)
            var Y = int(Player.Position.y + Direction.y)
            if X >= 0 && Y >= 0 && X < MapWidth && Y < MapHeight
                if IsDoor(X, Y)
                    DoorUse = true
                    assume Tile = Map[Y * MapWidth + X]
                    if Tile.DoorState == 0
                        Tile.DoorState = 1
    else
        DoorUse = false

    var DoorSpeed = Time * 1.0f
    for Y in range(0, MapHeight)
        for X in range(0, MapWidth)
            if !IsDoor(X, Y)
                continue

            assume Tile = Map[Y * MapWidth + X]

            if Tile.DoorState == 1
                Tile.DoorSlide = Tile.DoorSlide + DoorSpeed
                if Tile.DoorSlide > 4.0f
                    Tile.DoorSlide = 4.0f
                    Tile.DoorState = 2
            elif Tile.DoorState == 2
                Tile.DoorSlide = Tile.DoorSlide - DoorSpeed
                if Tile.DoorSlide < 0.0f
                    Tile.DoorSlide = 0.0f
                    Tile.DoorState = 0

            var Block = float2(X, Y)
            if Player.Position.x + Player.Size >= Block.x && Player.Position.y + Player.Size >= Block.y && Player.Position.x - Player.Size <= Block.x + 1.0f && Player.Position.y - Player.Size <= Block.y + 1.0f
                if Tile.DoorState == 2 && Tile.DoorSlide >= 1.0f
                    Tile.DoorSlide = 4.0f


def UpdateFlash(Time : float)
    var FlashSpeed = Time * 10.0f
    if Player.Health <= 0.0f && FlashState && FlashColor == 0x7f0000
        FlashPhase = FlashPhase + FlashSpeed * 0.1f
        if FlashPhase >= 1.0f
            FlashPhase = 1.0f

            if is_key_pressed(VK_SPACE)
                UncompressMap()
                InitPlayer()
                InitWeapon()
                InitObjects()
                ClearVisibility()

    else
        if FlashState
            FlashPhase = FlashPhase + FlashSpeed
            if FlashPhase >= 1.0f
                FlashPhase = 0.0f
                FlashState = false
                FlashColor = 0x000000

        elif Player.Health <= 0.0f
            FlashState = true
            FlashPhase = 0.0f
            FlashColor = 0x7f0000

def ClearVisibility()
    for Y in range(0, MapHeight)
        for X in range(0, MapWidth)
            Map[Y * MapWidth + X].Visible = false

def DrawBackground(var bmp : Bitmap)
    var ScreenWidth = bmp.size.x
    var ScreenHeight = bmp.size.y
    var CenterX = ScreenWidth / 2
    var CenterY = ScreenHeight / 2

    fill_rect(bmp, 0, 0, ScreenWidth, CenterY, 0xff383838)
    fill_rect(bmp, 0, CenterY, ScreenWidth, ScreenHeight - CenterY, 0xff707070)

[unsafe_deref, jit, hint(unsafe_range_check,noalias=bmp,hot)]
def DrawWalls(var bmp : Bitmap)
    var ScreenWidth = bmp.size.x
    var ScreenHeight = bmp.size.y
    var CenterX = ScreenWidth / 2
    var CenterY = ScreenHeight / 2

    clear(DepthBuffer)
    DepthBuffer |> resize(bmp.size.x)

    var Position = Player.Position
    var Direction = float2(cos(Player.Direction), sin(Player.Direction))

    var MapPtr = unsafe(addr(Map[0]))

    for X in range(0, ScreenWidth)
        var View = float2(CenterX, X - CenterX)
        var Ray = float2(View.x * Direction.x - View.y * Direction.y, View.x * Direction.y + View.y * Direction.x)

        var MapX = int(Position.x)
        var MapY = int(Position.y)

        var StepX : int
        if Ray.x > 0.0f
            StepX = 1
        elif Ray.x < 0.0f
            StepX = -1
        else
            StepX = 0

        var StepY : int
        if Ray.y > 0.0f
            StepY = 1
        elif Ray.y < 0.0f
            StepY = -1
        else
            StepY = 0

        var Fraction = float2(Position.x - float(MapX), Position.y - float(MapY))

        var Distance : float2
        if Ray.x > 0.0f
            Distance.x = 1.0f - Fraction.x
        elif Ray.x < 0.0f
            Distance.x = Fraction.x
        else
            Distance.x = 0.0f

        if Ray.y > 0.0f
            Distance.y = 1.0f - Fraction.y
        elif Ray.y < 0.0f
            Distance.y = Fraction.y
        else
            Distance.y = 0.0f

        var Sign = float2(StepX, StepY)
        var Abs = Sign * Ray

        var Ratio = float2(0.0f, 0.0f)
        if Abs.x != 0.0f
            Ratio.x = Abs.y / Abs.x
        if Abs.y != 0.0f
            Ratio.y = Abs.x / Abs.y

        var Axis = float2(1.0f, 1.0f)
        if Abs.x >= Abs.y
            Axis.y = Ratio.y
        else
            Axis.x = Ratio.x

        var Next = float2(Distance.x * Axis.x, Distance.y * Axis.y)

        if Abs.x == 0.0f
            Next = float2(1.0f, 0.0f)
            Axis = float2(0.0f, 0.0f)

        if Abs.y == 0.0f
            Next = float2(0.0f, 1.0f)
            Axis = float2(0.0f, 0.0f)

        Distance = Distance - float2(1.0f, 1.0f)

        var ID : int
        var Side : int
        var Door : bool
        var Slide : float
        var Empty : bool

        Door = IsDoor(MapX, MapY)

        Empty = true
        while Empty
            unsafe
                MapPtr[MapY * MapWidth + MapX].Visible = true

            if Next.x < Next.y
                Next.x = Next.x + Axis.x
                Distance.x = Distance.x + 1.0f
                MapX = MapX + StepX
                Side = 0
            else
                Next.y = Next.y + Axis.y
                Distance.y = Distance.y + 1.0f
                MapY = MapY + StepY
                Side = 1

            unsafe
                ID = MapPtr[MapY * MapWidth + MapX].ID
            Empty = ID == 0 || ID >= 128
            if Door && Empty
                Door = false

            if ID >= 90 && ID <= 101
                unsafe
                    Slide = MapPtr[MapY * MapWidth + MapX].DoorSlide
                var Vec = float2(MapX, MapY) - Position
                if Side == 0
                    Vec.x = (Vec.x + 0.5f) * Sign.x
                    var Open = Vec.x * Ratio.x * Sign.y - Vec.y
                    if Open >= Slide && Open <= 1.0f
                        Distance.x = Vec.x
                    else
                        Empty = true
                        Door = true
                        Slide = 0.0f
                else
                    Vec.y = (Vec.y + 0.5f) * Sign.y
                    var Open = Vec.y * Ratio.y * Sign.x - Vec.x
                    if Open >= Slide && Open <= 1.0f
                        Distance.y = Vec.y
                    else
                        Empty = true
                        Door = true
                        Slide = 0.0f

        if ID >= 90 && ID <= 91
            ID = 50
        if ID >= 92 && ID <= 99
            ID = 53
        if ID >= 100 && ID <= 101
            ID = 52

        if Door
            ID = 51

        if Side == 0
            Distance.y = Distance.x * Ratio.x
        else
            Distance.x = Distance.y * Ratio.y

        Distance = Distance * Sign

        var Size = CenterY
        var Depth = Distance.x * Direction.x + Distance.y * Direction.y
        if Depth != 0.0f
            Size = int((float(CenterX)) / Depth)
        var Y = CenterY - Size / 2

        var T : float
        if Side == 0
            T = Position.y + Distance.y
        else
            T = Position.x + Distance.x

        T = T - floor(T)

        if ID == 50 || ID == 52 || ID == 53
            T = T - Slide
        else
            if Side == 0
                if StepX < 0
                    T = 1.0f - T
            else
                if StepY > 0
                    T = 1.0f - T

        if T > 63.0f / 64.0f
            T = 63.0f / 64.0f

        var Sprite = (ID - 1) * 2 + 1 - Side

        var U = float(Sprite & 31) + T
        var V = float(Sprite >> 5)


        draw_vspan(bmp, Texture, X, Y, float4(U * 64.0f, V * 64.0f, 1.0f, 64.0f), Size)

        DepthBuffer[X] = Depth

[jit, hint(unsafe_range_check,noalias=bmp,hot)]
def DrawObjects(var bmp : Bitmap)
    var Direction = float2(cos(Player.Direction), sin(Player.Direction))

    var ScreenWidth = bmp.size.x
    var ScreenHeight = bmp.size.y
    var CenterX = ScreenWidth / 2
    var CenterY = ScreenHeight / 2

    var MinX = 0
    var MinY = 0
    var MaxX = ScreenWidth - 1
    var MaxY = ScreenHeight - 1

    var M11 = 0
    var M21 = 0
    var M12 = 0
    var M22 = 0
    var M31 = 0
    var M32 = 0

    if abs(Direction.x) >= abs(Direction.y)
        if Direction.X > 0.0f
            M21 = -1
        else
            M21 = 1

        if Direction.Y > 0.0f
            M12 = -1
        else
            M12 = 1
    else
        if Direction.X > 0.0f
            M11 = -1
        else
            M11 = 1

        if Direction.Y > 0.0f
            M22 = -1
        else
            M22 = 1

    if M11 < 0 || M21 < 0
        M31 = MapWidth - 1
    if M12 < 0 || M22 < 0
        M32 = MapHeight - 1

    var Width = MapWidth
    var Height = MapHeight
    if M12 == 0
        Width = MapHeight
    if M21 == 0
        Height = MapWidth

    for Y in range(0, Height)
        for X in range(0, Width)
            var MapX = X * M11 + Y * M21 + M31
            var MapY = X * M12 + Y * M22 + M32

            if !Map[MapY * MapWidth + MapX].Visible
                continue

            var Object = Objects[MapY * MapWidth + MapX]
            var ID = Object.ID
            if ID < 128
                continue

            var Distance = Object.Position - Player.Position

            var Center = float(CenterX)
            var Size = float(CenterX)

            var Position = Distance.y * Direction.x - Distance.x * Direction.y
            var Depth = Distance.x * Direction.x + Distance.y * Direction.y
            if Depth != 0.0f
                Center = Center * Position / Depth
                Size = float(CenterY) / Depth

            var X1 = CenterX - int(Size) + int(Center)
            var Y1 = CenterY - int(Size)
            var X2 = CenterX + int(Size) + int(Center)
            var Y2 = CenterY + int(Size)

            var OriginX = float(X1)
            var OriginY = float(Y1)
            var SizeX = float(X2 - X1)
            var SizeY = float(Y2 - Y1)

            if X1 > MaxX || Y1 > MaxY || X2 < MinX || Y2 < MinY
                continue

            if X1 < MinX
                X1 = MinX
            if Y1 < MinY
                Y1 = MinY
            if X2 > MaxX
                X2 = MaxX
            if Y2 > MaxY
                Y2 = MaxY

            while X1 <= X2 && DepthBuffer[X1] < Depth
                X1 = X1 + 1
            while X1 <= X2 && DepthBuffer[X2] < Depth
                X2 = X2 - 1

            if X1 > X2
                continue

            var FirstDirection = GetFirstDirection(ID)
            if FirstDirection != -1
                var PlayerAngle = GetPlayerAngle(Object.Position)
                var Index = ID - FirstDirection
                var Angle = Index % 8
                var Phase = Index / 8
                Angle = (Angle + 8 - PlayerAngle) % 8
                ID = Phase * 8 + Angle + FirstDirection

            var U = float(ID % 32)
            var V = float(ID / 32)

            var U1 = U + (float(X1) - OriginX) / SizeX
            var V1 = V + (float(Y1) - OriginY) / SizeY
            var U2 = U + (float(X2) - OriginX) / SizeX
            var V2 = V + (float(Y2) - OriginY) / SizeY

            draw_image_region_masked(bmp, Texture, X1, Y1, float4(U1 * 64.0f, V1 * 64.0f, (U2 - U1) * 64.0f, (V2 - V1) * 64.0f), int2(X2 - X1, Y2 - Y1), MASK_COLOR)

def DrawMap(var bmp : Bitmap)
    ClearVisibility()
    DrawWalls(bmp)
    DrawObjects(bmp)

def DrawWeapon(var bmp : Bitmap)
    var ScreenWidth = bmp.size.x
    var ScreenHeight = bmp.size.y
    var CenterX = ScreenWidth / 2
    var CenterY = ScreenHeight / 2

    var Size = ScreenHeight

    var X = CenterX - Size / 2
    var Y = ScreenHeight - Size

    var U = float(Weapon.Type * 5) + floor(Weapon.Animation)
    var V = 17.0f

    draw_image_region_masked(bmp, Texture, X, Y, float4(U * 64.0f, V * 64.0f, 64.0f, 64.0f), int2(Size, Size), MASK_COLOR)

def DrawHealth(var bmp : Bitmap)
    var ScreenWidth = bmp.size.x
    var ScreenHeight = bmp.size.y

    var BarWidth = ScreenHeight / 3
    var BarHeight = ScreenHeight / 30

    var Border = ScreenHeight / 20

    var Alpha = Player.Health / 100.0f
    if Alpha < 0.0f
        Alpha = 0.0f
    if Alpha > 1.0f
        Alpha = 1.0f

    var Value = uint(float(255.0f * Alpha))
    var Color = 0xff000000 | ((uint(255) - Value) << uint(16)) | (Value << uint(8))

    var X = Border
    var Y = ScreenHeight - Border - BarHeight

    var Width = int(float(BarWidth) * Alpha)
    var Height = BarHeight

    fill_rect(bmp, X, Y, Width, Height, Color)
    fill_rect(bmp, X + Width, Y, BarWidth - Width, Height, 0x7f7f7f7f)

def DrawFlash(var bmp : Bitmap)
    if !FlashState
        return

    var ScreenWidth = bmp.size.x
    var ScreenHeight = bmp.size.y

    var Alpha = FlashPhase
    if FlashColor != 0x7f0000
        Alpha = Alpha * 2.0f
        if Alpha > 1.0f
            Alpha = 2.0f - Alpha
        Alpha = Alpha * 0.5f

    if Alpha < 0.0f
        Alpha = 0.0f
    if Alpha > 1.0f
        Alpha = 1.0f

    var Color = (uint(Alpha * 255.0f) << uint(24)) | FlashColor
    fill_rect(bmp, 0, 0, ScreenWidth, ScreenHeight, Color)

def LoadTexture
    var x, y : int
    var comp : int
    let name = "{get_das_root()}/examples/w3dold/Texture.png"
    let data = stbi_load(name, safe_addr(x), safe_addr(y), safe_addr(comp), 4)
    if data == null
        panic("no texture {name}\n")
    Texture.size = int2(x, y)
    Texture.pixels = unsafe(reinterpret<uint?> data)
    for i in range(Texture.size.x * Texture.size.y)
        var pixel = unsafe(Texture.pixels[i])
        pixel = (pixel & 0xff00ff00) | ((pixel & 0xff) << 16u) | ((pixel & 0xff0000) >> 16u)
        unsafe(Texture.pixels[i]) = pixel

[export]
def on_initialize()
    set_debug_status_string("arrows - move, ctrl - attack, space - action, alt - strafe, 1,2,3,4 - select weapon")
    LoadTexture()
    UncompressMap()
    InitPlayer()
    InitWeapon()
    InitObjects()
    ClearVisibility()

var time = 12lf

[export]
def on_update( var bmp:Bitmap )

    let begin = ref_time_ticks()

    let Time = get_delta_time_sec()
    UpdatePlayer(Time)
    UpdateWeapon(Time)
    UpdateObjects(Time)
    UpdateEnemies(Time)
    UpdateDoors(Time)
    UpdateFlash(Time)

    DrawBackground(bmp)
    DrawMap(bmp)
    DrawWeapon(bmp)
    DrawHealth(bmp)
    DrawFlash(bmp)

    time = time * 0.95lf + double(get_time_usec(begin)/1000) * 0.05lf

    // set_debug_status_string("update time: {get_time_usec(begin)/1000}ms avg: {time}ms")

[export]
def main
    on_initialize()
    raster_app_main <| $ ( backbuffer )
        on_update(backbuffer)

