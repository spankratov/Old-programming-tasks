#include "huffman.h"

unsigned long long* frequency(std::ifstream& infile)
{
	unsigned long long* result = new unsigned long long[256];
	for(int i = 0; i <= 255; ++i)
		result[i] = 0;
	unsigned int c;
	while((c = infile.get()) != EOF)
		++result[c];
	return result;
}

void building_table(base* b, vector_of_bits& c, vector_of_bits* table)
{
	// b - текущий узел дерева, c - текущий код, table - таблица кодов (результат)
	if(dynamic_cast<node*>(b) != NULL) // если b - указатель на node
	{
		c.push_back(false); // если идЄм влево, то к коду добавл€етс€ 0
		building_table(dynamic_cast<node*>(b) -> left, c, table);
		c.push_back(true); // если идЄм вправо, то к коду добавл€етс€ 1
		building_table(dynamic_cast<node*>(b) -> right, c, table);
	}
	else // иначе b - указатель на leaf
		table[ dynamic_cast<leaf*>(b) -> byte ] = c;
	if(c.bits() > 0)
		--c; // при перемещении обратно к родителю последний бит стираетс€
}

void print_tree(std::ofstream& outfile, base* tree, vector_of_bits& buf)
{
	/* алгоритм записи дерева ’аффмана в файл и считывани€ его оттуда:
	–екурсивный проход по дереву в глубину слева направо.
	≈сли лист, пишем в вывод 1(бит) и сам символ.
	≈сли узел, пишем левую ветку, правую ветку, 0(бит).

	—читывание:
	—читали 1, считали символ, сформировали лист Ч положили в стек.
	—читали 0 Ч достали из стека два элемента, соединили в узел дерева(первый правый, второй левый), положили в стек.
	 онец записи Ч в стеке должен остатьс€ один элемент, он и есть дерево целиком. 
	*/
	// buf - вектор из битов, который постепенно накапливает биты, которые нужно напечатать, и печатает их, как только в последнем байте будет все 8 значимых битов
	if(dynamic_cast<node*>(tree) != NULL) // если tree - указатель на node
	{
		print_tree(outfile, dynamic_cast<node*>(tree) -> left, buf);
		print_tree(outfile, dynamic_cast<node*>(tree) -> right, buf);
		if(buf.is_ready_to_print())
		{
			outfile << buf;
			buf.clear();
		}
		buf.push_back(false);
		if(buf.is_ready_to_print())
		{
			outfile << buf;
			buf.clear();
		}
	}
	else // иначе tree - указатель на leaf
	{
		buf.push_back(true);
		if(buf.is_ready_to_print())
		{
			outfile << buf;
			buf.clear();
		}
		buf.push_back(dynamic_cast<leaf*>(tree) -> byte);
		if(buf.is_ready_to_print())
		{
			outfile << buf;
			buf.clear();
		}
	}
}

std::ostream& operator<<(std::ostream& out, const vector_of_bits& v)
{
	for(unsigned int i = 0; i < v.vect.size(); ++i)
		out.put(v.vect[i]);
	return out;
}

