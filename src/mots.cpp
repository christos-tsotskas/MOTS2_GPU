//============================================================================
// Name        : mots_3.cpp
// Author      : Christos Tsotskas
// Version     :
// Copyright   : L-GPL
// Description : MOTS in C++, Ansi-style
//============================================================================
/* This file is part of MOTS_2.

MOTS_2 is free software: you can redistribute it and/or modify
it under the terms of the  GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

MOTS_2 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with MOTS_2.  If not, see <http://www.gnu.org/licenses/>.

Copyright 2011 Christos Tsotskas
 */

#include "mots.h"
#include "pca2.h"
#include "latin_hypercube.h"
#include "global_defines.h"
#include "container.h"
#include "temp_container.h"

void show_pwd(){
	std::cout << "working directory (where all the accompanying files should be put)" << std::endl;
#if defined(__gnu_linux__)   || defined(__linux__)
	system("pwd");
#endif //linux_specific defines

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32) || defined(WINDOWS) && !defined(__CYGWIN__)
	system("echo %CD%");
#endif //windows_specific defines
}

void TabuSearch::Push_Base_Point(const Point2 &P,  std::string const entry_function){
#ifdef DEBUG_PUSH_BASE
	cout << " within Push_Base_Point" << endl << "\t" << P << endl;
#endif //DEBUG_PUSH_BASE
	numerical_check(P, entry_function);
	//	Points.push_front(P);
	//	iIter=Points.begin(); //point to the current top of the list
	//	BP_tracer.push_back( BP_tuple(P, entry_function ));
	basepoint_entry temp(P,entry_function);
	__BasepointMemory.push_back(temp);
	MTM.removeDominatedPoints();
}

template<typename T>
void TabuSearch::readInputFiles(char const *filepath, std::vector<T> &target_vector ){
	cout << endl <<  filepath << " file ...";

	unsigned int counter=0;
	if( FileExists(filepath) ){
		cout << " OK!"<< endl;

		//target_vector=vector<T>(target_vector.size(),0.0);

		ifstream RP(filepath);
		if(RP.is_open()){
			for (unsigned int i=0; i< target_vector.size(); ++i){
				RP>>target_vector[i];
				++counter;
			}
			if(counter != target_vector.size()){
				cout << "The file " << filepath << "has insufficient elements (" << counter <<" no equal to" << target_vector.size() << ")" << endl;
				exit (-1313);
			}

			RP.close();
			cout << "\t" << filepath << " read " << target_vector << endl;
		}else{
			cout <<"no " << filepath << " file was found" << endl;

		}
	}else{
		cout << "NOT FOUND!!!" << endl;
		exit(-1002);
	}
}


// TODO: to be improved ( 1 sunarthsh pou 8a kanei evaluate points) anti gia to following set entolwn

void TabuSearch::read_design_vector_file(double starting_point_setting, char const *filename, char const *file_description, Point2 &Pnt, const Point2 &lower_bound, const Point2 &upper_bound){
	if(starting_point_setting==0){
		for (unsigned int i=0; i < Pnt.size() ; ++i)
			Pnt[i]=RandomNumber2(lower_bound[i], upper_bound[i]); //RANDOM value for each variable
	}else{
		cout << endl << file_description << " file ...";
		if( FileExists(filename ) ){
			cout << " OK!"<< endl;
			ifstream file_buffer(filename);
			if(file_buffer.is_open()){
				for(unsigned int i=0 ; i<Pnt.size(); ++i)
					file_buffer >> Pnt[i];
				file_buffer.close();
			}else{
				cout << " The file " << filename <<"  was not found in root directory" << endl;
				exit(-200);
			}

			cout << "\tdatum design read: " << Pnt << endl;
		}else{
			cout << "WARNING, NOT FOUND!!!" << endl;
			if(starting_point_setting==1){
				cout << " either create a new "<< filename << "  file or alter the proper value in the configuration file!"<< endl;
				exit(-1003);
			}
		}
	}
}

void TabuSearch::read_objectives_file(char const *filename, char const *file_description, ObjFunction2 &ObjPnt){
	cout << endl << file_description << " file ...";
	if( FileExists(filename) ){
		cout << " OK!"<< endl;


		ifstream FILE("reference_point.txt");
		if(FILE.is_open()){
			for (unsigned int i=0; i< nObj; ++i)
				FILE>>ObjPnt[i];

			FILE.close();
			cout << "\t"<< file_description<< " read: " << ObjPnt << endl;
		}else{
			cout << " The file " << filename <<"  was not found in root directory" << endl;
		}
	}
	else{
		cout << "NOT FOUND!!!" << endl;
		exit(-1002);
	}
}

