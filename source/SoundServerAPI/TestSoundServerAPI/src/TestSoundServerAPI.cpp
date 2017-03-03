// TestSoundServerAPI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <sspSoundServerAPI.h>
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
  sspSoundServerAPI myAPI;
  myAPI.connect("192.168.0.100", 4445);
  Sleep(2000);

  sspSoundID myID1;
  myAPI.loadSound("Gitartone.wav", &myID1);
  sspSoundID myID2;
  myAPI.loadSound("Gitartone.wav", &myID2);
  sspSoundID myID3;
  myAPI.loadSound("Gitartone.wav", &myID3);
  sspSoundID myID4;
  myAPI.loadSound("Gitartone.wav", &myID4);
  sspSoundID myID5;
  myAPI.loadSound("Gitartone.wav", &myID5);
  sspSoundID myID6;
  myAPI.loadSound("Gitartone.wav", &myID6);
  Sleep(2000);

  if (myAPI.play(myID1, 7.0f, false) != sspSoundServerAPI::OK)
    cout << "Failed to play: " << myID1 << endl;
  Sleep(2000);
  if (myAPI.play(myID2, 7.0f, false) != sspSoundServerAPI::OK)
    cout << "Failed to play: " << myID2 << endl;
  Sleep(2000);
  if (myAPI.play(myID3, 7.0f, false) != sspSoundServerAPI::OK)
    cout << "Failed to play: " << myID3 << endl;
  Sleep(2000);
  if (myAPI.play(myID4, 7.0f, false) != sspSoundServerAPI::OK)
    cout << "Failed to play: " << myID4 << endl;
  Sleep(2000);
  if (myAPI.play(myID5, 7.0f, false) != sspSoundServerAPI::OK)
    cout << "Failed to play: " << myID5 << endl;
  Sleep(2000);
  if (myAPI.play(myID6, 7.0f, false) != sspSoundServerAPI::OK)
    cout << "Failed to play: " << myID6 << endl;
  Sleep(2000);
  if (myAPI.play(myID1, 7.0f, false) != sspSoundServerAPI::OK)
    cout << "Failed to play: " << myID1 << endl;
  Sleep(2000);
  if (myAPI.play(myID2, 7.0f, false) != sspSoundServerAPI::OK)
    cout << "Failed to play: " << myID2 << endl;
  Sleep(2000);
  if (myAPI.play(myID3, 7.0f, false) != sspSoundServerAPI::OK)
    cout << "Failed to play: " << myID3 << endl;
  Sleep(2000);
  if (myAPI.play(myID4, 7.0f, false) != sspSoundServerAPI::OK)
    cout << "Failed to play: " << myID4 << endl;
  Sleep(2000);
  if (myAPI.play(myID5, 7.0f, false) != sspSoundServerAPI::OK)
    cout << "Failed to play: " << myID5 << endl;
  Sleep(2000);
  if (myAPI.play(myID6, 7.0f, false) != sspSoundServerAPI::OK)
    cout << "Failed to play: " << myID6 << endl;
  Sleep(2000);

  sspSoundID myID7;
  myAPI.loadSound("KK13_polskmessing.wav", &myID7);
  Sleep(1000);
  myAPI.play(myID7, -1.0f, true);
  Sleep(1000);
  myAPI.setPosition(myID7, 1.0f, 1.0f, 1.0f);
  Sleep(1000);
  myAPI.movePosition(myID7, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 3.0f);
  Sleep(4000);
  myAPI.setVolume(myID7, 0.2f);
  Sleep(3000);
  myAPI.changeVolume(myID7, 0.2f, 1.0f, 3.0f);
  Sleep(5000);
  myAPI.stop(myID7);
  Sleep(2000);
  myAPI.disconnect();

  return 0;
}

