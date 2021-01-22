#include <Raven.h>
#include "EditorLayer.h"

#if defined (_MSC_VER) && defined (DEBUG)
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#define DUMP_MEMORY_LEAKS()	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#else
	#define DUMP_MEMORY_LEAKS()
#endif

int main() {
	rvn::Application app = rvn::Application("Raven Editor");
	app.getLayerStack().pushLayer(new rvn::EditorLayer());
	app.run();
	DUMP_MEMORY_LEAKS();
	return 0;
}