void TabuSearch::create_plot_scripts(){
	//3 (template)scripts will be created in the root directory
	Point2 temp(2,0.0);


	//1.plot: this plots the trade-off

	ofstream plot_trade_off_file( ("plot_"+__case_name+"_trade_off.plt").c_str() );
	plot_trade_off_file<<" reset" <<endl;

	plot_trade_off_file<<"#this script plots the monitor data from MOTS2" <<endl;

	plot_trade_off_file<<"#title & labels" <<endl;
	plot_trade_off_file<<"set title \"Trade-off\"" <<endl;
	plot_trade_off_file<<"set xlabel \"Pressure Drop\"" <<endl;
	plot_trade_off_file<<"set ylabel \"Vorticity\"" <<endl;
	plot_trade_off_file<<"set grid" <<endl;
	//todo get the min anx max range for each objective and insert to the following lines
	plot_trade_off_file<<"#set xrange [20:500]" <<endl;
	plot_trade_off_file<<"#set yrange [0:14 ]" <<endl;

	plot_trade_off_file<<"# Line styles" <<endl;
	plot_trade_off_file<<"set border linewidth 1.5" <<endl;

	plot_trade_off_file<<"set style line 1 linecolor rgb '#0000ff' linetype 1 linewidth 3  pointtype -1 pointsize default # blue" <<endl;
	plot_trade_off_file<<"set style line 2 linecolor rgb '#ff0000' linetype 2 linewidth 3  pointtype 0 pointsize default # red" <<endl;
	plot_trade_off_file<<"set style line 3 linecolor rgb '#00ff00' linetype 8 linewidth 3  pointtype -1 pointsize default # green" <<endl;
	plot_trade_off_file<<"set style line 4 linecolor rgb '#ffff00' linetype 8 linewidth 2  pointtype 2 pointsize default # yellow" <<endl;
	plot_trade_off_file<<"set style line 5 linecolor rgb '#ff0000' linetype 1 linewidth 2  pointtype 1 pointsize default # red" <<endl;


	plot_trade_off_file<<"#Legend" <<endl;
	plot_trade_off_file<<"set key under" <<endl;


	plot_trade_off_file<<"#MOTS2-specific configuration settings" <<endl;


	plot_trade_off_file<<"plot 'memories/HISTORY.txt' u 2:1 title 'History', 'memories/MTM.txt' u 2:1 title 'Pareto Front'" <<endl;

	plot_trade_off_file<<"#selecting designs:" << endl;
	plot_trade_off_file<<"replot \"<echo '-1 1'\" title 'Datum design' pointtype 6 pointsize 3 linecolor rgb '#ff0000'" <<endl;
	plot_trade_off_file<<"#replot \"<echo '-1.7103 1.09332'\" title 'Compromise design' pointtype 4 pointsize 3 linecolor rgb '#00ff00'" <<endl;
	plot_trade_off_file<<"#replot \"<echo '-3.88 2.09'\" title 'Maximum Vorticity' pointtype 14 pointsize 3 linecolor rgb '#0000ff'		#extreme 1" <<endl;
	plot_trade_off_file<<"#replot \"<echo '-0.0687 0.083426'\" title 'Minimum Pressure Drop' pointtype 12 pointsize 3  linecolor rgb '#ff00ff' #extreme 2" <<endl;


	plot_trade_off_file<<"#EPS" <<endl;
	plot_trade_off_file<<"set terminal postscript eps size 3.5,2.62 enhanced color \\" << endl;
	plot_trade_off_file<<"   font 'Helvetica,14' linewidth 2" <<endl;
	plot_trade_off_file<<"set output 'MOTS2_trade_off.eps'" <<endl;
	plot_trade_off_file<<"replot" <<endl;
	plot_trade_off_file<<"set term pop" <<endl;
	plot_trade_off_file<<"set output " <<endl;

	plot_trade_off_file.close();


	//2.plot: this is the plot for monitoring the progress of the optimiser
	ofstream plot_monitor_data_file( ("plot_"+__case_name+"_monitor_data.plt").c_str() );
	plot_monitor_data_file<<"reset"<<endl;

	plot_monitor_data_file<<"#this script plots the monitor data from MOTS2"<<endl;

	plot_monitor_data_file<<"#title & labels "<<endl;
	plot_monitor_data_file<<"set title \"Monitor Data\" "<<endl;
	plot_monitor_data_file<<"set xlabel \"Optimisation step\" "<<endl;
	plot_monitor_data_file<<"set ylabel \"Number of unsuccessful iterations\" "<<endl;
	plot_monitor_data_file<<"set grid"<<endl;
	plot_monitor_data_file<<"set xrange [0:"<< calculate_x_range_for_monitor_plots()  <<"]"<<endl;
	plot_monitor_data_file<<"set yrange [0:"<< calculate_y_range_for_monitor_plots()  <<"]"<<endl;

	plot_monitor_data_file<<"# Line styles"<<endl;
	plot_monitor_data_file<<"set border linewidth 1.5"<<endl;

	plot_monitor_data_file<<"set style line 1 linecolor rgb '#0000ff' linetype 1 linewidth 3  pointtype -1 pointsize default # blue "<<endl;
	plot_monitor_data_file<<"set style line 2 linecolor rgb '#ff0000' linetype 2 linewidth 3  pointtype 0 pointsize default # red "<<endl;
	plot_monitor_data_file<<"set style line 3 linecolor rgb '#00ff00' linetype 8 linewidth 3  pointtype -1 pointsize default # green "<<endl;
	plot_monitor_data_file<<"set style line 4 linecolor rgb '#ffff00' linetype 8 linewidth 2  pointtype 2 pointsize default # yellow "<<endl;
	plot_monitor_data_file<<"set style line 5 linecolor rgb '#ff0000' linetype 1 linewidth 2  pointtype 1 pointsize default # red "<<endl;


	plot_monitor_data_file<<"#Legend"<<endl;
	plot_monitor_data_file<<"set key under"<<endl;


	plot_monitor_data_file<<"#MOTS2-specific configuration settings"<<endl;
	plot_monitor_data_file<<"diversify_threshold(x)=" <<diversify<<endl;
	plot_monitor_data_file<<"intensify_threshold(x)="<<intensify<<endl;
	plot_monitor_data_file<<"reduction_threshold(x)="<<reduce<<endl;

	plot_monitor_data_file<<"plot 'monitor_data/"<<__case_name <<"_i_local.out' with linespoints ls 5 title 'i-local' , \\"<<endl;
	plot_monitor_data_file<<"	'monitor_data/"<<__case_name <<"_im_size.out' with linespoints ls 4 title 'IM size' "<<endl;


	plot_monitor_data_file<<"replot	diversify_threshold(x) with linespoints ls 1 title 'Diversification' "<<endl;
	plot_monitor_data_file<<"replot	intensify_threshold(x) with linespoints ls 2 title 'Intensification' "<<endl;
	plot_monitor_data_file<<"replot	reduction_threshold(x) with linespoints ls 3 title 'Step size reduction' "<<endl;




	plot_monitor_data_file<<"#EPS "<<endl;
	plot_monitor_data_file<<"set terminal postscript eps size 3.5,2.62 enhanced color \\"<<endl;
	plot_monitor_data_file<<"    font 'Helvetica,14' linewidth 2 "<<endl;
	plot_monitor_data_file<<"set output 'MOTS2_monitor_data.eps'"<<endl;
	plot_monitor_data_file<<"replot "<<endl;
	plot_monitor_data_file<<"set term pop" <<endl;
	plot_monitor_data_file<<"set output " <<endl;
	plot_monitor_data_file.close();


	//3.plot: this is the plot for monitoring the invocation of moves available to the optimiser

	ofstream plot_monitor_moves_file( ("plot_"+__case_name+"_monitor_moves.plt").c_str() );
	plot_monitor_moves_file<<"reset"<<endl;


	plot_monitor_moves_file<<"#this script plots the moves from MOTS2"<<endl;

	plot_monitor_moves_file<<"#title & labels"<<endl;
	plot_monitor_moves_file<<"set title \"Monitor Moves\" "<<endl;
	plot_monitor_moves_file<<"set xlabel \"Optimisation step\" "<<endl;
	plot_monitor_moves_file<<"set ylabel \"Number of invocations\" "<<endl;
	plot_monitor_moves_file<<"set grid"<<endl;
	plot_monitor_moves_file<<"set xrange [0:"<< calculate_x_range_for_monitor_plots()  <<"]"<<endl;
	plot_monitor_moves_file<<"set yrange [0:"<< calculate_y_range_for_monitor_plots()  <<"]"<<endl;

	plot_monitor_moves_file<<"# Line styles"<<endl;
	plot_monitor_moves_file<<"set border linewidth 1.5"<<endl;

	plot_monitor_moves_file<<"set style line 1 linecolor rgb '#0000ff' linetype 1 linewidth 3  pointtype -1 pointsize default # blue "<<endl;
	plot_monitor_moves_file<<"set style line 2 linecolor rgb '#ff0000' linetype 2 linewidth 3  pointtype 0 pointsize default # red "<<endl;
	plot_monitor_moves_file<<"set style line 3 linecolor rgb '#00ff00' linetype 8 linewidth 3  pointtype -1 pointsize default # green "<<endl;
	plot_monitor_moves_file<<"set style line 4 linecolor rgb '#ffff00' linetype 8 linewidth 2  pointtype 2 pointsize default # yellow "<<endl;
	plot_monitor_moves_file<<"set style line 5 linecolor rgb '#ff0000' linetype 1 linewidth 2  pointtype 1 pointsize default # red "<<endl;


	plot_monitor_moves_file<<"#Legend"<<endl;
	plot_monitor_moves_file<<"set key under"<<endl;


	plot_monitor_moves_file<<" plot 'monitor_data/intensify.out' with linespoints ls 5 title 'Intesification' , \\"<<endl;
	plot_monitor_moves_file<<" 'monitor_data/diversify.out' with linespoints ls 4 title 'Diversification', \\"<<endl;
	plot_monitor_moves_file<<" 'monitor_data/reduce.out' title 'Step size reduction' "<<endl;


	plot_monitor_moves_file<<" #EPS"<<endl;
	plot_monitor_moves_file<<"#set terminal postscript eps size 3.5,2.62 enhanced color \\"<<endl;
	plot_monitor_moves_file<<"#    font 'Helvetica,14' linewidth 2"<<endl;
	plot_monitor_moves_file<<"#set output 'MOTS2_monitor_moves.eps' "<<endl;
	plot_monitor_moves_file<<"replot"<<endl;
	plot_monitor_moves_file<<"set term pop" <<endl;
	plot_monitor_moves_file<<"set output " <<endl;
	plot_monitor_moves_file.close();
}

