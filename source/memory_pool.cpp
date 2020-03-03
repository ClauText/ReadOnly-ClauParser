
#include "readonly_clau_parser.h"


namespace wiz {
	Node* MemoryPool::Get() {
		if (size > 0 && count < size) {
			count++;
			return &arr[count - 1];
		}
		else {
			// in real depth?  <= 10 ?
			// static Node[10] and list<Node*> ?
			count++; // for number check.
			else_list.push_back(new Node());
			return else_list.back();
		}
	}
	MemoryPool::~MemoryPool() {
		//
	}
	
	void MemoryPool::Clear() { // maybe just one called....
		if (arr) {
			delete[] arr;
			arr = nullptr;
		}
		for (Node* x : else_list) {
			delete x;
		}
	}
}
