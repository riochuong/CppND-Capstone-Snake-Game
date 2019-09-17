
## CAPSTONE PROJECT

  

### SUMMARY:
I decied to add some fun features for the Snake game as part of the capstone project.

### NEWLY ADDED FEATURES

- Dual Players Mode for Snake game that allow two players to compete and chasing each other depends on the size of the snake to determine the winner.
- Adding menu options to select game mode (single player, dual players).
- Adding increasing and decreasing speed of snakes for both single and dual modes.
- Adding screen to show which player is the winner in dual mode or final score in single mode.
- Modifying **PlaceFood** method to put out number of food matching with number of snakes.

### REQUIRE INSTALLATION
SDL_ttf: `sudo apt-get install libsdl2-ttf-dev`


### BUILD STEPS
- cd <project_workspace>
- mkdir build
- cd build
- cmake ../
- make -j8

#### RUBRIC POINTS
- I added new class **InputDispatcher** with some references passing through the constructor.
- I add couple new UI related method in **main.cpp** that require acquiring and releasing resources similar to pointer semantics through SDL framework
- I use one lambda method in **game.cpp** line 187 for erase foods array.
