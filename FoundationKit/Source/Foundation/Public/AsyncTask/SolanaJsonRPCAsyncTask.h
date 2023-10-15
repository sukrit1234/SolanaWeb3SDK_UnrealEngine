// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonRPCAsyncTask.h"
#include "SolanaUtils/Utils/Types.h"

struct FSolanaJsonRpcGlobal
{
	static FString DefaultDevnetURL;
	static FString DefaultMainnetURL;
	static FString DefaultTestnetURL;
	static FString DefaultEmptyURL;
	static const FString& GetDefaultNetworkURL(ESolanaNetwork Network);
};

struct FSolanaRpcBody
{
	static void GetBlockHash(FString& Body, UINT Id);
	static void GetAccountInfoBody(FString& Body, UINT Id, const FString& PubKey);
	static void GetTokenAccountsByOwner(FString& Body, UINT Id, const FString& PubKey);
	static void GetTokenAccountsByOwner(FString& Body, UINT Id, const FString& PubKey, const FString& Mints);
	static void GetBalance(FString& Body, UINT Id, const FString& PubKey);
	static void GetProgramAccounts(FString& Body, UINT Id, const FString& ProgramID, const UINT& Size, const FString& PubKey);
	static void GetRequestAirdrop(FString& Body, UINT Id, const FString& PubKey);
	static void GetMultipleAccounts(FString& Body, UINT Id, const TArray<FString>& PubKeys);
	static void GetSendTransaction(FString& Body, UINT Id, const FString Base64Transaction);
	static void GetFeeForMessage(FString& Body, UINT Id, const FString Base64Transaction);
};


template<class R>
class FOUNDATION_API FSolanaJsonRPCAsyncTask : public FJsonRPCCallbackAsyncTask<R>
{
public:

	FSolanaJsonRPCAsyncTask(UINT _Id);
	virtual ~FSolanaJsonRPCAsyncTask() {}

protected:

	virtual FString ToString() const override;
	virtual const FString& GetRpcUrl() const override;
};

template<class R>
FSolanaJsonRPCAsyncTask<R>::FSolanaJsonRPCAsyncTask(UINT _Id) : FJsonRPCCallbackAsyncTask<R>(_Id) 
{

}