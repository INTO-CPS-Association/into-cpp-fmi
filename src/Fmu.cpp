/*
 * Fmu.cpp
 *
 *  Created on: Feb 23, 2017
 *      Author: kel
 */

#include "Fmu.h"

namespace fmi2
{

#ifdef _WIN64
//define something for Windows (64-bit)
const char* Fmu::platform="win64";
const char* Fmu::library_ext=".dll";
#elif _WIN32
//define something for Windows (32-bit)
const char* Fmu::platform="win32";
const char* Fmu::library_ext=".dll";
#elif __APPLE__

const char* Fmu::platform = "darwin64";
const char* Fmu::library_ext = ".dylib";

#elif __linux
// linux
const char* Fmu::library_ext=".so";
const char* Fmu::platform="linux64";
#elif __unix // all unices not caught above
// Unix
#elif __posix
// POSIX
#endif

inline char separator()
{
#ifdef _WIN32
	return '\\';
#else
	return '/';
#endif
}

#include "miniunzipz.h"
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>

static void *getAdrOptional(int *success, HMODULE dllHandle, const char *functionName,bool optional)
{

#ifdef _WIN32
	void* fp = (void*)GetProcAddress(dllHandle, functionName);
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_MAC
	// Other kinds of Mac OS
	void* fp = (void*)dlsym(dllHandle, functionName);
#else
	// Unsupported platform
#endif
#elif __linux
	// linux
	void* fp = (void*)dlsym(dllHandle, functionName);
#endif

	if (!fp)
	{
		if(!optional)
		{
			printf("warning: Function %s not found in dll\n", functionName);
			*success = 0;
		}}
	return fp;
}

static void *getAdr(int *success, HMODULE dllHandle, const char *functionName)
{
	return getAdrOptional(success,dllHandle,functionName,false);
}

// Load the given dll and set function pointers in fmu
// Return 0 to indicate failure
static int loadDll(const char* dllPath, FMU *fmu)
{
	int x = 1, s = 1;

#ifdef _WIN32
	HMODULE h = LoadLibraryEx(dllPath,NULL,LOAD_WITH_ALTERED_SEARCH_PATH);
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_MAC
	// Other kinds of Mac OS
	HMODULE h = dlopen(dllPath,RTLD_LAZY);
#else
	// Unsupported platform
#endif
#elif __linux
	// linux
	HMODULE h = dlopen(dllPath,RTLD_LAZY);
#endif

	if (!h)
	{
		printf("error: Could not load %s\n", dllPath);

#ifdef _WIN32
//TODO figure out how to format GetLastError() in C
#elif __APPLE__
#if TARGET_OS_MAC
		// Other kinds of Mac OS
		fputs(dlerror(), stderr);
		fflush(stderr);
#else
		// Unsupported platform
#endif
#elif __linux
		// linux
		fputs(dlerror(),stderr);
		fflush(stderr);
#endif
		return 0; // failure
	}
	fmu->dllHandle = h;
	fmu->getTypesPlatform = (fmi2GetTypesPlatformTYPE *) getAdr(&s, h, "fmi2GetTypesPlatform");
	fmu->getVersion = (fmi2GetVersionTYPE *) getAdr(&s, h, "fmi2GetVersion");
	fmu->setDebugLogging = (fmi2SetDebugLoggingTYPE *) getAdr(&s, h, "fmi2SetDebugLogging");
	fmu->instantiate = (fmi2InstantiateTYPE *) getAdr(&s, h, "fmi2Instantiate");
	fmu->freeInstance = (fmi2FreeInstanceTYPE *) getAdr(&s, h, "fmi2FreeInstance");
	fmu->setupExperiment = (fmi2SetupExperimentTYPE *) getAdr(&s, h, "fmi2SetupExperiment");
	fmu->enterInitializationMode = (fmi2EnterInitializationModeTYPE *) getAdr(&s, h, "fmi2EnterInitializationMode");
	fmu->exitInitializationMode = (fmi2ExitInitializationModeTYPE *) getAdr(&s, h, "fmi2ExitInitializationMode");
	fmu->terminate = (fmi2TerminateTYPE *) getAdr(&s, h, "fmi2Terminate");
	fmu->reset = (fmi2ResetTYPE *) getAdr(&s, h, "fmi2Reset");
	fmu->getReal = (fmi2GetRealTYPE *) getAdr(&s, h, "fmi2GetReal");
	fmu->getInteger = (fmi2GetIntegerTYPE *) getAdr(&s, h, "fmi2GetInteger");
	fmu->getBoolean = (fmi2GetBooleanTYPE *) getAdr(&s, h, "fmi2GetBoolean");
	fmu->getString = (fmi2GetStringTYPE *) getAdr(&s, h, "fmi2GetString");
	fmu->setReal = (fmi2SetRealTYPE *) getAdr(&s, h, "fmi2SetReal");
	fmu->setInteger = (fmi2SetIntegerTYPE *) getAdr(&s, h, "fmi2SetInteger");
	fmu->setBoolean = (fmi2SetBooleanTYPE *) getAdr(&s, h, "fmi2SetBoolean");
	fmu->setString = (fmi2SetStringTYPE *) getAdr(&s, h, "fmi2SetString");
	fmu->getFMUstate = (fmi2GetFMUstateTYPE *) getAdr(&s, h, "fmi2GetFMUstate");
	fmu->setFMUstate = (fmi2SetFMUstateTYPE *) getAdr(&s, h, "fmi2SetFMUstate");
	fmu->freeFMUstate = (fmi2FreeFMUstateTYPE *) getAdr(&s, h, "fmi2FreeFMUstate");
	fmu->serializedFMUstateSize = (fmi2SerializedFMUstateSizeTYPE *) getAdr(&s, h, "fmi2SerializedFMUstateSize");
	fmu->serializeFMUstate = (fmi2SerializeFMUstateTYPE *) getAdr(&s, h, "fmi2SerializeFMUstate");
	fmu->deSerializeFMUstate = (fmi2DeSerializeFMUstateTYPE *) getAdr(&s, h, "fmi2DeSerializeFMUstate");
	fmu->getDirectionalDerivative = (fmi2GetDirectionalDerivativeTYPE *) getAdr(&s, h, "fmi2GetDirectionalDerivative");
#ifdef FMI_COSIMULATION
	fmu->setRealInputDerivatives = (fmi2SetRealInputDerivativesTYPE *) getAdr(&s, h, "fmi2SetRealInputDerivatives");
	fmu->getRealOutputDerivatives = (fmi2GetRealOutputDerivativesTYPE *) getAdr(&s, h, "fmi2GetRealOutputDerivatives");
	fmu->doStep = (fmi2DoStepTYPE *) getAdr(&s, h, "fmi2DoStep");
	fmu->cancelStep = (fmi2CancelStepTYPE *) getAdr(&s, h, "fmi2CancelStep");
	fmu->getStatus = (fmi2GetStatusTYPE *) getAdr(&s, h, "fmi2GetStatus");
	fmu->getRealStatus = (fmi2GetRealStatusTYPE *) getAdr(&s, h, "fmi2GetRealStatus");
	fmu->getIntegerStatus = (fmi2GetIntegerStatusTYPE *) getAdr(&s, h, "fmi2GetIntegerStatus");
	fmu->getBooleanStatus = (fmi2GetBooleanStatusTYPE *) getAdr(&s, h, "fmi2GetBooleanStatus");
	fmu->getStringStatus = (fmi2GetStringStatusTYPE *) getAdr(&s, h, "fmi2GetStringStatus");

	fmu->getMaxStepsize = (fmi2GetMaxStepsizeTYPE *) getAdrOptional(&s, h, "fmi2GetMaxStepsize", true);

#else // FMI2 for Model Exchange
	fmu->enterEventMode = (fmi2EnterEventModeTYPE *) getAdr(&s, h, "fmi2EnterEventMode");
	fmu->newDiscreteStates = (fmi2NewDiscreteStatesTYPE *) getAdr(&s, h, "fmi2NewDiscreteStates");
	fmu->enterContinuousTimeMode = (fmi2EnterContinuousTimeModeTYPE *) getAdr(&s, h, "fmi2EnterContinuousTimeMode");
	fmu->completedIntegratorStep = (fmi2CompletedIntegratorStepTYPE *) getAdr(&s, h, "fmi2CompletedIntegratorStep");
	fmu->setTime = (fmi2SetTimeTYPE *) getAdr(&s, h, "fmi2SetTime");
	fmu->setContinuousStates = (fmi2SetContinuousStatesTYPE *) getAdr(&s, h, "fmi2SetContinuousStates");
	fmu->getDerivatives = (fmi2GetDerivativesTYPE *) getAdr(&s, h, "fmi2GetDerivatives");
	fmu->getEventIndicators = (fmi2GetEventIndicatorsTYPE *) getAdr(&s, h, "fmi2GetEventIndicators");
	fmu->getContinuousStates = (fmi2GetContinuousStatesTYPE *) getAdr(&s, h, "fmi2GetContinuousStates");
	fmu->getNominalsOfContinuousStates = (fmi2GetNominalsOfContinuousStatesTYPE *) getAdr(&s, h, "fmi2GetNominalsOfContinuousStates");
#endif

	if (fmu->getVersion == NULL && fmu->instantiate == NULL)
	{
		printf("warning: Functions from FMI 2.0 could not be found in %s\n", dllPath);
		printf("warning: Simulator will look for FMI 2.0 RC1 functions names...\n");
		fmu->getTypesPlatform = (fmi2GetTypesPlatformTYPE *) getAdr(&s, h, "fmiGetTypesPlatform");
		fmu->getVersion = (fmi2GetVersionTYPE *) getAdr(&s, h, "fmiGetVersion");
		fmu->setDebugLogging = (fmi2SetDebugLoggingTYPE *) getAdr(&s, h, "fmiSetDebugLogging");
		fmu->instantiate = (fmi2InstantiateTYPE *) getAdr(&s, h, "fmiInstantiate");
		fmu->freeInstance = (fmi2FreeInstanceTYPE *) getAdr(&s, h, "fmiFreeInstance");
		fmu->setupExperiment = (fmi2SetupExperimentTYPE *) getAdr(&s, h, "fmiSetupExperiment");
		fmu->enterInitializationMode = (fmi2EnterInitializationModeTYPE *) getAdr(&s, h, "fmiEnterInitializationMode");
		fmu->exitInitializationMode = (fmi2ExitInitializationModeTYPE *) getAdr(&s, h, "fmiExitInitializationMode");
		fmu->terminate = (fmi2TerminateTYPE *) getAdr(&s, h, "fmiTerminate");
		fmu->reset = (fmi2ResetTYPE *) getAdr(&s, h, "fmiReset");
		fmu->getReal = (fmi2GetRealTYPE *) getAdr(&s, h, "fmiGetReal");
		fmu->getInteger = (fmi2GetIntegerTYPE *) getAdr(&s, h, "fmiGetInteger");
		fmu->getBoolean = (fmi2GetBooleanTYPE *) getAdr(&s, h, "fmiGetBoolean");
		fmu->getString = (fmi2GetStringTYPE *) getAdr(&s, h, "fmiGetString");
		fmu->setReal = (fmi2SetRealTYPE *) getAdr(&s, h, "fmiSetReal");
		fmu->setInteger = (fmi2SetIntegerTYPE *) getAdr(&s, h, "fmiSetInteger");
		fmu->setBoolean = (fmi2SetBooleanTYPE *) getAdr(&s, h, "fmiSetBoolean");
		fmu->setString = (fmi2SetStringTYPE *) getAdr(&s, h, "fmiSetString");
		fmu->getFMUstate = (fmi2GetFMUstateTYPE *) getAdr(&s, h, "fmiGetFMUstate");
		fmu->setFMUstate = (fmi2SetFMUstateTYPE *) getAdr(&s, h, "fmiSetFMUstate");
		fmu->freeFMUstate = (fmi2FreeFMUstateTYPE *) getAdr(&s, h, "fmiFreeFMUstate");
		fmu->serializedFMUstateSize = (fmi2SerializedFMUstateSizeTYPE *) getAdr(&s, h, "fmiSerializedFMUstateSize");
		fmu->serializeFMUstate = (fmi2SerializeFMUstateTYPE *) getAdr(&s, h, "fmiSerializeFMUstate");
		fmu->deSerializeFMUstate = (fmi2DeSerializeFMUstateTYPE *) getAdr(&s, h, "fmiDeSerializeFMUstate");
		fmu->getDirectionalDerivative = (fmi2GetDirectionalDerivativeTYPE *) getAdr(&s, h,
		"fmiGetDirectionalDerivative");
#ifdef FMI_COSIMULATION
		fmu->setRealInputDerivatives = (fmi2SetRealInputDerivativesTYPE *) getAdr(&s, h, "fmiSetRealInputDerivatives");
		fmu->getRealOutputDerivatives = (fmi2GetRealOutputDerivativesTYPE *) getAdr(&s, h,
		"fmiGetRealOutputDerivatives");
		fmu->doStep = (fmi2DoStepTYPE *) getAdr(&s, h, "fmiDoStep");
		fmu->cancelStep = (fmi2CancelStepTYPE *) getAdr(&s, h, "fmiCancelStep");
		fmu->getStatus = (fmi2GetStatusTYPE *) getAdr(&s, h, "fmiGetStatus");
		fmu->getRealStatus = (fmi2GetRealStatusTYPE *) getAdr(&s, h, "fmiGetRealStatus");
		fmu->getIntegerStatus = (fmi2GetIntegerStatusTYPE *) getAdr(&s, h, "fmiGetIntegerStatus");
		fmu->getBooleanStatus = (fmi2GetBooleanStatusTYPE *) getAdr(&s, h, "fmiGetBooleanStatus");
		fmu->getStringStatus = (fmi2GetStringStatusTYPE *) getAdr(&s, h, "fmiGetStringStatus");
#else // FMI2 for Model Exchange
		fmu->enterEventMode = (fmi2EnterEventModeTYPE *) getAdr(&s, h, "fmiEnterEventMode");
		fmu->newDiscreteStates = (fmi2NewDiscreteStatesTYPE *) getAdr(&s, h, "fmiNewDiscreteStates");
		fmu->enterContinuousTimeMode = (fmi2EnterContinuousTimeModeTYPE *) getAdr(&s, h, "fmiEnterContinuousTimeMode");
		fmu->completedIntegratorStep = (fmi2CompletedIntegratorStepTYPE *) getAdr(&s, h, "fmiCompletedIntegratorStep");
		fmu->setTime = (fmi2SetTimeTYPE *) getAdr(&s, h, "fmiSetTime");
		fmu->setContinuousStates = (fmi2SetContinuousStatesTYPE *) getAdr(&s, h, "fmiSetContinuousStates");
		fmu->getDerivatives = (fmi2GetDerivativesTYPE *) getAdr(&s, h, "fmiGetDerivatives");
		fmu->getEventIndicators = (fmi2GetEventIndicatorsTYPE *) getAdr(&s, h, "fmiGetEventIndicators");
		fmu->getContinuousStates = (fmi2GetContinuousStatesTYPE *) getAdr(&s, h, "fmiGetContinuousStates");
		fmu->getNominalsOfContinuousStates = (fmi2GetNominalsOfContinuousStatesTYPE *) getAdr(&s, h, "fmiGetNominalsOfContinuousStates");
#endif
	}
	return s;
}

//#define MAX_MSG_SIZE 1000
//void fmuLogger(void *componentEnvironment, fmi2String instanceName, fmi2Status status, fmi2String category,
//		fmi2String message, ...)
//{
//	char msg[MAX_MSG_SIZE];
//	char* copy;
//	va_list argp;
//
//	// replace C format strings
//	va_start(argp, message);
//	vsprintf(msg, message, argp);
//	va_end(argp);
//
//	// replace e.g. ## and #r12#
//#ifdef _WIN32
//	copy = _strdup(msg);
//#else
//	copy = strdup(msg);
//#endif
//	//FIXME replaceRefsInMessage(copy, msg, MAX_MSG_SIZE, &fmu);
//	free(copy);
//
//	// print the final message
//	if (!instanceName)
//		instanceName = "?";
//	if (!category)
//		category = "?";
//	printf("%s %s (%s): %s\n", fmi2StatusToString(status), instanceName, category, msg);
//}

int error(const char* message)
{
	printf("%s\n", message);
	return 0;
}

}

