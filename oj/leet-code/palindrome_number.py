def isPalindrome(x):
	if x < 0:
		return False
	t = x
	sum = 0
	while x != 0:
		sum += sum * 10 + x % 10
		x = x /10
	return t == sum
