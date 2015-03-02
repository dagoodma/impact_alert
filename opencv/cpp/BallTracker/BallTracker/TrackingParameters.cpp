#include <stdexcept>
#include <iostream>
#include "TrackingParameters.hpp"


TrackingParameters::TrackingParameters(bool debug, int hueLower, int hueUpper, int satLower, int satUpper, int valLower,
    int valUpper, int filterIterations, int filterSigma, int houghDp, int houghMinDist, int houghThreshLower,
    int houghThreshUpper, int houghMinRadius, int houghMaxRadius) {
    // Save parameters
    parameters["hueLower"] = hueLower;
    parameters["hueUpper"] = hueUpper;
    parameters["satLower"] = satLower;
    parameters["satUpper"] = satUpper;
    parameters["valLower"] = valLower;
    parameters["valUpper"] = valUpper;
    parameters["filterIterations"] = filterIterations;
    parameters["filterSigma"] = filterSigma;
    parameters["houghDP"] = houghDp;
	parameters["houghMinDist"] = houghMinDist;
    parameters["houghThreshLower"] = houghThreshLower;
    parameters["houghThreshUpper"] = houghThreshUpper;
    parameters["houghMinRadius"] = houghMinRadius;
    parameters["houghMaxRadius"] = houghMaxRadius;
    this->debug = debug;
	parameterChanged = false;
}

void TrackingParameters::changeParameter(int value, std::string name) {
    //std::string key = std::string(reinterpret_cast<char(*)>(name));
    parameters[name] = value;
    parameterChanged = true;
    if (debug)
        std::cout << name << ": " << value << std::endl;
}

void TrackingParameters::printParameters() {
    std::cout << "Tracking parameters:" << std::endl;
    for (std::map<std::string,float>::iterator iter = parameters.begin();
		iter != parameters.end(); ++iter) {
        std::string key = iter->first;
        int value = iter->second;
        std::cout << "\t" << key << ": " << value << std::endl;
    }
}

int TrackingParameters::getParameter(std::string key) {
    if (hasParameter(key))
        return parameters[key];
    else
        throw std::out_of_range("Key not found");
}

bool TrackingParameters::hasParameter(std::string key) {
    return parameters.count(key) > 0;
}

bool TrackingParameters::wasChanged() {
    bool result = parameterChanged;
    parameterChanged = false;
    return result;
}