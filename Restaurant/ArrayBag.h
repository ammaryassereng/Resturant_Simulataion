#pragma once
#define Max=1000
//Change the Max to a the specified Max

template <typename T>
class BagArray : public BagADT<T>
{
private:
	T arr[Max];
	int count;
	int getIndexOf(T it)
	{
		for (int i = 0; i < count; i++)
			if (it == arr[i])
				return i;
	}
public:
	BagArray()	//Constructor
	{
		count = 0;
	}
	int getCurrentSize() const
	{
		return count;
	}
	bool isEmpty() const	//Checks if the bag is empty
	{
		return count == 0;
	}
	bool add(const ItemType& newEntry)	//Adds a new element
	{
		if (count < Max)
		{
			arr[count++] = newEntry;
			return true;
		}
		return false;
	}
	bool remove(const ItemType& anEntry)	//Removes an element
	{
		for (int i = 0; i < count; i++)
			if (anEntry == arr[i])
			{
				arr[i] = arr[count - 1];
				return true;
			}
		return false;
	}
	void clear()	//Clears the whole bag
	{
		count = 0;
	}
	int getFrequencyOf(const ItemType& anEntry) const		//Gets the frequency of an given parameter
	{
		int f = 0;
		for (int i = 0; i < count; i++)
			if (arr[i] == anEntry)
				f++;
		return f;
	}
	bool contains(const ItemType& anEntry) const		//Checks whether an element exists or not
	{
		for (int i = 0; i < count; i++)
			if (arr[i] == anEntry)
				return true;
		return false;
	}
};
