
# inverting amplifier --> Gain = Rf/Rin
# non-inverting amplifier --> Gain = 1 + Rf/Rin

resolution = 65535

Vmin = float(input('Enter min DAC output voltage (after amplification): ') or "0")
Vmax = float(input('Enter max DAC output voltage (after amplification): ') or "5.82")
Vfloor = float(input('Enter voltage floor (before calibration): ') or "0.5")
numOctaves = int(input('Enter number of OCTAVES: ') or "6")
invertOutput = input("Will the output be inverted (ie. the higher the value the lower the voltage output)? Enter 'true' or 'false': ")

if invertOutput == "true" or invertOutput == "True":
    invertOutput = True
elif invertOutput == "false" or invertOutput == "False":
    invertOutput = False

numNotes = 12

voltPerBit = (Vmax - Vmin) / resolution #  the change in voltage from ex. 22 to 23 == 0.00008880750744 volts

floor16bit = Vfloor / voltPerBit
range16Bit = 1 / voltPerBit
print('16 bit floor: ', floor16bit)
print('1 Volt Range (ie. 0.5v to 1.5v): ', range16Bit)


halfStep = range16Bit / numNotes
print('Half-step 16 bit value: ', halfStep)

VOLTAGE_MAP = []

for x in range( numNotes * numOctaves ):
    VOLTAGE_MAP.append(int(floor16bit + (halfStep*x)))

if invertOutput:
    VOLTAGE_MAP = list(reversed(VOLTAGE_MAP))

print("Array Length: ", len(VOLTAGE_MAP))
print(VOLTAGE_MAP) # 16,890.46
