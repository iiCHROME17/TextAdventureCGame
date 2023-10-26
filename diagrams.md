```mermaid
graph TD

// Start
start[Start]

// Load Rooms
start --> loadRooms[Load Rooms]

// Print Room Description
loadRooms --> printRoomDescription[Print Room Description]

// Get User Choice
printRoomDescription --> getMenuChoice[Get User Choice]

// User Chooses to Look Around
getMenuChoice --> printRoomDescription

// User Chooses to Move to Another Room
getMenuChoice --> getRoomChoice[Get Room Choice]

// User Quits
getMenuChoice --> quitGame[Quit]

// Move Player
getRoomChoice --> movePlayer[Move Player]

// Loop Until Quit
movePlayer --> getMenuChoice

// Player Quits
quitGame --> stop[Stop]

// End
stop[End]

subgraph loadRoomsSubgraph
  style subgraph fill:#f9f9f9,stroke:#333,stroke-width:2px;

  // Load Rooms
  loadRooms[Load Rooms]

  // Open File
  loadRooms --> openFile[Open File]

  // File Opened Successfully
  openFile --> checkFile[File Opened Successfully]

  // File Open Error
  openFile --> exit[File Open Error]

  // Count Number of Lines
  checkFile --> countLines[Count Number of Lines]

  // Rewind File
  countLines --> rewindFile[Rewind File]

  // Allocate Memory for Rooms
  rewindFile --> allocateMemory[Allocate Memory for Rooms]

  // Read Rooms from File
  allocateMemory --> readRooms[Read Rooms from File]

  // Close File
  readRooms --> closeFile[Close File]

  // Create Connections Between Rooms
  closeFile --> createConnections[Create Connections Between Rooms]

  // Return the Array of Rooms
  createConnections --> returnRooms[Return the Array of Rooms]

end
```
