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
	Lights(int usrFileTiming = 25, int usrDelayAdj = 0)
	{
		wiringPiSetup();

		//Setup the pins we need
		for(int indx = 0; indx != NUM_CHANNELS; indx++)
		{
			pinMode(indx,OUTPUT);
		}

		//Turn all the lights off
		for(int indx = 0; indx != NUM_CHANNELS; indx++)
		{
			digitalWrite(indx, OFF);
		}
		
		isLightsOn = false;

		//Check bounds of usrFileTiming
		if(usrFileTiming == 25 || usrFileTiming == 50 || usrFileTiming == 100)
		{
			fileTiming = usrFileTiming;
		}
		else
		{
			std::cout << "\n\nWARNING: Given fileTiming value: " << usrFileTiming << ", is out of bounds. Reverting back to default value of: 25" << std::endl;
		}

		//Check bounds of usrDelayAdj
		if((fileTiming - usrDelayAdj) >= 0)
		{
			delayAdj = usrDelayAdj;
		}
		else
		{
			std::cout << "\n\nWARNING: Given delayAdj value: " << usrDelayAdj << ", results in a negative result. Reverting back to default value of: 0" << std::endl;
			delayAdj = 0;
		}
	}
	
	bool lightsState()
	{
		return isLightsOn;
	}

	bool startShow(std::string vixenFile,int startDelay = 0,int mpvWaitTime = 4850)
	{
		
		this->vixenFile = vixenFile;

		//If there is a delay, run it
		if(startDelay > 0)
		{
			std::cout << "Delay for: " << startDelay << "ms" << std::endl;

			std::this_thread::sleep_for(std::chrono::milliseconds(startDelay)); 
		}	

		if(!parseFile())
		{
			std::cout << "\n\nParsing '" << vixenFile << "' failed, cant continue" << std::endl;
			return false;
		}
		
		std::cout << "\n\nStarting show..." << std::endl;//DEBUG
	/*	std::cout << "4..." << std::endl;//DEBUG
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));//DEBUG
		std::cout << "3..." << std::endl;//DEBUG
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));//DEBUG 
		std::cout << "2..." << std::endl;//DEBUG
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));//DEBUG 
		std::cout << "1..." << std::endl;//DEBUG	
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));//DEBUG 
		std::cout << "START" << std::endl;//DEBUG
//		std::this_thread::sleep_for(std::chrono::milliseconds(200));//DEBUG */ 

		std::thread second(system,"mpv carolOfTheBells.mp3");		
		
		std::this_thread::sleep_for(std::chrono::milliseconds(mpvWaitTime)); 

		for(int indx = 0; indx != parsedFile.size(); indx++)
		{
			for(int chIndx = 0; chIndx != NUM_CHANNELS; chIndx++)
			{
				if(parsedFile[indx][chIndx] == 255)
				{
					digitalWrite(ch2PinMap[chIndx], ON);
				}	
				else if(parsedFile[indx][chIndx] == 0)
				{
					digitalWrite(ch2PinMap[chIndx], OFF);
				}
				else
				{
					std::cout << "\n\nNot setup for values in between 0 & 255. Please edit startShow in Lights.h to allow for in between values." << std::endl;
					return false;
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(fileTiming-delayAdj)); 
		}

		std::cout << "Show finished" << std::endl;

		return true;
	}


	bool setState(const int state)
	{
		switch(state)
		{
		case 0://Turn all lights OFF
			for(int indx = 0; indx != NUM_CHANNELS; indx++)
			{
				digitalWrite(indx, OFF);
			}
			isLightsOn = false;
			break;
		case 1://Turn all lights ON
			for(int indx = 0; indx != NUM_CHANNELS; indx++)
			{
				digitalWrite(indx, ON);
			}
			isLightsOn = true;
			break;
		case 2://Sweep lights once
			for(int indx = 0; indx != NUM_CHANNELS; indx++)
			{
				digitalWrite(indx, ON);
	
				std::this_thread::sleep_for(std::chrono::milliseconds(700)); 
			}
			isLightsOn = true;		

			std::this_thread::sleep_for(std::chrono::milliseconds(2000)); 

			for(int indx = NUM_CHANNELS - 1; indx != -1; indx--)
			{
				digitalWrite(indx, OFF);
	
				std::this_thread::sleep_for(std::chrono::milliseconds(700)); 
			}
			isLightsOn = false;
			break;
		default:
			std::cout << "\n\nERROR: '" << state << "' not a valid state" << std::endl;
			return 0;
		}
		return 1;
	}


private:
	//Varibles
	
	const bool ON = false; 
	const bool OFF = true;

	int fileTiming;
	int delayAdj;
	int ch2PinMap[NUM_CHANNELS] = {0,1,2,3,4,5,6,7};//What channel correlates to what pin#
	const int MAX_CHAR_LIMIT = ((NUM_CHANNELS * 3) + (NUM_CHANNELS - 1)+2);
	bool isLightsOn;
	std::fstream csv;

	std::string vixenFile;

	std::vector<std::array<int,NUM_CHANNELS>> parsedFile;
		
	
	bool parseFile()
	{
		std::array<int,NUM_CHANNELS> lineValues = {}; //int lineValues[NUM_CHANNELS] = {};
		std::string lineData = "";
		std::string str2int = "";
		int charCnt = 0;//keeps track of how many characters we encountered since the last comma
		int chIndx = 0;

		std::cout << "Parsing '" << vixenFile << "'..." << std::endl;

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

			//Make sure we didnt reach the char limit
			if(csv.fail())
			{
			//	std::cout << "ERROR: failbit fliped in file: "<< vixenFile << std::endl;
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
					std::cout << "ERROR: Found '" << lineData[indx] << "' in " << vixenFile << ", expected a number value" << std::endl;
					return false;
				}
				else if(!(lineData[indx] >= 48 && lineData[indx] <= 57) && charCnt == 3)//If not a number and we have already read the 3 chars, then its a comma
				{
					lineValues[chIndx] = std::stoi(str2int);//take the value we got and store it with the other values for this line
				
					chIndx++;
					str2int = "";
					charCnt = 0;//reset the character count 
				}
			}

			//Store the values from the line with the other parsed line values	
			parsedFile.push_back(lineValues);

			//Reset variables before going to the next line
			lineData = "";
			lineValues.fill(0);
			str2int = "";
			chIndx = 0;
			charCnt = 0;
		}
		
	/*	//DEBUG
		std::cout << "\n\n\nFULL VECTOR" << std::endl;
		for(int indx = 0; indx != parsedFile.size(); indx++)
		{
			for(int indx2 = 0; indx2 != NUM_CHANNELS; indx2++)
			{
				std::cout << parsedFile[indx][indx2] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << "\nDone" << std::endl;*/

		std::cout << "\nDone parsing '" <<  vixenFile << "'" << std::endl;
		return true;
	}
};