void Huffman_encode(const char* in, const char* out)
{
	#ifdef STATISTICS
	std::cout << "Start encoding" << std::endl;
	#endif
	std::ifstream infile(in, std::ios_base::binary);
	std::ofstream outfile(out, std::ios_base::binary);

	outfile.put(0); // позже на это место станет количество значимых бит в последнем байте, а пока ставим "заглушку", чтобы, когда нам придЄтс€ вставить в начало это количество, не пришлось сдвигать весь файл вправо
	#ifdef STATISTICS
	std::cout << "Start building the table of frequency" << std::endl;
	#endif
	unsigned long long* freq = frequency(infile);
	#ifdef STATISTICS
	std::cout << "The table of frequency was built" << std::endl;
	#endif

	// строим дерево ’аффмана
	#ifdef STATISTICS
	std::cout << "Start building the Huffman\'s tree" << std::endl;
	#endif
	std::priority_queue<base*, std::vector<base*>, bigger_node> q;
	for(int i = 0; i <= 255; ++i)
		if(freq[i] > 0)
			q.push(new leaf((unsigned char)i, freq[i]));
	while(q.size() > 1)
	{
		base* first = q.top(); q.pop();
		base* second = q.top(); q.pop();
		q.push(new node(first, second));
	}
	base* tree = q.top(); q. pop();
	#ifdef STATISTICS
	std::cout << "The Huffman\'s tree was built" << std::endl;
	#endif

	// строим таблицу кодов символов
	#ifdef STATISTICS
	std::cout << "Start building the Huffman\'s table" << std::endl;
	#endif
	vector_of_bits* table_of_codes = new vector_of_bits[256];
	vector_of_bits c;
	building_table(tree, c, table_of_codes);
	#ifdef STATISTICS
	std::cout << "The Huffman\'s table was built" << std::endl;
	#endif

	// записываем в файл дерево
	#ifdef STATISTICS
	std::cout << "Start writing the Huffman\'s tree to the file" << std::endl;
	#endif
	vector_of_bits buf;
	print_tree(outfile, tree, buf);
	if(!buf.empty())
		outfile << buf;
	#ifdef STATISTICS
	std::cout << "The Huffman\'s tree was written to the file \"" << out << '\"' << std::endl;
	#endif

	// запись символов по их коду
	#ifdef STATISTICS
	std::cout << "Start compressing the file \"" << in << '\"' << std::endl;
	#endif
	buf.clear();
	infile.clear();
	infile.seekg(0, infile.beg);
	int ch;
	while((ch = infile.get()) != EOF)
	{
		buf.push_back(table_of_codes[ch]);
		if(buf.is_ready_to_print())
		{
			outfile << buf;
			buf.clear();
		}
	}
	if(!buf.empty())
		outfile << buf;

	// записываем количество значащих бит в последнем байте
	outfile.seekp(0);
	if(buf.empty())
		outfile.put(8);
	else
		outfile.put( buf.bits() % 8 );

	infile.close();
	outfile.close();
	#ifdef STATISTICS
	std::cout << "End of encoding, result is in \"" << out << '\"' << std::endl << std::endl;
	#endif
}

void Huffman_decode(const char* in, const char* out)
{
	#ifdef STATISTICS
	std::cout << "Start decoding" << std::endl;
	#endif
	std::ifstream infile(in, std::ios_base::binary);
	std::ofstream outfile(out, std::ios_base::binary);

	int count = infile.get(); // считываем количество значащих бит в последнем байте
	
	// считываем дерево
	#ifdef STATISTICS
	std::cout << "Start reading the Huffman\'s tree from the file" << std::endl;
	#endif
	vector_of_bits buf;
	std::stack<base*> stack_tree;
	while(!infile.eof())
	{
		if(buf.bits() == 0)
			buf.push_back((unsigned char)infile.get());
		if(buf.extract_first_bit())
		{
			if(buf.bits() < 8)
				buf.push_back((unsigned char)infile.get());
			stack_tree.push( new leaf(buf.extract_first_byte(), 0) );
		}
		else
		{
			if(stack_tree.size() < 2)
				break;
			base* r = stack_tree.top(); stack_tree.pop();
			base* l = stack_tree.top(); stack_tree.pop();
			stack_tree.push( new node(l, r) );
		}
	}
	base* tree = stack_tree.top(); stack_tree.pop();
	#ifdef STATISTICS
	std::cout << "The Huffman\'s tree was read from the file" << std::endl;
	#endif

	// разжимаем оставшиес€ байты
	#ifdef STATISTICS
	std::cout << "Start decompressing the file \"" << in << '\"' << std::endl;
	#endif
	buf.clear();
	int ch;
	while((ch = infile.peek()) != EOF)
	{
		base* tmp = tree;
		while( dynamic_cast<leaf*>(tmp) == NULL ) // пока tmp указывает не на leaf
		{
			if(buf.empty())
				buf.push_back((unsigned char)infile.get());
			if(buf.extract_first_bit())
				tmp = dynamic_cast<node*>(tmp) -> right;
			else
				tmp = dynamic_cast<node*>(tmp) -> left;
		}
		outfile.put( dynamic_cast<leaf*>(tmp) -> byte );
	}

	// если необходимо, работаем с последним байтом
	while(buf.bits() != (8 - count))
	{
		base* tmp = tree;
		while( dynamic_cast<leaf*>(tmp) == NULL ) // пока tmp указывает не на leaf
		{
			if(buf.extract_first_bit())
				tmp = dynamic_cast<node*>(tmp) -> right;
			else
				tmp = dynamic_cast<node*>(tmp) -> left;
		}
		outfile.put( dynamic_cast<leaf*>(tmp) -> byte );
	}

	infile.close();
	outfile.close();
	#ifdef STATISTICS
	std::cout << "End of decoding, result is in \"" << out << '\"' << std::endl << std::endl;
	#endif
}
