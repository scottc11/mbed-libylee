
# inverting amplifier --> Gain = Rf/Rin
# non-inverting amplifier --> Gain = 1 + Rf/Rin


Vin = float(input('Enter max voltage input: '))
Rf = int(input('Enter Rf Value (feedback): '))
Rin = int(input('Enter Rin (input):'))

output = Vin * (1 + Rf/Rin)

print(output)