#include "Dynamics.hpp"


//----------------------------CONSTRUCTOR-----------------------------------

Dynamics::Dynamics(double initialSpeed, double initialHeight, double angle)

{
	this->angle = (angle)* DEG2RAD;
	this->initialSpeed = initialSpeed;
	this->initialHeight_pixels = initialHeight;
	this->time=0;
	this->idealTime=0;
	this->timeInterval=1;//large then 1
	this->PHYSICS_REFRESH_RATE= .01;
	this->gravity = 9.81;
	this->airDensity = 1.225;//for temprature of 15 celcius
	// A used ball experiences a slightly lower drag force since it has a smoother surface. A standard new ball
	//of radius 3.3 cm therefore experiences a backwards force 
	this->dragCoefficient = 0.55;// Cd is about 0.55
	this->mass = 0.057;//  m = 0.057 kg for a standard tennis ball
	this->area =  0.00342;//A = pi R^2 is the cross-sectional area of the ball, R is the radius of the ball
	this->m_timeToFlyBy = 9999;
	this->m_timeToImpact = 9999;

	currentHorizontalSpeed = cos(this->angle) * initialSpeed;
	currentVerticalSpeed = sin(this->angle) * initialSpeed;
	currentAirSpeed = initialSpeed;


	/*this->height.insert(height.end(),initialHeight);
	distance.insert(distance.end(),0.0);*/

	logfile.open ("log.txt");

	logfile << "time   " << "DownRange   " << "Height   " << "HorizontalSpeed   " << "VerticalSpeed   "<< "x   " << "z   " <<endl ;


	iteration =1;
}

void Dynamics::Init(double initialSpeed, double initialHeight, double angle, int BallRadius)
{
	this->angle = (angle)* DEG2RAD;
	this->initialSpeed = initialSpeed;
	this->initialHeight_pixels = initialHeight;

	this->height.clear();
	this->distance.clear();
	this->idealHeight.clear();
	this->idealDistance.clear();
	this->x.clear();
	this->z.clear();
	this->idealX.clear();
	this->idealZ.clear();
	this->time=0;
	this->idealTime=0;
	this->timeInterval=1;//large then 1

	this->currentHorizontalSpeed = cos(this->angle) * initialSpeed;
	this->currentVerticalSpeed = sin(this->angle) * initialSpeed;
	this->currentAirSpeed = initialSpeed;

	this->m_timeToFlyBy = 9999;
	this->m_timeToImpact = 9999;

	//this->distance.insert(distance.end(),0.0);
	double pz = EstimateDistance(BallRadius);
	double py = EstimateCameraCoord(initialHeight, pz);
	m_initialHeight = py;
	this->idealHeight.insert(idealHeight.end(), py);
	this->height.insert(height.end(), py);

	this->distance.insert(distance.end(), pz);
	this->idealDistance.insert(idealDistance.end(), pz);

	z.insert(z.end(), distance[timeInterval - 1] * cos(m_horizontalAngle* DEG2RAD));
	x.insert(x.end(), distance[timeInterval - 1] * sin(m_horizontalAngle* DEG2RAD));
	//log


	if (logfile.is_open())
		//logfile << "%Init #" << iteration << endl;
		cout << "Init #" << iteration << endl;
	else
	{
		logfile.open("log.txt", std::ofstream::app);
		//logfile << "%Init " << iteration << endl;
	}

	iteration++;	

}
double Dynamics::EstimateDistance(int r)
{
	double z;
	//z = FORESHORTENING_GAIN * exp(-BallRadius / FORESHORTENING_DECAY);//foreshortening
    // D.G.
    z = FORESHORTENING_A*exp(FORESHORTENING_B*r) + FORESHORTENING_C*exp(FORESHORTENING_D*r);
         
	return z;

}

double Dynamics::EstimateCameraCoord(int pixel,double pz)
{
	double p;
	double focus = FOCAL_LENGTH;//unit in in pixels
	p = pixel / focus*pz;
	return p;
}


