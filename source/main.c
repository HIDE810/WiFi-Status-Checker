#include "ac.h"
#include "fs.h"
#include "sockets.h"

char log[128];

int main(int argc, char *argv[])
{
	gfxInitDefault();
	aciInit();
	FS_OpenArchive(&sdmc_archive, ARCHIVE_SDMC);
	archive = sdmc_archive;
	
	consoleInit(GFX_BOTTOM, NULL);
	printf(
	"\x1b[2;1H\x1b[0;1m Press A to write your status\n\n to sdmc:/3ds/Wi-Fi/\n\n\n"
	"\x1b[31;1m Warning:\n\n Do not share this information!\n\n\n"
	"\x1b[0;1m (c)2019 HIDE\n\n\n"
	"\x1b[0;1m Press start to exit."
	);

	consoleInit(GFX_TOP, NULL);
	printf("\x1b[3;1H\x1b[33;1m Wi-Fi Status Checker\n\n\n\n");

	SOC_buffer = (u32*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);
	
	if ((ret = socInit(SOC_buffer, SOC_BUFFERSIZE)) != 0){
		IP("socInit: 0x%08X\n", (unsigned int)ret);
	}

	memset (&server, 0, sizeof (server));
	server.sin_addr.s_addr = gethostid();
	printf("\x1b[0;1m IP   : \x1b[36;1m%s\n\n\n",inet_ntoa(server.sin_addr));
	printf("\x1b[0;1m Port : \x1b[36;1m5000\n\n\n");

	if (R_SUCCEEDED(ACI_GetSSID(ssid)))
		printf("\x1b[0;1m SSID : \x1b[32;1m%s\n\n\n", ssid);

	if (R_SUCCEEDED(ACI_GetPassphrase(passphrase)))
		printf("\x1b[0;1m Pass : \x1b[32;1m%s\n\n\n", passphrase);

	while (aptMainLoop())
	{
		gspWaitForVBlank();
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) break;
		
		if (kDown & KEY_A)
		{
			FS_RecursiveMakeDir(archive, "/3ds/Wi-Fi/");
			FS_CreateFile(archive, "/3ds/Wi-Fi/WiFi.log");
			sprintf(log, "IP   : %s\nPort : 5000\nSSID : %s\nPass : %s", inet_ntoa(server.sin_addr), ssid, passphrase);
			FS_Write(archive, "/3ds/Wi-Fi/WiFi.log", log);
			printf("\x1b[20;1H\x1b[32;1m Success!");
		}
	}
	FS_CloseArchive(sdmc_archive);
	gfxExit();
	aciExit();
	return 0;
}