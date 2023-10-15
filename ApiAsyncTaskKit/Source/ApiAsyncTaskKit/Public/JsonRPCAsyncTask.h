// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "HttpRequestAsyncTask.h"
#include "ApiAsyncTaskManager.h"

struct FJsonRPCGlobal {
	static FString Verb;
	static FString ContentType;
	static FString AppJson;
	static FString IdField;
	static FString ErrorField;
	static FString ResultField;
	static FString MessageField;
	static FString EmptyRPC;
};

class APIASYNCTASKKIT_API FJsonRPCAsyncTask : public FHttpRequestAsyncTask
{
public:

	DECLARE_DELEGATE_TwoParams(FErrorCallback, UINT, const FString& ErrorMessage);

	FJsonRPCAsyncTask(UINT _Id) : FHttpRequestAsyncTask() {
		Id = _Id;
	}
	virtual ~FJsonRPCAsyncTask() {}

	FORCEINLINE UINT GetRequestID() const { return Id; }
	FORCEINLINE const FString& RequestBody() const { return Body; }
	FORCEINLINE const FString& GetErrorReason() const { return ErrorReason; };

	FErrorCallback ErrorCallback;

protected:

	UINT Id;
	virtual void SetupRequest(FHttpRequestRef& Request) override;
	virtual bool OnHttpReponseBody(const FHttpResponsePtr& Response, FString& Reason) override;
	virtual const FString& GetRpcUrl() const;
	
	
	virtual void BeginParseResponseJson(int32 Num);
	virtual void ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index);
	virtual void ParseResponseJson(const FString& ResultAsString, int32 Index);
	virtual void EndParseResponseJson();

	virtual void TriggerDelegates() override;
	virtual FString ToString() const override;
};

template<class R>
class FJsonRPCCallbackAsyncTask : public FJsonRPCAsyncTask
{
public:

	DECLARE_DELEGATE_TwoParams(FSuccessfulCallback, UINT, const R&);
	
	FJsonRPCCallbackAsyncTask(UINT _Id) : FJsonRPCAsyncTask(_Id) {}

	FSuccessfulCallback Callback;

protected:

	virtual void TriggerDelegates() override;
	R ReturnData;
};

template<class R>
void FJsonRPCCallbackAsyncTask<R>::TriggerDelegates()
{
	FJsonRPCAsyncTask::TriggerDelegates();
	if (bIsComplete && bWasSuccessful && Callback.IsBound())
		Callback.Execute(Id, ReturnData);
}