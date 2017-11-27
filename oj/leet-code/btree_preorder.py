class TreeNode:
	def __init__(self, x):
		self.val = x
		self.left = None
		self.right = None

def preorderTraversal(root):
	if root == None:
		return []
	st = []
	res = [] 
	st.append(root)
	while len(st) > 0:
		p = st.pop()
		res.append(p.val)
		if p.right:
			st.append(p.right)
		if p.left:
			st.append(p.left)
	return res
