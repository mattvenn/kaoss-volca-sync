#include <MIDI.h>
#define LED 13
#include "FastLED.h"
#define NUM_LEDS 32
#define DATA_PIN 17 

unsigned int clock_count = 0;
unsigned long tick_time = 0;
unsigned long last_count = 0;
unsigned int sync_count = 0;
unsigned int led_count = 0;
bool demo_mode = 0;
CRGB bar_complete = CRGB(0, 30, 0);
CRGB bar_marker   = CRGB(2, 10, 0);

// midi clock is 24 pulses per quarter note https://en.wikipedia.org/wiki/MIDI_beat_clock
#define CLOCKS_PER_SYNC 12
#define SYNC_LEN 5 // ms
#define SYNCS_PER_LED  12 // so should be 1 led per bar

#define STATE_WAIT 0
#define STATE_SYNC_PULSE 1
#define INCREMENT_LED 2

unsigned int state = STATE_WAIT;

CRGB leds[NUM_LEDS];

void setup()
{
    pinMode(LED, OUTPUT);
    digitalWrite(LED, 1);
    Serial2.begin(31250);
    FastLED.addLeds<WS2812, DATA_PIN>(leds, NUM_LEDS);
    reset_leds();
    FastLED.show();
}

void reset_leds()
{
    for(int i = 0; i < NUM_LEDS; i ++)
    {
        leds[i] = CRGB::Black;
        if((i+1) % 4 == 0)
            leds[i] = bar_marker;
    }
}

void loop()
{
    if(Serial2.available())
        if(Serial2.read() == 248)
            clock_count += 1;
   
    // for testing LEDs.
    // get a sync from Kaoss pad 24 pulses per beat. @120bpm a beat is 500ms, 24 pulses in 500ms
    if(demo_mode)
        if(millis() > last_count + 20) {
            last_count = millis();
            clock_count += 1;
        }

    switch(state) 
    {
        case STATE_WAIT:
            if(clock_count >= CLOCKS_PER_SYNC) 
            {
                clock_count = 0;
                tick_time = millis();
                state = STATE_SYNC_PULSE;
                digitalWrite(LED, 1);
                sync_count ++;
            }
            break;
            
        case STATE_SYNC_PULSE: 
            if(millis() - tick_time > SYNC_LEN)
            {
                digitalWrite(LED, 0);
                Serial.println("clock");
                Serial.println(sync_count);
                if(sync_count == SYNCS_PER_LED)
                    state = INCREMENT_LED;
                else    
                    state = STATE_WAIT;
            }
            break;

        case INCREMENT_LED:
            sync_count = 0;

            if(led_count == NUM_LEDS)
            {
                reset_leds();
                led_count = 0;
            }

            leds[led_count] = bar_complete;
            FastLED.show();
            led_count ++;

            state = STATE_WAIT;

            break;

        default:
            state = STATE_WAIT;
    }
}

