
#include "readonly_clau_parser.h"


namespace wiz {
	Node* MemoryPool::Get() {
		if (size > 0 && count < size) {
			count++;
			return &arr[count - 1];
		}
		else {
			count++; // for number check..
			else_list.push_back(new Node());
			return else_list.back();
		}
	}
	MemoryPool::~MemoryPool() {
		//
	}
	
	void MemoryPool::Clear() {
		if (arr) {
			delete[] arr;
		}
		for (Node* x : else_list) {
			delete x;
		}
	}
}
