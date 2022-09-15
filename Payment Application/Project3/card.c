#ifndef _CARD_C_
#define _CARD_C_
#include <string.h>
#include <stdio.h>
#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	uint8_t i = 0;
	printf("Please Enter your Name: ");
	gets(cardData->cardHolderName);

	while (i != strlen(cardData->cardHolderName))
	{
		if (!isalpha(cardData->cardHolderName[i]) && cardData->cardHolderName[i] != ' ')
		{
			return WRONG_NAME;
		}
		i++;
	}
	if (strlen(cardData->cardHolderName) < 20 || strlen(cardData->cardHolderName) > 24 || cardData->cardHolderName == NULL )
	{
		return WRONG_NAME;
	}
	else
	{
		return CARD_OK;
	}
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	uint8_t monthExpired, yearExpired;
	printf("Please Enter your Expiry date:");
	gets(cardData->cardExpirationDate);
	//			month/year				ex: 08/25
	monthExpired = ((cardData->cardExpirationDate[0] - '0') * 10) + (cardData->cardExpirationDate[1] - '0');
	yearExpired = ((cardData->cardExpirationDate[3] - '0') * 10) + (cardData->cardExpirationDate[4] - '0');

	if (strlen(cardData->cardExpirationDate) == 5 && cardData->cardExpirationDate != NULL)
	{
		if (( monthExpired <= 12 ) && ( yearExpired >= 22 ) && (cardData->cardExpirationDate[2] == '/'))
		{
			return CARD_OK ;
		}
		else
		{
			return WRONG_EXP_DATE;
		}
	}
	else
	{
		return WRONG_EXP_DATE;
	}
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	uint8_t i = 0;
	printf("Please Enter your Card Pan:");
	gets(cardData->primaryAccountNumber);

	while (i != strlen(cardData->primaryAccountNumber))
	{
		if (!isdigit(cardData->primaryAccountNumber[i]))
		{
			return WRONG_PAN;
		}
		i++;
	}

	if (cardData->primaryAccountNumber == NULL || strlen(cardData->primaryAccountNumber) < 16 || strlen(cardData->primaryAccountNumber) > 19)
	{
		return WRONG_PAN;
	}
	else
	{
		return CARD_OK;
	}
}

#endif