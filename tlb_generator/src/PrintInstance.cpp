#include "PrintInstance.h"

void Print::WriteTripsAndStationsToFile(const std::vector<std::vector<Trip>> & scenarios, const std::vector<double> & capacities, int Qtot, const std::string & filename)
{
	FILE* output_file = fopen(filename.c_str(), "w");

    if (!output_file) {
        std::cerr << "Error: Unable to open the trips and stations output file." << std::endl;
        return;
    }
	printf("First ... Printing output stations to file:%s\n",filename.c_str());
    fprintf(output_file, "%d\n", (int)capacities.size()+1); // + the depot
    fprintf(output_file, "%d\n",Qtot);
	
	fprintf(output_file,"0 ");
    for (int cap : capacities)
        fprintf(output_file, "%d ",cap);
	fprintf(output_file,"\n");
	printf("Next ... Printing output trips to file: %s\n",filename.c_str());

    for(size_t e=0;e<scenarios.size();e++)
		for(size_t i=0;i<scenarios[e].size();i++)
			fprintf(output_file, "%d %d %d %d %d\n",
				scenarios[e][i].origin+1, scenarios[e][i].destination+1, scenarios[e][i].start_time, scenarios[e][i].end_time, (int)e);

    fclose(output_file);	
}

void Print::WriteTripsToFile(const std::vector<std::vector<Trip>> & scenarios, const std::string & filename) 
{
    FILE* file = fopen(filename.c_str(), "w");
    if (!file) {
        std::cerr << "Failed to open trips output file: " << filename << std::endl;
        return;
    }
    printf("Printing output trips to file: %s\n", filename.c_str());

    for(size_t e=0;e<scenarios.size();e++)
		for(size_t i=0;i<scenarios[e].size();i++)
			fprintf(file, "%d %d %d %d %d\n",
				scenarios[e][i].origin+1, scenarios[e][i].destination+1, scenarios[e][i].start_time, scenarios[e][i].end_time, (int)e);

    fclose(file);
}

void Print::WriteTargetsToFile(const std::string & filename, const std::vector<double> & targets) 
{
	std::ofstream outputFile(filename); 
	if (outputFile.is_open()) 
	{
		for (int i=0;i<targets.size();i++)
			outputFile << targets[i] <<'\n'; 
		outputFile.close();  // Close the file
		std::cout << "Exact targets have been written to " << filename << std::endl;
	} else 
		std::cerr << "Unable to open the file: " << filename << std::endl;
}