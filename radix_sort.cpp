#include <iostream>
#include <fstream>
using namespace std;

class listNode
{
	public:
		friend class linkedListStack;
		friend class linkedListQueue;
		friend class RadixSort;
		int data;
		listNode* next;
		string dummy;
		listNode(int d)
		{
			data = d;
			next = NULL;
		}
};	

class linkedListStack
{
	public:
		friend class RadixSort;
		listNode* top;

		linkedListStack()
		{
			top = NULL;
		}

		void push(listNode* new_node)
		{
			if(top == NULL){
				top = new_node;
			}
			else{
				new_node->next = top;
				top = new_node;
			}		
		}
		listNode* pop()
		{
			listNode* old_top = NULL;
			if(!isEmpty()){
				old_top = top;
				top = top->next;
			}
			return old_top;
		}
		bool isEmpty()
		{
			return top == NULL;
		}
		string printStack()
		{
			string stack;
			if(!isEmpty())
			{
				stack = "Top -> ";
				listNode* temp = top; 
				while(temp->next != NULL)
				{
					stack +=string("(")+ to_string(temp->data)+string(",")
						  +to_string(temp->next->data) + (")->");
					temp = temp->next;
				}
				stack += string("(") + to_string(temp->data)
						+string(", NULL)-->NULL\n");
			}
			return stack;
		}
};

class linkedListQueue
{
	public:
		friend class RadixSort;
		listNode* head;
		listNode* tail;
		listNode* dummy;
		string sorted_q;

		linkedListQueue()
		{
			dummy = new listNode(0);
			head = tail = dummy;
		}

		void addTail(linkedListQueue* Q, listNode* n)
		{	
			n->next = NULL;

			if(Q->isEmpty(Q)){
				Q->head = n;
				Q->tail = n;
			}
			else {
				Q->tail->next = n;
				Q->tail = n;
			}
		}

		listNode* deleteFront(linkedListQueue* Q)
		{
			listNode* temp = NULL;
			if(!(Q->isEmpty(Q))){
				temp = Q->head;
				Q->head = temp->next;
			}
			return temp;
		}

		bool isEmpty(linkedListQueue* Q)
		{
			bool empty;
			if(Q->head == dummy && Q->tail == Q->dummy)
				empty = true;
			else if(Q->head == NULL)
				empty = true;
			else empty = false;

			return empty;
		}

		string printQueue(int index)
		{
			string q;
			sorted_q ="";
			if(!(this->isEmpty(this)))
			{
				listNode* temp = this->head;
				q+= "Front("+to_string(index)+")->";

				while(temp->next != NULL){
					q+="("+to_string(temp->data)+","+ to_string(temp->next->data)+")->";
					sorted_q += to_string(temp->data) + " ";
					temp = temp->next;
				}
				q+="("+to_string(temp->data)+",NULL)->NULL\n";
				q+="Tail("+to_string(index)+")->("+to_string(temp->data)+",NULL)->NULL\n";
				sorted_q += to_string(temp->data)+" ";
			}
			return q;
		}
};

class RadixSort{
	const static int tableSize = 10;
	public:
		linkedListQueue* hashTable[2][tableSize];
		int data;
		int currentTable;
		int previousTable;
		int maxDigits;
		int currentDigit;
		int largestNumber;
		int currentQueue;
		linkedListStack* lls;
		string sorted_q;

		void Sort(listNode* top, ofstream& out,ofstream& debug)
		{	
			
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < tableSize; j++)
				{
					hashTable[i][j] = new linkedListQueue();
				}
			}

			maxDigits = getMaxDigits(largestNumber);

			currentDigit = 0;
			currentTable = 0;

			dumpStack(top, currentDigit, currentTable);

			previousTable = 0;
			currentTable = 1;
			currentQueue = 0;

			while(currentDigit <= maxDigits)
			{
				while(currentQueue < tableSize)
				{
					while(!(hashTable[previousTable][currentQueue]->isEmpty(hashTable[previousTable][currentQueue])))
					{
						listNode* node = hashTable[previousTable][currentQueue]->deleteFront(hashTable[previousTable][currentQueue]);
						int digit = getDigit(node, currentDigit);
						int hashIndex = this->hashIndex(digit);
						hashTable[currentTable][hashIndex]->addTail(hashTable[currentTable][hashIndex], node);
					}
					currentQueue++;
				}
				string table = printTable(hashTable[currentTable]);
				debug<<table;

				int temp = currentTable;
				currentTable = previousTable;
				previousTable = temp;
				currentQueue = 0;	
				currentDigit++;
			}
			out<<sorted_q;
		}

		listNode* loadStack(ifstream& in,ofstream& debug)
		{
			lls = new linkedListStack();

			int largest_num = 0;
			int data;
			while(in >> data)
			{
				if(data > largest_num)
					largest_num = data;
				listNode* new_node = new listNode(data);
				lls->push(new_node);
			}
			largestNumber = largest_num;
			string stack = lls->printStack();
			
			debug<<stack<<endl;
			return lls->top;
		}

		void dumpStack(listNode* top,int currentDigit, int currentTable)
		{
			while(!(lls->isEmpty()))
			{
				listNode* n = lls->pop();
				int digit = getDigit(n,currentDigit);
				int hashIndex = digit;
				hashTable[currentTable][hashIndex]->addTail(hashTable[currentTable][hashIndex],n);
			}
			string table = printTable(hashTable[currentTable]);
			
		}
		// tableIndex()// which index of the given hash table (HT)
		int getMaxDigits(int n)
		{
			int num_of_digits = 1;
			while(n > 10)
			{
				n = n/10;
				num_of_digits++;
			}
			return num_of_digits;
		}
		int getDigit(listNode* n, int currentDigit)//returns the current digit of the data in the node
		{	
			int num = n->data;
			while(currentDigit > 0)
			{
				num /= 10;
				currentDigit--;
			}
			return num%10;
		}
		int hashIndex(int digit)
		{	
			return digit;
		}
		string printTable(linkedListQueue* table[tableSize])
		{	
			int i = 0;
			string q;
			while(i < tableSize)
			{
				if(!(table[i]->isEmpty(table[i])))
				{
					q += table[i]->printQueue(i);
					sorted_q = table[i]->sorted_q;
				}
				i++;
			}
			q+="\n";
			return q;
		}
};

int main(int argc, char const *argv[])
{
	
	ifstream inFile;
	ofstream outFile1,debug;
	inFile.open(argv[1]);
	outFile1.open(argv[2]);
	debug.open(argv[3]);

	RadixSort* rs = new RadixSort();
	listNode* top = rs->loadStack(inFile,debug);
	rs->Sort(top, outFile1,debug);

	inFile.close();
	outFile1.close();
	debug.close();

	return 0;
}