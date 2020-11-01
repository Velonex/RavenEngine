#include <Raven.h>
#include "EditorLayer.h"

int main() {
	rvn::Application app = rvn::Application("Raven Editor");
	app.getLayerStack().pushLayer(new EditorLayer());
	app.run();
	return 0;
}