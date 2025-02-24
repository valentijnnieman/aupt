#pragma once
#include "RtMidi.h"

typedef struct {
  std::vector<float> samples;
} callback_data_s;

class MidiController {
public:
  RtMidiIn *midiIn = 0;
  RtMidiOut *midiOut = 0;

  std::vector<std::string> portNames;

  static void callback(double deltatime, std::vector<unsigned char> *message,
                       void *userData) {
    int key = (int)message->at(1);
    int value = (int)message->at(0);

    callback_data_s *data = static_cast<callback_data_s *>(userData);
    if (value >= 144 && value <= 159) {
      // handle note events
    }
    if (value >= 128 && value <= 143) {
      // handle note off events
    }
  }

  MidiController() {
    try {
      midiIn = new RtMidiIn();
    } catch (RtMidiError &error) {
      std::cout << "Couldn't initialize RtMidiIn: " << std::endl;
      error.printMessage();
    }

    unsigned int nPorts = midiIn->getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";

    std::string portName;

    for (int i = 0; i < nPorts; i++) {
      try {
        portName = midiIn->getPortName(i);
      } catch (RtMidiError &error) {
        error.printMessage();
      }
      std::cout << "  Input Port #" << i + 1 << ": " << portName << '\n';
    }

    try {
      midiOut = new RtMidiOut();
    } catch (RtMidiError &error) {
      error.printMessage();
    }

    nPorts = midiOut->getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
    for (unsigned int i = 0; i < nPorts; i++) {
      try {
        portName = midiOut->getPortName(i);
        portNames.push_back(portName);
      } catch (RtMidiError &error) {
        error.printMessage();
      }
      std::cout << "  Output Port #" << i + 1 << ": " << portName << '\n';
    }
    std::cout << '\n';
  }

  ~MidiController() {
    delete midiIn;
    delete midiOut;
  }
};
