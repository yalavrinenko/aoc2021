x_add = [11, 11, 14, 11, -8, -5, 11, -13, 12, -1, 14, -5, -4, -8, 0]
z_div = [1, 1, 1, 1, 26, 26, 1, 26, 1, 26, 1, 26, 26, 26]
w_add = [1, 11, 1, 11, 2, 9, 7, 11, 6, 15, 7, 1, 8, 6, 0]

z = [0, ]

def z_value(z, w, d):
    x = not (z[-1] + x_add[d] == w)
    if not (z[-1] + x_add[d] == w):
        z.append(w + w_add[d])
    else:
        z.pop()

    print("Digit = {4}: Current x_add={2} w_add={3}.\tNext x_add={0} w_add={1}".format(x_add[d+1], w_add[d + 1], x_add[d], w_add[d], d))
    print(z)

# num = [9, 2, 9, 6, 9, 5, 9, 3, 4, 9, 7, 9, 9, 2]

num = [8, 1, 5, 1, 4, 1, 7, 1, 1, 6, 1, 3, 8, 1]
for i, c in enumerate(num):
    z_value(z, c, i)

# z_value(z, 9, 0)
# z_value(z, 2, 1)
# z_value(z, 9, 2)
# z_value(z, 6, 3)
# z_value(z, 9, 4)
# z_value(z, 5, 5)
# z_value(z, 9, 6)
# z_value(z, 3, 7)
# z_value(z, 4, 8)
# z_value(z, 9, 9)
# z_value(z, 7, 10)
# z_value(z, 9, 11)
# z_value(z, 9, 12)
# z_value(z, 2, 13)
