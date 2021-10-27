#include "pch.h"
#include "Vuforia.h"
#include "VuforiaEngine/VuforiaEngine.h"


using namespace TestApp;

void VuforiaTest::setupVuforia() {
	VuEngine* engine = NULL;

	// Create an engine instance using the default parameters 
	vuEngineCreate(&engine, NULL, NULL);

	// Start the engine 
	vuEngineStart(engine);

	// Stop the engine 
	vuEngineStop(engine);

	// Destroy the engine 
	vuEngineDestroy(engine);
}
