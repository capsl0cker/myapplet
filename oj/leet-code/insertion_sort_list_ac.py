class ListNode:
	def __init__(self, x):
		self.val = x
		self.next = None

def findpos(head, node):
	pos = head
	while head and node.val > head.val:
		pos = head
		head = head.next
	
	return pos

def insertionSortList(head):
	if head == None or head.next == None:
		return head
	node = head

	while node.next != None:
		cur = node.next
		if cur.val >= node.val:
			node = node.next
			continue
		node.next = cur.next
		if cur.val <= head.val:
			cur.next = head
			head = cur
			continue
		pos = findpos(head, cur)
		cur.next = pos.next
		pos.next = cur
	return head

