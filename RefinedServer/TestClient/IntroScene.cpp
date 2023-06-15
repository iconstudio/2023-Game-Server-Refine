module;
#include <GL/freeglut.h>
module Client.IntroScene;

void IntroScene::Awake()
{
	basis::Awake();

	int argc = 0;
	glutInit(&argc, nullptr); // glut √ ±‚»≠

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Practice");
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
