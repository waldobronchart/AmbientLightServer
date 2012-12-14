/*
 * blink.c:
 *      Simple test program to blink an LED on pin 7
 */

 //
//  How to access GPIO registers from C-code on the Raspberry-Pi
//  Example program
//  15-January-2012
//  Dom and Gert
//

/*
// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) // GPIO controller


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
char *gpio_mem, *gpio_map;
char *spi0_mem, *spi0_map;


// I/O access
volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

void setup_io();

int main(int argc, char **argv)
{ 
  int g,rep;

  // Set up gpi pointer for direct register access
  setup_io();

  // Switch GPIO 7..11 to output mode

  // Set GPIO pins 0 and 1 to output
  for (g=0; g<=1; g++)
  {
    INP_GPIO(g); // must use INP_GPIO before we can use OUT_GPIO
    OUT_GPIO(g);
  }

  INP_GPIO(0);
  OUT_GPIO(0);
  INP_GPIO(1);
  OUT_GPIO(1);

  for (rep=0; rep<10; rep++)
  {
     for (g=7; g<=11; g++)
     {
       GPIO_SET = 1<<g;
       sleep(1);
     }
     for (g=7; g<=11; g++)
     {
       GPIO_CLR = 1<<g;
       sleep(1);
     }
  }

  return 0;

} // main


//
// Set up a memory regions to access GPIO
//
void setup_io()
{
   // open /dev/mem 
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit (-1);
   }

   // mmap GPIO 

   // Allocate MAP block
   if ((gpio_mem = malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {
      printf("allocation error \n");
      exit (-1);
   }

   // Make sure pointer is on 4K boundary
   if ((unsigned long)gpio_mem % PAGE_SIZE)
     gpio_mem += PAGE_SIZE - ((unsigned long)gpio_mem % PAGE_SIZE);

   // Now map it
   gpio_map = (unsigned char *)mmap(
      (caddr_t)gpio_mem,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED|MAP_FIXED,
      mem_fd,
      GPIO_BASE
   );

   if ((long)gpio_map < 0) {
      printf("mmap error %d\n", (int)gpio_map);
      exit (-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;


} // setup_io

*/


#include "../wiringPi/wiringPi.h"
#include <stdio.h>

#include <iostream>

using namespace std;

int clockPin = 0;
int dataPin = 1;

void ShiftOut8Bits(unsigned char c)
{
  for (int bit = 0; bit < 8; bit++)
  {
    bool val = (c >> (7 - bit)) & 1;

    digitalWrite(clockPin, 0);
    digitalWrite(dataPin, val);
    digitalWrite(clockPin, 1);

    //cout << (int)val;
  }
    //cout << " ";
}


int main (void)
{

  if (wiringPiSetup() == -1)
    return 1;

  printf("Raspberry Pi wiringPi blink test\n");

  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for (int i=0; i<50; i++)
  {
    ShiftOut8Bits(0);    ShiftOut8Bits(0);    ShiftOut8Bits(0);
    //cout << "\n";
  }
  digitalWrite(clockPin, 0);

    //cout << "\n";
    //cout << "\n";

  unsigned char r = 255;
  unsigned char g = 0;
  unsigned char b = 0;

  float shiz = 0;
  float numLeds = 2;
  while (true)
  {
    shiz++;
    if (shiz > 10)
    {
      shiz = 0;
      numLeds++;

      if (numLeds > 30)
      {
        numLeds = 2;
      }

        // clear all leds
        for (int i=0; i<50; i++)
          ShiftOut8Bits(0);    ShiftOut8Bits(0);    ShiftOut8Bits(0);
        digitalWrite(clockPin, 0);
        delay(20);
    }

    for (int i=0; i<numLeds; i++)
    {
      ShiftOut8Bits(r);
      ShiftOut8Bits(g);
      ShiftOut8Bits(b);

    //cout << "\n";
    }

    //ShiftOut8Bits(0);ShiftOut8Bits(0);ShiftOut8Bits(0);

    digitalWrite(clockPin, 0);
    //cout << "\n";
    //cout << "\n";

    delay(10);

    for (int i=0; i<numLeds; i++)
    {
      ShiftOut8Bits(0);
      ShiftOut8Bits(0);
      ShiftOut8Bits(0);
      //cout << "\n";
    }

    //ShiftOut8Bits(0);ShiftOut8Bits(0);ShiftOut8Bits(0);

    digitalWrite(clockPin, 0);

    delay(10);
  }

  return 0;
}

