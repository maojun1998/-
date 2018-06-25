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
	printf("1---- ¼�뺽����Ϣ.\n");
	printf("2---- ��ѯ������Ϣ.\n");
	printf("3---- �ͻ���Ʊ.\n");
	printf("4---- �ͻ���Ʊ.\n");
	printf("5---- ������к�����Ϣ.\n");
	printf("6---- �޸ĺ�����Ϣ.\n");
    printf("7---- ����.\n");
	printf("0---- �˳�ϵͳ.\n");
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
        printf("* �����뺽�����վ: \n");
        scanf("%s", &pAirlineList->pAirline->startStationName[0]);
        fflush(stdin);
        
        printf("* �����뺽���յ�վ: \n");
        scanf("%s", &pAirlineList->pAirline->endStationName[0]);
        fflush(stdin);
        
        printf("* �����뺽���:(����) \n");
        scanf("%s", &pAirlineList->pAirline->flightNo[0]);
        fflush(stdin);
        
        printf("* ������ɻ���: \n");
        scanf("%s", &pAirlineList->pAirline->airplaneNo[0]);
        fflush(stdin);

        printf("* ���������˿�����: \n");
        scanf("%d", &pAirlineList->pAirline->limitPeopleNo);
        pAirlineList->pAirline->lastTicketNo = 
            pAirlineList->pAirline->limitPeopleNo;
        fflush(stdin);
        
        printf("* �����뺽�����ʱ��(2018-3-8-12:30��ʾ2018��8��12��12��30): \n");
        scanf("%d-%d-%d-%d:%d", &years, &month, &days, &hours, &minutes);
        
        pAirlineList->pAirline->departureTime.tm_year = years - 1900;
        pAirlineList->pAirline->departureTime.tm_mon = month - 1;
        pAirlineList->pAirline->departureTime.tm_mday = days;
        pAirlineList->pAirline->departureTime.tm_hour = hours;
        pAirlineList->pAirline->departureTime.tm_min = minutes;
        pAirlineList->pAirline->departureTime.tm_sec = 0;
        pAirlineList->pAirline->departureTime.tm_isdst = localTimes->tm_isdst;
        fflush(stdin);
        
        printf("* �����뺽�ൽ��ʱ��(2018-3-8-12:30��ʾ2018��8��12��12��30): \n");
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
            printf("��Ӻ��߳ɹ�\n");
            printlog("��Ӻ��߳ɹ�\n");
        } else {
            Free_AirlineList(pAirlineList);
            printf("��Ӻ���ʧ��\n");
            printlog("��Ӻ���ʧ��\n");
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
        printf("������Ҫ�޸ĵĺ����:\n");
        scanf("%s", flightNo);
        fflush(stdin);
    
        while (QueryByAirlineListFlightNo(flightNo, &pAirlineList)) {
            
            ShowAirlineList(pAirlineList);
            
            printf("\n----------------------------\n");
            printf("1---- �޸ĺ������վ.\n");
            printf("2---- �޸ĺ����յ�վ.\n");
            printf("3---- �޸ĺ���ɻ���.\n");
            printf("4---- �޸ĺ������ʱ��.\n");
            printf("5---- �޸ĺ��ൽ��ʱ��.\n");
            printf("6---- �޸ĺ������˿�����.\n");
            printf("0---- �˳���ǰ����.\n");
            printf("----------------------------\n");
            scanf("%d", &option);
            fflush(stdin);
            
            switch(option) {
                case 1 : {
                    printf("* �����뺽�����վ: \n");
                    scanf("%s", &pAirlineList->pAirline->startStationName[0]);
                    fflush(stdin);
                    break;
                }
                case 2 : {
                    printf("* �����뺽���յ�վ: \n");
                    scanf("%s", &pAirlineList->pAirline->endStationName[0]);
                    fflush(stdin);
                    break;
                }
                case 3 : {
                    printf("* ������ɻ���: \n");
                    scanf("%s", &pAirlineList->pAirline->airplaneNo[0]);
                    fflush(stdin);
                    break;
                }
                case 4 : {
                    printf("* �����뺽�����ʱ��(2018-3-8-12:30��ʾ2018��8��12��12��30): \n");
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
                    printf("* �����뺽�ൽ��ʱ��(2018-3-8-12:30��ʾ2018��8��12��12��30): \n");
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
                    printf("* ���������˿�����: \n");
                    scanf("%d", &maxPeople);
                    fflush(stdin);
                    if (maxPeople >= pAirlineList->pAirline->limitPeopleNo 
                                        - pAirlineList->pAirline->lastTicketNo) {
                        pAirlineList->pAirline->lastTicketNo += maxPeople - 
                                        pAirlineList->pAirline->limitPeopleNo;
                        pAirlineList->pAirline->limitPeopleNo = maxPeople;
                        printf("�޸����˿������ɹ�\n");
                    } else {
                        printf("�޸����˿�����ʧ��, ���˿���С�ڵ�ǰ�Ѿ������Ʊ��.\n");
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
                printf("�����ڸú���\n");
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
        printf("1---- ���պ���Ų�ѯ.\n");
        printf("2---- �������վ�յ�վ��ѯ.\n");
        printf("3---- ��ѯ���һ�캽��.\n");
        printf("0---- �˳���ǰ����.\n");
        printf("----------------------------\n");
        
        scanf("%d", &option);
        fflush(stdin);
        
        switch (option) {
        case 1 : {
            printf("�����뺽���:\n");
            scanf("%s", flightNo);
            fflush(stdin);
            status = QueryByAirlineListFlightNo(flightNo, &pAirlineList);
            if (status) {
                ShowAirlineList(pAirlineList);
            } else {
                printf("û���ҵ��ú���\n");
            }
            break;
        }
        case 2 : {
            printf("���������վ:\n");
            scanf("%s", startStationName);
            fflush(stdin);
            printf("�������յ�վ:\n");
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
                printf("û���ҵ��ú���\n");
            }
            FreeALLAirlineListByStation(pAirlineList);
            break;
        }
        case 3 : {
            printf("���������վ:\n");
            scanf("%s", startStationName);
            fflush(stdin);
            printf("�������յ�վ:\n");
            scanf("%s", endStationName);
            fflush(stdin);

            status = QueryByAirlineListStartAndEnd(
                        startStationName,
                        endStationName,
                        &pAirlineList
                    );
            if (status) {
                printf("�ҵ����º���:\n");
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
                printf("û���ҵ��ú���\n");
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
        printf("1---- ��Ʊ.\n");
        printf("0---- �˳���ǰ����.\n");
        printf("----------------------------\n");
        scanf("%d", &option);
        fflush(stdin);
        
        if (option == 1) {
                printf("��������Ҫ��Ʊ�ĺ����:\n");
                scanf("%s", flightNo);
                fflush(stdin);
                if (!QueryByAirlineListFlightNo(flightNo,&pAirlineList)) {
                    printf("û�в鵽�ú���\n");
                    continue;
                }
                ShowAirlineList(pAirlineList);
                printf("\n**�˿���Ϣ**:\n");
                ShowAllOrderList(pAirlineList);
                printf("\n**�Ŷӵ�Ʊ�˿���Ϣ**:\n");
                ShowAllWaitOrderList(pAirlineList);
                printf("\n\n");
                do {
                    goon_orderflag = 'N';
                    pOrderList = Alloc_OrderList();

                    if (pOrderList != NULL) {
                        printf("��������Ҫ��Ʊ���û���:\n");
                        scanf("%s", &pOrderList->customer.customerName[0]);
                        fflush(stdin);
                        printf("��������Ҫ��Ʊ���û����֤(����):\n");
                        scanf("%s", &pOrderList->customer.identyNo[0]);
                        fflush(stdin);
                        printf("��������Ҫ��Ʊ������:\n");
                        scanf("%d", &pOrderList->customer.needSeatNo);
                        fflush(stdin);
                        if (pAirlineList->pAirline->lastTicketNo >= pOrderList->customer.needSeatNo) {
                            if (AddOrderList(pOrderList, pAirlineList)) {
                                printf("��Ʊ�ɹ�\n");
                            }
                        } else {
                            isAddWait = 'N';
                            printf("Ʊ������\n");
                            printf("�Ƿ���Ҫ����ȴ���Ʊ����(Y:��, N:��):\n");
                            scanf("%c", &isAddWait);
                            fflush(stdin);
                            if (isAddWait == 'Y')  {
                                pWaitOrderList = (WaitOrderList *)pOrderList;
                                AddWaitOrderList(pWaitOrderList, pAirlineList);
                                printf("������Ʊ����ɹ�\n");
                            } else {
                                 Free_OrderList(pOrderList);
                            }
                        }
                    }
                    printf("�Ƿ������Ʊ(Y:��, N:��)\n");
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
        printf("1---- ��Ʊ.\n");
        printf("0---- �˳���ǰ����.\n");
        printf("----------------------------\n");
        scanf("%d", &option);
        fflush(stdin);
        if (option == 1) {
            do {
                printf("��������Ҫ��Ʊ�ĺ����:\n");
                scanf("%s", flightNo);
                fflush(stdin);
                if (!QueryByAirlineListFlightNo(flightNo,&pAirlineList)) {
                    printf("û�в鵽�ú���\n");
                    continue;
                }
                CheckWaitor(pAirlineList);
                ShowAirlineList(pAirlineList);
                printf("\n**�˿���Ϣ**:\n");
                ShowAllOrderList(pAirlineList);
                printf("\n**�Ŷӵ�Ʊ�˿���Ϣ**:\n");
                ShowAllWaitOrderList(pAirlineList);
                printf("\n\n");
                reRefoud:           // this lable is for goto here and reRefoud.
                printf("\n��������Ҫ��Ʊ�ĳ˿͵����֤��:\n");
                scanf("%s", &identyNo);
                fflush(stdin);
                if (QueryOrderListByIdentyNo(identyNo, pAirlineList, &pOrderList)) {
                    printf("\n\n�鵽����Ʊ�ĳ˿���Ϣ:\n");
                    ShowOrderList(pOrderList);
                    if (RemovByPorderList(pOrderList, pAirlineList, MM_FREE_FLAG)) {
                        printf("��Ʊ�ɹ�");
                    } else {
                        printf("��Ʊʧ��\n");
                    }
                CheckWaitor(pAirlineList);
                } else {
                    printf("û�в鵽�ó˿���Ϣ.\n");
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
            printf("\n�Ŷ��û�����Ʊ�ɹ�,��ϸ��Ϣ:\n");
            ShowOrderList(pOrderList);
            if (!RemovByPwaitOrderList(pWaitOrderList, pAirlineList, MM_NOFREE_FLAG)) {
                printlog("Sytem error: RemovByPwaitOrderList is NOT Find the pWaitOrderList\n");
            }
            pOrderList->pNextOrderList = NULL;
        }
    }
}