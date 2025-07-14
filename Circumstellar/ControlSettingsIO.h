#pragma once
#include "pch.h"

class ControlSettingsIO {
public:
	bool GetSetting(std::string fileName, UINT settingType, UINT& valueOutput);
	void SetSetting(std::string fileName, UINT settingType, UINT settingValue);
	void GetSettingPosition(std::string fileName, UINT settingID, int& position);
};