int Dynamics::EstimateImageCoord(double p,double pz)
{
	double pixel;

	pixel = p / pz*FOCAL_LENGTH;
	return pixel;
}

void Dynamics::EstimateVelocityComponents(cv::Point3d currentPnt, cv::Point3d prevPnt,int currentBallRadius, int prevBallRadius, double &V, double &verticalAngle ,double &horizontalAngle,unsigned int diffBetweenTracks)
{

	double dt12 = diffBetweenTracks*FRAME_RATE;  // difference time between succssive frames of the tracked object
	//assume we have 30 fps so, if we're tracking the object continiusly it will be: 



	double Vz,Vx,Vy;//velocity 
	double Vz2,Vx2,Vy2,V2;//velocity squared
	double Vxz;//velocity in horizontal plane
	double factor1 = 1;

	double px1 = currentPnt.x, py1 = currentPnt.y, pz1 = currentPnt.z;//unit in in meters 
	double px2 = prevPnt.x, py2 = prevPnt.y, pz2 = prevPnt.z;//unit in in meters 

	pz1 = EstimateDistance(currentBallRadius);// 4.0 * exp(-currentBallRadius / 20.0);//foreshortening
	pz2 = EstimateDistance(prevBallRadius);// 4.0 * exp(-prevBallRadius / 20.0);//foreshortening


	//TODO change to intrinsic matrix
	//px1 = EstimateCameraCoord(x1, pz1);// x1 / focus*pz1;
	//px2 = EstimateCameraCoord(x2, pz2);//x2 / focus*pz2;
	//py1 = EstimateCameraCoord(y1, pz1);//y1 / focus*pz1;
	//py2 = EstimateCameraCoord(y2, pz2);//y2 / focus*pz2;



	Vz2 = pow((float)(pz1 - pz2) / dt12, 2);	//  velocity through principal axes
	Vx2 = (float)pow((float)(px1 - px2) / dt12, 2);	// x component velocity (pixel/sec)
	Vy2 = (float)pow((float)(py1 - py2) / dt12, 2);	// y component velocity (pixel/sec)
	Vxz = factor1 *sqrt(Vx2 + Vz2) ;				// camera planar velocity, the only one we can measured

	Vx = sqrt(Vx2) ;
	Vy = sqrt(Vy2) ;
	Vz = sqrt(Vz2) ;//negative for moving through camera
	V = sqrt(Vx2 + Vy2 + Vz2) ;

	m_verticalAngle = RAD2DEG*acos(Vxz / V);
	m_horizontalAngle = RAD2DEG*atan(Vx / Vz);
	verticalAngle = m_verticalAngle;
	horizontalAngle = m_horizontalAngle;

	return ;

}
//---------------------------CALCULATIONS------------------------------------


//Calculates air resistance for a given moment in time. This must be called in set intervals
//of time to continually update changing air resistance. 

//The drag force is proportional to the ball speed squared and is given by the formula
//F = Cd A d v2/2
void Dynamics::calculateAirResistance()
{
	airResistance = (1 / 2.0) * airDensity * pow(currentAirSpeed, 2) * area * dragCoefficient;
	// if v = 10 m/s then F = 0.114 N. 
	//If v = 20 m/s then F = 0.456 N.
	//If v = 30 m/s then F = 1.026 N.
}

//Speed is recalculated every PHYSICS_REFRESH_RATE milliseconds, and the calculated air resistance 
//can be modeled to act on the object for that period of time, allowing for the change 
//in speed to be calculated. 
void Dynamics::calculateCurrentSpeeds()
{
	calculateAirResistance();

	angle = atan2(currentVerticalSpeed, currentHorizontalSpeed);

	currentHorizontalSpeed -= ((airResistance * cos(angle)) / mass)
		* PHYSICS_REFRESH_RATE;
	currentVerticalSpeed -= (gravity + ((airResistance * sin(angle)) / mass))
		* PHYSICS_REFRESH_RATE;
	currentAirSpeed = hypot(currentHorizontalSpeed, currentVerticalSpeed);

}

