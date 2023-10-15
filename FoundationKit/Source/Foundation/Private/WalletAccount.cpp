/*
Copyright 2022 ATMTA, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Daniele Calanna
Contributers: Riccardo Torrisi, Federico Arona
*/

#include "WalletAccount.h"

#include "JsonObjectConverter.h"
#include "TokenAccount.h"
#include "Network/RequestManager.h"
#include "Network/RequestUtils.h"
#include "AsyncTask/Standard/SolanaStandardTasks.h"
#include "AsyncTask/Standard/SolanaSendTransactionTasks.h"
#include "SolanaUtils/Utils/TransactionUtils.h"

void UWalletAccount::SetAccountName(const FString& Name)
{
	if (AccountData.Name != Name)
	{
		AccountData.Name = Name;
		OnAccountNameChanged.Broadcast(this, Name);
	}
}

void UWalletAccount::Update()
{
	UpdateData();
	UpdateTokenAccounts();
}

void UWalletAccount::UpdateData()
{
	FSolanaRequestAccountInfoAsyncTask* NewTask = new FSolanaRequestAccountInfoAsyncTask(FRequestManager::GetNextMessageID(), AccountData.PublicKey);
	NewTask->Callback.BindLambda([this](UINT Id, const FAccountInfoJson& AccountInfo)
	{
		UpdateFromAccountInfoJson(AccountInfo);
	});
	AsyncTaskMgr->AddToParallelTasks(NewTask);
}
void UWalletAccount::UpdateTokenAccounts()
{
	FSolanaRequestAllTokenAccountsAsyncTask* NewTask = new FSolanaRequestAllTokenAccountsAsyncTask(FRequestManager::GetNextMessageID(), AccountData.PublicKey);
	NewTask->Callback.BindLambda([this](UINT Id, const FTokenAccountArrayJson& TokenAccountJson)
		{
			if (TokenAccountJson.value.Num() > 0)
			{
				for (FTokenBalanceDataJson entry : TokenAccountJson.value)
				{
					FTokenInfoJson info = entry.account.data.parsed.info;
					UTokenAccount* const* TokenAccountPtr = TokenAccounts.Find(info.mint);

					UTokenAccount* TokenAccount = nullptr;
					if (TokenAccountPtr != nullptr)
						TokenAccount = (*TokenAccountPtr);
					else
					{
						TokenAccount = NewObject<UTokenAccount>(this);
						if (IsValid(TokenAccount))
						{
							TokenAccount->BindAsyncTaskManager(AsyncTaskMgr);
							TokenAccounts.Add(info.mint, TokenAccount);
						}
					}
					if (IsValid(TokenAccount))
					{
						FAccountData& account = TokenAccount->AccountData;
						account.Pubkey = entry.pubkey;
						account.Balance = info.tokenAmount.uiAmount;
						account.Mint = info.mint;
						TokenAccount->OnBalanceUpdated.Broadcast(TokenAccount, account.Balance);
						OnTokenAccountAdded.Broadcast(this, TokenAccount);
					}
				}
			}
			OnTokenAccountReceived.Broadcast();
		});
	AsyncTaskMgr->AddToParallelTasks(NewTask);
}


