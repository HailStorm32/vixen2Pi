#include "lights.h"

template <unsigned int NUM_CHANNELS>
Lights<NUM_CHANNELS>::Lights(int delayTime, std::string vixenFile)
{
	this->delayTime = delayTime;
	this->vixenFile = vixenFile;
}


template <unsigned int NUM_CHANNELS>
bool Lights<NUM_CHANNELS>::startShow(int startDelay)
{
	//If there is a delay, run it
	if(startDelay > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(startDelay)); 
	}	
	return true;
}


template <unsigned int NUM_CHANNELS>
bool Lights<NUM_CHANNELS>::parseFile()
{
	int lineValues[NUM_CHANNELS - 1] = {};
	char lineData[MAX_CHAR_LIMIT] = {};
	std::string str2int = "";
	int dataCnt = 0;
	int chIndx = 0;

	csv.open(vixenFile);

	//Return error if cant open file
	if(!csv.is_open())
	{
		std::cout << "ERROR: Cant open file: " << vixenFile << std::endl;
		return false;
	}

	while(!csv.eof())
	{
		csv.getline(lineData, MAX_CHAR_LIMIT);

		//Make sure we didnt reach the char limit
		if(csv.fail())
		{
			std::cout << "ERROR: Expected less characters when reading line in:"<< vixenFile << std::endl;
			return false;
		}

		//Go through the line we just got and take out the data
		for(int indx=0; indx < lineData.length(); indx++)
		{
			if((lineData[indx] >= 48 || lineData[indx] <= 57) && dataCnt != 3)
			{
				str2int += lineData[indx];
				dataCnt++;
			}
			else if(!(lineData[indx] >= 48 || lineData[indx] <= 57) && dataCnt < 3)
			{
				std::cout << "ERROR: Found '" << lineData[indx] << "' in " << vixenFile << ", expected a number value at beginning of line" << std::endl;
				return false;
			}
			else if(!(lineData[indx] >= 48 || lineData[indx] <= 57) && dataCnt == 3)//If not a number and we have already read the 3 chars, then its a comma
			{
				lineValues[chIndx] = std::stoi(str2int);	
				
				chIndx++;
				dataCnt = 0;
			}
		}
		
	}
}
