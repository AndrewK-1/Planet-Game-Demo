#pragma once
#include "pch.h"
#include "ControlSettingsIO.h"
#include <fstream>

using namespace std;

//Saves settings as an ID/value pair.  Therefore, every two bytes is a setting.

//Search for setting 
bool ControlSettingsIO::GetSetting(std::string fileName, UINT settingID, UINT& valueOutput) {
	ifstream inputStream(fileName, ios::in | ios::binary);
	UINT currentID, currentValue;

	while (inputStream.peek() != EOF) {
		inputStream.read(reinterpret_cast<char*>(&currentID), sizeof(UINT));
		inputStream.read(reinterpret_cast<char*>(&currentValue), sizeof(UINT));
		if (currentID == settingID) {
			OutputDebugString(L"ControlSettingsIO: Setting found.\n");
			valueOutput = currentValue;
			return 1;
		}
	}
	inputStream.close();
	valueOutput = 0;
	return 0;
}

void ControlSettingsIO::SetSetting(std::string fileName, UINT settingID, UINT settingValue) {
	UINT currentID, currentValue;
	int linePos, loopCount = 0;

	GetSettingPosition(fileName, settingID, linePos);
	//If not found, append setting to the end of the file.
	if (linePos == -1) {
		OutputDebugString(L"ControlSettingsIO: Setting not found, appending to end of file.\n");
		ofstream outputStream(fileName, ios::out | ios::binary | ios::app);
		outputStream.write(reinterpret_cast<char*>(&settingID), sizeof(UINT));
		outputStream.write(reinterpret_cast<char*>(&settingValue), sizeof(UINT));
		outputStream.close();
	}
	else {
		//Otherwise, overwrite value at the setting position.
		OutputDebugString(L"ControlSettingsIO: Setting found; overwriting.\n");
		std::wstring msg = L"ControlSettingsIO: Overwriting setting at position "; msg += std::to_wstring(linePos); msg += L"\n"; OutputDebugString(msg.c_str());
		ofstream editStream(fileName, ios::out | ios::in | ios::binary);
		editStream.seekp(sizeof(UINT) * 2 * linePos + sizeof(UINT));
		editStream.write(reinterpret_cast<char*>(&settingValue), sizeof(UINT));
		editStream.close();
	}
}

void ControlSettingsIO::GetSettingPosition(std::string fileName, UINT settingID, int& position) {
	ifstream inputStream(fileName, ios::in | ios::binary);
	UINT currentID;
	UINT unused;
	int linePos = -1;

	if (inputStream.is_open()) {
		OutputDebugString(L"ControlSettingsIO: File successfully opened.\n");
	}
	else {
		OutputDebugString(L"ControlSettingsIO: File failed to open.\n");
	}
	if (inputStream.tellg() < static_cast<std::streampos>(sizeof(UINT) * 2)) {
		OutputDebugString(L"ControlSettingsIO: File is empty or too small to contain settings.\n");
	}
	OutputDebugString(L"ControlSettingsIO: Searching for setting...\n  ");
	std::wstring msg = L"";
	while (inputStream.read(reinterpret_cast<char*>(&currentID), sizeof(UINT)) && inputStream.read(reinterpret_cast<char*>(&unused), sizeof(UINT))) {
		linePos++;
		msg = std::to_wstring(linePos) + L", ";
		OutputDebugString(msg.c_str());
		if (currentID == settingID) {
			position = linePos;
			return;
		}
	}
	position = -1;
	inputStream.close();
}