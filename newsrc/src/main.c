#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "ars.h"
#include "main.h"


int main(void)
{
	int option;
    InitLogFile();
    InitGlobalAirLineListHead();
    printlog("In main()\n");
    LoadFileRecordToMem();
	do {
        OutputInfo();
        scanf("%d", &option);
		switch (option) {
		case 1	:
            InsertAirlineList();
            break;
		case 2	:
            Query();
            break;
		case 3	:
            OrderTicket();
            break;
		case 4	:
            RefundTicket();
            break;
		case 5	:
            OutputAllAirlineList();
            break;
		case 6	:
            ModifyAirlineList();
            break;
        case 7	:
            system("cls");
            break;
		case 0	: 
            StoryRecordToFile();
            break;
        default :
            break;
		}  
	} while (option);
    
    printlog("Out main()\n");
    CloseLogFile();
}

static void OutputInfo(void)
{
    printlog("In OutputInfo()\n");
	printf("\n----------------------------\n");
	printf("1---- 录入航班信息.\n");
	printf("2---- 查询航班信息.\n");
	printf("3---- 客户订票.\n");
	printf("4---- 客户退票.\n");
	printf("5---- 输出所有航班信息.\n");
	printf("6---- 修改航班信息.\n");
    printf("7---- 清屏.\n");
	printf("0---- 退出系统.\n");
	printf("----------------------------\n");
    printlog("Out OutputInfo()\n");
}

static void OutputAllAirlineList(void)
{
    ShowAllAirlineList();
    
}

static void InsertAirlineList(void)
{
    AirlineList *pAirlineList;
    time_t times;
    struct tm *localTimes;
    int years;
    int month;
    int days;
    int hours;
    int minutes;
    char *buffer;
    
    time(&times);
    localTimes = localtime (&times);
    printlog("In InsertAirlineList() \n");
    
    pAirlineList = Alloc_AirlineList();
    
    if (pAirlineList != NULL) {
        printf("* 请输入航班起点站: \n");
        scanf("%s", &pAirlineList->pAirline->startStationName[0]);
        fflush(stdin);
        
        printf("* 请输入航班终点站: \n");
        scanf("%s", &pAirlineList->pAirline->endStationName[0]);
        fflush(stdin);
        
        printf("* 请输入航班号:(主码) \n");
        scanf("%s", &pAirlineList->pAirline->flightNo[0]);
        fflush(stdin);
        
        printf("* 请输入飞机号: \n");
        scanf("%s", &pAirlineList->pAirline->airplaneNo[0]);
        fflush(stdin);

        printf("* 请输入最大乘客人数: \n");
        scanf("%d", &pAirlineList->pAirline->limitPeopleNo);
        pAirlineList->pAirline->lastTicketNo = 
            pAirlineList->pAirline->limitPeopleNo;
        fflush(stdin);
        
        printf("* 请输入航班起飞时间(2018-3-8-12:30表示2018年8月12日12点30): \n");
        scanf("%d-%d-%d-%d:%d", &years, &month, &days, &hours, &minutes);
        
        pAirlineList->pAirline->departureTime.tm_year = years - 1900;
        pAirlineList->pAirline->departureTime.tm_mon = month - 1;
        pAirlineList->pAirline->departureTime.tm_mday = days;
        pAirlineList->pAirline->departureTime.tm_hour = hours;
        pAirlineList->pAirline->departureTime.tm_min = minutes;
        pAirlineList->pAirline->departureTime.tm_sec = 0;
        pAirlineList->pAirline->departureTime.tm_isdst = localTimes->tm_isdst;
        fflush(stdin);
        
        printf("* 请输入航班到达时间(2018-3-8-12:30表示2018年8月12日12点30): \n");
        scanf("%d-%d-%d-%d:%d", &years, &month, &days, &hours, &minutes);
        fflush(stdin);
        pAirlineList->pAirline->arrivalTime.tm_year = years - 1900;
        pAirlineList->pAirline->arrivalTime.tm_mon = month - 1;
        pAirlineList->pAirline->arrivalTime.tm_mday = days;
        pAirlineList->pAirline->arrivalTime.tm_hour = hours;
        pAirlineList->pAirline->arrivalTime.tm_min = minutes;
        pAirlineList->pAirline->arrivalTime.tm_sec = 0;
        pAirlineList->pAirline->arrivalTime.tm_isdst = localTimes->tm_isdst;
        
        if (CheckAirlineListValid(pAirlineList, &buffer)) {
            AddArilineListToGlobalList(pAirlineList);
            printf("添加航线成功\n");
            printlog("添加航线成功\n");
        } else {
            Free_AirlineList(pAirlineList);
            printf("添加航线失败\n");
            printlog("添加航线失败\n");
        }
    }
    
    printlog("Out InsertAirlineList() \n");
}

