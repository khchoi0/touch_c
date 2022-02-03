#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

FILE *masterFile;
int validatingUser;

char userAcctInfo[17];
char balanceSign[2];
char strBalance[16];
double intBalance;

int chosenService;

char targetAcctInfo[17];

double transacAmount;

int masterFileExists()
{
    if ((masterFile = fopen("master.txt", "r")))
    {
        fclose(masterFile);
        return 1;
    }
    return 0;
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
    int userAtmChoice = -1;
    printf("=> PLEASE CHOOSE THE ATM\n"
           "=> PRESS 1 FOR ATM 711\n"
           "=> PRESS 2 FOR ATM 713\n");
    scanf("%d", &userAtmChoice);
    while (getchar() != '\n')
        ;
    return userAtmChoice;
}

int validateAcctInfo(char *inputAcctInfo)
{
    char nameBuffer[100];
    char acctInfoBuffer[100];
    char tmp[2];

    masterFile = fopen("master.txt", "r");
    while (fgets(nameBuffer, 21, masterFile))
    {
        if (validatingUser)
        {
            fgets(acctInfoBuffer, 23, masterFile);
            fgets(balanceSign, 2, masterFile);
            fgets(strBalance, 16, masterFile);
        }
        else
        {
            fgets(acctInfoBuffer, 17, masterFile);
        }
        while (getc(masterFile) != '\n')
            ;

        if (strcmp(acctInfoBuffer, inputAcctInfo) == 0)
        {
            printf("\n");
            printf("==>  HOLDER: %s \n", nameBuffer);
            printf("==>    ACCT: %.16s \n", acctInfoBuffer);
            if (validatingUser)
            {
                printf("==> BALANCE: %s%s \n", balanceSign, strBalance);
            }
            printf("\n");

            fclose(masterFile);
            if ((!validatingUser) && strcmp(acctInfoBuffer, userAcctInfo) == 0)
            {
                printf("=> YOU CANNOT TRANSFER TO YOURSELF\n");
                return 0;
            }
            return 1;
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
    return 0;
}

int inputAcctInfo()
{
    char inputAcctInfo[100];
    char inputUserAcctNumber[100];
    char userAcctPassword[100];

    do
    {
        if (validatingUser)
        {

            printf("=> ACCOUNT\n");
            scanf("%s", inputUserAcctNumber);
            while (getchar() != '\n')
                ;

            printf("=> PASSWORD\n");
            scanf("%s", userAcctPassword);
            while (getchar() != '\n')
                ;

            strcpy(inputAcctInfo, inputUserAcctNumber);
            strcat(inputAcctInfo, userAcctPassword);
        }
        else
        {
            printf("=> TARGET ACCOUT\n");
            scanf("%s", inputAcctInfo);
            while (getchar() != '\n')
                ;
        }
    } while (validateAcctInfo(inputAcctInfo) == 0);

    if (validatingUser)
    {
        strcpy(userAcctInfo, inputUserAcctNumber);
    }
    else
    {
        strcpy(targetAcctInfo, inputAcctInfo);
    }
    memset(inputAcctInfo, '\0', 100);

    return 0;
}

char chooseService()
{
    char input[100];
    printf("=> PLEASE CHOOSE YOUR SERVICE\n"
           "=> PRESS D FOR DEPOSIT\n"
           "=> PRESS W FOR WITHDRAWAL\n"
           "=> PRESS T FOR TRANSFER\n");
    scanf("%s", input);
    while (getchar() != '\n')
        ;
    if (strlen(input) != 1)
    {
        printf("=> strlen != 1 but == %d\n", strlen(input));
        return '\0';
    }
    return input[0];
}

void inputAmount()
{
    transacAmount = -1;
    intBalance = atoi(strBalance) / 100.0;
    while (1)
    {
        printf("=> AMOUNT\n");
        scanf("%lf", &transacAmount);
        while (getchar() != '\n')
            ;

        printf("==> OPERATION: %lf %c %lf\n\n", intBalance, chosenService, transacAmount);

        if (transacAmount < 0)
        {
            printf("=> INVALID INPUT\n");
            continue;
        }
        else if (chosenService != 'D' && transacAmount > intBalance)
        {
            printf("=> INSUFFICIENT BALANCE\n");
            continue;
        }
        break;
    }
}

int outputRecord(){

}

char chooseIfContinue()
{
    char input[100];
    printf("=> CONTINUE?\n"
           "=> N FOR NO\n"
           "=> Y FOR YES\n");
    scanf("%s", input);
    while (getchar() != '\n')
        ;
    if (strlen(input) != 1)
    {
        printf("=> strlen != 1 but == %d\n", strlen(input));
        return '\0';
    }
    return input[0];
}

int main()
{
    FILE *t71_One = fopen("trans711.txt", "w");
    FILE *t71_Three = fopen("trans713.txt", "w");

    int timeStamp = 0;
    char chosenIf = 'Y';

    while (chosenIf == 'Y')
    {
        if (!masterFileExists())
        {
            printf("[ABORT]: MASTER FILE NOT FOUND");
            return 1;
        }

        atmInitialize();

        int chosenAtm = chooseAtm();
        while (chosenAtm != 1 && chosenAtm != 2)
        {
            printf("=> INVALID INPUT\n");
            chosenAtm = chooseAtm();
        }

        validatingUser = 1;
        inputAcctInfo();
        if (validatingUser && strcmp(balanceSign, "-") == 0)
        {
            printf("=> NEGATIVE REMAINS TRANSACTION ABORT\n");
            continue;
        }

        chosenService = chooseService();
        while (chosenService != 'D' && chosenService != 'W' && chosenService != 'T')
        {
            printf("=> INVALID INPUT\n");
            chosenService = chooseService();
        }

        if (chosenService == 'T')
        {
            validatingUser = 0;
            inputAcctInfo();
        }
        inputAmount();

        printf("==> WRITTEN RECORD: \n");
        char writeStr[30];
        char chosenAtmName[8];
        FILE * chosenAtmFile;
        if (chosenAtm == 1)
        {
            chosenAtmFile = t71_One;
            strcpy(chosenAtmName, "ATM-711");
        }
        else
        {
            chosenAtmFile = t71_Three;
            strcpy(chosenAtmName, "ATM-713");
        }
        if (chosenService == 'T')
        {   
            sprintf(writeStr, "%s%c%07d%05d\n", userAcctInfo, 'W', (int)(transacAmount * 100), timeStamp++);
            fputs( writeStr, chosenAtmFile);
            sprintf(writeStr, "%s%c%07d%05d\n", targetAcctInfo, 'D', (int)(transacAmount * 100), timeStamp++);
            fputs( writeStr, chosenAtmFile);

            printf("==> [%s]: %s %c %07d %05d\n", chosenAtmName, userAcctInfo, 'W', (int)(transacAmount * 100), timeStamp);
            printf("==> [%s]: %s %c %07d %05d\n", chosenAtmName, targetAcctInfo, 'D', (int)(transacAmount * 100), timeStamp);
        }
        else
        {
            sprintf(writeStr, "%s%c%07d%05d\n", userAcctInfo, chosenService, (int)(transacAmount * 100), timeStamp++);
            fputs( writeStr, chosenAtmFile);
            printf("==> [%s]: %s %c %07d %05d\n", chosenAtmName, userAcctInfo, chosenService, (int)(transacAmount * 100), timeStamp);
        }
        printf("\n");

        chosenIf = chooseIfContinue();
        while (chosenIf != 'Y' && chosenIf != 'N')
        {
            printf("=> INVALID INPUT\n");
            chosenIf = chooseIfContinue();
        }
    }

    fclose(t71_One);
    fclose(t71_Three);
    return 0;
}