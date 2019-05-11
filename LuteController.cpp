// LuteController.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <string>
#include "RtMidi.h"
#include <windows.h>

constexpr auto VK_E = 0x45;
constexpr auto VK_Q = 0x51;
constexpr auto VK_U = 0x55;
constexpr auto VK_I = 0x49;
constexpr auto VK_P = 0x50;
constexpr auto VK_M = 0x4D;
constexpr auto VK_N = 0x4E;
constexpr auto VK_T = 0x54;

constexpr auto VK_C = 0x43;
constexpr auto VK_O = 0x4F;
constexpr auto VK_A = 0x41;
constexpr auto VK_D = 0x44;


constexpr auto VK_0 = 0x30;
constexpr auto VK_1 = 0x31;
constexpr auto VK_2 = 0x32;
constexpr auto VK_3 = 0x33;
constexpr auto VK_4 = 0x34;
constexpr auto VK_5 = 0x35;
constexpr auto VK_6 = 0x36;
constexpr auto VK_7 = 0x37;
constexpr auto VK_8 = 0x38;
constexpr auto VK_9 = 0x39;

class LuteController {
public:
	void play(std::vector<unsigned char>* message) {
		int id = (int)message->at(0);
		int pitch = (int)message->at(1);
		int value = (int)message->at(2);

		int lute_note = pitch - 49;

		if (id == _velocity_id && value > 0) {
			if (_lute_min <= lute_note && lute_note <= _lute_max) {
				// handle command

				// slow down sending if commands are being sent too fast
				using namespace std::chrono;

				auto curr_time = high_resolution_clock::now();
				auto time_span = duration_cast<milliseconds>(curr_time - _prev_send).count();
				auto diff = _min_wait - time_span;
				
				if (diff > 0) {
					Sleep(DWORD(diff));
				}

				_sendLuteCommand(std::to_string(lute_note));
				_prev_send = high_resolution_clock::now();
			}
		}
	}

private:
	const int _lute_max = 23;
	const int _lute_min = 0;

	const int _velocity_id = 144;
	const int _pressure_id = 160;
	const int _bend_id = 224;


	// helpers for handling midi data
	// time in milliseconds before sending another note
	int _min_wait = 20;
	std::chrono::high_resolution_clock::time_point _prev_send = std::chrono::high_resolution_clock::now();

	const char _num_key_list[10] = { VK_0, VK_1, VK_2, VK_3, VK_4, VK_5, VK_6, VK_7, VK_8, VK_9 };

	void _tapKey(char key_code) {
		keybd_event(key_code, KEYEVENTF_SCANCODE, 0, 0);
		keybd_event(key_code, KEYEVENTF_SCANCODE, KEYEVENTF_KEYUP, 0);
	}

	void _sendLuteCommand(std::string command) {

		keybd_event(VK_SHIFT, KEYEVENTF_SCANCODE, 0, 0);
		_tapKey(VK_E);
		keybd_event(VK_SHIFT, KEYEVENTF_SCANCODE, KEYEVENTF_KEYUP, 0);
		_tapKey(VK_Q);
		_tapKey(VK_U);
		_tapKey(VK_I);
		_tapKey(VK_P);
		_tapKey(VK_M);
		_tapKey(VK_E);
		_tapKey(VK_N);
		_tapKey(VK_T);
		keybd_event(VK_SHIFT, KEYEVENTF_SCANCODE, 0, 0);
		_tapKey(VK_C);
		keybd_event(VK_SHIFT, KEYEVENTF_SCANCODE, KEYEVENTF_KEYUP, 0);
		_tapKey(VK_O);
		_tapKey(VK_M);
		_tapKey(VK_M);
		_tapKey(VK_A);
		_tapKey(VK_N);
		_tapKey(VK_D);

		_tapKey(VK_SPACE);

		for (char&c : command) {
			unsigned int index = c - '0';
			_tapKey(_num_key_list[index]);
		}

		_tapKey(VK_RETURN);	
	}

};


void callback(double deltatime, std::vector< unsigned char > *message, void *userData)
{
	// uncomment the commented lines for perf test
	//using namespace std::chrono;
	//auto start_time = high_resolution_clock::now();
	unsigned int nBytes = message->size();
	if (nBytes == 3) {
		LuteController* controller = (LuteController*)userData;
		controller->play(message);
	}
	//auto end_time = high_resolution_clock::now();

	//auto delta = duration_cast<milliseconds>(end_time - start_time).count();
	//std::cout << "Processing time: " << delta << std::endl;
}

int main()
{
	int port_number = 0;
	// create instance of lute controller
	LuteController controller;


	RtMidiIn *midi_in = new RtMidiIn();
	// Check available ports.
	unsigned int nPorts = midi_in->getPortCount();
	if (nPorts == 0) {
		std::cout << "ERROR: No input ports available, SHUTTING DOWN!\n";
		goto cleanup;
	}

	// open the first port and register callback
	// don't ignore fancy types
	midi_in->openPort(port_number);
	midi_in->setCallback(&callback, &controller);
	midi_in->ignoreTypes(false, false, false);

	std::cout << "\nConnected to MIDI input device: " << midi_in->getPortName(port_number) << std::endl;
	std::cout << "Reading MIDI input, press <enter> to quit." << std::endl;
	char input;
	std::cin.get(input);
	std::cout << "Shutting down.\n" << std::endl;
	// Clean up
cleanup:
	delete midi_in;
	return 0;
}
