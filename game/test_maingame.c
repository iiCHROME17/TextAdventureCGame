#include <stdio.h>

int canReachLastRoom(int currentRoom) {
    if (currentRoom == 9) {
        return 1; // We have reached the last room
    } else if (currentRoom > 9) {
        return 0; // We have gone beyond the last room
    } else {
        // Recursive call to check if we can reach the last room from the next room
        return canReachLastRoom(currentRoom + 1);
    }
}

int main() {
    int startingRoom = 0;
    int result = canReachLastRoom(startingRoom);
    
    if (result) {
        printf("We can reach the last room starting from room %d\n", startingRoom);
    } else {
        printf("We cannot reach the last room starting from room %d\n", startingRoom);
    }
    
    return 0;
}