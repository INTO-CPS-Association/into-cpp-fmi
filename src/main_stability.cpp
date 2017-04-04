#include "Fmu.h"
#include <iostream>
#include <memory>
#include <fstream>

using namespace std;
using namespace fmi2;

#define SIM20

#ifdef OM
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

#elif defined(SIM20)

#define FMU1_NAME "MassSpringDamper1.fmu"
#define FMU1_GUID "{da03fa66-a02a-43d6-a906-db865e2ea044}"
#define FMU1_m1 2
#define FMU1_d1 1
#define FMU1_c1 0
#define FMU1_fk 5
#define FMU1_x1 7
#define FMU1_v1 6

#define FMU2_NAME "MassSpringDamper2.fmu"
#define FMU2_GUID "{9113a387-2c02-42ae-ae68-5073bf845034}"
#define FMU2_m2 4
#define FMU2_dc 3
#define FMU2_d2 1
#define FMU2_cc 2
#define FMU2_c2 0
#define FMU2_fk 7
#define FMU2_x2 11
#define FMU2_v2 10
#define FMU2_x1 8
#define FMU2_v1 9

#endif

fmi2Status setReal(shared_ptr<FmuComponent> comp, fmi2ValueReference id,
		fmi2Real val) {
	const fmi2ValueReference vr[] { id };
	size_t nvr = 1;
	fmi2Real value[] { val };
	return comp->fmu->setReal(comp->component, vr, nvr, value);
}

fmi2Real getReal(shared_ptr<FmuComponent> comp, fmi2ValueReference id) {
	const fmi2ValueReference vr[] { id };
	size_t nvr = 1;
	fmi2Real value[1];
	comp->fmu->getReal(comp->component, vr, nvr, value);

	return value[0];
}

