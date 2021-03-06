// iRemote demo
// 2014-2-19

#include <IRSendRev.h>

#define __Debug         1


const int PINIR = 0;                   // define pin ir here


unsigned char dtaRecv[20];              // receive data here



void setup()
{

#if __Debug
    Serial.begin(115200);
    while(!Serial);
#endif
    
    Keyboard.begin();
    
    IR.Init(PINIR);
    
    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
    
    pinMode(13, LOW);
    pinMode(12, LOW);

    
}


long timers = 0;


int __State = 0;
int __State_buf = 0;

void press_release(unsigned char keyVal)
{
    Keyboard.press(keyVal);
    delay(10);
    Keyboard.release(keyVal);
}

void enterString(char *str)
{
    while(*str)
    {
        press_release(str[0]);
        delay(10);
        str++;
    }
    
    delay(10);
}

unsigned char key_buf[2] = {KEY_LEFT_ARROW, KEY_RIGHT_ARROW};

void loop()
{
    if(IR.IsDta())
    {

        IR.Recv(dtaRecv);
        
#if __Debug
        Serial.println("get data from IR:");
        
        for(int i=0; i< dtaRecv[0]+1; i++)
        {
            Serial.print(dtaRecv[i]);
            Serial.print('\t');
        }
        Serial.println();
#endif

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
        else if(dtaRecv[dtaRecv[0]] == 93)      // logout
        {
            digitalWrite(12, HIGH);
            delay(20);
            digitalWrite(12, LOW);
            
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press(KEY_DELETE);
            delay(500);
            Keyboard.releaseAll();
            //ALT-s:
            //delay(2000);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press('k');
            
            delay(100);
            Keyboard.releaseAll();
        }
        else if(dtaRecv[dtaRecv[0]] == 31)  // enter key
        {
            digitalWrite(12, HIGH);
            delay(20);
            digitalWrite(12, LOW);
            
            
            press_release(KEY_RETURN);
            
            delay(500);
            
            enterString("123456");
            
            press_release(KEY_RETURN);

        }
    }

    if(millis()-timers>500)
    {
        timers = millis();
       // digitalWrite(13, !digitalRead(13));
       // digitalWrite(12, !digitalRead(12));
    }

}
