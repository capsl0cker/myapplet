class TreeNode:
	def __init__(self, x):
		self.val = x
		self.left = None
		self.right = None

class Solution:
	d = 0x7fffffff
	def minDepth(self, root):
		def dfs(root, dep):
			if not root:
				return 0
			if not root.left and not root.right:
				if dep + 1 < Solution.d:
					Solution.d = dep + 1
				return
			dfs(root.left, dep + 1)
			dfs(root.right, dep + 1)
		Solution.d = 0x7fffffff	#class attribute, should init to MAX each call
		dfs(root, 0)
		if Solution.d != 0x7fffffff:
			return Solution.d
		else:
			return 0

r = TreeNode(1)

s = Solution()
print s.minDepth(r)

n1 = TreeNode(1)
r = TreeNode(0)
r.left = n1

s = Solution()
print s.minDepth(r)



