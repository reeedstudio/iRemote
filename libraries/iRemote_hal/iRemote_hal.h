#ifndef __IREMOTE_HAL_H__
#define __IREMOTE_HAL_H__


class iRemote_hal{

private:


private:




public:

    void begin();
    
    void timerIsr();
    
    void redLedOn(unsigned int time_ms);
    void greenLedOn(unsigned int time_ms);
    
    //void keyPressRelease(unsigned char keyNum, unsigned int time_ms);
    
    void keyPressRelease(unsigned char keyNum);
    
    
    void enterString(char *str);
    
    int isGetIrDta();
    
    int getIrDta(unsigned char *dta);           // return data Len, and data will storge in *dta
    
    int getIrLastDta();                         // get last data of all data, if get nothing , return -1;


};


extern iRemote_hal iRemote;

#endif