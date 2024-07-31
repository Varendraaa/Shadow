# Shadow Descent FPS
This is an old school, Doom/Wolfenstein inspired first person shooter and base engine entirely coded in C++ and GLSL.<br/>
It was done for my MSc thesis project at the University of Dundee.<br/>
**It is in no way, shape or form meant for any commercial use or application.<br/>**
It uses the Tiled Map Editor, and the tmx files it generates, as layout data for generating walls, ceilings, floors, enemy and item spawns.<br/>
It is designed for easy addition of new game objects, such as enemies and items, and uses billboarding sprites and cycling textures to create animations.<br/>


## Fair Warning
**The game and its base engine was created in a very limited development window of around 7 weeks.** <br/>
It is my first foray into game development and graphics programming, so features and components were implemented as I was learning the OpenGL pipeline. <br/>
There are many things I would do differently, having learnt the lessons I did during this process. If I decide to refactor the implementation into a more streamlined, optimised product, I will link that repository here. <br/>

## Feature List
- BFS Pathfinding Algorithm for enemies to detect player.
- Enemy state machine that switches between Walking, Attacking and Death based on distance between enemy and player, and enemy's health.
- Rudimentary collision detection based on evaluating object position relative to terrain.
- Texture Cycling as a means of pseudo-animation of sprites (was not possible to introduce model loading via libraries like Assimp in the timeframe).
- Sound playback via implementation of the irrKlang library.
- Enemy Factory setup based on the decorator design pattern to allow for efficient implementation of new enemies into the game.
- Dynamic adjustment of sprite models to properly work with direct spotlight effects.


## Demo Video
https://github.com/user-attachments/assets/fa83e99f-fdc1-406a-8594-f856e2367eea

