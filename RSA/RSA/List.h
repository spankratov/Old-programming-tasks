#ifndef __LIST_131__
#define __LIST_131__

template <typename TIP>
class List
{
protected:
	struct Node
	{
		Node* prev; 
		Node* next;
		TIP data;
		Node(const TIP &X): prev(NULL), next(NULL), data(X) {}
	};
	Node* first;
	Node* last;
	unsigned int count;
public:
	List():first(NULL), last(NULL), count(0) {}
	void push_back(const TIP&);
	void push_front(const TIP&);
	void clear();
	void empty() {clear();}
	~List() {clear();}
	unsigned int size()const {return count;}
	TIP& operator[] (unsigned int)const;
	List(const List&);
	List& operator=(const List&);
	class iterator
	{
		Node* current;
	public:
		iterator(Node* p = NULL): current(p) {}
		TIP& operator* ()const {return current -> data;}
		bool operator== (const iterator& p)const {return current == p.current;}
		bool operator!= (const iterator& p)const {return current != p.current;}
		iterator& operator++() //++x
		{
			if(current) current = current -> next;
			else throw "Iterator: out of bounds";
			return *this;
		}
		iterator operator++ (int) //x++
		{
			Node* tmp = current;
			if(current) current = current -> next;
			else throw "Iterator: out of bounds";
			return iterator(tmp);
		}
		friend class List<TIP>;
	};
	iterator begin()const {return iterator(first);}
	iterator end()const {return iterator(NULL);}
	class reverse_iterator
	{
		Node* current;
	public:
		reverse_iterator(Node* p = NULL): current(p) {}
		TIP& operator* ()const {return current -> data;}
		bool operator== (const reverse_iterator& p)const {return current == p.current;}
		bool operator!= (const reverse_iterator& p)const {return current != p.current;}
		reverse_iterator& operator++() //++x
		{
			if(current) current = current -> prev;
			else throw "Iterator: out of bounds";
			return *this;
		}
		reverse_iterator operator++ (int) //x++
		{
			Node* tmp = current;
			if(current) current = current -> prev;
			else throw "Iterator: out of bounds";
			return reverse_iterator(tmp);
		}
		friend class List<TIP>;
	};
	reverse_iterator rbegin()const {return reverse_iterator(last);}
	reverse_iterator rend()const {return reverse_iterator(NULL);}
	void sort();
	iterator find(const TIP&)const;
	void remove(iterator& pos)
	{
		--count;
		if(pos.current -> prev)
			pos.current -> prev -> next = pos.current -> next;
		else
			first = pos.current -> next;
		if(pos.current -> next)
			pos.current -> next -> prev = pos.current -> prev;
		else
			last = pos.current -> prev;
		iterator tmp = pos;
		++pos;
		delete tmp.current;
	}
	void remove(reverse_iterator& pos)
	{
		--count;
		if(pos.current -> prev)
			pos.current -> prev -> next = pos.current -> next;
		else
			first = pos.current -> next;
		if(pos.current -> next)
			pos.current -> next -> prev = pos.current -> prev;
		else
			last = pos.current -> prev;
		reverse_iterator tmp = pos;
		++pos;
		delete tmp.current;
	}
};

template <typename TIP>
List<TIP>::List(const List<TIP>& A)
{
	Node* tmp = A.first;
	if(tmp)
	{
		first = new Node(tmp -> data);
		tmp = tmp -> next;
		last = first;
		while(tmp)
		{
			last -> next = new Node(tmp -> data);
			last -> next -> prev = last;
			last = last -> next;
			tmp = tmp -> next;
		}
	}
	else
		first = last = NULL;
	count = A.count;
}

template <typename TIP>
List<TIP>& List<TIP>::operator= (const List<TIP>& A)
{
	if(this != &A)
	{
		clear();
		Node* tmp = A.first;
		if(tmp)
		{
			first = new Node(tmp -> data);
			tmp = tmp -> next;
			last = first;
			while(tmp)
			{
				last -> next = new Node(tmp -> data);
				last -> next -> prev = last;
				last = last -> next;
				tmp = tmp -> next;
			}
		}
		else
			first = last = NULL;
		count = A.count;
	}
	return *this;
}

template <typename TIP>
void List<TIP>::push_back(const TIP& X)
{
	if(!first)
	{
		first = last = new Node(X);
	}
	else
	{
		last -> next = new Node(X);
		last -> next -> prev = last;
		last = last -> next;
	}
	++count;
}

template <typename TIP>
void List<TIP>::push_front(const TIP& X)
{
	if(!first)
	{
		first = last = new Node(X);
	}
	else
	{
		first -> prev = new Node(X);
		first -> prev -> next = first;
		first = first -> prev;
	}
	++count;
}

template <typename TIP>
void List<TIP>::clear()
{
	Node* tmp;
	while(first)
	{
		tmp = first;
		first = first -> next;
		delete tmp;
	}
	count = 0; first = last = NULL;
}

template <typename TIP>
TIP& List<TIP>::operator[] (unsigned int i)const
{
	if(i >= count)
		throw "Out of list bounds";
	Node* tmp = first;
	unsigned int n = 0;
	while(n < i)
	{
		tmp = tmp -> next;
		++n;
	}
	if(tmp) return tmp -> data;
	throw "Out of list bounds";
}

template <typename TIP>
void List<TIP>::sort()
{
	if(!first) return;
	if(first == last) return;
	bool notsorted = true;
	do
	{
		Node* begins[2];
		Node* ends[2];
		unsigned char p = 0;
		// Split
		ends[p] = begins[p] = first;
		first = first -> next;
		ends[!p] = begins[!p] = NULL;
		while(first)
		{
			if(first -> data < ends[p] -> data)
				p = !p;
			if(!begins[p]) begins[p] = first;
			else ends[p] -> next = first;
			ends[p] = first;
			first = first -> next;
		}
		if(ends[p]) ends[p] -> next = NULL;
		if(ends[!p]) ends[!p] -> next = NULL;
		// Merge
		if(begins[!p])
		{
			bool s1, s2;
			p = (begins[0] -> data < begins[1] -> data) ? 0 : 1;
			last = first = begins[p];
			begins[p] = begins[p] -> next;
			while(begins[p])
			{
				s1 = (begins[p] -> data < last -> data) ? true : false;
				s2 = (begins[!p] -> data < last -> data) ? true : false;
				if(s1 == s2)
					p = (begins[0] -> data < begins[1] -> data) ? 0 : 1;
				else
					if(s1) p = !p;
				last -> next = begins[p];
				begins[p] = begins[p] -> next;
				last = last -> next;
			}
			last -> next = begins[!p]; last = ends[!p];
		}
		else
		{
			first = begins[p];
			notsorted = false;
		}
	}while(notsorted);
	// Восстановление связей prev
	first -> prev = NULL;
	Node* now = first;
	while(now -> next)
	{
		now -> next -> prev = now;
		now = now -> next;
	}
	last = now;
}

template<typename TIP>
typename List<TIP>::iterator
	List<TIP>::find(const TIP& X)const
{
	Node* now = first;
	while(now)
	{
		if(now -> data == X)
			return iterator(now);
		now = now -> next;
	}
	return end();
}

#endif