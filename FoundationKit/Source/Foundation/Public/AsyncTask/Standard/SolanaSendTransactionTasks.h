// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsyncTask/SolanaJsonRPCAsyncTask.h"
#include "WalletAccount.h"
#include "SolanaUtils/Utils/Types.h"

class FOUNDATION_API FSolanaSendSOLAsyncTask: public FSolanaJsonRPCAsyncTask<FString>
{
public:

	FSolanaSendSOLAsyncTask(UINT _Id,const FAccount& from, const FString& to, int64 amount)
		: FSolanaJsonRPCAsyncTask(_Id),
		From(from),To(to),Lamports(amount) {
		MaxStepCount = 2;
	}
	virtual ~FSolanaSendSOLAsyncTask() {}

protected:

	FAccount From;
	FString To;
	uint64 Lamports;
	FString Blockhash;

	virtual void CreateRequestBody(FString& _Body) override;
	virtual FString ToString() const override;
	virtual void ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index) override;
	virtual void ParseResponseJson(const FString& ResponseAsString, int32 Index) override;
};


class FOUNDATION_API FSolanaSendTokenAsyncTask : public FSolanaJsonRPCAsyncTask<FString>
{
public:

	FSolanaSendTokenAsyncTask(UINT _Id, const FAccount& from, const FString& to, const FString& tokenAcc, const FString& mint, int64 amount)
		: FSolanaJsonRPCAsyncTask(_Id), TokenAccount(tokenAcc), Mint(mint), Amount(amount), To(to)
	{
		MaxStepCount = 4;
	}
	virtual ~FSolanaSendTokenAsyncTask() {}

protected:

	FAccount From;
	FString TokenAccount;
	FString Mint;
	FString To;
	uint64 Amount;
	FString Blockhash;

	FString TokenAccountKey;

	virtual void CreateRequestBody(FString& _Body) override;
	virtual FString ToString() const override;
	virtual void ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index) override;
	virtual void ParseResponseJson(const FString& ResponseAsString, int32 Index) override;
};