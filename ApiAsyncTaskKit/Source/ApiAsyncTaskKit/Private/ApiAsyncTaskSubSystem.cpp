// Fill out your copyright notice in the Description page of Project Settings.


#include "ApiAsyncTaskSubSystem.h"

FApiAsyncTaskSubSystem::FApiAsyncTaskSubSystem(const FString& Name)
{
	InstanceName = Name;
}

void FApiAsyncTaskSubSystem::Init()
{
	ApiAsyncTaskThreadRunnable = new FAPIAsyncTaskManager();
	ApiAsyncTaskThread = FRunnableThread::Create(ApiAsyncTaskThreadRunnable, *FString::Printf(TEXT("ApiAsyncTaskThread %s"), *InstanceName));

	FTickerDelegate TickDelegate = FTickerDelegate::CreateRaw(this, &FApiAsyncTaskSubSystem::Tick);
	TickHandle = FTSTicker::GetCoreTicker().AddTicker(TickDelegate, 0.0f);
}
void FApiAsyncTaskSubSystem::Shutdown()
{
	if (ApiAsyncTaskThread)
	{
		delete ApiAsyncTaskThread;
		ApiAsyncTaskThread = nullptr;
	}
	if (ApiAsyncTaskThreadRunnable)
	{
		delete ApiAsyncTaskThreadRunnable;
		ApiAsyncTaskThreadRunnable = nullptr;
	}
	if (TickHandle != FTSTicker::FDelegateHandle())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
		TickHandle = FTSTicker::FDelegateHandle();
	}
}
bool FApiAsyncTaskSubSystem::Tick(float DeltaTime)
{
	if (ApiAsyncTaskThreadRunnable)
		ApiAsyncTaskThreadRunnable->GameTick();
	return (ApiAsyncTaskThreadRunnable != nullptr);
}
void FApiAsyncTaskSubSystem::AddToParallelTasks(FOnlineAsyncTask* NewTask)
{
	if (ApiAsyncTaskThreadRunnable != nullptr)
		ApiAsyncTaskThreadRunnable->AddToParallelTasks(NewTask);
}