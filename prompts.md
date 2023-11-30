# AI Prompts

## 1. Task 4:Adding Spell structure (CHATGPT)

### Input:
Whole Script + Create a similar datastructure to contain spells which each spell has a name,element and damage.

### Output:
We need a data structure for player spells

//We need a datastructure for player spellstypedef struct spell{
char* name;
char* element;
int damage;
} spell;


## 2. Task 4:Adding Boss structure (CHATGPT)

### Input:
Do the same thing but for bosses with a name,element,health and damage.

### Output:
typedef struct boss{
  char* name;
  char* element;
  int health;
  int damage;
} boss;


## 3. Task 4: Load Spells function (CHATGPT)

### Input: 
<Load Rooms Function snippet> Using this as a template, create a function to load Spells.

### Output:
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


## 3.2 Task 4: Load Spells function (CoPilot)

### Input:
the format of the csv is name, element and damage, store this into a spell per line. Damage needs to be an int

### Output:
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
    //printf("%s\n", spells[i].name);
    //printf("%s\n", spells[i].element);
    //printf("%d\n", spells[i].damage);
  }
  fclose(file);
  return spells;
## 5 Task 4: Load Bosses function  (CoPilot)

### Input: 
Do the exact same thing for load bosses but it is name, element, health (int), damage (int) *


### Output:
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
    

    //fix the damage for last boss
    if(damage < 10){
      damage *= 10;
    }
    currentBoss.damage = damage;
    // Add the boss to the array
    bosses[i] = currentBoss;
  }

  fclose(file);


  return bosses;
}
* Few lines required tweaks to them

## 6 Task 4: Fight Function (CoPilot)

### Input
<Fight Function snippet> Here, make sure if the spells->element == boss->element than the boss takes no damage

