# KOTH Waves

Événement King of the Hill côté serveur pour DayZ, avec vagues d'ennemis configurables (zombies, animaux, mutants de mods, etc.).

## Arborescence

```text
KOTHWaves/
├── config.cpp              → déclaration mod + entités
├── $PBOPREFIX$             → préfixe PBO
├── Scripts/
│   ├── 4_World/KOTHWaves/  → logique serveur + config
│   └── 5_Mission/KOTHWaves/→ hook mission
├── ProfileExample/         → exemples JSON de référence
├── docs/                   → documentation technique
└── assets/                 → ressources (images, etc.)
```

Voir aussi [docs/architecture.md](docs/architecture.md) pour le détail des modules.

## Installation

1. Empaqueter le dossier `KOTHWaves` en mod DayZ.
2. Ajouter le mod aux paramètres de lancement du serveur.
3. Au premier démarrage, le mod crée automatiquement les fichiers de profil (format propre, sans doublons legacy) :

```text
$profile:KOTHWaves\KOTHWavesConfig.json   → zones KOTH + vagues progressives
$profile:KOTHWaves\ItemPreset.json        → loot par preset
$profile:KOTHWaves\WavePreset.json        → ennemis par preset
```

Vous pouvez supprimer le dossier `KOTHWaves` dans `profiles` pour forcer une regénération (après repack du mod à jour). Les fichiers recréés correspondent au `ProfileExample` du mod.

## Presets de loot (`ItemPreset.json`)

Le butin de récompense est défini par preset nommé (`military`, `medical`, `basebuilding`, …). Chaque zone KOTH dans `KOTHWavesConfig.json` peut :

- **Un seul preset** : `Loot_Preset` (rétrocompatible).
- **Plusieurs presets avec chances** : `Loot_Preset_Chances` — tirage pondéré à chaque KOTH gagné.

Si `Loot_Preset_Chances` est vide, le mod utilise `Loot_Preset`, puis `Default_Preset` dans `ItemPreset.json`.

## Pools d'ennemis (`WavePreset.json`)

Chaque preset définit uniquement une liste d'**Enemies** (classnames DayZ ou mods) :

- `military_zombies`, `city_infected` — infectés vanilla
- `mutants` — remplacez par vos classnames de mod
- `wildlife` — loups et ours
- `boss` — un seul type de boss

```json
{
    "Name": "mutants",
    "Enemies": [
        { "ClassName": "MonMutant_Classname", "SpawnRarity": 0, "Difficulty": 4 }
    ]
}
```

`Difficulty` sert au filtrage optionnel. `SpawnRarity` = % de skip aléatoire (0 = toujours éligible).

## Planning des vagues (par KOTH dans `KOTHWavesConfig.json`)

Chaque KOTH peut définir son propre tableau **`Waves`**. Chaque vague choisit **son preset d'ennemis** :

### Vague simple — beaucoup de zombies

```json
{
    "Name": "Vague 1 — Militaires",
    "DelaySeconds": 0,
    "Enemy_Preset": "military_zombies",
    "ZombieCount": 25,
    "DifficultyMin": 1,
    "DifficultyMax": 4,
    "SpawnRadiusMin": 18.0,
    "SpawnRadiusMax": 35.0
}
```

### Vague mutants — 2 de chaque type du preset

```json
{
    "Name": "Vague 3 — Mutants",
    "DelaySeconds": 450,
    "SpawnRadiusMin": 22.0,
    "SpawnRadiusMax": 45.0,
    "Spawns": [
        { "Preset": "mutants", "CountPerType": 2 }
    ]
}
```

`CountPerType` : spawn N exemplaires **de chaque** ennemi listé dans le preset (idéal pour mutants / faune rare).

### Vague faune — 1 loup + 1 ours

```json
{
    "Name": "Vague 4 — Faune",
    "DelaySeconds": 675,
    "Spawns": [
        { "Preset": "wildlife", "CountPerType": 1 }
    ]
}
```

### Vague boss — 1 boss + 10 zombies

```json
{
    "Name": "Vague 5 — Boss",
    "DelaySeconds": 900,
    "Spawns": [
        { "Preset": "boss", "Count": 1 },
        { "Preset": "military_zombies", "Count": 10, "DifficultyMin": 1, "DifficultyMax": 3 }
    ]
}
```

`Spawns` permet de combiner plusieurs presets dans **une même vague**.

### Rétrocompatibilité

- Si `Waves` sur le KOTH est **vide** : le mod utilise le planning du preset tiré via `Wave_Preset` / `Wave_Preset_Chances`, puis `Default_Waves`.
- `Wave_Preset` sert de **fallback** pour les vagues sans `Enemy_Preset` ni `Spawns`.

## Vagues par défaut

Les vagues ne démarrent que lorsqu'au moins un joueur vivant entre dans le rayon de capture (`Capture_Radius`). Les délais sont comptés à partir de ce moment.

Planning type Zelenogorsk (15 min, `Capture_Time` = 900 s) :

- Vague 1 (0 s) — 25 militaires
- Vague 2 (225 s) — 20 civils
- Vague 3 (450 s) — 2× chaque mutant du preset
- Vague 4 (675 s) — 1× chaque loup/ours
- Vague 5 (750 s) — 1 boss + 10 militaires ( aussi déclenchée à la victoire si pas encore passée )

**Loot victoire** : preset `Victory_Loot_Preset` (défaut `boss` dans `ItemPreset.json`), distinct des presets loot aléatoires pendant l'événement.

**Spawn extérieur** : `Spawn_Outdoor_Only` = `1` (défaut) — raycast pour éviter les spawns sous toit / dans bâtiments. Mettre à `0` pour désactiver.

`Debug_QuickMode` = `1` pour des tests locaux plus rapides.

## Dépannage spawn

Si aucun ennemi n'apparaît, vérifiez les logs serveur `[KOTHWaves]` :

- `preset « xxx » introuvable` → nom de preset incorrect dans `Waves` ou `WavePreset.json`
- `Spawn échoué pour « ClassName »` → classname invalide ou mod absent client+serveur

Les animaux vanilla (`Animal_*`) sont initialisés automatiquement. Les mutants de mods doivent utiliser le **classname exact** du mod.

## Notes

- Drapeau `KOTHWavesTerritoryFlag` : non revendicable (LBmaster / Expansion compatibles sur le même serveur).
- Le drapeau est retiré après capture réussie.
- Caisse `KOTHWavesRewardCrate` par défaut (loot accessible, caisse non déplaçable).
- `Reward_Despawn_Seconds` : disparition du butin (défaut 600 s).
- `Reward_Delete_On_Cleanup` = `1` : suppression forcée des caisses au reset de zone.
- Feux d'artifice après capture si `Fireworks_Enabled` = `1`.
- Indépendant de MZ KOTH.

## Licence

MIT — voir [LICENSE](LICENSE).

## Contribution

Les contributions sont les bienvenues : voir [CONTRIBUTING.md](CONTRIBUTING.md).