Point2 TabuSearch::sample_and_assess(int discovery_evaluations){

	//		evaluation_percentage=starting_point*(-1);
	//		discovery_evaluations=(int)ceil(evaluation_percentage*EL);

	int n=discovery_evaluations,m=nVar;
	int seed=get_seed ( );
	double *lhs_data=new double[m*n]; //data for LHS
	Point2 importance_vector(__DefaultConfigurationSettings.get_starting_point());
	Point2 temp_design(__DefaultConfigurationSettings.get_starting_point());
	Point2 temp_objectives(__DefaultConfigurationSettings.get_penalty_point());
	latin_random ( m, n, &seed, lhs_data, __DefaultConfigurationSettings.get_lower_bound(), __DefaultConfigurationSettings.get_upper_bound() );
	//olh th diergasia EDW!

	cout << "initial sampling will sample " << n << " points, of dimensionality " << m << endl;
	for(int i=0; i<n; ++i){
		for(int j=0; j<m; ++j){
			temp_design[j]=lhs_data[i*m+j];
		}
		if(i%100==0)
			cout << "sampling progress: " << i/n*100 << "%" <<endl;
		temp_objectives=evaluate_point(temp_design);
		MTM.addIfNotDominated( temp_design, temp_objectives );
		HISTORY[temp_design]=temp_objectives;
	}
	MTM.removeDominatedPoints();
	//		save_full_map("final_solutions.txt", MTM);
	MTM.save_container_snapshot("sampled_solutions_",n);

	cout << "Initial sampling report" << endl;
	cout << "\tsampled: " << n << " designs." << endl;
	cout << "\tsuggestions:" << endl;
	temp_design=MTM.find_extreme(1);
	cout << "\tA:"<< temp_design << " with " << MTM[temp_design] << endl;
	temp_design=MTM.find_extreme(0);
	cout << "\tB:" << temp_design << " with " << MTM[temp_design] << endl;

	datumPnt=temp_design;
	datumPnt=MTM.selectRandom();
	Push_Base_Point(datumPnt, "initial_sampling" );


	vector< Point2 > data3(MTM.size(), Point2(m,-1) );
	//prepare LHS data for PCA
	//todo to documentation, the PCA-covariance matrix is used because the data are in similar scale, by theory/definition
	cout << "copy to data" << endl;


	int i=0;
	for(map<Point2,ObjFunction2>::iterator it=MTM.begin(); it!=MTM.end(); ++it ){
		for(int j=0; j<m ;++j)
			data3[i][j]=it->first[j];
		++i;
	}

	PCA(data3,MTM.size(), m, importance_vector);

	cout << "\tImportance Report:" << endl;

	cout << "\t" <<  importance_vector << endl ;

	double max=0.0, min=1000000.0;
	int max_position=0;
	int min_position=0;

	//find max
	for(int i=0; i<m; ++i){
		if(importance_vector[i]>max){
			max=importance_vector[i];
			max_position=i;
		}
		if(importance_vector[i]<min){
			min=importance_vector[i];
			min_position=i;
		}
	}

	Point2 normalised_importance(__DefaultConfigurationSettings.get_starting_point());
	cout << "most important single decision variable:" << max << " at position " << max_position << endl;
	cout << "\t(note: there may be more than one decision variables with the same importance)" << endl;

	cout << "\tnormalising importance vector with its max element" << endl;
	for(int i=0; i<m; ++i)
		normalised_importance[i]=importance_vector[i]/max;
	cout << normalised_importance << " ";
	cout << endl;

	cout << "\treciprocal of the previous" << endl;
	for(int i=0; i<m; ++i)
		cout << max/importance_vector[i] << " ";
	cout << endl;

	Point2 SuggestedStep(nVar,1.0);
	cout << "\tsuggested step:" << endl;
	double temp_buffer;
	for(int i=0; i<m; ++i){
		temp_buffer=InitialStep[i]*(max/importance_vector[i]);
		if( temp_buffer<1.0)
			SuggestedStep[i]=temp_buffer;
	}
	cout << SuggestedStep << endl;
	//todo at documentation: when the sample_and_assess option is selected, the SS is overiden!
	delete [] lhs_data;
	return normalised_importance;
}

TabuSearch::TabuSearch(
		std::string case_name,
		ConfigurationSettings &conf_set,  int restart_flag_argument,
		ObjectiveFunction &ObjectiveFunctionModule,
		Container2 &MediumTermMemory,
		Container2 &IntensificationMemory,
		Container2 &HistoryMemory,
		STM_Container2 &ShortTermMemory,
		LTM_Container2Basic2<double> &LongTermMemory
): //0 new search, 1 restart from checkpoint memories
			__case_name(case_name),
			__DefaultConfigurationSettings(conf_set),
			restart_flag(restart_flag_argument),
			diversify(conf_set.getExternalConfigurationFile().getDiversify()),//1
			intensify(conf_set.getExternalConfigurationFile().getIntensify()),//2
			reduce(conf_set.getExternalConfigurationFile().getReduce()),//3
			SS(conf_set.getExternalConfigurationFile().getSs()),//4
			SSRF(conf_set.getExternalConfigurationFile().getSsrf()),//5
			save_step(conf_set.getExternalConfigurationFile().getSaveStep()),//6
			n_sample(conf_set.getExternalConfigurationFile().getSample()),//7
			nVar(conf_set.getExternalConfigurationFile().getVar()),//8
			nObj(conf_set.getExternalConfigurationFile().getObj()),//9
			LL(conf_set.getExternalConfigurationFile().getLl()),//10
			EL(conf_set.getExternalConfigurationFile().getEl()),//11
			IL(conf_set.getExternalConfigurationFile().getIl()),//12
			nRegions(conf_set.getExternalConfigurationFile().getRegions()),//14
			STM_size(conf_set.getExternalConfigurationFile().getStmSize()),//15
			assessment(conf_set.getExternalConfigurationFile().getAssessment()),//13
			logtype(conf_set.getExternalConfigurationFile().getLogtype()),//16
			starting_point(conf_set.getExternalConfigurationFile().getStartingPoint()),//17
			maximum_improvements(conf_set.getExternalConfigurationFile().getMaximumImprovements()),//18
			maximum_duplicates(conf_set.getExternalConfigurationFile().getMaximumDuplicates()),//19

			TS_ObjFunc(ObjectiveFunctionModule),

			datumPnt(conf_set.get_starting_point()),
			InitialStep(conf_set.get_current_step()), // vector opou apo8hkeyetai to arxiko step
			CurrentStep(conf_set.get_current_step()),
			reference_point(conf_set.get_reference_point()),

			MTM(MediumTermMemory),
			IM(IntensificationMemory),
			HISTORY(HistoryMemory),
			STM(ShortTermMemory),

			//						Points( deque<Point2>()),
			//						iIter(Points.begin()), //initialisation
			__BasepointMemory(),

			LTM ( LongTermMemory),
			//	progressive_HISTORY("prog_HISTORY", nVar, nObj, 2), //first item is the iteration_number and second is the order of evaluation
			//	progressive_MTM("prog_HISTORY", nVar, nObj, 2),
			//the values are set to default, as if the optimisation starts from scratch
			iLocal(0),
			cur_loops(1),
			previous_cur_loops(0),
			previous_evaluations(0),
			nextMoveHookeJeeves(1), //initial value
			diversification(0),
			intensification(0),
			reduction(0),
			BP_tracer(deque<  BP_tuple >())
{
	cout << "MOTS2, provided by Christos Tsotskas (c.tsotskas@gmail.com), started " << __DATE__ << " at " << __TIME__ << endl;
	show_pwd();


#ifdef _MSC_VER
	cout <<"(VISUAL STUDIO detected, the vld is automatically added in the DEBUG)" << endl;
#endif

	cout << endl << "Initialising TS" << endl;

	srand((unsigned)(time(0)+2182));//this is needed only for random initialisation

	create_plot_scripts();
	ObjFunction2 currentObjFunc(conf_set.get_penalty_point());

	cout << endl << endl << "Initial Point is "  << endl << datumPnt << endl;
	if( TS_ObjFunc.isValid(datumPnt )==0 ){
		std::cout << "invalid point form the beginning, please start of with a valid point" << std::endl;
		exit(-9999999);
	}


	currentObjFunc = TS_ObjFunc.calculateObjFun(datumPnt); // respective obj function
	MTM.insert(entry( datumPnt , currentObjFunc ));
	IM.insert(entry( datumPnt , currentObjFunc ));
	HISTORY.insert(entry( datumPnt , currentObjFunc ) );
	// TODO: to be improved ( 1 sunarthsh pou 8a kanei evaluate points) anti gia to following set entolwn
	cout << "initial decision variables point & objectives:" <<endl;
	cout<< "initial point" << datumPnt  << " maps to " << currentObjFunc << endl;

	Push_Base_Point(datumPnt, "initial" );

	for (unsigned int i=0; i<InitialStep.size(); ++i)
		InitialStep[i]=TS_ObjFunc.range[i]*CurrentStep[i];
	cout << "initial step" << endl << "\t" << InitialStep << endl;


	Point2 recalculated_step(InitialStep), importance_indicator_point(InitialStep);
	if(-1<starting_point && starting_point<0){
		importance_indicator_point=sample_and_assess( (int) std::ceil(starting_point*(-1)*EL) );
		for(unsigned int i=0; i<nVar; ++i)
			recalculated_step[i]=InitialStep[i]*(1.0+importance_indicator_point[i]);

		InitialStep=recalculated_step;
		CurrentStep=InitialStep;
	cout <<"recalculated step:" << recalculated_step << endl;
	}


	save_optimisation_report(__case_name+"_MOTS2_initial_configuration_settings_report.log");
	cout << "starting a new search" << endl;
	cout << "starting point:" << endl;
	cout << __BasepointMemory.get_current_base_point() << "===>" << HISTORY[__BasepointMemory.get_current_base_point()] << endl;
	cout << "step:" << endl;
	cout << CurrentStep << endl;

	cout <<"just type 0" << endl;
		//cin >> iLocal;
};



