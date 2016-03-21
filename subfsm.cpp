#include <iostream>
#include <fstream>
#include "subfsm.h"

using namespace std;

int main()
{ 
	subfsm("0.fsm");
	return 0;
}

longbool::longbool(unsigned char *a1, int size_bool1)
{
	if (a!=NULL) delete []a;
	size_tail=size_bool1%(sizeof(a1[0])*8);
	size_vec=size_bool1/(sizeof(a1[0])*8)+((size_bool1%(sizeof(a1[0])*8))?(1):(0));	
	a = new unsigned char [size_vec];
	for (int i=0; i<size_vec; a[size_vec-i-1]=a1[i], i++);
}

longbool::longbool (longbool& ar)
{
	if (this==&ar) return;
	if (a!=NULL) delete []a;
	size_vec = ar.size_vec;
	size_tail = ar.size_tail;
	a=new unsigned char [size_vec];
	for (int i=0; i<size_vec; a[i]=ar.a[i], i++);
	return;
}

void longbool::rawprint()
{
	cout<<"Print RAWlongbool:\n";
	int i=0;
	for (; i < size_vec; i++)
	{
		for(int j=sizeof(a[i])*8-1;j>-1;j--) cout<<((a[i]>>j)&1);
		cout<<'\n';
	}
}

void longbool::false_vec(int size_bool1)
{
	size_tail=size_bool1%(sizeof(this->a[0])*8);
	size_vec=size_bool1/(sizeof(this->a[0])*8)+((bool)size_tail);
	a = new unsigned char [size_vec];
	for (int i=0; i<size_vec; a[size_vec-i-1]=0, i++);
}

bool longbool::plus()
{
	int len=this->length(), i=0;
	while (true) {
		if (i==len) return 1;
		if (this->get(i)) this->unset(i);
		else {
			this->set(i);
			return 0;
		}
		i++;
	}
}

longbool longbool::operator =(longbool &ar)
{
	if (this==&ar) return *this;
	if (a!=NULL) delete []a;
	size_vec = ar.size_vec;
	size_tail = ar.size_tail;
	a=new unsigned char [size_vec];
	for (int i=0; i<size_vec; a[i]=ar.a[i], i++);
	return *this;
}

istream& operator >> (istream& strm, longbool &ar)
{
	deque my_list; char tmp; int i=0, num_of_deque=1, sizeofa=sizeof(ar.a[0])*8;
	ifstream tmpin;
	ofstream outtmp;
	while(((tmp=strm.get())!=EOF)&&(tmp!=32)&&(tmp!=10)) push_back(my_list,(tmp-48)); 						// stdin -> deque
	ar.size_tail=my_list.size%sizeofa;
	ar.size_vec=(my_list.size+((num_of_deque-1)*sizeofa*10))/sizeofa+(bool)ar.size_tail;
	if (ar.a!=NULL) delete []ar.a;
	ar.a = new unsigned char [ar.size_vec];
	for (i=0; i<ar.size_vec; ar.a[i]=0, i++);
	if(ar.size_tail) for(i=0;i<ar.size_tail; i++) if(dell_front(my_list)) ar.a[0]|=(1<<ar.size_tail-i-1);	// deque -> tail (if need)
	i=0;
	while(my_list.size!=0)																					// deque -> longbool
	{
		if(dell_front(my_list)) ar.a[(i/sizeofa)+(bool)ar.size_tail]|=(1<<sizeofa-i%sizeofa-1);
		i++;
	}
 	return strm;
}

ostream& operator << (ostream& strm, const longbool &ar)
{	

	int i=0; bool tmp, trigger=0;
	if(ar.size_tail)
	{
		for(int j=ar.size_tail-1;j>-1;j--)
		{
			tmp=((ar.a[0]>>j)&1);
			if (!trigger&&tmp)trigger=1;
			if (trigger) strm<<tmp;
		}
	i++;
	}
	for (; i < ar.size_vec; i++)
	{
		for(int j=sizeof(ar.a[i])*8-1;j>-1;j--)
		{
			tmp=((ar.a[i]>>j)&1);
			if (!trigger&&tmp)trigger=1;
			if (trigger) strm<<tmp;
		}
	}
	return strm;
}

void subfsm(const char* file){
	longbool vec;
	string name;
	int count=0, size=0;
	ifstream in;
	ofstream out;

	in.open(file);

	for (int i=0; i<6; i++) getline(in,name);//проитать шесть строк (пропустить)
	while (getline(in,name)) size++; //посчитать количество оставшихся строк
	vec.false_vec(size); //создать соответствующий битов вектор
	//in.close();
	in.clear();
	in.seekg(0, ios::beg);

	do { //цикл - все битовы вектора этой длины
		//out.open(name);
		//первые шесть строк
		cout<<'\n';
		for (int i=0; i<6; i++){
			getline(in,name);
			cout<<name<<'\n';
		}
		//вывести остальные строки по маске битового вектора (1 вывести, 0 не вывести) 
		//getline (cin,name);
		cout<<vec<<'\n';
	} while (!vec.plus());

	return;
}

void push_back(deque &l, bool k)
{
    element *tmp = new element;
    if(l.head == NULL) l.head = tmp;
    else l.tail -> next = tmp;
    tmp -> key = k;
    l.tail = tmp;
    l.size++;
}

bool dell_front(deque &l)
{
	bool out=l.head->key;
    if(l.tail == NULL) return 0;
    element *tmp = l.head;
    if (tmp -> next == NULL) l.tail = NULL;	
    l.head = tmp -> next;
    delete tmp;
    l.size--;
    return out;
}

void print_List(deque &l)
{
    element *tmp = l.head;
    if(tmp == NULL) {cout << "list is empty\n"; return;}
    cout << "list is:\n";
    cout << "size is: " << l.size << "\n";
    while (tmp != NULL)
    {
        cout << "element is: " << tmp -> key << "\n";
        tmp = tmp -> next;
    }
}