void UWalletAccount::UpdateFromAccountInfoJson(const FAccountInfoJson& AccountInfoJson)
{
	Lamports = AccountInfoJson.lamports;
	OnSolBalanceChanged.Broadcast(this, GetSolBalance());
}
void UWalletAccount::SendSOL(const FString& RecipientPublicKey, int64 amount)
{
	FSolanaSendSOLAsyncTask* NewTask = new FSolanaSendSOLAsyncTask(FRequestManager::GetNextMessageID(), AccountData, RecipientPublicKey, amount);
	NewTask->Callback.BindLambda([this](UINT Id, const FString& TxHash)
		{
			FRequestUtils::DisplayInfo(FString::Printf(TEXT("Transaction Id: %s"), *TxHash));
			UpdateData();
		});
	NewTask->ErrorCallback.BindLambda([this](UINT Id, const FString& ErrorMessage)
		{
			FRequestUtils::DisplayError(ErrorMessage);
		});

	AsyncTaskMgr->AddToParallelTasks(NewTask);
}
/*void UWalletAccount::SendSOL(const FAccount& from, const FAccount& to, int64 amount) const
{
	FSolanaSendSOLAsyncTask* NewTask = new FSolanaSendSOLAsyncTask(FRequestManager::GetNextMessageID(), from, to, amount);
	NewTask->Callback.BindLambda([this](UINT Id, const FString& TxHash)
		{
			FRequestUtils::DisplayInfo(FString::Printf(TEXT("Transaction Id: %s"), *TxHash));
		});
	AsyncTaskMgr->AddToParallelTasks(NewTask);

	
	FRequestData* request = FRequestUtils::RequestBlockHash();
	request->Callback.BindLambda([this, from, to, amount](FJsonObject& data)
	{
		FString blockHash = FRequestUtils::ParseBlockHashResponse(data);

		TArray<uint8> tranaction = FTransactionUtils::TransferSOLTransaction(from, to, amount, blockHash);

		FRequestData* transaction = FRequestUtils::SendTransaction( FBase64::Encode(tranaction));
		transaction->Callback.BindLambda([this, from, to, amount](FJsonObject& data)
		{
			FString transactionID = FRequestUtils::ParseTransactionResponse(data);
			FRequestUtils::DisplayInfo(FString::Printf(TEXT("Transaction Id: %s"), *transactionID));
		});
		FRequestManager::SendRequest(transaction);
	});
	FRequestManager::SendRequest(request);
	
}

void UWalletAccount::SendSOLEstimate(const FAccount& from, const FAccount& to, int64 amount) const
{
	FRequestData* request = FRequestUtils::RequestBlockHash();
	request->Callback.BindLambda([this, from, to, amount](FJsonObject& data)
	{
		FString blockHash = FRequestUtils::ParseBlockHashResponse(data);

		TArray<uint8> transaction = FTransactionUtils::TransferSOLTransaction(from, to, amount, blockHash);
	
		FRequestData* feeRequest = FRequestUtils::GetTransactionFeeAmount( FBase64::Encode(transaction));
		feeRequest->Callback.BindLambda([this, from, to, amount](FJsonObject& data)
		{
			int fee = FRequestUtils::ParseTransactionFeeAmountResponse(data);
			FRequestUtils::DisplayInfo(FString::Printf(TEXT("Estimate Id: %i"), fee));
		});
		FRequestManager::SendRequest(feeRequest);

	});
	FRequestManager::SendRequest(request);
}*/

