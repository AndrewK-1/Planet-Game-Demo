#pragma once
#include "pch.h"
#include "World.h"
#include <random>

using namespace DirectX;

World::World() 
{
	XMVECTOR pos;
	XMVECTOR rot;
	XMVECTOR scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
}