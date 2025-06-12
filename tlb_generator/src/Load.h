#ifndef LOAD_H
#define LOAD_H

#include <vector>
#include <matio.h> // C library to parse Matlab files
#include <iostream>

class Load 
{
	public:
		
		void LoadMatlabOD(const char * filename,
							std::vector<std::vector<std::vector<double>>> & arrival_rates_od,
							std::vector<double> & targets);
		
		void LoadMatlabStations(const char * filename, 
								std::vector<std::vector<double>> & riding_times, 
								std::vector<double> & stations);
	
};


#endif