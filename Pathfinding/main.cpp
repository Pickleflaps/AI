#include "PathfindingApp.h"

int main() {
	
	// allocation
	auto app = new PathfindingApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app; // DO NOT MANUALLY DELETE "Font" or 2dRender etc << this will take care of it

	return 0;
}