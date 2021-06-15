# simulate BNN and INT8 quantized models
## requires
* CPU compatible with `ARMv7-A` ISA
* or `ARMv7-M` ISA (for `simulate.ino`)

## how to compile
`gcc -mfpu=neon -O3 [bitcount.c | int8.c]`

## test computes
### ARMv7-A
* `bitcount.c	: NUM_RANDOM * 32 * NUM_REPEAT MACs`
* `int8.c		: NUM_RANDOM * 4  * NUM_REPEAT MACs`

### ARMv7-M (`simulate.ino`)
* `BNN : NUM_RANDOM * 32 * NUM_REPEAT MACs`
* `INT8 : NUM_RANDOM * NUM_REPEAT MACs`

## test result
On Raspberry Pi 2, both test spends about 6 seconds
* `BNN : ~715 MMACS`
* `INT8 : ~89.5 MMACS`

On Arduino Nano 33 BLE, 
* `BNN : ~67.1 MMACS`
* `INT8 : ~6.87 MMACS`
