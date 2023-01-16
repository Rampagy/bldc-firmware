# STM32F4 Null=V0 performance tests

The goal is that the 'new' Null=V0 implementation will will have comparable performance to the previous Alternating-Reversing Sequence commutation.

## Methodology

Five different methods performance will be tested using an STM32F4 discovery board:

1. Original implementation (alternating-reversing sequence)
1. Using [math.h](https://cplusplus.com/reference/cmath/) (null=v0)
1. Using [utils_math.h](https://github.com/vedderb/bldc/blob/master/util/utils_math.c) (null=v0)
1. Using [onboard dsp](https://www.keil.com/pack/doc/CMSIS/DSP/html/index.html) (null=v0)
1. A hybrid of all of the above taking the fastest part from each (we'll see if I get to this one)

Optimization [-O2](https://developer.arm.com/documentation/dui0375/g/Compiler-Command-line-Options/-Onum?lang=en) will be used as that's what's currently being used in the project.

## Results

With a 2MHz wall clock doing 36000 iterations

**function**|**clocks**|**time per call**
:-----:|:-----:|:-----:
foc_svm0|378045|5.25 us
foc_svm1|550889|7.65 us
foc_svm2|535851|7.44 us
foc_svm3|267847|3.72 us
foc_svm4|360930|5.01 us
foc_svm5(without USE_ONBOARD_DSP)|519661|7.21 us
foc_svm5(with USE_ONBOARD_DSP)|304900|4.23 us

## Notes

foc_svm2 was okay but using utils_fast_atan2() did not have as much precision as the other two methods and it was noticable in the duty cycles (1% to 2% different per phase than the other methods).

foc_svm3 is super fast, but I don't trust the sector detection.

My recommendation is foc_svm5 with USE_ONBOARD_DSP.  It provides the best accuracy and is sslightly faster than the original implementation.

## Resources

https://www.st.com/content/ccc/resource/technical/layouts_and_diagrams/schematic_pack/group1/60/36/18/8c/71/30/45/dc/MB997-F407VGT6-E01_Schematic/files/MB997-F407VGT6-E01_Schematic.pdf/jcr:content/translations/en.MB997-F407VGT6-E01_Schematic.pdf

https://www.st.com/resource/en/user_manual/dm00039084-discovery-kit-with-stm32f407vg-mcu-stmicroelectronics.pdf

http://socialledge.com/sjsu/index.php/FreeRTOS_Tutorial

https://www.freertos.org/a00125.html

https://cdn-shop.adafruit.com/product-files/1138/SK6812+LED+datasheet+.pdf

https://www.iar.com/support/resources/articles/using-iar-embedded-workbench-for-arm-and-the-cmsis-dsp-library/

https://stm32f4-discovery.net/2014/10/stm32f4-fft-example/

https://www.keil.com/pack/doc/CMSIS/DSP/html/group__RealFFT.html#ga5d2ec62f3e35575eba467d09ddcd98b5

https://community.st.com/s/question/0D50X00009XkibiSAB/stm32f207-adctimerdma-poor-peripheral-library-examples

https://arm-software.github.io/CMSIS_5/DSP/html/index.html

https://www.freertos.org/FreeRTOS-Coding-Standard-and-Style-Guide.html
