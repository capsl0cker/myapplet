class ListNode:
	def __init__(self, x):
		self.val = x
		self.next = None

def insertionSortList(head):
	if head == None or head.next == None:
		return head
	p = head
	cur = head.next

	while cur:
		p = head
		while p.val <= cur.val and p != cur:
			p = p.next
		if p != cur:
			while p != cur:
				p.val, cur.val = cur.val, p.val
				p = p.next
		cur = cur.next
	return head
