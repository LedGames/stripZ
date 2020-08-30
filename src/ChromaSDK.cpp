#include <iostream>

#include "ChromaSDK.h"
#include "UdpSocket.h"
#include <iomanip>

#ifdef _WIN64
#define CHROMASDKDLL        L"RzChromaBroadcastAPI64.dll"
#else
#define CHROMASDKDLL        L"RzChromaBroadcastAPI.dll"
#endif

typedef RZRESULT(*INIT)(UINT, UINT, UINT, UINT);
typedef RZRESULT(*UNINIT)(void);


typedef int (*cb)(int, intptr_t);
typedef int (*cb1)(int, intptr_t);

typedef RZRESULT(*REGISTEREVENTNOTIFICATION)(cb);
typedef RZRESULT(*REGISTERKEYBOARDEVENTNOTIFICATION)(cb1);
typedef RZRESULT(*UNREGISTEREVENTNOTIFICATION)(void);
typedef RZRESULT(*QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE& DeviceInfo);

#define EVENT_NAME  L"{4784D90A-1179-4F7D-8558-52511D809190}"

QUERYDEVICE QueryDevice = NULL;
REGISTEREVENTNOTIFICATION RegisterEventNotification = NULL;
REGISTERKEYBOARDEVENTNOTIFICATION RegisterKeyboardEventNotification = NULL;
UNREGISTEREVENTNOTIFICATION UnregisterEventNotification = NULL;

INIT Init = NULL;
UNINIT UnInit = NULL;
 
extern UdpSocket *udp;

CChromaSDK::CChromaSDK() :
    m_hModule(NULL),
    m_hEvent(NULL)
{

} 


CChromaSDK::~CChromaSDK() 
{
}

void CChromaSDK::Initialize(LPIID lpIid)
{
    if (m_hModule == NULL)
    {
        m_hModule = ::LoadLibrary(CHROMASDKDLL);
        if (m_hModule != NULL)
        {
			
           struct dwarr  {
                DWORD32 dw1;
                DWORD32 dw2;
                DWORD32 dw3;
                DWORD32 dw4;
            };
         

            INIT Init = (INIT)::GetProcAddress(m_hModule, "Init");
            if (Init != NULL)
            {

                dwarr* p = (dwarr*)lpIid;
         
                  RZRESULT rzResult = Init(0x4c24ad2b, 0x409fd0a6, 0x76ec95b8, 0xe6bef332);
		
                if (rzResult == RZRESULT_SUCCESS)
                {
					std::wcout << L"Razer connected" << std::endl;

                    RegisterEventNotification = (REGISTEREVENTNOTIFICATION)GetProcAddress(m_hModule, "RegisterEventNotification");
                    RegisterKeyboardEventNotification = (REGISTERKEYBOARDEVENTNOTIFICATION)GetProcAddress(m_hModule, "RegisterKeyboardEventNotification");
                    UnregisterEventNotification = (UNREGISTEREVENTNOTIFICATION)GetProcAddress(m_hModule, "UnRegisterEventNotification");

                } else {
					std::wcout << L"Razer connection failed" << std::endl;
				}
            } else {
					std::wcout << L"GetProcAddress faied for Init" << std::endl;
			} 
			
			
		}
		else {
				std::wcout << L"Failed to open chroma dll" << std::endl;	
		}
    }

    if (m_hEvent == NULL)
    {
        m_hEvent = ::CreateEvent(NULL, TRUE, FALSE, EVENT_NAME);
    }
}

void CChromaSDK::UnInitialize()
{
    if (m_hEvent != NULL)
    {
        ::CloseHandle(m_hEvent);
        m_hEvent = NULL;
    }

    if (m_hModule != NULL)
    {
        UNINIT UnInit = (UNINIT)::GetProcAddress(m_hModule, "UnInit");
        if (UnInit != NULL)
        {

            RZRESULT rzResult = UnInit();
            if (rzResult != RZRESULT_SUCCESS)
            {
                // Some error here
            }
        }

        ::FreeLibrary(m_hModule);
        m_hModule = NULL;
    }
}
int callback2(int message, intptr_t data) {
	//seems that led start at 25x4x8 
    if (message == 1) {
        if (data != 0) {
            udp->send((char*)(data + (28 * 8)), 26 * 4);
			
			for(int i=0; i<20;i++) {
			wprintf(L"%02x ", *((uint8_t*)((data + (28 * 8))+i)));
}
        }
    }
    return 0;
}


int callback(int message, intptr_t data) {   if (message == 1) {
        if (data != 0) {
            udp->send((char*)(data), 5 * 4);
        }
    }

    return 0; 
}

void CChromaSDK::RegisterEvendNotificationImpl()
{
    RegisterEventNotification(callback);
}

void CChromaSDK::UnRegisterEvendNotificationImpl()
{
    UnregisterEventNotification();
}
