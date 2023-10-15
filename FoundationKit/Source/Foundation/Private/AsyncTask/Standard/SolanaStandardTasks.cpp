// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncTask/Standard/SolanaStandardTasks.h"
#include "FoundationSettings.h"
#include "UObject/UObjectGlobals.h"
#include "JsonObjectConverter.h"
#include "SolanaUtils/Utils/Types.h"




void FSolanaRequestAccountInfoAsyncTask::ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index)
{
	const TSharedPtr<FJsonObject> resultData = ParsedJsonPtr->GetObjectField("value");
	FJsonObjectConverter::JsonObjectToUStruct(resultData.ToSharedRef(), &ReturnData);
}
FString FSolanaRequestAccountInfoAsyncTask::ToString() const
{
	return GetCallerString(TEXT("FSolanaRequestAccountInfoAsyncTask"));
}
void FSolanaRequestAccountInfoAsyncTask::CreateRequestBody(FString& _Body)
{
	FSolanaRpcBody::GetAccountInfoBody(_Body, Id, PublicKey);
}

void FSolanaRequestAllTokenAccountsAsyncTask::CreateRequestBody(FString& _Body)
{
	FSolanaRpcBody::GetTokenAccountsByOwner(_Body, Id, PublicKey);
}
FString FSolanaRequestAllTokenAccountsAsyncTask::ToString() const
{
	return GetCallerString(TEXT("FSolanaRequestAllTokenAccountsAsyncTask"));
}
void FSolanaRequestAllTokenAccountsAsyncTask::ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index)
{
	FJsonObjectConverter::JsonObjectToUStruct(ParsedJsonPtr.ToSharedRef(), &ReturnData);
}

void FSolanaRequestBalanceAsyncTask::CreateRequestBody(FString& _Body)
{
	FSolanaRpcBody::GetBalance(_Body, Id, PublicKey);
}
void FSolanaRequestBalanceAsyncTask::ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index)
{
	FJsonObjectConverter::JsonObjectToUStruct(ParsedJsonPtr.ToSharedRef(), &ReturnData);
}
FString FSolanaRequestBalanceAsyncTask::ToString() const
{
	return GetCallerString(TEXT("FSolanaRequestBalanceAsyncTask"));
}

void FSolanaRequestTokenAccountAsyncTask::CreateRequestBody(FString& _Body)
{
	FSolanaRpcBody::GetTokenAccountsByOwner(_Body, Id, PublicKey, Mint);
}
FString FSolanaRequestTokenAccountAsyncTask::ToString() const
{
	return GetCallerString(TEXT("FSolanaRequestAllTokenAccountsAsyncTask"));
}
void FSolanaRequestTokenAccountAsyncTask::ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index)
{
	FTokenAccountArrayJson AccountArray;
	FJsonObjectConverter::JsonObjectToUStruct(ParsedJsonPtr.ToSharedRef(), &AccountArray);
	if (AccountArray.value.Num() > 0)
		ReturnData = AccountArray.value[0];
}

void FSolanaRequestProgramAccountsAsyncTask::CreateRequestBody(FString& _Body)
{
	FSolanaRpcBody::GetProgramAccounts(_Body, Id, ProgramID, Size, PublicKey);
}
void FSolanaRequestProgramAccountsAsyncTask::BeginParseResponseJson(int32 Num)
{
	ReturnData.Reset();
}
void FSolanaRequestProgramAccountsAsyncTask::ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index)
{
	if (ParsedJsonPtr.IsValid())
	{
		if (TSharedPtr<FJsonObject> account = ParsedJsonPtr->GetObjectField("account"))
		{
			FProgramAccountJson accountData;
			FJsonObjectConverter::JsonObjectToUStruct(account.ToSharedRef(), &accountData);
			ReturnData.Add(accountData);
		}
	}
}
FString FSolanaRequestProgramAccountsAsyncTask::ToString() const
{
	return GetCallerString(TEXT("FSolanaRequestProgramAccountsAsyncTask"));
}

void FSolanaRequestMultipleAccountAsyncTask::ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index)
{
	ReturnData.Reset();
#if ENGINE_MAJOR_VERSION >= 5
		FJsonObjectConverter::JsonArrayToUStruct(ParsedJsonPtr->GetArrayField("value"), &ReturnData);
#else
		FJsonObjectConverter::JsonArrayToUStruct(ParsedJsonPtr->GetArrayField("value"), &ReturnData, 0, 0);
#endif
}
FString FSolanaRequestMultipleAccountAsyncTask::ToString() const
{
	return GetCallerString(TEXT("FSolanaRequestMultipleAccountAsyncTask"));
}
void FSolanaRequestMultipleAccountAsyncTask::CreateRequestBody(FString& _Body)
{
	FSolanaRpcBody::GetMultipleAccounts(_Body, Id, PublicKeys);
}

void FSolanaAirdropAsyncTask::CreateRequestBody(FString& _Body)
{
	FSolanaRpcBody::GetRequestAirdrop(_Body, Id, PublicKey);
}
FString FSolanaAirdropAsyncTask::ToString() const
{
	return GetCallerString(TEXT("FSolanaAirdropAsyncTask"));
}
void FSolanaAirdropAsyncTask::ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index)
{

}