# Shadow Descent FPS (Prototype)
This is an old school, Doom/Wolfenstein inspired first person shooter and base engine **entirely coded in C++ and GLSL**, using OpenGL 4.4 as the graphics API, and various other low level libraries to aid in core functionality.<br/>
It was done for my MSc thesis project at the University of Dundee.<br/>
**It is in no way, shape or form meant for any commercial use or application.<br/>**  
![image](https://github.com/Varendraaa/Shadow/blob/d5b7434ad995dcbed106896e55d4117358ae1fc3/Screenshot%201.png) 

It uses the Tiled Map Editor, and the tmx files it generates, as layout data for generating walls, ceilings, floors and their associated textures. It also defines the location data for enemy and item spawns.<br/>
It is designed for easy addition of new game objects, and uses billboarding sprites and texture cycling to create animations. <br/>  
![image](https://github.com/Varendraaa/Shadow/blob/d5b7434ad995dcbed106896e55d4117358ae1fc3/Screenshot%202024-08-07%20030018.png)

## Fair Warning
**The game and its base engine was created in a very limited development window of around 7 weeks**, learning OpenGL and thesis writing also needed some time :( <br/>
Would not recommend anyone to do this, 12+ hour days programming, and bugfixing is hell. <br/>

It is my first foray into game development and graphics programming, so features and components were implemented as I was learning the OpenGL pipeline.
There are many things I would do differently, having learnt the lessons I did during this process. If I decide to refactor the implementation into a more streamlined, optimised product, I will link that repository here. <br/>  
[Download the game exe here!](https://github.com/Varendraaa/Shadow/blob/1b451607d9d1c161cf9a6a6c3657cb0284ea49ad/Game%20Executable.zip)

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

![image](https://github.com/Varendraaa/Shadow/blob/d5b7434ad995dcbed106896e55d4117358ae1fc3/Screenshot%202024-08-12%20150849.png)


