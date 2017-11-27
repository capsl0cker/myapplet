def singleNumber(A):
	A.sort()
	t = 1
	n = A[0]
	for i in range(1, len(A)):
		if A[i] != n:
			if t == 1:
				break
			else:
				n = A[i]
				t = 1
		else:
			t += 1
	return n

l = [1,1,2,2,3,4,4,5,5]
print singleNumber(l)