void TabuSearch::save_optimisation_report(std::string name){
	ofstream report_file(name.c_str());


	report_file << "MOTS2 configuration:! " << endl;


	report_file << "\t1.diversify " << diversify << endl;
	report_file << "\t2.intensify " <<  intensify<< endl;
	report_file << "\t3.reduce " <<  reduce<< endl;
	report_file << "\t4.SS " <<  SS<< endl;
	report_file << "\t5.SSRF " <<  SSRF<< endl;
	report_file << "\t6.save_step " <<  save_step<< endl;
	report_file << "\t7.n_sample " <<  n_sample<< endl;
	report_file << "\t8.nVar " <<  nVar<< endl;
	report_file << "\t9.nObj " <<  nObj<< endl;
	report_file << "\t10.n_of_loops " <<  LL<< endl;
	report_file << "\t11.n_of_evaluations " <<  EL<< endl;
	report_file << "\t12.n_of_consecutive_improvements " <<  IL<< endl;
	report_file << "\t13.assessment " <<  assessment<< endl;
	report_file << "\t14.nRegions " <<  nRegions<< endl;
	report_file << "\t15.STM_size " <<  STM_size<< endl;
	report_file << "\t16.LogType " <<  logtype<< endl;
	report_file << "\t17.Starting point " <<  starting_point << endl;
	report_file << "\t18.maximum_improvements " <<  maximum_improvements<< endl;
	report_file << "\t19.maximum_duplicates " <<  maximum_duplicates<< endl;
	report_file << endl << endl;


	report_file << "Design vector ranges:" <<endl;
	for(unsigned int i=0; i<nVar; ++i)
		report_file <<"\tvariable " << i << " ranges from " << __DefaultConfigurationSettings.get_lower_bound()[i] << " to " << __DefaultConfigurationSettings.get_upper_bound()[i] << endl;
	report_file << endl << endl;

	report_file << "Reference point:" <<endl;
	report_file << "\t" << reference_point << endl;
	report_file << endl << endl;

	report_file << "Failed objectives:" <<endl;
	report_file << "\t" << __DefaultConfigurationSettings.get_penalty_point() << endl;
	report_file << endl << endl;

	report_file << "Start step (related with SS, above):" <<endl;
	if( SS==0 )
		report_file << "\t Start step(s) is assigned by user in start_step.txt file!" << endl;
	else
		report_file << "\t Start step(s) (" << SS <<")  is uniform for all variables" << endl;
	report_file << "\t" << InitialStep << endl;
	report_file << endl << endl;

	report_file << "Datum design (related with Stating point, above):" <<endl;
	if( starting_point==0)
		report_file << "\t Starting point is assigned randomly!" << endl;
	else
		report_file << "\t Starting point is by user from file" << endl;
	report_file << "\t" << datumPnt <<endl;
	report_file << endl << endl;

	report_file.close();

	cout << name.c_str() <<" report saved!" << endl;
}

//Algorithm 6a
void TabuSearch::numerical_check(Point2 input, string const location){
	for (unsigned int v=0; v<nVar; ++v)
		if( abs( input[v])< EPSILON   ){
#ifdef DEBUG
			cout << "numerical error @"<< location << " (" << v << "th element) " << input[v] << endl;
#endif //DEBUG
			input[v]=0.0;
			//			exit(-50);
		}
}


void TabuSearch::save_monitor_data(){
	//todo implement this similarly to memories

}


ObjFunction2 TabuSearch::evaluate_point(Point2 &P1){
	ObjFunction2 currentObjective;



	if (HISTORY.find(P1)==HISTORY.end()){
#ifdef DEBUG
		cout << "\t point evaluated" << endl;
#endif
		currentObjective=TS_ObjFunc.calculateObjFun(P1);
		HISTORY.insert(entry( P1 , currentObjective ) );
	}else{
#ifdef DEBUG
		cout << "\t point recalled from memory" << endl;
#endif
		currentObjective=HISTORY[P1];
	}
	return currentObjective;
}


void TabuSearch::evaluate_point_parallel(Container2 &solutions_requested_for_evaluation){
	//returns fills the solutions_requested_for_evaluation container with the appropriate objective values
	//that correspond to the respective design vectors
	Container2 toBeAcuallyEvaluated(nVar, nObj, "toBeActuallyEvaluated", "toBeActuallyEvaluated.txt");

	//checks which points already exist into history (and are loaded directly), and which should be evaluated
	for(Container2::iterator it=solutions_requested_for_evaluation.begin(); it!=solutions_requested_for_evaluation.end(); ++it){
		if (HISTORY.find(it->first)==HISTORY.end()){
			toBeAcuallyEvaluated.insert(entry(it->first,it->second));
		}else{
			it->second=HISTORY[it->first];
		}
	}

	//evaluates the solutions that have never been evaluated before and updates the solutions_requested_for_evaluation
	if(toBeAcuallyEvaluated.size()>0){
		TS_ObjFunc.calculateObjFun_parallel(toBeAcuallyEvaluated);
		//fill in the the solutions_requested_for_evaluation
		for(std::map<Point2,ObjFunction2>::iterator it=toBeAcuallyEvaluated.begin(); it!=toBeAcuallyEvaluated.end(); ++it){
			solutions_requested_for_evaluation[it->first]=it->second;
			HISTORY.insert(entry( it->first,it->second ) );
		}
	}
}

unsigned int
TabuSearch::calculate_y_range_for_monitor_plots() const {
	unsigned int max_range=0;
	max_range=std::max(max_range , iLocal );
	//	max_range=std::max(max_range , (unsigned int) IM.size() );
	max_range=std::max(max_range , diversification );
	max_range=std::max(max_range , intensification );
	max_range=std::max(max_range , reduction );

	max_range=std::max(max_range , diversify );
	max_range=std::max(max_range , intensify );
	max_range=std::max(max_range , reduce );

	return std::ceil( max_range*1.1 );
}

unsigned int
TabuSearch::calculate_x_range_for_monitor_plots() const{
	return std::ceil( cur_loops*1.1 );
}

const  std::vector<int>&
TabuSearch::calculate_y_range_for_trade_off_plots() const{
	std::vector<int> range(2,0.0);
	double min_range, max_range;
	//todo find the min and max of objective 1

	range[0]= std::floor( min_range*01.1 );
	range[1]= std::ceil( max_range*01.1 );
	return range;
}

const std::vector<int>&
TabuSearch::calculate_x_range_for_trade_off_plots() const{
	std::vector<int> range(2,0.0);
	double min_range, max_range;
	//todo find the min and max of objective 2
	range[0]= std::floor( min_range*01.1 );
	range[1]= std::ceil( max_range*01.1 );
	return range;
}


Point2  TabuSearch::increase(Point2 const &input, unsigned int const index){
	Point2 output(input);

	double temp_step=InitialStep[index]* pow(SSRF,(int) reduction);
	double temp_value=input[index];

	double temp_result= temp_value +  temp_step ;
#ifdef DEBUG_STEP
	cout << "for " << index << "th variable, the increase step is " << temp_step << endl;
#endif

	if( abs(temp_result)<  EPSILON ){
#ifdef DEBUG_STEP
		cout << " accuracy fixed ! " << endl;
#endif
		temp_result=0.0;
	}

	output[index]= temp_result  ;
#ifdef DEBUG_STEP
	cout << "\tso " << input << " equals" << output << endl;
#endif
	return output;
}

