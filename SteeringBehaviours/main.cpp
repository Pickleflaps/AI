#include "SteeringBehavioursApp.h"

int main() {
	
	// allocation
	auto app = new SteeringBehavioursApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}