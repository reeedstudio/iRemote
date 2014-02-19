// ir receive test
// loovee 2013-9-12

#include <IRSendRev.h>

const int PINIR = A4;                   // define pin ir here
unsigned char dtaRecv[20];              // receive data here

void setup()
{
    Serial.begin(115200);
    IR.Init(PINIR);
    Serial.println("init over");
}

void loop()
{
    if(IR.IsDta())
    {
        IR.Recv(dtaRecv);
        Serial.println("get data from IR:");
        
        for(int i=0; i< dtaRecv[0]+1; i++)
        {
            Serial.print(dtaRecv[i]);
            Serial.print('\t');
        }
        Serial.println();
    }
    
}
