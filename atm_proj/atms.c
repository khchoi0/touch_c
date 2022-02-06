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

#define VALIDATING_USER 1
#define VALIDATING_TARGET 0

int checkMasterFileExists()
{
    FILE *masterFile;
    if ((masterFile = fopen("master.txt", "r")))
    {
        fclose(masterFile);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

void atmInitialize()
{
    printf("\n"
           "##############################################\n"
           "##         Gringotts Wizarding Bank         ##\n"
           "##                 Welcome                  ##\n"
           "##############################################\n");
}

int chooseAtm()
{
    char input[100];
    while (1)
    {
        printf("=> PLEASE CHOOSE THE ATM\n"
               "=> PRESS 1 FOR ATM 711\n"
               "=> PRESS 2 FOR ATM 713\n");
        scanf("%s", &input);
        while (getchar() != '\n') // skip stdin content until line break
            ;

        if (strcmp(input, "1") && strcmp(input, "2")) // strcmp() return 0 if identical strings
        {
            printf("=> INVALID INPUT\n");
            memset(input, '\0', 100); // reset content to null
            continue;
        }
        break;
    }

    if (input[0] == '1')
        return 711;
    return 713;
}

int validateAcctInfo(int validatingUser, char *inAcctInfo, long double *userBalancePtr)
{
    FILE *masterFile;
    char nameBuffer[50], acctInfoBuffer[50], userBalanceSign[2], strUserBalance[16];
    char userAcctNumber[17], targetAcctNumber[17], chosenService;

    masterFile = fopen("master.txt", "r");
    while (fgets(nameBuffer, 21, masterFile)) // loop if still have content in masterFile
    {
        if (validatingUser)
        {
            fgets(acctInfoBuffer, 23, masterFile); // 16 (account number) + 6 (password) + 1 ('\0') = 23
            fgets(userBalanceSign, 2, masterFile); // 1 (sign of balance) + 1 ('\0') = 2
            fgets(strUserBalance, 16, masterFile); // 15 (unsigned balance) + 1 ('\0') = 16
        }
        else
        {
            fgets(acctInfoBuffer, 17, masterFile); // 16 (account number) + 1 ('\0') = 17
        }
        while (getc(masterFile) != '\n') // skip content (if any) until line break
            ;

        if (strcmp(acctInfoBuffer, inAcctInfo) == 0) // account found: 22 if validating user, 16 if validating target
        {
            printf("\n");
            printf("==>  HOLDER: %s \n", nameBuffer);
            printf("==>    ACCT: %.16s \n", acctInfoBuffer); // only show 16 characters (account number)
            if (validatingUser)
            {
                *userBalancePtr = atoll(strUserBalance) / 100.0;                                            // balance string to balance number
                *userBalancePtr = (strcmp(userBalanceSign, "+") == 0) ? *userBalancePtr : -*userBalancePtr; // sign character to signed balance
                printf("==> BALANCE: %+016.02Lf \n", *userBalancePtr);
            }
            printf("\n");
            if ((!validatingUser) && strcmp(acctInfoBuffer, userAcctNumber) == 0) // identical user and target account number
            {
                printf("=> YOU CANNOT TRANSFER TO YOURSELF\n");
                return EXIT_FAILURE; // found but not valid
            }

            fclose(masterFile);
            return EXIT_SUCCESS; // found
        }
    }

    // account not found
    if (validatingUser)
        printf("=> INCORRECT ACCOUNT/PASSWORD\n");
    else
        printf("=> TARGET ACCOUNT DOES NOT EXIST\n");

    fclose(masterFile);
    return EXIT_FAILURE; // not found
}

char *inputAcctInfo(int validatingUser, long double *userBalancePtr)
{
    char *inAcctInfo = (char *)malloc(sizeof(char) * 100);
    char *inUserAcctNumber = (char *)malloc(sizeof(char) * 100);
    char inUserAcctPassword[100];

    do
    {
        if (validatingUser)
        {
            printf("=> ACCOUNT\n");
            scanf("%s", inUserAcctNumber);
            while (getchar() != '\n') // skip stdin content until line break
                ;

            printf("=> PASSWORD\n");
            scanf("%s", inUserAcctPassword);
            while (getchar() != '\n') // skip stdin content until line break
                ;

            strcpy(inAcctInfo, inUserAcctNumber);
            strcat(inAcctInfo, inUserAcctPassword); // inAcctInfo contains both account number & password
        }
        else
        {
            printf("=> TARGET ACCOUNT\n");
            scanf("%s", inAcctInfo);  // inAcctInfo contains account number only
            while (getchar() != '\n') // skip stdin content until line break
                ;
        }
    } while (validateAcctInfo(validatingUser, inAcctInfo, userBalancePtr) == EXIT_FAILURE); // loop until input account found

    if (validatingUser)
    {
        return inUserAcctNumber;
    }
    else
    {
        return inAcctInfo;
    }
}

char chooseService()
{
    char input[100];
    while (1)
    {
        printf("=> PLEASE CHOOSE YOUR SERVICE\n"
               "=> PRESS D FOR DEPOSIT\n"
               "=> PRESS W FOR WITHDRAWAL\n"
               "=> PRESS T FOR TRANSFER\n");
        scanf("%s", input);
        while (getchar() != '\n') // skip stdin content until line break
            ;

        if (strcmp(input, "D") && strcmp(input, "W") && strcmp(input, "T")) // strcmp return 0 if identical strings
        {
            printf("=> INVALID INPUT\n");
            memset(input, '\0', 100); // reset content to null
            continue;
        }
        break;
    }

    return input[0];
}

double inputAmount(char chosenService, long double *userBalancePtr)
{
    double transacAmount = -1;
    while (1)
    {
        printf("=> AMOUNT\n");
        scanf("%lf", &transacAmount);
        while (getchar() != '\n') // skip stdin content until line break
            ;

        printf("\n==> OPERATION: %+016.02Lf %c %08.02lf\n\n", *userBalancePtr, chosenService, transacAmount);
        if (transacAmount < 0)
        {
            printf("=> INVALID INPUT\n");
            continue;
        }
        else if (chosenService != 'D' && transacAmount > *userBalancePtr)
        {
            printf("=> INSUFFICIENT BALANCE\n");
            continue;
        }
        break;
    }

    return transacAmount;
}

char chooseIfContinue()
{
    char input[100];
    while (1)
    {
        printf("=> CONTINUE?\n"
               "=> N FOR NO\n"
               "=> Y FOR YES\n");
        scanf("%s", input);
        while (getchar() != '\n') // skip stdin content until line break
            ;

        if (strcmp(input, "Y") && strcmp(input, "N")) // strcmp return 0 if identical strings
        {
            printf("=> INVALID INPUT\n");
            memset(input, '\0', 100); // reset content to null
            continue;
        }
        break;
    }

    return input[0];
}

int main()
{
    FILE *t71_One_File = fopen("trans711.txt", "w");
    FILE *t71_Three_File = fopen("trans713.txt", "w");

    int timeStamp = 0;
    int chosenAtm;
    char *userAcctNumber = (char *)malloc(sizeof(char) * 16 + 1);
    long double *userBalancePtr = (long double *)malloc(sizeof(long double) + 1);
    char chosenService;
    char *targetAcctNumber = (char *)malloc(sizeof(char) * 16 + 1);
    double transacAmount = -1;
    char chosenIfContinue = 'Y';

    do
    {
        if (checkMasterFileExists() == EXIT_FAILURE)
        {
            printf("[ABORT]: MASTER FILE NOT FOUND\n\n");
            return EXIT_FAILURE;
        }

        atmInitialize();

        chosenAtm = chooseAtm();

        userAcctNumber = inputAcctInfo(VALIDATING_USER, userBalancePtr);
        if (*userBalancePtr < 0)
        {
            printf("=> NEGATIVE REMAINS TRANSACTION ABORT\n");
            continue;
        }

        chosenService = chooseService();
        if (chosenService == 'T')
        {
            targetAcctNumber = inputAcctInfo(VALIDATING_TARGET, NULL);
        }

        transacAmount = inputAmount(chosenService, userBalancePtr);

        printf("==> WRITTEN RECORD: \n");

        FILE *chosenAtmFile = (chosenAtm == 711) ? t71_One_File : t71_Three_File;

        if (chosenService == 'T')
        {
            chosenService = 'W'; // withdraw first
            fprintf(chosenAtmFile, "%s%c%07.0lf%05d\n", userAcctNumber, chosenService, transacAmount * 100.0, timeStamp++);
            printf("==> [ATM-%d]: %s %c %07.0lf %05d\n", chosenAtm, userAcctNumber, chosenService, transacAmount * 100.0, timeStamp);

            chosenService = 'D'; // deposit second
            fprintf(chosenAtmFile, "%s%c%07.0lf%05d\n", targetAcctNumber, chosenService, transacAmount * 100.0, timeStamp++);
            printf("==> [ATM-%d]: %s %c %07.0lf %05d\n", chosenAtm, targetAcctNumber, chosenService, transacAmount * 100.0, timeStamp);
        }
        else
        {
            fprintf(chosenAtmFile, "%s%c%07.0lf%05d\n", userAcctNumber, chosenService, transacAmount * 100.0, timeStamp++);
            printf("==> [ATM-%d]: %s %c %07.0lf %05d\n", chosenAtm, userAcctNumber, chosenService, transacAmount * 100.0, timeStamp);
        }
        printf("\n");

    } while (*userBalancePtr < 0 || chooseIfContinue() == 'Y'); // short-circuiting

    fclose(t71_One_File);
    fclose(t71_Three_File);
    return EXIT_SUCCESS;
}