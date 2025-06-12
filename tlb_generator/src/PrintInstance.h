#ifndef PRINT_INSTANCE_H
#define PRINT_INSTANCE_H

#include "Trip.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

class Print
{
	public:
	
		void WriteTripsAndStationsToFile(const std::vector<std::vector<Trip>> & scenarios, const std::vector<double> & capaities, int Qtot, const std::string & filename);
		void WriteTripsToFile(const std::vector<std::vector<Trip>> & scenarios, const std::string & filename);
		void WriteTargetsToFile(const std::string & filename, const std::vector<double> & targets);		
	
};

#endif