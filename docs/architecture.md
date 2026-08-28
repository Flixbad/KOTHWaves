# Architecture KOTH Waves

## Vue d'ensemble

KOTH Waves est un mod **100 % serveur** pour DayZ. Il gère des événements King of the Hill avec spawn d'ennemis, capture de zone et distribution de loot.

```text
Mission (5_Mission)
  └── KOTHWavesMission.c          → point d'entrée mission

World (4_World)
  ├── Config/
  │   ├── KOTHWavesConfig.c        → zones KOTH + vagues
  │   ├── KOTHWavesItemPreset.c   → presets loot
  │   ├── KOTHWavesWavePreset.c   → presets ennemis
  │   └── KOTHWavesPresetPicker.c → tirage pondéré
  └── Server/
      ├── KOTHWavesManager.c      → orchestration globale
      ├── KOTHWavesEvent.c        → cycle d'un KOTH actif
      ├── KOTHWavesSpawnUtil.c    → spawn ennemis / outdoor
      ├── KOTHWavesRewardCrate.c  → caisse de récompense
      ├── KOTHWavesTerritoryFlag.c→ drapeau d'événement
      └── …                         → compat, cleanup, visuels
```

## Configuration runtime

Au premier démarrage, le mod génère dans `$profile:KOTHWaves/` :

| Fichier | Rôle |
|---------|------|
| `KOTHWavesConfig.json` | Zones, rayons, vagues, presets |
| `ItemPreset.json` | Loot par preset nommé |
| `WavePreset.json` | Ennemis par preset nommé |

Les fichiers dans `ProfileExample/` servent de référence pour une config propre.

## Flux d'un événement

1. Le manager charge la config et surveille les zones KOTH.
2. Un joueur entre dans le rayon de capture → démarrage du timer et des vagues.
3. Les vagues spawnent des ennemis selon `Waves` / presets.
4. Capture réussie → loot victoire, feux d'artifice optionnels, cleanup.
