/*
 * IRremote
 * Version 0.11 August, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
 *
 * Modified by Paul Stoffregen <paul@pjrc.com> to support other boards and timers
 * Modified  by Mitra Ardron <mitra@mitra.biz> 
 * Added Sanyo and Mitsubishi controllers
 * Modified Sony to spot the repeat codes that some Sony's send
 * 
 * Modifier by
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 *
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 */

#include "IRSendrev.h"
#include "IRsendRevInt.h"

#include <Streaming.h>
// Provides ISR
#include <avr/interrupt.h>

volatile irparams_t irparams;

void IRSendRev::Init(int revPin)
{
    irparams.recvpin    = revPin;
    
    enableIRIn(); // Start the receiver
    delay(20);
    Clear();
}

void IRSendRev::Init()
{
    delay(20);
    Clear();
}
// initialization
void IRSendRev::enableIRIn() {
  cli();
  // setup pulse clock timer interrupt
  //Prescale /8 (16M/8 = 0.5 microseconds per tick)
  // Therefore, the timer interval can range from 0.5 to 128 microseconds
  // depending on the reset value (255 to 0)
  TIMER_CONFIG_NORMAL();

  //Timer2 Overflow Interrupt Enable
  TIMER_ENABLE_INTR;

  TIMER_RESET;

  sei();  // enable interrupts

  // initialize state machine variables
  irparams.rcvstate = STATE_IDLE;
  irparams.rawlen = 0;

  // set pin modes
  pinMode(irparams.recvpin, INPUT);
}

// TIMER2 interrupt code to collect raw data.
// Widths of alternating SPACE, MARK are recorded in rawbuf.
// Recorded in ticks of 50 microseconds.
// rawlen counts the number of entries recorded so far.
// First entry is the SPACE between transmissions.
// As soon as a SPACE gets long, ready is set, state switches to IDLE, timing of SPACE continues.
// As soon as first MARK arrives, gap width is recorded, ready is cleared, and new logging starts

ISR(TIMER_INTR_NAME)
{
  TIMER_RESET;

  uint8_t irdata = (uint8_t)digitalRead(irparams.recvpin);

  irparams.timer++; // One more 50us tick
  if (irparams.rawlen >= RAWBUF) {
    // Buffer overflow
    irparams.rcvstate = STATE_STOP;
  }
  switch(irparams.rcvstate) {
  case STATE_IDLE: // In the middle of a gap
    if (irdata == MARK) {
      if (irparams.timer < GAP_TICKS) {
        // Not big enough to be a gap.
        irparams.timer = 0;
      } 
      else {
        // gap just ended, record duration and start recording transmission
        irparams.rawlen = 0;
        irparams.rawbuf[irparams.rawlen++] = irparams.timer;
        irparams.timer = 0;
        irparams.rcvstate = STATE_MARK;
      }
    }
    break;
  case STATE_MARK: // timing MARK
    if (irdata == SPACE) {   // MARK ended, record time
      irparams.rawbuf[irparams.rawlen++] = irparams.timer;
      irparams.timer = 0;
      irparams.rcvstate = STATE_SPACE;
    }
    break;
  case STATE_SPACE: // timing SPACE
    if (irdata == MARK) { // SPACE just ended, record it
      irparams.rawbuf[irparams.rawlen++] = irparams.timer;
      irparams.timer = 0;
      irparams.rcvstate = STATE_MARK;
    }
    else { // SPACE
      if (irparams.timer > GAP_TICKS) {
        // big SPACE, indicates gap between codes
        // Mark current code as ready for processing
        // Switch to STOP
        // Don't reset timer; keep counting space width
        irparams.rcvstate = STATE_STOP;
      } 
    }
    break;
  case STATE_STOP: // waiting, measuring gap
    if (irdata == MARK) { // reset gap timer
      irparams.timer = 0;
    }
    break;
  }

}

void IRSendRev::Clear() {
  irparams.rcvstate = STATE_IDLE;
  irparams.rawlen = 0;
}

// Decodes the received IR message
// Returns 0 if no data ready, 1 if data ready.
// Results of decoding are stored in results
int IRSendRev::decode(decode_results *results) {
  results->rawbuf = irparams.rawbuf;
  results->rawlen = irparams.rawlen;
  if (irparams.rcvstate != STATE_STOP) {
    return ERR;
  }
  // Throw away and start over
  Clear();
  return 1;
}

unsigned char IRSendRev::Recv(unsigned char *revData)
{
    int count       = results.rawlen;
    int nshort      = 0;
    int nlong       = 0;
    int count_data  = 0;

    count_data = (count-4)/16;

    for(int i = 0; i<10; i++)           // count nshort
    {
        nshort += results.rawbuf[3+2*i];
    }
    nshort /= 10;

    int i = 0;
    int j = 0;
    while(1)        // count nlong
    {
        if(results.rawbuf[4+2*i] > (2*nshort))
        {
            nlong += results.rawbuf[4+2*i];
            j++;
        }
        i++;
        if(j==10)break;
        if((4+2*i)>(count-10))break;
    }
    nlong /= j;

    int doubleshort = 2*nshort;
    for(i = 0; i<count_data; i++)
    {
        revData[i+D_DATA] = 0x00;
        for(j = 0; j<8; j++)
        {
            if(results.rawbuf[4 + 16*i + j*2] > doubleshort) // 1
            {
                revData[i+D_DATA] |= 0x01<< (7-j);
            }
            else
            {
                revData[i+D_DATA] &= ~(0x01<<(7-j));
            }
        }
    }
    revData[D_LEN]      = count_data+5;
    revData[D_STARTH]   = results.rawbuf[1];
    revData[D_STARTL]   = results.rawbuf[2];
    revData[D_SHORT]    = nshort;
    revData[D_LONG]     = nlong;
    revData[D_DATALEN]  = count_data;
 
#if __DEBUG
    Serial.print("\r\n*************************************************************\r\n");
    Serial.print("len\t = ");Serial.println(revData[D_LEN]);
    Serial.print("start_h\t = ");Serial.println(revData[D_STARTH]);
    Serial.print("start_l\t = ");Serial.println(revData[D_STARTL]);
    Serial.print("short\t = ");Serial.println(revData[D_SHORT]);
    Serial.print("long\t = ");Serial.println(revData[D_LONG]);
    Serial.print("data_len = ");Serial.println(revData[D_DATALEN]);
    for(int i = 0; i<revData[D_DATALEN]; i++)
    {
        Serial.print(revData[D_DATA+i]);Serial.print("\t");
    }
    Serial.print("\r\n*************************************************************\r\n");
#endif

    Clear(); // Receive the next value
    return revData[D_LEN]+1;
}

//if get some data from IR
unsigned char IRSendRev::IsDta()
{

    if(decode(&results))
    {
    
        cout << "rawlen = " << results.rawlen << endl;
        for(int i=0; i<results.rawlen; i++)
        {
            cout << results.rawbuf[i] << '\t';
                
            if((i+1)%10 == 0)cout << endl;
        }
            
        cout << endl;

        int count       = results.rawlen;
        if(count < 64  || (count -4)%8 != 0)
        {
#if __DEBUG
            Serial.print("IR GET BAD DATA!\r\n");
            Serial.print("count = ");Serial.println(count);
    
#endif
            Clear();        // Receive the next value
            return 0;
        }
        
        int count_data  = (count-4) / 16;
#if __DEBUG
        Serial.print("ir get data! count_data = ");
        Serial.println(count_data);
#endif
        return (unsigned char)(count_data+6);
    }
    else 
    {
        return 0;
    }

}

IRSendRev IR;