Point2  TabuSearch::decrease(Point2 const &input, unsigned int const index){
	Point2 output(input);

	double temp_step=InitialStep[index]* pow(SSRF,(int) reduction);
	double temp_value=input[index];

	double temp_result= temp_value -  temp_step ;
#ifdef DEBUG_STEP
	cout << "for" << index << "th variable, the decrease step is " << temp_step << endl;
#endif

	if( abs(temp_result)<  EPSILON ){
#ifdef DEBUG_STEP
		cout << " accuracy fixed ! " << endl;
#endif
		temp_result=0.0;
	}

	output[index]= temp_result  ;
#ifdef DEBUG_STEP
	cout << "\tso " << input << " equals" << output << endl;
#endif
	return output;
}

//Algorithm 7
int TabuSearch::PatternMove(){
	//STM_Container2::iterator tempback;
	Point2 lastPoint, lastMove, newPoint;
	Point2 currentPoint;
	ObjFunction2 tempObjFunc;
	ObjFunction2 currentObjFunc;

#ifdef DEBUG
	cout << "PatternMove " << endl;
#endif


	//	currentPoint= *iIter; //currentPoint=points(iIter) ;
	currentPoint=__BasepointMemory.get_current_base_point();
	currentObjFunc=HISTORY[currentPoint];
	//	++iIter; //go to the previous point
	//	/*
	//	 * shifts the pointer, so that it points the next "second" point on the deque.
	//	 * This happens in order to "produce" gradient.
	//	 * In essence, I recall the "second" item and the I return the pointer back to the top of the deque
	//	 */
	//	lastPoint= *iIter; //fetch the previous point
	//	--iIter; //return it to the current point
	lastPoint=__BasepointMemory.get_previous_base_point();


	lastMove=currentPoint-lastPoint;

	newPoint=currentPoint+lastMove;

#ifdef DEBUG
	// debugging output


	cout << "current point" << endl << currentPoint << endl;
	cout << "last point" << endl << lastPoint << endl;
	cout << "last move" << endl << lastMove << endl;
	cout << "new point" << endl << newPoint << endl;
#endif

	if(!TS_ObjFunc.isValid(newPoint) ){
#ifdef DEBUG
		cout << "PatternMove EXIT-0 invalid design" << endl;
#endif
		return 0;
	}

	tempObjFunc=evaluate_point(newPoint);

	if ( MTM.addIfNotDominated(  newPoint, tempObjFunc )==1 ){
		IM.insert(entry ( newPoint, tempObjFunc ));

		++iLocal;

#ifdef DEBUG
		cout << "\tPmove" << endl << newPoint << endl;
#endif

		Push_Base_Point(newPoint, "PatMove");

#ifdef DEBUG
		cout << "PatternMove EXIT-1" << endl;
#endif
		return 1;
	}else{
#ifdef DEBUG
		cout << "PatternMove EXIT-0 did not found non-dominated design" << endl;
#endif
		return 0;
	}
}

void TabuSearch::check_memories(){
#ifdef DEBUG
	cout << " check_memories START" << endl;
#endif
	ofstream mem_status;
	mem_status.open ("./monitor_data/memory_status.txt",  ios::out  );


#ifdef DEBUG
	cout << "\tLTM";
#endif
	mem_status << " LTM regions" << endl;
	mem_status<< " region 0: " << LTM.Region[0].size() << " size "<< endl;

	for (unsigned int i=1 ; i < nRegions ; ++i)
		mem_status << " region "<< i << ": " << LTM.Region[i].size()<< " size " << endl;


#ifdef DEBUG
	cout << "\tIM";
#endif
	mem_status << " IM " << IM.check_memory() << "empty variables and objectives "<< endl;

#ifdef DEBUG
	cout << "\tMTM";
#endif
	mem_status  << " MTM "<< MTM.check_memory()<< "empty variables and objectives "<< endl;

#ifdef DEBUG
	cout << "\tHISTORY";
#endif
	mem_status << " HISTORY "<< HISTORY.check_memory()<< "empty variables and objectives "<< endl;

	mem_status.close();
#ifdef DEBUG
	cout << "check_memories END" << endl;
#endif
}

//Algorithm 8
void TabuSearch::UpdateMemories(){
	Point2 currentPoint;

	int success=0;
#ifdef DEBUG
	cout << "UpdateMemories_START" << endl;


	cout << "\tmemories' sizes:" << endl;
	cout << "\tBasePoints" << __BasepointMemory.size() << endl;
	cout << "\tSTM " << STM.size() << endl;
	cout << "\tMTM " << MTM.size() << endl;
	cout << "\tIM " << IM.size() << endl;
	cout << "\tLTM " << LTM.getSize() << endl;
	cout << "\tHISTORY " << HISTORY.size() << endl<< endl;

	cout << "\tiLocal" << iLocal << endl;
#endif
	currentPoint=__BasepointMemory.get_current_base_point();
#ifdef DEBUG
	if (currentPoint.empty() ){
		cout << "empty vector!!!!!!!!!!!!!!!" << endl;
		exit(-29999);
	}
	cout << "\t currentPoint" << currentPoint << endl;
#endif

	STM.import(currentPoint);

	success=MTM.addIfNotDominated(  currentPoint , HISTORY[currentPoint]   );

#ifdef DEBUG
	cout <<"\tUp M, addIfNotDominated =" <<success << endl;
#endif

	if(success){
		MTM.removeDominatedPoints();
		iLocal=0;
	}
	IM.removeDominatedPoints();

	LTM.insert(currentPoint);

	check_memories();

#ifdef DEBUG
	cout << "\tiLocal" << iLocal << endl;
	cout << "UpdateMemories EXIT " << endl;
#endif
}

int TabuSearch::stoppingCriteriaNotMet(const unsigned int &n_of_loops,const unsigned int &n_of_evaluations, const unsigned int &n_of_consecutive_improvements){

	//#loops
	//#evaluations
	//# consecutive improvements

	// LL - loop limit
	// EL - evaluations limit
	// IL - improvements limit
	//the limits are set on the conf file


	//cout << "n_of_loops=" << n_of_loops << " n_of_consecutive_improvements=" << n_of_consecutive_improvements << " n_of_evaluations"  << n_of_evaluations << endl;

	//stops after NEVALS evaluations of ObjFunction, use with TS_ObjFunc.n_of_successful_calculations
	if( ( (n_of_loops != LL )  && (n_of_consecutive_improvements!= IL)) && (n_of_evaluations < EL || n_of_evaluations==0) ){
		//cout << "sunexizw" << endl;
		return 1; //keep iterating
	}
	else{
		//cout << "bgainw" << endl;
		return 0; //stop
	}
}



void TabuSearch::update_check_point_file(){
	std::ofstream checkpoint_file("./monitor_data/chekpoint.out", ios::out);
	checkpoint_file  << cur_loops <<"     " << __BasepointMemory.get_current_base_point() << " "<< diversification <<" "<< intensification <<" "<< reduction << " " << iLocal  << " " << CurrentStep << endl;
	checkpoint_file.close();
}

