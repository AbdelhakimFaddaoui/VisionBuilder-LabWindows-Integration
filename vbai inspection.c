//==============================================================================
//
// Title:		vbai inspection
// Purpose:		.
//
// Created on:	9/27/2024 at 4:34:57 PM by abdelhakim faddaoui
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

//==============================================================================
// Types
//==============================================================================

//==============================================================================
// Static global variables
//==============================================================================

static int panelHandle = 0;

//==============================================================================
// Static functions
//==============================================================================

//==============================================================================
// Global variables
//==============================================================================

//==============================================================================
// Global functions
//==============================================================================
int LaunchVbaiAndOpenConnexion(char Engine_Name[512], VBAISession *vbSession);
int selectInspection(VBAISession vbSession, char inspectionPath[]);
int getMeasurement(VBAISession vbSession, bool32 *inspectionStatus);


/// HIFN Document your function here.
/// HIFN You may use multiple lines for documentation.
/// HIRET Document return value here.
/// HIRET You may use multiple lines for documentation.
/// HIPAR Engine_Name/Document parameter here.
/// HIPAR vbSession/Document parameter here.
int LaunchVbaiAndOpenConnexion (char Engine_Name[512], VBAISession *vbSession)
{
	VBAIError vbError;
	String255 Session_Name;
	int oldValue = SetBreakOnLibraryErrors (0);
	
	//vbError = vbaiLaunchLocalVBAIEngine (Engine_Name, NULL, 1, Session_Name, sizeof (String255));
	vbError = vbaiLaunchLocalVBAIEngine (Engine_Name, NULL, 1, Session_Name, sizeof (String255));
	if (vbError != 	VBAIErrorSuccess)
		goto ERROR;

	vbError = vbaiOpenConnection (Session_Name, NULL, 1, vbSession);
	if (vbError != 	VBAIErrorSuccess)
		goto ERROR;

	return 0;
ERROR:
	SetBreakOnLibraryErrors (oldValue);
	return -1;

}

/// HIFN Document your function here.
/// HIFN You may use multiple lines for documentation.
/// HIRET Document return value here.
/// HIRET You may use multiple lines for documentation.
/// HIPAR vbSession/Document parameter here.
/// HIPAR inspectionPath/Document parameter here.
int selectInspection (VBAISession vbSession, char inspectionPath[])
{
	VBAIError vbError;
	int oldValue = SetBreakOnLibraryErrors (0);

	vbError = vbaiOpenInspection (vbSession, inspectionPath);
	if (vbError != 	VBAIErrorSuccess)
	{
		SetBreakOnLibraryErrors (oldValue);
		return -1;
	}

	SetBreakOnLibraryErrors (oldValue);
	return 0;
}


/// HIFN Document your function here.
/// HIFN You may use multiple lines for documentation.
/// HIRET Document return value here.
/// HIRET You may use multiple lines for documentation.
/// HIPAR vbSession/Document parameter here.
/// HIPAR inspectionStatus/Document parameter here.
int getMeasurement (VBAISession vbSession, bool32 * inspectionStatus)
{
	VBAIError vbError;
	VBAIDateTime lastUpdatedTime;
	VBAIStepResult stepResults[20];
	VBAIMeasurements stepMeasurements[6][10];
	int oldValue = SetBreakOnLibraryErrors (0);

	vbError = vbaiEnableInspectionMeasurements(vbSession, 1);
	if (vbError != 	VBAIErrorSuccess)
		goto Error;

	vbError = vbaiRunInspectionOnce(vbSession, 10000);
	if (vbError != 	VBAIErrorSuccess)
		goto Error;

	vbError = vbaiGetInspectionMeasurements(vbSession, NULL, 0, stepMeasurements, &lastUpdatedTime);
	if (vbError != 	VBAIErrorSuccess)
		goto Error;

	vbError = vbaiGetInspectionResults(vbSession, stepResults, NULL , &lastUpdatedTime, inspectionStatus);
	if (vbError != 	VBAIErrorSuccess)
		goto Error;

	vbaiDispose(stepMeasurements);

	SetBreakOnLibraryErrors (oldValue);

	return 0;

Error:
	vbaiDispose(stepMeasurements);
	SetBreakOnLibraryErrors (oldValue);
	return -1;
}

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	int error = 0;
	int status = 0;
	VBAISession vbSession;
	bool32 inspectionStatus;
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (panelHandle = LoadPanel (0, "vbai inspection.uir", PANEL));

	
	status = LaunchVbaiAndOpenConnexion("Etoggle", &vbSession);
	status = selectInspection(vbSession, ".\\inspectionCamera01.vbai");
	status = getMeasurement(vbSession, &inspectionStatus);
	
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