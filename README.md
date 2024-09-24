# Shadow Descent FPS (Prototype)
This is an old school, Doom/Wolfenstein inspired first person shooter and base engine **entirely coded in C++ and GLSL**, using OpenGL 4.4 as the graphics API, and various other low level libraries to aid in core functionality.<br/>
It was done for my MSc thesis project at the University of Dundee.<br/>
**It is in no way, shape or form meant for any commercial use or application.<br/>**

It uses the Tiled Map Editor, and the tmx files it generates, as layout data for generating walls, ceilings, floors and their associated textures. It also defines the location data for enemy and item spawns.<br/>
It is designed for easy addition of new game objects, and uses billboarding sprites and texture cycling to create animations.<br/>

## Fair Warning
**The game and its base engine was created in a very limited development window of around 7 weeks**, learning OpenGL and thesis writing also needed some time :( <br/>
Would not recommend anyone to do this, 12+ hour days programming, and bugfixing is hell. <br/>

It is my first foray into game development and graphics programming, so features and components were implemented as I was learning the OpenGL pipeline.
There are many things I would do differently, having learnt the lessons I did during this process. If I decide to refactor the implementation into a more streamlined, optimised product, I will link that repository here. <br/>

## Libaries and Frameworks Used
- OpenGL
- GLAD
- GLFW
- GLM
- tmxlite
- Stb
- irrklang
- ImGui
  
## Feature List
- BFS Pathfinding Algorithm for enemies to detect player.
- Enemy state machine that switches between Walking, Attacking and Death based on distance between enemy and player, and enemy's health.
- Rudimentary collision detection based on evaluating object position relative to terrain.
- Texture Cycling as a means of pseudo-animation of sprites (was not possible to introduce model loading via libraries like Assimp in the timeframe).
- Sound playback via implementation of the irrKlang library.
- Enemy Factory setup based on the decorator design pattern to allow for efficient implementation of new enemies into the game.
- Dynamic adjustment of sprite textures to properly work with direct spotlight effects.

![image](https://github.com/user-attachments/assets/ec65e6c8-e0b6-480e-8c16-a899f9997480) <br/>
Tiled Map Layout

## Demo Video
https://github.com/user-attachments/assets/fa83e99f-fdc1-406a-8594-f856e2367eea

