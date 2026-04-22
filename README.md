# ECE-shoot-them-up
Jeux

Un shoot'em up horizontal développé en **C** avec la bibliothèque **Allegro 5**, dans le cadre du projet ING1.

Gameplay

Pilotez votre vaisseau spatial et survivez à l'invasion alien ! Les extraterrestres arrivent de la droite en nombre croissant et tirent sur vous sans relâche. Abattez-les pour marquer des points — mais attention, la difficulté monte au fil du temps.

Objectif
survivre le plus longtemps possible et maximiser votre score.


Contrôles

| Touche | Action |
|--------|--------|
| ↑ ↓ ← → | Déplacer le vaisseau |
| `Espace` | Tirer |
| `Entrée` | Démarrer / Rejouer |
| `Échap` | Quitter |

Structure du projet
- main.c                  # Boucle principale, initialisation Allegro
- game.c/h                # États du jeu (menu, jeu, game over), collisions, score
- player.c/h              # Déplacement joueur, tir, pool de balles, sprite
- enemy.c/h               # Ennemis aliens, tir automatique, pool de balles ennemies
- input.c/h               # Gestion clavier (variables globales par touche)
- pngegg.png              # Sprite du vaisseau joueur
- Image_extraterestre.png # Sprite des ennemis aliens

Compilation

Build

```bash
mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
cmake --build .
```

Les fichiers `pngegg.png` et `Image_extraterestre.png` doivent être copiés dans `cmake-build-debug/` à côté de `game.exe`.


Fonctionnalités

-  3 états de jeu : Menu → Partie → Game Over
-  Déplacement fluide du joueur (borné à l'écran)
-  Ennemis aliens avec tir automatique aléatoire
-  Difficulté progressive (spawn accéléré toutes les 600 frames)
-  Affichage du score en temps réel




