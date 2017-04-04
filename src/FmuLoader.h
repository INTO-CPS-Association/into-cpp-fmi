/*
 * #%~
 * INTO-CPP-FMI
 * %%
 * Copyright (C) 2017 - 2014 Aarhus University
 * %%
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/gpl-3.0.html>.
 * #~%
 */
/*
 * FmuLoader.h
 *
 *  Created on: Mar 15, 2017
 *      Author: kel
 */

#ifndef SRC_FMULOADER_H_
#define SRC_FMULOADER_H_

#include "fmi2Functions.h"
#include<stdio.h>

//#include "XmlParserCApi.h"
#ifdef __cplusplus
extern "C" {
#endif



#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_MAC
// Other kinds of Mac OS
#include <dlfcn.h>
#define HMODULE void*
#else
// Unsupported platform
#endif
#elif __linux
#include <limits.h>
#include <dlfcn.h>
#define MAX_PATH PATH_MAX
#define HMODULE void*
#endif



#define BUFSIZE 4096




typedef struct
{
	//ModelDescription* modelDescription;

	HMODULE dllHandle; // fmu.dll handle
	/***************************************************
	 Common Functions
	 ****************************************************/
	fmi2GetTypesPlatformTYPE *getTypesPlatform;
	fmi2GetVersionTYPE *getVersion;
	fmi2SetDebugLoggingTYPE *setDebugLogging;
	fmi2InstantiateTYPE *instantiate;
	fmi2FreeInstanceTYPE *freeInstance;
	fmi2SetupExperimentTYPE *setupExperiment;
	fmi2EnterInitializationModeTYPE *enterInitializationMode;
	fmi2ExitInitializationModeTYPE *exitInitializationMode;
	fmi2TerminateTYPE *terminate;
	fmi2ResetTYPE *reset;
	fmi2GetRealTYPE *getReal;
	fmi2GetIntegerTYPE *getInteger;
	fmi2GetBooleanTYPE *getBoolean;
	fmi2GetStringTYPE *getString;
	fmi2SetRealTYPE *setReal;
	fmi2SetIntegerTYPE *setInteger;
	fmi2SetBooleanTYPE *setBoolean;
	fmi2SetStringTYPE *setString;
	fmi2GetFMUstateTYPE *getFMUstate;
	fmi2SetFMUstateTYPE *setFMUstate;
	fmi2FreeFMUstateTYPE *freeFMUstate;
	fmi2SerializedFMUstateSizeTYPE *serializedFMUstateSize;
	fmi2SerializeFMUstateTYPE *serializeFMUstate;
	fmi2DeSerializeFMUstateTYPE *deSerializeFMUstate;
	fmi2GetDirectionalDerivativeTYPE *getDirectionalDerivative;
	/***************************************************
	 Functions for FMI2 for Co-Simulation
	 ****************************************************/
	fmi2SetRealInputDerivativesTYPE *setRealInputDerivatives;
	fmi2GetRealOutputDerivativesTYPE *getRealOutputDerivatives;
	fmi2DoStepTYPE *doStep;
	fmi2CancelStepTYPE *cancelStep;
	fmi2GetStatusTYPE *getStatus;
	fmi2GetRealStatusTYPE *getRealStatus;
	fmi2GetIntegerStatusTYPE *getIntegerStatus;
	fmi2GetBooleanStatusTYPE *getBooleanStatus;
	fmi2GetStringStatusTYPE *getStringStatus;

	//INTO CPS specific
	fmi2GetMaxStepsizeTYPE *getMaxStepsize;
	/***************************************************
	 Functions for FMI2 for Model Exchange
	 ****************************************************/
	fmi2EnterEventModeTYPE *enterEventMode;
	fmi2NewDiscreteStatesTYPE *newDiscreteStates;
	fmi2EnterContinuousTimeModeTYPE *enterContinuousTimeMode;
	fmi2CompletedIntegratorStepTYPE *completedIntegratorStep;
	fmi2SetTimeTYPE *setTime;
	fmi2SetContinuousStatesTYPE *setContinuousStates;
	fmi2GetDerivativesTYPE *getDerivatives;
	fmi2GetEventIndicatorsTYPE *getEventIndicators;
	fmi2GetContinuousStatesTYPE *getContinuousStates;
	fmi2GetNominalsOfContinuousStatesTYPE *getNominalsOfContinuousStates;
}FMU;

 int loadDll(const char* dllPath, FMU *fmu);


#ifdef __cplusplus
}
#endif

#endif /* SRC_FMULOADER_H_ */
