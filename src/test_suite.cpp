/*
 * test_suite.cpp
 *
 *  Created on: Jan 18, 2014
 *      Author: ctsotskas
 */


#include "mots.h"

#include "objective_function_formulae.h"
#include "objective_function.h"
#include "verification_code.h"

#include "test_suite.h"
#include "external_configuration_file.h"
#include "configuration_settings.h"

test_suite::test_suite(){
	//optimisation


	// zdt1 mots

}




void test_suite::test_zdt1(){

	//prepares a fresh run of mots2 for zdt1 configurations

	//feeds the information to mots2

	//runs mots2

	//collect results

}



void test_suite::test_all(){
	test_zdt1();

}

test_report tabu_test_ZDT1(std::string mycasename, int flag){


	//create configuration object
	external_configuration_file ext_conf_file("configuration_ZDT1.txt");
	ConfigurationSettings myConf2("demo_1",
			ext_conf_file,
			"reference_point_ZDT1.txt",
			"penalty_point_ZDT1.txt",
			"lower_bound_ZDT1.txt",
			"upper_bound_ZDT1.txt",
			"starting_point_ZDT1.txt",
			"current_step_ZDT1.txt");

	objective_function_formulae ZDT1_test(ext_conf_file.getObj());

	//todo: pass ONLY configuration (the rest of information will be extracted from the configuration file)

	ObjectiveFunctionBasic<double> TS_ObjFunc(myConf2, ZDT1_test);

	Container2 MTM(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "MTM","./memories");
	Container2 IM(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "IM","./memories");
	Container2 HISTORY(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "HISTORY","./memories");

	STM_Container2 STM(myConf2.getExternalConfigurationFile().getStmSize(), myConf2.getExternalConfigurationFile().getVar(), "STM", "./memories");
	LTM_Container2Basic2<double> LTM( myConf2.getExternalConfigurationFile().getVar() ,  myConf2.getExternalConfigurationFile().getRegions(), myConf2.get_lower_bound(), myConf2.get_upper_bound(),"LTM", "./memories");
	std::cout << "Memories done!" << std::endl;

	TabuSearch TS(mycasename, myConf2, flag, TS_ObjFunc, MTM, IM, HISTORY, STM, LTM);

	double temp1 ;
	temp1=TS.search2(mycasename );
	std::cout << " HV: " << temp1 << std::endl;


	return test_report(mycasename, TS.getDatumPnt(), temp1) ;

}

test_report tabu_test_ZDT2(std::string mycasename, int flag, int nVar){


	//create configuration object
//	external_configuration_file ext_conf_file("configuration_ZDT2.txt");

	external_configuration_file ext_conf_file("none",
			20, //1
			10, //2
			35, //3
			0.05, //4
			0.5, //5
			175, //6
			(int) std::ceil(nVar/5), //7
			nVar, //8
			2, //9
			0, //10 loop limit
			(int) std::ceil(nVar/5)*3000, //11 evaluations limit
			0, //12 Improvements limit , number of consecutive improvements
			4, //13
			(int) std::ceil( nVar * 2/3), //14
			"HV", // 15
			"full", //16
			1, //17
			2000000, //18
			2000000); //19

	ConfigurationSettings myConf2("demo_1",
			ext_conf_file,
			"reference_point_ZDT2.txt",
			"penalty_point_ZDT2.txt",
			"lower_bound_ZDT2.txt",
			"upper_bound_ZDT2.txt",
			"starting_point_ZDT2.txt",
			"current_step_ZDT2.txt");

	objective_function_formulae ZDT2_test(ext_conf_file.getObj());

	//todo: pass ONLY configuration (the rest of information will be extracted from the configuration file)

	ObjectiveFunctionBasic<double> TS_ObjFunc(myConf2, ZDT2_test);

	Container2 MTM(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "MTM","./memories");
	Container2 IM(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "IM","./memories");
	Container2 HISTORY(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "HISTORY","./memories");

	STM_Container2 STM(myConf2.getExternalConfigurationFile().getStmSize(), myConf2.getExternalConfigurationFile().getVar(), "STM", "./memories");
	LTM_Container2Basic2<double> LTM( myConf2.getExternalConfigurationFile().getVar() ,  myConf2.getExternalConfigurationFile().getRegions(), myConf2.get_lower_bound(), myConf2.get_upper_bound(),"LTM", "./memories");
	std::cout << "Memories done!" << std::endl;

	TabuSearch TS(mycasename, myConf2, flag, TS_ObjFunc, MTM, IM, HISTORY, STM, LTM);

	double temp1 ;
	temp1=TS.search2(mycasename );
	std::cout << " HV: " << temp1 << std::endl;

	test_report tr1(mycasename, TS.getDatumPnt(), temp1) ;
	return tr1;
}

