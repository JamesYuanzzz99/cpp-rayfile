#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdlib.h>
#include "unistd.h"
#include <string.h>
typedef unsigned int uint;

struct PDU
{
    uint uiPDULen;
    uint uiMsgType;
    char cadata[64];
    uint uiMsgLen;
    int caMsg[];

};
PDU *mkPDU(uint uiMsgLen);



#endif // PROTOCOL_H
