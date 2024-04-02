# PathFindingAI

<!-- ![logo](presentation/projectLogo.png) -->
<p align="center">
  <img src="presentation/projectLogo.png" alt="Image Description">
</p>

PathFindingAI is an AI to follow a player in a game.

For a description of the work consult the doc folder.

***
## Features
[!(https://www.youtube.com/watch?v=OsOChnBTgcI)]

The following AI has these features:
- Line of sight: implemented Line Of Sight(in an adapted version for tilegame with grid-based movement) to not track player
that are hidden from a wall.   
- 
- AI register and use the last target's pos: when the "eye contact" is interrupted, no worries, the AI register the last location of the target and move to that location to explore the area.
- Idle: a simple routine to explore the area when there isn't many information about the target position.


***
## Files
The AI is developed in the following files:
- a.cpp
***

