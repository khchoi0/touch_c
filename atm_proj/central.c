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

    char oneBuffer[50], threeBuffer[50];
    char oneStr[20], threeStr[20];
    long long oneAcct, threeAcct;
    short oneTime = 99, threeTime = 99;

    // printf("%d ", fgets(oneBuffer, 31, sortedT71_One) != 0);
    // printf("%.16s ", oneBuffer);
    // printf("%.5s \n", &oneBuffer[24]);
    fgets(oneBuffer, 31, sortedT71_One);
    sprintf(oneStr, "%.16s", oneBuffer);
    oneAcct = atoll(oneStr);
    memset(oneStr, '\0', 20);
    sprintf(oneStr, "%.5s", &oneBuffer[24]);
    oneAcct = (oneTime == atoi(oneStr)) ? LONG_LONG_MAX : oneAcct;
    oneTime = atoi(oneStr);
    memset(oneStr, '\0', 20);

    // printf("%d ", fgets(threeBuffer, 31, sortedT71_Three) != 0);
    // printf("%.16s ", threeBuffer);
    // printf("%.5s \n", &threeBuffer[24]);
    fgets(threeBuffer, 31, sortedT71_Three);
    sprintf(threeStr, "%.16s", threeBuffer);
    threeAcct = atoll(threeStr);
    memset(oneStr, '\0', 20);
    sprintf(threeStr, "%.5s", &threeBuffer[24]);
    threeAcct = (threeTime == atoi(threeStr)) ? LONG_LONG_MAX : threeAcct;
    threeTime = atoi(threeStr);
    memset(oneStr, '\0', 20);
    printf("\n");

    while (oneAcct != LONG_LONG_MAX || threeAcct != LONG_LONG_MAX)
    {
        if (oneAcct < threeAcct)
        {
            printf("[NUM] ONE < THREE: %lld at %d \n", oneAcct, oneTime);
            fprintf(sortedTransac, oneBuffer);
            fgets(oneBuffer, 31, sortedT71_One);
            sprintf(oneStr, "%.16s", oneBuffer);
            oneAcct = atoll(oneStr);
            sprintf(oneStr, "%.5s", &oneBuffer[24]);
            oneAcct = (oneTime == atoi(oneStr)) ? LONG_LONG_MAX : oneAcct;
            oneTime = atoi(oneStr);
        }
        else if (oneAcct > threeAcct)
        {
            printf("[NUM] ONE < THREE: %lld at %d \n", threeAcct, threeTime);
            fprintf(sortedTransac, threeBuffer);
            fgets(threeBuffer, 31, sortedT71_Three);
            sprintf(threeStr, "%.16s", threeBuffer);
            threeAcct = atoll(threeStr);
            sprintf(threeStr, "%.5s", &threeBuffer[24]);
            threeAcct = (threeTime == atoi(threeStr)) ? LONG_LONG_MAX : threeAcct;
            threeTime = atoi(threeStr);
        }
        else
        {
            if (oneTime < threeTime)
            {
                printf("[ TS] ONE < THREE: %lld at %d \n", oneAcct, oneTime);
                fprintf(sortedTransac, oneBuffer);
                fgets(oneBuffer, 31, sortedT71_One);
                sprintf(oneStr, "%.16s", oneBuffer);
                oneAcct = atoll(oneStr);
                sprintf(oneStr, "%.5s", &oneBuffer[24]);
                oneAcct = (oneTime == atoi(oneStr)) ? LONG_LONG_MAX : oneAcct;
                oneTime = atoi(oneStr);
            }
            else
            {
                printf("[ TS] ONE > THREE: %lld at %d \n", threeAcct, threeTime);
                fprintf(sortedTransac, threeBuffer);
                fgets(threeBuffer, 31, sortedT71_Three);
                sprintf(threeStr, "%.16s", threeBuffer);
                threeAcct = atoll(threeStr);
                sprintf(threeStr, "%.5s", &threeBuffer[24]);
                threeAcct = (threeTime == atoi(threeStr)) ? LONG_LONG_MAX : threeAcct;
                threeTime = atoi(threeStr);
            }
        }
    }

    fclose(sortedT71_One);
    fclose(sortedT71_Three);
    fclose(sortedTransac);
    return 1;
}

int update()
{
    FILE *masterFile = fopen("master.txt", "r");
    FILE *updateFile = fopen("updatedMaster.txt", "w");

    char masterBuffer[100], acctNumber[17], balanceSign[2], balanceStr[16];
    long double balance;

    while (fgets(masterBuffer, 60, masterFile))
    {
        printf("==================================================\n> %s\n", masterBuffer);

        sprintf(acctNumber, "%.16s", &masterBuffer[20]);
        sprintf(balanceSign, "%.1s", &masterBuffer[42]);
        sprintf(balanceStr, "%.15s", &masterBuffer[43]);
        balance = atoll(balanceStr) / 100.0;
        balance = (strcmp(balanceSign, "+") == 0) ? balance : 0 - balance;

        // printf("=>        ACCT: %s \n", acctNumber);
        // printf("=>        SIGN: %s \n", balanceSign);
        // printf("=> BALANCE-STR: %s \n", balanceStr);
        // printf("=>     BALANCE: %Lf \n\n", balance);

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
            if (strcmp(transacAcctNumber, acctNumber) > 0)
            {
                printf("==>        END: %s\n", sortedTransacBuffer);
                break;
            }
            else
            {
                printf("=> UPDATING ON: %s", sortedTransacBuffer);
            }
            printf("\n                %+016Lf \n", balance);

            sprintf(operation, "%.1s", &sortedTransacBuffer[16]);
            sprintf(amountStr, "%.7s", &sortedTransacBuffer[17]);
            amount = atoi(amountStr) / 100.0;
            printf("                %s%015lf \n", operation, amount);
            printf("--------------------------------------------------\n");

            balance = (strcmp(operation, "D") == 0) ? balance + amount : balance - amount;
            balanceSign[0] = (balance < 0) ? '-' : '+';

            printf("=>     UPDATED: %+016Lf \n\n", balance);

        } while (fgets(sortedTransacBuffer, 31, transSFile));

        fclose(transSFile);

        sprintf(balanceStr, "%.0Lf", balance * 100.00);

        printf("[ %.42s%+016.0Lf ]\n\n", masterBuffer, balance * 100.00);
        fprintf(updateFile, "%.42s%+016.0Lf\n", masterBuffer, balance * 100.00);
    }

    fclose(masterFile);
    fclose(updateFile);
    return 1;
}

int generateNegReport()
{
    FILE *updatedFile = fopen("updatedMaster.txt", "r");
    FILE *reportFile = fopen("negReport.txt", "w");

    char updatedBuffer[100], acctName[21], acctNumber[17], balanceSign[2], balanceStr[16];

    while (fgets(updatedBuffer, 60, updatedFile))
    {
        sprintf(balanceSign, "%.1s", &updatedBuffer[42]);
        if (strcmp(balanceSign, "+") == 0)
        {
            printf("=>    SKIPPED: %s", updatedBuffer);
            continue;
        }
        else
        {
            printf("==>  REPORTED: %s", updatedBuffer);
        }
        sprintf(acctName, "%.20s", updatedBuffer);
        sprintf(acctNumber, "%.16s", &updatedBuffer[20]);
        sprintf(balanceStr, "%.15s", &updatedBuffer[43]);

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
    if (merge())
    {
        printf("           MERGED: [TWO] TRANSAC FILES\n\n");
    };

    // update
    update();

    printf("==================================================\n");

    generateNegReport();

    printf("==================================================\n");
    return 0;
}