test_report tabu_test_ZDT2_many_nvar(std::string mycasename, int flag){


	//create configuration object
	external_configuration_file ext_conf_file("configuration_ZDT2.txt");
	ConfigurationSettings myConf2("demo_1",
			ext_conf_file,
			"reference_point_ZDT2.txt",
			"penalty_point_ZDT2.txt",
			"lower_bound_ZDT2.txt",
			"upper_bound_ZDT2.txt",
			"starting_point_ZDT2.txt",
			"current_step_ZDT2.txt");

	objective_function_formulae ZDT2_test(ext_conf_file.getObj());

	//todo: pass ONLY configuration (the rest of information will be extracted from the configuration file)

	ObjectiveFunctionBasic<double> TS_ObjFunc(myConf2, ZDT2_test);

	Container2 MTM(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "MTM","./memories");
	Container2 IM(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "IM","./memories");
	Container2 HISTORY(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "HISTORY","./memories");

	STM_Container2 STM(myConf2.getExternalConfigurationFile().getStmSize(), myConf2.getExternalConfigurationFile().getVar(), "STM", "./memories");
	LTM_Container2Basic2<double> LTM( myConf2.getExternalConfigurationFile().getVar() ,  myConf2.getExternalConfigurationFile().getRegions(), myConf2.get_lower_bound(), myConf2.get_upper_bound(),"LTM", "./memories");
	std::cout << "Memories done!" << std::endl;

	TabuSearch TS(mycasename, myConf2, flag, TS_ObjFunc, MTM, IM, HISTORY, STM, LTM);

	double temp1 ;
	temp1=TS.search2(mycasename );
	std::cout << " HV: " << temp1 << std::endl;

	return test_report(mycasename, TS.getDatumPnt(), temp1) ;
}


test_report tabu_test_ZDT3(std::string mycasename, int flag){


	//create configuration object
	external_configuration_file ext_conf_file( ("configuration_ZDT3.txt") );
	ConfigurationSettings myConf2("demo_1",
			ext_conf_file,
			"reference_point_ZDT3.txt",
			"penalty_point_ZDT3.txt",
			"lower_bound_ZDT3.txt",
			"upper_bound_ZDT3.txt",
			"starting_point_ZDT3.txt",
			"current_step_ZDT3.txt");

	objective_function_formulae ZDT3_test(ext_conf_file.getObj());

	//todo: pass ONLY configuration (the rest of information will be extracted from the configuration file)

	ObjectiveFunctionBasic<double> TS_ObjFunc(myConf2, ZDT3_test);

	Container2 MTM(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "MTM","./memories");
	Container2 IM(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "IM","./memories");
	Container2 HISTORY(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "HISTORY","./memories");

	STM_Container2 STM(myConf2.getExternalConfigurationFile().getStmSize(), myConf2.getExternalConfigurationFile().getVar(), "STM", "./memories");
	LTM_Container2Basic2<double> LTM( myConf2.getExternalConfigurationFile().getVar() ,  myConf2.getExternalConfigurationFile().getRegions(), myConf2.get_lower_bound(), myConf2.get_upper_bound(),"LTM", "./memories");
	std::cout << "Memories done!" << std::endl;

	TabuSearch TS(mycasename, myConf2, flag, TS_ObjFunc, MTM, IM, HISTORY, STM, LTM);

	double temp1 ;
	temp1=TS.search2(mycasename );
	std::cout << " HV: " << temp1 << std::endl;


	return test_report(mycasename, TS.getDatumPnt(), temp1) ;

}

