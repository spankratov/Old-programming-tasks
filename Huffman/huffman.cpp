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
	// b - ������� ���� ������, c - ������� ���, table - ������� ����� (���������)
	if(dynamic_cast<node*>(b) != NULL) // ���� b - ��������� �� node
	{
		c.push_back(false); // ���� ��� �����, �� � ���� ����������� 0
		building_table(dynamic_cast<node*>(b) -> left, c, table);
		c.push_back(true); // ���� ��� ������, �� � ���� ����������� 1
		building_table(dynamic_cast<node*>(b) -> right, c, table);
	}
	else // ����� b - ��������� �� leaf
		table[ dynamic_cast<leaf*>(b) -> byte ] = c;
	if(c.bits() > 0)
		--c; // ��� ����������� ������� � �������� ��������� ��� ���������
}

void print_tree(std::ofstream& outfile, base* tree, vector_of_bits& buf)
{
	/* �������� ������ ������ �������� � ���� � ���������� ��� ������:
	����������� ������ �� ������ � ������� ����� �������.
	���� ����, ����� � ����� 1(���) � ��� ������.
	���� ����, ����� ����� �����, ������ �����, 0(���).

	����������:
	������� 1, ������� ������, ������������ ���� � �������� � ����.
	������� 0 � ������� �� ����� ��� ��������, ��������� � ���� ������(������ ������, ������ �����), �������� � ����.
	����� ������ � � ����� ������ �������� ���� �������, �� � ���� ������ �������. 
	*/
	// buf - ������ �� �����, ������� ���������� ����������� ����, ������� ����� ����������, � �������� ��, ��� ������ � ��������� ����� ����� ��� 8 �������� �����
	if(dynamic_cast<node*>(tree) != NULL) // ���� tree - ��������� �� node
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
	else // ����� tree - ��������� �� leaf
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

	outfile.put(0); // ����� �� ��� ����� ������ ���������� �������� ��� � ��������� �����, � ���� ������ "��������", �����, ����� ��� ������� �������� � ������ ��� ����������, �� �������� �������� ���� ���� ������
	#ifdef STATISTICS
	std::cout << "Start building the table of frequency" << std::endl;
	#endif
	unsigned long long* freq = frequency(infile);
	#ifdef STATISTICS
	std::cout << "The table of frequency was built" << std::endl;
	#endif

	// ������ ������ ��������
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

	// ������ ������� ����� ��������
	#ifdef STATISTICS
	std::cout << "Start building the Huffman\'s table" << std::endl;
	#endif
	vector_of_bits* table_of_codes = new vector_of_bits[256];
	vector_of_bits c;
	building_table(tree, c, table_of_codes);
	#ifdef STATISTICS
	std::cout << "The Huffman\'s table was built" << std::endl;
	#endif

	// ���������� � ���� ������
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

	// ������ �������� �� �� ����
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

	// ���������� ���������� �������� ��� � ��������� �����
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

	int count = infile.get(); // ��������� ���������� �������� ��� � ��������� �����
	
	// ��������� ������
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

	// ��������� ���������� �����
	#ifdef STATISTICS
	std::cout << "Start decompressing the file \"" << in << '\"' << std::endl;
	#endif
	buf.clear();
	int ch;
	while((ch = infile.peek()) != EOF)
	{
		base* tmp = tree;
		while( dynamic_cast<leaf*>(tmp) == NULL ) // ���� tmp ��������� �� �� leaf
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

	// ���� ����������, �������� � ��������� ������
	while(buf.bits() != (8 - count))
	{
		base* tmp = tree;
		while( dynamic_cast<leaf*>(tmp) == NULL ) // ���� tmp ��������� �� �� leaf
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
