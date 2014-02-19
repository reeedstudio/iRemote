// iRemote demo
// 2014-2-19

#include <IRSendRev.h>
#include <iRemote_hal.h>

#define __Debug         0


unsigned char dtaRecv[20];              // receive data here


void setup()
{

#if __Debug
    Serial.begin(115200);
    while(!Serial);
#endif
    
    iRemote.begin();

}


void dataProc(int keyVal)
{

    iRemote.redLedOn(0);
    
    switch(keyVal)
    {
    
        case 253:                       // left
        
        iRemote.keyPressRelease(KEY_LEFT_ARROW);
        break;
            
        case 51:                        // right
        
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
