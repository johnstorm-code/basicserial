#include "SerialComm.h"

int main(int argc, char **argv[])
{
	// New Serial Object
	SerialComm MySerialComm;

	// Enumerate ALL Ports if available, else can't do anything
	if (!MySerialComm.enumCommPorts())
	{
		wcout << "No devices found." << endl;
		return 0;
	}

	// Port#s may change, collect it
	// TODO: cmd line args?
	wcout << "Select Port: ";
	const UINT syze = 4;
	TCHAR p[syze];
	const UINT8 len = 11;
	TCHAR port[len];
	wcin.getline(p, syze * sizeof(TCHAR));
	StringCbPrintf(port, len * sizeof(TCHAR), L"//./COM%s", p);

	// Open Specific Port
	MySerialComm.openCommPort(port);

	// Set Settings
	// rates & timeouts
	MySerialComm.setCommState(L"baud=9600 parity=N data=8 stop=1");
	MySerialComm.setTimeouts(0,0,0,0,0);
	
	// Info
	MySerialComm.dumpCommState();
	
	// Transmit/Recieve
	BYTE bytesend = 0;
	BYTE byterecv = 0;
	wcout << L"== Loopback mode ==" << endl;
	do {
		bytesend = _getch();
		MySerialComm.writeByte(bytesend);
		MySerialComm.readByte(byterecv);
		/*if (byterecv == '\r') {
			wcout << (char)'\n';
		}*/
		wcout << (char)byterecv;
	} while (true);
}