double TabuSearch::search2(std::string casename){
	time_t start,end;
	time (&start);

	iLocal=0;

	unsigned int improvement=1;
	int success=0;

	double current_quality_indicator=0.0;
	double previous_quality_indicator=0.0;

	Point2 newPoint(nVar, 0.0);

	//for restart

	cout << "start of search, datum point:" << endl;
	//	cout << *iIter << endl; //fetch current point
	cout << __BasepointMemory.get_current_base_point() << endl; //fetch current point

	cout << casename << endl;
	double temp;
	//			cin >> temp;

#ifdef FULL_LOG
	ofstream diversify_file;
	ofstream evals_file;
	ofstream hypervolume_file;
	ofstream i_local_file;
	ofstream im_size_file;
	ofstream intensify_file;
	ofstream step_size_file;
	ofstream reduction_file;
	ofstream basePoint_file;
	ofstream update_memories_file;

	if(restart_flag==1){
		diversify_file.open ( ("./monitor_data/"+__case_name+"_diversify.out").c_str(),  ios::app );
		evals_file.open ( ("./monitor_data/"+__case_name+"_evals.out").c_str(), ios::app);
		hypervolume_file.open ( ("./monitor_data/"+__case_name+"_hypervolume.out").c_str() , ios::app);
		i_local_file.open ( ("./monitor_data/"+__case_name+"_i_local.out").c_str(),  ios::app);
		im_size_file.open ( ("./monitor_data/"+__case_name+"_im_size.out").c_str(), ios::app);
		intensify_file.open ( ("./monitor_data/"+__case_name+"_intensify.out").c_str(), ios::app );
		step_size_file.open ( ("./monitor_data/"+__case_name+"_step_size.out").c_str(),  ios::app);
		reduction_file.open ( ("./monitor_data/"+__case_name+"_reduce.out").c_str(),  ios::app);
		basePoint_file.open ( ("./monitor_data/"+__case_name+"_basePoint.out").c_str(),  ios::app );
		update_memories_file.open ( ("./monitor_data/"+__case_name+"_update_memories.out").c_str(),  ios::app );
	}else{
		diversify_file.open ( ("./monitor_data/"+__case_name+"_diversify.out").c_str(),   ios::out );
		evals_file.open ( ("./monitor_data/"+__case_name+"_evals.out").c_str(),  ios::out);
		hypervolume_file.open ( ("./monitor_data/"+__case_name+"_hypervolume.out").c_str(),  ios::out);
		i_local_file.open ( ("./monitor_data/"+__case_name+"_i_local.out").c_str(),   ios::out);
		im_size_file.open ( ("./monitor_data/"+__case_name+"_im_size.out").c_str(),  ios::out);
		intensify_file.open ( ("./monitor_data/"+__case_name+"_intensify.out").c_str(),  ios::out );
		step_size_file.open ( ("./monitor_data/"+__case_name+"_step_size.out").c_str(),   ios::out);
		reduction_file.open ( ("./monitor_data/"+__case_name+"_reduce.out").c_str(),   ios::out);
		basePoint_file.open ( ("./monitor_data/"+__case_name+"_basePoint.out").c_str(),   ios::out );
		update_memories_file.open ( ("./monitor_data/"+__case_name+"_update_memories.out").c_str(),   ios::out );
	}
#endif //FULL_LOG

#ifdef QUICK_LOG
	ofstream quick_file;
	quick_file.open ("./monitor_data/quick.out", ios::out );
	quick_file<< "#N\tEVALS{+}\tVIOL\tE/I\tHV\tMTM\tiLOC\tIM\tDIVFS\tINTFS\tRDC"<<endl;

#endif //QUICK_LOG

	while (stoppingCriteriaNotMet(cur_loops-previous_cur_loops, HISTORY.count_evaluations(TS_ObjFunc.get_penalty_objectives())-previous_evaluations, improvement)){ // 0 - exit

#ifdef FULL_LOG
		diversify_file << cur_loops <<"     "<< diversification <<endl;
		evals_file << cur_loops <<"     "<< TS_ObjFunc.n_of_successful_calculations+TS_ObjFunc.violations  <<endl;
		//hypervolume_file << cur_loops <<"     "<< diversification <<endl;
		i_local_file << cur_loops <<"     "<< iLocal <<endl;
		im_size_file << cur_loops <<"     "<< IM.size() <<endl;
		intensify_file << cur_loops <<"     "<< intensification <<endl;
		step_size_file << cur_loops <<"     "<< CurrentStep <<endl;
		reduction_file << cur_loops <<"     "<< reduction <<endl;
		//		basePoint_file << cur_loops <<"     "<< *iIter <<endl; //that's the top point in the POINTS container, current point
		basePoint_file << cur_loops <<"     "<< __BasepointMemory.get_current_base_point() <<endl; //that's the top point in the POINTS container, current point
#endif //FULL_LOG

#ifdef QUICK_LOG
		quick_file<< cur_loops << "\t" << TS_ObjFunc.n_of_successful_calculations+TS_ObjFunc.violations << "{" << TS_ObjFunc.n_of_successful_calculations <<"}" << "\t(" << TS_ObjFunc.violations << ")"<< "\t" << TS_ObjFunc.n_of_successful_calculations / cur_loops << "\t"<< current_quality_indicator <<"(" << improvement <<")" << "\t" << MTM.size() << "\t" << iLocal <<"\t"<< IM.size() <<"\t"<< diversification  <<"\t"<< intensification <<"\t" << reduction << endl;
#endif //QUICK_LOG

		cout << "base point on iteration " << cur_loops << " is" << __BasepointMemory.get_current_base_point() << " ===>" << HISTORY[__BasepointMemory.get_current_base_point()] << endl;
		cout << "\t with current_step: " << CurrentStep << endl;
		update_check_point_file();


		//		numerical_check(*iIter, "main_loop");//current point
		numerical_check(__BasepointMemory.get_current_base_point() , "main_loop");//current point
		if (nextMoveHookeJeeves){
			HookeJeevesMove_parallel(cur_loops) ;
		}else {
			success = PatternMove() ;
			if (success)
				nextMoveHookeJeeves = 1 ;
			else{
				HookeJeevesMove_parallel(cur_loops) ;
			}
		}

		line18: //coming from the goto, below
#ifdef FULL_LOG
		diversify_file << cur_loops <<"     "<< diversification <<endl;
		evals_file << cur_loops <<"     "<< TS_ObjFunc.n_of_successful_calculations << "     " << TS_ObjFunc.violations << endl;
		//hypervolume_file << cur_loops <<"     "<< diversification <<endl;
		i_local_file << cur_loops <<"     "<< iLocal <<endl;
		im_size_file << cur_loops <<"     "<< IM.size() <<endl;
		intensify_file << cur_loops <<"     "<< intensification <<endl;
		step_size_file << cur_loops <<"     "<< CurrentStep <<endl;
		reduction_file << cur_loops <<"     "<< reduction <<endl;
#endif //FULL_LOG

#ifdef QUICK_LOG
		quick_file<< cur_loops << "\t" << TS_ObjFunc.n_of_successful_calculations+TS_ObjFunc.violations << "{" << TS_ObjFunc.n_of_successful_calculations <<"}" << "\t(" << TS_ObjFunc.violations << ")"<< "\t" << TS_ObjFunc.n_of_successful_calculations / cur_loops << "\t"<< current_quality_indicator <<"(" << improvement <<")" << "\t\t" << iLocal <<"\t"<< IM.size() <<"\t"<< diversification  <<"\t"<< intensification <<"\t" << reduction << endl;
#endif //QUICK_LOG


		UpdateMemories(); // <<<----- 18

		if (iLocal==diversify) {
			newPoint=DiversifyMove2() ;
			Push_Base_Point(newPoint, "DivMove");

			goto line18 ;
		}else if (iLocal==intensify) {
			newPoint=IntensifyMove2() ;
			Push_Base_Point(newPoint, "IntMove");

			goto line18 ;
		}else if (iLocal==reduce) {
			newPoint=ReduceMove2() ;
			Push_Base_Point(newPoint, "RedMove");

			goto line18 ;
		}else if (  (MTM.activate_kick("./memories/MTMfrequencies.txt", maximum_duplicates, TS_ObjFunc.failedObjectiveFunctionVector) && improvement>maximum_improvements*0.1) || improvement>maximum_improvements ){
			newPoint=ReduceMove2() ;
			Push_Base_Point(newPoint, "kickMove");

			improvement=1;
			cout << " KICK! " << endl;
			goto line18 ;
		}

		if(cur_loops % save_step == 0)
			save_memories(__case_name, cur_loops);

		current_quality_indicator=MTM.calculate_quality_indicator( reference_point );
		if(current_quality_indicator==previous_quality_indicator)
			++improvement;
		else{
			previous_quality_indicator=current_quality_indicator;
			improvement=1;
		}

		++cur_loops;// diko mou
	}

#ifdef FULL_LOG
	diversify_file.close();
	evals_file.close();
	hypervolume_file.close();
	i_local_file.close();
	im_size_file.close();
	intensify_file.close();
	step_size_file.close();
	basePoint_file.close();
	update_memories_file.close();
#endif //FULL_LOG

#ifdef QUICK_LOG
	quick_file.close();
#endif //QUICK_LOG

	save_memories(__case_name, cur_loops);

	time (&end);
	double dif = difftime (end,start);
	cout << "end in " << dif<< "seconds" << endl;
	save_optimisation_report(__case_name+"_MOTS2_end_report.log");
	return MTM.calculate_quality_indicator(reference_point);
}

