#include "pch.hpp"
#include "Framework.hpp"
//#include "../Utility/Testbed.h"
import Utility.Datagram.Packer;

Framework* myFramework;

int main()
{
	myFramework = new Framework{};

	util::test::test_datapacker();

	myFramework->Awake();
	myFramework->Start();
	myFramework->Update();
}