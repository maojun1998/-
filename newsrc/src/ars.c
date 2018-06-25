#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "ars.h"


//logFile Handle.
static FILE *logFile;

void InitLogFile(
	void 
    )

{
    
	if ((logFile = fopen("LOG.log","w")) == NULL) {
        printf("InitLogFile() False\n");
		exit(-1);
	}
    printlog("Init LOG file.\n");
}

void CloseLogFile (
	void 
    )

{
	if (logFile != NULL) {
		printlog("LOG File close.\n");
        fclose(logFile);
	}
    
}

void printlog(
    const char *str
    )

{
    time_t times;
    struct tm *localTimes;
    if (logFile == NULL) {
        InitLogFile();
    }
    time(&times);
    localTimes = localtime (&times);
    fprintf(logFile, "* %d/%d/%d %d:%d:%d    : ",localTimes->tm_year+1900, 
            localTimes->tm_mon +1 , localTimes->tm_mday, localTimes->tm_hour, 
            localTimes->tm_min, localTimes->tm_sec);
            
    fprintf(logFile, str);
    fflush(logFile);
}


// global AirlineListHead
AirlineListHead globalAirLineListHead;


void InitGlobalAirLineListHead(
	void
    )
{
    globalAirLineListHead.pAirlineListHead = NULL;
    globalAirLineListHead.AirlineNo = 0;
}

void AddArilineListToGlobalList(
    _IN AirlineList *pAirlineList
    )
{
    
    AirlineList *newAirlineList;
    printlog("In AddArilineToList()\n");

    newAirlineList = globalAirLineListHead.pAirlineListHead;
    
    globalAirLineListHead.AirlineNo += 1;
    if (newAirlineList == NULL) {
        globalAirLineListHead.pAirlineListHead = pAirlineList;
        return ;
    }
    while (newAirlineList->pNextAirlineList != NULL) {
        newAirlineList = newAirlineList->pNextAirlineList;
    }
    newAirlineList->pNextAirlineList = pAirlineList;
    
    printlog("Out AddArilineToList()\n");
}

AirlineList *Alloc_AirlineList(
    void
    )
    
{
    AirlineList *pAirlineList;
    Airline *pAirline;
    OrderListHead *pOrderListHead;
    WaitOrderListHead *pWaitOrderListHead;
    
    printlog("In Alloc_AirlineList()\n");
    pAirlineList = (AirlineList *)malloc(sizeof(AirlineList));
    pAirline = (Airline *)malloc(sizeof(Airline));
    
    pOrderListHead = Alloc_OrderListHead();
    pWaitOrderListHead = Alloc_WaitOrderListHead();
    
    if (pAirlineList && pAirline && pOrderListHead && pWaitOrderListHead) {
        pAirline->pOrderListHead = pOrderListHead;
        pAirline->pWaitOrderListHead = pWaitOrderListHead;
        pAirlineList->pAirline = pAirline;
        pAirlineList->pNextAirlineList = NULL;
    } else {
        free(pAirlineList);
        free(pAirline);
        free(pOrderListHead);
        free(pWaitOrderListHead);
        pAirlineList = NULL;
    }
    printlog("Out Alloc_AirlineList()\n");
    return pAirlineList;
}
static const char errorMessage[][40] = {
    {"startStationName is OverFllow.\n"},
    {"endStationName is OverFllow.\n"},
    {"departuresTime is OverFllow.\n"},
    {"airplaneNo is OverFllow.\n"},
    {"limitPeopleNo is not equl lastTicketNo\n"},
    {"The AirList is Valid.\n"}
};

STATUS CheckAirlineListValid(
    _IN  AirlineList *pAirlineList,
    _OUT const char **buffer
    )
/*++

Routine Description:

    This function check the AirlineList argument valid. if the AirlineList is
    invalid, the argument buffer set the error message, if AirlineList is  

Arguments:

    pAirlineList - a pointer to a be detected AirlineList Object.
    buffer - a pointer to error message, if return STATUS_OK, this pointer is NULL.


Return Value:

    STATUS_OK is standfor the AirlineList is valid.
    STATUS_FALSE is standfor the AirlineList is invalid.

--*/
    
