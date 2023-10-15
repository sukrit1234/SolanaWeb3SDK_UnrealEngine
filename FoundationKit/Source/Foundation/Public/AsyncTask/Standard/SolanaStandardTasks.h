// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsyncTask/SolanaJsonRPCAsyncTask.h"
#include "SolanaUtils/Utils/Types.h"

class FOUNDATION_API FSolanaRequestAccountInfoAsyncTask : public FSolanaJsonRPCAsyncTask<FAccountInfoJson>
{
public:

	FSolanaRequestAccountInfoAsyncTask(UINT _Id, const FString& pubKey) 
		: FSolanaJsonRPCAsyncTask(_Id) , 
		PublicKey(pubKey)
	{
		ReturnData = FAccountInfoJson::Empty;
	}
	virtual ~FSolanaRequestAccountInfoAsyncTask() {}

protected:

	FString PublicKey;
	virtual void CreateRequestBody(FString& _Body) override;
	virtual FString ToString() const override;
	virtual void ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index) override;
};

class FOUNDATION_API FSolanaRequestAllTokenAccountsAsyncTask : public FSolanaJsonRPCAsyncTask<FTokenAccountArrayJson>
{
public:
	FSolanaRequestAllTokenAccountsAsyncTask(UINT _Id, const FString& pubKey)
		: FSolanaJsonRPCAsyncTask(_Id),
		PublicKey(pubKey) {
		ReturnData.value.Reset();
	}
	virtual ~FSolanaRequestAllTokenAccountsAsyncTask() {}

protected:

	FString PublicKey;

	virtual void CreateRequestBody(FString& _Body) override;
	virtual FString ToString() const override;
	virtual void ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index) override;
};

class FOUNDATION_API FSolanaRequestTokenAccountAsyncTask : public FSolanaJsonRPCAsyncTask<FTokenBalanceDataJson>
{
public:

	FSolanaRequestTokenAccountAsyncTask(UINT _Id, const FString& pubKey, const FString& mint)
		: FSolanaJsonRPCAsyncTask(_Id),
		PublicKey(pubKey), 
		Mint(mint){
		ReturnData = FTokenBalanceDataJson::Empty;
	}
	virtual ~FSolanaRequestTokenAccountAsyncTask() {}

protected:

	FString PublicKey;
	FString Mint;

	virtual void CreateRequestBody(FString& _Body) override;
	virtual FString ToString() const override;
	virtual void ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index) override;
};

class FOUNDATION_API FSolanaRequestBalanceAsyncTask : public FSolanaJsonRPCAsyncTask<FBalanceResultJson>
{
public:


	FSolanaRequestBalanceAsyncTask(UINT _Id) : FSolanaJsonRPCAsyncTask(_Id) {
		ReturnData = FBalanceResultJson::Empty;
	}
	virtual ~FSolanaRequestBalanceAsyncTask() {}

protected:
	FString PublicKey;
	virtual void CreateRequestBody(FString& _Body) override;
	virtual FString ToString() const override;
	virtual void ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index) override;
};

class FOUNDATION_API FSolanaRequestProgramAccountsAsyncTask : public FSolanaJsonRPCAsyncTask<TArray<FProgramAccountJson>>
{
public:

	FSolanaRequestProgramAccountsAsyncTask(UINT _Id, const FString& programID, const UINT& size, const FString& pubKey)
		: FSolanaJsonRPCAsyncTask(_Id), ProgramID(programID), Size(size), PublicKey(pubKey)
	{
		ReturnData.Reset();
	}
	virtual ~FSolanaRequestProgramAccountsAsyncTask() {}

protected:

	FString ProgramID;
	UINT Size;
	FString PublicKey;

	virtual FString ToString() const override;
	virtual void CreateRequestBody(FString& _Body) override;
	virtual void BeginParseResponseJson(int32 Num) override;
	virtual void ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index) override;
};

class FOUNDATION_API FSolanaRequestMultipleAccountAsyncTask : public FSolanaJsonRPCAsyncTask<TArray<FAccountInfoJson>>
{
public:
	
	
	FSolanaRequestMultipleAccountAsyncTask(UINT _Id,const TArray<FString>& pubKeys) 
		: FSolanaJsonRPCAsyncTask(_Id),
		PublicKeys(pubKeys) 
	{
		ReturnData.Reset();
	}
	virtual ~FSolanaRequestMultipleAccountAsyncTask() {}

protected:

	TArray<FString> PublicKeys;

	virtual FString ToString() const override;
	virtual void CreateRequestBody(FString& _Body) override;
	virtual void ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index) override;
};

class FOUNDATION_API FSolanaAirdropAsyncTask : public FSolanaJsonRPCAsyncTask<int32>
{
public:

	FSolanaAirdropAsyncTask(UINT _Id, const FString& pubKey)
		: FSolanaJsonRPCAsyncTask(_Id),
		PublicKey(pubKey)
	{
		
	}
	virtual ~FSolanaAirdropAsyncTask() {}

protected:

	FString PublicKey;
	virtual void CreateRequestBody(FString& _Body) override;
	virtual FString ToString() const override;
	virtual void ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index) override;
};