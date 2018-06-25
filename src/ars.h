#ifndef __ARS_H__   //Airline Reservation System
#define __ARS_H__

/*
 *  This is standfor function argument in, out.
 */
 
#ifdef _DEBUG_MODE // debug

#define printlog(x)                     \
        do {                            \
           fprintf(strerr, x);          \
        } while (0)
            
#else printlog(x) 
#endif //debug

#define _IN 
#define _OUT
#define _IN_OUT

typedef enum _Week {
    Mon, Tue, Wed, Thu, Fri, Sat, Sun
} Week;

typedef  struct _Airline {
    char terminalStationName[20];
    int  flightNo;
    int  airplaneNo;
    Week week;
    int  limitPeopleNo;
    int  lastTicketNo;
    struct _OrderList *pOrderList;
    struct _WaitOrderList *pWaitOrderList;
} Airline, *PAirline;

typedef struct _AirlineList {
    PAirline  pAirline;
    struct _AirlineList *pNextAirlineList;
} AirlineList, *PAirlineList;

typedef enum _SheepLevel {
    FirstLevel, SecondLevel, ThirdLevel
} ShipLevel;

typedef struct _Customer {
    char customerNmae[20];
    int  orderTicketNo;
    ShipLevel shipLevel;
} Customer, *PCustomer;

typedef struct _WaitCustomer {
    char customerNmae;
    int  needTicketNo;
} WaitCustomer, *PWaitCustomer;

typedef struct _OrderList {
    Customer customer;
    struct _OrderList *pNextOrderList;
} OrderList, *POrderList;

typedef struct _WaitOrderList {
    WaitCustomer waitCustomer;
    struct _WaitCustomer *pNextWaitCustomer;
} WaitOrderList, *PWaitOrderList;

// The following is for function declation

void AddToAirlineList(
    _IN PAirline
);

void QuerryALLAirlineList(
    void
);


#endif // __ARS_H__