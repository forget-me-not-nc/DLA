////////////////////////////////////////////////////////////
//	Headers
////////////////////////////////////////////////////////////
#include "Program.h"

////////////////////////////////////////////////////////////
///	Entry point of application
///
/// \return Application exit code
////////////////////////////////////////////////////////////
int main()
{
	srand(static_cast<unsigned int>(time(NULL))); //for random

	Program program;

	while (program.isRunning())
	{
		program.onUpdate();

		std::this_thread::sleep_for(std::chrono::milliseconds(10)); //decrease CPU load

		program.onDraw();
	}

	return 0;
}