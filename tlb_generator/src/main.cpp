#include <iostream>
#include <cstring>
#include <vector>
#include <cctype>  // std::isdigit
#include "Trip.h"
#include "Load.h"
#include "MakeScenario.h"
#include "PrintInstance.h"

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <mat-trips-file> <mat-stations-file> <nb_scenarios> <seed>"  << std::endl;
        return 0;
    }
	
	const char* filename_trips = argv[1];
	const char* filename_capacities = argv[2];
	int nb_scenarios = atoi(argv[3]);
	int seed = atoi(argv[4]);
	
	std::vector<std::vector<Trip>> scenarios;
	std::vector<std::vector<std::vector<double>>> arrival_rates_od;
	std::vector<std::vector<double>> riding_times;
	std::vector<double> capacities, targets;
	
	Load load;
	load.LoadMatlabOD(filename_trips,arrival_rates_od,targets);
	load.LoadMatlabStations(filename_capacities,riding_times,capacities);

	//Printing some output
	printf("Size stations:%d ODpairs:%d RidingTimes:%d ToMakeScenarios:%d\n",
		(int)capacities.size(),
		(int)arrival_rates_od[0].size()*(int)arrival_rates_od[0][0].size(),
		(int)riding_times.size() * (int)riding_times[0].size(),
		nb_scenarios);
	
	//for (size_t t = 0; t < 2; ++t)
	//	for (size_t i = 0; i < 2; ++i)
	//		for (size_t j = 0; j < 2; ++j)
	//			std::cout << "ArrivalRatesOD[" << t << "][" << i << "][" << j << "] = "
	//					  << arrival_rates_od[t][i][j] << std::endl;
						  
	//for(int i=0;i<10;i++)
	//	printf("cap[%d] = %.1lf\n",i,capacities[i]);
	
	//for(int i=0;i<5;i++)
	//	for(int j=0;j<2;j++)
	//		std::cout << "RidingTimes[" << i << "][" << j << "] = "
	//					  << riding_times[i][j] << std::endl;
	
	Scenario sce;
	sce.SetSeed(seed);
	sce.Generate(nb_scenarios,scenarios,arrival_rates_od,riding_times,capacities);
	
	// Retrieve the city name
	std::string full_path(filename_capacities);
	size_t pos_capital = full_path.find("Capital");
	size_t pos_divvy = full_path.find("Divvy");
	size_t pos_hubway = full_path.find("Hubway");

	int Qtot;
	std::string instance_filename;
	std::string targets_filename;
	std::string trips_400_filename;
	
	std::string city_name;
	if(pos_capital != std::string::npos)
	{
		city_name = "washington";
		Qtot = 6000;
	}
	else if(pos_divvy != std::string::npos)
	{
		city_name = "chicago";
		Qtot = 15242;
	}
	else if(pos_hubway != std::string::npos)
	{
		city_name = "boston";
		Qtot = 4000; 
	}
	
	std::string full_path_trips(filename_trips);
	size_t slash = full_path_trips.find_last_of("/\\");
	std::string filename = (slash == std::string::npos) ? full_path_trips : full_path_trips.substr(slash + 1);

	std::string number_str;
	for (char& c : filename) {
		if (std::isdigit(c)) {
			number_str += c;
		} else if (!number_str.empty()) {
			break;
		}
	}
	
	// +1 for the depot
	instance_filename = city_name + number_str + "_n" + std::to_string((int)capacities.size()+1) + "_e" + std::to_string(nb_scenarios) + ".txt";
	targets_filename = "targets_" + city_name + number_str + ".txt";	
	trips_400_filename = city_name + number_str + "_e" + std::to_string(nb_scenarios) + ".txt";
	
	Print print;
	
	if(nb_scenarios == 500)
	{
		print.WriteTripsToFile(scenarios, trips_400_filename);
		
		double avg_scenarios = 0.0;
		for(size_t e=0;e<scenarios.size();e++)
			avg_scenarios += (double)scenarios[e].size();
		avg_scenarios /= (double)scenarios.size();
		printf("Avg scenarios:%.3lf\n",avg_scenarios);
		
		return 0;		
	}
	
	print.WriteTripsAndStationsToFile(scenarios, capacities, Qtot, instance_filename);
	print.WriteTargetsToFile(targets_filename, targets);
		
    return 0;
}
