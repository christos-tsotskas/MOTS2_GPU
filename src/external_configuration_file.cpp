/*
 * external_configuration_file.cpp
 *
 *  Created on: Jan 20, 2014
 *      Author: ct01
 */


#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>

#include "external_configuration_file.h"

extern int FileExists(char const *strFilename);

unsigned int externalTopLevelConfigurationFile::get_decision_variable_size(){
	return __nVar;
}

unsigned int externalTopLevelConfigurationFile::get_objectives_size(){
	return __nObj;
}

std::string externalTopLevelConfigurationFile::get_filename(){
	return __filename;
}

void externalTopLevelConfigurationFile::list_readings(){

}

const std::string& externalTopLevelConfigurationFile::getAssessment() const {
	return __assessment;
}

unsigned int externalTopLevelConfigurationFile::getDiversify() const {
	return __diversify;
}

unsigned int externalTopLevelConfigurationFile::getEl() const {
	return __EL;
}

const std::string& externalTopLevelConfigurationFile::getFilename() const {
	return __filename;
}

unsigned int externalTopLevelConfigurationFile::getIl() const {
	return __IL;
}

unsigned int externalTopLevelConfigurationFile::getIntensify() const {
	return __intensify;
}

unsigned int externalTopLevelConfigurationFile::getLl() const {
	return __LL;
}

const std::string& externalTopLevelConfigurationFile::getLogtype() const {
	return __logtype;
}

unsigned int externalTopLevelConfigurationFile::getMaximumDuplicates() const {
	return __maximum_duplicates;
}

unsigned int externalTopLevelConfigurationFile::getMaximumImprovements() const {
	return __maximum_improvements;
}

unsigned int externalTopLevelConfigurationFile::getSample() const {
	return __n_sample;
}

unsigned int externalTopLevelConfigurationFile::getNumberOfObjectives() const {
	return __nObj;
}

unsigned int externalTopLevelConfigurationFile::getRegions() const {
	return __nRegions;
}

unsigned int externalTopLevelConfigurationFile::getVar() const {
	return __nVar;
}

unsigned int externalTopLevelConfigurationFile::getReduce() const {
	return __reduce;
}

unsigned int externalTopLevelConfigurationFile::getSaveStep() const {
	return __save_step;
}

double externalTopLevelConfigurationFile::getSs() const {
	return __SS;
}

double externalTopLevelConfigurationFile::getSsrf() const {
	return __SSRF;
}

double externalTopLevelConfigurationFile::getStartingPoint() const {
	return __starting_point;
}

unsigned int externalTopLevelConfigurationFile::getStmSize() const {
	return __STM_size;
}

externalTopLevelConfigurationFile::externalTopLevelConfigurationFile(
		std::string filename, //1
unsigned int diversify, //2
unsigned int intensify, //3
unsigned int reduce, //4
double SS, //5
double SSRF, //6
unsigned int save_step, //7
unsigned int n_sample, //8
unsigned int nVar, //9
unsigned int nObj, //10
unsigned int LL, //11, loop limit
unsigned int EL, //12, evaluations limit
unsigned int IL, //13, Improvements limit , number of consecutive improvements
unsigned int nRegions, //14
unsigned int STM_size, //15
std::string assessment, //16
std::string logtype, //17
double starting_point, //18
unsigned int maximum_improvements, //19
unsigned int maximum_duplicates): //20

		__filename(filename),
__diversify(diversify),
__intensify(intensify),
__reduce(reduce),
__SS(SS),
__SSRF(SSRF),
__save_step(save_step),
__n_sample(n_sample),
__nVar(nVar),
__nObj(nObj),
__LL(LL), //loop limit
__EL(EL), //evaluations limit
__IL(IL), //Improvements limit , number of consecutive improvements
__nRegions(nRegions),
__STM_size(STM_size),
__assessment(assessment),
__logtype(logtype),
__starting_point(starting_point),
__maximum_improvements(maximum_improvements),
__maximum_duplicates(maximum_duplicates) {

	show_configurations();
}

void externalTopLevelConfigurationFile::show_configurations() const{
	std::cout << std::endl << "Tabu Search Options" << std::endl;

	std::cout << " 1.diversify " << __diversify << std::endl;
	std::cout << " 2.intensify " <<  __intensify<< std::endl;
	std::cout << " 3.reduce " <<  __reduce<< std::endl;

	std::cout << " 4.SS " <<  __SS<< std::endl;
	if( __SS==0 )
		std::cout << "\t Start step(s) is assigned by user in start_step.txt file!" << std::endl;
	else
		std::cout << "\t Start step(s) (" << __SS <<")  is uniform for all variables" << std::endl;

	std::cout << " 5.SSRF " <<  __SSRF<< std::endl;
	std::cout << " 6.save_step " <<  __save_step<< std::endl;
	std::cout << " 7.n_sample " <<  __n_sample<< std::endl;
	std::cout << " 8.nVar " <<  __nVar<< std::endl;
	std::cout << " 9.nObj " <<  __nObj<< std::endl;
	std::cout << " 10.n_of_loops " <<  __LL<< std::endl;
	std::cout << " 11.n_of_evaluations " <<  __EL<< std::endl;
	std::cout << " 12.n_of_consecutive_improvements " <<  __IL<< std::endl;
	std::cout << " 13.assessment " <<  __assessment<< std::endl;
	std::cout << " 14.nRegions " <<  __nRegions<< std::endl;
	std::cout << " 15.STM_size " <<  __STM_size<< std::endl;
	std::cout << " 16.LogType " <<  __logtype<< std::endl;

	std::cout << " 17.Starting point " <<  __starting_point << std::endl;
	if( __starting_point==0)
		std::cout << "\t Starting point is assigned randomly!" << std::endl;
	else
		std::cout << "\t Starting point is by user from the file datum_design_vector.txt" << std::endl;

	if(-1<__starting_point && __starting_point<0){
		std::cout << "\t\tInitial sampling was activated, too!" << std::endl;
	}

	std::cout << " 18.maximum_improvements " <<  __maximum_improvements<< std::endl;
	std::cout << " 19.maximum_duplicates " <<  __maximum_duplicates<< std::endl;
}

