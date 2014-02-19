// ir receive test
// loovee 2013-9-12

#include <IRSendRev.h>

#define __Debug         0


#

const int PINIR = 0;                   // define pin ir here


unsigned char dtaRecv[20];              // receive data here



void setup()
{

#if ____Debug
    Serial.begin(115200);
    
    while(!Serial);
#endif
    
    Keyboard.begin();
    
    IR.Init(PINIR);
    
    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
    
    pinMode(13, LOW);

    
}


long timers = 0;


int __State = 0;
int __State_buf = 0;


unsigned char key_buf[2] = {KEY_LEFT_ARROW, KEY_RIGHT_ARROW};

void loop()
{
    if(IR.IsDta())
    {
        IR.Recv(dtaRecv);
      //  Serial.println("get data from IR:");
        
        for(int i=0; i< dtaRecv[0]+1; i++)
        {
           // Serial.print(dtaRecv[i]);
           // Serial.print('\t');
        }
       // Serial.println();
        
        if(dtaRecv[dtaRecv[0]] == 253)
        {
           // Serial.println("left");
            Keyboard.press(key_buf[0]);
            digitalWrite(12, HIGH);
            delay(20);
            digitalWrite(12, LOW);
            Keyboard.release(key_buf[0]);
        }
        else if(dtaRecv[dtaRecv[0]] == 61)
        {
           // Serial.println("right");
            Keyboard.press(key_buf[1]);
            digitalWrite(12, HIGH);
            delay(20);
            digitalWrite(12, LOW);
            Keyboard.release(key_buf[1]);
        }
    }

    if(millis()-timers>500)
    {
        timers = millis();
       // digitalWrite(13, !digitalRead(13));
       // digitalWrite(12, !digitalRead(12));
    }

}
