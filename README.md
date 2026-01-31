## CrushFuzz

Bitcrusher & Fuzz LV2 plugin for guitar. destructive digital artifacts with an analog-style parallel mixing architecture.<br>
No GUI.<br>
Tested Environment: Arch Linux / Ardour.<br>

![Screenshot](./images/crushplug.png)

### Features
- Digital Distortion: Combines Bit Depth Reduction and Sample Rate Reduction (Downsampling) for sonic textures.

- Parallel Processing Architecture: Features independent Dry and Wet level controls. This allows you to blend the destroyed signal with the clean input to maintain low-end punch and transient clarity.

### Parameters
Input Gain: Pre-processing amplification.<br>
Bit Depth: Amplitude quantization. Lower values introduce "crunchy" digital noise and lo-fi textures.<br>
Downsample: Sample rate reduction factor. Higher values result in "aliased" and synth-like tones.<br>
Dry Level: Level of the original clean signal.<br>
Wet Level: Level of the processed signal.<br>

### Requirements
- LV2 SDK
- C++20 compatible compiler (GCC/Clang)
- CMake

### Build & Install (LINUX)
```bash
# Clone the repository
git clone https://github.com/YimRakHee/CrushFuzz.git
cd CrushFuzz

# Create a build directory
mkdir build && cd build

# Configure CMake and compile
cmake ..
make

# Install the plugin (installs to ~/.lv2/ by default)
make install
```

