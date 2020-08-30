#pragma once

#include <windows.h>

#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"
#include "RzErrors.h"

#include "UdpSocket.h"
    
class CChromaSDK
{
public:
    CChromaSDK();
    ~CChromaSDK();


public:
    void Initialize(LPIID);
    void UnInitialize();

    void RegisterEvendNotificationImpl();
    void UnRegisterEvendNotificationImpl();

private:

    HMODULE m_hModule;
    HANDLE m_hEvent;

};
