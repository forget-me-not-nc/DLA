#pragma once

////////////////////////////////////////////////////////////
//	Headers
////////////////////////////////////////////////////////////
#include "Includes.h"

/*
	Class that describes program logic
*/
class Program
{
public:
	//constr and destr
	Program();
	~Program();

	//public func
	bool isRunning();
	void onUpdate();	
	void onDraw();
private:
	//private variables
	RenderWindow* renderWindow;
	VideoMode videoMode;
	Event event;

	vector<CircleShape> circles;
	vector<CircleShape> staticCircles;

	//private func
	void initWindow();
	void eventHandler();
	void moveCircles();
	void validateCirclePosition();
	void checkForCollision();

	bool areColliding(CircleShape& object1, CircleShape& object2);
};

