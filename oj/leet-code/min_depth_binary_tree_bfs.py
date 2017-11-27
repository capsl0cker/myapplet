class TreeNode:
	def __init__(self, x):
		self.val = x
		self.left = None
		self.right = None

class Solution:
	def minDepth(self, root):
		queue = []
		d = 0
		if not root:
			return 0
		queue.append(root)
		first = root
		while queue:
			head = queue.pop(0)
			if head == first:
				d += 1
			if not head.left and not head.right:
				return d
			l = len(queue)
			if head.left:
				queue.append(head.left)
			if head.right:
				queue.append(head.right)
			if head == first:	#note here, first should point to correct place
				first = queue[l]

r = TreeNode(1)
l = TreeNode(2)
r = TreeNode(3)
r.left = l
r.right = r

s = Solution()
print s.minDepth(r)

