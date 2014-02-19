// ir send test
// loovee 2013-9-12
// Infrared Emitter connect to D3

#include <IRSendRev.h>

unsigned char dtaSend[] = {9, 90, 91, 11, 31, 4, 1, 2, 3, 4};

void setup()
{

}

void loop()
{
    IR.Send(dtaSend, 38);
    delay(1000);
}