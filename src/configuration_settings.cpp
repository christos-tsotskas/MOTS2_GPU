/*
 * configuration_settings.cpp
 *
 *  Created on: Jan 19, 2014
 *      Author: ctsotskas
 */

#include <iostream>
#include <vector>
#include <fstream>

#include "configuration_settings.h"

extern int FileExists(char const *strFilename);

const std::string& ConfigurationSettings::getCaseName() const {
	return __case_name;
}

ConfigurationSettings::~ConfigurationSettings(){
	std::cout << "end of configuration settings" << std::endl;
}

ObjFunction2  ConfigurationSettings::read_vector_from_file(char const *filename, char const *file_description, const int elements,const int necessary){
	ObjFunction2 ObjPnt(elements,1111.0);
	std::cout << std::endl << file_description << " file (" <<filename <<")...";
	std::ifstream FILE(filename);
	if( FileExists(filename) ){
		std::cout << " OK!"<< std::endl;

		if(FILE.is_open()){

			double buff=0.0;
			std::vector<double> buff_vec;

			while(FILE>>buff)
				buff_vec.push_back(buff);

			if(buff_vec.size()==ObjPnt.size()){
				for (unsigned int i=0; i<ObjPnt.size() ; ++i)
					ObjPnt[i]=buff_vec[i];
			}else
				ObjPnt= ObjFunction2(elements,buff_vec[0]);

			FILE.close();
			std::cout << "\t"<< file_description<< " read: " << ObjPnt << std::endl;
			return ObjPnt;
		}else{
			std::cout << " The file " << filename <<"  was not found in root directory" << std::endl;
		}
	}
	else{
		std::cout << "NOT FOUND!!! (looking for "<< filename << " within the current directory)"<< std::endl;
		if( necessary==1 ){
			std::cout << "(this file is mandatory)" << std::endl;
			exit(-1002);
		}
	}
}



ConfigurationSettings::ConfigurationSettings(std::string case_name,
		external_configuration_file &ExternalConfigurationFile,
		std::string ReferencePointFilename,
		std::string PenaltyPointFilename,
		std::string LowerBoundFilename,
		std::string UpperBoundFilename,
		std::string StartingPointFilename,
		std::string CurrentStepFilename):
		__case_name(case_name),
								__ExternalConfigurationFile(ExternalConfigurationFile),
								reference_point( ExternalConfigurationFile.get_objectives_size(),11111.1),
								penalty_point( ExternalConfigurationFile.get_objectives_size(),100000.0),
								lower_bound( ExternalConfigurationFile.get_decision_variable_size(),0.0),
								upper_bound( ExternalConfigurationFile.get_decision_variable_size(),2222222.2),
								starting_point( ExternalConfigurationFile.get_decision_variable_size(),0.0),
								current_step( ExternalConfigurationFile.get_decision_variable_size(),ExternalConfigurationFile.getSs())
{
	srand((unsigned)(time(NULL)));
	std::cout << "Improved Configuration Settings" << std::endl;
	std::cout << "external configuration file read:" << ExternalConfigurationFile.get_filename() << std::endl;


	//read_design_vector_file( starting_point, "datum_design_vector.txt", "datum design", datumPnt, TS_ObjFunc.min_bound, TS_ObjFunc.max_bound);
	reference_point=read_vector_from_file(ReferencePointFilename.c_str(), "reference point", ExternalConfigurationFile.getObj(), 1);
	penalty_point=read_vector_from_file(PenaltyPointFilename.c_str(), "penalty point", ExternalConfigurationFile.getObj(), 1);
	lower_bound=read_vector_from_file(LowerBoundFilename.c_str(), "lower bound", ExternalConfigurationFile.getVar(), 1);
	upper_bound=read_vector_from_file(UpperBoundFilename.c_str(), "upper bound", ExternalConfigurationFile.getVar(), 1);


	if(ExternalConfigurationFile.getStartingPoint()==0){
		for (unsigned int i=0; i < starting_point.size() ; ++i)
			starting_point[i]=RandomNumber2(lower_bound[i], upper_bound[i]); //RANDOM value for each variable
	}else
		starting_point=read_vector_from_file(StartingPointFilename.c_str(), "starting point", ExternalConfigurationFile.getVar() ,1);

	if(std::abs(ExternalConfigurationFile.getSs())<=1e-6)
		current_step=read_vector_from_file(CurrentStepFilename.c_str(), "current step", ExternalConfigurationFile.getVar() , (std::abs(ExternalConfigurationFile.getSs())<=1e-6) );


	//todo create all the required directories
	//todo read and store any checkpoint file (should it exists)
	// char const *checkpoint_path="./monitor_data/chekpoint.out";

	ExternalConfigurationFile.list_readings();
}

const ObjFunction2& ConfigurationSettings::get_reference_point() const{
	return reference_point;
}

const ObjFunction2& ConfigurationSettings::get_penalty_point() const{
	return penalty_point;
}

const Point2& ConfigurationSettings::get_lower_bound() const{
	return lower_bound;
}

const Point2& ConfigurationSettings::get_upper_bound() const{
	return upper_bound;
}

const Point2& ConfigurationSettings::get_starting_point() const{
	return starting_point;
}

const external_configuration_file& ConfigurationSettings::getExternalConfigurationFile() const {
	return __ExternalConfigurationFile;
}

Point2 ConfigurationSettings::generateRandomPointWithinBounds() const{


	Point2 newRandomPoint(lower_bound.size(),0.0);
	double r;

	for(unsigned int i=0; i<lower_bound.size(); ++i){
		r = (double) rand() /  ( RAND_MAX );
		newRandomPoint[i]= (lower_bound[i] + r * (upper_bound[i] - lower_bound[i]));
	}
	return newRandomPoint;
}

const Point2& ConfigurationSettings::get_current_step() const{
	return current_step;
}
