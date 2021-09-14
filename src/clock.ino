#include <MIDI.h>
#define LED 13
#include "FastLED.h"
#define NUM_LEDS 16 
#define DATA_PIN 17 
#define SWITCH_P 8
#define SWITCH_N 6

unsigned int clock_count = 0;
unsigned long tick_time = 0;
unsigned long last_count = 0;
unsigned int sync_count = 0;
unsigned int led_count = 0;
bool demo_mode = 0;
bool button_pressed = 0;
CRGB bar_complete = CRGB(0, 30, 0);
CRGB bar_marker   = CRGB(2, 10, 0);

// midi clock is 24 pulses per quarter note https://en.wikipedia.org/wiki/MIDI_beat_clock
#define CLOCKS_PER_SYNC 12
#define SYNC_LEN 5 // ms
#define SYNCS_PER_LED 8 // so should be 1 led per bar

#define STATE_WAIT 0
#define STATE_SYNC_PULSE 1
#define STATE_INCREMENT_LED 2

unsigned int state = STATE_INCREMENT_LED;

// leds are backwards; 0 on the right and NUM_LEDS-1 on the left.
CRGB leds[NUM_LEDS];

void setup()
{
    pinMode(LED, OUTPUT);
    pinMode(SWITCH_N, OUTPUT);
    pinMode(SWITCH_P, INPUT_PULLUP);
    digitalWrite(SWITCH_N, LOW);

    digitalWrite(LED, 1);
    Serial2.begin(31250);
    FastLED.addLeds<WS2812, DATA_PIN>(leds, NUM_LEDS);
    reset_leds();
}

void reset_leds()
{
    for(int i = NUM_LEDS-1; i >= 0; i --)
    {
        if(i % 4 == 0)
            leds[i] = bar_marker;
        else
            leds[i] = CRGB::Black;
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

    if(digitalRead(SWITCH_P) == false) // pressed
    {
        if(button_pressed == false)
        {
        button_pressed = true;
        sync_count = 0;
        reset_leds();
        leds[NUM_LEDS-1] = bar_complete;
        led_count = 1;
        FastLED.show();
        }
    }
    else
        button_pressed = false;

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
                    state = STATE_INCREMENT_LED;
                else    
                    state = STATE_WAIT;
            }
            break;

        case STATE_INCREMENT_LED:
            sync_count = 0;
            leds[NUM_LEDS-1-led_count] = bar_complete;
            FastLED.show();
            led_count ++;

            if(led_count >= NUM_LEDS)
            {
                led_count = 0;
                reset_leds();
            }

            state = STATE_WAIT;
            break;

        default:
            state = STATE_WAIT;
    }
}