{
    printlog("In CheckAirlineListValid()\n");
    printlog("Out CheckAirlineListValid()\n");
    if (strlen(&pAirlineList->pAirline->startStationName[0]) > 19) {
        *buffer = errorMessage[0];
        return STATUS_FALSE;
    } else if (strlen(&pAirlineList->pAirline->endStationName[0]) > 19) {
        *buffer = errorMessage[1];
        return STATUS_FALSE;
    } else if (strlen(&pAirlineList->pAirline->airplaneNo[0]) > 19) {
        *buffer = errorMessage[3];
        return STATUS_FALSE;
    } else if (pAirlineList->pAirline->limitPeopleNo != 
                    pAirlineList->pAirline->lastTicketNo) {
        *buffer = errorMessage[4];
        return STATUS_FALSE;
    } else {
        *buffer = errorMessage[5];
        return STATUS_OK;
    }
    
    return STATUS_OK;
}

void Free_AirlineList(
    _IN AirlineList *pAirlineList
    )
{
    OrderList *pOrderList;
    OrderList *pNewOrderList;
    WaitOrderList *pWaitOrderList;
    WaitOrderList *pNewWaitOrderList;
    
    printlog("In Free_AirlineList()\n");
    if (pAirlineList != NULL) {
        if (pAirlineList->pAirline != NULL) {
            if(pAirlineList->pAirline->pOrderListHead != NULL) {
                pOrderList = pAirlineList->pAirline->pOrderListHead->pOrderList;
                pWaitOrderList = pAirlineList->pAirline->pWaitOrderListHead->pWaitOrderList;
                while (pOrderList != NULL) {
                    pNewOrderList = pOrderList->pNextOrderList;
                    Free_OrderList(pOrderList);
                    pOrderList = pNewOrderList;
                }
                while (pWaitOrderList != NULL) {
                    pNewWaitOrderList = pWaitOrderList->pNextWaitOrderList;
                    Free_OrderList(pWaitOrderList);
                    pWaitOrderList = pNewWaitOrderList;
                }
                free(pAirlineList->pAirline->pOrderListHead);
                free(pAirlineList->pAirline->pWaitOrderListHead);
            }
            free(pAirlineList->pAirline);
        }
        free(pAirlineList);
    }
    printlog("Out Free_AirlineList()\n");
}

void ShowAllAirlineList(
    void
    )

{
    AirlineList *pNewAirlineList;
    int AirlineNo;
    
    printlog("In ShowAllAirlineList()\n");
    
    pNewAirlineList = globalAirLineListHead.pAirlineListHead;
    for (AirlineNo = 0; AirlineNo < globalAirLineListHead.AirlineNo; AirlineNo++) {
        ShowAirlineList(pNewAirlineList);
        printf("\n**乘客信息**:\n");
        ShowAllOrderList(pNewAirlineList);
        printf("\n**排队等票乘客信息**:\n");
        ShowAllWaitOrderList(pNewAirlineList);
        printf("\n\n");
        pNewAirlineList = pNewAirlineList->pNextAirlineList;
    }
    printlog("Out ShowAllAirlineList()\n");
}

