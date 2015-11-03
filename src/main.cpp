//============================================================================
// Name        : mots_3.cpp
// Author      : Christos Tsotskas
// Version     :
// Copyright   : L-GPL
// Description : MOTS in C++, Ansi-style
//============================================================================


//library that contains the tabu search function
//#include <cstring>
#include <sstream>
#include <fstream>

#include "mots.h"
#include "test_suite.h"
#include "objective_function_formulae.h"
#include "objective_function.h"
#include "verification_code.h"

#include <fstream>

#include <vector>
//#include <io.h>   // For access().
//#include <sys/types.h>  // For stat().
//#include <sys/stat.h>   // For stat().





void dummy_test(objective_function_formulae& dummy_objective_function_formulae){
	std::cout << "dummy case" << std::endl;
	Point2 d1=simple_read_vector_from_file("dummy_ZDT6.txt");
	std::cout << "objectives: "<<	dummy_objective_function_formulae(d1) << std::endl;
}

void test_basepoint(){
	std::cout << "start basepoint test!" << std::endl;
	Point2 p1(3, 1.1); p1[2]=2.9; p1[1]=2.9;
	Point2 p2(3, 2.2); p2[2]=1.9; p2[1]=11.9;
	Point2 p3(3, 3.3); p3[2]=5.9; p3[1]=1.9;
	Point2 p4(3, 4.4); p4[2]=2.9; p4[1]=4.9;

	basepoint_entry b1(p1,"arxiko");
	basepoint_entry b2(p2,"deutero");
	basepoint_entry b3(p3,"trito");
	basepoint_entry b4(p4,"tetarto");

	basepoint_memory BM;

	BM.push_back(b1);
	BM.push_back(b2);
	BM.push_back(b3);
	BM.push_back(b4);

	BM.show_memory();

	std::cout << "current"<<  BM.get_current_base_point()  << std::endl;

	std::cout << "previous" << BM.get_previous_base_point()  << std::endl;



	std::cout << "basepoint tested!" << std::endl;
}

int main(int argc, char *argv[]){
	std::cout << "MOTS2 main" << std::endl;

	int flag=0;
	for(int f=0; f<argc; ++f){
		std::cout << f << ". " << argv[f] << std::endl;
		if(!strcmp(argv[f],"-r")  )
			flag=1;
	}

	//	objective_function_formulae ZDT1_test(2);
	//	dummy_test(ZDT1_test);



	std::ostringstream mycasename;
	test_report tr("t1", Point2(1,1), 1.0);
	std::ofstream test_report_file("all_tests.txt");


	if(test_report_file.is_open()){
		for(int i=7;i<11; ++i){

			//perform optimisation search

			mycasename << "case_" << i ;

			//		test_report tr (tabu_test_ZDT1(mycasename.str(), flag));
			//		test_report tr (tabu_test_ZDT2(mycasename.str(), flag, i*30 ));
			//		test_report tr (tabu_test_ZDT3(mycasename.str(), flag));
			//		test_report tr (tabu_test_ZDT4(mycasename.str(), flag));
			//		test_report tr (tabu_test_ZDT6(mycasename.str(), flag));

			tr=tabu_test_ZDT2(mycasename.str(), flag, i*i*30 );

			std::cout << "loop " << i << ": " << tr.show()  << std::endl;
			test_report_file << "loop " << i << ": " << tr.show()  << std::endl;


			mycasename.clear();
			mycasename.str(std::string());

			std::cin >> flag;
		}
		test_report_file.close();
	}else
		std::cout << "cannot open file!" << std::endl;

	std::cout << "optimiser tested!" << std::endl;

	return 0;
}

/**
 * For ease of implementation, it is useful to link together all the configuration
settings that are related to the size of variables used. So, all of them should be
a function of the number of decision variables.
 */



