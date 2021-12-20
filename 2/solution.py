
def part_1(course):
	forward = 0
	depth = 0
	for [c, v] in course:
		if c == "forward":
			forward += v
		elif c == "up":
			depth -= v
		elif c == "down":
			depth += v

	return (forward, depth)

def part_2(course):
	forward = 0
	depth = 0
	aim = 0
	for [c, v] in course:
		if c == "forward":
			forward += v
			depth += aim * v
		elif c == "up":
			aim -= v
		elif c == "down":
			aim += v

	return (forward, depth)



course = [(k, int(v)) for line in open("data.in", "r") for (k, v) in [line.split(' ')]]
[forward, depth] = part_1(course)
print(forward * depth)
[forward, depth] = part_2(course)
print(forward * depth)
