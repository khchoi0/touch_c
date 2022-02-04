#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "sort.h"

int merge()
{
    FILE *sortedT71_One = fopen("transSorted711.txt", "r");
    FILE *sortedT71_Three = fopen("transSorted713.txt", "r");
    FILE *sortedTransac = fopen("transSorted.txt", "w");

    char oneLineBuffer[50], threeLineBuffer[50];
    char oneDataBuffer[20], threeDataStr[20];
    long long oneAcct, threeAcct;
    short oneTime = 99, threeTime = 99;

    fgets(oneLineBuffer, 31, sortedT71_One);

    sprintf(oneDataBuffer, "%.16s", oneLineBuffer);
    oneAcct = atoll(oneDataBuffer);

    sprintf(oneDataBuffer, "%.5s", &oneLineBuffer[24]);
    oneAcct = (oneTime == atoi(oneDataBuffer) || oneAcct == 0) ? LONG_LONG_MAX : oneAcct;
    oneTime = atoi(oneDataBuffer);

    fgets(threeLineBuffer, 31, sortedT71_Three);

    sprintf(threeDataStr, "%.16s", threeLineBuffer);
    threeAcct = atoll(threeDataStr);

    sprintf(threeDataStr, "%.5s", &threeLineBuffer[24]);
    threeAcct = (threeTime == atoi(threeDataStr) || threeAcct == 0) ? LONG_LONG_MAX : threeAcct;
    threeTime = atoi(threeDataStr);

    printf("\n");

    while (oneAcct != LONG_LONG_MAX || threeAcct != LONG_LONG_MAX)
    {
        if (oneAcct < threeAcct)
        {
            printf("[NUM] ONE < THREE: %lld at %d \n", oneAcct, oneTime);
            fprintf(sortedTransac, oneLineBuffer);

            fgets(oneLineBuffer, 31, sortedT71_One);

            sprintf(oneDataBuffer, "%.16s", oneLineBuffer);
            oneAcct = atoll(oneDataBuffer);

            sprintf(oneDataBuffer, "%.5s", &oneLineBuffer[24]);
            oneAcct = (oneTime == atoi(oneDataBuffer)) ? LONG_LONG_MAX : oneAcct;
            oneTime = atoi(oneDataBuffer);
        }
        else if (oneAcct > threeAcct)
        {
            printf("[NUM] ONE > THREE: %lld at %d \n", threeAcct, threeTime);
            fprintf(sortedTransac, threeLineBuffer);

            fgets(threeLineBuffer, 31, sortedT71_Three);

            sprintf(threeDataStr, "%.16s", threeLineBuffer);
            threeAcct = atoll(threeDataStr);

            sprintf(threeDataStr, "%.5s", &threeLineBuffer[24]);
            threeAcct = (threeTime == atoi(threeDataStr)) ? LONG_LONG_MAX : threeAcct;
            threeTime = atoi(threeDataStr);
        }
        else
        {
            if (oneTime < threeTime)
            {
                printf("[ TS] ONE < THREE: %lld at %d \n", oneAcct, oneTime);
                fprintf(sortedTransac, oneLineBuffer);

                fgets(oneLineBuffer, 31, sortedT71_One);

                sprintf(oneDataBuffer, "%.16s", oneLineBuffer);
                oneAcct = atoll(oneDataBuffer);

                sprintf(oneDataBuffer, "%.5s", &oneLineBuffer[24]);
                oneAcct = (oneTime == atoi(oneDataBuffer)) ? LONG_LONG_MAX : oneAcct;
                oneTime = atoi(oneDataBuffer);
            }
            else
            {
                printf("[ TS] ONE > THREE: %lld at %d \n", threeAcct, threeTime);
                fprintf(sortedTransac, threeLineBuffer);

                fgets(threeLineBuffer, 31, sortedT71_Three);

                sprintf(threeDataStr, "%.16s", threeLineBuffer);
                threeAcct = atoll(threeDataStr);

                sprintf(threeDataStr, "%.5s", &threeLineBuffer[24]);
                threeAcct = (threeTime == atoi(threeDataStr)) ? LONG_LONG_MAX : threeAcct;
                threeTime = atoi(threeDataStr);
            }
        }
    }

    fclose(sortedT71_One);
    fclose(sortedT71_Three);
    fclose(sortedTransac);

    printf("==>        MERGED: [TWO] TRANSAC FILES\n\n");
    return 1;
}