Fmu::Fmu(const char* path)
{
	this->path = make_shared<string>(path);
	this->handles = NULL;
	this->extractedDirectory = NULL;
}

bool Fmu::initialize()
{

	this->extractedDirectory = make_shared<string>("/tmp");
	if (this->unpack(path->c_str(), this->extractedDirectory->c_str()))
	{
		cout << "unzip ok" << endl;
	} else
	{
		cerr << "unzip failed: " << path << endl;
	}

	FMU* fmu = (FMU *) malloc(sizeof(FMU));

	auto fmuLoc = path->rfind(".fmu");

	auto subPath = path->substr(0, fmuLoc);

	auto fmuNameLoc = subPath.rfind(separator());

	auto name = subPath.substr(fmuNameLoc);

	auto dllPath = string(*this->extractedDirectory);
	dllPath.push_back(separator());
	dllPath.append("binaries");
	dllPath.push_back(separator());
	dllPath.append(platform);
//dllPath.push_back(separator());
	dllPath.append(name);
	dllPath.append(library_ext);

	if (!loadDll(dllPath.c_str(), fmu))
	{
		return false;
	}
	this->handles = fmu;
	return true;
}

void closeHandle(HMODULE handle)
{
#ifdef _WIN32
	FreeLibrary(handle);
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_MAC
	// Other kinds of Mac OS
	dlclose(handle);
#else
	// Unsupported platform
#endif
#elif __linux
	dlclose(handle);
#endif
}

