#include <MIDI.h>
#define LED 13

unsigned int clock_count = 0;
unsigned long tick_time = 0;

// midi clock is 24 pulses per quarter note https://en.wikipedia.org/wiki/MIDI_beat_clock
#define CLOCKS_PER_SYNC 12
#define SYNC_LEN 5 // ms

#define STATE_WAIT 0
#define STATE_SYNC_PULSE 1

unsigned int state = STATE_WAIT;

void setup()
{
    pinMode(LED, OUTPUT);
    digitalWrite(LED, 1);
    Serial2.begin(31250);
}

void loop()
{
    if(Serial2.available())
        if(Serial2.read() == 248)
            clock_count += 1;
    
    switch(state) 
    {
        case STATE_WAIT:
            if(clock_count >= CLOCKS_PER_SYNC) 
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
                Serial.println("clock");
                state = STATE_WAIT;
            }
            break;

        default:
            state = STATE_WAIT;
    }
}