void ShowAirlineList(
    _IN AirlineList *pAirlineList
    )
{
    printlog("In ShowAirlineList()\n");
    printf(
        "\n* 航班号: %s,飞机号: %s, 起点站: %s, 终点站: %s\n",
        &pAirlineList->pAirline->flightNo[0], 
        &pAirlineList->pAirline->airplaneNo[0],
        &pAirlineList->pAirline->startStationName[0], 
        &pAirlineList->pAirline->endStationName[0]
    );
    printf(
        "* 起飞时间: %d年-%d月-%d日-%d:%d\n",
        pAirlineList->pAirline->departureTime.tm_year + 1900,
        pAirlineList->pAirline->departureTime.tm_mon + 1,
        pAirlineList->pAirline->departureTime.tm_mday,
        pAirlineList->pAirline->departureTime.tm_hour,
        pAirlineList->pAirline->departureTime.tm_min
    );
    printf(
        "* 到达时间: %d年-%d月-%d日-%d:%d\n",
        pAirlineList->pAirline->arrivalTime.tm_year + 1900,
        pAirlineList->pAirline->arrivalTime.tm_mon + 1,
        pAirlineList->pAirline->arrivalTime.tm_mday,
        pAirlineList->pAirline->arrivalTime.tm_hour,
        pAirlineList->pAirline->arrivalTime.tm_min
    );
    printf(
        "* 最大乘客数: %d, 剩余票数: %d. \n\n",
        pAirlineList->pAirline->limitPeopleNo,
        pAirlineList->pAirline->lastTicketNo
    );
    printlog("Out ShowAirlineList()\n");
}

STATUS QueryByAirlineListFlightNo(
    _IN char *flightNo,
    _OUT AirlineList **pPAirlineList
    )
{
    AirlineList *pAirlineList;
    
    printlog("In QueryByAirlineListFlightNo()\n");
    
    pAirlineList = globalAirLineListHead.pAirlineListHead;
    
    while (pAirlineList != NULL) {
        if (strcmp(flightNo, &pAirlineList->pAirline->flightNo[0]) == 0) {
            *pPAirlineList = pAirlineList;
            printlog("Out QueryByAirlineListFlightNo()\n");
            return STATUS_OK;
        }
        pAirlineList = pAirlineList->pNextAirlineList;
    }
    *pPAirlineList = NULL;
    printlog("Out QueryByAirlineListFlightNo()\n");
    return STATUS_FALSE;
}

STATUS RemoveByAirlineListFlightNo(
    _IN char *flightNo
    )
{
    AirlineList *pAirlineList;
    AirlineList *pNewAirlineList;
    printlog("In RemoveByAirlineListFlightNo()\n");
    
    pAirlineList = globalAirLineListHead.pAirlineListHead;
    if (pAirlineList != NULL) {
        if (strcmp(flightNo, &pAirlineList->pAirline->flightNo[0]) == 0) {
            globalAirLineListHead.pAirlineListHead = pAirlineList->pNextAirlineList;
            Free_AirlineList(pAirlineList);
            goto exit_normol_lable;
        } 
        while (pAirlineList->pNextAirlineList != NULL) {
            if (strcmp(flightNo,&pAirlineList->pNextAirlineList
                                    ->pAirline->flightNo[0]) == 0) {
                pNewAirlineList = pAirlineList->pNextAirlineList;
                pAirlineList->pNextAirlineList = pNewAirlineList->pNextAirlineList;
                Free_AirlineList(pNewAirlineList);
                goto exit_normol_lable;
            }
            pAirlineList = pAirlineList->pNextAirlineList;
        }
        goto exit_notnormol_lable;
    }
    
    printlog("pAirlineList : NULL");
    
    exit_notnormol_lable:
    printlog("Out RemoveByAirlineListFlightNo()\n");
    return STATUS_FALSE;
    
    exit_normol_lable:
    printlog("Out RemoveByAirlineListFlightNo()\n");
    globalAirLineListHead.AirlineNo -= 1;
    return STATUS_OK;
}

