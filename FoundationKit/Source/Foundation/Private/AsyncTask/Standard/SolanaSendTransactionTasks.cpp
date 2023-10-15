// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncTask/Standard/SolanaSendTransactionTasks.h"
#include "FoundationSettings.h"
#include "UObject/UObjectGlobals.h"
#include "JsonObjectConverter.h"
#include "SolanaUtils/Utils/Types.h"
#include "SolanaUtils/Utils/TransactionUtils.h"

void FSolanaSendSOLAsyncTask::ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index)
{
	int32 _step = GetCurrentStep();
	if (_step == 0) {
		const TSharedPtr<FJsonObject> value = ParsedJsonPtr->GetObjectField("value");
		Blockhash = value->GetStringField("blockhash");
	}
	
}
void FSolanaSendSOLAsyncTask::ParseResponseJson(const FString& ResponseAsString, int32 Index)
{
	int32 _step = GetCurrentStep();
	if (_step == 1)
		ReturnData = ResponseAsString;
}
FString FSolanaSendSOLAsyncTask::ToString() const
{
	return GetCallerString(TEXT("FSolanaSendSOLAsyncTask"));
}
void FSolanaSendSOLAsyncTask::CreateRequestBody(FString& _Body)
{
	int32 _step = GetCurrentStep();
	if (_step == 0)
		FSolanaRpcBody::GetBlockHash(_Body, Id);
	else if (_step == 1) {
		Id++;

		FAccount ToAcc = FAccount::FromPublicKey(To);
		TArray<uint8> tranaction = FTransactionUtils::TransferSOLTransaction(From, ToAcc, Lamports, Blockhash);
		FSolanaRpcBody::GetSendTransaction(_Body, Id, FBase64::Encode(tranaction));
	}	 
}


//////////////////////////


void FSolanaSendTokenAsyncTask::ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index)
{
	int32 _step = GetCurrentStep();
	if (_step == 0) {
		FTokenAccountArrayJson AccountArray;
		FJsonObjectConverter::JsonObjectToUStruct(ParsedJsonPtr.ToSharedRef(), &AccountArray);
		if (AccountArray.value.Num() > 0)
			TokenAccountKey = AccountArray.value[0].pubkey;
	}
	else if (_step == 1) {
		const TSharedPtr<FJsonObject> value = ParsedJsonPtr->GetObjectField("value");
		Blockhash = value->GetStringField("blockhash");
	}

}
void FSolanaSendTokenAsyncTask::ParseResponseJson(const FString& ResponseAsString, int32 Index)
{
	int32 _step = GetCurrentStep();
	if (_step == 1)
		ReturnData = ResponseAsString;
}
FString FSolanaSendTokenAsyncTask::ToString() const
{
	return GetCallerString(TEXT("FSolanaSendTokenAsyncTask"));
}
void FSolanaSendTokenAsyncTask::CreateRequestBody(FString& _Body)
{
	int32 _step = GetCurrentStep();
	if (_step == 0)
		FSolanaRpcBody::GetTokenAccountsByOwner(_Body, Id, TokenAccount, Mint);
	else if (_step == 1)
		FSolanaRpcBody::GetBlockHash(_Body, Id);
	else if (_step == 2) {
		Id++;
		FAccount ToAcc = FAccount::FromPublicKey(To);
		TArray<uint8> tranaction;// = FTransactionUtils::TransferTokenTransaction(From, ToAcc, Amount, Blockhash);
		FSolanaRpcBody::GetSendTransaction(_Body, Id, FBase64::Encode(tranaction));
	}
}
