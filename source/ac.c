#include "ac.h"

static Handle acHandle;

Result aciInit(void)
{
	return srvGetServiceHandle(&acHandle, "ac:i");
}

Result aciExit(void)
{
	return svcCloseHandle(acHandle);
}

Result ACI_GetSSID(char * ssid)
{
	u32* cmdbuf = getThreadCommandBuffer();
	
	cmdbuf[0] = IPC_MakeHeader(0x40F,0,0); // 0x040F0000
	
	u32* staticbufs = getThreadStaticBuffers();
	
	staticbufs[0] = IPC_Desc_StaticBuffer(0x20, 0); //SSID length is 0x20
	staticbufs[1] = (u32)ssid;
	
	Result ret = 0;
	if(R_FAILED(ret = svcSendSyncRequest(acHandle))) return ret;
	
	return (Result)cmdbuf[1];
}

Result ACI_GetPassphrase(char * passphrase)
{
	u32* cmdbuf = getThreadCommandBuffer();
	
	cmdbuf[0] = IPC_MakeHeader(0x415,0,0); // 0x04150000
	
	u32* staticbufs = getThreadStaticBuffers();
	
	staticbufs[0] = IPC_Desc_StaticBuffer(0x40, 0); //passphrase length is 0x40
	staticbufs[1] = (u32)passphrase;
	
	Result ret = 0;
	if(R_FAILED(ret = svcSendSyncRequest(acHandle))) return ret;
	
	return (Result)cmdbuf[1];
}