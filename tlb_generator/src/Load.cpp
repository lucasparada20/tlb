#include "Load.h"

void Load::LoadMatlabOD(const char * filename,std::vector<std::vector<std::vector<double>>> & arrival_rates_od, std::vector<double> & targets)
{
    printf("Loading .mat trips file:%s\n",filename);
	mat_t* matfp = Mat_Open(filename, MAT_ACC_RDONLY);
    if (!matfp) {
        std::cerr << "Error opening trips file: " << filename << std::endl;
        exit(1);
    }

	matvar_t* matvar; //will take the value of FILE *, returns a null if it fails (meaning we arrived at EOF)
	
    while ((matvar = Mat_VarReadNext(matfp)) != NULL) 
	{
        std::string varname = matvar->name;
        //Print output from .mat files
		std::cout << "Variable name: " << varname << std::endl;
		std::cout << "  Class type: " << matvar->class_type << ", Data type: " << matvar->data_type << std::endl;
        std::cout << "  Rank: " << matvar->rank << std::endl;
        std::cout << "  Dimensions: ";
        for (int i = 0; i < matvar->rank; ++i)
            std::cout << matvar->dims[i] << " ";
        std::cout << std::endl;
		
		/*if (varname == "Description" && matvar->data_type == MAT_T_UTF8 && matvar->class_type == MAT_C_CHAR) 
		{
			size_t len = matvar->nbytes / sizeof(uint8_t); // total number of characters
			char* str = new char[len + 1]; 

			// Copy the data and null-terminate it
			memcpy(str, matvar->data, len);
			str[len] = '\0';

			std::cout << "  Description: " << str << std::endl;

			delete[] str;
		}*/


		/*if (varname == "ArrivalRates" && matvar->data_type == MAT_T_DOUBLE && matvar->rank == 2) {
            size_t rows = matvar->dims[0];
            size_t cols = matvar->dims[1];
            double* data = static_cast<double*>(matvar->data);

            // Print values row by row (converted from column-major)
            std::cout << "  Values:\n";
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    std::cout << data[j * rows + i] << " ";
                }
                std::cout << "\n";
            }
        }*/
		
		if(varname == "InitialState00ofer")
		{
			size_t n = matvar->dims[0];
			targets.resize(n,-1);
			
			double * data = (double *)(matvar->data);
			
			for(size_t i=0;i<n;i++)
				targets[i] = data[i];
		}
		
		if (varname == "ArrivalRatesOD" &&
			matvar->class_type == MAT_C_DOUBLE &&
			matvar->data_type == MAT_T_DOUBLE &&
			matvar->rank == 3)
		{
			size_t T = matvar->dims[0];  // 48
			size_t N1 = matvar->dims[1]; // i
			size_t N2 = matvar->dims[2]; // j

			double * data = (double *)(matvar->data);

			// allocate 3D vector: [T][i][j]
			arrival_rates_od.resize(
				T, std::vector<std::vector<double>>(N1, std::vector<double>(N2)));

			// MATLAB stores column-major: data[t + T * (i + N1 * j)]
			for (size_t t = 0; t < T; ++t) {
				for (size_t i = 0; i < N1; ++i) {
					for (size_t j = 0; j < N2; ++j) {
						size_t idx = t + T * (i + N1 * j);
						arrival_rates_od[t][i][j] = data[idx];
					}
				}
			}

			//std::cout << "  ArrivalRatesOD[0][0][0] = " << arrivalRatesOD[0][0][0] << std::endl;
		}


        Mat_VarFree(matvar);
    }

    Mat_Close(matfp);	
}

void Load::LoadMatlabStations(const char * filename, 
								std::vector<std::vector<double>> & riding_times, 
								std::vector<double> & stations)
{
	printf("Loading .mat stations file:%s\n",filename);
	mat_t * matfp = Mat_Open(filename, MAT_ACC_RDONLY);
    if (!matfp) {
        std::cerr << "Error opening stations file: " << filename << std::endl;
        exit(1);
    }
	
	matvar_t* matvar; //will take the value of FILE *, returns a null if it fails (meaning we arrived at EOF)
	
	while ((matvar = Mat_VarReadNext(matfp)) != NULL)
	{
        std::string varname = matvar->name;
        //Print output from .mat files
		//std::cout << "Variable name: " << varname << std::endl;
		//std::cout << "  Class type: " << matvar->class_type << ", Data type: " << matvar->data_type << std::endl;
        //std::cout << "  Rank: " << matvar->rank << std::endl;
        //std::cout << "  Dimensions: ";
        //for (int i = 0; i < matvar->rank; ++i)
        //    std::cout << matvar->dims[i] << " ";
        //std::cout << std::endl;
		
		if(varname == "C" && matvar->data_type == MAT_T_DOUBLE)
		{
			size_t n = matvar->dims[0];
			stations.resize(n,-1);
			
			double * data = (double *)(matvar->data);
			
			for(int i=0;i<n;i++)
				stations[i] = data[i]; 
		}
		
		if(varname == "RidingTime" && matvar->data_type == MAT_T_DOUBLE)
		{
			size_t n = matvar->dims[0]; // A 2D squared matrix with zeroes in the diagonal
			//printf("RidingTime dim:%d\n",n);
			riding_times.resize(n,std::vector<double>(n,-1));
			
			double * data = (double *)(matvar->data);
			for(size_t i=0;i<n;i++)
				for(size_t j=0;j<n;j++)
				{
					size_t idx = i + n * j;
					riding_times[i][j] = data[idx];
				}
					
		}
			
		Mat_VarFree(matvar);
	}
	
	Mat_Close(matfp);
}