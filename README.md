# Sync Kaoss Pad 3 midi clock out to Korg Volca clock in

Kaoss pad puts out midi beat clock

https://en.wikipedia.org/wiki/MIDI_beat_clock

24 pulses per beat. Volca modular wants 12 per beat. 
Use callback on midi clock and a simple state machine to make 5ms pulses positive pulses for Volca.

# Circuit

https://www.instructables.com/Send-and-Receive-MIDI-with-Arduino/

![circuit](https://content.instructables.com/ORIG/FHN/J43V/H6MEVNIE/FHNJ43VH6MEVNIE.jpg?auto=webp&frame=1&width=1024&fit=bounds&md=6e9fa64c79e410c4f38b10631f547327)


# Midi library

https://github.com/FortySevenEffects/arduino_midi_library

details on choosing different hardware serial:

https://www.pjrc.com/teensy/td_libs_MIDI.html