STATUS QueryByAirlineListStartAndEnd(
    _IN char *startStationName,
    _IN char *endStationName,
    _OUT AirlineList **pPAirlineList
    )
{
    AirlineList *pAirlineList;

    AirlineList *pNewAirlineList;
    
    STATUS status = STATUS_FALSE;
    
    pNewAirlineList = (AirlineList *)malloc(sizeof(AirlineList));
    pNewAirlineList->pAirline = NULL;
    pNewAirlineList->pNextAirlineList = NULL;
    *pPAirlineList = pNewAirlineList;
    
    printlog("In QueryByAirlineListFlightNo()\n");
    
    pAirlineList = globalAirLineListHead.pAirlineListHead;
    
    while (pAirlineList != NULL) {
        if (strcmp(startStationName, &pAirlineList->pAirline->startStationName[0]) == 0 
            && strcmp(endStationName, &pAirlineList->pAirline->endStationName[0]) == 0) {
                
            pNewAirlineList->pAirline = pAirlineList->pAirline;
            pNewAirlineList = pNewAirlineList->pNextAirlineList 
                                = (AirlineList *)malloc(sizeof(AirlineList));
            pNewAirlineList->pAirline = NULL;
            pNewAirlineList->pNextAirlineList = NULL;
            status = STATUS_OK;
        }
        pAirlineList = pAirlineList->pNextAirlineList;
    }
    
    printlog("Out QueryByAirlineListFlightNo()\n");
    return status;
    
}
void ShowAllAirlineListByStation(
    _IN AirlineList *pAirlineList
    )
{
    printlog("In ShowAllAirlineListByStation()\n");
    while (pAirlineList != NULL && pAirlineList->pAirline != NULL) {
        ShowAirlineList(pAirlineList);
        pAirlineList = pAirlineList->pNextAirlineList;
    }
    printlog("Out ShowAllAirlineListByStation()\n");
}

void FreeALLAirlineListByStation(
    _IN AirlineList *pAirlineList
    )
{
    AirlineList *pNewAirlineList;
    printlog("In FreeALLAirlineListByStation\n");
    while (pAirlineList != NULL) {
        pNewAirlineList = pAirlineList->pNextAirlineList;
        free(pAirlineList);
        pAirlineList = pNewAirlineList;
    }
    printlog("Out FreeALLAirlineListByStation\n");
}

OrderListHead *Alloc_OrderListHead(
    void
    )
{
    OrderListHead *pOrderListHead;
    printlog("In Alloc_OrderListHead()\n");
    pOrderListHead = (OrderListHead *)malloc(sizeof(OrderListHead));
    
    if (pOrderListHead != NULL) {
        pOrderListHead->pOrderList = NULL;
        pOrderListHead->orderNo = 0;
        pOrderListHead->current = 0;
        pOrderListHead->pCurrentOrderList = NULL;
    }
    printlog("Out Alloc_OrderListHead()\n");
    return pOrderListHead;
}

WaitOrderListHead *Alloc_WaitOrderListHead(
    void
    )
{
    WaitOrderListHead *pWaitOrderListHead;
    
    printlog("In Alloc_WaitOrderListHead()\n");
    
    pWaitOrderListHead = (WaitOrderListHead *)malloc(sizeof(WaitOrderListHead));
    
    if (pWaitOrderListHead != NULL) {
        pWaitOrderListHead->pWaitOrderList = NULL;
        pWaitOrderListHead->waitNo = 0;
        pWaitOrderListHead->current = 0;
        pWaitOrderListHead->pCurrentWaitOrderList = NULL;
    }
    printlog("Out Alloc_WaitOrderListHead()\n");
    return pWaitOrderListHead;
}

STATUS AddOrderList(
    _IN OrderList *pOrderList,
    _IN AirlineList *pAirlineList
    )
{
    OrderList *pNewOrderList;
    printlog("In AddOrderList()\n");
    if (pAirlineList->pAirline->lastTicketNo < pOrderList->customer.needSeatNo) {
        printlog("Out AddOrderList():lastTicketNo < needSeatNo\n");
        return STATUS_FALSE;
    }
    
    pAirlineList->pAirline->lastTicketNo -= pOrderList->customer.needSeatNo;
    pAirlineList->pAirline->pOrderListHead->orderNo += 1;
    
    pNewOrderList = pAirlineList->pAirline->pOrderListHead->pOrderList;
    
    if (pNewOrderList == NULL) {
        pAirlineList->pAirline->pOrderListHead->pOrderList = pOrderList;
        printlog("Out AddOrderList():pNewOrderList == NULL\n");
        return STATUS_OK;
    }
    while (pNewOrderList->pNextOrderList != NULL) {
        pNewOrderList = pNewOrderList->pNextOrderList;
    }
    pNewOrderList->pNextOrderList = pOrderList;
    printlog("Out AddOrderList()\n");
    return STATUS_OK;
}

