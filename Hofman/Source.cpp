// In The Name Of Allah

#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>

using namespace std;

struct treeNode
{
	char ch = NULL;
	int num = 0;
	double per = 0;
	treeNode *left = nullptr;
	treeNode *right = nullptr;
	treeNode *back = nullptr;
	string singleCode = "";
};

struct listNode
{
	treeNode *data = new treeNode;
	listNode *link = nullptr;
};

struct charListNode
{
	char ch = NULL;
	string hoffmanCode = "";
	charListNode *link = nullptr;
};

listNode *front = nullptr;
listNode *t = nullptr;
int totalNum = 0;
char recent = NULL;
charListNode *frontOfCharList = nullptr;

void addToFirst(char, int, treeNode*, treeNode*);
void addToEnd(char, int, treeNode*, treeNode*);
void add(char);
void setPercents();
void del(char, int);
treeNode* findMin();
void createTree();
void addToCharList(char, string);
void createHoffmanCodes();
int createNum(int, int);

int main()
{
	ifstream input("input.txt");
	if (!input)
	{
		cout << "Input file did not open." << endl;
		return EXIT_FAILURE;
	}

	ofstream table("table.txt");
	if (!table)
	{
		cout << "Table file did not open." << endl;
		return EXIT_FAILURE;
	}

	ofstream output("output.txt");
	if (!output)
	{
		cout << "Output file did not open." << endl;
		return EXIT_FAILURE;
	}

	recent = input.get();
	while (recent != EOF)
	{
		add(recent);
		recent = input.get();
	}
	input.close();

	setPercents();

	createTree();
	createHoffmanCodes();

	charListNode *p = frontOfCharList;
	table << "Char\tHoffman Code\n\n";
	while (p)
	{
		if (p->ch == '\n')
			table << "\\n";
		else if (p->ch == '\t')
			table << "\\t";
		else if (p->ch == ' ')
			table << "space";
		else
			table << p->ch;
		table << "\t" << p->hoffmanCode << "\n";
		p = p->link;
	}
	table.close();

	ofstream oTemp("temp.txt");
	if (!oTemp)
	{
		cout << "OTemp file did not open." << endl;
		return EXIT_FAILURE;
	}
	input.open("input.txt");
	if (!input)
	{
		cout << "Input file did not open." << endl;
		return EXIT_FAILURE;
	}
	recent = input.get();
	while (recent != EOF)
	{
		p = frontOfCharList;
		while (p)
		{
			if (p->ch == recent)
			{
				oTemp << p->hoffmanCode;
				break;
			}
			p = p->link;
		}
		recent = input.get();
	}
	input.close();
	oTemp.close();

	ifstream iTemp("temp.txt");
	if (!iTemp)
	{
		cout << "ITemp file did not open." << endl;
		return EXIT_FAILURE;
	}
	int n;
	recent = iTemp.get();
	while (recent != EOF)
	{
		n = 0;
		for (int i = 10000000; i >= 1 && recent != EOF; i /= 10)
		{
			n += (static_cast<int>(recent)-48) * i;
			recent = iTemp.get();
		}
		n = createNum(n, 10);
		output << char(n);
	}
	iTemp.clear();
	iTemp.close();
	output.close();

	ofstream temp("temp.txt");
	if (!temp)
	{
		cout << "Temp file did not open." << endl;
		return EXIT_FAILURE;
	}
	temp.close();

	cout << "Successfull :)" << endl;
	cout << "Press any key to exit...";
	while (!_kbhit());
	return EXIT_SUCCESS;
}

void addToFirst(char c, int n, treeNode* l, treeNode* r)
{
	front = new listNode;
	front->data->ch = c;
	front->data->num = n;
	front->data->left = l;
	front->data->right = r;
	if (l)
		l->back = front->data;
	if (r)
		r->back = front->data;
	return;
}

void addToEnd(char c, int n, treeNode* l, treeNode* r)
{
	t = front;
	while (t->link)
		t = t->link;
	t->link = new listNode;
	t->link->data->ch = c;
	t->link->data->num = n;
	t->link->data->left = l;
	t->link->data->right = r;
	if (l)
		l->back = t->link->data;
	if (r)
		r->back = t->link->data;
	return;
}

void add(char c)
{
	if (!front)
	{
		addToFirst(c, 1, nullptr, nullptr);
		totalNum++;
		return;
	}

	t = front;
	while (t)
	{
		if (t->data->ch == c)
		{
			t->data->num++;
			totalNum++;
			return;
		}
		t = t->link;
	}

	addToEnd(c, 1, nullptr, nullptr);
	totalNum++;
	return;
}

void setPercents()
{
	t = front;
	while (t)
	{
		t->data->per = (t->data->num / static_cast<double>(totalNum)) * 100;
		t = t->link;
	}
	return;
}

void del(char c, int n)
{
	if (front->data->ch == c && front->data->num == n)
	{
		front = front->link;
		return;
	}

	t = front;
	while (t->link)
	{
		if (t->link->data->ch == c && t->link->data->num == n)
		{
			t->link = t->link->link;
			return;
		}
		t = t->link;
	}
	
	return;
}

treeNode* findMin()
{
	listNode* min = front;
	t = front;

	while (t)
	{
		if (t->data->per < min->data->per)
			min = t;
		t = t->link;
	}

	return min->data;
}

void createTree()
{
	if (!front)
		return;

	treeNode *m1, *m2 = nullptr;

	while (front->link)
	{
		m1 = findMin();
		del(m1->ch, m1->num);
		m1->singleCode = "0";
		m2 = findMin();
		del(m2->ch, m2->num);
		m2->singleCode = "1";
		if (front)
			addToEnd(NULL, (m1->num + m2->num), m1, m2);
		else
			addToFirst(NULL, (m1->num + m2->num), m1, m2);
		setPercents();
	}

	return;
}

void addToCharList(char c, string h)
{
	if (!frontOfCharList)
	{
		frontOfCharList = new charListNode;
		frontOfCharList->ch = c;
		frontOfCharList->hoffmanCode = h;
		return;
	}

	charListNode *point = frontOfCharList;
	while (point->link)
		point = point->link;
	point->link = new charListNode;
	point->link->ch = c;
	point->link->hoffmanCode = h;

	return;
}

void createHoffmanCodes()
{
	if (!front)
		return;

	char c = NULL;
	string h = "";
	treeNode *p = front->data;

	while (front->data->left || front->data->right)
	{
		if (p->left)
		{
			h += p->singleCode;
			p = p->left;
		}
		else if (p->right)
		{
			h += p->singleCode;
			p = p->right;
		}
		else if (p->ch == NULL)
		{
			if (p->singleCode == "0")
				p->back->left = nullptr;
			else
				p->back->right = nullptr;
			p = front->data;
			h = "";
		}
		else
		{
			h += p->singleCode;
			c = p->ch;
			addToCharList(c, h);
			if (p->singleCode == "0")
				p->back->left = nullptr;
			else
				p->back->right = nullptr;
			p = front->data;
			h = "";
		}
	}

	return;
}

int createNum(int number, int mabna)
{
	if (number == 1)
	{
		return 1;
	}

	int result = 0;
	for (int i = 1; number >= mabna; i *= 2)
	{
		result += (number % mabna) * i;
		number /= mabna;

		if (number < mabna)
		{
			i *= 2;
			result += (number * i);
		}
	}
	return result;
}