void UWalletAccount::SendToken(UTokenAccount* TokenAccount, const FString& RecipientPublicKey, float Amount)
{
	const FAccountData& TokenAccountData = TokenAccount->AccountData;
	FAccount RecipientAccount = FAccount::FromPublicKey(RecipientPublicKey);
	
	/*FSolanaRequestTokenAccountAsyncTask* NewTask = new FSolanaRequestTokenAccountAsyncTask(FRequestManager::GetNextMessageID(), TokenAccountData.Pubkey, TokenAccountData.Mint);
	NewTask->Callback.BindLambda([this](UINT Id, const FTokenBalanceDataJson& Data)
	{
			FRequestData* blockhashRequest = FRequestUtils::RequestBlockHash();
			blockhashRequest->Callback.BindLambda([this, TokenAccountData, RecipientAccount, Amount, Data.pubkey](FJsonObject& data)
				{
					FString blockHash = FRequestUtils::ParseBlockHashResponse(data);

					const TArray<uint8> transaction = FTransactionUtils::TransferTokenTransaction(AccountData, RecipientAccount, AccountData, Amount, TokenAccountData.Mint, blockHash, existingAccount);

					FRequestData* sendTransaction = FRequestUtils::SendTransaction(FBase64::Encode(transaction));
					sendTransaction->Callback.BindLambda([this](FJsonObject& data)
						{
							const FString transactionID = FRequestUtils::ParseTransactionResponse(data);
							FRequestUtils::DisplayInfo(FString::Printf(TEXT("Transaction Id: %s"), *transactionID));
						});
					FRequestManager::SendRequest(sendTransaction);
				});
			FRequestManager::SendRequest(blockhashRequest);
	});*/

	FRequestData* accountRequest = FRequestUtils::RequestTokenAccount(TokenAccountData.Pubkey, TokenAccountData.Mint);
	accountRequest->Callback.BindLambda([this, TokenAccountData, RecipientAccount, Amount](FJsonObject& data)
	{
		FString existingAccount = FRequestUtils::ParseTokenAccountResponse(data);
		if(!existingAccount.IsEmpty())
		{
			FRequestData* blockhashRequest = FRequestUtils::RequestBlockHash();
			blockhashRequest->Callback.BindLambda([this, TokenAccountData, RecipientAccount, Amount, existingAccount](FJsonObject& data)
			{
				FString blockHash = FRequestUtils::ParseBlockHashResponse(data);

				const TArray<uint8> transaction = FTransactionUtils::TransferTokenTransaction(AccountData, RecipientAccount, AccountData, Amount, TokenAccountData.Mint, blockHash, existingAccount);

				FRequestData* sendTransaction = FRequestUtils::SendTransaction(FBase64::Encode(transaction));
				sendTransaction->Callback.BindLambda([this](FJsonObject& data)
				{
					const FString transactionID = FRequestUtils::ParseTransactionResponse(data);
					FRequestUtils::DisplayInfo(FString::Printf(TEXT("Transaction Id: %s"), *transactionID));
				});
				FRequestManager::SendRequest(sendTransaction);
			});
			FRequestManager::SendRequest(blockhashRequest);
		}
	});
	FRequestManager::SendRequest(accountRequest);
}

void UWalletAccount::SendTokenEstimate(UTokenAccount* TokenAccount, const FString& RecipientPublicKey, float Amount) const
{
	const FAccountData& TokenAccountData = TokenAccount->AccountData;
	FAccount RecipientAccount = FAccount::FromPublicKey(RecipientPublicKey);
	

	FRequestData* accountRequest = FRequestUtils::RequestTokenAccount(TokenAccountData.Pubkey, TokenAccountData.Mint);
	accountRequest->Callback.BindLambda([this, TokenAccountData, RecipientAccount, Amount](FJsonObject& data)
	{
		FString existingAccount = FRequestUtils::ParseTokenAccountResponse(data);
		if(!existingAccount.IsEmpty())
		{
			FRequestData* blockhashRequest = FRequestUtils::RequestBlockHash();
			blockhashRequest->Callback.BindLambda([this, TokenAccountData, RecipientAccount, Amount, existingAccount](FJsonObject& data)
			{
				FString blockHash = FRequestUtils::ParseBlockHashResponse(data);
				const TArray<uint8> transaction = FTransactionUtils::TransferTokenTransaction(AccountData, RecipientAccount, AccountData, Amount, TokenAccountData.Mint, blockHash, existingAccount);

				FRequestData* sendTransaction = FRequestUtils::GetTransactionFeeAmount(FBase64::Encode(transaction));
				sendTransaction->Callback.BindLambda([this](FJsonObject& data)
				{
					int fee = FRequestUtils::ParseTransactionFeeAmountResponse(data);
					FRequestUtils::DisplayInfo(FString::Printf(TEXT("Estimate Id: %i"), fee));
				});
				FRequestManager::SendRequest(sendTransaction);
			});
			FRequestManager::SendRequest(blockhashRequest);
		}
	});
	FRequestManager::SendRequest(accountRequest);
}

float UWalletAccount::GetSolBalance() const
{
	return Lamports.Get(0.f) / 1e9;
}
