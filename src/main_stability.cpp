#include "Fmu.h"
#include <iostream>
#include <memory>
#include <fstream>

using namespace std;
using namespace fmi2;

#define FMU1_NAME "MassSpringDamper1.fmu"
#define FMU1_GUID "{8bb9f6f8-3436-4f87-93bb-6981ab480991}"
#define FMU1_m1 7
#define FMU1_d1 6
#define FMU1_c1 5
#define FMU1_fk 4
#define FMU1_x1 1
#define FMU1_v1 0

#define FMU2_NAME "MassSpringDamper2.fmu"
#define FMU2_GUID "{9409625b-1611-4a9f-ba98-b6f3923b9fca}"
#define FMU2_m2 11
#define FMU2_dc 10
#define FMU2_d2 9
#define FMU2_cc 8
#define FMU2_c2 7
#define FMU2_fk 4
#define FMU2_x1 6
#define FMU2_v1 5

int main(int argc, char *argv[])
{
	auto path1 = argv[1];
	auto path2 = argv[2];

	cout << "Path 1: " << path1 << endl;
	cout << "Path 2: " << path2 << endl;

	cout << "Creating path 1 fmu" << endl;
	auto fmu1 = make_shared<fmi2::Fmu>(path1);
	auto init1 = fmu1->initialize();
	cout << init1 << endl;
	if(!init1)
		return -1;
	cout << fmu1->getVersion() << endl;

	cout << "Creating path 2 fmu" << endl;
	auto fmu2 = make_shared<fmi2::Fmu>(path2);
	auto init2 = fmu2->initialize();
	cout << init2 << endl;
	if(!init2)
		return -1;
	cout << fmu2->getVersion() << endl;

	cout << "Instantiating..." << endl;

	cout << "Instantiating path 1 fmu" << endl;
	auto callback1 = make_shared<fmi2::Callback>();
	auto instance1 = fmu1->instantiate(FMU1_NAME, fmi2CoSimulation, FMU1_GUID, true, true, callback1);
	cout << "path 1 fmu instance: " << instance1->component << endl;

	cout << "Instantiating path 2 fmu" << endl;
	auto callback2 = make_shared<fmi2::Callback>();
	auto instance2 = fmu2->instantiate(FMU2_NAME, fmi2CoSimulation, FMU2_GUID, true, true, callback2);
	cout << "path 1 fmu instance: " << instance2->component << endl;

	//set parameters
	fmi2ValueReference vr1[3] =
	{ FMU1_m1, FMU1_d1, FMU1_c1 };
	fmi2Real val1[3] =
	{ 1.0, 1.0, 1.0 };
	instance1->fmu->setReal(instance1->component, vr1, 3, val1);

	fmi2ValueReference vr2[5] =
	{ FMU2_m2, FMU2_dc, FMU2_d2, FMU2_cc, FMU2_c2 };
	fmi2Real val2[5] =
	{ 1.0, 50.0, 2.0, 1.0, 1.0 };
	instance2->fmu->setReal(instance2->component, vr2, 5, val2);

	// Simulation
	auto time = 0.0;
	auto endTime = 1.0;
	auto stepSize = 0.01;

	//prepare
	instance1->fmu->setupExperiment(instance1->component, false, 0.0, 0.0, true, endTime);
	instance1->fmu->enterInitializationMode(instance1->component);
	instance1->fmu->exitInitializationMode(instance1->component);

	instance2->fmu->setupExperiment(instance2->component, false, 0.0, 0.0, true, endTime);
	instance2->fmu->enterInitializationMode(instance2->component);
	instance2->fmu->exitInitializationMode(instance2->component);

	fmi2ValueReference vr2_out[1] =
	{ FMU2_fk };
	fmi2ValueReference vr2_in[2] =
	{ FMU2_x1, FMU2_v1 };

	fmi2ValueReference vr1_out[2] =
	{ FMU2_x1, FMU1_v1 };
	fmi2ValueReference vr1_in[1] =
	{ FMU2_fk };

	fmi2Real vals[2] =
	{ 0.0, 0.0 };

	std::fstream fs;
	  fs.open ("result.csv", std::fstream::in | std::fstream::out | std::fstream::trunc);


	//cout << "------------ Header ---------" << endl;
	fs << "\"{FMU2}.FMU2Instance.fk\",\"{FMU}.FMUInstance.x1\",\"{FMU}.FMUInstance.v1\"" << endl;
	while (time < endTime)
	{

		// get

		//copy fk
		instance2->fmu->getReal(instance2->component, vr2_out, 1, vals);
		instance1->fmu->setReal(instance1->component, vr1_in, 1, vals);
		fs << vals[0] << ",";

		instance1->fmu->getReal(instance1->component, vr1_out, 2, vals);
		instance2->fmu->setReal(instance2->component, vr2_in, 2, vals);
		fs << vals[0] << "," << vals[1] << endl;

		instance1->fmu->doStep(instance1->component, time, stepSize, false);
		instance2->fmu->doStep(instance2->component, time, stepSize, false);

		time += stepSize;
	}

	cout << "Done."<<endl;

//	cout << " ----------- end --------------" << endl;

//	cout << "hello" << endl;
//	auto fmu = make_shared<fmi2::Fmu>(
//			"/Users/kel/data/into-cps/case-studies/case-study_single_watertank/FMUs/singlewatertank-20sim.fmu");
//	cout << fmu->initialize() << endl;
//	cout << fmu->getVersion() << endl;
//
//	auto callback = make_shared<fmi2::Callback>();
//	auto instance = fmu->instantiate("k", fmi2CoSimulation, "{cfc65592-9ece-4563-9705-1581b6e7071c}", true, true,
//			callback);
//
//	cout << "instance: " << instance->component << endl;
}
