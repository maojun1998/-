#ifndef __ARS_H__   //Airline Reservation System
#define __ARS_H__

#include <time.h>

/*
 *  This is standfor function argument in, out.
 */

#define _IN 
#define _OUT
#define _IN_OUT


/*
 *  Memory Free Flag, this Flag is just for Free function.
 */
#define MM_FREE_FLAG    (1)
#define MM_NOFREE_FLAG  (0)
/*
 *  STATUS return info.
 */
typedef char STATUS;

#define STATUS_OK       (1)
#define STATUS_FALSE    (0)


typedef  struct _Airline {
    char startStationName[20];					
    char endStationName[20]; 					
    struct tm departureTime;						
    struct tm arrivalTime;							
    char flightNo[20];	
    char airplaneNo[20];
    int  limitPeopleNo;
    int  lastTicketNo;
    struct _OrderListHead *pOrderListHead;
    struct _WaitOrderListHead *pWaitOrderListHead;
} Airline;

typedef struct _AirlineList {
	struct _Airline *pAirline;
	struct _AirlineList *pNextAirlineList;
} AirlineList;


typedef struct _AirlineListHead {
	struct _AirlineList *pAirlineListHead;
	int AirlineNo;
} AirlineListHead;

typedef struct _OrderListHead {
	struct _OrderList *pOrderList;
	int orderNo;
    int current;                          // Just for Query Function used.
    struct _OrderList *pCurrentOrderList; // Just for Query Function used.
} OrderListHead;


typedef struct _WaitOrderListHead {
	struct _WaitOrderList *pWaitOrderList;
	int waitNo;
    int current;
    struct _WaitOrderList *pCurrentWaitOrderList;
} WaitOrderListHead;



typedef struct _Customer {
    char customerName[20];
    int  needSeatNo;
    char identyNo[20];
} Customer;
typedef struct _WaitCustomer {
	char customerName[20];
    int  needSeatNo;
	char identyNo[20];
} WaitCustomer;

typedef struct _OrderList {
    Customer customer;
    struct _OrderList *pNextOrderList;
} OrderList;

typedef struct _WaitOrderList {
    WaitCustomer waitCustomer;
    struct _WaitOrderList *pNextWaitOrderList;
} WaitOrderList;



// Debug Function declation.
void InitLogFile(
	void 
    );
    
void CloseLogFile (
	void 
    );
    
void printlog(
    const char *str
    );

    
// These function is for ArilineList.

void AddArilineListToGlobalList(
    _IN AirlineList *pAirlineList
    );
    
AirlineList *Alloc_AirlineList(
    void
    );
    
STATUS CheckAirlineListValid(
    _IN  AirlineList *pAirlineList,
    _OUT const char **buffer
    );
    
void Free_AirlineList(
    _IN AirlineList *pAirlineList
    );  
    
void ShowAllAirlineList(
    void
    );
    
void ShowAirlineList(
    _IN AirlineList *pAirlineList
    );
    
STATUS QueryByAirlineListFlightNo(
    _IN char *flightNo,
    _OUT AirlineList **pPAirlineList
    );
    
STATUS QueryByAirlineListStartAndEnd(
    _IN char *startStationName,
    _IN char *endStationName,
    _OUT AirlineList **pPAirlineList
);

void ShowAllAirlineListByStation(
    _IN AirlineList *pAirlineList
    );
    
void FreeALLAirlineListByStation(
    _IN AirlineList *pAirlineList
    );

STATUS RemoveByAirlineListFlightNo(
    _IN char *flightNo
    );

    
// This function declation for OrderList.

OrderListHead *Alloc_OrderListHead(
    void
    );
    
OrderList *Alloc_OrderList(
    void
    );
    
void Free_OrderList(
    OrderList *pOrderList
    );
    
STATUS AddOrderList(
    _IN OrderList *pOrderList,
    _IN AirlineList *pAirlineList
    );
    
void ShowOrderList(
    _IN OrderList *pOrderList
    );
    
void ShowAllOrderList(
    _IN AirlineList *pAirlineList
    );
    
STATUS QueryOrderListByIdentyNo(
    _IN  char *identyNo,
    _IN  AirlineList *pAirlineList,
    _OUT OrderList **pPOrderList
    );
    
STATUS RemovByPorderList(
    _IN OrderList *pOrderList,
    _IN AirlineList *pAirlineList,
    _IN char FLAG
    );

// The following function used for  WaitOrderList.

WaitOrderList *Alloc_WaitOrderList(
    void
    );   
WaitOrderListHead *Alloc_WaitOrderListHead(
    void
    );  
void Free_WaitOrderList(
    WaitOrderList *pWaitOrderList
    );
STATUS AddWaitOrderList(
    _IN WaitOrderList *pWaitOrderList,
    _IN AirlineList *pAirlineList
    );
    
void ShowWaitOrderList(
    _IN WaitOrderList *pWaitOrderList
    );
    
void ShowAllWaitOrderList(
    _IN AirlineList *pAirlineList
    );
    
STATUS QueryWaitOrderListByIdentyNo(
    _IN  char *identyNo,
    _IN  AirlineList *pAirlineList,
    _OUT WaitOrderList **pPWaitOrderList
    );
    
STATUS RemovByPwaitOrderList(
    _IN WaitOrderList *pWaitOrderList,
    _IN AirlineList *pAirlineList,
    _IN char FLAG
    );
    
WaitOrderList *GetNextWaitOrderList(
    _IN WaitOrderListHead *pWaitOrderListHead
    );
    
void WaitOrderListHeadInitPistion(
    _IN WaitOrderListHead *pWaitOrderListHead
    );
    
void PrintWaringStderror(
    char *buffer
    );
#endif //__ARS_H__