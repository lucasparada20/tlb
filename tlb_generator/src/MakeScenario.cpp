#include "MakeScenario.h"

void Scenario::Generate(const int nb_scenarios, 
				std::vector<std::vector<Trip>> & scenarios,
				std::vector<std::vector<std::vector<double>>> & arrival_rates_od,
				std::vector<std::vector<double>> & riding_times,
				std::vector<double> & capacities)
{
	
	printf("In Generate scenarios:%d stations:%d\n",nb_scenarios,(int)capacities.size());
	
	std::cout << "T (first dimension) = " << arrival_rates_od.size() << std::endl;
	if (!arrival_rates_od.empty()) {
		std::cout << "N (second dimension) = " << arrival_rates_od[0].size() << std::endl;
		if (!arrival_rates_od[0].empty()) {
			std::cout << "N (third dimension) = " << arrival_rates_od[0][0].size() << std::endl;
		}
	}

	
	// The times are given in counts of half hours : 48 in total = 24h
	int start_hour = 14; // 7h starting from 0h
	int end_hour = 32; // 7h + 9.5h = 16h30 in pp. 9 of Datner et al., (2017)
	
	int nb_stations = (int)capacities.size();
	
	scenarios.resize(nb_scenarios);
	
	for(int e=0; e<nb_scenarios; e++)
	{	
		scenarios[e].reserve(10000);
		for(int i=0;i<nb_stations;i++)
		{
			for(int j=0;j<nb_stations;j++)
			{
				for(int t = start_hour; t <= end_hour; t++)
				{
					//if(arrival_rates_od[t][i][j] < 0.00001) continue;
					
					double time=0.0;
					while(true)
					{
						double u = rn.rand01();
						double rate_per_min = arrival_rates_od[t][i][j] / 30.0;
						
						double epsilon = 1e-9; // A small tolerance value
						if (std::abs(rate_per_min) < epsilon) break;
						double time_next = time + (-std::log(1 - u) / rate_per_min);
						
						if(time_next > 30.0) break; 
						
						int origin = i;
						int destination = j;
						int scenario = e;
						
						double y = (i == j) ? 30.0 : 30.0*riding_times[i][j];
						
						int start_time = t*30.0 + time_next - start_hour*30.0; // time 0 is 7h to match TLB instances
						int end_time = t*30.0 + time_next + y - start_hour*30.0;
						
						if(start_time == end_time) continue;
						
						scenarios[e].emplace_back( origin, destination, start_time, end_time, scenario );
						
						//scenarios[e][scenarios[e].size()-1].Show(); getchar();
						
						if((int)scenarios[e].size() % 500 == 0)
							printf("Scenario %d generated %d trips ...\n",e,(int)scenarios[e].size());
						
						time = time_next;
					}
				}				
			}
		}
	}
	
	printf("Generated the following scenarios and trips ...\n");
	for(int e=0;e<nb_scenarios;e++)
		printf("Scenario:%d trips:%d\n",e,(int)scenarios[e].size());
	
	double sum=0.0;
	for(size_t t=14;t<=32;t++)
		for(size_t i=0;i<arrival_rates_od[t].size();i++)
			for(size_t j=0;j<arrival_rates_od[t][i].size();j++)
				sum += arrival_rates_od[t][i][j];
	
	printf("Sum of lambdas:%.2lf\n",sum);
		
}