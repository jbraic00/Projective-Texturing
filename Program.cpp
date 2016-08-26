#include "pch.h"

#include "Renderer3D.h"

#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Scene\Input\Input Controller.h"

using namespace Platform;
using namespace Windows::ApplicationModel::Core;

using namespace vxe;
using namespace std;

// The main function is only used to initialize our IFrameworkView class.
[MTAThread]
auto main(Array<String ^>^)->int
{
	DebugPrint(std::string("main() started ...\n"));

	InputController^ inputcontroller = ref new InputController();
	shared_ptr<RendererBase3D> renderer3D = make_shared<Renderer3D>(inputcontroller);

	auto factory = ref new ApplicationFactory(renderer3D);
	CoreApplication::Run(factory);

	return 0;
}