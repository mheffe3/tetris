#include "Piece.h"

const byte Piece::PIECE_DIMENSIONS[7][2] = {{4,4}, {3,3}, {3,3}, {3,4}, {3,3}, {3,3}, {3,3}};
const byte Piece::PIECE_BOARD_COORDINATES[7][4] = {{2,5,1,1}, {2,4,0,1}, {2,4,0,1}, {3,4,0,1}, {2,4,0,1}, {2,4,0,1}, {2,4,0,1}};  // xBoardLeft, xBoardRight, yBoardUp, yBoardLow
const byte Piece::PIECE_ARRAY_COORDINATES[7][4] = {{0,3,1,1}, {0,2,0,1}, {0,2,0,1}, {1,2,0,1}, {0,2,0,1}, {0,2,0,1}, {0,2,0,1}};  // xArrayLeft, xArrayRight, yArrayUp, yArrayLow

#include "Arduino.h"

const byte Piece::PIECE_TEMPLATES[7][4][4] = {
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // I piece
    {{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // J piece
    {{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // L piece
    {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // O piece
    {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // S piece
    {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // T piece
    {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // Z piece
};

// Creates a 2d array representing the tetris piece
void Piece::createArray() {
    for (int i = 0; i < MAX_X_SIZE; i++) {
        for (int j = 0; j < MAX_Y_SIZE; j++) {
            pieceArray[i][j] = PIECE_TEMPLATES[type][i][j];
        }
    }
}

Piece::Piece(const int PIECE_TYPE, const int NUM_COLS) {
    type = PIECE_TYPE;
    width = PIECE_DIMENSIONS[type][1];
    height = PIECE_DIMENSIONS[type][0];

    currentWidth = width;
    currentHeight = height;

    int middleOfRow = (NUM_COLS - width) / 2;
    x = middleOfRow;
    y = 0;

    xBoardLeft = PIECE_BOARD_COORDINATES[type][0];
    xBoardRight = PIECE_BOARD_COORDINATES[type][1];
    yBoardUp = PIECE_BOARD_COORDINATES[type][2];
    yBoardLow = PIECE_BOARD_COORDINATES[type][3];

    xArrayLeft = PIECE_ARRAY_COORDINATES[type][0];
    xArrayRight = PIECE_ARRAY_COORDINATES[type][1];
    yArrayUp = PIECE_ARRAY_COORDINATES[type][2];
    yArrayLow = PIECE_ARRAY_COORDINATES[type][3];

    this->createArray();
}

void Piece::copyArray() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            copy[i][j] = pieceArray[i][j];
        }
    }
}

void Piece::rotate90C() {
  Serial.println("C");
  byte xMax = 0, xMin = width-1, yMin = height-1, yMax = 0;
  signed char xRightDelta = 0, xLeftDelta = 0, yUpDelta = 0, yLowDelta = 0;
  for(byte i = 0; i < width; i++) {
    for(byte j = 0; j < height; j++) {
      pieceArray[i][j] = copy[height-1-j][i];
      Serial.println("xMax: " + String(xMax) + " xMin: " + String(xMin));
      if (pieceArray[i][j] == 1) {
        if (j > xMax) {
          xMax = j;
        }
      }

      if (pieceArray[i][j] == 1) {
        if (j < xMin){
          xMin = j;
        }
      }

      if (pieceArray[i][j] == 1) {
        if (i > yMax){
          yMax = i;
        }
      }

      if (pieceArray[i][j] == 1) {
        if (i < yMin) {
          yMin = i;
        }
      }
      
    }
  }
  Serial.println("final: xMax: " + String(xMax) + " xMin: " + String(xMin));
//  if (xMax != width-1) {
//    xRightDelta = xMax - xArrayRight-1;
//    xLeftDelta = xMin - xArrayLeft-1;
//  }
//  else if (xMin != 0) {
//    xRightDelta = xMax - xArrayRight+1;
//    xLeftDelta = xMin - xArrayLeft+1;
//  } else {
    xRightDelta = xMax - xArrayRight;
    xLeftDelta = xMin - xArrayLeft;
  //}
  yLowDelta = yMax - yArrayLow;
  yUpDelta = yMin - yArrayUp;
  
  xArrayRight=xMax;
  xArrayLeft=xMin;
  yArrayLow=yMax;
  yArrayUp=yMin;

  xBoardRight+=xRightDelta;
  xBoardLeft+=xLeftDelta;
  yBoardLow+=yLowDelta;
  yBoardUp+=yUpDelta;

  if (xBoardLeft < 0) {
    x +=(0-xBoardLeft);
    xBoardRight += (0-xBoardLeft);
    xBoardLeft = 0;
  }

  if (xBoardRight > 7) {
    x +=(7-xBoardRight);
    xBoardLeft += (7-xBoardRight);
    xBoardRight = 7;
  }

  if (yBoardLow > 31) {
    y +=(31-yBoardLow);
    yBoardUp += (31-yBoardLow);
    yBoardLow = 31;
  }

  Serial.println("xBoardRight: " + String(xBoardRight) + " xBoardLeft: " + String(xBoardLeft));

}

void Piece::rotate90CC() {
  Serial.println("CC");
  byte xMax = 0, xMin = width-1, yMin = height-1, yMax = 0;
  signed char xRightDelta = 0, xLeftDelta = 0, yUpDelta = 0, yLowDelta = 0;
  for(byte i = 0; i < width; i++) {
    for(byte j = 0; j < height; j++) {
      pieceArray[i][j] = copy[j][width-1-i];
      Serial.println("xMax: " + String(xMax) + " xMin: " + String(xMin));

      if (pieceArray[i][j] == 1) {
        if(j > xMax) {
          xMax = j;
        }
      }

      if (pieceArray[i][j] == 1) {
        if (j < xMin){
          xMin = j;
        }
      }

      if (pieceArray[i][j] == 1) {
        if (i > yMax) {
          yMax = i;
        }
      }

      if (pieceArray[i][j] == 1) {
        if(i < yMin) {
          yMin = i;
        }
      }
    }
  }
  Serial.println("final: xMax: " + String(xMax) + " xMin: " + String(xMin));
  //
//  if (xMax != width-1) {
//    xRightDelta = xMax - xArrayRight-1;
//    xLeftDelta = xMin - xArrayLeft-1;
//  }
//  else if (xMin != 0) {
//    xRightDelta = xMax - xArrayRight+1;
//    xLeftDelta = xMin - xArrayLeft+1;
//  } else {
    xRightDelta = xMax - xArrayRight;
    xLeftDelta = xMin - xArrayLeft;
  //}
  yLowDelta = yMax - yArrayLow;
  yUpDelta = yMin - yArrayUp;
  
  xArrayRight = xMax;
  xArrayLeft = xMin;
  yArrayLow = yMax;
  yArrayUp = yMin;

  xBoardRight+=xRightDelta;
  xBoardLeft+=xLeftDelta;
  yBoardLow+=yLowDelta;
  yBoardUp+=yUpDelta;

  if (xBoardLeft < 0) {
    x +=(0-xBoardLeft);
    xBoardRight += (0-xBoardLeft);
    xBoardLeft = 0;
  }

  if (xBoardRight > 7) {
    x +=(7-xBoardRight);
    xBoardLeft += (7-xBoardRight);
    xBoardRight = 7;
  }

  if (yBoardLow > 31) {
    y += (31-yBoardLow);
    yBoardUp += (31-yBoardLow);
    yBoardLow = 31;
  }
  Serial.println("xBoardRight: " + String(xBoardRight) + " xBoardLeft: " + String(xBoardLeft));
}

void Piece::rotate(String direction) {
  if (type != 3) {
    copyArray();
    if (direction == "Right") {
        rotate90C();
    } else {
        rotate90CC();
    }
  }
}

/*
    Moves the piece in the x direction on a grid
    - if x is 1 => moves left,  if x is -1 => moves right
    - if y is -1 => moves down
    - if y is 1 => not valid throw error
*/

void Piece::move(int xDirection, int yDirection) {
    bool inputValid =
        (abs(xDirection) == 1 ||
         xDirection == 0) &&  // xDirection can either be -1, 1, or 0
        (abs(yDirection) == 1 ||
         yDirection == 0) &&  // yDirection can either be -1, 1, or 0
        (yDirection == 0 ||
         xDirection == 0);  // One of the two variables must be 0

    if (!inputValid) {
        return;
    }

    if (xBoardRight + xDirection > 7 || xBoardLeft + xDirection < 0) {
        return;
    }
    if (yBoardLow + yDirection > 31 || yBoardUp + yDirection < 0) {
        return;
    }
    
    x += xDirection;
    xBoardRight+=xDirection;
    xBoardLeft+=xDirection;
    y += yDirection;
    yBoardUp+=yDirection;
    yBoardLow+=yDirection;
    Serial.println("x: " + String(x) + " Left: " + String(xBoardLeft));
}

void Piece::hardDrop() {
    /*
     *
     * TODO: this pseudo code assumes x,y is top left corner -- double check
     * need a way to get the grid in this function
     * talk to matt to see how he uses grid in piece
     *
     * from y = first row to last row do
     *  from piece x to piece x + width of x - 1:
     *    int belowUs = if y isn't the bottom y+1 else y
     *    if below us us a block:
     *      let piece's y be current y in for loop
     *      return
     *
     */
}
