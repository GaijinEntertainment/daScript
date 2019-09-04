options log=true, log_stack=true, log_nodes=true
// options log_infer_passes=true
// options print_var_access=true

struct ItemGeneration
    items : array<tuple<string; float> >
    amount : int2

def ItemGeneration(var items : tuple<string; float> []; amount : int2 = int2(1,1))
    return <- [[ ItemGeneration items <- to_array_move(items), amount = amount ]]

struct SoldierScheme
    templ : string
    w : int
    sClass : string
    itemSlots : table<string; ItemGeneration>
    perks : array<table<string; float> >

let
    soldierScheme <- {{
        "generic" => {{
            "soldier_1" => [[ SoldierScheme
                templ="generic_soldier_1",
                w = 15,
                sClass = "noclass",
                itemSlots <- {{
                    "primary" => ItemGeneration([[ auto[] "m1_garand" =>2. ]]);
                    "secondary" => ItemGeneration([[ auto
                        "browning_auto_5" => 2.;
                        "__noitem__" => 2. ]]);
                    "melee" => ItemGeneration([[ auto
                        "knife" => 10.;
                        "axe" => 5. ]]);
                    "helmet" => ItemGeneration([[ auto
                        "helmet_usa_1" => 2.;
                        "__noitem__" => 10. ]]);
                    "inventory" => ItemGeneration([[ auto
                          "small_medkit" => 2.;
                          "__noitem__" => 1. ]],
                          int2(1, 4))
                }},
                perks <- to_array_move([[ auto[]
                    {{ "regeneration" => 1.; "stamina" => 3.; "sprint" => 3. }} ]])
            ]];
            "soldier_2" => [[ SoldierScheme
                templ="generic_soldier_2",
                w = 15,
                sClass = "noclass",
                itemSlots <- {{
                    "primary" => ItemGeneration([[ auto
                        "m1903_springfield_tunisia" => 2.;
                        "m1_garand" => 2. ]]);
                    "secondary" => ItemGeneration([[ auto
                        "browning_auto_5" => 2.;
                        "browning_m1918" => 3.;
                        "__noitem__" => 2. ]]);
                    "melee" => ItemGeneration([[ auto
                        "knife" => 10.;
                        "axe" => 5. ]]);
                    "helmet" => ItemGeneration([[ auto
                        "helmet_usa_2" => 2.;
                        "__noitem__" => 1. ]]);
                    "inventory" => ItemGeneration([[ auto
                          "small_medkit" => 2.;
                          "__noitem__" => 1. ]],
                          int2(1, 4))
                }},
                perks <- to_array_move([[ auto[]
                    {{ "regeneration" => 1.; "stamina" => 3.; "sprint" => 3. }} ]])
            ]]
        }}
    }}

[export]
def test
    //debug(soldierScheme)
    var qq <- {{ 1 => "one"; 2 => "two" }}
    debug(qq)
    return true

