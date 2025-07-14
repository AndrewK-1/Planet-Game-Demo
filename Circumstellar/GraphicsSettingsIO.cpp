#pragma once
#include "pch.h"
#include "GraphicsSettingsIO.h"
#include <fstream>

using namespace std;

//Returns the setting at the line specified by the type.  Returns empty string if setting is empty.
bool GraphicsSettingsIO::GetSetting(string fileName, string settingType, string& stringOutput) {
	string line;
	size_t equalsPos;
	if (GetSettingString(fileName, settingType, line)) {
		equalsPos = line.find("=");
		stringOutput = line.substr(equalsPos + 1, string::npos);
		return 1;
	}
	else {
		OutputDebugString(L"GetSetting(): Setting not found.\n");
		stringOutput = "";
		return 0;
	}
}

//Sets the setting at the line specified by the type.
void GraphicsSettingsIO::SetSetting(string fileName, string settingType, string settingValue) {
	string line, fileContents = "", newLine = settingType + "=" + settingValue;
	int linePos;
	OutputDebugString(L"SetSetting(): Starting loop\n");
	
	if (GetSettingString(fileName, settingType, line, linePos)) {
		ifstream inputStream(fileName, ios::in);
		int count = 0;
		while (count < linePos) {
			count++;
			getline(inputStream, line);
			fileContents.append(line + "\n");
			OutputDebugString(L"SetSetting(): Reading lines for later input.\n");
		}
		fileContents.append(newLine + "\n");
		getline(inputStream, line);
		while (inputStream.peek() != EOF) {
			getline(inputStream, line);
			fileContents.append(line + "\n");
			OutputDebugString(L"SetSetting(): Reading lines after file has been input.\n");
		}
		inputStream.close();
		OutputDebugString(L"SetSetting(): Starting output.\n");
		//Output new file contents.  Deletes previous contents and outputs new contents.
		ofstream outputStream(fileName, ios::out | ios::trunc);
		outputStream << fileContents;
		outputStream.close();
	}
	else {
		OutputDebugString(L"SetSetting(): Line wasn't found, creating new setting.\n");
		//If the line was not found, append it to the end.
		ifstream inputStream(fileName, ios::in);
		if (inputStream.is_open()) {
			while (inputStream.peek() != EOF) {
				getline(inputStream, line);
				fileContents.append(line + "\n");
				OutputDebugString(L"SetSetting(): Getting existing lines.\n");
			}
		}
		inputStream.close();
		fileContents.append(newLine + "\n");

		OutputDebugString(L"SetSetting(): Writing to output.\n");
		ofstream outputStream(fileName, ios::out | ios::trunc);
		outputStream << fileContents;
		outputStream.close();
	}
}

//First string is the setting, second string will be modified to the found string, third setting is which line it's on.  Returns one if found, zero if not found.
bool GraphicsSettingsIO::GetSettingString(string fileName, string settingType, string& lineString, int& lineCount) {
	string type;
	ifstream inputStream(fileName, std::ios::in);
	size_t equalsPos;
	OutputDebugString(L"GetSettingString(): Getting the necessary string\n");

	lineCount = -1;
	if (inputStream.is_open()) {
		//While end of file hasn't been reached.
		while (!inputStream.eof()) {
			OutputDebugString(L"GetSettingString(): Checking for the setting\n");
			lineCount++;
			OutputDebugString(L"GetSettingString(): Getting line.\n");
			getline(inputStream, lineString);
			equalsPos = lineString.find("=");
			type = lineString.substr(0, equalsPos);
			//If found
			if (type == settingType) {
				OutputDebugString(L"GetSettingString(): Line has been found\n");
				inputStream.close();
				return 1;
			}
		}
	}
	else {
		OutputDebugString(L"GetSettingString(): File not found.\n");
	}
	
	inputStream.close();
	OutputDebugString(L"GetSettingString(): Setting not found.\n");
	lineString = "";
	return 0;
}

//First string is the setting, second string will be modified to the found string.  Returns one if found, zero if not found.
bool GraphicsSettingsIO::GetSettingString(string fileName, string settingType, string& lineString) {
	string type;
	ifstream inputStream(fileName, std::ios::in);
	size_t equalsPos;

	if (inputStream.is_open()) {
		OutputDebugString(L"GetSettingString(): Input stream is open.\n");
		//While end of file hasn't been reached.
		while (!inputStream.eof()) {
			OutputDebugString(L"GetSettingString(): Getting next line.\n");
			getline(inputStream, lineString);
			equalsPos = lineString.find("=");
			type = lineString.substr(0, equalsPos);

			std::wstring msg = L"GetSettingString(): Next type: ";
			std::wstring msg1(type.begin(), type.end());
			msg += msg1; msg += L"\n";
			OutputDebugString(msg.c_str());

			//If found
			if (type == settingType) {
				inputStream.close();
				OutputDebugString(L"GetSettingString(): Line has been found\n");
				return 1;
			}
		}
	}
	else {
		OutputDebugString(L"GetSettingString(): File not found.\n");
	}

	inputStream.close();
	OutputDebugString(L"GetSettingString(): Setting not found.\n");
	lineString = "";
	return 0;
}