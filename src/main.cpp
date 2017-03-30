#include "Fmu.h"
#include <iostream>

using namespace std;
using namespace fmi2;
int main()
{
cout << "hello" << endl;
	auto fmu =make_shared< fmi2::Fmu>(string("/Users/kel/data/into-cps/case-studies/case-study_single_watertank/FMUs/singlewatertank-20sim.fmu"));
	cout <<fmu->initialize()<<endl;
	cout << fmu->getVersion() << endl;

	auto callback = make_shared<fmi2::Callback>();
	auto instance = fmu->instantiate("k",fmi2CoSimulation,"{cfc65592-9ece-4563-9705-1581b6e7071c}",true,true,callback);

	cout <<"instance: " << instance->component   << endl;
}