void ModifyAirlineList(
    void
    )
{
    char flightNo[20];
    AirlineList *pAirlineList;

    int option;
    int years;
    int month;
    int days;
    int hours;
    int minutes;
    int maxPeople;

    do {
        printf("请输入要修改的航班号:\n");
        scanf("%s", flightNo);
        fflush(stdin);
    
        while (QueryByAirlineListFlightNo(flightNo, &pAirlineList)) {
            
            ShowAirlineList(pAirlineList);
            
            printf("\n----------------------------\n");
            printf("1---- 修改航班起点站.\n");
            printf("2---- 修改航班终点站.\n");
            printf("3---- 修改航班飞机号.\n");
            printf("4---- 修改航班起飞时间.\n");
            printf("5---- 修改航班到达时间.\n");
            printf("6---- 修改航班最大乘客人数.\n");
            printf("0---- 退出当前界面.\n");
            printf("----------------------------\n");
            scanf("%d", &option);
            fflush(stdin);
            
            switch(option) {
                case 1 : {
                    printf("* 请输入航班起点站: \n");
                    scanf("%s", &pAirlineList->pAirline->startStationName[0]);
                    fflush(stdin);
                    break;
                }
                case 2 : {
                    printf("* 请输入航班终点站: \n");
                    scanf("%s", &pAirlineList->pAirline->endStationName[0]);
                    fflush(stdin);
                    break;
                }
                case 3 : {
                    printf("* 请输入飞机号: \n");
                    scanf("%s", &pAirlineList->pAirline->airplaneNo[0]);
                    fflush(stdin);
                    break;
                }
                case 4 : {
                    printf("* 请输入航班起飞时间(2018-3-8-12:30表示2018年8月12日12点30): \n");
                    scanf("%d-%d-%d-%d:%d", &years, &month, &days, &hours, &minutes);
        
                    pAirlineList->pAirline->departureTime.tm_year = years - 1900;
                    pAirlineList->pAirline->departureTime.tm_mon = month - 1;
                    pAirlineList->pAirline->departureTime.tm_mday = days;
                    pAirlineList->pAirline->departureTime.tm_hour = hours;
                    pAirlineList->pAirline->departureTime.tm_min = minutes;
                    pAirlineList->pAirline->departureTime.tm_sec = 0;
                    fflush(stdin);
                    break;
                }
                case 5 : {
                    printf("* 请输入航班到达时间(2018-3-8-12:30表示2018年8月12日12点30): \n");
                    scanf("%d-%d-%d-%d:%d", &years, &month, &days, &hours, &minutes);
                    fflush(stdin);
                    pAirlineList->pAirline->arrivalTime.tm_year = years - 1900;
                    pAirlineList->pAirline->arrivalTime.tm_mon = month - 1;
                    pAirlineList->pAirline->arrivalTime.tm_mday = days;
                    pAirlineList->pAirline->arrivalTime.tm_hour = hours;
                    pAirlineList->pAirline->arrivalTime.tm_min = minutes;
                    pAirlineList->pAirline->arrivalTime.tm_sec = 0;
                    break;
                }
                case 6 : {
                    printf("* 请输入最大乘客人数: \n");
                    scanf("%d", &maxPeople);
                    fflush(stdin);
                    if (maxPeople >= pAirlineList->pAirline->limitPeopleNo 
                                        - pAirlineList->pAirline->lastTicketNo) {
                        pAirlineList->pAirline->lastTicketNo += maxPeople - 
                                        pAirlineList->pAirline->limitPeopleNo;
                        pAirlineList->pAirline->limitPeopleNo = maxPeople;
                        printf("修改最大乘客人数成功\n");
                    } else {
                        printf("修改最大乘客人数失败, 最大乘客数小于当前已经购买的票数.\n");
                    }
                }
                case 0 :
                    return;
                    break;
                default : 
                    break;
                }
            }
            
            if (pAirlineList == NULL) {
                printf("不存在该航班\n");
            }
        } while (pAirlineList != NULL);
    
    
}

