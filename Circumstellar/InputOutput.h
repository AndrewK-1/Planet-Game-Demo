#pragma once
#include "pch.h"
#include "World.h"

class InputOutput {
public:
	InputOutput();
	//Creaating and retrieving world files.  Return 1 if successful, 0 if not.  Files are .cwd (Circumstellar World Data) for world files.
	bool ImportWorldInfo(std::wstring worldName, World* world);
	bool ExportWorldInfo(std::wstring worldName, World* world);
	bool DoesPlanetExist();
private:
	bool m_planetExists;
};