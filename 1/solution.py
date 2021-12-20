import numpy as np

def part_1(data):
	diff = data[1::] > data[0:len(data)-1]

	return np.count_nonzero(diff)

def part_2(data):
	N = len(data)
	sums = data[0:N - 2] + data[1:N - 1] + data[2:N]
	return part_1(sums) 

data = np.array([int(v) for v in open("data.in", "r")])


print(part_1(data))
print(part_2(data))