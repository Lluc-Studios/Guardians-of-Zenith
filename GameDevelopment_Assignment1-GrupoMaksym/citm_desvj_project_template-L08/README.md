# Obsolete
Repository dedicated to the releases of the development subject (GDDV-CITM)

In this repository we will be updating our platformer for the Development subject, "Obsolete".
## Description
Obsolete is a basic 2D platformer in which your character is a robot. 

## Changelog
Version 1.1: 
-Remodeled all assets from the game (tiles, player, enemy...) 
-Added enemies with pathfinding towards the player.
-Added enemies to the save/load game state

## Key Features

### General features
 - **Configuration loaded though XML** files:
	- Assets pathing
	- Window configuration
	- Main characther and saw parameters
 - **Map Loading from TMX:**
	- Colliders taken from object groups defined in Tiled
 - **Save/Load file detection**
 - **Menu and settings**: Volume can be modified from options menu, before starting the level
### Gameplay
 - Double jump
 
## Controls

 - A: Move left
 - D: Move right
 - Spacebar: Jump
 - Left click: Player attack

## Debug Keys

 - F1: Avoid the initial transitions
 - F2: Show colliders and pathfinding
 - F3: Start from the beginning of the level
 - F4: Player death
 - F5: Save game state
 - F6: Load game state
 - F10: God mode
 
## Known bugs

 - Walking towards a wall while colliding will prevent the player to fall, even if there is no floor. 
 - While saving, the positions of the saw won't be saved, so don't save if you are inside a saw's pathing
 
## Developers

 - Biel Rubio - Art, level design, module saw creation, map TMX loading and drawing 
 > https://github.com/BielRubio
 - Nacho Moreno - Player mechanics, collision detection and Load/Save game state 
 > https://github.com/NachoMoren
 - Maksym Polupan - Scenes and scenes transitions, audio and fx, fonts, GUI
 > https://github.com/Maksym203

## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.


