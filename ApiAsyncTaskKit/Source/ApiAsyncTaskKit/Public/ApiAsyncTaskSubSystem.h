// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "HAL/RunnableThread.h"
#include "APIAsyncTaskManager.h"

class APIASYNCTASKKIT_API FApiAsyncTaskSubSystem
{
public:
	
	FApiAsyncTaskSubSystem(const FString& Name);
	virtual ~FApiAsyncTaskSubSystem() {}

	virtual void Init();
	virtual void Shutdown();

	void AddToParallelTasks(FOnlineAsyncTask* NewTask);

protected:

	bool Tick(float DeltaTime);

	FAPIAsyncTaskManager* ApiAsyncTaskThreadRunnable;
	FRunnableThread* ApiAsyncTaskThread;

	FTSTicker::FDelegateHandle TickHandle;

	FString InstanceName;
};

typedef TSharedPtr<FApiAsyncTaskSubSystem, ESPMode::ThreadSafe> FApiAsyncTaskSubSystemPtr;
