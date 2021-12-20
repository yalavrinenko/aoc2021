
def bit_freq(data):
	digits = []
	for i in range(0, len(data[0])):
		digits.append({'0' : 0, '1' : 0})

	for number in data:
		i = 0
		for digit in number:
			digits[i][digit] += 1;
			i += 1

	return digits

def part_1(data):
	digits = bit_freq(data)

	
	gamma = [1 if max(d['0'], d['1']) == d['1'] else 0 for d in digits]
	gamma_value = int("".join(str(e) for e in gamma), 2)
	epsilon = [1 if min(d['0'], d['1']) == d['1'] else 0 for d in digits]
	epsilon_value = int("".join(str(e) for e in epsilon), 2)

	return (gamma_value, epsilon_value)


def log_filtration(data, filter_selector):
	bit_number = 0
	while len(data) != 1 and bit_number != len(data[0]):
		freq = bit_freq(data)[bit_number]
		filter_bit = filter_selector(freq['0'], freq['1']) # '1' if max(freq['0'], freq['1']) == freq['1'] else '0'
		data = [x for x in data if x[bit_number] == filter_bit]
		bit_number += 1
	
	return int(data[0], 2)


def part_2(data):
	return (log_filtration(data, lambda zeros, ones : '1' if max(zeros, ones) == ones else '0'), log_filtration(data, lambda zeros, ones : '0' if min(zeros, ones) == zeros else '1'))


data = [l.strip() for l in open('data.in', 'r')]
[gamma, epsilon] = part_1(data)
print(gamma * epsilon)

[oxy, co2] = part_2(data)
print (oxy * co2)