int update()
{
    FILE *masterFile = fopen("master.txt", "r");
    FILE *updateFile = fopen("updatedMaster.txt", "w");

    char masterBuffer[100], acctNumber[17], balanceSign[2], strBalance[16];
    long double balance;

    while (fgets(masterBuffer, 60, masterFile))
    {
        printf("==================================================\n> %s\n", masterBuffer);

        sprintf(acctNumber, "%.16s", &masterBuffer[20]);
        sprintf(balanceSign, "%.1s", &masterBuffer[42]);
        sprintf(strBalance, "%.15s", &masterBuffer[43]);
        balance = atoll(strBalance) / 100.0;
        balance = (strcmp(balanceSign, "+") == 0) ? balance : 0 - balance;

        printf("=>        ACCT: %s \n", acctNumber);
        printf("=>     BALANCE: %+016.02Lf \n\n", balance);

        FILE *transSFile = fopen("transSorted.txt", "r");
        char sortedTransacBuffer[50], transacAcctNumber[17], operation[2], amountStr[8];
        double amount;

        while (fgets(sortedTransacBuffer, 31, transSFile))
        {
            sprintf(transacAcctNumber, "%.16s", sortedTransacBuffer);
            if (strcmp(transacAcctNumber, acctNumber) >= 0)
            {
                break;
            }
            else
            {
                printf("=>   SEARCHING: %s", sortedTransacBuffer);
            }
        }

        do
        {
            sprintf(transacAcctNumber, "%.16s", sortedTransacBuffer);
            sprintf(operation, "%.1s", &sortedTransacBuffer[16]);
            sprintf(amountStr, "%.7s", &sortedTransacBuffer[17]);
            amount = atoi(amountStr) / 100.0;

            if (strcmp(transacAcctNumber, acctNumber) > 0)
            {
                printf("==>        END: %s\n", sortedTransacBuffer);
                break;
            }
            else
            {
                printf("=> UPDATING ON: %s", sortedTransacBuffer);
            }

            printf("\n                %+016.02Lf \n", balance);
            printf("                %s%015.02lf \n", operation, amount);
            printf("--------------------------------------------------\n");

            balance = (strcmp(operation, "D") == 0) ? balance + amount : balance - amount;
            balanceSign[0] = (balance < 0) ? '-' : '+';
            printf("=>     UPDATED: %+016.02Lf \n\n", balance);

        } while (fgets(sortedTransacBuffer, 31, transSFile));

        fclose(transSFile);

        fprintf(updateFile, "%.42s%+016.0Lf\n", masterBuffer, balance * 100.0);
        printf("[ %.42s%+016.0Lf ]\n\n", masterBuffer, balance * 100.0);
    }

    fclose(masterFile);
    fclose(updateFile);
    return 1;
}

int generateNegReport()
{
    printf("==================================================\n");

    FILE *updatedFile = fopen("updatedMaster.txt", "r");
    FILE *reportFile = fopen("negReport.txt", "w");

    char updatedBuffer[100], acctName[21], acctNumber[17], balanceSign[2], balanceStr[16];

    while (fgets(updatedBuffer, 60, updatedFile))
    {
        sprintf(balanceSign, "%.1s", &updatedBuffer[42]);
        sprintf(acctName, "%.20s", updatedBuffer);
        sprintf(acctNumber, "%.16s", &updatedBuffer[20]);
        sprintf(balanceStr, "%.15s", &updatedBuffer[43]);
        if (strcmp(balanceSign, "+") == 0)
        {
            printf("=>    SKIPPED: %s", updatedBuffer);
            continue;
        }
        printf("==>  REPORTED: %s", updatedBuffer);
        fprintf(reportFile, "Name: %s Account Number: %s Balance: -%s\n", acctName, acctNumber, balanceStr);
    }

    fclose(updatedFile);
    fclose(reportFile);
    return 1;
}

int main()
{
    // sort
    sort_transaction("trans711.txt", "transSorted711.txt");
    sort_transaction("trans713.txt", "transSorted713.txt");

    // merge
    merge();

    // update
    update();

    // generate negative report
    generateNegReport();

    printf("==================================================\n");
    return 0;
}