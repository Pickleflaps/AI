#include "AI_AssignmentApp.h"

int main() {
	
	// allocation
	auto app = new AI_AssignmentApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}