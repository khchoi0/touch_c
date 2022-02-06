/*
 * CSCI3180 Principles of Programming Languages
 *
 * --- Declaration ---
 *
 * I declare that the assignment here submitted is original except
 * for source material explicitly acknowledged. I also acknowledge
 * that I am aware of University policy and regulations on honesty
 * in academic work, and of the disciplinary guidelines and procedures
 * applicable to breaches of such policy and regulations, as contained
 * in the website: https//www.cuhk.edu.hk/policy/academichonesty/
 *
 * Assignment 1
 * Name        : CHOI, Ka Hou
 * Student ID  : 1155135747
 * Email Addr  : 1155135747@link.cuhk.edu.hk
 */

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

    // one = data from ATM711, three = data from ATM713
    char oneLineBuffer[50], threeLineBuffer[50];
    char oneDataBuffer[20], threeDataBuffer[20];
    long long oneAcct, threeAcct;
    short oneTime = 99, threeTime = 99; // timestamp

    fgets(oneLineBuffer, 31, sortedT71_One); // 16 (account number) + 1 (operation) + 7 (transacAmount) + 5 (timestamp) + 1 ('\n') + 1 ('\0') = 31

    sprintf(oneDataBuffer, "%.16s", oneLineBuffer); // fetch only first 16 characters (account number)
    oneAcct = atoll(oneDataBuffer);                 // string to long long int

    sprintf(oneDataBuffer, "%.5s", &oneLineBuffer[24]); // fetch 5 characters start form character 24 (timestamp)

    // LLONG_MAX account number means no writing from the transSorted711 file
    oneAcct = oneAcct ? oneAcct : LLONG_MAX; // check if empty transSorted711 file
    oneTime = atoi(oneDataBuffer);           // string to int

    fgets(threeLineBuffer, 31, sortedT71_Three); // 16 (account number) + 1 (operation) + 7 (transacAmount) + 5 (timestamp) + 1 ('\n') + 1 ('\0') = 31

    sprintf(threeDataBuffer, "%.16s", threeLineBuffer); // fetch only first 16 characters (account number)
    threeAcct = atoll(threeDataBuffer);                 // string to long long int

    sprintf(threeDataBuffer, "%.5s", &threeLineBuffer[24]); // fetch 5 characters start form character 24 (timestamp)

    // LLONG_MAX account number means no writing from the transSorted713 file
    threeAcct = threeAcct ? threeAcct : LLONG_MAX; // check if empty transSorted711 file
    threeTime = atoi(threeDataBuffer);             // string to int

    printf("\n");

    while (oneAcct != LLONG_MAX || threeAcct != LLONG_MAX) // loop if at least one transSorted file still have record
    {
        if (oneAcct < threeAcct) // write form file with smaller account number
        {
            printf("[NUM] ONE < THREE: %lld at %d \n", oneAcct, oneTime);
            fprintf(sortedTransac, oneLineBuffer);

            fgets(oneLineBuffer, 31, sortedT71_One); // fetch new line of record after write

            sprintf(oneDataBuffer, "%.16s", oneLineBuffer); // fetch only first 16 characters (account number)
            oneAcct = atoll(oneDataBuffer);                 // string to long long int

            sprintf(oneDataBuffer, "%.5s", &oneLineBuffer[24]);               // fetch 5 characters start form character 24 (timestamp)
            oneAcct = (oneTime == atoi(oneDataBuffer)) ? LLONG_MAX : oneAcct; // record identical to last timestamp (EOF reached)
            oneTime = atoi(oneDataBuffer);                                    // string to int
        }
        else if (oneAcct > threeAcct) // write form file with smaller account number
        {
            printf("[NUM] ONE > THREE: %lld at %d \n", threeAcct, threeTime);
            fprintf(sortedTransac, threeLineBuffer);

            fgets(threeLineBuffer, 31, sortedT71_Three); // fetch new line of record after write

            sprintf(threeDataBuffer, "%.16s", threeLineBuffer); // fetch only first 16 characters (account number)
            threeAcct = atoll(threeDataBuffer);                 // string to long long int

            sprintf(threeDataBuffer, "%.5s", &threeLineBuffer[24]);                   // fetch 5 characters start form character 24 (timestamp)
            threeAcct = (threeTime == atoi(threeDataBuffer)) ? LLONG_MAX : threeAcct; // record identical to last timestamp (EOF reached)
            threeTime = atoi(threeDataBuffer);                                        // string to int
        }
        else // account number identical from two files
        {
            if (oneTime < threeTime) // write form file with earlier timestamp
            {
                printf("[ TS] ONE < THREE: %lld at %d \n", oneAcct, oneTime);
                fprintf(sortedTransac, oneLineBuffer);

                fgets(oneLineBuffer, 31, sortedT71_One); // fetch new line of record after write

                sprintf(oneDataBuffer, "%.16s", oneLineBuffer); // fetch only first 16 characters (account number)
                oneAcct = atoll(oneDataBuffer);                 // string to long long int

                sprintf(oneDataBuffer, "%.5s", &oneLineBuffer[24]);               // fetch 5 characters start form character 24 (timestamp)
                oneAcct = (oneTime == atoi(oneDataBuffer)) ? LLONG_MAX : oneAcct; // record identical to last timestamp (EOF reached)
                oneTime = atoi(oneDataBuffer);                                    // string to int
            }
            else // write form file with earlier timestamp
            {
                printf("[ TS] ONE > THREE: %lld at %d \n", threeAcct, threeTime);
                fprintf(sortedTransac, threeLineBuffer);

                fgets(threeLineBuffer, 31, sortedT71_Three); // fetch new line of record after write

                sprintf(threeDataBuffer, "%.16s", threeLineBuffer); // fetch only first 16 characters (account number)
                threeAcct = atoll(threeDataBuffer);                 // string to long long int

                sprintf(threeDataBuffer, "%.5s", &threeLineBuffer[24]);                   // fetch 5 characters start form character 24 (timestamp)
                threeAcct = (threeTime == atoi(threeDataBuffer)) ? LLONG_MAX : threeAcct; // record identical to last timestamp (EOF reached)
                threeTime = atoi(threeDataBuffer);                                        // string to int
            }
        }
    }

    fclose(sortedT71_One);
    fclose(sortedT71_Three);
    fclose(sortedTransac);

    printf("==>        MERGED: [TWO] TRANSAC FILES\n\n");
    return EXIT_SUCCESS;
}

