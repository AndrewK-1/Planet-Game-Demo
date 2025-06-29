#pragma once
#include "pch.h"
#include "World.h"
#include <random>

using namespace DirectX;

World::World() 
{
	std::mt19937 genAlgorithm(static_cast<unsigned int>(std::time(nullptr)));
	std::uniform_real_distribution<float> distribution(-10.0f, 10.0f);
	XMVECTOR pos;
	XMVECTOR rot;
	XMVECTOR scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 4; i++) {
		pos = XMVectorSet(distribution(genAlgorithm), distribution(genAlgorithm), distribution(genAlgorithm), distribution(genAlgorithm));
		rot = XMVectorSet(distribution(genAlgorithm), distribution(genAlgorithm), distribution(genAlgorithm), distribution(genAlgorithm));
		WorldRenderedObject rendObj(pos, rot, scale);
		m_worldObjects.push_back(rendObj);
		std::wstring msg = L"Put world object ";
		msg += std::to_wstring(i);
		msg += L" into world vector array.\n";
		OutputDebugString(msg.c_str());
	}
}