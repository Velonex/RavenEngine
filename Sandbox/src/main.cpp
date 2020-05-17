#include <Raven.h>
#include <TestLayer.h>

#if defined (_MSC_VER) && defined (DEBUG)
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#define DUMP_MEMORY_LEAKS()	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#else
	#define DUMP_MEMORY_LEAKS()
#endif

int main() {
	rvn::Application app("Sandbox");
	app.getLayerStack().pushLayer(new TestLayer());
	app.run();
	DUMP_MEMORY_LEAKS();
	return 0;
}