OrderList *Alloc_OrderList(
    void
    )
{
    OrderList *pOrderList;
    printlog("In Alloc_OrderList()\n");
    pOrderList = (OrderList *)malloc(sizeof(OrderList));
    if (pOrderList != NULL) {
        pOrderList->pNextOrderList = NULL;
    }
    printlog("Out Alloc_OrderList()\n");
    return pOrderList;
}

void Free_OrderList(
    OrderList *pOrderList
    )
{
    printlog("In Free_OrderList()");
    if (pOrderList != NULL) {
        free(pOrderList);
    }
    printlog("Out Free_OrderList()");
}
void ShowOrderList(
    _IN OrderList *pOrderList
    )
{
    printlog("In ShowOrderList()\n");
    printf(
        "\n* 乘客姓名:%s, 乘客订票数量:%d\n", 
        &pOrderList->customer.customerName[0],
        pOrderList->customer.needSeatNo
    );
    printf(
        "* 乘客身份证号:%s\n\n", 
        &pOrderList->customer.identyNo[0]
    );
    printlog("Out ShowOrderList()\n");
}
    
void ShowAllOrderList(
    _IN AirlineList *pAirlineList
    )
{
    OrderList *pOrderList;
    
    printlog("In ShowAllOrderList()\n");
    pOrderList = pAirlineList->pAirline->pOrderListHead->pOrderList;
    while (pOrderList != NULL) {
        ShowOrderList(pOrderList);
        pOrderList = pOrderList->pNextOrderList;
    }
    printlog("Out ShowAllOrderList()\n");
}

STATUS QueryOrderListByIdentyNo(
    _IN  char *identyNo,
    _IN  AirlineList *pAirlineList,
    _OUT OrderList **pPOrderList
    )
{
    OrderList *pOrderList;
    int i = 0;
    printlog("In QueryOrderListByIdentyNo()\n");
    pOrderList = pAirlineList->pAirline->pOrderListHead->pOrderList;
    pAirlineList->pAirline->pOrderListHead->pCurrentOrderList = pOrderList;
    pAirlineList->pAirline->pOrderListHead->current = 0;

    while(pOrderList != NULL) {
        i++;
        pAirlineList->pAirline->pOrderListHead->current = i;
        pAirlineList->pAirline->pOrderListHead
                    ->pCurrentOrderList = pOrderList->pNextOrderList;
        
        if (strcmp(identyNo, &pOrderList->customer.identyNo[0]) == 0) {
            *pPOrderList = pOrderList;
            printlog("Out QueryOrderListByIdentyNo(): STATUS_OK\n");
            return STATUS_OK;
        }
        pOrderList = pOrderList->pNextOrderList;
    }
    *pPOrderList = NULL;
    
    printlog("Out QueryOrderListByIdentyNo(): STATUS_FALSE\n");
    
    return STATUS_FALSE;
}
STATUS RemovByPorderList(
    _IN OrderList *pOrderList,
    _IN AirlineList *pAirlineList,
    _IN char FLAG
    )
{
    OrderList *pNewOrderList;
    printlog("In RemovByPorderList()\n");
    pNewOrderList = pAirlineList->pAirline->pOrderListHead->pOrderList;
    
    if (pNewOrderList != NULL) {
        if (pOrderList == pNewOrderList) {
            
            pAirlineList->pAirline->pOrderListHead->pOrderList 
            = pOrderList->pNextOrderList;
            pAirlineList->pAirline->lastTicketNo += pOrderList->customer.needSeatNo;
            pAirlineList->pAirline->pOrderListHead->orderNo -= 1;
            if (FLAG) {
                Free_OrderList(pOrderList);
            }
            printlog("Out RemovByPorderList(): Head is Remove\n");
            return STATUS_OK;
        }
        while (pNewOrderList->pNextOrderList != NULL) {
            if (pNewOrderList->pNextOrderList == pOrderList) {
                pNewOrderList->pNextOrderList = pOrderList->pNextOrderList;
                pAirlineList->pAirline->lastTicketNo += pOrderList->customer.needSeatNo;
                pAirlineList->pAirline->pOrderListHead->orderNo -= 1;
                if (FLAG) {
                    Free_OrderList(pOrderList);
                }
                return STATUS_OK;
            }
            pNewOrderList = pNewOrderList->pNextOrderList;
        }
    }
    
    printlog("Out RemovByPorderList():Remove Flase\n");
    return STATUS_FALSE;
}