int update()
{
    FILE *masterFile = fopen("master.txt", "r");
    FILE *updateFile = fopen("updatedMaster.txt", "w");

    char masterBuffer[100], acctNumber[17], balanceSign[2], strBalance[16];
    long double balance;

    while (fgets(masterBuffer, 60, masterFile)) // loop if still have content in masterFile (fetch one account from master file for searching corresponding transac record)
    {
        printf("==================================================\n> %s\n", masterBuffer);

        sprintf(acctNumber, "%.16s", &masterBuffer[20]);                   // fetch 16 characters start form character 20 (account number)
        sprintf(balanceSign, "%.1s", &masterBuffer[42]);                   // fetch 1 characters start form character 42 (sign of balance)
        sprintf(strBalance, "%.15s", &masterBuffer[43]);                   // fetch 15 characters start form character 43 (string of unsigned balance)
        balance = atoll(strBalance) / 100.0;                               // string to long long int
        balance = (strcmp(balanceSign, "+") == 0) ? balance : 0 - balance; // long long int to long double

        printf("=>        ACCT: %s \n", acctNumber);
        printf("=>     BALANCE: %+016.02Lf \n\n", balance);

        FILE *transSortedFile = fopen("transSorted.txt", "r");
        char sortedTransacBuffer[50], transacAcctNumber[17], operation[2], amountStr[8];
        double transacAmount;

        while (fgets(sortedTransacBuffer, 31, transSortedFile)) // loop if still have content in transSortedFile
        {
            sprintf(transacAcctNumber, "%.16s", sortedTransacBuffer);
            if (strcmp(transacAcctNumber, acctNumber) >= 0) // found corresponding transac record OR impossible to find in upcoming loop
            {
                break; // stop looping
            }
            else
            {
                printf("=>   SEARCHING: %s", sortedTransacBuffer); // continue looping
            }
        }

        do
        {
            sprintf(transacAcctNumber, "%.16s", sortedTransacBuffer); // fetch first 16 characters (account number)
            sprintf(operation, "%.1s", &sortedTransacBuffer[16]);     // fetch 1 characters start form character 16 (operation)
            sprintf(amountStr, "%.7s", &sortedTransacBuffer[17]);     // fetch 7 characters start form character 17 (string of transaction amount)
            transacAmount = atoi(amountStr) / 100.0;                  // string to int

            if (strcmp(transacAcctNumber, acctNumber) > 0) // impossible to find corresponding transac record in upcoming loop
            {
                printf("==>        END: %s\n", sortedTransacBuffer);
                break; // stop looping
            }
            else
            {
                printf("=> UPDATING ON: %s", sortedTransacBuffer); // found corresponding transac record
            }

            printf("\n                %+016.02Lf \n", balance);
            printf("                %s%015.02lf \n", operation, transacAmount);
            printf("     -----------------------------------          \n");

            balance = (strcmp(operation, "D") == 0) ? balance + transacAmount : balance - transacAmount; // perform update according to operation
            printf("=>     UPDATED: %+016.02Lf \n\n", balance);

        } while (fgets(sortedTransacBuffer, 31, transSortedFile)); // loop if still have content in transSortedFile

        fclose(transSortedFile); // close transSortedFile for reopening it (loop it from start again)

        fprintf(updateFile, "%.42s%+016.0Lf\n", masterBuffer, balance * 100.0); // write to updatedMaster if all transac record of corresponding account is processed
        printf("[ %.42s%+016.0Lf ]\n\n", masterBuffer, balance * 100.0);
    }

    fclose(masterFile);
    fclose(updateFile);
    return EXIT_SUCCESS;
}

