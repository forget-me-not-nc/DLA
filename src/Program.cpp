

////////////////////////////////////////////////////////////
//	Headers
////////////////////////////////////////////////////////////
#include "Program.h"

////////////////////////////////////////////////////////////
/// Constructor
/// 
////////////////////////////////////////////////////////////
Program::Program()
{
	this->initWindow();

	CircleShape tempCircle;
	tempCircle.setRadius(4.0f);
	tempCircle.setOrigin(tempCircle.getRadius(), tempCircle.getRadius());
	tempCircle.setFillColor(sf::Color::White);

	for (int i = 0; i < 1000; i++) //spawn only 1000 circles to decrease CPU load
	{
		tempCircle.setPosition(
			static_cast<float>(rand() % this->renderWindow->getSize().x),
			static_cast<float>(rand() % this->renderWindow->getSize().y));

		this->circles.push_back(CircleShape(tempCircle));

	}
}

////////////////////////////////////////////////////////////
/// Destructor
/// 
////////////////////////////////////////////////////////////
Program::~Program()
{
	delete this->renderWindow;
}

bool Program::isRunning()
{
	return this->renderWindow->isOpen();
}

////////////////////////////////////////////////////////////
/// Programs main update func
/// 
////////////////////////////////////////////////////////////
void Program::onUpdate()
{
	this->eventHandler();
	this->moveCircles();
	this->validateCirclePosition();
	this->checkForCollision();
}

////////////////////////////////////////////////////////////
///	Programs main render func
/// 
////////////////////////////////////////////////////////////
void Program::onDraw()
{
	this->renderWindow->clear();

	for (unsigned iter = 0; iter < this->circles.size(); iter++)
	{
		this->renderWindow->draw(this->circles[iter]);
	}

	for (unsigned iter = 0; iter < this->staticCircles.size(); iter++)
	{
		this->renderWindow->draw(this->staticCircles[iter]);
	}

	this->renderWindow->display();
}

////////////////////////////////////////////////////////////
///	Initialize startup window
/// 
////////////////////////////////////////////////////////////
void Program::initWindow()
{
	this->videoMode = VideoMode(600, 400);

	this->renderWindow = new RenderWindow(this->videoMode, "Aggregation", sf::Style::Titlebar | sf::Style::Close);

	this->renderWindow->setVerticalSyncEnabled(true);
}

////////////////////////////////////////////////////////////
///	Func to handle all events
/// 
////////////////////////////////////////////////////////////
void Program::eventHandler()
{
	if (this->renderWindow->pollEvent(this->event))
	{
		switch (this->event.type)
		{
			case sf::Event::Closed:
			{
				std::cout << "Closing window...\n" << std::endl;

				this->renderWindow->close();

				break;
			}

			case sf::Event::MouseButtonPressed:
			{
				Vector2i mousePosition = sf::Mouse::getPosition(*this->renderWindow);

				CircleShape tempCircle;
				tempCircle.setRadius(5.0f);
				tempCircle.setOrigin(tempCircle.getRadius(), tempCircle.getRadius());
				tempCircle.setFillColor(sf::Color::Green);
				tempCircle.setPosition(
					static_cast<float>(mousePosition.x),
					static_cast<float>(mousePosition.y));

				this->staticCircles.push_back(tempCircle);

				break;
			}

			default: break;
		}
	}
}

////////////////////////////////////////////////////////////
///	Move circle by random offset
/// 
////////////////////////////////////////////////////////////
void Program::moveCircles()
{
	for (unsigned iter = 0; iter < this->circles.size(); iter++)
	{
		this->circles[iter].move(
			static_cast<float>(rand() % 9 - 4),
			static_cast<float>(rand() % 9 - 4));
	}
}

////////////////////////////////////////////////////////////
///	Do not let circle move out of window 
/// 
////////////////////////////////////////////////////////////
void Program::validateCirclePosition()
{
	for (unsigned iter = 0; iter < this->circles.size(); iter++)
	{
		if (this->circles[iter].getPosition().x < 0)
		{
			this->circles[iter].setPosition(0.0f, this->circles[iter].getPosition().y);
		}
		else if (this->circles[iter].getPosition().x > this->renderWindow->getSize().x)
		{
			this->circles[iter].setPosition(
				static_cast<float>(this->renderWindow->getSize().x),
				this->circles[iter].getPosition().y);
		}
			
		if (this->circles[iter].getPosition().y < 0)
		{
			this->circles[iter].setPosition(this->circles[iter].getPosition().x, 0.0f);
		}
		else if (this->circles[iter].getPosition().y > this->renderWindow->getSize().y)
		{
			this->circles[iter].setPosition(
				this->circles[iter].getPosition().x,
				static_cast<float>(this->renderWindow->getSize().y));
		}
	}
}

void Program::checkForCollision()
{
	if (!this->staticCircles.empty() && !this->circles.empty())
	{
		for (unsigned int i = 0; i < this->circles.size(); i++)
		{
			for (unsigned int j = 0; j < this->staticCircles.size(); j++)
			{
				if (areColliding(this->circles[i], this->staticCircles[j]))
				{
					this->circles[i].setFillColor(sf::Color::Green);

					this->staticCircles.push_back(CircleShape(this->circles[i]));

					this->circles.erase(this->circles.begin() + i);

					break;
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////
///	Compare distance according ro radiuses 
/// 
/// \return true if distance < sum(radius 1, radius 2)
////////////////////////////////////////////////////////////
bool Program::areColliding(CircleShape& object1, CircleShape& object2)
{
	float dx = object1.getPosition().x - object2.getPosition().x;
	float dy = object1.getPosition().y - object2.getPosition().y;

	return sqrt(dx * dx + dy * dy) <= object1.getRadius() + object2.getRadius();
}
