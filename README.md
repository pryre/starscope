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
pikaur -S picocom arm-none-eabi-gdb
```

### (Linux) Udev rule for non-sudo access and upload
Make sure your user is in the `dialout` group:
```
sudo usermod -aG dialout ${USER}
```
You will need to log out and back in (or reboot) for this to take effect

Create a udev rule (e.g. `/etc/udev/rules.d/53-rp2040.rules`):
```
SUBSYSTEMS=="usb", ACTION=="add", ATTR{idVendor}=="2e8a", ATTRS{idProduct}=="0003|000a", MODE="660", GROUP="dialout"
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
