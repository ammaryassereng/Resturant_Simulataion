#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include"..\LinkedList.h"
#include<fstream>
#include "..\Events\Event.h"
#include "..\Events\ArrivalEvent.h"
#include"..\PromotionEvent.h"
#include "..\CancelEvent.h"
#include "Order.h"

// it is the maestro of the project
class Restaurant
{
private:
	GUI* pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file
	LinkedList<Cook*> AvailableCooks;		//List to available cooks
	LinkedList<Cook*> NonAvailableCooks;	//List to cooks who are in break or serving an order
	Queue<Order*> NormalOrders;				//List of normal orders sorted  to be served
	Queue<Order*> VeganOrders;				//List of Vegan Orders to be served
	LinkedList<Order*> VIPOrders;			//List of VIP orders to be served
	LinkedList<Order*> ArrivalOrders;		//List of Orders sorted according to arrival time <<for the gui drawing>>
	LinkedList<Order*> InServiceOrders;		//List of Orders in the service operation
	LinkedList<Order*> DoneOrders;			//List of Done orders
	Queue<Cook*> justAssigned;		//Contains the Cooks who were assigned orders in the last timestep	
	LinkedList<Cook*> injuredCooks;		//List of Injured Cooks

	int TimeForPromotion;		//Auto-Promotion
	int VIP_WT;		//For Urgent VIP Orders

	double InjProb;		//Probabilty of the Cook getting Injured
	int RstPrd;			//Rest Period of the Injured Cook

	string File;
	int NumberOfPromotion;
	int NumberOfUrgent;
	int TotalNormalOrdrs;

public:

	Restaurant();
	~Restaurant();

	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();

	void FillDrawingList();

	void PromoteOrder(Order* pOrd);
	void GeneralPromote(int TS); // this function will promote all normal orders with wait time higher than that of the Time of promotion
	void SortVIPOrders(); //this function will sort all vip orders 
	void IncrementWaiting(); //this function will increment the waiting time of all orders in the waiting list
							//it should be called in each time step
							//it will also affect the priority of the vip orders
	void SortDone();		// this function will sort Done orders according to their finish time the according to their service time
							// note that all done orders are initially sorted according to their finish time

	void FileLoadingFunction();		//Loads the input file
	void OutputFile();				// this function will make the output file
	
	// Functions that add to lists
	void ADDtoAvailableCooks(Cook* pCook);
	void ADDtoNonAvailableCooks(Cook* pCook);
	void ADDtoNormalOrders(Order* pOrd);
	void ADDtoVeganOrders(Order* pOrd);
	void ADDtoVipOrders(Order* pOrd);
	void ADDtoArvOrders(Order* pOrd);
	void ADDtoInServiceOrders(Order* pOrd);
	void ADDtoDoneOrders(Order* pOrd);


	//Functions To find Data using the ID
	Order* FindOrderInArvOrders(int ID);
	Order* FindOrderInInserviceList(int ID);
	Cook* FindCookInAvailableCooks(int ID);
	Cook* FindCookInNonAvailableCooks(int ID);



	//Functions to remove from lists
	void RemoveFromAvailableCooks(Cook* pCook);
	void RemoveFromNonAvailableCooks(Cook* pCook);
	void RemoveFromNormalOrders(Order* pOrd);
	void RemoveFromVeganOrders(Order* pOrd);
	void RemoveFromVipOrders(Order* pOrd);
	void RemoveFromArrivalOrders(Order* pOrd);
	void RemoveFromInServiceOrders(Order* pOrd);


	void countCookTypes(int& N, int& G, int& V);	//Counts & returns the number of cooks of each type
	void countInserviceTypes(int& N, int& G, int& V);	//Counts & returns the number of Inservice Orders of each type

	//Cook Getters
	Cook* getVc();		//Gets a Cook for VIP Order
	Cook* getNc();		//Gets a Cook for Normal Order
	Cook* getGc();		//Gets a Cook for veGan Order

	//Order Transition Functions through different Status
	void serveOrders(int timestep);		//Should be Called every timestep in the Simulator
	void serveUrgents(int timestep);	//Serves Urgent Orders & Called at the beginning of serveOrders fn
	void FinishOrders(int timestep);	//Responsible for Finishing Orders

	//Cooks' Breaks & Injuries
	void injureCooks(int timestep);		//Responsible for Cook Injury & Recovery
	void takeBreak(int timestep);		//Responsible for Break & Work Times
	int getInjuredCooks();

	/// ================================================================================================== 

	void Simulator(PROG_MODE);				//Simulator Function

};

#endif