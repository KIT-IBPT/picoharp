/************************************************************************

  Demo access to PicoHarp 300 Hardware via PHLIB v 2.2.
  The program performs a measurement based on hardcoded settings.
  The resulting histogram (65536 channels) is stored in an ASCII output file.

  Michael Wahl, PicoQuant GmbH, Nov 2007

  Note: This is a console application (i.e. run in terminal / cmd box)
  
  Note v2.x: This is now a multi-device library. All functions take a device 
  index. New functions for Open and Close.

  Tested with the following compilers:
  Windows:
  - MinGW 2.0.0-3 (free compiler for Win 32 bit)
  - MS Visual C++ 4.0/5.0/6.0 (Win 32 bit)
  - Borland C++ 5.3 / C++Builder 3.0 (Win 32 bit)
  Linux: 
  - gcc 3.3 through 4.1

************************************************************************/

#ifdef __linux__
#define Sleep(msec) usleep(msec*1000)
#else
#include <windows.h>
#include <dos.h>
#include <conio.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "phdefin.h"
#include "phlib.h"
#include "errorcodes.h"


int main(int argc, char* argv[])
{

 int dev[MAXDEVNUM]; 
 int found=0;
 FILE *fpout;   
 int retcode;
 char LIB_Version[8];
 char HW_Model[16];
 char HW_Version[8];
 char HW_Serial[8];
 char Errorstring[40];
 int Range=0; //you can change this
 int Offset=0; 
 int Tacq=1000; //Measurement time in millisec, you can change this
 int SyncDivider = 8; //you can change this 
 int CFDZeroCross0=10; //you can change this
 int CFDLevel0=100; //you can change this
 int CFDZeroCross1=10; //you can change this
 int CFDLevel1=100; //you can change this
 int Resolution; 
 int Countrate0;
 int Countrate1;
 double Integralcount; 
 unsigned int counts[HISTCHAN];
 int i;
 int flags;
 int waitloop;
 char cmd=0;


 printf("\nPicoHarp 300 PHLib.DLL Demo Application    M. Wahl, PicoQuant GmbH, 2007");
 printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
 PH_GetLibraryVersion(LIB_Version);
 printf("\nPHLIB version is %s",LIB_Version);
 if(strncmp(LIB_Version,LIB_VERSION,sizeof(LIB_VERSION))!=0)
         printf("\nWarning: The application was built for version %s.",LIB_VERSION);

 if((fpout=fopen("dlldemo.out","w"))==NULL)
 {
        printf("\ncannot open output file\n"); 
        goto ex;
 }

 fprintf(fpout,"Range No         : %ld\n",Range);
 fprintf(fpout,"Offset           : %ld\n",Offset);
 fprintf(fpout,"AcquisitionTime  : %ld\n",Tacq);
 fprintf(fpout,"SyncDivider      : %ld\n",SyncDivider);
 fprintf(fpout,"CFDZeroCross0    : %ld\n",CFDZeroCross0);
 fprintf(fpout,"CFDLevel0        : %ld\n",CFDLevel0);
 fprintf(fpout,"CFDZeroCross1    : %ld\n",CFDZeroCross1);
 fprintf(fpout,"CFDLevel1        : %ld\n",CFDLevel1);


 printf("\nSearching for PicoHarp devices...");
 printf("\nDevidx     Status");


 for(i=0;i<MAXDEVNUM;i++)
 {
	retcode = PH_OpenDevice(i, HW_Serial); 
	if(retcode==0) //Grab any PicoHarp we can open
	{
		printf("\n  %1d        S/N %s", i, HW_Serial);
		dev[found]=i; //keep index to devices we want to use
		found++;
	}
	else
	{
		if(retcode==ERROR_DEVICE_OPEN_FAIL)
			printf("\n  %1d        no device", i);
		else 
		{
			PH_GetErrorString(Errorstring, retcode);
			printf("\n  %1d        %s", i,Errorstring);
		}
	}
 }

 //in this demo we will use the first PicoHarp device we found, i.e. dev[0]
 //you could also check for a specific serial number, so that you always know 
 //which physical device you are talking to.

 if(found<1)
 {
	printf("\nNo device available.");
	goto ex; 
 }
 printf("\nUsing device #%1d",dev[0]);
 printf("\nInitializing the device...");

 retcode = PH_Initialize(dev[0],MODE_HIST); 
 if(retcode<0)
 {
        printf("\nPH init error %d. Aborted.\n",retcode);
        goto ex;
 }
 
 retcode = PH_GetHardwareVersion(dev[0],HW_Model,HW_Version); /*this is only for information*/
 if(retcode<0)
 {
        printf("\nPH_GetHardwareVersion error %d. Aborted.\n",retcode);
        goto ex;
 }
 else
	printf("\nFound Model %s Version %s",HW_Model,HW_Version);

 printf("\nCalibrating...");
 retcode=PH_Calibrate(dev[0]);
 if(retcode<0)
 {
        printf("\nCalibration Error %d. Aborted.\n",retcode);
        goto ex;
 }

 retcode = PH_SetSyncDiv(dev[0],SyncDivider);
 if(retcode<0)
 {
        printf("\nPH_SetSyncDiv error %ld. Aborted.\n",retcode);
        goto ex;
 }

 retcode=PH_SetCFDLevel(dev[0],0,CFDLevel0);
 if(retcode<0)
 {
        printf("\nPH_SetCFDLevel error %ld. Aborted.\n",retcode);
        goto ex;
 }

 retcode = PH_SetCFDZeroCross(dev[0],0,CFDZeroCross0);
 if(retcode<0)
 {
        printf("\nPH_SetCFDZeroCross error %ld. Aborted.\n",retcode);
        goto ex;
 }

 retcode=PH_SetCFDLevel(dev[0],1,CFDLevel1);
 if(retcode<0)
 {
        printf("\nPH_SetCFDLevel error %ld. Aborted.\n",retcode);
        goto ex;
 }

 retcode = PH_SetCFDZeroCross(dev[0],1,CFDZeroCross1);
 if(retcode<0)
 {
        printf("\nPH_SetCFDZeroCross error %ld. Aborted.\n",retcode);
        goto ex;
 }

 retcode = PH_SetRange(dev[0],Range);
 if(retcode<0)
 {
        printf("\nPH_SetRange error %d. Aborted.\n",retcode);
        goto ex;
 }

 retcode = PH_SetOffset(dev[0],Offset);
 if(retcode<0)
 {
        printf("\nPH_SetOffset error %d. Aborted.\n",retcode);
        goto ex;
 }

 Resolution = PH_GetResolution(dev[0]);

 //Note: after Init or SetSyncDiv you must allow 100 ms for valid new count rate readings
 Sleep(200);
 Countrate0 = PH_GetCountRate(dev[0],0);
 Countrate1 = PH_GetCountRate(dev[0],1);

 printf("\nResolution=%1dps Countrate0=%1d/s Countrate1=%1d/s", Resolution, Countrate0, Countrate1);

 PH_SetStopOverflow(dev[0],1,65535);

 while(cmd!='q')
 { 

        PH_ClearHistMem(dev[0],0);             // always use Block 0 if not Routing

        printf("\npress RETURN to start measurement");
        getchar();

        Countrate0 = PH_GetCountRate(dev[0],0);
        Countrate1 = PH_GetCountRate(dev[0],1);
        printf("\nCountrate0=%1d/s Countrate1=%1d/s",Countrate0,Countrate1);
        
        retcode = PH_StartMeas(dev[0],Tacq); 
        if(retcode<0)
        {
                printf("\nError %1d in StartMeas. Aborted.\n",retcode);
                goto ex;
        }
         
        printf("\nMeasuring for %1d milliseconds...",Tacq);
        
        waitloop=0;
        while(PH_CTCStatus(dev[0])==0) waitloop++; //wait
         
        retcode = PH_StopMeas(dev[0]);
        if(retcode<0)
        {
                printf("\nError %1d in StopMeas. Aborted.\n",retcode);
                goto ex;
        }
        
        retcode = PH_GetBlock(dev[0],counts,0);
        if(retcode<0)
        {
                printf("\nError %1d in GetBlock. Aborted.\n",retcode);
                goto ex;
        }

        flags = PH_GetFlags(dev[0]);
        if(flags<0)
        {
                printf("\nError %1d in GetFlags. Aborted.\n",flags);
                goto ex;
        }

		Integralcount = 0;
		for(i=0;i<HISTCHAN;i++)
			Integralcount+=counts[i];
        
        printf("\nWaitloop=%1d  TotalCount=%1.0lf",waitloop,Integralcount);
        
        if(flags&FLAG_OVERFLOW) printf("  Overflow.");

        printf("\nEnter c to continue or q to quit and save the count data.");
        cmd=getchar();
		getchar();
 }
 
 for(i=0;i<HISTCHAN;i++)
         fprintf(fpout,"\n%5d",counts[i]);

ex:
 for(i=0;i<MAXDEVNUM;i++) //no harm to close all
 {
	PH_CloseDevice(i);
 }
 if(fpout) fclose(fpout);
 printf("\npress RETURN to exit");
 getchar();

 return 0;
}


