# Tetris-SFML-
A Tetris game made in C++ using SFML library.

![tetris_scrnshot](https://user-images.githubusercontent.com/31830553/69095053-d88d7780-0a51-11ea-931b-ee0abbd12080.png)

## Features
It's Tetris.

Use the left/right arrow keys to adjust the horizontal position of a falling piece.
Use the 'up' arrow to rotate a falling piece.
Use the 'down' arrow to increase the falling-speed of a piece.

Build horizontal lines of blocks/tiles for the lines to 'collapse' and gain points from them.
The game is over when the block/tiles (attempt to) reach outside of the window on the y-axis.

## Software Analysis
It's made using C++ and the SFML library. I'm using these as C++ is my preferred language and SFML is a reasonably lightweight graphics library that fits a task like this. 

My second pick would have been Javascript. It was quite hard for me to decide between the two. Tetris is a game perfect to run in the browser, and Javascript itself is very lightweight and does not require any additional (graphics) libraries to display the game since you can just use its built-in canvas. I ended up choosing C++/SFML anyway since I liked the idea of having my own desktop-version of the game and I didn't want to make it myself too easy.

I also considered using Java, but quickly dismissed that idea as I did not see any specific advantage of doing so except that it might be more portable. If I had decided to use it, then I'd probably have used JavaFX to display the game's visuals.

The thoughts of using either the Unity engine or the 'Phaser' framework also crossed my mind, but were both quickly dismissed as using Unity would have been overkill, and using Phaser would just have been unnecessary since native Javascript already has everything that you might need for a task like this.

Usually when making games, I directly manipulate the objects that get visibly drawn to the screen, but for this project I'll be manipulating a 2D array of characters. I'll then use SFML to 'read' my array of characters and represent them on the screen using a specific texture for each character.

## Learning purpose
Tetris has been on my bucket list for quite a while. It's more complicated than Snake mainly due to the math required to rotate the falling pieces, but also due to the collision physics involved in the game. Still, I already have quite a good grasp on both of these subjects so I think the task will be easy enough for me to complete within a week.

I think making your own version of a 'simple-but-classic' game such as Tetris is usually a nice little programming challenge since they all have a certain unique mechanic that serves as a new fresh-feeling problem. In 'Tetris' it's the rotating pieces and collision, in 'Snake' it's the player's movement behaviour, in 'Pacman' it's the ghost AI's, etc.

## Planning 
I will approach this task by doing the complicated work first; meaning that I won't be touching the game's window and outer visuals until all the inner logic of it is (as good as) finished.

As I said earlier, I think the task is easy enough to finish within a week. When I'm done, I'll make a desktop shortcut linking to the executable, and give it a thumbnail; just for the satisfaction =)

| | Monday | Tuesday | Wednesday | Thursday | Friday |
| --- | --- | --- | --- | --- | --- |
||Setup SFML. Create a window, test the input, and make the array. Also make a 'piece' class/struct and a Vector struct.|Start working on making the pieces rotate. Use the console as a temporary visual aspect. Also setup a 'delta-time' variable and start letting pieces fall down tick by tick.|Implement collision. Next is to visualize everything with textures in the SFML window. Make some textures in paint, or get some from the internet.|Continue visualizing the game.|Final day. Optimize the code and get rid of any potential bugs.

## Sources
I'm using older projects for reference.
