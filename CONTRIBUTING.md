# Contribuer à KOTH Waves

Merci de votre intérêt pour ce mod DayZ.

## Prérequis

- Serveur DayZ dédié pour tester les changements côté serveur.
- Outils de packaging PBO habituels pour votre workflow mod.

## Workflow

1. Forkez le dépôt et créez une branche depuis `main`.
2. Modifiez le code ou la configuration d'exemple.
3. Testez sur un serveur local avec les logs `[KOTHWaves]`.
4. Ouvrez une pull request avec une description claire du changement.

## Conventions

- Messages de commit : `feat:`, `fix:`, `docs:`, `refactor:`, etc.
- Conservez la rétrocompatibilité des champs JSON existants lorsque c'est possible.
- Mettez à jour `ProfileExample/` si la structure de configuration change.
- Documentez les nouvelles options dans `README.md`.

## Signaler un bug

Incluez dans votre issue :

- Version du mod
- Extrait des logs serveur `[KOTHWaves]`
- Fichiers de config concernés (sans données sensibles)
