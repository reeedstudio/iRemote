// iRemote - getIrKey
// loovee 2014-2-21

#include <IRSendRev.h>
#include <iRemote_hal.h>
#include <Streaming.h>

const int PINIR = 0;                   // define pin ir here


unsigned char dtaRecv[20];              // receive data here


void setup()
{

#if ____Debug
    Serial.begin(115200);
#endif
    iRemote.begin();
    cout << "init over" << endl;
}

void loop()
{

    if(iRemote.isGetIrDta())
    {
        unsigned char irKey = iRemote.getIrLastDta();
        
        cout << "get data from your ir remote: " << irKey << endl;        
    }
    delay(10);
}
