// we need the include files io and stdlib and string and stdbool and time
// we need to define a constant for the number of rooms
// we need to define a constant for the maximum number of connections per room
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// we want to write a text based game that is a little bit like a choose your own adventure book
// each turn we will present the player with a choice of actions
// the player will choose an action and we will carry it out
// the player can look at the room, or move to another room

// we will need to keep track of the current room
// we need a way to represent the rooms
// we need a way to represent the connections between the rooms
// let's have a datastructure that represents the rooms
// room has connections and connections have rooms so we need to define connection first but we can't because room needs it
// we can use a forward declaration to tell the compiler that connection is a struct that will be defined later
typedef struct boss{
  char* name;
  char* element;
  int health;
  int damage;
} boss;
typedef struct connection connection;
typedef struct room {
  char* name;
  boss* bossEnemy;
  char* description;
  connection* connections;
  int numConnections;
} room;

// we need a way to represent the connections between the rooms
// let's have a datastructure that represents the connections
typedef struct connection {
  room* room1;
  room* room2;
} connection;
//We need a datastructure for player spells
typedef struct spell{
  char* name;
  char* element;
  int damage;
} spell;
// we need a way to represent the player
// let's have a datastructure that represents the player
typedef struct player {
  room* currentRoom;
  spell* spells;
  int health;
  int baseHealth;
} player;

// we need a way to represent the game
// let's have a datastructure that represents the game
typedef struct game {
  room* rooms;
  boss* bosses;
  int numRooms;
  player* player;
} game;

// let's have a function to print a menu and get user choice, return the choice
int getMenuChoice(player* player) {
  int choice;
  printf("What would you like to do?\n");
  //Check if the room has a boss
  if(player->currentRoom->bossEnemy != NULL){
    printf("1. Challenge %s\n", player->currentRoom->bossEnemy->name);
  }
  else{
    printf("1. Rest by the campfire.\n");
    }
  printf("2. Move to another room\n");
  printf("3. Look at the graveyard\n");
  printf("4. Think upon your accolades\n");
  printf("5. Quit\n");
  
  scanf("%d", &choice);
  // we need to check that the choice is valid
  while (choice < 1 || choice > 5) {
    printf("Invalid choice, please try again\n");
    scanf("%d", &choice);
  }
  return choice;
}

