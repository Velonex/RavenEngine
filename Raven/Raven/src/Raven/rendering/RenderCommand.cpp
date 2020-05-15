#include <pch.h>
#include "RenderCommand.h"
#include <Raven/platform/opengl/OpenGLRendererAPI.h>

namespace rvn {

	scope<RendererAPI> RenderCommand::s_API = createScope<OpenGLRendererAPI>();

}