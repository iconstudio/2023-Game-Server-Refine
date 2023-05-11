#include "pch.hpp"
#include "Framework.hpp"

Framework* myFramework;

int main()
{
	myFramework = new Framework{};

	myFramework->Awake();
	myFramework->Start();
	myFramework->Update();
}