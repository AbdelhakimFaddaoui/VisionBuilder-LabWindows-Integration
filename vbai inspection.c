//==============================================================================
//
// Title:		vbai inspection
// Purpose:		A short description of the application.
//
// Created on:	9/27/2024 at 4:34:57 PM by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "VBAIInterfaceC.h"
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "vbai inspection.h"
#include "toolbox.h"
#include "vbai inspection.h"


//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle = 0;

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	int i = 0;
	
	int error = 0;
	char Session_Name[512] = "Untitled1";
	char Engine_Name[512] = "Etoggle";
	//int Session_Name_Length = 512;
	int Session_Name_Length = sizeof (String255);
	VBAISession vbSession;
	VBAIError vbError;
	int oldValue;
	VBAIMeasurements stepMeasurements[6][10];
	VBAIDateTime lastUpdatedTime;
	
	oldValue = SetBreakOnLibraryErrors (0);

	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (panelHandle = LoadPanel (0, "vbai inspection.uir", PANEL));

	// lunch VBAI 
	for (i=0,vbError =-1; (i<10 && vbError != VBAIErrorSuccess); ++i)
	{
		vbError = vbaiLaunchLocalVBAIEngine (Engine_Name, VBAI_2023, 1, Session_Name, sizeof (String255));
	}
	
	printf ("%d\n", i);
	
	vbError = vbaiOpenConnection (Session_Name, NULL, 1, &vbSession);
	vbError = vbaiOpenInspection (vbSession, "C:\\Users\\EOLT E-TOGGLE\\Desktop\\job camera ST80\\job bumper.vbai");
	vbError = vbaiEnableInspectionMeasurements(vbSession, 1);
	vbError = vbaiRunInspectionOnce(vbSession, 10000);
	
	vbError = vbaiGetInspectionMeasurements(vbSession, NULL, 0, stepMeasurements, &lastUpdatedTime);
	vbError = vbaiGetInspectionResults(vbSession, VBAIStepResult stepResults[], size_t* size, VBAIDateTime* lastUpdatedTime, bool32* inspectionStatus);

	
	vbaiDispose(stepMeasurements);
	
	SetBreakOnLibraryErrors (oldValue);
	
	/* display the panel and run the user interface */
	errChk (DisplayPanel (panelHandle));
	errChk (RunUserInterface ());

Error:
	/* clean up */
	if (panelHandle > 0)
		DiscardPanel (panelHandle);
	return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	if (event == EVENT_CLOSE)
		QuitUserInterface (0);
	return 0;
}