Fmu::~Fmu()
{
	if (handles)
	{
		closeHandle(handles->dllHandle);
		delete handles;
		handles = NULL;
	}
}

bool Fmu::unpack(const char *zip_file_path, const char* output_folder)
{
	/*
	 Usage : miniunz [-e] [-x] [-v] [-l] [-o] [-p password] file.zip [file_to_extr.] [-d extractdir]
	 -e  Extract without pathname (junk paths)
	 -x  Extract with pathname
	 -v  list files
	 -l  list files
	 -d  directory to extract into
	 -o  overwrite files without prompting
	 -p  extract crypted file using password
	 */

	/* A call to minunz may change the current directory and therefore we must change it back */
	char cd[FILENAME_MAX];

	int argc = 6;
	const char *argv[6];
	int status;

//		jm_log_verbose(callbacks, module, "Unpacking FMU into %s", output_folder);

	argv[0] = "miniunz";
	argv[1] = "-x";
	argv[2] = "-o";
	argv[3] = zip_file_path;
	argv[4] = "-d";
	argv[5] = output_folder;

	/* Temporary save the current directory */
//		if (jm_portability_get_current_working_directory(cd, sizeof(cd) / sizeof(char)) == jm_status_error) {
//			jm_log_fatal(callbacks, module, "Could not get Current Directory");
//			return jm_status_error;
//		}
	/* Unzip */
	status = miniunz(argc, (const char**) argv);
	return status;
}

