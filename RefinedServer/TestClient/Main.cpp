#include "pch.hpp"
#include "Main.hpp"

Framework* framework = nullptr;

int main()
{
	framework = new Framework();
	framework->Awake();
	framework->Start();
	framework->Update();

	return 0;
}
