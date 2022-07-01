# starscope
A low-cost solution for star-finding and ranging.

## Setup


### Udev rule for non-sudo access and upload for Linux
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