externalTopLevelConfigurationFile::externalTopLevelConfigurationFile(char const *filename):
												__filename(filename),
												__diversify(0),
												__intensify(0),
												__reduce(0),
												__SS(0.0),
												__SSRF(0.0),
												__save_step(0),
												__n_sample(0),
												__nVar(0),
												__nObj(0),
												__LL(0),
												__EL(0),
												__IL(0), // available values HV / HB
												__nRegions(0),
												__STM_size(0),
												__assessment(""),
												__logtype(""), // available values full/quick
												__starting_point(0.0),
												__maximum_improvements(0),
												__maximum_duplicates(0)
{

	std::cout << "external configuration file ...";
	if( FileExists(filename) )
		std::cout << " OK!"<< std::endl;
	else{
		std::cout << "NOT FOUND!!!" << std::endl;
		exit(-1000);
	}

	std::ifstream configuration_file(filename);

	if(   configuration_file.is_open() ){

		std::cout << "configuration file found! " << std::endl;

		std::cout << std::endl << "Tabu Search Options" << std::endl;
		configuration_file >> __diversify; //1
		std::cout << " 1.diversify " << __diversify << std::endl;

		configuration_file >> __intensify; //2
		std::cout << " 2.intensify " <<  __intensify<< std::endl;

		configuration_file >> __reduce ; //3
		std::cout << " 3.reduce " <<  __reduce<< std::endl;

		configuration_file >> __SS ; //4
		std::cout << " 4.SS " <<  __SS<< std::endl;
		if( __SS==0 )
			std::cout << "\t Start step(s) is assigned by user in start_step.txt file!" << std::endl;
		else
			std::cout << "\t Start step(s) (" << __SS <<")  is uniform for all variables" << std::endl;

		configuration_file >> __SSRF ; //5
		std::cout << " 5.SSRF " <<  __SSRF<< std::endl;

		configuration_file >> __save_step ; //6
		std::cout << " 6.save_step " <<  __save_step<< std::endl;

		configuration_file >> __n_sample ; //7
		std::cout << " 7.n_sample " <<  __n_sample<< std::endl;

		configuration_file >> __nVar ; //8
		std::cout << " 8.nVar " <<  __nVar<< std::endl;

		configuration_file >> __nObj ; //9
		std::cout << " 9.nObj " <<  __nObj<< std::endl;

		configuration_file >> __LL ; //10
		std::cout << " 10.n_of_loops " <<  __LL<< std::endl;

		configuration_file >> __EL ; //11
		std::cout << " 11.n_of_evaluations " <<  __EL<< std::endl;

		configuration_file >> __IL ; //12  : available values HV / HB
		std::cout << " 12.n_of_consecutive_improvements " <<  __IL<< std::endl;

		configuration_file >> __assessment ; //13
		std::cout << " 13.assessment " <<  __assessment<< std::endl;

		configuration_file >> __nRegions ; //14
		std::cout << " 14.nRegions " <<  __nRegions<< std::endl;

		configuration_file >> __STM_size ; //15
		std::cout << " 15.STM_size " <<  __STM_size<< std::endl;

		configuration_file >> __logtype ; //16 -  available values full/quick
		std::cout << " 16.LogType " <<  __logtype<< std::endl;

		configuration_file >> __starting_point ; //17
		std::cout << " 17.Starting point " <<  __starting_point << std::endl;
		if( __starting_point==0)
			std::cout << "\t Starting point is assigned randomly!" << std::endl;
		else
			std::cout << "\t Starting point is by user from the file datum_design_vector.txt" << std::endl;
		if(-1<__starting_point && __starting_point<0){
			std::cout << "\t\tInitial sampling was activated, too!" << std::endl;
		}

		configuration_file >> __maximum_improvements;
		std::cout << " 18.maximum_improvements " <<  __maximum_improvements<< std::endl;

		configuration_file >> __maximum_duplicates;
		std::cout << " 19.maximum_duplicates " <<  __maximum_duplicates<< std::endl;


		configuration_file.close();
		std::cout << "configuration file read! " << std::endl;
	}else{
		std::cout << "NO configuration file found!" <<std::endl;
	}
};

