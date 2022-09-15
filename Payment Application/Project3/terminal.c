#ifndef _TERMINAL_C_
#define _TERMINAL_C_

#include <string.h>
#include <stdio.h>
#include "terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	uint8_t day, month;
	uint16_t year;
	printf("Please enter your Transaction Date: ");
	gets(termData->transactionDate);
	day = (((termData->transactionDate[0] - '0') * 10) + (termData->transactionDate[1] - '0'));
	month = (((termData->transactionDate[3] - '0') * 10) + (termData->transactionDate[4] - '0'));
	year = ((termData->transactionDate[6] - '0') * 1000) + ((termData->transactionDate[7] - '0') * 100) 
			+ ((termData->transactionDate[8] - '0') * 10) + (termData->transactionDate[9] - '0');
	if (termData->transactionDate != NULL && strlen(termData->transactionDate) == 10 && (termData->transactionDate[2] == '/') && (termData->transactionDate[5] == '/'))
	{
		//			24/08/2022
		if ((day <= 31)	&& (month <= 12) && (year >= 2022))
		{
			return TERMINAL_OK;
		}
		else
		{
			return WRONG_DATE;
		}
		
	}
	else
	{
		return WRONG_DATE;
	}
}

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t* termData)
{
	uint8_t monthTransaction, monthExpired , yearTransaction, yearExpired;

	monthTransaction = ((termData->transactionDate[3] - '0') * 10) + (termData->transactionDate[4] - '0');
	monthExpired = ((cardData->cardExpirationDate[0] - '0') * 10) + (cardData->cardExpirationDate[1] - '0');

	yearTransaction = ((termData->transactionDate[8] - '0') * 10) + (termData->transactionDate[9] - '0');
	yearExpired = ((cardData->cardExpirationDate[3] - '0') * 10) + (cardData->cardExpirationDate[4] - '0');

	if (yearExpired < yearTransaction)
	{
		return EXPIRED_CARD;
	}
	if (monthExpired < monthTransaction)
	{
		return EXPIRED_CARD;
	}
	else
	{
		return TERMINAL_OK;
	}

}


EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	printf("Please Enter your Transaction amount: ");
	scanf_s(" %f",&termData->transAmount);
	
	if (termData->transAmount <= 0)
	{
		return INVALID_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount)
	{
		return EXCEED_MAX_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	printf("Please enter max amount allowed for transaction : ");
	scanf_s("%f",&termData->maxTransAmount);

	if (termData->maxTransAmount <= 0)
	{
		return INVALID_MAX_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}

#endif