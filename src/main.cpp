/*
 * CrushFuzz
 * Copyright (C) 2026 YimRakHee <https://github.com/YimRakHee>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <lv2/core/lv2.h>
#include <cmath>
#include <algorithm>
#include <stdint.h>

#define CRUSH_FUZZ_URI "https://github.com/YimRakHee/CrushFuzz"

class CrushFuzz {
public:
    enum PortIndex {
        IN = 0, OUT = 1, INPUT_GAIN = 2, BIT_DEPTH = 3, DOWNSAMPLE = 4, DRY_LEVEL = 5, WET_LEVEL = 6
    };

    const float* in_port = nullptr;
    float* out_port = nullptr;
    const float* input_gain_ptr = nullptr;
    const float* bit_depth_ptr = nullptr;
    const float* downsample_ptr = nullptr;
    const float* dry_level_ptr = nullptr;
    const float* wet_level_ptr = nullptr;

    float held_sample = 0.0f;
    uint32_t sample_counter = 0;

    CrushFuzz() = default;
};

static LV2_Handle instantiate(const LV2_Descriptor* descriptor,
                              double rate,
                              const char* bundle_path,
                              const LV2_Feature* const* features) {
    return new CrushFuzz();
}

static void connect_port(LV2_Handle instance, uint32_t port, void* data) {
    CrushFuzz* self = static_cast<CrushFuzz*>(instance);
    switch (static_cast<CrushFuzz::PortIndex>(port)) {
        case CrushFuzz::IN:          self->in_port = static_cast<const float*>(data); break;
        case CrushFuzz::OUT:         self->out_port = static_cast<float*>(data); break;
        case CrushFuzz::INPUT_GAIN:  self->input_gain_ptr = static_cast<const float*>(data); break;
        case CrushFuzz::BIT_DEPTH:   self->bit_depth_ptr = static_cast<const float*>(data); break;
        case CrushFuzz::DOWNSAMPLE:  self->downsample_ptr = static_cast<const float*>(data); break;
        case CrushFuzz::DRY_LEVEL:   self->dry_level_ptr = static_cast<const float*>(data); break;
        case CrushFuzz::WET_LEVEL:   self->wet_level_ptr = static_cast<const float*>(data); break;
    }
}

void run(LV2_Handle instance, uint32_t n_samples) {
    CrushFuzz* self = static_cast<CrushFuzz*>(instance);

    const float in_gain    = *self->input_gain_ptr;
    const float bit_depth  = std::clamp(*self->bit_depth_ptr, 1.0f, 24.0f);
    const uint32_t ds_fact = static_cast<uint32_t>(*self->downsample_ptr);
    const float dry_vol    = *self->dry_level_ptr;
    const float wet_vol    = *self->wet_level_ptr;

    const float steps = std::exp2f(bit_depth);
    const float inv_steps = 1.0f / steps;

    for (uint32_t i = 0; i < n_samples; ++i) {
        const float raw_input = self->in_port[i];

        const float gained_input = raw_input * in_gain;

        if (++self->sample_counter >= ds_fact) {
            self->held_sample = gained_input;
            self->sample_counter = 0;
        }

        float wet_signal = std::roundf(self->held_sample * steps) * inv_steps;

        float mixed_signal = (raw_input * dry_vol) + (wet_signal * wet_vol);

        self->out_port[i] = std::clamp(mixed_signal, -1.0f, 1.0f);
    }
}

static void cleanup(LV2_Handle instance) {
    delete static_cast<CrushFuzz*>(instance);
}

static const LV2_Descriptor descriptor = {
    CRUSH_FUZZ_URI,
    instantiate,
    connect_port,
    nullptr, // activate
    run,
    nullptr, // deactivate
    cleanup,
    nullptr  // extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index) {
    return (index == 0) ? &descriptor : nullptr;
}
