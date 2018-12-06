#include <REG51.h>
#include <math.h>

xdata at 0xe060 PortSeg;
code unsigned char CodeSeg[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x76}; // 0 - 7,H

sbit sw0 = p1 ^ 0;
sbit sw1 = p1 ^ 1;
sbit sw2 = p1 ^ 2;
sbit sw3 = p1 ^ 3;
sbit sw4 = p1 ^ 4;
sbit sw5 = p1 ^ 5;
sbit sw6 = p1 ^ 6;
sbit sw7 = p1 ^ 7;
void Pulse(unsigned char time, HI, LOW)
{ unsigned char i;
  for (i = 0; i < time; i++)
  { TH0 = HI;
    TL0 = LOW;
    TF0 = 0;
    TR0 = 1;
    while (TF0 == 0);
    TR0 = 0;
  }
}


void main() {
  SCON = 0x52; // Serial Config n,8,1
  TMOD = 0x20; // Set Timer for buad Gen.
  TH1 = 0xfd; // Buad rate 9600bps@11.0592MHz
  TR1 = 1; // Start Timer1
  TI = 1;

  sw0 = 1;
  sw1 = 1;
  sw2 = 1;
  sw3 = 1;
  sw4 = 1;
  sw5 = 1;
  sw6 = 1;
  sw7 = 1;

  while (1) {
    if (sw0 == 0) {
      Pulse(3, 0xfc, 0x17);
      PortSeg = CodeSeg[0];

    }
    else if (sw1 == 0) {
      Pulse(12, 0xff, 0x9d);
      PortSeg = CodeSeg[1];

    }

    else if (sw2 == 0) {
      Pulse(12, 0xff, 0x37);
      PortSeg = CodeSeg[2];

    }

    else if (sw3 == 0) {
      Pulse(12, 0xfe, 0xb3);
      PortSeg = CodeSeg[3];

    }

    else if (sw4 == 0) {
      Pulse(12, 0xfe, 0x6f);
      PortSeg = CodeSeg[4];

    }

    else if (sw5 == 0) {
      Pulse(6, 0xfe, 0x0b);
      PortSeg = CodeSeg[5];

    }

    else if (sw6 == 0) {
      Pulse(6, 0xfd, 0xa7);
      PortSeg = CodeSeg[6];

    }

    else if (sw7 == 0) {
      Pulse(6, 0xfd, 0x43);
      PortSeg = CodeSeg[7];

    }
    else {
      Pulse(0, 0x00, 0x01);
      PortSeg = CodeSeg[8];
    }


  }
}



