#include <VEngine.h>
#include <VEngine/EntryPoint.h>

class Sandbox : public VEngine::Application 
{
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

VEngine::Application* VEngine::CreateApplication()
{
	return new Sandbox;
}