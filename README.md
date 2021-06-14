# simulate BNN and INT8 quantized models
## requires
* CPU compatible with `ARMv7-A` ISA

## how to compile
`gcc -mfpu=neon -O3 [bitcount.c | int8.c]`

## test computes
* `bitcount.c	: NUM_RANDOM * 32 * NUM_REPEAT MACs`
* `int8.c		: NUM_RANDOM * 4  * NUM_REPEAT MACs`

## test result
On Raspberry Pi 2, both test spends about 6 seconds

which implies that BNN will be 8x faster than INT8 quantized model