shared_ptr<string> Fmu::fmi2StatusToString(fmi2Status status)
{
	switch (status)
	{
	case fmi2OK:
		return make_shared<string>("ok");
	case fmi2Warning:
		return make_shared<string>("warning");
	case fmi2Discard:
		return make_shared<string>("discard");
	case fmi2Error:
		return make_shared<string>("error");
	case fmi2Fatal:
		return make_shared<string>("fatal");
#ifdef FMI_COSIMULATION
	case fmi2Pending:
		return make_shared<string>("fmi2Pending");
#endif
	default:
		return make_shared<string>("?");
	}
}

/* FMI common */
const char* Fmu::getTypesPlatform(void)
{
	return handles->getTypesPlatform();
}

const char* Fmu::getVersion(void)
{
	return this->handles->getVersion();
}
fmi2Status Fmu::setDebugLogging(fmi2Component a, fmi2Boolean b, size_t c, const fmi2String* d)
{
	return this->handles->setDebugLogging(a, b, c, d);
}

void Callback::log(fmi2String instanceName, fmi2Status status, fmi2String category, fmi2String message)
{
	cout << Fmu::fmi2StatusToString(status)->c_str() << " " << instanceName << " - " << category << " " << message
			<< endl;

}

shared_ptr<ComponentContext> Fmu::getContext(const char* instanceName)
{

	auto itr = activeCallbacks.find(instanceName);

	if (itr == activeCallbacks.end())
	{
		return NULL;
	} else
	{
		return itr->second;
	}
}

