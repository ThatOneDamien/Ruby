#include <Ruby.h>

class Sandbox : public Ruby::App
{
public:
	Sandbox()
	{

	}

	virtual ~Sandbox()
	{

	}
};

int main(int argc, char** argv)
{
	Ruby::Logger::init();
	Ruby::Time::init();

	Ruby::App* app = new Sandbox();

	app->run();

	delete app;

}

