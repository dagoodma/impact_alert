
#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <fstream>
// Include OpenCV libraries
// TODO
#include <opencv2/core.hpp>
#define M_PI       3.14159265358979323846

#define RAD2DEG 180.0/M_PI
#define DEG2RAD M_PI/180.0

#define FOCAL_LENGTH 1600.0f

// Foreshortening coefficients (with 95% confidence bounds):
#define FORESHORTENING_A 7.479f   // (5.066, 9.892)
#define FORESHORTENING_B -0.1759f //  (-0.245, -0.1069
#define FORESHORTENING_C 1.7f     //  (0.8402, 2.559)
#define FORESHORTENING_D -0.0231f //  (-0.03496, -0.01123)

#define FORESHORTENING_GAIN 2.0f //meter * pixel
#define FORESHORTENING_DECAY 20.0f // pixel

#define FRAME_RATE 0.033f
#pragma once
using namespace std;
using namespace cv;

class Dynamics
{
public:
	Dynamics(double initialSpeed, double initialHeight, double angle);
	void runSimulation();
	void calculateCurrentPositions();
	void calculateCurrentSpeeds();
	void calculateAirResistance();
	bool onlyCalculateIdealPath();
	void Init(double initialSpeed, double initialHeight, double angle, int BallRadius);
	void EstimateVelocityComponents(cv::Point3d currentPnt, cv::Point3d prevPnt,int currentBallRadius, int prevBallRadius, double &V, double &verticalAngle ,double &horizontalAngle,unsigned int diffBetweenTracks);
	double EstimateDistance(int r);
	double EstimateCameraCoord(int pixel, double pz);
	int EstimateImageCoord(double p,double pz);

	void Dynamics::SetIdealPath();
	//------------------------VARIABLES-------------------------------
private:
	//GRAVITY
	double gravity; // m/s^2 (down is defined to be positive)
	//AIR
	double airDensity; // Air density in kg/m^3
	double airResistance; // Air resistance at current moment in N
	double dragCoefficient;
	//ANGLE
	double angle; // Angle throw in degrees
	double m_verticalAngle;
	
	//AREA
	double area; // Cross sectional area of object
	//SPEED
	double initialSpeed;
	double currentHorizontalSpeed; // x component of object
	double currentVerticalSpeed; // y component of object
	double currentAirSpeed; // actual speed of object
	//POSITION
	double initialHeight_pixels;// in pixels
	double m_initialHeight;//in meters
public:
	double m_horizontalAngle;

	std::vector<double> height;
	std::vector<double> distance;
	std::vector<double> z;
	std::vector<double> x;

	std::vector<double> idealHeight;
	std::vector<double> idealDistance;
	std::vector<double> idealZ;
	std::vector<double> idealX;

	//MASS
	double mass;
	//TIME
	double time ; // Time that has passed in simulation
	double idealTime ; //Time passed in ideal simulation. 
	int timeInterval; //Used to access height and distance array
	double PHYSICS_REFRESH_RATE ;  //Time interval calculated
	

	ofstream logfile;
  

	int iteration;
public:
	double m_timeToImpact;
	double m_timeToFlyBy;

	//--------------------------GETTERS AND SETTERS--------------------------
public:
	void setGravity(double gravity)
	{
		this->gravity = gravity;
	}

	void setAirDensity(double airDensity)
	{
		this->airDensity = airDensity;
	}

	void setDragCoefficient(double dragCoefficient)
	{
		this->dragCoefficient = dragCoefficient;
	}

	void setArea(double area)
	{
		this->area = area;
	}

	void setMass(double mass)
	{
		this->mass = mass;
	}

	int getArraySize()
	{
		return min(height.size(), distance.size());
	}

	int getIdealArraySize()
	{
		return min(idealHeight.size(), idealDistance.size());
	}

	double getHeight(int i)
	{
		return height[i];
	}

	double getMaxHeight()
	{
		return 0;// max(height);
	}

	double getMaxDistance()
	{
		return distance[distance.size()];
	}

	double getMaxIdealHeight()
	{
		return 0;// Collections.max(idealHeight);
	}

	double getMaxIdealDistance()
	{
		return idealDistance[idealDistance.size()];
	}

	double getLargestArrayValue(bool isIdealPath)
	{
		if (isIdealPath)
		{
			std::vector<double> myVector(std::max_element(std::max_element(idealHeight.begin(), idealHeight.end()),
				std::max_element(idealDistance.begin(), idealDistance.end())),
				std::max_element(std::max_element(height.begin(), height.end()),
				std::max_element(distance.begin(), distance.end())));
//			double temp =  std::max_element(myVector.begin(), myVector.end());
			return 1;
		}
		else
		{
			//std::vector<double> myVector = std::max_element(height.begin(), height.end()), std::max_element(distance.begin(), distance.end());
			return 0;//std::max_element(myVector);
		}
	}

	double getDistance(int i)
	{
		return distance[i];
	}

	double getIdealHeight(int i)
	{
		return idealHeight[i];
	}

	double getIdealDistance(int i)
	{
		return idealDistance[i];
	}

	int getTimeInterval()
	{
		return timeInterval;
	}
	double getTime()
	{
		return time;
	}

	double getIdealTime()
	{
		return idealTime;
	}

	double getAirSpeed()
	{
		return currentHorizontalSpeed;
	}


};
