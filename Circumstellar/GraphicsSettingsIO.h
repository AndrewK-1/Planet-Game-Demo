#pragma once
#include "pch.h"

class GraphicsSettingsIO {
public:
	bool GetSetting(std::string fileName, std::string settingType, std::string& stringOutput);
	void SetSetting(std::string fileName, std::string settingType, std::string settingValue);
	bool GetSettingString(std::string fileName, std::string settingType, std::string& lineString, int& lineCount);
	bool GetSettingString(std::string fileName, std::string settingType, std::string& lineString);
};