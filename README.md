# imu

`firmware` - Firmware for an `STM32F3Discovery` board

`client` - Host software.

## Firmware

- Make the dependencies first: `make toolchain && make openocd`.

- Variables to set before build: 
	- `HOST_PLAT={macos,x86_64}` (default: `macos`)
	- `PROFILE={debug,release}` (default: `debug`)

- Build: `make binary`

## Client

TBD