class TreeNode:
	def __init__(self, x):
		self.val = x
		self.left = None
		self.right = None

class Solution_runtime_error:
	balanced = True

	def isBalanced(self, root):
		def height(root):	# define height() as a embedded function will occur runtime error when the recurision
			if not root:	# is too deep, i guess this may lead to stack overflow, because each-recurison will 
				return 0	# define a copy of height()
			l = height(root.left) + 1
			r = height(root.right) + 1

			if abs(l - r) > 1:
				Solution.balanced = False
			return max(l, r)

		height(root)
		if Solution.balanced:
			print 'balanced'
			return Solution.balanced
		else:
			print 'un balanced'
			return Solution.balanced


class Solution:
	balanced = True
	def height(self, root):
		if not root:
			return 0
		l = self.height(root.left) + 1
		r = self.height(root.right) + 1

		if abs(l - r) > 1:
			Solution.balanced = False
		return max(l, r)
	
	def isBalanced(self, root):
		Solution.balanced = True
		self.height(root)
		return Solution.balanced

n1 = TreeNode(1)
n2 = TreeNode(2)
n3 = TreeNode(3)
n4 = TreeNode(4)
n5 = TreeNode(5)
n6 = TreeNode(6)
n7 = TreeNode(7)
n8 = TreeNode(8)
n9 = TreeNode(9)

n1.left = n2
n2.left = n3
n3.left = n4
n4.left = n5
n5.left = n6
n6.left = n7
n7.left = n8
n8.left = n9


s = Solution()
s.isBalanced(n1)
