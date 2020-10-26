#include <MIDI.h>
#define LED 13

// https://github.com/FortySevenEffects/arduino_midi_library

//MIDI_CREATE_DEFAULT_INSTANCE();
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);


unsigned int clock_count = 0;
unsigned long tick_time = 0;

// midi clock is 24 pulses per quarter note https://en.wikipedia.org/wiki/MIDI_beat_clock
#define CLOCKS_PER_SYNC 12
#define SYNC_LEN 5 // ms

#define STATE_WAIT 0
#define STATE_SYNC_PULSE 1

unsigned int state = STATE_WAIT;

void handleClock()
{
    clock_count += 1;
}

void setup()
{
    MIDI.setHandleClock(handleClock); 

    // Initiate MIDI communications, listen to all channels
    MIDI.begin(MIDI_CHANNEL_OMNI);
    pinMode(LED, OUTPUT);
}

void loop()
{
    // Call MIDI.read the fastest you can for real-time performance.
    MIDI.read();

    switch(state) 
    {
        case STATE_WAIT:
            if(clock_count == CLOCKS_PER_SYNC) 
            {
                clock_count = 0;
                tick_time = millis();
                state = STATE_SYNC_PULSE;
                digitalWrite(LED, 1);
            }
            break;
            
        case STATE_SYNC_PULSE: 
            if(millis() - tick_time > SYNC_LEN)
            {
                digitalWrite(LED, 0);
                state = STATE_WAIT;
            }
            break;

        default:
            state = STATE_WAIT;
    }
}

