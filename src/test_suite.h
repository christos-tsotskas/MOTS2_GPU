/*
 * test_suite.h
 *
 *  Created on: Jan 18, 2014
 *      Author: ctsotskas
 */

#ifndef TEST_SUITE_H_
#define TEST_SUITE_H_

#include <sstream>

#include "global_defines.h"


class test_report{
	std::string __case_name;
	Point2 __initial_point;
	double __HV;

public:
	test_report ( std::string case_name,
			Point2  initial_point,
			double HV ):
				__case_name( case_name),
				__initial_point(initial_point),
				__HV(HV){}

	std::string show(){
		std::ostringstream string_line_report;
		string_line_report << __case_name  << "\tHV: " << __HV << "\t"<< __initial_point;
		return string_line_report.str();
	}
};


class test_suite{
	void test_zdt1();

public:
	test_suite();
	void test_all();



};


test_report tabu_test_ZDT1(std::string mycasename, int flag);
test_report tabu_test_ZDT2(std::string mycasename, externalTopLevelConfigurationFile this_ext_conf_file, int flag, int nVar);
test_report tabu_test_ZDT3(std::string mycasename, int flag);
test_report tabu_test_ZDT4(std::string mycasename, int flag);
test_report tabu_test_ZDT6(std::string mycasename, int flag);

test_report tabu_test_ZDT2_many_nvar(std::string mycasename, int flag);
#endif /* TEST_SUITE_H_ */