void TabuSearch::saveParetoFront(){
	cout << "Pareto Front:" << endl;
	cout << "MTM size=" << MTM.size() << endl;
	ofstream myfile;
	myfile.open ("TS.txt",  ios::out );

	for(Container2::iterator it=MTM.begin() ; it != MTM.end() ; ++it){
		for( unsigned int i=0 ; i< it->second.size() ; ++i)
			myfile << it->second[i] << "\t";

		myfile << endl; //save only the objective values (NOT THE DESIGN VECTOR)
	}
	myfile.close();
}

std::string TabuSearch::give_the_move(const Point2 &P){
	for (deque<BP_tuple>::iterator it=BP_tracer.begin(); it!=BP_tracer.end() ; ++it)
		if( (*it).first == P )
			return (*it).second ;

	return "ERROR_MOVE";
}

void TabuSearch:: save_base_point(std::string case_qualifier, int loop){
	std::ostringstream base_point_name;

	base_point_name<<"./memories/" << case_qualifier << "_BASE_snap"<<loop <<".txt";

	ofstream BPfile;
	Point2 tempv1;
	BPfile.open (base_point_name.str().c_str(),  ios::out  );

	BPfile << "#loop" << "\t"<< "basepoint" << "\t" << "objectives"  << "\t" << "function" << endl;
	for(unsigned int k=0; k< __BasepointMemory.size(); ++k)
		BPfile << k << "\t"<< __BasepointMemory[k].get__basepoint() << "\t" << HISTORY[__BasepointMemory[k].get__basepoint()]  << "\t" << __BasepointMemory[k].getFunction() << endl;

	BPfile.close();
#ifdef DEBUG
	cout << "Base Points saved!" << endl;
#endif //DEBUG
}

void TabuSearch::save_memories(std::string case_qualifier, int loop){

	create_plot_scripts();
	cout << "BasePoint size=" << __BasepointMemory.size() <<" STM size=" << STM.size() << " MTM size=" << MTM.size()	<< " LTM size=" << LTM.getSize() << " IM size=" << IM.size() <<" HISTORY size=" << HISTORY.size() << endl;
	const int evaluations_counter_so_far=HISTORY.size();

	saveParetoFront();
	save_base_point(case_qualifier, evaluations_counter_so_far);

//	__BasepointMemory.save_memory("./memories",__case_name,"BASEPOINT", evaluations_counter_so_far);

	STM.save_stm_container(case_qualifier, evaluations_counter_so_far);
	MTM.save_container_snapshot(case_qualifier, evaluations_counter_so_far);
	LTM.save_ltm_container(case_qualifier, evaluations_counter_so_far);
	IM.save_container_snapshot(case_qualifier, evaluations_counter_so_far);
	HISTORY.save_container_snapshot(case_qualifier, evaluations_counter_so_far);
#ifdef TIMOS_REPORT //for TIMOS only
	MTM.report_optima_plot("optima_plot.txt");
	MTM.report_optima_current("optima_current.txt");
	HISTORY.report_optima_plot("tabu_plot.txt");
#endif //main_report
	cout << "Memories Saved!" << endl;
}

void TabuSearch::displayDeque(  deque<Point2> D){
	cout << "-=Deque Start=-" << endl;

	int i=1;
	for ( deque<Point2>::iterator it1=D.begin() ; it1!=D.end() ; ++it1 ){
		cout << i <<". "<< *it1 << endl;
		++i;
	}
	cout << "-=Deque end=-" << endl;
}

void TabuSearch::HookeJeevesMove_parallel(unsigned int loop){
	unsigned int i; //for counter
	Container2::iterator it;

	int remaining_sample_sets;

	temp_Container2 bestCandidatePoints;

	Container2 temp_sampled(nVar,nObj,"HJ_P1","temp_sampled.txt");
	Container2 temp_sampled_dominant(nVar,nObj,"HJ_P2","temp_sampled_dominant.txt"); // = new Container;
	Container2 temp_sampled_dominated(nVar,nObj,"HJ_P3","temp_sampled_dominated.txt");// = new Container;
	Container2 C1(nVar,nObj,"HJ_P4","C1.txt");
	Container2 buffer_container(nVar,nObj,"HJ_P5","buffer_container.txt");

	Point2 newPoint(nVar,0.0);
	Point2 tempPoint(nVar,0.0);
	Point2 nextPoint(nVar,0.0);
	//	Point2 currentPoint(*iIter); //current point
	Point2 currentPoint(__BasepointMemory.get_current_base_point()); //current point

#ifdef DEBUG
	cout << "HookeJeevesMove START" << endl;

	cout << "\tcur point" << endl << currentPoint <<endl;
	cout << "\tcurrent steps" << endl << CurrentStep << endl;

	cout << "\twithin tabu" << endl;
	STM.showTabu();

	cout << "\tupper bound " << TS_ObjFunc.max_bound << endl;
	cout << "\tlower bound " << TS_ObjFunc.min_bound << endl;

#endif

	bestCandidatePoints.clear();

	Point2 newPoint_incr(currentPoint), newPoint_decr(currentPoint);

	for ( i=0 ; i < nVar ; ++i) { //each designVariable
		newPoint_incr=increase(currentPoint,i) ;
#ifdef DEBUG
		cout  <<  "\tduring perturbation(+) " << newPoint_incr << " was created! " << endl;
		cout << "\tis tabu" << STM.isTabu(newPoint_incr) << " and valid" << TS_ObjFunc.isValid(newPoint_incr) << endl;
#endif
		if ( !STM.isTabu(newPoint_incr) &&  TS_ObjFunc.isValid(newPoint_incr)  ){ // isNotTabu(newPoint_incr) and isNotInvalid (newPoint_incr)
			//cout << "tabu and validity check pass" << endl;
			bestCandidatePoints.insert(newPoint_incr);
#ifdef DEBUG
			cout << "\tPoints inserted!" << endl;
#endif
		}

		newPoint_decr=decrease(currentPoint,i) ;
		//newPoint_decr[i]=temp_value - temp_step ;
#ifdef DEBUG
		cout  << "\tduring perturbation(-) " << newPoint_decr << " was created! " << endl;
		cout << "\tis tabu" << STM.isTabu(newPoint_decr) << " and valid" << TS_ObjFunc.isValid(newPoint_decr) << endl;
#endif
		if ( !STM.isTabu(newPoint_decr) && TS_ObjFunc.isValid(newPoint_decr) ){ // isNotTabu(newPoint) and isNotInvalid (newPoint)
			//cout << "tabu and validity check pass" << endl;
			bestCandidatePoints.insert(newPoint_decr);
#ifdef DEBUG
			cout << "\tPoints inserted!" << endl;
#endif
		}
	}

	remaining_sample_sets=(int)(bestCandidatePoints.size()/n_sample);
	if(bestCandidatePoints.size()%n_sample!=0)
		++remaining_sample_sets;

#ifdef DEBUG
	int total_sets=remaining_sample_sets;
	unsigned int generated_points=bestCandidatePoints.size();
	cout << "\t<bestPoints><size> There are" << generated_points <<" generated elements in best points</size>" << endl;
	cout << "\t<sample_set>which form" <<  remaining_sample_sets << "SAMPLE SETS!</sample_set></bestPoints>" << endl;

#endif

	if(bestCandidatePoints.size()==0){
		if(IM.size()>0){
			Point2 tempPnt=IM.selectRandom();
			IM.erase(tempPnt);
			++remaining_sample_sets;
			bestCandidatePoints.insert(tempPnt);
#ifdef DEBUG
			cout << "design:" << tempPnt << " was selected from IM" << endl;
			cout << "\t<bestPoints><size> contains " << bestCandidatePoints.size() <<" candidate points</size>" << endl;
			cout << "\t<sample_set>which form" <<  remaining_sample_sets << "SAMPLE SETS!</sample_set></bestPoints>" << endl;
			cout << "IM's size " << IM.size() << endl;
#endif

		}else{
			cout << "algorithm trapped, neither good candidate point was generated, nor IM has any candidate point" << endl;
			save_memories(" ",loop);
			exit(-3000);
		}
	}//if(bestPoints.size()==0)

	Container2 evaluation_buffer(nVar,nObj,"HJ_P6","evaluation_buffer.txt");//this contains the points to be sent for evaluation and then its entries are  updated

	//random sampling
	unsigned int keep_sampling=1;
	int set_counter=-1;
	while( bestCandidatePoints.size()>0 && remaining_sample_sets>0 && keep_sampling==1){
		++set_counter;
#ifdef DEBUG
		cout << "\t set " << set_counter  << " out of" << 	total_sets << endl;
#endif
		for(  i=0 ; i<n_sample && 0<bestCandidatePoints.size() ; ++i){ //evaluate 6 Points
#ifdef DEBUG
			cout << "\t sample "<< i << endl;
#endif
			tempPoint=bestCandidatePoints.selectRandom();
			bestCandidatePoints.erase(tempPoint);
			evaluation_buffer.insert(entry( tempPoint , TS_ObjFunc.penalise(tempPoint) ) );
		}

		evaluate_point_parallel(evaluation_buffer);
		for(Container2::iterator it=evaluation_buffer.begin(); it!=evaluation_buffer.end(); ++it)
			temp_sampled.insert(entry( it->first , it->second ) );

		evaluation_buffer.clear();

		for(Container2::iterator it1=temp_sampled.begin(); it1!=temp_sampled.end(); ++it1){
			if(    MTM.addIfNotDominated(   it1->first, it1->second     )==1  )
				temp_sampled_dominant.insert( entry (  it1->first, it1->second  ) );
			else
				temp_sampled_dominated.insert( entry ( it1->first, it1->second ) );
		}

#ifdef DEBUG
		cout << "so far" << temp_sampled_dominant.size() << " dominant were found!" << endl;

		cout << "\tmemories" << endl;
		cout << "\tdominant ="<< temp_sampled_dominant.size() << endl;
		cout << "\tdominated =" <<temp_sampled_dominated.size()<<endl;
#endif
		switch ( temp_sampled_dominant.size() ) {
		case 0 : //not dominant points found"
#ifdef DEBUG
			cout << "\t<ZERO_dominant> in " << set_counter << "th set, " << temp_sampled_dominant.size() << " points dominate </ZERO_dominant>" <<endl;
#endif
			--remaining_sample_sets;
			break;	//end of case 0
		case 1 : //the point is selected as basePoint / nextPoint
			//TODO: merge the following case =1 and case >1
#ifdef DEBUG
			cout << "\t<ONE_dominant> in " << set_counter << "th set, " << temp_sampled_dominant.size() << " points dominate </ONE_dominant>" <<endl;
#endif
			nextPoint=temp_sampled_dominant.begin()->first;
			keep_sampling=0;
			break;	//end of case 1
		default :
#ifdef DEBUG
			cout << "\t<N_dominant> in " << set_counter << "th set, " << temp_sampled_dominant.size() << " points dominate </N_dominant>" <<endl;
#endif
			nextPoint=temp_sampled_dominant.selectRandom();
			keep_sampling=0;
			break;
		}// end of switch
	}

	if( temp_sampled_dominant.size()==0 && temp_sampled_dominated.size()>0 ){
#ifdef DEBUG
		cout << "\t<report>sampling ended, dominant points were NOT found </report>" << endl;
#endif
		buffer_container=temp_sampled;
		buffer_container.removeDominatedPoints();

		if(buffer_container.size()!=0){
#ifdef DEBUG
			cout << "\t<report>out of the dominated, the dominant was selected </report>" << endl;
#endif
			nextPoint=buffer_container.selectRandom();
		}else{
#ifdef DEBUG
			cout << "\t<report>out of the dominated, neither dominates! random among ALL </report>" << endl;
#endif
			nextPoint=temp_sampled.selectRandom();
		}
	}

	++iLocal ;

	Push_Base_Point(nextPoint, "HJ_P_Move");


#ifdef DEBUG
	cout << "\t>>>newly inserted point @ H&J is" << nextPoint << endl; //current point
#endif
	//TODO : to be improved, prin apo ka8e evaluation, anazhtatai sto history an uparxoun hdh data
	nextMoveHookeJeeves=0;
	bestCandidatePoints.clear();

	temp_sampled.erase(nextPoint);
	for(Container2::iterator it=temp_sampled.begin() ; it!=temp_sampled.end(); ++it){
		IM.addIfNotDominated(it->first , it->second);
	}

	IM.removeDominatedPoints();

#ifdef DEBUG
	cout << "H&J EXIT " << endl;
#endif
}

