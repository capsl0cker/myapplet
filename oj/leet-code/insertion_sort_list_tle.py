
class ListNode:
	def __init__(self, x):
		self.val = x
		self.next = None

def insertionSortList(head):
	if head == None or head.next = None:
		return head
	h = ListNode(0)
	h.next= None
	p = head

	while p:
		q = h.next
		prev = h
		while q and q.val < p.val:
			prev = q
			q = q.next
		r = p.next
		p.next = prev.next
		prev.next = p
		p = r
	head = h.next
	return head
