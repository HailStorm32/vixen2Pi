#include "lights.h"
//#include <array>


int main(int argc, const char** argv)
{	
	wiringPiSetup();

	pinMode(4, OUTPUT);




	Lights<8> show(500, "Carol_of_the_bells.csv");

	std::cout << "Test" << std::endl;


	show.startShow(500);




	
	//TESTING
	std::cout << "\n\nTESTING" << std::endl;	
//	int testArray[4] = {2,4,6,8};
	std::array<int,4> testArray = {2,4,6,8};

	std::vector<std::array<int, 4>> parsedFile;
	
	parsedFile.push_back({40,41,42,43});	
	parsedFile.push_back(testArray);
	parsedFile.push_back({50,60,70,90});

	for(int indx = 0; indx != parsedFile.size(); indx++)
	{
		for(int indx2 = 0; indx2 != 4; indx2++)
		{
			std::cout << parsedFile[indx][indx2] << " ";
		}
		std::cout << std::endl;
	}
	

	while(true)
	{

	digitalWrite(4, HIGH);

	delay(500);

	digitalWrite(4, LOW);

	delay(500);

	}
	
}
