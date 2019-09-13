options indenting=2

struct Campaign
  title : string
  id : string
  queue : string
  armies : array<string>

struct GameProfile
  campaigns : table<string; Campaign>
  availableCampaigns : array<string>
  visibleCampaigns : array<string>
  minSoldiersInSquad : int
  maxSquadSoldiersInBattle : int
  baseSoldiersAmount : int2

let
  gameProfile = [[ GameProfile
    campaigns <- {{
      "normandy" => [[ Campaign
        title = "Invasion of Normandy",
        id = "normandy",
        queue = "enlisted_normandy",
        armies <- [{ auto[] "normandy_allies"; "normandy_axis" }]
      ]];

      "moscow" => [[ Campaign
        title = "Battle for Moscow",
        queue = "enlisted_moscow",
        armies <- [{ auto[] "moscow_allies"; "moscow_axis" }]
      ]];

      "tunisia" => [[ Campaign
        title = "Battle of Tunisia",
        queue = "enlisted_tunisia",
        armies <- [{ auto[] "tunisia_allies"; "tunisia_axis" }]
      ]]
    }},

    availableCampaigns <- [{ auto[] "normandy"; "moscow"; "tunisia" }],
    visibleCampaigns <- [{ auto[] "normandy"; "moscow"; "tunisia" }],

    minSoldiersInSquad = 4,
    maxSquadSoldiersInBattle = 7
  ]]