static void Query(void)
{
    STATUS status;
    int option;
    AirlineList *pAirlineList;
    char flightNo[20];
    char startStationName[20];					
    char endStationName[20]; 
    time_t times;
    time_t newtimes;
    struct tm *localTimes;
    time_t difft;
    do {
        printf("\n----------------------------\n");
        printf("1---- 按照航班号查询.\n");
        printf("2---- 按照起点站终点站查询.\n");
        printf("3---- 查询最近一天航班.\n");
        printf("0---- 退出当前界面.\n");
        printf("----------------------------\n");
        
        scanf("%d", &option);
        fflush(stdin);
        
        switch (option) {
        case 1 : {
            printf("请输入航班号:\n");
            scanf("%s", flightNo);
            fflush(stdin);
            status = QueryByAirlineListFlightNo(flightNo, &pAirlineList);
            if (status) {
                ShowAirlineList(pAirlineList);
            } else {
                printf("没有找到该航班\n");
            }
            break;
        }
        case 2 : {
            printf("请输入起点站:\n");
            scanf("%s", startStationName);
            fflush(stdin);
            printf("请输入终点站:\n");
            scanf("%s", endStationName);
            fflush(stdin);
            
            status = QueryByAirlineListStartAndEnd(
                        startStationName,
                        endStationName,
                        &pAirlineList
                    );
            if (status) {
                ShowAllAirlineListByStation(pAirlineList);
            } else {
                printf("没有找到该航班\n");
            }
            FreeALLAirlineListByStation(pAirlineList);
            break;
        }
        case 3 : {
            printf("请输入起点站:\n");
            scanf("%s", startStationName);
            fflush(stdin);
            printf("请输入终点站:\n");
            scanf("%s", endStationName);
            fflush(stdin);

            status = QueryByAirlineListStartAndEnd(
                        startStationName,
                        endStationName,
                        &pAirlineList
                    );
            if (status) {
                printf("找到以下航班:\n");
                while (pAirlineList != NULL && pAirlineList->pAirline != NULL) {
                    time(&times);
                    newtimes = mktime(&pAirlineList->pAirline->departureTime);
                    //debug
                    //printf("\n\nnewtimes:%d, times:%d\n", newtimes, times);
                    difft = newtimes - times;
                    //debug
                    //printf("difftime : %d\n", difft);
                    if (difft > 0 && difft < 172800) {
                        printlog("In difft\n");
                        ShowAirlineList(pAirlineList);
                        printlog("Out difft\n");
                    }
                    pAirlineList = pAirlineList->pNextAirlineList;
                }
            } else {
                printf("没有找到该航班\n");
            }
            FreeALLAirlineListByStation(pAirlineList);
            break;
        }
        case 0 :
            break;
        }
    } while (option != 0);
}

