//////////////////////////
//  Protean modulator   //
//    for ATtiny 85     //
// Kinetik Laboratories //
//    Pangrus 2015      //
//////////////////////////
#define N_WAVEFORMS 10

byte sample = 127;
byte sampleOld = 127;

void setup() {
    pinMode(1, OUTPUT);
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    TCCR0A = 2 << COM0A0 | 2 << COM0B0 | 3 << WGM00;
    TCCR0B = 0 << WGM02 | 1 << CS00;
}

void loop() {
    for (byte x = 0; x <= 255; x++) {
        byte waveform = (byte) map_input(analogRead(2), 0, 1023, 0, N_WAVEFORMS - 1);
        int interval = analogRead(3);
        if (interval < 5) waveform = 255;
        interval = map_input(interval, 0, 1023, 32000, 1);
        analogWrite(1, waveforms(waveform, x));
        delayMicroseconds(interval);
    }
}

byte waveforms(byte wave, byte x) {
    byte output = 255;
    if (wave == 0 && x < 128) output = 0;                                       // Square
    else if (wave == 1) output = (x < 128) ? (x << 1) : ((255 - x) << 1) + 1;   // Triangle
    else if (wave == 2) output -= x;                                            // Saw
    else if (wave == 3) output = x;                                             // Reversed Saw
    else if (wave == 4) output = 255 / (x + 1);                                 // Radiation effect
    else if (wave == 5) {                                                       // Heart beat
        if (x < 20) output = 234;
        else if (x < 82 || x >= 110) output = 0;
    } else if (wave == 6) output = (x % 2) ? (255 - x) : x;                     // Weird function
    else if (wave == 7) {                                                       // Smooth Random
        if (sampleOld == sample) sampleOld = random(255);
        else if (sampleOld > sample) sample++;
        else sample--;
        output = sample;
    } else if (wave == 8) {                                                     // Sample and Hold
        if (x == sample) sample = random(255);
        output = sample;
    } else if (wave == 9) output = random(255);                                 // Random
    return output;
}

short map_input(short x, short in_min, short in_max, short out_min, short out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}