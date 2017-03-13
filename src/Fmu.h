/*
 * Fmu.h
 *
 *  Created on: Feb 23, 2017
 *      Author: kel
 */

#ifndef SRC_FMU_H_
#define SRC_FMU_H_

#include <map>
//#include <iostream>
#include <string>

#include <memory>
#include <iostream>
#include <string>
#include <cstdio>

namespace fmi2
{

using namespace std;

extern "C"
{

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

#include "fmi2Functions.h"

//#include "XmlParserCApi.h"

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

#define BUFSIZE 4096

//void fmuLogger(fmi2Component c, fmi2String instanceName, fmi2Status status, fmi2String category, fmi2String message, ...);
static int loadDll(const char* dllPath, FMU *fmu);
//int error(const char *message);

}

class Callback
{
public:
	void log(fmi2String instanceName, fmi2Status status, fmi2String category, fmi2String message);
};

typedef struct
{
	fmi2CallbackFunctions * functions;
	shared_ptr<Callback> callbacks;
} ComponentContext;

class Fmu
{
public:
	Fmu(const char* path);bool initialize();
	virtual ~Fmu();

	shared_ptr<ComponentContext> getContext(const char* instanceName);

	static shared_ptr<string> fmi2StatusToString(fmi2Status status);
	static shared_ptr<string> combinePath(shared_ptr<string> path1, shared_ptr<string> path3);

	fmi2Component instantiate(fmi2String instanceName, fmi2Type fmuType, fmi2String fmuGUID,
			 fmi2Boolean visible, fmi2Boolean loggingOn,shared_ptr<Callback> callback);

	/***************************************************
	 Common Functions
	 ****************************************************/
	fmi2GetTypesPlatformTYPE getTypesPlatform;
	fmi2GetVersionTYPE getVersion;
	fmi2SetDebugLoggingTYPE setDebugLogging;
	fmi2InstantiateTYPE instantiate;
	fmi2FreeInstanceTYPE freeInstance;
	fmi2SetupExperimentTYPE setupExperiment;
	fmi2EnterInitializationModeTYPE enterInitializationMode;
	fmi2ExitInitializationModeTYPE exitInitializationMode;
	fmi2TerminateTYPE terminate;
	fmi2ResetTYPE reset;
	fmi2GetRealTYPE getReal;
	fmi2GetIntegerTYPE getInteger;
	fmi2GetBooleanTYPE getBoolean;
	fmi2GetStringTYPE getString;
	fmi2SetRealTYPE setReal;
	fmi2SetIntegerTYPE setInteger;
	fmi2SetBooleanTYPE setBoolean;
	fmi2SetStringTYPE setString;
	fmi2GetFMUstateTYPE getFMUstate;
	fmi2SetFMUstateTYPE setFMUstate;
	fmi2FreeFMUstateTYPE freeFMUstate;
	fmi2SerializedFMUstateSizeTYPE serializedFMUstateSize;
	fmi2SerializeFMUstateTYPE serializeFMUstate;
	fmi2DeSerializeFMUstateTYPE deSerializeFMUstate;
	fmi2GetDirectionalDerivativeTYPE getDirectionalDerivative;
	/***************************************************
	 Functions for FMI2 for Co-Simulation
	 ****************************************************/
	fmi2SetRealInputDerivativesTYPE setRealInputDerivatives;
	fmi2GetRealOutputDerivativesTYPE getRealOutputDerivatives;
	fmi2DoStepTYPE doStep;
	fmi2CancelStepTYPE cancelStep;
	fmi2GetStatusTYPE getStatus;
	fmi2GetRealStatusTYPE getRealStatus;
	fmi2GetIntegerStatusTYPE getIntegerStatus;
	fmi2GetBooleanStatusTYPE getBooleanStatus;
	fmi2GetStringStatusTYPE getStringStatus;

	//INTO CPS specific
	fmi2GetMaxStepsizeTYPE getMaxStepsize;

private:


	std::shared_ptr<string>  extractedDirectory;
	std::shared_ptr<string> path;
	static const char* platform;
	static const char* library_ext;
	FMU* handles;
	std::map<const char*, shared_ptr<ComponentContext>> activeCallbacks;

	bool unpack(const char *zip_file_path, const char* output_folder);

};

} /* namespace fmi2 */

#endif /* SRC_FMU_H_ */
