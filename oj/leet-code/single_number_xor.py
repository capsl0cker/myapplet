def singleNumber(A):
	t = 0
	for x in A:
		t = t ^ x
	return t

A = [1,1,2,2,3]
print singleNumber(A)
