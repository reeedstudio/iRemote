#include <Arduino.h>
#include <iRemote_hal.h>
#include <IRSendRev.h>

#define PINIR   0

#define PINREDLED   12
#define PINGREENLED 13

#define __DBG           1

void iRemote_hal::begin()
{
    Keyboard.begin();
    
    IR.Init(PINIR);
    
    pinMode(PINREDLED, OUTPUT);
    pinMode(PINGREENLED, OUTPUT);
    
    pinMode(PINREDLED, LOW);
    pinMode(PINGREENLED, LOW);
}
    
void iRemote_hal::timerIsr()
{
    
}
    

void iRemote_hal::redLedOn(unsigned int time_ms)
{
    digitalWrite(PINREDLED, HIGH);
    delay(20);
    digitalWrite(PINREDLED, LOW);
    
}

void iRemote_hal::greenLedOn(unsigned int time_ms)
{
    digitalWrite(PINGREENLED, HIGH);
    delay(20);
    digitalWrite(PINGREENLED, LOW);
}
    
    //void keyPressRelease(unsigned char keyNum, unsigned int time_ms);
    
void iRemote_hal::keyPressRelease(unsigned char keyNum)
{
    Keyboard.press(keyNum);
    delay(10);
    Keyboard.release(keyNum);
}
    
void iRemote_hal::enterString(char *str)
{
    if(NULL == str)return;
    
    while(*str)
    {
        keyPressRelease(str[0]);
        delay(10);
        str++;
    }
    
    delay(10);
}


int iRemote_hal::isGetIrDta()
{
    return IR.IsDta();
}
    
int iRemote_hal::getIrDta(unsigned char *dta)                   // return data Len, and data will storge in *dta
{
    if(NULL == dta)return 0;
    
    unsigned char dta_tmp[20];
    IR.Recv(dta_tmp);
    int dtaLen = dta_tmp[5];
    
    for(int i=0; i<dtaLen; i++)
    {
        dta[i] = dta_tmp[6+i];
    }
    
    
#if __DBG

    Serial.println("ir get data:");
    
    for(int i=0; i<dta_tmp[0]+1; i++)
    {
        Serial.print(dta_tmp[i]);
        Serial.print("\t");
    }
    
    Serial.println();
#endif
    
    return dtaLen;
}


int iRemote_hal::getIrLastDta()                         // get last data of all data, if get nothing , return -1;
{
    unsigned char dta_tmp[20];
    
    int dtaLen = getIrDta(dta_tmp);
    
    if(0 == dtaLen)return -1;
    
    return dta_tmp[dtaLen-1];

    
}


iRemote_hal iRemote;