//Using the calculated speeds we can just multiply by the time passed or PHYSICS_REFRESH_RATE
//to find where the object is at the new time. 
void Dynamics::calculateCurrentPositions()
{
	calculateCurrentSpeeds();

	z.insert(z.end(), distance[timeInterval - 1] * cos(m_horizontalAngle* DEG2RAD));
	x.insert(x.end(), distance[timeInterval - 1] * sin(m_horizontalAngle* DEG2RAD));

	height.insert(height.end(),height[timeInterval - 1] + (currentVerticalSpeed * PHYSICS_REFRESH_RATE));
	distance.insert(distance.end(),distance[timeInterval - 1] + (currentHorizontalSpeed * PHYSICS_REFRESH_RATE));



	time += PHYSICS_REFRESH_RATE;

}

bool Dynamics::onlyCalculateIdealPath()
{
	return (mass <= 0 || dragCoefficient <= 0 || area <= 0 || airDensity <= 0);
}

void Dynamics::SetIdealPath()
{
	mass = 0;
}

//Runs simulation to place appropriate values in array. 
void Dynamics::runSimulation()
{
	/*idealHeight.insert(idealHeight.end(), initialHeight);
	idealDistance.insert(idealDistance.end(),0.0);
	idealX.insert(idealX.end(), idealDistance[timeInterval - 1] * cos(m_horizontalAngle* DEG2RAD));
	idealZ.insert(idealZ.end(), idealDistance[timeInterval - 1] * sin(m_horizontalAngle* DEG2RAD));*/
	//timeInterval++;

	//Calculates ideal path. 
	do
	{

		idealTime += PHYSICS_REFRESH_RATE;

		/*currentVerticalSpeed = initialSpeed * sin(angle) - gravity * idealTime;
		currentHorizontalSpeed = initialSpeed * cos(angle);*/

		idealHeight.insert(idealHeight.end(),(-(gravity / 2.0) * pow(idealTime, 2) + (initialSpeed * sin(angle)
			* idealTime)) + m_initialHeight);
		idealDistance.insert(idealDistance.end(),(idealDistance[timeInterval - 1] + (initialSpeed * cos(angle))
			* PHYSICS_REFRESH_RATE));

		idealZ.insert(idealZ.end(), idealDistance[timeInterval - 1] * cos(m_horizontalAngle* DEG2RAD));
		idealX.insert(idealX.end(), idealDistance[timeInterval - 1] * sin(m_horizontalAngle* DEG2RAD));

		//logfile << timeInterval << "   " << idealDistance[timeInterval] << "   " << idealHeight[timeInterval] << "   " << currentHorizontalSpeed << "   " <<currentVerticalSpeed<<  endl ; 

		timeInterval++;

	} while (idealHeight[timeInterval - 1] > 0);//> idealHeight[0]

	timeInterval = 1;

	if (onlyCalculateIdealPath())
	{
		height = idealHeight;
		distance = idealDistance;
		time = idealTime;

	}
	else
	{
		do
		{
			calculateCurrentPositions();


			logfile << timeInterval << "   " << distance[timeInterval] << "   " << height[timeInterval] << "   " << currentHorizontalSpeed << "   " <<currentVerticalSpeed<< "   " << x[timeInterval] << "   " << z[timeInterval]<<  endl ; 

			if(height[timeInterval] < 0)
				this->m_timeToFlyBy = timeInterval*PHYSICS_REFRESH_RATE;

			if(distance[timeInterval] > 1.5*distance[0])
				this->m_timeToImpact = timeInterval*PHYSICS_REFRESH_RATE;

			timeInterval++;

		} while (height[timeInterval - 1] > 0); //> height[0]

	}




	idealTime -= 0.01;
	time -= 0.01;

	logfile.close();                                       // explicit close

}
