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
#include <cstring>
#include <memory>
#include <iostream>
#include <string>
#include <cstdio>
#include <stdarg.h>

#include "FilesystemUtil.h"
#include "FmuLoader.h"

namespace fmi2
{



class Callback
{
public:
	virtual ~Callback();
	virtual void log(fmi2String instanceName, fmi2Status status, fmi2String category, fmi2String message);
};

typedef struct
{
	fmi2CallbackFunctions * functions;
	weak_ptr<Callback> callbacks;
} ComponentContext;

 struct FmuComponent;

class Fmu :public std::enable_shared_from_this<Fmu>
{
public:
	Fmu(string path);
	bool initialize();
	virtual ~Fmu();

	shared_ptr<ComponentContext> getContext(const char* instanceName);

	static shared_ptr<string> fmi2StatusToString(fmi2Status status);
	static shared_ptr<string> combinePath(shared_ptr<string> path1, shared_ptr<string> path3);

	shared_ptr<FmuComponent> instantiate(fmi2String instanceName, fmi2Type fmuType, fmi2String fmuGUID, fmi2Boolean visible,
			fmi2Boolean loggingOn, weak_ptr<Callback> callback);

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

  //Other
  std::shared_ptr<string> getPath();
private:

	std::shared_ptr<string> extractedDirectory;
	std::shared_ptr<string> path;
	static const char* platform;
	static const char* library_ext;
	FMU* handles;
	std::map<string, shared_ptr<ComponentContext>> activeCallbacks;

	bool unpack(const char *zip_file_path, const char* output_folder);

};


 struct FmuComponent
{
	shared_ptr<Fmu> fmu;
	fmi2Component component;
} ;

} /* namespace fmi2 */

#endif /* SRC_FMU_H_ */
