#pragma once

#include <Windows.h>
#include <strsafe.h>
#include <conio.h>
#include <iostream>
using namespace std;

class SerialComm
{
private:
	HANDLE hCommPort;
	DCB dcb;
	BOOL getCommState();
	BOOL initialize(LPCWSTR commPort);
	HANDLE Port(LPCWSTR commPort);
public:
	SerialComm();
	SerialComm(LPCWSTR commPort);//overload
	~SerialComm();
	BOOL setCommState(LPCWSTR state);
	void dumpCommState();
	DWORD writeByte(const BYTE byte);
	DWORD writeBytes(const BYTE* bytes, UINT16 syze);
	DWORD readByte(BYTE& byte);
	DWORD readBytes(BYTE* bytes, UINT16 syze);
	void setTimeouts(DWORD rit, DWORD rtm, DWORD rtc, DWORD wtm, DWORD wtc);
	BOOL openCommPort(LPCWSTR commPort);
	bool enumCommPorts();
};