// let's have a function to print the room description
void printRoomDescription(room* room) {
  printf("You are in the %s.\n", room->name);
  printf("%s\n", room->description);
}
void loadGraveyard(){
  FILE* file = fopen("graves.csv", "r");
  if (file == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  //Count the number of lines in the file
  int numLines = 0;
  char line[500];
  while (fgets(line, 500, file) != NULL) {
    numLines++;
  }
  //Rewind the file
  rewind(file);
  //Allocate memory for the grave messages
  char** graves = malloc(sizeof(char*) * numLines);
  //Read the grave messages from the file
  printf("The omniscient narrator reads the tombstones:\n");
  // read each line of the file
  for(int i = 0; i < numLines; i++){
    fgets(line, 500, file);
    // remove the newline character
    line[strlen(line)-1] = '\0';
    // print the line
    printf("%s\n\n", line);
  }
}

// a function to get user choice of room to move to
int getRoomChoice(room* currentRoom) {
  int choice;
  printf("Which room would you like to move to?\n");
  for (int i = 0; i < currentRoom->numConnections; i++) {
    printf("%d. %s\n", i+1, currentRoom->connections[i].room2->name);
  }
  scanf("%d", &choice);
  // we need to check that the choice is valid
  while (choice < 1 || choice > currentRoom->numConnections) {
    printf("Invalid choice, please try again\n");
    scanf("%d", &choice);
  }
  return choice;
}

// a function to move the player to another room, and describe it to the user
void movePlayer(player* player, int choice) {
  player->currentRoom = player->currentRoom->connections[choice-1].room2;
  printRoomDescription(player->currentRoom);
}

spell* loadSpells(){
  //Open the file and make array of spells
  FILE* file = fopen("spells.csv", "r");
  if (file == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  //Count the number of lines in the file
  int numLines = 0;
  char line[501];
  while (fgets(line, 501, file) != NULL) {
    numLines++;
  }
  //Rewind the file
  rewind(file);
  //Allocate memory for the spells
  spell* spells = malloc(sizeof(spell) * numLines);
  //Read the spells from the file
  for(int i = 0; i < numLines; i++){
    fgets(line, 501, file);
    line[strlen(line)-1] = '\0';
    //Split the line into the name, element, and damage
    char* name = strtok(line, ",");
    char* element = strtok(NULL, ",");
    char* damageString = strtok(NULL, ",");
    //Convert the damage string into an int
    int damage = atoi(damageString);
    //fix the damage for last spell
    if(damage < 10){
      damage *= 10;
    }
    //Create a spell
    spell currentSpell;
    currentSpell.name = strdup(name);
    currentSpell.element = strdup(element);
    currentSpell.damage = damage;
    //Add the spell to the array
    spells[i] = currentSpell;
    printf("%s\n", spells[i].name);
    printf("%s\n", spells[i].element);
    //printf("%d\n", spells[i].damage);
  }
  fclose(file);
  return spells;
}

boss* loadBosses() {
  // Open the file
  FILE* file = fopen("enemies.csv", "r");
  // Check that the file opened successfully
  if (file == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  // Count the number of lines in the file
  int numLines = 0;
  char line[500];
  while (fgets(line, 500, file) != NULL) {
    numLines++;
  }
  // Rewind the file
  rewind(file);
  // Allocate memory for the bosses
  boss* bosses = malloc(sizeof(boss) * numLines);

  // Read the bosses from the file
  for (int i = 0; i < numLines; i++) {
    fgets(line, 500, file);
    line[strlen(line)-1] = '\0';

    // Split the line into the name, element, health, and damage
    char* name = strtok(line, ",");
    char* element = strtok(NULL, ",");
    char* healthString = strtok(NULL, ",");
    char* damageString = strtok(NULL, ",");

    // Convert the health and damage strings into ints
    int health = atoi(healthString);
    int damage = atoi(damageString);

    // Create a boss
    boss currentBoss;
    currentBoss.name = strdup(name);
    currentBoss.element = strdup(element);
    currentBoss.health = health;
    currentBoss.damage = damage;

    // Add the boss to the array
    bosses[i] = currentBoss;
  }

  fclose(file);


  return bosses;
}

// a function to load the rooms from a file
// the file is called rooms.csv, and has a room name and room description on each line
// the function returns an array of rooms
room* loadRooms(boss* bosses) {
  // open the file
  FILE* file = fopen("rooms.csv", "r");
  // we need to check that the file opened successfully
  if (file == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  // we need to count the number of lines in the file
  int numLines = 0;
  char line[500];
  while (fgets(line, 500, file) != NULL) {
    numLines++;
  }
  // we need to rewind the file
  rewind(file);
  // we need to allocate memory for the rooms
  room* rooms = malloc(sizeof(room) * numLines);
  // we need to read the rooms from the file
  for (int i = 0; i < numLines; i++) {
    // we need to read the line
    fgets(line, 500, file);
    // we need to remove the newline character
    line[strlen(line)-1] = '\0';
    // we need to split the line into the name and description
    // the strings are in quotation marks, and the two quoted strings are separated by a comma
    // we need to split the line on ", " (the four characters)
    //everything between the first and second " is the name
    //everything between the third and fourth " is the description
    // we need to find the first "
    char* name = strtok(line, "\"");
    // we need to find the second " and record where it is so we can null terminate the string
    char* endofname=strtok(NULL, "\"");
    
    
    // we need to find the third "
    char* description = strtok(NULL, "\"");
    // we need to find the fourth "
    char* endofdesc=strtok(NULL, "\0");
    //we need to be sure that name and description are null terminated strings
    name[endofname-name]='\0';
    //description[endofdesc-description]='\0';
    // we need to create a room
    room room;
    //we need to copy the string into the room name
    room.name = malloc(sizeof(char) * strlen(name) + 1);
    strcpy(room.name, name);
    //we need to copy the string into the room description
    room.description = malloc(sizeof(char) * strlen(description) + 1);
    strcpy(room.description, description);
    room.connections = NULL;
    room.numConnections = 0;
    // we need to add the room to the array
    rooms[i] = room;
    // we need to add the boss to the room
    rooms[i].bossEnemy = &bosses[i];
  }
  // we need to close the file
  fclose(file);

  // we need to create a maze like set of connections between the rooms
  // we need to loop through the rooms
  //let's pick a number between 1 and 3 for the number of connections for each room
  srand(time(NULL));
  for (int i = 0; i < numLines; i++) {
    // we need to pick a random number between 1 and 3
    int numConnections = rand() % 3 + 1;
    int roomToConnectTo;
    // we need to loop numConnections times
    for (int j = 0; j < numConnections; j++) {
      // we need to pick a random room to connect to
      roomToConnectTo = rand() % numLines;
      // we need to create a connection between the rooms
      connection connection;
      connection.room1 = &rooms[i];
      connection.room2 = &rooms[roomToConnectTo];
      // we need to add the connection to the room
      rooms[i].connections = realloc(rooms[i].connections, sizeof(connection) * (rooms[i].numConnections + 1));
      rooms[i].connections[rooms[i].numConnections] = connection;
      rooms[i].numConnections++;
      //and don't forget to add the connection to the other room
      rooms[roomToConnectTo].connections = realloc(rooms[roomToConnectTo].connections, sizeof(connection) * (rooms[roomToConnectTo].numConnections + 1));
      rooms[roomToConnectTo].connections[rooms[roomToConnectTo].numConnections] = connection;
      rooms[roomToConnectTo].numConnections++;
    }
  }
  // we need to return the array of rooms
  return rooms;
}

// let's have a function to create a player
player* createPlayer(room* currentRoom) {
  // we need to allocate memory for the player
  player* player = malloc(sizeof(player));
  //Set the spells
  player->spells = loadSpells();
  // we need to set the current room
  player->currentRoom = currentRoom;
  // We need to set bosses defeated to 0
  // we need to return the player
  return player;
}

// let's have a function to create a game
game* createGame() {
  // we need to allocate memory for the game
  game* game = malloc(sizeof(game));
  // we need to load the bosses
  game->bosses = loadBosses();
  // we need to load the rooms
  //printf("debug - about to load rooms\n");
  game->rooms = loadRooms(game->bosses);
  //printf("debug - rooms loaded\n");
  // we need to set the number of rooms
  game->numRooms = 10;
  // we need to create the player
  game->player = createPlayer(&game->rooms[0]);
  // we need to return the game
  return game;
}
//A function to write the players' obituary
void theGraveyard(player* player, bool legend){
  printf("You have met your end in the %s.\n", player->currentRoom->name);
  printf("Who does this tombstone belong to?\n");
  char name[100];
  scanf("%s", name);
  FILE* file = fopen("graves.csv", "a");
  //Print the player's name and the room they died in to the file
  //Print what killed them
  // <player> could not overome <boss name> in the <room name>
  //Random between three phrases:
  // Could not overcome, was defeated by, underestimated
  srand(time(NULL));

  // Array of phrases
  if(legend == false){
    //The player died to a boss
    char* phrases[] = {"could not overcome", "was defeated by", "underestimated"};
    int phraseIndex = rand() % 3;  // Generate a random index
  // Use the random phrase in fprintf
    fprintf(file, "%s in the %s %s %s!\n", name, player->currentRoom->name, phrases[phraseIndex], player->currentRoom->bossEnemy->name);
    printf("Your tombstone has been added to the graveyard.\n");
    printf("%s in the %s %s %s!\n", name, player->currentRoom->name, phrases[phraseIndex], player->currentRoom->bossEnemy->name);
  } else {
    //The player died as a legend
    fprintf(file,"%s lived a legendary life, and will be remembered forever.\n", name);
    printf("After many years, Your tombstone has been added to the graveyard.\n");
    printf("%s lived a legendary life, and will be remembered forever.\n", name);
  }

  //end the game
  exit(0);
}
// For Choice 1, we need a combat system.
//Use a function to play 
void fightBoss(player* player) {
  // Get the player's current room
  room* currentRoom = player->currentRoom;
  // Get the room's boss
  boss* roomBoss = currentRoom->bossEnemy;
  printf("ARENA: %s\n", currentRoom->name);
  printf("OPPONENT: %s\n", roomBoss->name);
  printf("OPPONENT HEALTH: %d\n", roomBoss->health);
  printf("OPPONENT ELEMENT: %s\n", roomBoss->element);

  // Set the player's health to 100
  // We need to give the player a choice of spells to use
  // The player can choose to use a spell or try to run
  // Once used, the boss will attack the player
  // Both the player and the boss have a chance to crit
  // We must compare elements
  // If a spell element is the same as the boss element, no damage is dealt

  //Fatigue threshold for using the same spell consecutively, encourage the player to use different spells
  int fatigueThreshold = 15;

  int choice;
  printf("What is your strategy?\n");
  printf("1. Use a spell\n");
  printf("2. Run\n");
  scanf("%d", &choice);
  // we need to check that the choice is valid
  while (choice < 1 || choice > 2) {
    printf("Invalid choice, please try again\n");
    scanf("%d", &choice);
  }

  if (choice == 1) {
    // Print the player's spells
    printf("Your spells:\n");
    // for loop equal to the number of spells
    for (int i = 0; i < 5; i++) {
      printf("%d. %s\n", i + 1, player->spells[i].name);
      printf("Element: %s\n", player->spells[i].element);
      
    }
    // Get the player's choice of spell
    int lastSpellUsed = -1; // Initialize to an invalid index
    while (roomBoss->health > 0 && player->health > 0) {
      int spellChoice;
      printf("Which spell would you like to use?\n");
      scanf("%d", &spellChoice);
      // we need to check that the choice is valid
      while (spellChoice < 1 || spellChoice > 5) {
        printf("Invalid choice, please try again\n");
        scanf("%d", &spellChoice);
      }
      // Check if the player is using the same spell as the last turn
      if (spellChoice - 1 == lastSpellUsed) {
        printf("You can't use the same spell consecutively! Choose a different spell.\n");
      } else {
        // Get the spell the player chose
        spell* chosenSpell = &player->spells[spellChoice - 1];
        
        // Check if the spell element is the same as the boss element
        if (strcmp(chosenSpell->element, roomBoss->element) == 0) {
          printf("You used %s!\n", chosenSpell->name);
          printf("The attack is a %s attack!\n", chosenSpell->element);
          printf("%s took no damage!\n", roomBoss->name);
        } else {
          // If the spell element is not the same as the boss element
          // Randomise the damage between 40% and 120% of the spell's damage
          int trueDamage = chosenSpell->damage * (rand() % 80 + 40) / 100;
          printf("You used %s!\n", chosenSpell->name);
          //Print if critical hit
          if(trueDamage > chosenSpell->damage){
            printf("The attack was super effective!\n");
          }
          printf("You dealt %d damage!\n", trueDamage);
          
          // Apply fatigueThreshold if the same spell is used consecutively
          if (spellChoice - 1 == lastSpellUsed) {
            roomBoss->health -= (chosenSpell->damage - fatigueThreshold);
          } else {
            roomBoss->health -= chosenSpell->damage;
          }
          
          printf("%s has %d health left!\n", roomBoss->name, roomBoss->health);
          
          // Update lastSpellUsed
          lastSpellUsed = spellChoice - 1;
        }

        // Boss's turn if the boss is still alive
        if (roomBoss->health > 0) {
          printf("%s Attacked You!\n", roomBoss->name);
          printf("You took %d damage!\n", roomBoss->damage);
          player->health -= roomBoss->damage;
          printf("You have %d health left!\n", player->health);
        }
      }
    }

    // Check if the player or the boss won
    if (player->health <= 0) {
      printf("Game Over! You were defeated by %s.\n", roomBoss->name);
      // Player has been placed in the graveyard
      theGraveyard(player, false);
    } else if (roomBoss->health <= 0) {
      printf("Congratulations! You defeated %s.\n", roomBoss->name);
      // Remove the boss from the room
      player->currentRoom->bossEnemy = NULL;
    }
  } else {
    // The player chose to run
    printf("You ran away from %s.\n", roomBoss->name);
    // Get a random number between 0 and 4, if 11, the player dies in their escape
    int escapeChance = rand() % 4;
    if (escapeChance > 2) {
      printf("You were killed while trying to escape.\n");
      // Player has been placed in the graveyard
      theGraveyard(player, false);
    } else {
      printf("You escaped safely.\n\n");
      // the player is moved to a random room
      // Random number between 0 and 9
      int randomRoom = rand() % 10;
      player->currentRoom = &player->currentRoom[randomRoom];
    }
  }
}

// let's have a function to play the game which is the main function
void playGame() {
  // we need to create the game
  printf("Welcome to the game\n");
  game* game = createGame();
  //Set the player's health to 100
  game->player->health = 100;
  game->player->baseHealth = 100;
  //Set the bosses defeated to 0
  // we need to print the room description
  printRoomDescription(game->player->currentRoom);
  // we need to loop until the user quits
  bool quit = false;
  while (!quit) {
    // we need to print the menu and get the user choice
    int choice = getMenuChoice(game->player);
    // we need to carry out the user choice
    if(choice == 1 && game->player->currentRoom->bossEnemy != NULL)
    {
      fightBoss(game->player);
      //if code has not been stopped, the player has defeated the boss
      //Add one to the bosses defeated

  
    } else if(choice == 1 && game->player->currentRoom->bossEnemy == NULL){
      //Look at the boss's tombstone
      printf("The boss has fallen. Sit and rest.\n");
      printf("You absorb their soul and regain your health.\n");
      //MAKE SURE THE CODE BELOW CAN BE DONE ONCE
      bool hasRested = false;
      if(hasRested == false){
        game->player->health = game->player->baseHealth + 20;
        game->player->baseHealth = game->player->health;
        hasRested = true;
      }
      printf("You have %d health.\n", game->player->health);

    }
    else if (choice == 2) {
      // we need to get the user choice of room to move to
      int choice = getRoomChoice(game->player->currentRoom);
      // we need to move the player to the room
      movePlayer(game->player, choice);
    } else if (choice == 3) {
      // we need to quit
      loadGraveyard();
    }
    else if (choice == 4) {
      //print the player's health and spells
      printf("You have %d health.\n\n", game->player->health);
      printf("Your spells:\n");
      // for loop equal to the number of spells
      for (int i = 0; i < 5; i++) {
        printf("%d. %s\n", i + 1, game->player->spells[i].name);
        printf("Element: %s\n", game->player->spells[i].element);
        printf("Damage: %d\n", game->player->spells[i].damage);
      }
      printf("\n");
    }
    
    else if (choice == 5) {
      // we need to quit
      quit = true;
    }

  }
  //There are 10 bosses, if the player has defeated all of them, they win
 // if(game->bossesDefeated == 10){
   //printf("Congratulations! You have defeated all the bosses!\n");
    //printf("You win!\n");
    // Open the graveyard
    //theGraveyard(game->player, true);

  //}
}

// let's have a main function to call the playGame function
int main() {
  playGame();
  return 0;
}










