#include <GL/freeglut.h>
module Client.IntroScene;

void IntroScene::Awake()
{
	basis::Awake();
	myLoadTime = 0;
}

void IntroScene::Update(const float dt)
{
	basis::Update(dt);

	myLoadTime += dt;

	if (maxPeriod <= myLoadTime)
	{
		MakeCompletion();
	}
}

void IntroScene::Destroy()
{
	basis::Destroy();
	myLoadTime = 0;
}
