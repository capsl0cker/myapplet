class TreeNode:
	def __init__(self, x):
		self.val = x
		self.left = None
		self.right = None

def postorderTraversal(root):
	if root == None:
		return []
	res = []
	st = []
	cur = pre = None
	st.append(root)
	whiel st:
		cur = st[-1]
		if (cur.left == None and cur.right == None) or (pre != None and (pre == cur.left or pre == cur.right)):
			res.append(cur.val)
			pre = st.pop()
		else:
			if cur.right:
				st.append(cur.right)
			if cur.left:
				st.append(cur.left)
	return res