void fmuLogger(void *componentEnvironment, fmi2String instanceName, fmi2Status status, fmi2String category,
		fmi2String message, ...)
{
	va_list argp;

	// replace C format strings
	va_start(argp, message);
	size_t size = vsnprintf( nullptr, 0, message, argp ) + 1; // Extra space for '\0'
	va_end(argp);

	va_start(argp, message);
	unique_ptr<char[]> buf(new char[size]);
	vsnprintf(buf.get(), size, message, argp);
	va_end(argp);

	string completeMessage = string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside

	Fmu* fmu = (Fmu*) componentEnvironment;


	shared_ptr<ComponentContext> context = (fmu==NULL?NULL:fmu->getContext(instanceName));

	if (context != NULL)
	{
		context->callbacks->log(instanceName, status, category, completeMessage.c_str());
	} else
	{
		cout << "Name: " << instanceName << " Status: " << status << " Category: " << category << " Msg: "
				<< completeMessage << endl;
	}

}

/* Creation and destruction of FMU instances and setting debug status */
fmi2Component Fmu::instantiate(fmi2String instanceName, fmi2Type fmuType, fmi2String fmuGUID,
		 fmi2Boolean visible, fmi2Boolean loggingOn, shared_ptr<Callback> callback)
{
	fmi2CallbackFunctions *functions = (fmi2CallbackFunctions *) malloc(sizeof(fmi2CallbackFunctions));

	fmi2CallbackFunctions _functions =
	{ fmuLogger, calloc, free, NULL, this };

	memcpy(functions, &_functions, sizeof(fmi2CallbackFunctions));

	auto context = make_shared<ComponentContext>();
	context->functions = functions;
	context->callbacks = callback;

	activeCallbacks.insert(make_pair(instanceName, context));

	auto fmuResourceLocation = string("file:");
		fmuResourceLocation.append(*this->extractedDirectory);
		fmuResourceLocation.push_back('/');
		fmuResourceLocation.append("resources");

	fmi2Component comp = this->instantiate(instanceName, fmuType, fmuGUID, fmuResourceLocation.c_str(), functions, visible,
			loggingOn);

	if (comp == NULL)
	{
		activeCallbacks.erase(activeCallbacks.find(instanceName));
	}
	return comp;
}

