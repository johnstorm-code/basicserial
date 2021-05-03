#include "SerialComm.h"

SerialComm::SerialComm()
{
	hCommPort = NULL;
	memset(&dcb, 0, sizeof(dcb));
}
SerialComm::SerialComm(LPCWSTR commPort)
{
	hCommPort = NULL;
	memset(&dcb, 0, sizeof(dcb));
	openCommPort(commPort);
}
BOOL SerialComm::initialize(LPCWSTR commPort)
{
	if (hCommPort != NULL) {
		CloseHandle(hCommPort);
	}
	hCommPort = Port(commPort);
	if (hCommPort == INVALID_HANDLE_VALUE)
	{
		wcout << "ERROR: INVALID_HANDLE_VALUE, CODE: " << GetLastError() << endl;
		exit(0);
	}
	return TRUE;
}
HANDLE SerialComm::Port(LPCWSTR commPort)
{
	LPCWSTR lpFileName = commPort;
	DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
	DWORD dwShareMode = 0;
	LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL;
	DWORD dwCreationDisposition = OPEN_EXISTING;
	DWORD dwFlagsAndAttributes = 0;
	HANDLE hTemplateFile = NULL;
	return CreateFile
	(
		lpFileName,
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile
	);
}
SerialComm::~SerialComm()
{
	if (hCommPort != NULL)
	{
		CloseHandle(hCommPort);
		hCommPort = NULL;
	}
}
BOOL SerialComm::setCommState(LPCWSTR state)
{
	BOOL nonZero = 1;
	if (getCommState() == nonZero)
	{
		if (BuildCommDCB(state, &dcb) != nonZero)
		{
			return FALSE;
		}
		if (SetCommState(hCommPort, &dcb) != nonZero)
		{
			return FALSE;
		}
	}
	return TRUE;
}
BOOL SerialComm::getCommState()
{
	BOOL nonZero = GetCommState(hCommPort, &dcb);
	if (!nonZero) return FALSE;
	return TRUE;
}
void SerialComm::dumpCommState()
{
	getCommState();
	wcout << L"DCBlength: " << dcb.DCBlength << endl;
	wcout << L"BaudRate: " << dcb.BaudRate << endl;
	wcout << L"fBinary: " << dcb.fBinary << endl;
	wcout << L"fParity: " << dcb.fParity << endl;
	wcout << L"fOutxCtsFlow: " << dcb.fOutxCtsFlow << endl;
	wcout << L"fOutxDsrFlow: " << dcb.fOutxDsrFlow << endl;
	wcout << L"fDtrControl: " << dcb.fDtrControl << endl;
	wcout << L"fDsrSensitivity: " << dcb.fDsrSensitivity << endl;
	wcout << L"fTXContinueOnXoff: " << dcb.fTXContinueOnXoff << endl;
	wcout << L"fOutX: " << dcb.fOutX << endl;
	wcout << L"fInX: " << dcb.fInX << endl;
	wcout << L"fErrorChar: " << dcb.fErrorChar << endl;
	wcout << L"fNull: " << dcb.fNull << endl;
	wcout << L"fRtsControl: " << dcb.fRtsControl << endl;
	wcout << L"fAbortOnError: " << dcb.fAbortOnError << endl;
	wcout << L"fDummy2: " << dcb.fDummy2 << endl;
	wcout << L"XonLim: " << dcb.XonLim << endl;
	wcout << L"XoffLim: " << dcb.XoffLim << endl;
	wcout << L"ByteSize: " << dcb.ByteSize << endl;
	wcout << L"Parity: " << dcb.Parity << endl;
	wcout << L"StopBits: " << dcb.StopBits << endl;
	wcout << L"XonChar: " << dcb.XonChar << endl;
	wcout << L"XoffChar: " << dcb.XoffChar << endl;
	wcout << L"ErrorChar: " << dcb.ErrorChar << endl;
	wcout << L"EofChar: " << dcb.EofChar << endl;
	wcout << L"EvtChar: " << dcb.EvtChar << endl;
}
DWORD SerialComm::writeByte(const BYTE byte)
{
	DWORD bytesWritten = 0;
	BOOL nonZero = WriteFile(hCommPort, &byte, 1, &bytesWritten, NULL);
	if (!nonZero) return 0;
	return bytesWritten;
}
DWORD SerialComm::writeBytes(const BYTE* bytes, UINT16 syze)
{
	DWORD bytesWritten = 0;
	BOOL nonZero = WriteFile(hCommPort, &bytes, syze, &bytesWritten, NULL);
	if (!nonZero) return 0;
	return bytesWritten;
}
DWORD SerialComm::readByte(BYTE& byte)
{
	DWORD bytesRead = 0;
	BOOL nonZero = ReadFile(hCommPort, &byte, 1, &bytesRead, NULL);
	if (!nonZero) return 0;
	return bytesRead;
}
DWORD SerialComm::readBytes(BYTE* bytes, UINT16 syze)
{
	DWORD bytesRead = 0;
	BOOL nonZero = ReadFile(hCommPort, &bytes, syze, &bytesRead, NULL);
	if (!nonZero) return 0;
	return bytesRead;
}
void SerialComm::setTimeouts(DWORD rit, DWORD rtm, DWORD rtc, DWORD wtm, DWORD wtc)
{
	// https://docs.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-commtimeouts
	COMMTIMEOUTS timeout;
	timeout.ReadIntervalTimeout = rit;
	timeout.ReadTotalTimeoutMultiplier = rtm;
	timeout.ReadTotalTimeoutConstant = rtc;
	timeout.WriteTotalTimeoutMultiplier = wtm;
	timeout.WriteTotalTimeoutConstant = wtc;
	SetCommTimeouts(hCommPort, &timeout);
}
BOOL SerialComm::openCommPort(LPCWSTR commPort)
{
	return initialize(commPort);
}
bool SerialComm::enumCommPorts()
{
	const UINT16 len = 11;
	TCHAR comm[len];
	HANDLE port;
	UINT8 next = 0;
	for (register short i = 0; i < MAXUINT8; i++)
	{
		StringCbPrintf(comm, len * sizeof(TCHAR), L"//./COM%d", i);
		if ((port = Port(comm)) != INVALID_HANDLE_VALUE)
		{
			wcout << "Found: " << "Port# " << i << " " << comm << endl;
			CloseHandle(port);
			return true;
		}
	}
	return false;
}