static void OrderTicket(void)
{
    int option;
    char flightNo[20];
    AirlineList *pAirlineList;
    OrderList *pOrderList;
    WaitOrderList *pWaitOrderList;
    
    char goon_orderflag;
    char isAddWait;
    do {
        printf("\n----------------------------\n");
        printf("1---- 订票.\n");
        printf("0---- 退出当前界面.\n");
        printf("----------------------------\n");
        scanf("%d", &option);
        fflush(stdin);
        
        if (option == 1) {
                printf("请输入需要订票的航班号:\n");
                scanf("%s", flightNo);
                fflush(stdin);
                if (!QueryByAirlineListFlightNo(flightNo,&pAirlineList)) {
                    printf("没有查到该航班\n");
                    continue;
                }
                ShowAirlineList(pAirlineList);
                printf("\n**乘客信息**:\n");
                ShowAllOrderList(pAirlineList);
                printf("\n**排队等票乘客信息**:\n");
                ShowAllWaitOrderList(pAirlineList);
                printf("\n\n");
                do {
                    goon_orderflag = 'N';
                    pOrderList = Alloc_OrderList();

                    if (pOrderList != NULL) {
                        printf("请输入需要订票的用户名:\n");
                        scanf("%s", &pOrderList->customer.customerName[0]);
                        fflush(stdin);
                        printf("请输入需要订票的用户身份证(主码):\n");
                        scanf("%s", &pOrderList->customer.identyNo[0]);
                        fflush(stdin);
                        printf("请输入需要订票的数量:\n");
                        scanf("%d", &pOrderList->customer.needSeatNo);
                        fflush(stdin);
                        if (pAirlineList->pAirline->lastTicketNo >= pOrderList->customer.needSeatNo) {
                            if (AddOrderList(pOrderList, pAirlineList)) {
                                printf("订票成功\n");
                            }
                        } else {
                            isAddWait = 'N';
                            printf("票数不足\n");
                            printf("是否需要加入等待退票队伍(Y:是, N:否):\n");
                            scanf("%c", &isAddWait);
                            fflush(stdin);
                            if (isAddWait == 'Y')  {
                                pWaitOrderList = (WaitOrderList *)pOrderList;
                                AddWaitOrderList(pWaitOrderList, pAirlineList);
                                printf("加入退票队伍成功\n");
                            } else {
                                 Free_OrderList(pOrderList);
                            }
                        }
                    }
                    printf("是否继续订票(Y:是, N:否)\n");
                    scanf("%s", &goon_orderflag);
                    fflush(stdin);
                } while (goon_orderflag == 'Y'); 
        }        
    } while (option != 0);
      
}
static void RefundTicket(void)
{
    int option;
    char flightNo[20];
    char identyNo[20];
    AirlineList *pAirlineList;
    OrderList *pOrderList;
    WaitOrderList *pWaitOrderList;
    
    char goon_orderflag;
    char isAddWait;
    
    do {
        printf("\n----------------------------\n");
        printf("1---- 退票.\n");
        printf("0---- 退出当前界面.\n");
        printf("----------------------------\n");
        scanf("%d", &option);
        fflush(stdin);
        if (option == 1) {
            do {
                printf("请输入需要退票的航班号:\n");
                scanf("%s", flightNo);
                fflush(stdin);
                if (!QueryByAirlineListFlightNo(flightNo,&pAirlineList)) {
                    printf("没有查到该航班\n");
                    continue;
                }
                CheckWaitor(pAirlineList);
                ShowAirlineList(pAirlineList);
                printf("\n**乘客信息**:\n");
                ShowAllOrderList(pAirlineList);
                printf("\n**排队等票乘客信息**:\n");
                ShowAllWaitOrderList(pAirlineList);
                printf("\n\n");
                reRefoud:           // this lable is for goto here and reRefoud.
                printf("\n请输入需要退票的乘客的身份证号:\n");
                scanf("%s", &identyNo);
                fflush(stdin);
                if (QueryOrderListByIdentyNo(identyNo, pAirlineList, &pOrderList)) {
                    printf("\n\n查到待退票的乘客信息:\n");
                    ShowOrderList(pOrderList);
                    if (RemovByPorderList(pOrderList, pAirlineList, MM_FREE_FLAG)) {
                        printf("退票成功");
                    } else {
                        printf("退票失败\n");
                    }
                CheckWaitor(pAirlineList);
                } else {
                    printf("没有查到该乘客信息.\n");
                    goto reRefoud;
                }
                break;
            } while (1);
        }
        
    } while (option != 0);
}

static void CheckWaitor(AirlineList *pAirlineList)
{
    OrderList *pOrderList;
    OrderListHead *pOrderListHead;
    WaitOrderList *pWaitOrderList;
    WaitOrderListHead *pWaitOrderListHead;
    
    pOrderListHead = pAirlineList->pAirline->pOrderListHead;
    pWaitOrderListHead = pAirlineList->pAirline->pWaitOrderListHead;
    
    WaitOrderListHeadInitPistion(pWaitOrderListHead);
    
    while (pWaitOrderList = GetNextWaitOrderList(pWaitOrderListHead)) {
        pOrderList = (OrderList *)pWaitOrderList;
        if (AddOrderList(pOrderList, pAirlineList)) {
            printf("\n排队用户办理订票成功,详细信息:\n");
            ShowOrderList(pOrderList);
            if (!RemovByPwaitOrderList(pWaitOrderList, pAirlineList, MM_NOFREE_FLAG)) {
                printlog("Sytem error: RemovByPwaitOrderList is NOT Find the pWaitOrderList\n");
            }
            pOrderList->pNextOrderList = NULL;
        }
    }
}