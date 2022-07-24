# starscope
A low-cost solution for star-finding and ranging.

## Setup

### Setup submodules
```
git submodule update --init
```

### Dependencies

#### Arch Linux - fish
Install SDK and help tools:
```
pikaur -S pico-sdk

set -x PICO_SDK_PATH /usr/share/pico-sdk
pikaur -S picotool

# optional (terminal emulator and debugger)
pikaur -S picocom openocd-picoprobe
```

### (Linux) Udev rule for non-sudo access and upload
Make sure your user is in the `dialout` group:
```
sudo usermod -aG dialout ${USER}
```
You will need to log out and back in (or reboot) for this to take effect

Create a udev rule (e.g. `/etc/udev/rules.d/53-rp2040.rules`):
```
SUBSYSTEMS=="usb", ACTION=="add", ATTR{idVendor}=="2e8a", ATTRS{idProduct}=="0003|0004|000a", MODE="660", GROUP="dialout"
```
Then trigger a udev reload:
```
sudo udevadm control --reload-rules && sudo udevadm trigger
```

### Prepare and Build Environment
In Code:
```
> CMake Configure
> CMake Build
```

There are also Code Tasks set up to perform actions like uploading and monitoring:
```
> Tasks: Run
```

## Debugging
Debugging is currently supported using the picoprobe method. Connect the Pico's in the following configuration:
![Pico debug wiring diagram](/resources/debug-diagram.png)
It is a good idea to ensure that your debug wires are reasonably short (<20cm if possible) and have good connections.

You can download the UF2 file for the "debugger Pico [here](https://datasheets.raspberrypi.com/soft/picoprobe.uf2), or go to the official repository [here](https://github.com/raspberrypi/picoprobe).

Make sure you have installed OpenOCD with the picoprobe modifications. You can also build this from scratch using:
```
git clone https://github.com/raspberrypi/openocd.git --branch picoprobe --depth=1
cd openocd
./bootstrap
./configure --enable-picoprobe --disable-werror
make -j4
make install
```

To start the debug session:
1. Go to the Debug menu in VSCode
2. Select `Pico Debug (Picoprobe)`
3. Press the Launch button

If everything works, you should see output from OpenOCD in the debug console that looks like:
```
Thread 1 hit Temporary breakpoint 1, main () at /home/pryre/Workspace/github/starscope/src/starscope/starscope_pico.cpp:110
110	int main() {
```
Note, messages like the following in the terminal window are also fine:
```
Info : New GDB Connection: 1, Target rp2040.core0, state: halted
target halted due to debug-request, current mode: Thread
xPSR: 0xf1000000 pc: 0x000000ee msp: 0x20041f00
```

### Issues (DAP Init Failed)
This suggest that your configuration and setup is good, but your wiring has an issue. This could mean your wires are too long, or simply not plugged into the correct pins.

### Issues (Error: spawn nm-multiarch ENOENT)
```
pikaur -S arm-none-eabi-binutils
cd /usr/local/bin
sudo ln -s /usr/bin/objdump objdump-multiarch
sudo ln -s /usr/bin/nm nm-multiarch
```
