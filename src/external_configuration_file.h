/*
 * external_configuration_file.h
 *
 *  Created on: Jan 20, 2014
 *      Author: ct01
 */

#ifndef EXTERNAL_CONFIGURATION_FILE_H_
#define EXTERNAL_CONFIGURATION_FILE_H_

#include <cstring>
#include <cmath>

/**
 * Contains the external settings, as read on the configuration file prescribed
 */

class externalTopLevelConfigurationFile{
	std::string __filename; //1
	unsigned int __diversify; //2
	unsigned int __intensify; //3
	unsigned int __reduce; //4
	double __SS; //5
	double __SSRF; //6
	unsigned int __save_step; //7
	unsigned int __n_sample; //8
	unsigned int __nVar; //9
	unsigned int __nObj; //10
	unsigned int __LL; //loop limit
	unsigned int __EL; //evaluations limit
	unsigned int __IL; //Improvements limit , number of consecutive improvements
	unsigned int __nRegions; //14
	unsigned int __STM_size; //15
	std::string __assessment; //16
	std::string __logtype; //17
	double __starting_point; //18
	unsigned int __maximum_improvements; //19
	unsigned int __maximum_duplicates; //20

public:
	externalTopLevelConfigurationFile(
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
	unsigned int maximum_duplicates); //20

	void show_configurations() const;
	externalTopLevelConfigurationFile(char const *name);
	unsigned int get_decision_variable_size();
	unsigned int get_objectives_size();

	std::string get_filename();
	void list_readings();

	const std::string& getAssessment() const ;

	unsigned int getDiversify() const ;

	unsigned int getEl() const ;

	const std::string& getFilename() const ;

	unsigned int getIl() const ;

	unsigned int getIntensify() const ;

	unsigned int getLl() const ;

	const std::string& getLogtype() const ;

	unsigned int getMaximumDuplicates() const ;

	unsigned int getMaximumImprovements() const ;

	unsigned int getSample() const ;

	unsigned int getNumberOfObjectives() const ;

	unsigned int getRegions() const ;

	unsigned int getVar() const ;

	unsigned int getReduce() const ;

	unsigned int getSaveStep() const ;

	double getSs() const ;

	double getSsrf() const ;

	double getStartingPoint() const ;

	unsigned int getStmSize() const;

	void updateRelatedParameters(){
		__n_sample = (int) std::ceil( __nVar / 5);
		__STM_size = (int) std::ceil( __nVar * 2/3);
	}

	void setVar(unsigned int var) {
		__nVar = var;
		updateRelatedParameters();
	}
};

#endif /* EXTERNAL_CONFIGURATION_FILE_H_ */
