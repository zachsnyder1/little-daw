//
//  wavetable.cpp
//  jamboard
//
//  Created by Zach Snyder on 11/26/17.
//  Copyright © 2017 Zach Snyder. All rights reserved.
//

#include "wavetable.h"

/*
 WaveTable constructor: make table, populate with pseudo square wave
*/
WaveTable::WaveTable() {
    this->table = new float[TABLE_SIZE];
    this->harmonic_amplitudes = new int[HIGHEST_HARMONIC]; // for custom synth
    square_wave();
}

/*
 Populate wave table with a sine waveform
*/
void WaveTable::sine_wave() {
    int x;
    float calculated_sample;
    double sin_input;
    
    for(x = 0; x < TABLE_SIZE; x++) {
        sin_input = ((double)x/(double)TABLE_SIZE) * M_PI * 2.0;
        calculated_sample = (float) sin(sin_input);
        this->table[x] = SINE_MAX_AMP * calculated_sample;
    }
}

/*
 Populate wave table with a pseudo square waveform
 */
void WaveTable::square_wave() {
    int x;
    for(x = 0; x < TABLE_SIZE; x++) {
        if(x < 10) {
            this->table[x] = (SQUARE_MAX_AMP * (0 + (x * 0.1)));
        }
        if(9 < x && x < 190) {
            this->table[x] = SQUARE_MAX_AMP;
        }
        if(189 < x && x < 200) {
            this->table[x] = (SQUARE_MAX_AMP * (1 - ((x-190) * 0.1)));
        }
        if(199 < x && x < 210) {
            this->table[x] = (SQUARE_MAX_AMP * (0 - ((x-200) * 0.1)));
        }
        if(209 < x && x < 390) {
            this->table[x] = -SQUARE_MAX_AMP;
        }
        if(389 < x && x < TABLE_SIZE) {
            this->table[x] = (SQUARE_MAX_AMP * (-1 + ((x-390) * 0.1)));
        }
    }
}

/*
 Populate wave table with a custom waveform
 */
void WaveTable::custom_wave() {
    float amplitude_scale;
    float amplitude_total;
    float temp_table[TABLE_SIZE];
    int target_index;
    int i, x;
    
    // set table to reference sine wave
    this->sine_wave();
    // add harmonic amplitudes
    amplitude_scale = 0.0;
    for(i = 0; i < HIGHEST_HARMONIC; i++) {
        amplitude_scale += this->harmonic_amplitudes[i];
    }
    // harmonics waveforms are added together, one wavetable index at a time
    for(i = 0; i < TABLE_SIZE; i++) {
        amplitude_total = 0.0;
        for(x = 0; x < (HIGHEST_HARMONIC); x++) {
            target_index = (int)(i * pow((double)2, x));
            target_index = target_index % TABLE_SIZE;
            amplitude_total += this->table[target_index] *
              (this->harmonic_amplitudes[x] / amplitude_scale);
        }
        temp_table[i] = amplitude_total;
    }
    // main wavetable is replaced with data stored in the temporary table
    for(i = 0; i < TABLE_SIZE; i++) {
        this->table[i] = temp_table[i];
    }
}