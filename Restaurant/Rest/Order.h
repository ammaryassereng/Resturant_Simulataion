#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	int Distance;	//The distance (in meters) between the order location and the resturant 
	                
	double totalMoney;	//Total order money

	int ArrTime, WaitTime, ServTime, FinishTime;	//arrival, service start, and finish times
	int size;

	double priority;
	
	//
	// TODO: Add More Data Members As Needed
	//

public:
	Order(int ID, ORD_TYPE r_Type);
	virtual ~Order();

	int GetID();

	void ChangeType(ORD_TYPE);
	ORD_TYPE GetType() const;

	void SetDistance(int d);
	int GetDistance() const;

	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;
	
	//
	// TODO: Add More Member Functions As Needed
	//
	void SetSize(int S);
	int GetSize() const;

	void SetMoney(double mon);
	double GetMoney() const;

	//Time Setters and Getters

	void setArrivalT(int a);
	int getArrivalT() const;

	void setWaitT(int w);
	int getWaitT() const;

	void setServT(int w);
	int getServT() const;

	void setFinishT(int f);
	int getFinishT() const;

	void InitializePriority();	// this function will give the priority a value
	double GetPriority();		//this function will return the priority

	bool operator > (Order* Pord);
	bool operator < (Order* Pord);
	bool operator == (Order* Pord);
};

#endif