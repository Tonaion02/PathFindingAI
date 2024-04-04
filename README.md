# PathFindingAI

<!-- ![logo](presentation/projectLogo.png) -->
<p align="center">
  <img src="presentation/projectLogo.png" alt="Image Description">
</p>

PathFindingAI is an AI that follows a target in a game.

For more detail about our work consult the [doc folder](https://github.com/Tonaion02/PathFindingAI/tree/readme/doc).

***
## Goals

We developed this project with some goals in mind:
- **Performance**: we want that this AI is effectively ready to be implemented in a game that runs at least at 60 fps.
- **Plausibility**: we want that the action of the AI is really understandable. It's particulary important to increase the immersion. 

***
## Features

Showcase of the ai in action: https://www.youtube.com/watch?v=OsOChnBTgcI

The following AI has these features:
- **Line of sight**: implemented Line Of Sight(in an adapted version for tilegame with grid-based movement) to not track player
that are hidden from a wall.   
- __A*__: An optimized implementation of the A* algorithm to find an optimal path efficiently.
- **Registering and using the last pos**: when the "eye contact" is interrupted, no worries, the AI register the last location of the target and move to that location to explore the area.
- **Idle**: a simple routine to explore the area when there isn't many information about the target position.

***
## Try the demo

To compile the repository, you must use premake5.

I developed this demo in vs2017:
```
$premake5 vs2017
```

***
## Files
The AI is developed in the following files:
- [PathFindingSystem.h](https://github.com/Tonaion02/PathFindingAI/blob/main/src/Systems/Exploring/PathFindingSystem.h), [PathFindingSystem.cpp](https://github.com/Tonaion02/PathFindingAI/blob/main/src/Systems/Exploring/PathFindingSystem.cpp): for the implementation of the algorithm used to compute the best path.
- [LineOfSightSystem.h](https://github.com/Tonaion02/PathFindingAI/blob/main/src/Systems/Exploring/LineOfSightSystem.h), [LineOfSightSystem.cpp](https://github.com/Tonaion02/PathFindingAI/blob/main/src/Systems/Exploring/LineOfSightSystem.cpp): for the implementation of the line of sight system adapted to a tilegame with a grid-based movement.
- [RenderDebugFieldOfView.h](https://github.com/Tonaion02/PathFindingAI/blob/main/src/Systems/Exploring/RenderDebugFieldOfView.h), [RenderDebugFieldOfView.cpp](https://github.com/Tonaion02/PathFindingAI/blob/main/src/Systems/Exploring/RenderDebugFieldOfView.cpp), [RenderDebugLineOfSight.h](https://github.com/Tonaion02/PathFindingAI/blob/main/src/Systems/Exploring/RenderDebugLineOfSight.h), [RenderDebugLineOfSight.cpp](https://github.com/Tonaion02/PathFindingAI/blob/main/src/Systems/Exploring/RenderDebugLineOfSight.cpp) : systems created to render and debug visually the line of sight and field of view.
***
