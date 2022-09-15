#ifndef APP_C_
#define APP_C_
#include "server.h"

void appStart(void)
{
	ST_cardData_t cardData;
	ST_terminalData_t terminalData;
	ST_transaction_t transdata;

	printf("------------------------Welcome!------------------------\n");
	/************************************CARD MODULE******************************************/
	EN_cardError_t CardErrorState = CARD_OK;
	/*Get Card Holder Name*/
	CardErrorState = getCardHolderName(&cardData);
	while (CardErrorState == WRONG_NAME)
	{
		printf("WRONG_NAME\n");
		CardErrorState = getCardHolderName(&cardData);
	}
	/*Get Card Expiry Date*/
	CardErrorState = getCardExpiryDate(&cardData);
	if (CardErrorState == WRONG_EXP_DATE)
	{
		printf("WRONG_EXP_DATE\n");
		return;
	}
	/*Get Card Pan*/
	CardErrorState = getCardPAN(&cardData);
	while (CardErrorState == WRONG_PAN)
	{
		printf("WRONG_PAN\n");
		CardErrorState = getCardPAN(&cardData);
	}

	/************************************TERMINAL MODULE******************************************/
	EN_terminalError_t TerminalErrorState = TERMINAL_OK;
	/*Getting the Transaction Date*/
	TerminalErrorState = getTransactionDate(&terminalData);
	while (TerminalErrorState == WRONG_DATE)
	{
		printf("WRONG_DATE\n");
		TerminalErrorState = getTransactionDate(&terminalData);
	}
	/*Setting The Maximum amount of the Transaction for the Terminal*/
	TerminalErrorState = setMaxAmount(&terminalData);
	while (TerminalErrorState == INVALID_MAX_AMOUNT)
	{
		printf("INVALID_MAX_AMOUNT\n");
		TerminalErrorState = setMaxAmount(&terminalData);
	}
	
	/*Checking if the Card is expired or not*/
	TerminalErrorState = isCardExpired(&cardData, &terminalData);
	if (TerminalErrorState == EXPIRED_CARD)
	{
		printf("Declined Expired Card!!\n");
		return;
	}
	/*Get the Transaction Amount*/
	TerminalErrorState = getTransactionAmount(&terminalData);
	/*Checking if the Transaction amount exceeds its Max amount or not*/
	TerminalErrorState = isBelowMaxAmount(&terminalData);
	if (TerminalErrorState == EXCEED_MAX_AMOUNT)
	{
		printf("Declined Amount Exceeding Limit\n");
		return;
	}
	/************************************Server MODULE******************************************/
	EN_transState_t ServerErrorState = SERVER_OK;
	/*Saving Data in Transaction data with card and terminal data*/
	transdata.cardHolderData = cardData;
	transdata.terminalData = terminalData;
	/*Checking the account details and amount availability*/
	ServerErrorState = recieveTransactionData(&transdata);
	/*Checking the state of the Transaction*/
	if (ServerErrorState == DECLINED_STOLEN_CARD)
	{
		printf("Declined Stolen Card !!\n");
	}
	else if (ServerErrorState == DECLINED_INSUFFECIENT_FUND)
	{
		printf("Declined Insuffecient Fund !!\n");
	}
	else if (ServerErrorState == APPROVED)
	{
		printf("APPROVED\n");
		printf("Updating Account ....\n Transaction is Saved \n");
	}


}	

#endif