fmi2Component Fmu::instantiate(fmi2String instanceName, fmi2Type fmuType, fmi2String fmuGUID,fmi2String fmuResourceLocation,
		 const fmi2CallbackFunctions* functions, fmi2Boolean visible,
		fmi2Boolean loggingOn)
{

	return this->handles->instantiate(instanceName, fmuType, fmuGUID, fmuResourceLocation, functions, visible,
			loggingOn);
}
void Fmu::freeInstance(fmi2Component c)
{
	return this->handles->freeInstance(c);
}

/* Enter and exit initialization mode, terminate and reset */
fmi2Status Fmu::setupExperiment(fmi2Component a, fmi2Boolean b, fmi2Real c, fmi2Real d, fmi2Boolean e, fmi2Real f)
{
	return this->handles->setupExperiment(a, b, c, d, e, f);
}
fmi2Status Fmu::enterInitializationMode(fmi2Component c)
{
	return this->handles->enterInitializationMode(c);
}
fmi2Status Fmu::exitInitializationMode(fmi2Component c)
{
	return this->handles->exitInitializationMode(c);
}
fmi2Status Fmu::terminate(fmi2Component c)
{
	return this->handles->terminate(c);
}
fmi2Status Fmu::reset(fmi2Component c)
{
	return this->handles->reset(c);
}