int generateNegReport()
{
    printf("==================================================\n");

    FILE *updatedFile = fopen("updatedMaster.txt", "r");
    FILE *reportFile = fopen("negReport.txt", "w");

    char updatedBuffer[100], acctName[21], acctNumber[17], balanceSign[2], balanceStr[16];

    while (fgets(updatedBuffer, 60, updatedFile)) // loop if still have content in updatedMasterFile
    {
        sprintf(acctName, "%.20s", updatedBuffer);        // fetch first 10 characters (account holder name)
        sprintf(acctNumber, "%.16s", &updatedBuffer[20]); // fetch 16 characters start form character 20 (account number)
        sprintf(balanceSign, "%.1s", &updatedBuffer[42]); // fetch 1 characters start form character 42 (sign of balance)
        sprintf(balanceStr, "%.15s", &updatedBuffer[43]); // fetch 15 characters start form character 43 (string of unsigned balance)
        if (strcmp(balanceSign, "+") == 0)                // strcmp return 0 if identical strings
        {
            printf("=>    SKIPPED: %s", updatedBuffer);
            continue; // skip if positive balance
        }
        printf("==>  REPORTED: %s", updatedBuffer);
        fprintf(reportFile, "Name: %s Account Number: %s Balance: -%s\n", acctName, acctNumber, balanceStr); // write if negative balance
    }

    fclose(updatedFile);
    fclose(reportFile);
    return EXIT_SUCCESS;
}

int main()
{
    printf("\n==================================================");

    // sort two trans files
    sort_transaction("trans711.txt", "transSorted711.txt");
    sort_transaction("trans713.txt", "transSorted713.txt");

    // merge two trans sorted files
    merge();

    // generate updated master file
    update();

    // generate negative report
    generateNegReport();

    printf("==================================================\n");
    return EXIT_SUCCESS;
}