// 3DGameTemplate.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../Headers/Framework.h"

int main()
{
    Framework* frameWork = new Framework();
    frameWork->Loop();
    delete frameWork;

    return 0;
}