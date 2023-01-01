import ctypes

libfact = ctypes.CDLL("C:/Users/Alex Simon/Desktop/Software/bldc-firmware/CommutationLookupTable.so")

assert libfact.GetPWMDutyCycles(0, 0, 0, 0) == (0, 0, 0)