int main(int argc, char *argv[]) {
	auto path1 = argv[1];
	auto path2 = argv[2];

	cout << "Path 1: " << path1 << endl;
	cout << "Path 2: " << path2 << endl;

	cout << "Creating path 1 fmu" << endl;
	auto fmu1 = make_shared<fmi2::Fmu>(string(path1));
	auto init1 = fmu1->initialize();
	cout << init1 << endl;
	if (!init1)
		return -1;
	cout << fmu1->getVersion() << endl;

	cout << "Creating path 2 fmu" << endl;
	auto fmu2 = make_shared<fmi2::Fmu>(string(path2));
	auto init2 = fmu2->initialize();
	cout << init2 << endl;
	if (!init2)
		return -1;
	cout << fmu2->getVersion() << endl;

	cout << "Instantiating..." << endl;

	cout << "Instantiating path 1 fmu" << endl;
	auto callback1 = make_shared<fmi2::Callback>();
	auto instance1 = fmu1->instantiate(FMU1_NAME, fmi2CoSimulation, FMU1_GUID,
			true, true, callback1);
	cout << "path 1 fmu instance: " << instance1->component << endl;

	cout << "Instantiating path 2 fmu" << endl;
	auto callback2 = make_shared<fmi2::Callback>();
	auto instance2 = fmu2->instantiate(FMU2_NAME, fmi2CoSimulation, FMU2_GUID,
			true, true, callback2);
	cout << "path 1 fmu instance: " << instance2->component << endl;

	// Simulation
	auto time = 0.0;
	auto endTime = 10.0;
	auto stepSize = 0.001;

	//prepare
	instance1->fmu->setupExperiment(instance1->component, false, 0.0, 0.0, true,
			endTime);
	instance1->fmu->enterInitializationMode(instance1->component);

	instance2->fmu->setupExperiment(instance2->component, false, 0.0, 0.0, true,
			endTime);
	instance2->fmu->enterInitializationMode(instance2->component);

	//set parameters
	fmi2ValueReference vr2[7] = { 0, 1, 2, 3, 4, 5, 6 };
	fmi2Real val2[7] = { 1.0, 2.0, 100.0, 100.0, 1.0, 0.0, 0.0 };
	instance2->fmu->setReal(instance2->component, vr2, 7, val2);

	fmi2ValueReference vr1[5] = { 0, 1, 2, 3, 4 };
	fmi2Real val1[5] = { 1.0, 1.0, 1.0, 0.0, 1.0 };
	instance1->fmu->setReal(instance1->component, vr1, 5, val1);

	setReal(instance2, 8, 0.0);
	setReal(instance2, 9, 0.0);
	setReal(instance1, 5, 0.0);

	setReal(instance2, 8, 0.0);
	setReal(instance2, 9, 0.0);
	setReal(instance2, 5, 0.0);
//	setReal(instance1, 4, 1.0);
//	setReal(instance1, 3, 0.0);
//
//	setReal(instance1, FMU1_c1, 1.0);
//	setReal(instance1, FMU1_d1, 1.0);
//	setReal(instance1, FMU1_m1, 1.0);
//
//	setReal(instance1, FMU1_fk, 0.0);

//	fmi2ValueReference vr2[7] =
//	{ FMU2_m2, FMU2_dc, FMU2_d2, FMU2_cc, FMU2_c2,8,9 };
//	fmi2Real val2[7] =
//	{ 1.0, 100.0, 2.0, 1.0, 1.0,0.0,0.0 };
//	instance2->fmu->setReal(instance2->component, vr2, 7, val2);

//	setReal(instance2, 6, 0.0);
//	setReal(instance2, 5, 0.0);
//
//	setReal(instance2, FMU2_c2, 1.0);
//	setReal(instance2, FMU2_d2, 2.0);
//	setReal(instance2, FMU2_cc, 1.0);
//	setReal(instance2, FMU2_dc, 100.0);
//	setReal(instance2, FMU2_m2, 1.0);
//
//	setReal(instance2, FMU2_x2, 0.0);
//	setReal(instance2, FMU2_v2, 0.0);

	instance1->fmu->exitInitializationMode(instance1->component);
	instance2->fmu->exitInitializationMode(instance2->component);

	fmi2ValueReference vr2_out[3] = { FMU2_fk, FMU2_x2, FMU2_v2 };
	fmi2ValueReference vr2_in[2] = { FMU2_x1, FMU2_v1 };

	fmi2ValueReference vr1_out[2] = { FMU1_x1, FMU1_v1 };
	fmi2ValueReference vr1_in[1] = { FMU1_fk };

	fmi2Real vals[3] = { 0.0, 0.0, 0.0 };

	fmi2Real vals2[2] = { 0.0, 0.0 };

	std::fstream fs;
	//\\psf\Home\data\into-cps\into-cps_stability-check-project\mass-springer-damper\FMUs\Dymola
	//fs.open("result.csv", std::fstream::in | std::fstream::out | std::fstream::trunc);
	fs.open(
			"\\\\psf\\Home\\data\\into-cps\\into-cps_stability-check-project\\mass-springer-damper\\FMUs\\Dymola\\result.csv",
			std::fstream::in | std::fstream::out | std::fstream::trunc);

	//cout << "------------ Header ---------" << endl;
	fs << "\"time\",\"fk\",\"x2\",\"v2\",\"x1\",\"v1\"" << endl;


	//34 = 2

	instance2->fmu->getReal(instance2->component, vr2_out, 3, vals);

	cout << "fk: " << vals[0] << endl; //2 fk
	cout << "x2:  " << vals[1] << endl; //2 x2
	cout << "v2: " << vals[2] << endl; //2 v2

	instance1->fmu->getReal(instance1->component, vr1_out, 2, vals2);

	cout << "x1: " << vals2[0] << endl; // 1 x1
	cout << "v1: " << vals2[1] << endl; //1 v1

	fmi2FMUstate s1=NULL;
	instance1->fmu->getFMUstate(instance1->component, &s1);
	fmi2FMUstate s2=NULL;
	instance2->fmu->getFMUstate(instance2->component, &s2);


	bool f = true;
	while (time < endTime) {
		cout << ".";
		// get
		for (int repeat = 0; repeat < 6; repeat++)
		{
		fs << time << ",";
			instance1->fmu->setFMUstate(instance1->component, s1);
		instance2->fmu->setFMUstate(instance2->component, s2);

		fs << vals[0] << ","; //2 fk
		fs << vals[1] << ","; //2 x2
		fs << vals[2] << ","; //2 v2

		fs << vals2[0] << ","; // 1 x1
		fs << vals2[1]; //1 v1
		fs << endl;

		//copy fk
		//i/f(!f){
		//f=true;
		instance1->fmu->setReal(instance1->component, vr1_in, 1, vals);

		instance2->fmu->setReal(instance2->component, vr2_in, 2, vals2);
//		fs << vals2[0] << "," << vals2[1] << endl;
		//}
		//dostep
		instance1->fmu->doStep(instance1->component, time, stepSize, false);
		instance2->fmu->doStep(instance2->component, time, stepSize, false);

		//get
		instance2->fmu->getReal(instance2->component, vr2_out, 3, vals);
		instance1->fmu->getReal(instance1->component, vr1_out, 2, vals2);
			}
		time += stepSize;

		instance1->fmu->getFMUstate(instance1->component, &s1);
		instance2->fmu->getFMUstate(instance2->component, &s2);
	}

	cout << "Done." << endl;

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
