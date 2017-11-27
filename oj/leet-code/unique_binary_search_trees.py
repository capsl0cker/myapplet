#num = C(2n, n) / (n + 1)

def numTrees1(n):
	def factor(n):
		sum = 1
		for i in range(1, n + 1):
			sum *= i
		return sum
	t1 = factor(n)
	t1 = factor(n * 2)
	t3 = t2 / (t1 * t1)
	return t3 / (n + 1)

# dp[i] = sum(dp[j] * dp[i - 1 -j]), 0 <= j <= n - 1
def numTrees2(n):
	if n == 0:
		return 1
	dp = [0] * (n + 1)
	dp[0] = 1
	dp[1] = 1

	for i in range(2, n + 1):
		dp[i] = 0
		for j in range(0, n):
			dp[i] += dp[j] * dp[i - 1 - j]
	return dp[n]
