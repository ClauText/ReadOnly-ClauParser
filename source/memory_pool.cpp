
#include "readonly_clau_parser.h"


namespace wiz {

	MemoryPool::MemoryPool(long long _size) {
		arr = new Node[_size];
		count = 0;
		size = _size;
	}
	Node* MemoryPool::Get() {
		if (count < size) {
			count++;
			return &arr[count - 1];
		}
		else {
			else_list.push_back(new Node());
			return else_list.back();
		}
	}
	MemoryPool::~MemoryPool() {
		delete[] arr;
		Node* temp;
		for (Node* x : else_list) {
			delete x;
		}
	}
}
