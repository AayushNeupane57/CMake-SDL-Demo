#include <memory>
#include "app_platform.h"
#include "demo_app.h"

int main(int argc, char** argv)
{
	return RunApp(std::make_shared<DemoApp>());
}
