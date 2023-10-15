// Fill out your copyright notice in the Description page of Project Settings.


#include "APIAsyncTaskManager.h"

FAPIAsyncTaskManager::FAPIAsyncTaskManager()
{

}
FAPIAsyncTaskManager::~FAPIAsyncTaskManager()
{

}
void FAPIAsyncTaskManager::OnlineTick()
{
	check(FPlatformTLS::GetCurrentThreadId() == OnlineThreadId);
	check(!IsInGameThread());
}

void FAPIAsyncTaskBasic::Finalize()
{
	FOnlineAsyncTask::Finalize();
}

void FAPIAsyncTaskBasic::TriggerDelegates()
{
	FOnlineAsyncTask::TriggerDelegates();
}

FString FAPIAsyncTaskBasic::GetCallerString(const FString& DefaultTaskName) const
{
	return DefaultTaskName;
}