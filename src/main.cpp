#include "libs.h"
#include "vulkanApp.h"

int main()
{
	vulkanApp app;
	try
	{
		app.run();
		
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

//edited with VIM



//GLOBAL VARIABLES
const unsigned int WIDTH = 800;
const  unsigned int  HEIGHT = 600;

const std::vector <const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
#ifdef NDBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif