class TreeNode:
	def __init__(self, x):
		self.val = x
		self.left = None
		self.right = None

def hasPathSum(root, sum):
	def check(root, total):
		if root:
			if total + root.val == sum and root.left == None and root.right == None:
				return True
			if check(root.left, total + root.val):
				return True
			else:
				return check(root.right, total + root.val)
	return check(root, 0)
			