WaitOrderList *Alloc_WaitOrderList(
    void
    )
{
    WaitOrderList *pWaitOrderList;
    printlog("In Alloc_WaitOrderList()\n");
    pWaitOrderList = (WaitOrderList *)malloc(sizeof(WaitOrderList));
    
    if (pWaitOrderList != NULL) {
        pWaitOrderList->pNextWaitOrderList = NULL;
    }
    printlog("Out Alloc_WaitOrderList()\n");
    
    return pWaitOrderList;
}

void Free_WaitOrderList(
    WaitOrderList *pWaitOrderList
    )
{
    printlog("In Free_WaitOrderList()\n");
    if (pWaitOrderList != NULL) {
        free(pWaitOrderList);
    }
    printlog("Out Free_WaitOrderList()\n");
}

STATUS AddWaitOrderList(
    _IN WaitOrderList *pWaitOrderList,
    _IN AirlineList *pAirlineList
    )
{
    WaitOrderList *pNewWaitOrderList;
    printlog("In AddWaitOrderList()\n");
    pNewWaitOrderList = pAirlineList->pAirline->pWaitOrderListHead->pWaitOrderList;
    
    pAirlineList->pAirline->pWaitOrderListHead->waitNo += 1;
    if (pNewWaitOrderList == NULL) {
        pAirlineList->pAirline->pWaitOrderListHead->pWaitOrderList = pWaitOrderList;
        printlog("Out AddWaitOrderList pNewWaitOrderList = NULL ()\n");
        return STATUS_OK;
    }
    while (pNewWaitOrderList->pNextWaitOrderList != NULL) {
        pNewWaitOrderList = pNewWaitOrderList->pNextWaitOrderList;
    }
    pNewWaitOrderList->pNextWaitOrderList = pWaitOrderList;
    printlog("Out AddWaitOrderList ()\n");
    return STATUS_OK;
}

void ShowWaitOrderList(
    _IN WaitOrderList *pWaitOrderList
    )
{
    printlog("In ShowOrderList()\n");
    printf(
        "\n* 乘客姓名:%s, 乘客需要订票数量:%d\n", 
        &pWaitOrderList->waitCustomer.customerName[0],
        pWaitOrderList->waitCustomer.needSeatNo
    );
    printf(
        "* 乘客身份证号:%s\n\n", 
        &pWaitOrderList->waitCustomer.identyNo[0]
    );
    printlog("Out ShowOrderList()\n");
}
    
void ShowAllWaitOrderList(
    _IN AirlineList *pAirlineList
    )
{
    WaitOrderList *pWaitOrderList;
    printlog("In ShowAllWaitOrderList()\n");
    pWaitOrderList = pAirlineList->pAirline->pWaitOrderListHead->pWaitOrderList;
    while (pWaitOrderList != NULL) {
        ShowWaitOrderList(pWaitOrderList);
        pWaitOrderList = pWaitOrderList->pNextWaitOrderList;
    }
    printlog("Out ShowAllWaitOrderList()\n");
}
    
