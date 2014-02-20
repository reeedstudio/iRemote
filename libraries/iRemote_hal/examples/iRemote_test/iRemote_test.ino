// iRemote demo
// 2014-2-19

#include <IRSendRev.h>
#include <iRemote_hal.h>

#define __Debug         0

char ctrlKey = KEY_LEFT_GUI;

unsigned char dtaRecv[20];              // receive data here


void setup()
{

#if __Debug
    Serial.begin(115200);
    while(!Serial);
#endif

    iRemote.begin();

}


void write_code()
{
    delay(10);

    // Type out "blink":
    Keyboard.print("// iRemote demo, by loovee\r\n");
    
    Keyboard.print("void setup() \r\n{\r\n");
    Keyboard.print("pinMode(13, OUTPUT);\r\n");
    Keyboard.print("}\r\n");
    Keyboard.println();
    Keyboard.println();
    Keyboard.print("void loop() \r\n{\r\n");
    Keyboard.print("digitalWrite(13, HIGH);\r\n");
    Keyboard.print("delay(1000);\r\n");

    Keyboard.print("digitalWrite(13, LOW);\r\n");
    Keyboard.print("delay(1000);\r\n");
    Keyboard.print("}\r\n");

    Keyboard.releaseAll();
}

void dataProc(int keyVal)
{

    iRemote.redLedOn(0);

    switch(keyVal)
    {

        case 253:                       // left

        iRemote.keyPressRelease(KEY_LEFT_ARROW);
        break;

        case 61:                        // right

        iRemote.keyPressRelease(KEY_RIGHT_ARROW);
        break;

        case 93:                                // logout



        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_DELETE);
        delay(500);
        Keyboard.releaseAll();
        delay(100);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('k');

        delay(100);
        Keyboard.releaseAll();


        break;


        case 31:                                // enter key

        iRemote.keyPressRelease(KEY_RETURN);

        delay(500);

        iRemote.enterString("123456");

        iRemote.keyPressRelease(KEY_RETURN);

        break;


        case 173:

        write_code();

        break;

        default:

        ;

    }
}
void loop()
{
    if(iRemote.isGetIrDta())
    {
        int dta_last = iRemote.getIrLastDta();

        Serial.println(dta_last);

        if(dta_last>=0)
        {
            dataProc(dta_last);
        }
    }

}
