// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonRPCAsyncTask.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

FString FJsonRPCGlobal::Verb = TEXT("POST");
FString FJsonRPCGlobal::ContentType = TEXT("Content-Type");
FString FJsonRPCGlobal::AppJson = TEXT("application/json");

FString FJsonRPCGlobal::IdField = TEXT("id");
FString FJsonRPCGlobal::ErrorField = TEXT("error");
FString FJsonRPCGlobal::ResultField = TEXT("result");
FString FJsonRPCGlobal::MessageField = TEXT("message");
FString FJsonRPCGlobal::EmptyRPC = TEXT("");

FString FJsonRPCAsyncTask::ToString() const
{
	return GetCallerString(TEXT("FJsonRPCAsyncTask"));
}

void FJsonRPCAsyncTask::SetupRequest(FHttpRequestRef& Request)
{
	Request->SetURL(GetRpcUrl());
	Request->SetVerb(FJsonRPCGlobal::Verb);
	Request->SetHeader(FJsonRPCGlobal::ContentType, FJsonRPCGlobal::AppJson);
}

void FJsonRPCAsyncTask::BeginParseResponseJson(int32 Num)
{

}

void FJsonRPCAsyncTask::ParseResponseJson(const TSharedPtr<FJsonObject>& ParsedJsonPtr, int32 Index)
{

}

void FJsonRPCAsyncTask::EndParseResponseJson()
{

}

void FJsonRPCAsyncTask::ParseResponseJson(const FString& ResultAsString, int32 Index)
{

}

bool FJsonRPCAsyncTask::OnHttpReponseBody(const FHttpResponsePtr& Response, FString& Reason)
{
	TSharedPtr<FJsonObject> ParsedJSONPtr;
	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(Reader, ParsedJSONPtr))
	{
		const FJsonObject& ParsedJson = (*ParsedJSONPtr);
		if (!ParsedJson.HasTypedField<EJson::Object>(FJsonRPCGlobal::ErrorField))
		{
			int id = ParsedJson.GetIntegerField(FJsonRPCGlobal::IdField);
			check(id == Id);

			if (ParsedJson.HasTypedField<EJson::Object>(FJsonRPCGlobal::ResultField))
			{
				TSharedPtr<FJsonObject> ResultJsonPtr = ParsedJson.GetObjectField(FJsonRPCGlobal::ResultField);
				if (ResultJsonPtr.IsValid())
				{
					BeginParseResponseJson(1);
					ParseResponseJson(ResultJsonPtr, 0);
					EndParseResponseJson();
				}
				return true;
			}
			else if (ParsedJson.HasTypedField<EJson::Array>(FJsonRPCGlobal::ResultField))
			{
				TArray<TSharedPtr<FJsonValue>> DataArray = ParsedJson.GetArrayField(FJsonRPCGlobal::ResultField);
				BeginParseResponseJson(DataArray.Num());
				for (auto It = DataArray.CreateConstIterator(); It; ++It)
				{
					TSharedPtr<FJsonValue> entryPtr = *It;
					if (entryPtr.IsValid())
					{
						const TSharedPtr<FJsonObject> entryObjectPtr = entryPtr->AsObject();
						if (entryObjectPtr.IsValid())
							ParseResponseJson(entryObjectPtr, It.GetIndex());
					}
				}
				EndParseResponseJson();
				return true;
			}
			else if (ParsedJson.HasTypedField<EJson::String>(FJsonRPCGlobal::ResultField)) {
				FString ResponseString = ParsedJson.GetStringField(FJsonRPCGlobal::ResultField);
				BeginParseResponseJson(1);
				ParseResponseJson(ResponseString, 0);
				EndParseResponseJson();
			}
			return false;
		}
		else
		{
			const TSharedPtr<FJsonObject> error = ParsedJson.GetObjectField(FJsonRPCGlobal::ErrorField);
			ErrorReason = error->GetStringField(FJsonRPCGlobal::MessageField);
			return false;
		}
	}
	else
	{
		ErrorReason = TEXT("Failed to parse Response from the server");
		return false;
	}
	return false;
}

const FString& FJsonRPCAsyncTask::GetRpcUrl() const
{
	return FJsonRPCGlobal::EmptyRPC;
}


void FJsonRPCAsyncTask::TriggerDelegates()
{
	FHttpRequestAsyncTask::TriggerDelegates();
	if (bIsComplete && !bWasSuccessful && ErrorCallback.IsBound())
		ErrorCallback.Execute(Id, ErrorReason);
}