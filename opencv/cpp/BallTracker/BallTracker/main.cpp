#include <string>
#include <iostream>
#include <stdexcept>
#include "BallTracker.hpp"
#include "TrackingParameters.hpp"
#include "Utility.hpp"


int main (int argn, char* args[]) {

    if (argn != 2) {
        std::cout << "Usage: ballTracker <file or device>" << std::endl;
		return ERROR;
	}

    std::string fileOrDevice = args[1];
	BallTracker *tracker;
	try {
		if (utility::isNumber(fileOrDevice)) 
			tracker = new BallTracker(std::stoi(fileOrDevice), true, true);
		else
			tracker = new BallTracker(fileOrDevice, true, true);

		tracker->run();
	}
	catch(const std::exception &e) {
		std::cout << e.what() << std::endl;
		return ERROR;
	}

	return SUCCESS;
}