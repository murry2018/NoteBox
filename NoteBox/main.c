#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>

/*  Declare ptich constants  */
char const midi_note_on = 0x90;
char const midi_zero = 0;
char const midi_mute = 0;
char const midi_loud = 0x60;
typedef enum {PITCH_C=0x3c, PITCH_Dm, PITCH_D, PITCH_Em, PITCH_E,
              PITCH_F, PITCH_Gm, PITCH_G, PITCH_Am, PITCH_A,
              PITCH_Bm, PITCH_B} Pitch;
Pitch Majors[] = {PITCH_C, PITCH_D, PITCH_E, PITCH_F, PITCH_G,
                  PITCH_A, PITCH_B};

void PlayNote(HMIDIOUT pdevice, int length, Pitch pitch, int velocity);
Pitch assemPitch(char major_code, int minor, int octave);

int main(void) {
  /*  Open MIDI Out Device  */
  int err;
  HMIDIOUT device;
  int midiport = 0; // Default midi device ID is 0.
  err = midiOutOpen(&device, midiport, 0, 0, CALLBACK_NULL);
  if (err != MMSYSERR_NOERROR) { // Error Handling
    printf("Error opening MIDI Output");
    exit(1);
  }
  /*  Play MIDI what user input  */
  int octave, Longevity;
  char Melody[4];
  while (scanf_s("%d %d%s", &Longevity, &octave, Melody, 4) == 3) {
    Pitch p = assemPitch(tolower(Melody[0]), !!Melody[1], octave); 
    PlayNote(device, Longevity, p, midi_loud);
  }
  /*  Close MIDI Out Device  */
  midiOutClose(device);
  return 0;
}

Pitch assemPitch(char major_code, int bminor, int octave) {
  // major_code must be lower-case character
  Pitch p = Majors[(major_code - 'c' + 7)%7];
  p += octave*12;
  p -= bminor;
  return p;
}

typedef union {
  char data[4];
  unsigned int word;
} MidiMessage;

int const unit_len = 10; // 노트 단위길이 10ms
void PlayNote(HMIDIOUT device, int length, Pitch pitch, int velocity) {
  MidiMessage Note = { {midi_note_on, pitch, velocity, midi_zero} };
  midiOutShortMsg(device, Note.word);
  Sleep(length * unit_len);
  Note.data[2] = midi_mute;
  midiOutShortMsg(device, Note.word);
}
