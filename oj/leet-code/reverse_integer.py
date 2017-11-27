def reverse(x):
	flag = 0
	if x < 0:
		flag = 1
		x = -x
	t = 0
	while x != 0:
		t = t * 10 + x % 10
		x = x / 10
	if flag:
		return -t
	else:
		return t

print reverse(123)
print reverse(-123)
