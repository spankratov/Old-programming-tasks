#ifndef __HASH_TABLE__
#define __HASH_TABLE__
#include <list>
#include <algorithm>

const unsigned long long primes[] = {89, 239, 65449, 16777213, 4294967291, 922337203603819}; // простые числа, которые будут по очереди выступать в роли верхней границы для результата хэш-функции
const unsigned int limit = 5; // когда количество различных элементов в списке элементов с одинаковым хэшем больше limit, таблица перестраивается функцией rebuild()

template <class KeyType, class ValueType, class Hasher>
class hash_table
{
	struct pair
	{
		KeyType key;
		ValueType value;
		bool operator==(const pair& x) {return key == x.key;}
	};
	typedef std::list<typename hash_table::pair> list;
	struct node
	{
		list same_hash_list; // список из пар с одинаковым хэшем
		unsigned long long different_elements; // количество элементов с различных ключом в списке same_hash_list, когда эта переменная становится больше limit, таблица перестраивается
		node(): different_elements(0) {}
		unsigned long long size()const {return same_hash_list.size();}
		void push_back(const typename hash_table::pair& x)
		{
			if(std::find(same_hash_list.begin(), same_hash_list.end(), x) == same_hash_list.end())
				++different_elements;
			same_hash_list.push_back(x);
		}
		typename list::iterator begin() {return same_hash_list.begin();}
		typename list::iterator end() {return same_hash_list.end();}
		void erase(typename list::iterator pos) {same_hash_list.erase(pos);}
		void remove(const typename hash_table::pair& rem) {same_hash_list.remove(rem);}
	};
	node* table;
	unsigned long long count; // primes[count] - размер массива table и верхнее значение для хэш-функции
public:
	hash_table(): count(0) {table = new node[primes[0]];}
	~hash_table() {delete[] table;}
	pair make_pair(const KeyType& new_key, const ValueType& new_value)const { pair result; result.key = new_key; result.value = new_value; return result; }
	void print_table()const;
	void rebuild(); // перестроить таблицу, взяв в качестве верхнего значения хэш-функции следующий элемент в массиве primes[]
	void insert(const KeyType& new_key, const ValueType& new_value);
	class searcher // класс, похожий по работе на обычный итератор, но перемещающийся только по тем парам из списка list хэш-таблицы htable, у которых ключ равен current_key.
	{
		const hash_table& htable;
		typename list::iterator current;
		KeyType current_key;
	public:
		searcher() {}
		searcher(const hash_table& ptable, KeyType some_key): current_key(some_key), htable(ptable) // searcher инициализируется хэш-таблицей, в которой производится поиск, и ключом, по которому производится поиск, при этом current - итератор, указывающий на первую пару в списке с key == some_key
		{
			unsigned long long index = Hasher()( current_key, primes[htable.count] );
			if(htable.table[index].size() == 0)
				current = htable.table[index].end();
			else
			{
				current = htable.table[index].begin();
				while( ( current != htable.table[index].end() ) && ((*current).key != current_key) ) ++current;
			}
		}
		ValueType operator* ()const {return (*current).value;}
		bool operator== (const searcher& p)const {return current == p.current;}
		bool operator!= (const searcher& p)const {return current != p.current;}
		searcher& operator++()
		{
			unsigned long long index = Hasher()( current_key, primes[htable.count] );
			if(current == htable.table[index].end())  throw "Iterator: out of bounds";
			++current;
			while( ( current != htable.table[index].end() ) && ((*current).key != current_key) ) ++current;
			return *this;
		}
		bool end()const {
			list::iterator pos = htable.table[Hasher()( current_key, primes[htable.count] )].end();
			return current == pos; 
		}
	};
	void remove(const KeyType& rem); // удалить первую встретившуюся пару с ключом, равным rem
	void remove_all(const KeyType& rem); // удалить все пары с ключом rem
};

template <class KeyType, class ValueType, class Hasher>
void hash_table<KeyType, ValueType, Hasher>::rebuild()
{
	if(count == 5)
		return;
	node* new_table = new node[primes[++count]];
	for(unsigned long long i = 0; i < primes[count - 1]; ++i)
	{
		if(table[i].size() != 0)
		{
			for(list::iterator pos = table[i].begin(); pos != table[i].end(); ++pos)
			{
				unsigned long long index = Hasher()( (*pos).key, primes[count] );
				if( (new_table[index].different_elements >= limit) && (count < 5) )
					rebuild();
				new_table[index].push_back( *pos );
			}
		}
	}
	delete[] table;
	table = new_table;
}

template <class KeyType, class ValueType, class Hasher>
void hash_table<KeyType, ValueType, Hasher>::insert(const KeyType& new_key, const ValueType& new_value)
{
	unsigned long long index = Hasher()(new_key, primes[count]);
	if( (table[index].different_elements >= limit) && (count < 5) )
	{
		rebuild();
		insert(new_key, new_value);
	}
	else
		table[index].push_back( make_pair(new_key, new_value) );
}

template <class KeyType, class ValueType, class Hasher>
void hash_table<KeyType, ValueType, Hasher>::print_table()const
{
	for(unsigned long long i = 0; i < primes[count]; ++i)
	{
		if(table[i].size() != 0)
		{
			std::cout << i << ':' << std::endl;
			for(typename list::iterator pos = table[i].begin(); pos != table[i].end(); ++pos)
				std::cout << '\t' << (*pos).key << ' ' << (*pos).value << std::endl;
			std::cout << std::endl;
		}

	}
}

template <class KeyType, class ValueType, class Hasher>
void hash_table<KeyType, ValueType, Hasher>::remove(const KeyType& rem)
{
	unsigned long long index = Hasher()(rem, primes[count]);
	if(table[index].size() == 0)
		return;
	list::iterator pos = table[index].begin();
	while( (pos != table[index].end()) && ((*pos).key != rem) )
		++pos;
	if(pos != table[index].end())
		table[index].erase(pos);
}

template <class KeyType, class ValueType, class Hasher>
void hash_table<KeyType, ValueType, Hasher>::remove_all(const KeyType& rem)
{
	unsigned long long index = Hasher()(rem, primes[count]);
	table[index].remove(make_pair(rem, ValueType()));
}

#endif