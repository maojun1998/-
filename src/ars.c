#include <stdio.h>
#include <stdlib.h>

#include "ars.h"
static const char WeekStr[][] = {
    "一",
    "二",
    "三",
    "四",
    "五",
    "六",
    "日"
};
// pAirlineList is the global var to save the AirlineList address.
static PAirlineList pAirlineList = NULL;

/**
 *  Must keep that the PAirline is local at the heap 
 *  but not in the local calling argument stack.
 *
 */
void AddToAirlineList(
    _IN PAirline
)

{
    if (pAirlineList == NULL) {
        pAirlineList = (PAirlineList)malloc(sizeof(AirlineList));
        if (pAirlineList == NULL) {
            printlog("* AddToAirline():pAirlineList init fail()\n");
            exit(-1);
        }
    }
    pAirlineList->pPirline = PAirline;
    pAirlineList->pNextAirlineList = NULL;
    
}

void QuerryALLAirlineList(
    void
)

{
    PAirline pNewAirline ;
    PAirlineList pNewAirlineList = pAirlineList;
    
    printlog("* In QuerryALLAirlineList()\n");
    
    while (pNewAirlineList != NULL) {
        pNewAirline = pNewAirlineList->pAirline;
        ShowAirline(pNewAirline);
    }
    printlog("* Out QuerryALLAirlineList()\n");
}

void QuerryAirlineListByTname (
    _IN char *tName
)

{
    PAirlineList pNewAirlineList = pAirlineList;
    while (pNewAirlineList != NULL) {
        if (strcmp(&pNewAirlineList->terminalStationName[0], tName) == 0) {
            
            ShowAirline(pNewAirlineList->pAirline);
        }
        pNewAirlineList = pNewAirlineList->pNextAirlineList;
    }
    
}



static void ShowAirline(
    _IN PAirline pAirline
) 

{
    printf("航班号:%d, 飞机号:%d, 星期: %s, 余票额: %d\n", pAirline->flightNo, 
        pAirline->airplaneNo, WeekStr[(int)pAirline->week], pAirline->lastTicketNo);
    
}

static void AddToOrderList(
    _IN PAirline pAirline,
    _IN POrderList pOrderList
)

{
    POrderList pNewOrderList;
    POrderList pFrontOrderList;
    POrderList pOrderListHead = pAirline->pOrderList;
    printlog("* In AddToOrderList()");
    if (pOrderListHead == NULL || strcmp(&pOrderList->customerNmae[0], 
                                &pOrderListHead->customerNmae[0]) < 0) {
        pAirline->pOrderList = pOrderList;
        pOrderList->pNextOrderList = pOrderListHead;
        return;
    }
    while (pOrderListHead->pNextOrderList != NULL) {
        if (strcmp(&pOrderList->customerNmae[0], 
                        &pOrderListHead->pNextOrderList->customerNmae[0]) < 0) {
            
            pOrderList->pNextOrderList = pOrderListHead->pNextOrderList;
            pOrderListHead->pNextOrderList = pOrderList;
            return;
        }
        pOrderListHead = pOrderListHead->pNextOrderList;
    }
    pOrderListHead->pNextOrderList = pOrderList;
    printlog("* Out AddToOrderList()");
}