//Algorithm 9
Point2 TabuSearch::DiversifyMove2(){
	Point2 newPoint(nVar,0.0);
	ObjFunction2 newObjFunc(nObj,0.0);

#ifdef DEBUG
	cout << "DiversifyMove " << endl;
#endif
	int patience=20;
	do{

#ifdef DEBUG
		cout << "\t<message>Div Move: find region with minimum changes ang generate new point </message>" << endl;
		cout << "\t<size>LTM size before selection = "<< LTM.getSize() << "</size>"<< endl;
#endif
		newPoint=LTM.generate_Random_Point_From_Least_Visited_Region2();

#ifdef DEBUG
		cout << "\t<message> Div Move: new Point @ random selection </message> " << endl;
		cout << "\t<np>" << newPoint << "</np>" << endl;

		cout << "\t<bp>basePoint is =" << newPoint << "</bp>"<< endl;
		if (newPoint.empty())
			cout << "\t<error>ERROR: empty basePoint at DiversifyMove</error>" << endl;
		if (HISTORY.find(newPoint)==HISTORY.end())
			cout << "\t<error>ERROR: empty Objective Vector for basePoint at DiversifyMove</error>" << endl;
#endif
		--patience;
	}while ( (STM.isTabu(newPoint) || (!TS_ObjFunc.isValid(newPoint))) &&  (patience!=0) /*!isValid( newPoint )*/ );

	if(patience==0)
		newPoint=TabuSearch::IntensifyMove2();

	evaluate_point(newPoint);

#ifdef DEBUG
	cout << "\tDiv move" << endl << newPoint << endl;
#endif

	++iLocal;
	++diversification;
	nextMoveHookeJeeves = 1 ;

#ifdef DEBUG
	cout << "Diversify EXIT " << endl;
#endif

	return newPoint;
}

//Algorithm 10
Point2 TabuSearch::IntensifyMove2(){
	Point2 newPoint(nVar,0.0);
	ObjFunction2 newObjFunc(nObj,0.0);
#ifdef DEBUG
	cout << "IntensifyMove START" << endl;
	cout << " IM size" << IM.size() << endl;
#endif

	do{
		newPoint = IM.selectRandom();

#ifdef DEBUG
		cout << "\tIntensification loops" << endl << newPoint << endl;
		cout << "\tIM size" << IM.size() << endl;
#endif

		IM.erase(newPoint); //remove point
	}while( STM.isTabu(newPoint) && IM.size()>0); //TOCHANGE: condition maybe wrong

#ifdef DEBUG
	cout << "\tcheck after loops" << endl;
	cout << " IM size" << IM.size() << endl;
#endif

	++iLocal;
	++intensification;
	nextMoveHookeJeeves = 1 ;
	IM.removeDominatedPoints();
#ifdef DEBUG
	cout << "IntensifyMove EXIT" << endl;
#endif

	return newPoint;
}

//Algorithm 11
Point2 TabuSearch::ReduceMove2(){

	Point2 newPoint(nVar,0.0);
	ObjFunction2 newObjFunc(nObj,0.0);

#ifdef DEBUG
	cout << "ReduceMove START" << endl;
	cout << "\tIM size" << IM.size() << endl;
#endif

	do{
		newPoint = IM.selectRandom();

#ifdef DEBUG
		cout << "\t Reduction iteration" << endl << newPoint << endl;
#endif

	}while( STM.isTabu(newPoint)  );

	for(unsigned int i=0; i<nVar; ++i)
		CurrentStep[i]*=SSRF;

#ifdef DEBUG
	if(newPoint.empty())
		cout << "\t empty point!" << endl;
	if(newObjFunc.empty())
		cout << "\t empty objective!" << endl;
#endif

	iLocal=0;
	++reduction;
	nextMoveHookeJeeves = 1 ;

#ifdef DEBUG
	cout << "ReduceMove EXIT" << endl;
#endif

	return newPoint;
}

const Point2& TabuSearch::getDatumPnt() const {
	return datumPnt;
}
