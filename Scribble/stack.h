#pragma once


//A class for a stack
//NOTE: Put into another file
template <typename T>
class Stack
{
public:
	//Constructor
	Stack(int maxSize)
	{
		size = maxSize;
		currentIndex = -1; //This means that the stack is empty

		items = new T[size]; //Allocate memory for an array of variables
	}
	~Stack()
	{
		//Deallocate the memory
		delete[] items;
	}
	T* items;
	int size;
	int currentIndex;

	void push(T element)
	{
		currentIndex++;
		//If the stack's array is full, we create another array with double the size
		//The time complexity for this case is O(n) where n is size of the array
		if (currentIndex >= size)
		{
			size *= 2;
			T* temp = new T[size];
			//Copying elements
			for (int i = 0;i < size / 2;i++)
			{
				temp[i] = items[i];
			}
			//Delete the old array;
			delete[] items;
			//Set items equal to temp
			items = temp;
		}
		//Add item at the current index
		items[currentIndex] = element;
	}
	void pop()
	{
		//Set the item at the current index to zero and reduce the current index provided that the stack is not empty
		//REVISED: Pretend that the popped value doesn't exist, C++ doesn't have null types so idk what to do
		if (currentIndex > -1)
		{
			//items[currentIndex] = 0;
			currentIndex--;
		}

	}
	//Returns true when the stack is empty
	bool isEmpty()
	{
		return currentIndex < 0 ? true : false;
	}
	//Returns the top element of the stack
	T topElement()
	{
		return items[currentIndex];
	}

};