/* Getting and setting variable values */
fmi2Status Fmu::getReal(fmi2Component comp, const fmi2ValueReference* vr, size_t s, fmi2Real* val)
{
	return this->handles->getReal(comp, vr, s, val);
}
fmi2Status Fmu::getInteger(fmi2Component comp, const fmi2ValueReference* vr, size_t s, fmi2Integer* val)
{
	return this->handles->getInteger(comp, vr, s, val);
}
fmi2Status Fmu::getBoolean(fmi2Component comp, const fmi2ValueReference* vr, size_t s, fmi2Boolean* val)
{
	return this->handles->getBoolean(comp, vr, s, val);
}
fmi2Status Fmu::getString(fmi2Component comp, const fmi2ValueReference* vr, size_t s, fmi2String* val)
{
	return this->handles->getString(comp, vr, s, val);
}

fmi2Status Fmu::setReal(fmi2Component comp, const fmi2ValueReference* vr, size_t s, const fmi2Real* val)
{
	return this->handles->setReal(comp, vr, s, val);
}
fmi2Status Fmu::setInteger(fmi2Component comp, const fmi2ValueReference* vr, size_t s, const fmi2Integer* val)
{
	return this->handles->setInteger(comp, vr, s, val);
}
fmi2Status Fmu::setBoolean(fmi2Component comp, const fmi2ValueReference* vr, size_t s, const fmi2Boolean* val)
{
	return this->handles->setBoolean(comp, vr, s, val);
}
fmi2Status Fmu::setString(fmi2Component comp, const fmi2ValueReference* vr, size_t s, const fmi2String* val)
{
	return this->handles->setString(comp, vr, s, val);
}

/* Getting and setting the internal Fmu state */
fmi2Status Fmu::getFMUstate(fmi2Component comp, fmi2FMUstate* state)
{
	return this->handles->getFMUstate(comp, state);
}
fmi2Status Fmu::setFMUstate(fmi2Component comp, fmi2FMUstate state)
{
	return this->handles->setFMUstate(comp, state);
}
fmi2Status Fmu::freeFMUstate(fmi2Component comp, fmi2FMUstate* state)
{
	return this->handles->freeFMUstate(comp, state);
}
fmi2Status Fmu::serializedFMUstateSize(fmi2Component comp, fmi2FMUstate state, size_t* s)
{
	return this->handles->serializedFMUstateSize(comp, state, s);
}
fmi2Status Fmu::serializeFMUstate(fmi2Component comp, fmi2FMUstate state, fmi2Byte* v, size_t s)
{
	return this->handles->serializeFMUstate(comp, state, v, s);
}
fmi2Status Fmu::deSerializeFMUstate(fmi2Component comp, const fmi2Byte* v, size_t s, fmi2FMUstate* state)
{
	return this->handles->deSerializeFMUstate(comp, v, s, state);
}

/* Getting partial derivatives */
fmi2Status Fmu::getDirectionalDerivative(fmi2Component comp, const fmi2ValueReference* vr, size_t s1,
		const fmi2ValueReference* vr1, size_t s2, const fmi2Real* vr2, fmi2Real* vr3)
{
	return this->handles->getDirectionalDerivative(comp, vr, s1, vr1, s2, vr2, vr3);
}

} /* namespace fmi2 */
