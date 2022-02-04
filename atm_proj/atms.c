#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

FILE *masterFile;
char userAcctNumber[17], targetAcctNumber[17], chosenService;
long double userBalance = -1;
double transacAmount;

int VALIDATING_USER = 1;
int VALIDATING_TARGET = 0;

int checkMasterFileExists()
{
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
        while (getchar() != '\n')
            ;
        printf("%s\n", input);

        if (strcmp(input, "1") && strcmp(input, "2"))
        {
            printf("=> INVALID INPUT\n");
            memset(input, '\0', 100);
            continue;
        }
        break;
    }

    if (strcmp(input, "1") == 0)
    {
        return 711;
    }
    else if (strcmp(input, "2") == 0)
    {
        return 713;
    }
    else
    {
        return '\0';
    }
}

int validateAcctInfo(int validatingUser, char *inAcctInfo)
{
    char nameBuffer[50], acctInfoBuffer[50], userBalanceSign[2], strUserBalance[16];

    masterFile = fopen("master.txt", "r");
    while (fgets(nameBuffer, 21, masterFile))
    {
        if (validatingUser)
        {
            fgets(acctInfoBuffer, 23, masterFile);
            fgets(userBalanceSign, 2, masterFile);
            fgets(strUserBalance, 16, masterFile);
        }
        else
        {
            fgets(acctInfoBuffer, 17, masterFile);
        }
        while (getc(masterFile) != '\n')
            ;

        if (strcmp(acctInfoBuffer, inAcctInfo) == 0)
        {
            printf("\n");
            printf("==>  HOLDER: %s \n", nameBuffer);
            printf("==>    ACCT: %.16s \n", acctInfoBuffer);
            if (validatingUser)
            {
                userBalance = atoll(strUserBalance) / 100.0;
                userBalance = (strcmp(userBalanceSign, "+") == 0) ? userBalance : 0 - userBalance;
                printf("==> BALANCE: %+016.02Lf \n", userBalance);
            }
            printf("\n");

            fclose(masterFile);
            if ((!validatingUser) && strcmp(acctInfoBuffer, userAcctNumber) == 0)
            {
                printf("=> YOU CANNOT TRANSFER TO YOURSELF\n");
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        }
    }

    if (validatingUser)
    {
        printf("=> INCORRECT ACCOUNT/PASSWORD\n");
    }
    else
    {
        printf("=> TARGET ACCOUNT DOES NOT EXIST\n");
    }

    fclose(masterFile);
    return EXIT_FAILURE;
}

void inputAcctInfo(int validatingUser)
{
    char inAcctInfo[100], inUserAcctNumber[100], inUserAcctPassword[100];

    do
    {
        if (validatingUser)
        {
            printf("=> ACCOUNT\n");
            scanf("%s", inUserAcctNumber);
            while (getchar() != '\n')
                ;
            printf("%s\n", inUserAcctNumber);

            printf("=> PASSWORD\n");
            scanf("%s", inUserAcctPassword);
            while (getchar() != '\n')
                ;
            printf("%s\n", inUserAcctPassword);

            strcpy(inAcctInfo, inUserAcctNumber);
            strcat(inAcctInfo, inUserAcctPassword);
        }
        else
        {
            printf("=> TARGET ACCOUNT\n");
            scanf("%s", inAcctInfo);
            while (getchar() != '\n')
                ;
            printf("%s\n", inAcctInfo);
        }
    } while (validateAcctInfo(validatingUser, inAcctInfo) == EXIT_FAILURE);

    if (validatingUser)
    {
        strcpy(userAcctNumber, inUserAcctNumber);
    }
    else
    {
        strcpy(targetAcctNumber, inAcctInfo);
    }

    memset(inAcctInfo, '\0', 100);
}

void chooseService()
{
    char input[100];

    while (1)
    {
        printf("=> PLEASE CHOOSE YOUR SERVICE\n"
               "=> PRESS D FOR DEPOSIT\n"
               "=> PRESS W FOR WITHDRAWAL\n"
               "=> PRESS T FOR TRANSFER\n");
        scanf("%s", input);
        while (getchar() != '\n')
            ;
        printf("%s\n", input);

        if (strcmp(input, "D") && strcmp(input, "W") && strcmp(input, "T"))
        {
            printf("=> INVALID INPUT\n");
            memset(input, '\0', 100);
            continue;
        }
        break;
    }

    chosenService = input[0];
}

void inputAmount()
{
    transacAmount = -1;
    while (1)
    {
        printf("=> AMOUNT\n");
        scanf("%lf", &transacAmount);
        while (getchar() != '\n')
            ;
        printf("%lf\n", transacAmount);

        printf("\n==> OPERATION: %+016.02Lf %c %08.02lf\n\n", userBalance, chosenService, transacAmount);
        if (transacAmount < 0)
        {
            printf("=> INVALID INPUT\n");
            continue;
        }
        else if (chosenService != 'D' && transacAmount > userBalance)
        {
            printf("=> INSUFFICIENT BALANCE\n");
            continue;
        }
        break;
    }
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
        while (getchar() != '\n')
            ;
        printf("%s\n", input);

        if (strcmp(input, "Y") && strcmp(input, "N"))
        {
            printf("=> INVALID INPUT\n");
            memset(input, '\0', 100);
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
    char chosenIfContinue = 'Y';

    do
    {
        if (checkMasterFileExists() == 1)
        {
            printf("[ABORT]: MASTER FILE NOT FOUND\n\n");
            return EXIT_FAILURE;
        }

        atmInitialize();

        int chosenAtm = chooseAtm();

        inputAcctInfo(VALIDATING_USER);
        if (userBalance < 0)
        {
            printf("=> NEGATIVE REMAINS TRANSACTION ABORT\n");
            continue;
        }

        chooseService();
        if (chosenService == 'T')
        {
            inputAcctInfo(VALIDATING_TARGET);
        }

        inputAmount();

        printf("==> WRITTEN RECORD: \n");

        FILE *chosenAtmFile = (chosenAtm == 711) ? t71_One_File : t71_Three_File;

        if (chosenService == 'T')
        {
            chosenService = 'W';
            fprintf(chosenAtmFile, "%s%c%07.0lf%05d\n", userAcctNumber, chosenService, transacAmount * 100.0, timeStamp++);
            printf("==> [ATM-%d]: %s %c %07.0lf %05d\n", chosenAtm, userAcctNumber, chosenService, transacAmount * 100.0, timeStamp);
            
            chosenService = 'D';
            fprintf(chosenAtmFile, "%s%c%07.0lf%05d\n", targetAcctNumber, chosenService, transacAmount * 100.0, timeStamp++);
            printf("==> [ATM-%d]: %s %c %07.0lf %05d\n", chosenAtm, targetAcctNumber, chosenService, transacAmount * 100.0, timeStamp);
        }
        else
        {
            fprintf(chosenAtmFile, "%s%c%07.0lf%05d\n", userAcctNumber, chosenService, transacAmount * 100.0, timeStamp++);
            printf("==> [ATM-%d]: %s %c %07.0lf %05d\n", chosenAtm, userAcctNumber, chosenService, transacAmount * 100.0, timeStamp);
        }
        printf("\n");

    } while (userBalance < 0 || chooseIfContinue() == 'Y');

    fclose(t71_One_File);
    fclose(t71_Three_File);
    return EXIT_SUCCESS;
}