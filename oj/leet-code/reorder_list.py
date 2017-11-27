class ListNode:
	def __init__(self, x):
		self.val = x
		self.next = None

def reorderList(head):
	if head == None:
		return
	first = second = head
	while second and second.next:
		first = first.next
		second = second.next.next
	second = first.next
	first.next = None

	tmp = ListNode(0)
	tmp.next = None
	p = second
	while p:
		q = p.next
		p.next = tmp.next
		tmp.next = p
		p = q
	second = tmp.next
	first = head
	
	while first and second:
		p = first.next
		q = second.next
		second.next = first.next
		first.next = second
		first = p
		second = q

	

