#include <wiringPi.h>
#include <fstream>
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>
#include <string>

template <int NUM_CHANNELS>
class Lights
{

public:
	Lights(int delayTime, std::string vixenFile)
	{
		this->delayTime = delayTime;
		this->vixenFile = vixenFile;
	}


	bool startShow(int startDelay = 0)
	{
		//If there is a delay, run it
		if(startDelay > 0)
		{
			std::cout << "Delay for: " << startDelay << "ms" << std::endl;

			std::this_thread::sleep_for(std::chrono::milliseconds(startDelay)); 
		}	

		parseFile();

		return true;
	}


private:
	//Varibles
	int delayTime;
	const int MAX_CHAR_LIMIT = ((NUM_CHANNELS * 3) + (NUM_CHANNELS - 1)+2);
	std::fstream csv;

	std::string vixenFile;

	std::vector<std::array<int,NUM_CHANNELS>> parsedFile;
		
	
	bool parseFile()
	{
		std::array<int,NUM_CHANNELS> lineValues = {}; //int lineValues[NUM_CHANNELS] = {};
		//std::array<char,4> lineData = {};//char lineData[MAX_CHAR_LIMIT] = {};
		std::string lineData = "";
		std::string str2int = "";
		int charCnt = 0;//keeps track of how many characters we encountered since the last comma
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
			std::getline(csv, lineData);

			std::cout << lineData << std::endl;

			//Make sure we didnt reach the char limit
			if(csv.fail())
			{
				std::cout << "ERROR: failbit fliped in file: "<< vixenFile << std::endl;
				//return false;
			}
			
			//Go through the line we just got and take out the data
			for(int indx=0; indx != lineData.length(); indx++)
			{
				if((lineData[indx] >= 48 && lineData[indx] <= 57) && charCnt != 3)
				{
					str2int += lineData[indx];
					charCnt++;
				}
				else if(lineData[indx] == 10)//if we have reached the end of the line
				{
					break;
				}
				else if(!(lineData[indx] >= 48 && lineData[indx] <= 57) && charCnt < 3)
				{
					std::cout << "ERROR: Found '" << lineData[indx] << "' in " << vixenFile << ", expected a number value at beginning of line" << std::endl;
					return false;
				}
				else if(!(lineData[indx] >= 48 && lineData[indx] <= 57) && charCnt == 3)//If not a number and we have already read the 3 chars, then its a comma
				{
					std::cout << "Reached Comma" << std::endl;

					lineValues[chIndx] = std::stoi(str2int);//take the value we got and store it with the other values for this line
				
					chIndx++;
					str2int = "";
					charCnt = 0;//reset the character count 
				}
			}

			//Store the values from the line with the other parsed line values	
			parsedFile.push_back(lineValues);

			//Reset variables before going to the next line
			//lineData.fill('-');
			lineData = "";
			lineValues.fill(0);
			str2int = "";
			chIndx = 0;
			charCnt = 0;
		}
		
		//DEBUG
		std::cout << "\n\n\nFULL VECTOR" << std::endl;
		for(int indx = 0; indx != parsedFile.size(); indx++)
		{
			for(int indx2 = 0; indx2 != NUM_CHANNELS; indx2++)
			{
				std::cout << parsedFile[indx][indx2] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << "\nDone" << std::endl;

		return true;
	}
};


