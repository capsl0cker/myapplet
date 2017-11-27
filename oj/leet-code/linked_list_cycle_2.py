
class ListNode:
	def __init__(self, x):
		self.val = x
		self.next = None

def detectCycle(head):
	if head == None:
		return None
	
	slow = fast = head
	while fast and fast.next:
		slow = slow.next
		fast = fast.next.next
		if slow == fast:
			break
	
	if fast == None or fast.next == None:
		return None

	slow = head
	while slow != fast:
		slow = slow.next
		fast = fast.next
	return slow