STATUS QueryWaitOrderListByIdentyNo(
    _IN  char *identyNo,
    _IN  AirlineList *pAirlineList,
    _OUT WaitOrderList **pPWaitOrderList
    )
{
    WaitOrderList *pWaitOrderList;
    
    int i = 0;
    printlog("In QueryWaitOrderListByIdentyNo\n");
    pWaitOrderList = pAirlineList->pAirline->pWaitOrderListHead->pWaitOrderList;
    pAirlineList->pAirline->pWaitOrderListHead->pCurrentWaitOrderList = pWaitOrderList;
    pAirlineList->pAirline->pWaitOrderListHead->current = 0;

    while(pWaitOrderList != NULL) {
        i++;
        pAirlineList->pAirline->pWaitOrderListHead->current = i;
        pAirlineList->pAirline->pWaitOrderListHead
                    ->pCurrentWaitOrderList = pWaitOrderList->pNextWaitOrderList;
        
        if (strcmp(identyNo, &pWaitOrderList->waitCustomer.identyNo[0]) == 0) {
            *pPWaitOrderList = pWaitOrderList;
            return STATUS_OK;
        }
        pWaitOrderList = pWaitOrderList->pNextWaitOrderList;
    }
    *pPWaitOrderList = NULL;
    printlog("Out QueryWaitOrderListByIdentyNo\n");
    return STATUS_FALSE;
    
}
    
STATUS RemovByPwaitOrderList(
    _IN WaitOrderList *pWaitOrderList,
    _IN AirlineList *pAirlineList,
    _IN char FLAG
    )
{
    WaitOrderList *pNewWaitOrderList;
    printlog("In RemovByPwaitOrderList\n");
    pNewWaitOrderList = pAirlineList->pAirline->pWaitOrderListHead->pWaitOrderList;
    
    if (pNewWaitOrderList != NULL) {
        if (pWaitOrderList == pNewWaitOrderList) {
            pAirlineList->pAirline->pWaitOrderListHead->pWaitOrderList = pNewWaitOrderList->pNextWaitOrderList;
            if (FLAG) {
                Free_WaitOrderList(pWaitOrderList);
            }
            pAirlineList->pAirline->pWaitOrderListHead->waitNo -= 1;
            return STATUS_OK;
        }
        while (pNewWaitOrderList->pNextWaitOrderList != NULL) {
            if (pNewWaitOrderList->pNextWaitOrderList == pWaitOrderList) {
                pNewWaitOrderList->pNextWaitOrderList = pWaitOrderList->pNextWaitOrderList;
                if (FLAG) {
                    Free_WaitOrderList(pWaitOrderList);
                }
                pAirlineList->pAirline->pWaitOrderListHead->waitNo -= 1;
                return STATUS_OK;
            }
            pNewWaitOrderList = pNewWaitOrderList->pNextWaitOrderList;
        }
    }
    printlog("Out RemovByPwaitOrderList\n");
    return STATUS_FALSE;
}

WaitOrderList *GetNextWaitOrderList(
    _IN WaitOrderListHead *pWaitOrderListHead
    )
{
    WaitOrderList *pWaitOrderList;
    printlog("In GetNextWaitOrderList\n");
    if ((pWaitOrderList = pWaitOrderListHead->pCurrentWaitOrderList)) {
        pWaitOrderListHead->current += 1;
        pWaitOrderListHead->pCurrentWaitOrderList = pWaitOrderList->pNextWaitOrderList;
    }
    printlog("Out GetNextWaitOrderList\n");
    return pWaitOrderList;
}

void WaitOrderListHeadInitPistion(
    _IN WaitOrderListHead *pWaitOrderListHead
    )
{
    printlog("In LesskWaitOrderListInit\n");
    pWaitOrderListHead->current = 0;
    pWaitOrderListHead->pCurrentWaitOrderList = pWaitOrderListHead->pWaitOrderList;
    printlog("Out LesskWaitOrderListInit\n");
}

void PrintWaringStderror(char *buffer)
{
    // Not implement , this is just for declation.
}
