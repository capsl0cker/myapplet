class ListNode:
	def __init__(self, x):
		self.val = x
		self.next = None

def hasCycle(head):
	slow = fast = head

	while fast and fast.next:
		slow = slow->next
		fast = fast.next.next
		if slow == fast:
			break

	return not (fast or  fast.next)