test_report tabu_test_ZDT4(std::string mycasename, int flag){


	//create configuration object
	external_configuration_file ext_conf_file( ("configuration_ZDT4.txt") );
	ConfigurationSettings myConf2("demo_1",
			ext_conf_file,
			"reference_point_ZDT4.txt",
			"penalty_point_ZDT4.txt",
			"lower_bound_ZDT4.txt",
			"upper_bound_ZDT4.txt",
			"starting_point_ZDT4.txt",
			"current_step_ZDT4.txt");

	objective_function_formulae ZDT4_test(ext_conf_file.getObj());

	//todo: pass ONLY configuration (the rest of information will be extracted from the configuration file)

	ObjectiveFunctionBasic<double> TS_ObjFunc(myConf2, ZDT4_test);

	Container2 MTM(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "MTM","./memories");
	Container2 IM(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "IM","./memories");
	Container2 HISTORY(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "HISTORY","./memories");

	STM_Container2 STM(myConf2.getExternalConfigurationFile().getStmSize(), myConf2.getExternalConfigurationFile().getVar(), "STM", "./memories");
	LTM_Container2Basic2<double> LTM( myConf2.getExternalConfigurationFile().getVar() ,  myConf2.getExternalConfigurationFile().getRegions(), myConf2.get_lower_bound(), myConf2.get_upper_bound(),"LTM", "./memories");
	std::cout << "Memories done!" << std::endl;

	TabuSearch TS(mycasename, myConf2, flag, TS_ObjFunc, MTM, IM, HISTORY, STM, LTM);

	double temp1 ;
	temp1=TS.search2(mycasename );
	std::cout << " HV: " << temp1 << std::endl;


	return test_report(mycasename, TS.getDatumPnt(), temp1) ;

}

test_report tabu_test_ZDT6(std::string mycasename, int flag){


	//create configuration object
	external_configuration_file ext_conf_file( ("configuration_ZDT6.txt") );
	ConfigurationSettings myConf2("demo_1",
			ext_conf_file,
			"reference_point_ZDT6.txt",
			"penalty_point_ZDT6.txt",
			"lower_bound_ZDT6.txt",
			"upper_bound_ZDT6.txt",
			"starting_point_ZDT6.txt",
			"current_step_ZDT6.txt");

	objective_function_formulae ZDT6_test(ext_conf_file.getObj());

	//todo: pass ONLY configuration (the rest of information will be extracted from the configuration file)

	ObjectiveFunctionBasic<double> TS_ObjFunc(myConf2, ZDT6_test);

	Container2 MTM(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "MTM","./memories");
	Container2 IM(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "IM","./memories");
	Container2 HISTORY(myConf2.getExternalConfigurationFile().getVar(), myConf2.getExternalConfigurationFile().getObj(), "HISTORY","./memories");

	STM_Container2 STM(myConf2.getExternalConfigurationFile().getStmSize(), myConf2.getExternalConfigurationFile().getVar(), "STM", "./memories");
	LTM_Container2Basic2<double> LTM( myConf2.getExternalConfigurationFile().getVar() ,  myConf2.getExternalConfigurationFile().getRegions(), myConf2.get_lower_bound(), myConf2.get_upper_bound(),"LTM", "./memories");
	std::cout << "Memories done!" << std::endl;

	TabuSearch TS(mycasename, myConf2, flag, TS_ObjFunc, MTM, IM, HISTORY, STM, LTM);

	double temp1 ;
	temp1=TS.search2(mycasename );
	std::cout << " HV: " << temp1 << std::endl;


	return test_report(mycasename, TS.getDatumPnt(), temp1) ;

}
