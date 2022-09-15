#ifndef _SERVER_C_
#define _SERVER_C_

#include <string.h>
#include <stdio.h>
#include "server.h"


ST_accountsDB_t accountsDatabase[255] = { {2000.0, RUNNING, "8989374615436851"} , {100000.0, BLOCKED, "5807007076043875"} ,
									   {3000.0, BLOCKED, "8989654136546542"} , {200000.0, RUNNING, "6194026051065108"} ,
									   {4000.0, RUNNING, "9810246516847532"} , {300000.0, BLOCKED, "6549840620488651"} , 
									   {5000.0, BLOCKED, "6519848789461654"} , {400000.0, RUNNING, "5048401202150842"} ,
									   {6000.0, RUNNING, "3216879846246544"} , {500000.0, BLOCKED, "8781954150540650"} };

ST_transaction_t transactionDatabase[255] = { 0 };

int found = -1;


EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{

	if (isValidAccount(&transData->cardHolderData) == ACCOUNT_NOT_FOUND)
	{
		transData->transState = FRAUD_CARD;
		return FRAUD_CARD;
	}

	if (isAmountAvailable(&transData->terminalData) == LOW_BALANCE)
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		return DECLINED_INSUFFECIENT_FUND;
	}

	if (isBlockedAccount(&accountsDatabase[found]) == BLOCKED_ACCOUNT)
	{
		transData->transState = DECLINED_STOLEN_CARD;
		return DECLINED_STOLEN_CARD;
	}
	transData->transState = APPROVED;

	accountsDatabase[found].balance = accountsDatabase[found].balance - transData->terminalData.transAmount;

	if (saveTransaction(transData) == SAVING_FAILED)
	{
		return INTERNAL_SERVER_ERROR;
	}

	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	uint8_t counter;
	for (counter = 0; counter < 255; counter++)
	{
	if (strcmp(cardData->primaryAccountNumber, accountsDatabase[counter].primaryAccountNumber) == 0)
		{
		found = counter;
		return SERVER_OK;
		}
	}
	return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	if (accountRefrence->state == BLOCKED)
	{
		return BLOCKED_ACCOUNT;
	}
	else
	{
		return SERVER_OK;
	}
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{

			if (termData->transAmount < accountsDatabase[found].balance)
			{
				return SERVER_OK;
			}
			else
			{
				return LOW_BALANCE;
			}

}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	static uint8_t transNumber = 0;
	printf("Transaction Process:\n");
	printf("\tCard Holder Name : %s\n", transData->cardHolderData.cardHolderName);
	printf("\tPan: %s\n", transData->cardHolderData.primaryAccountNumber);
	printf("\tExpiry date: %s\n", transData->cardHolderData.cardExpirationDate);
	printf("\tTransaction amount: %.3f\n", transData->terminalData.transAmount);
	printf("\tMax Transaction amount: %.3f\n", transData->terminalData.maxTransAmount);
	printf("\tTransacation state: ");

	if (transData->transState == APPROVED)
	{
		printf("APPROVED\n");
	}
	else if (transData->transState == DECLINED_INSUFFECIENT_FUND)
	{
		printf("DECLINED_INSUFFECIENT_FUND\n");
	}
	else if (transData->transState == DECLINED_STOLEN_CARD)
	{
		printf("DECLINED_STOLEN_CARD\n");
	}
	transData->transactionSequenceNumber = transNumber+1;
	transactionDatabase[transData->transactionSequenceNumber].cardHolderData = transData->cardHolderData;
	transactionDatabase[transData->transactionSequenceNumber].terminalData = transData->terminalData;
	transactionDatabase[transData->transactionSequenceNumber].transactionSequenceNumber = transData->transactionSequenceNumber;
	transactionDatabase[transData->transactionSequenceNumber].transState = transData->transState;
	if (transData->transactionSequenceNumber == 255)
	{
		printf("SAVING_FAILED\n");
		return SAVING_FAILED;
	}


	printf("Transaction sequence number : %d\n", transData->transactionSequenceNumber);

	return SERVER_OK;

}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	return SERVER_OK;
}

#endif