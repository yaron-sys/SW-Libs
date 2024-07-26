#ifndef FORWARD_LIST_HPP_
#define FORWARD_LIST_HPP_

#include <cstddef> // size_t

namespace ilrd{
	
class ForwardList{
public:
	enum Status{ALLOC_SUCCESS, ALLOC_FAILURE};

	ForwardList();
   ~ForwardList();
	ForwardList(const ForwardList& other);
	ForwardList& operator=(const ForwardList& other);
	
	Status push_front(int data);
	int pop_front();   // Undefined behavior in case of empty list
	int front() const;    // Undefined behavior in case of empty list
	size_t size() const;  // O(n)
	bool empty() const;   // O(1)
	
private:	
	struct Node
	{	
		int m_data;
		Node *m_next;
		Node(int data, Node *next);
	};
	Status clone_list(const ForwardList& other);
	Node *m_head;
};

} // NAMESPACE ILRD

#endif  // FORWARD_LIST_HPP_
