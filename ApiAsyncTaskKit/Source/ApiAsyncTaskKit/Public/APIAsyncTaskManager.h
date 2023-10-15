// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineAsyncTaskManager.h"


class APIASYNCTASKKIT_API FAPIAsyncTaskBasic : public FOnlineAsyncTask
{
PACKAGE_SCOPE:
	/** Has the task completed */
	FThreadSafeBool bIsComplete;
	/** Has the task complete successfully */
	FThreadSafeBool bWasSuccessful;

	/** Hidden on purpose */
	FAPIAsyncTaskBasic()
		: bIsComplete(false)
		, bWasSuccessful(false)
	{

	}

public:

	FAPIAsyncTaskBasic(UObject* InCallerObject)
		: bIsComplete(false)
		, bWasSuccessful(false)
	{

	}

	virtual ~FAPIAsyncTaskBasic(){}

	/**
	 * Copies the contents of this task to another
	 * Use sparingly, it is not atomic.
	 */
	FAPIAsyncTaskBasic& operator= (const FAPIAsyncTaskBasic& Other)
	{
		if (this != &Other)
		{
			bIsComplete = !!Other.bIsComplete;
			bWasSuccessful = !!Other.bWasSuccessful;
		}
		return *this;
	}
protected:

	FString GetCallerString(const FString& DefaultTaskName) const;
	virtual bool IsDone() const override { return bIsComplete; }
	virtual bool WasSuccessful() const override { return bWasSuccessful; }
	virtual void Finalize() override;
	virtual void TriggerDelegates() override;
};

class APIASYNCTASKKIT_API FAPIAsyncTaskManager : public FOnlineAsyncTaskManager
{
protected:

public:

	FAPIAsyncTaskManager();
	virtual ~FAPIAsyncTaskManager();

	FORCEINLINE uint32 GetThreadID() const { return OnlineThreadId; }

	// FAPIAsyncTaskManager
	virtual void OnlineTick() override;
};