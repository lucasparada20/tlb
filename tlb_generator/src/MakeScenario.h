#ifndef MAKE_SCENARIO_H
#define MAKE_SCENARIO_H

#include "Trip.h"
#include <vector>
#include "RandomNumbers.h"
#include <valarray>
#include <iostream>

class Scenario
{
	public:
		void Generate(const int nb_scenarios, 
				std::vector<std::vector<Trip>> & scenarios,
				std::vector<std::vector<std::vector<double>>> & arrival_rates_od,
				std::vector<std::vector<double>> & riding_times,
				std::vector<double> & capacities);
		
		RandomNumbers rn;
		void SetSeed(int s){ rn.init(s);}
};

#endif