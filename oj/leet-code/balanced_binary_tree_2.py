# another method

class TreeNode:
	def __init__(self, x):
		self.val = x
		self.left = None
		self.right = None

class Solution:
	def isBalanced(self, root):
		return self.check(root)[1]

	def check(self, root):
		if not root:
			return (0, True)
		LH, LisB = self.check(root.left)
		RH, RisB = self.check(root.right)
		return (max(LH, RH) + 1, LisB and RisB and abs(LH - RH) <= 1)
