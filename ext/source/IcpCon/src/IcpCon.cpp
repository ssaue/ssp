// IcpCon.cpp : Defines the entry point for the console application.
//

#include <conio.h>
#include <iostream>
#include <windows.h>

// DCON include files
#include <i7000.h>
#include <uart.h>

using namespace std;

enum {BAUDRATE=9600L, DATABITS=8, STOPBIT=0, PARITY=0, ESC_KEY=27, TIMEOUT_DELAY=100};

void main (int argc, char* argv[])
{
  if (argc < 3) {
	  cout << endl << "Program usage: IcpCon <comport> <module> <option>";
    cout << endl << "    <port> = COM port (typically 1 or 2)";
    cout << endl << "    <module> = module address (0 - 255)";
    cout << endl << "    <option> = one of /a (Analog) /d (Digital)";
	  return;
  }

  unsigned char nPort = (unsigned char) atoi(argv[1]);
  short nModule = atoi(argv[2]);
  char cOption = argv[3][1];

  cout << endl << "ICP CON: COM" << (short) nPort << ", module " << nModule << ", option " << cOption << endl;

  char cInput;

  unsigned short w7000[8] = {nPort, nModule, 0, 0, TIMEOUT_DELAY, 0, 1, 0};
  char	szSend[20], szReceive[20];
  float f7000;

  if(Open_Com(nPort,BAUDRATE,DATABITS,PARITY,STOPBIT) != NoError) {
    cout << endl << "ICP CON: unable to open port COM" << nPort << endl;
    return;
  }

  do {
    switch (cOption) {
      case 'a': // Analog input
		    w7000[2]=0x7017;   // ID
		    for (short nChannel=0; nChannel<4; nChannel++) {
				  w7000[5] = nChannel;
				  if (AnalogIn(w7000, &f7000, szSend, szReceive) != NoError) {
				    cout << endl << "ICP CON: error reading channel " << nChannel << endl;
				  }
				  else {
				    cout << endl << "Analog " << nChannel << " " << f7000;
				    cout << "\t\t(sent " << szSend << ", received " << szReceive << ")";
				  }
			  }
        break;
      case 'd': // Digital input
  		  w7000[2]=0x7053;   // ID
			  if (DigitalIn(w7000, &f7000, szSend, szReceive) != NoError) {
				  cout << endl << "ICP CON: error digital input" << endl;
			  }
			  else {
				  cout << endl << "Digital bits " << w7000[5];
				  cout << "\t\t(sent " << szSend << ", received " << szReceive << ")";
			  }
        break;
      default:
        break;
    }    
    cout << endl << "Press a key to read again, or ESC to quit." << endl;
  	while (!_kbhit()){}
	  cInput = _getch();

  } while (cInput != ESC_KEY);

  if (Close_Com(nPort) != NoError) {
  	cout << endl << "ICP CON: unable to close port COM" << nPort << endl;
	}

  return;
}
