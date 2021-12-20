import numpy as np

def part_1(lines):
    filtered = [entry for entry in lines if entry[0][0] == entry[1][0] or entry[0][1] == entry[1][1]]
    field = {}
    for start, end in filtered:
        sx_, sy_ = [min(start[0], end[0]), min(start[1], end[1])]
        ex_, ey_ = [max(start[0], end[0]), max(start[1], end[1])]
        xrange = list(range(sx_, ex_ + 1))
        yrange = list(range(sy_, ey_ + 1))
        if len(yrange) == 1:
            yrange *= len(xrange)
        if len(xrange) == 1:
            xrange *= len(yrange)
        for x, y in list(zip(xrange, yrange)):
            if (x, y) not in field:
                field[(x, y)] = 1
            else:
                field[(x, y)] += 1

    count = 0
    for k,v in field.items():
        if v > 1:
            count += 1
    return count

def part_2(lines):
    field = {}
    for start, end in lines:
        sx_, sy_ = start[0], start[1]
        ex_, ey_ = end[0], end[1]
        xrange = list(range(sx_, ex_ + (1 if ex_ >= sx_ else -1), 1 if ex_ >= sx_ else -1))
        yrange = list(range(sy_, ey_ + (1 if ey_ >= sy_ else -1), 1 if ey_ >= sy_ else -1))

        if len(yrange) == 1:
            yrange *= len(xrange)
        if len(xrange) == 1:
            xrange *= len(yrange)

        assert (len(xrange) == len(yrange))

        for x, y in list(zip(xrange, yrange)):
            if (x, y) not in field:
                field[(x, y)] = 1
            else:
                field[(x, y)] += 1

    count = 0
    for k,v in field.items():
        if v > 1:
            count += 1

    return count

with open("data.in", "r") as f:
    lines = []
    for inline in f:
        inline = inline.strip()
        inline = inline.split(' -> ')
        inline = list(map(lambda x: x.split(','), inline))
        lines.append([list(map(lambda x : int(x), l)) for l in inline])

    print(part_1(lines))
    print(part_2(lines))