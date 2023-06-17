module;
#include <GL/freeglut.h>

module Client.IntroScene;
import Client.Framework;

void IntroScene::Awake()
{
	basis::Awake();

	int argc = 0;
	glutInit(&argc, nullptr); // glut ÃÊ±âÈ­

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);

	const int handle = glutCreateWindow("Practice");
	Framework::Instance->SetWindowID(handle);
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
