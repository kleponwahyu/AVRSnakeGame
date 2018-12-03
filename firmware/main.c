/* Name: main.c
 * Author: Wahyu Berlianto
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <util/delay.h>

struct Location {
  int x, y;
};
struct Location currentLocation[128];
int displayLoop, snakePath, displayCount, snakeLength, snakeLengthCounter, eating = 0, lose, differentPath = 0;

struct Location food;

void borderCheck() {
  if(currentLocation[0].x < 0 || currentLocation[0].x > 15 || currentLocation[0].y < 0 || currentLocation[0].y > 15) {
    lose = 1;
  }
}

void movement(int x) {
  int temp;
  //check if eating or not
  if(food.x == currentLocation[0].x && food.y == currentLocation[0].y) {
    for(temp = snakeLength; temp >= 0; temp--) {
      currentLocation[temp + 1] = currentLocation[temp];
    }
    snakeLength++;
    food.x = rand() % 16;
    food.y = rand() % 16;
  }
  else if(snakeLength > 0) {
    for(temp = 1; temp <= snakeLength; temp++) {
      if(currentLocation[0].x == currentLocation[temp].x && currentLocation[0].y == currentLocation[temp].y) {
        lose = 1;
      }
    }

    for(temp = snakeLength; temp > 0; temp--) {
      currentLocation[temp] = currentLocation[temp - 1];
    }
  }
  switch(x) {
    case 0:
     currentLocation[0].y--;
     break;
    case 1:
     currentLocation[0].x--;
     break;
    case 2:
     currentLocation[0].y++;
     break;
    case 3:
     currentLocation[0].x++;
     break;
   }

  //Cek lokasi apabila lewat border
  borderCheck();
}

void scanButton() {
  if(bit_is_clear(PINA, 4) && differentPath == 0) {
    if((snakePath % 2) != 0) {
      snakePath = 0;
      differentPath = 1;
    }
  }
  else if(bit_is_clear(PINA, 5) && differentPath == 0) {
    if((snakePath % 2) != 1) {
      snakePath = 1;
      differentPath = 1;
    }
  }
  else if(bit_is_clear(PINA, 6) && differentPath == 0) {
    if((snakePath % 2) != 0) {
      snakePath = 2;
      differentPath = 1;
    }
  }
  else if(bit_is_clear(PINA, 7) && differentPath == 0) {
    if((snakePath % 2) != 1) {
      snakePath = 3;
      differentPath = 1;
    }
  }
}

void display(int x, int y) {
  if(x < 8 && y < 8) {         //Dot Matrix A
    PORTD = 0x00;
    x = 7 - x;
    PORTB = (1 << x);
  }
  else if(x > 7 && y < 8) {   //Dot Matrix B
    PORTD = 0x00;
    x = 15 - x;
    PORTB = (1 << x);
    y = y + 8;
  }
  else if(x < 8 && y > 7) {   //Dot Matrix C
    PORTB = 0x00;
    x = 7 - x;
    PORTD = (1 << x);
    y = y - 8;
  }
    else if(x > 7 && y > 7) {   //Dot Matirx D
    PORTB = 0x00;
    x = 15 - x;
    PORTD = (1 << x);
  }

  PORTA = y;
  _delay_us(5);
  PORTB = PORTD = 0x00;
}

int main(void) {
  /* insert your hardware initialization here */
  DDRB = DDRD = 0xFF;
  DDRA = 0x0F;
  int x, y, delay;

  loses:
  PORTB = PORTD = 0x00;
  _delay_ms(50);
  lose = 0;
  food.x = rand() % 16;
  food.y = rand() % 16;
  currentLocation[0].x = rand() % 16;
  currentLocation[0].y = rand() % 16;
  snakePath = rand() % 4;
  snakeLength = 0;

  while(1) {
    //Untuk merubah arah pergerakan
    movement(snakePath);
    if (lose == 1) {
      goto loses;
    }
    //Tampilkan sesuai lokasi logical
    delay = 1000 / (snakeLength + 1);

    for(displayCount = 0; displayCount < delay; displayCount++) {
      for(snakeLengthCounter = 0; snakeLengthCounter <= snakeLength; snakeLengthCounter++) {

        x = currentLocation[snakeLengthCounter].x;
        y = currentLocation[snakeLengthCounter].y;

        display(x, y);

      }
      //Display food
      x = food.x;
      y = food.y;
      display(x,y);

      scanButton();
      if(differentPath == 1) {
        differentPath = 0;
        break;
      }
    }
  }
  return 0;   /* never reached */
}

// void debugDisplay() {
//   for(currentLocation[0].y = 0; currentLocation[0].y < 16; currentLocation[0].y++) {
//     for(currentLocation[0].x = 0; currentLocation[0].x < 16; currentLocation[0].x++) {
//       display(currentLocation[0].x, currentLocation[0].y);
//       _delay_ms(2);
//       PORTB = PORTD = 0x00;
//     }
//   }
// }
