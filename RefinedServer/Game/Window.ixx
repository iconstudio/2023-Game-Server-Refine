module;
#include <GL/freeglut.h>
export module Game.Window;

export namespace game
{
	class Window
	{
	public:
		void Awake()
		{
			int window = ::glutGetWindow();
		}
	};
}
