// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "APIAsyncTaskManager.h"

class APIASYNCTASKKIT_API FHttpRequestAsyncTask : public FAPIAsyncTaskBasic
{
public:

	FHttpRequestAsyncTask() : FAPIAsyncTaskBasic() {}
	virtual ~FHttpRequestAsyncTask() {}

protected:

	virtual void CreateRequestBody(FString& _Body);
	void SendRequest();
	void OnResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	virtual bool OnHttpReponseBody(const FHttpResponsePtr& Response, FString& Reason);
	virtual void SetupRequest(FHttpRequestRef& Request);

	FString Body;
	bool bIsRequestSent = false;
	FString ErrorReason = TEXT("");

	virtual FString ToString() const override;
	virtual void Tick() override;

	int32 MaxStepCount = 1;

	FORCEINLINE int32 GetCurrentStep() const { return _CurrentStep; }

private:

	int32 _CurrentStep = 0;
	int32 _PrevStep = 0;
};

