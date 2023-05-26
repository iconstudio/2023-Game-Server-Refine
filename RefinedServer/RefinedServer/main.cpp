#include "pch.hpp"
#include "Framework.hpp"
//#include "../Utility/Testbed.h"

Framework* myFramework;

int main()
{
	myFramework = new Framework{};

	myFramework->Awake();
	myFramework->Start();
	myFramework->Update();
}