## Output
/ Check if the spell element is the same as the boss element
        if (strcmp(chosenSpell->element, roomBoss->element) == 0) {
          printf("You used %s!\n", chosenSpell->name);
          printf("The attack is a %s attack!\n", chosenSpell->element);
          printf("%s took no damage!\n", roomBoss->name);
        } else { ...


## 7. Task 3: GetConnections Function (CHATGPT)

### Input
<Whole code> We need to need to create a function that returns an integer array. Use connections.csv to get the ints in format x,y,z and add all of them into a single array. There are about 30 ints. E.g 0,4,7 \n 3,2,2 need to be one array 0,4,7,3,2,2.

### Output (tweaked)
int* getConnections() {
  // Open the file
  FILE* file = fopen("connections.csv", "r");
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
  // Allocate memory for the connections
  int* dataArray = malloc(sizeof(int) * numLines * 3);
  // Read the connections from the file
  for (int i = 0; i < numLines; i++) {
    fgets(line, 500, file);
    char* token = strtok(line, ",");
    for (int j = 0; j < 3; j++) {
      dataArray[i * 3 + j] = atoi(token);
      token = strtok(NULL, ",");
    }
  }
  fclose(file);
  return dataArray;
}
## 8. Task 3: Enhance room code

### Input
Change roomToConnectTo from a random int to something in the range of i to i+3. This means room 0 connects to index 0,1,2 and room 2 connects to 3,4,5 ... room 9 connects to 27,28,29. If the room is equal to itself when connection , add one unless it is the final room.

### Output
  srand(time(NULL));
  int* dataArray = getConnections();
  for (int i = 0; i < numLines; i++) {
    for(int j = 0; j < 3; j++){
 
      
      //if the room is trying to connect to itself, add 1 to the room it is trying to connect to, unless it is the last room
      if(dataArray[i * 3 + j] == i && i != numLines - 1){
        dataArray[i * 3 + j]++;
      }

      //For each room, we need to connect to three rooms of dataArray
      //Room 0 = 0, 1, 2 | Room 1 = 3, 4, 5 | Room 2 = 6, 7, 8 | ... Room 9 = 27, 28, 29
      connection connection;
      connection.room1 = &rooms[i];
      connection.room2 = &rooms[dataArray[i * 3 + j]];

      // we need to add the connection to the room
      rooms[i].connections = realloc(rooms[i].connections, sizeof(connection) * (rooms[i].numConnections + 1));
      rooms[i].connections[rooms[i].numConnections] = connection;
      rooms[i].numConnections++;
      //Dont forget to add the connection to the other room
      rooms[dataArray[i * 3 + j]].connections = realloc(rooms[dataArray[i * 3 + j]].connections, sizeof(connection) * (rooms[dataArray[i * 3 + j]].numConnections + 1));
      rooms[dataArray[i * 3 + j]].connections[rooms[dataArray[i * 3 + j]].numConnections] = connection;
      rooms[dataArray[i * 3 + j]].numConnections++;

## 9 Task 4: Add Boss counter (CHATGPT)

### Input
- <Whole Code> add to the player an int to track bosses
- Increment it ONLY ONCE per boss defeated
- If Checking our diary (option) prinnt number of bossesDefeated
- If the Boss of Room 9 has been defeated (the unchronogically final boss) then call thegreaveyard and set the legend parameter to true

##Output
- typedef struct player {
  room* currentRoom;
  spell* spells;
  int health;
  int baseHealth;
  int bossesDefeated;
} player;
- player* createPlayer(room* currentRoom) {
  // we need to allocate memory for the player
  player* player = malloc(sizeof(player));
  //Set the spells
  player->spells = loadSpells();
  //Set Bosses Defeated to 0
  player->bossesDefeated = 0;
  // we need to set the current room
  player->currentRoom = currentRoom;
  // we need to return the player
  return player;
}
-     bool canIncrement = true;
      if(canIncrement == true){
        player->bossesDefeated++;
        canIncrement = false;
      }
      printf("You have slain %d bosses.\n\n", player->bossesDefeated);
      // Remove the boss from the room
      player->currentRoom->bossEnemy = NULL;
-     else if (choice == 4) {
      //print the player's health and spells
      printf("You have %d health.\n\n", game->player->health);
      printf("You have defeated %d bosses.\n\n", game->player->bossesDefeated);
      printf("Your spells:\n");
      // for loop equal to the number of spells
      for (int i = 0; i < 5; i++) {
        printf("%d. %s\n", i + 1, game->player->spells[i].name);
        printf("Element: %s\n", game->player->spells[i].element);
        printf("Damage: %d\n", game->player->spells[i].damage);
      }
      printf("\n");
    }

## Links to CHATGPT Prompts
https://chat.openai.com/share/32535671-7853-42eb-911e-d635f214a13d
https://chat.openai.com/share/d946c52f-d84f-4232-939f-bfda97f1d137
https://chat.openai.com/share/b2f7f775-43e0-4bd9-9e3a-47615c7e2a91
https://chat.openai.com/share/1e2ca048-2b24-44bb-8aa9-b4cce5666b00
https://chat.openai.com/share/5c8fd072-f81f-40a9-a445-c880306e0cb2
https://chat.openai.com/share/abc0850e-29bc-47c5-8854-4ce5d8af0163
https://chat.openai.com/share/6582ee4d-c9a5-4077-9320-ff6e4bce14bb
https://chat.openai.com/share/eb01f800-2346-4461-8c64-eac2bf124188
https://chat.openai.com/share/99e558c8-6fa9-44b6-a468-ccb55191cfff
https://chat.openai.com/share/197f9941-2617-48af-bd75-1b03ca3c89a5
https://chat.openai.com/share/26a55a95-7bb4-454e-96ef-8f4ef9b0c1b7
https://chat.openai.com/share/901364bf-4127-410e-ac97-621ac905ca10
https://chat.openai.com/share/4c1f3bac-502f-4343-b19f-0ef11841d80a
