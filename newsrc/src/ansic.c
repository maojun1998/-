#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "ars.h"

extern AirlineListHead globalAirLineListHead;

void LoadFileRecordToMem(void)
{
    FILE *fp;
    int maxAirline;
    int maxOrderList;
    int maxWaitOrderList;
    AirlineList *pAirlineList;
    WaitOrderList *pWaitOrderList;
    OrderList *pOrderList;
    time_t times;
    struct tm *localTimes;
    int years;
    int month;
    int days;
    int hours;
    int minutes;
    int i;
    int j;
    fp = fopen("arsinfo.txt", "r");
    
    time(&times);
    localTimes = localtime (&times);
    
    if (fp == NULL) {
        fprintf(stderr, "\n* Waring: arsinfo.txt Load Fail.Because of :%s\n\n", strerror(errno));
        printlog("Waring: arsinfo.txt Load Fail.\n\n");
        return ;
    }
    fscanf(fp, "������:%d\n", &maxAirline);

    for (i = 0; i < maxAirline; i++) {
        if((pAirlineList = Alloc_AirlineList())) {
            fscanf(
                fp,
                "\n* �����: %s �ɻ���: %s  ���վ: %s  �յ�վ: %s\n",
                &pAirlineList->pAirline->flightNo[0], 
                &pAirlineList->pAirline->airplaneNo[0],
                &pAirlineList->pAirline->startStationName[0], 
                &pAirlineList->pAirline->endStationName[0]
            );
            fscanf(
                fp,
                "* ���ʱ��: %d��-%d��-%d��-%d:%d\n",
                &years,
                &month,
                &days,
                &hours,
                &minutes
            );
            pAirlineList->pAirline->departureTime.tm_year = years - 1900;
            pAirlineList->pAirline->departureTime.tm_mon = month - 1;
            pAirlineList->pAirline->departureTime.tm_mday = days;
            pAirlineList->pAirline->departureTime.tm_hour = hours;
            pAirlineList->pAirline->departureTime.tm_min = minutes;
            pAirlineList->pAirline->departureTime.tm_sec = 0;
            pAirlineList->pAirline->departureTime.tm_isdst = localTimes->tm_isdst;
            fscanf(
                fp,
                "* ����ʱ��: %d��-%d��-%d��-%d:%d\n",
                &years,
                &month,
                &days,
                &hours,
                &minutes
            );
            pAirlineList->pAirline->arrivalTime.tm_year = years - 1900;
            pAirlineList->pAirline->arrivalTime.tm_mon = month - 1;
            pAirlineList->pAirline->arrivalTime.tm_mday = days;
            pAirlineList->pAirline->arrivalTime.tm_hour = hours;
            pAirlineList->pAirline->arrivalTime.tm_min = minutes;
            pAirlineList->pAirline->arrivalTime.tm_sec = 0;
            pAirlineList->pAirline->arrivalTime.tm_isdst = localTimes->tm_isdst;
            fscanf(
                fp,
                "* ���˿���: %d  ʣ��Ʊ��: %d. \n\n",
                &pAirlineList->pAirline->limitPeopleNo,
                &pAirlineList->pAirline->lastTicketNo
            );
            pAirlineList->pAirline->lastTicketNo = pAirlineList->pAirline->limitPeopleNo;
            AddArilineListToGlobalList(pAirlineList);
            fscanf(fp, "�˿���:%d\n", &maxOrderList);
            for (j = 0; j < maxOrderList; j++) {
                pOrderList = Alloc_OrderList();
                if (pOrderList) {
                    fscanf(
                        fp,
                        "\n* �˿�����:%s  �˿Ͷ�Ʊ����:%d\n", 
                        &pOrderList->customer.customerName[0],
                        &pOrderList->customer.needSeatNo
                    );
                    fscanf(
                        fp,
                        "* �˿����֤��:%s\n\n", 
                        &pOrderList->customer.identyNo[0]
                    );
                    if(AddOrderList(pOrderList, pAirlineList)) {
                        printlog("���OrderList�ɹ�\n");
                    } else {
                        printlog("���OrderListʧ��\n");
                    }
                }
            }
            
            fscanf(fp, "��Ʊ�˿���:%d\n", &maxWaitOrderList);
            for (j = 0; j < maxWaitOrderList; j++) {
                pWaitOrderList = Alloc_WaitOrderList();
                if (pWaitOrderList) {
                    fscanf(
                        fp,
                        "\n* �˿�����:%s  �˿���Ҫ��Ʊ����:%d\n", 
                        &pWaitOrderList->waitCustomer.customerName[0],
                        &pWaitOrderList->waitCustomer.needSeatNo
                    );
                    fscanf(
                        fp,
                        "* �˿����֤��:%s\n\n", 
                        &pWaitOrderList->waitCustomer.identyNo[0]
                    );
                    if(AddWaitOrderList(pWaitOrderList, pAirlineList)) {
                        printlog("���WaitOrderList�ɹ�\n");
                    } else {
                        printlog("���WaitOrderListʧ��\n");
                    }
                }
            }
        }
    }
    fclose(fp);
    printf("\n�����ļ�arsinfo.txt�ɹ�\n\n");
}
void StoryRecordToFile(void)
{
    FILE *fp;
    int maxAirline;
    int maxOrderList;
    int maxWaitOrderList;
    AirlineList *pAirlineList;
    WaitOrderList *pWaitOrderList;
    OrderList *pOrderList;
    int i;
    int j;
    printlog("In StoryRecordToFile()\n");
    fp = fopen("arsinfo.txt", "w");
    
    if (fp == NULL) {
        fprintf(stderr, "\n* Waring: arsinfo.txt Story Fail.Because of :%s\n\n", strerror(errno));
        printlog("Waring: arsinfo.txt Load Fail.\n\n");
        return ;
    }
    
    maxAirline = globalAirLineListHead.AirlineNo;
    pAirlineList = globalAirLineListHead.pAirlineListHead;
    
    fprintf(fp, "������:%d\n", maxAirline);
    for (i = 0; i < maxAirline; i++) {
        maxOrderList = pAirlineList->pAirline->pOrderListHead->orderNo;
        maxWaitOrderList = pAirlineList->pAirline->pWaitOrderListHead->waitNo;
        fprintf(
            fp,
            "\n* �����: %s �ɻ���: %s  ���վ: %s  �յ�վ: %s\n",
            &pAirlineList->pAirline->flightNo[0], 
            &pAirlineList->pAirline->airplaneNo[0],
            &pAirlineList->pAirline->startStationName[0], 
            &pAirlineList->pAirline->endStationName[0]
        );

        fprintf(
            fp,
            "* ���ʱ��: %d��-%d��-%d��-%d:%d\n",
            pAirlineList->pAirline->departureTime.tm_year + 1900,
            pAirlineList->pAirline->departureTime.tm_mon + 1,
            pAirlineList->pAirline->departureTime.tm_mday,
            pAirlineList->pAirline->departureTime.tm_hour,
            pAirlineList->pAirline->departureTime.tm_min
        );
        fprintf(
            fp,
            "* ����ʱ��: %d��-%d��-%d��-%d:%d\n",
            pAirlineList->pAirline->arrivalTime.tm_year + 1900,
            pAirlineList->pAirline->arrivalTime.tm_mon + 1,
            pAirlineList->pAirline->arrivalTime.tm_mday,
            pAirlineList->pAirline->arrivalTime.tm_hour,
            pAirlineList->pAirline->arrivalTime.tm_min
        );
        fprintf(
            fp,
            "* ���˿���: %d  ʣ��Ʊ��: %d. \n\n",
            pAirlineList->pAirline->limitPeopleNo,
            pAirlineList->pAirline->lastTicketNo
        );
        pWaitOrderList = pAirlineList->pAirline->pWaitOrderListHead->pWaitOrderList;
        pOrderList = pAirlineList->pAirline->pOrderListHead->pOrderList;
        fprintf(fp, "�˿���:%d\n", maxOrderList);
        
        for (j = 0; j < maxOrderList; j++) {
            fprintf(
                fp,
                "\n* �˿�����:%s  �˿Ͷ�Ʊ����:%d\n", 
                &pOrderList->customer.customerName[0],
                pOrderList->customer.needSeatNo
            );
            fprintf(
                fp,
                "* �˿����֤��:%s\n\n", 
                &pOrderList->customer.identyNo[0]
            );
            pOrderList = pOrderList->pNextOrderList;
        }
        fprintf(fp, "��Ʊ�˿���:%d\n", maxWaitOrderList);
        for (j = 0; j < maxWaitOrderList; j++) {
            fprintf(
                fp,
                "\n* �˿�����:%s  �˿���Ҫ��Ʊ����:%d\n", 
                &pWaitOrderList->waitCustomer.customerName[0],
                pWaitOrderList->waitCustomer.needSeatNo
            );
            fprintf(
                fp,
                "* �˿����֤��:%s\n\n", 
                &pWaitOrderList->waitCustomer.identyNo[0]
            );
            pWaitOrderList = pWaitOrderList->pNextWaitOrderList;
        }
        pAirlineList = pAirlineList->pNextAirlineList;
    }
    fclose(fp);
    printf("\n����ɹ�,�ļ���: arsinfo.txt\n\n");
    printlog("StoryRecordToFile ok\n");
}