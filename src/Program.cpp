
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
	tempCircle.setRadius(7.0f);
	tempCircle.setOrigin(tempCircle.getRadius(), tempCircle.getRadius());
	tempCircle.setFillColor(sf::Color::White);

	for (int i = 0; i < 500; i++) //spawn only 500 circles to decrease CPU load
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

	for (vector<CircleShape>::iterator iter = this->circles.begin(); iter != this->circles.end();)
	{
		this->renderWindow->draw(*iter);

		++iter;
	}

	for (vector<CircleShape>::iterator iter = this->staticCircles.begin(); iter != this->staticCircles.end();)
	{
		this->renderWindow->draw(*iter);

		++iter;
	}

	this->renderWindow->display();
}

////////////////////////////////////////////////////////////
///	Initialize startup window
/// 
////////////////////////////////////////////////////////////
void Program::initWindow()
{
	this->videoMode = VideoMode(800, 600);

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
				tempCircle.setRadius(7.0f);
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
	for (vector<CircleShape>::iterator iter = circles.begin(); iter != circles.end();)
	{
		iter->move(
			static_cast<float>(rand() % 9 - 4),
			static_cast<float>(rand() % 9 - 4));

		++iter;
	}
}

////////////////////////////////////////////////////////////
///	Do not let circle move out of window 
/// 
////////////////////////////////////////////////////////////
void Program::validateCirclePosition()
{
	for (vector<CircleShape>::iterator iter = circles.begin(); iter != circles.end();)
	{
		if (iter->getPosition().x < 0)
		{
			iter->setPosition(0.0f, iter->getPosition().y);
		}
		else if (iter->getPosition().x > this->renderWindow->getSize().x)
		{
			iter->setPosition(
				static_cast<float>(this->renderWindow->getSize().x),
				iter->getPosition().y);
		}
			
		if (iter->getPosition().y < 0)
		{
			iter->setPosition(iter->getPosition().x, 0.0f);
		}
		else if (iter->getPosition().y > this->renderWindow->getSize().y)
		{
			iter->setPosition(
				iter->getPosition().x,
				static_cast<float>(this->renderWindow->getSize().y));
		}

		++iter;
	}
}

void Program::checkForCollision()
{
	if (!this->staticCircles.empty() && !this->circles.empty())
	{
		for (vector<CircleShape>::iterator i = this->circles.begin(); i != this->circles.end();)
		{
			bool isErased = false;

			for (vector<CircleShape>::iterator j = this->staticCircles.begin(); j < this->staticCircles.end(); ++j)
			{
				if (areColliding(*i, *j))
				{
					i->setFillColor(sf::Color::Green);

					this->staticCircles.push_back(CircleShape(*i));

					i = this->circles.erase(i);

					isErased = true;

					break;
				}
			}

			if (!isErased) ++i;
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
