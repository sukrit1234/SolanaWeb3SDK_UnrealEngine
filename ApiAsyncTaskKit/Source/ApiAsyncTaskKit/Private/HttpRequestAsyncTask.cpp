// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpRequestAsyncTask.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

void FHttpRequestAsyncTask::Tick()
{
	check(!IsInGameThread());

	if (!bIsRequestSent)
	{
		CreateRequestBody(Body);
		SendRequest();
		bIsRequestSent = true;
	}
}

FString FHttpRequestAsyncTask::ToString() const
{
	return GetCallerString(TEXT("FHttpRequestAsyncTask"));
}

void FHttpRequestAsyncTask::SendRequest()
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	SetupRequest(Request);
	Request->SetContentAsString(Body);
	Request->OnProcessRequestComplete().BindRaw(this, &FHttpRequestAsyncTask::OnResponse);
	Request->ProcessRequest();
}

bool FHttpRequestAsyncTask::OnHttpReponseBody(const FHttpResponsePtr& Response, FString& Reason)
{
	Reason = TEXT("Unknown Http Error");
	return false;
}

void FHttpRequestAsyncTask::OnResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess) 
	{
		if (Response.IsValid())
		{
			if (!OnHttpReponseBody(Response, ErrorReason))
				bSuccess = false;
		}
		else
			bSuccess = false;
	}
	else
	{
		ErrorReason = TEXT("Http Request Failed");
		bSuccess = false;
	}

	_PrevStep = _CurrentStep;
	if (bSuccess)
		_CurrentStep++;

	bWasSuccessful = (_CurrentStep >= MaxStepCount);
	bIsComplete = (!bSuccess) || (_CurrentStep >= MaxStepCount);
	if (!bIsComplete)
		bIsRequestSent = false;
}

void FHttpRequestAsyncTask::CreateRequestBody(FString& _Body)
{

}

void FHttpRequestAsyncTask::SetupRequest(FHttpRequestRef& Request)
{

}