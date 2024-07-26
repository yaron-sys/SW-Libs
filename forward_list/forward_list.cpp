#include <iostream>
#include <cassert>
#include "forward_list.hpp"

#if __cplusplus < 201103L
#define nullptr 0
#endif

using namespace ilrd;

ForwardList::Node::Node(int data, Node *next): m_data(data), m_next(next){}

ForwardList::ForwardList(): m_head(nullptr){}

ForwardList::~ForwardList()
{
	Node* tmp = m_head;
	
	while (m_head != nullptr){
		tmp = tmp->m_next;
		delete m_head;
		m_head = tmp;
	}
}

ForwardList::Status ForwardList::clone_list(const ForwardList& other)
{
	const Node* iter = other.m_head;
	Node* dest_head = nullptr;
	
	if(iter != nullptr){
		dest_head = new(std::nothrow) Node(iter->m_data, nullptr);
		if(dest_head == nullptr){
			return ALLOC_FAILURE;
		}
		iter = iter->m_next;
	}
	
	m_head = dest_head;
	
	while (iter != nullptr)
	{
		dest_head->m_next = new(std::nothrow) Node(iter->m_data, nullptr);
		if(dest_head->m_next == nullptr){
			return ALLOC_FAILURE;
		}
		dest_head = dest_head->m_next;
		iter = iter->m_next;
	}
	
	return ALLOC_SUCCESS;
}

ForwardList::ForwardList(const ForwardList& other)
{	
	if(this != &other){
		Status stat = clone_list(other);
		if (stat == ALLOC_FAILURE){
			this->~ForwardList();
		}
	}
}

ForwardList& ForwardList::operator=(const ForwardList& other)
{
	// copy and swap idiom
	if(this != &other){
		ForwardList assg_list(other);
		if (assg_list.m_head == nullptr){
			return *this;
		}
		
		Node* tmp_node = m_head;
		m_head = assg_list.m_head;
		assg_list.m_head = tmp_node;
	}
	return *this;
}

ForwardList::Status ForwardList::push_front(int data)
{
	Node* new_head = new(std::nothrow) Node(data, m_head);
	if(new_head == nullptr){
		return ALLOC_FAILURE;
	}
	m_head = new_head;
	
	return ALLOC_SUCCESS;
}

int ForwardList::pop_front()
{
	Node* node_del = m_head;
	int data = 0;
	
	assert(node_del != nullptr);
	
	data = node_del->m_data;
	m_head = node_del->m_next;
	delete node_del;
	
	return data;
}

size_t ForwardList::size() const
{
	size_t size = 0;
	Node* iter = m_head;
	
	while (iter != nullptr){
		++size;
		iter = iter->m_next;
	}
	return size;
}

bool ForwardList::empty() const
{
	return m_head == 0;
}

int ForwardList::front() const
{
	assert(m_head != nullptr);
	
	return m_head->m_data;
}
