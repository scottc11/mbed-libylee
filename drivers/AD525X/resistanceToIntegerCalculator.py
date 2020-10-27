
max_res = int(input('Enter the max resistance: ' or 1000))
min_res = int(input('Enter the min resistance: ' or 120))
num_taps = int(input('Number of Taps: ' or 255))
increment = (max_res - min_res) / num_taps

target_res = int(input('target resistance: ')) - min_res


output = num_taps - (int(round(target_res / increment)))

print(output)



# example
# with max res @ 1000, min res @ 120, num taps @ 255, and a target resistance of 160ohms, integer value would be 236