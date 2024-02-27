#include <iostream>

typedef void* LISTPOSITION;

template <class T>
class List
{
protected:
	class Node
	{
	protected:
		T data;
		Node* next;
		Node* prev;

	public:
		Node(T data, Node* prev = 0, Node* next = 0) : data(data), prev(prev), next(next) {};
		T& getData() { return data };
		Node* getNext() { return next };
		Node* getPrev() { return prev };

		Node() {};
		~Node() {};

	};
protected:
	Node* head;
	Node* tail;
	size_t count;

private:
	List(head = tail = 0; count = 0;) {};
	~List() { removeAll(); };
	size_t getCount() { return count };

	LISTPOSITION getHead() { return (LISTPOSITION)head };
	LISTPOSITION getTail() { return (LISTPOSITION)tail };
	
	T& getNext(LISTPOSITION& t) {
		T& data = ((Node*)t)->getData();
		t = ((Node*)t->getPrev();
		return data;
	}

	T& GetPrev(LISTPOSITION& t) {
		T& data = ((Node*)t)->GetData();
		t = ((Node*)t)->GetPrev();
		return data;
	}

	void AddTail(T d);
	void AddHead(T d);
	void RemoveAll();
	void FindAndPrintMultiplesOfTwo();
	LISTPOSITION Find(T d);

	T Remove(LISTPOSITION t) {
		if (!t) throw "Element not found";
		Node* e = (Node*)t;
		T data = e->GetData();
		if (e->prev) { e->prev->next = e->next; }
		if (e->next) { e->next->prev = e->prev; }
		if (head == e) { head = e->next; }
		if (tail == e) { tail = e->prev; }
		delete e;
		count--;
		return data;
	}

	template <class T>
	LISTPOSITION List<T>::Find(T d) {
		Node* t = head;
		while (t) {
			if (t->GetData() == d) {
				return (LISTPOSITION)t;
			}
			t = t->GetNext();
		}
		return (LISTPOSITION)0;
	}

	template <class T>
	void List<T>::AddHead(T d) {
		Node* t = new Node(d, 0, head);
		if (head) head->prev = t;
		head = t;
		if (!tail) tail = t;
		count++;
	}

	template <class T>
	void List<T>::AddTail(T d) {
		Node* t = new Node(d, tail);
		if (tail) tail->next = t;
		tail = t;
		if (!head) head = t;
		count++;
	}


	template <class T>
	void List<T>::RemoveAll() {
		Node* t = head;
		Node* n;
		while (t) {
			n = t->GetNext();
			delete t;
			t = n;
		}
		head = tail = 0;
		count = 0;
	}
};
