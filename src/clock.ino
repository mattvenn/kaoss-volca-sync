#include <MIDI.h>
#define LED 13

// https://github.com/FortySevenEffects/arduino_midi_library

MIDI_CREATE_DEFAULT_INSTANCE();

unsigned int clock_count = 0;
unsigned long tick_time = 0;

// midi clock is 24 pulses per quarter note https://en.wikipedia.org/wiki/MIDI_beat_clock
#define CLOCKS_PER_SYNC 24
#define SYNC_LEN 5 // ms

#define STATE_WAIT 0
#define STATE_SYNC_ON 1
#define STATE_SYNC_WAIT 2
#define STATE_SYNC_OFF 3

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
            if(clock_count == CLOCKS_PER_SYNC - 1)
                state = STATE_SYNC_ON;
            break;
            
        case STATE_SYNC_ON: 
            clock_count = 0;
            tick_time = millis();
            digitalWrite(LED, 1);
            state = STATE_SYNC_WAIT;
            break;

        case STATE_SYNC_WAIT: 
            if(millis() - tick_time > SYNC_LEN)
                state = STATE_SYNC_OFF;
            break;
                
        case STATE_SYNC_OFF: 
            digitalWrite(LED, 0);
            state = STATE_WAIT;
            break;

        default:
            state = STATE_WAIT;
    }
}

