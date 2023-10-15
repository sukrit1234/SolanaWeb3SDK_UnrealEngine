// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncTask/SolanaJsonRPCAsyncTask.h"
#include "FoundationSettings.h"
#include "UObject/UObjectGlobals.h"
#include "JsonObjectConverter.h"

FString FSolanaJsonRpcGlobal::DefaultDevnetURL = TEXT("https://api.devnet.solana.com");
FString FSolanaJsonRpcGlobal::DefaultMainnetURL = TEXT("https://api.mainnet-beta.solana.com");
FString FSolanaJsonRpcGlobal::DefaultTestnetURL = TEXT("https://api.testnet.solana.com");
FString FSolanaJsonRpcGlobal::DefaultEmptyURL = TEXT("");


void FSolanaRpcBody::GetBlockHash(FString& Body, UINT Id)
{
	Body = FString::Printf(TEXT(R"({"id":%d,"jsonrpc":"2.0","method":"getRecentBlockhash","params":[{"commitment":"processed"}]})"), Id);
}
void FSolanaRpcBody::GetAccountInfoBody(FString& Body, UINT Id, const FString& PubKey)
{
	Body = FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%u,"method":"getAccountInfo","params":["%s",{"encoding": "base58"}]})"), Id, *PubKey);
}
void FSolanaRpcBody::GetTokenAccountsByOwner(FString& Body, UINT Id, const FString& PubKey)
{
	Body = FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getTokenAccountsByOwner","params":["%s",{"programId": "%s"},{"encoding": "jsonParsed"}]})"), Id, *PubKey, *TokenProgramId);
}
void FSolanaRpcBody::GetTokenAccountsByOwner(FString& Body, UINT Id, const FString& PubKey, const FString& Mint)
{
	Body = FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getTokenAccountsByOwner","params":["%s",{"mint": "%s"},{"encoding": "jsonParsed"}]})"), Id, *PubKey, *Mint);
}
void FSolanaRpcBody::GetBalance(FString& Body, UINT Id, const FString& PubKey)
{
	Body = FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getBalance","params":["%s",{"commitment": "processed"}]})"), Id, *PubKey);
}
void FSolanaRpcBody::GetProgramAccounts(FString& Body, UINT Id, const FString& ProgramID, const UINT& Size, const FString& PubKey)
{
	Body = FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getProgramAccounts","params":["%s",{"encoding":"base64","filters":[{"dataSize":%d},{"memcmp":{"offset":8,"bytes":"%s"}}]}]})"), Id, *ProgramID, Size, *PubKey);
}
void FSolanaRpcBody::GetRequestAirdrop(FString& Body, UINT Id, const FString& PubKey)
{
	Body = FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d, "method":"requestAirdrop", "params":["%s", 1000000000]})"), Id, *PubKey);
}
void FSolanaRpcBody::GetMultipleAccounts(FString& Body, UINT Id, const TArray<FString>& PubKeys)
{
	FString list;
	for (FString key : PubKeys)
	{
		list.Append(FString::Printf(TEXT(R"("%s")"), *key));
		if (key != PubKeys.Last())
			list.Append(",");
	}
	Body = FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method": "getMultipleAccounts","params":[[%s],{"dataSlice":{"offset":0,"length":0}}]})"), Id, *list);
}
void FSolanaRpcBody::GetSendTransaction(FString& Body, UINT Id, const FString Base64Transaction)
{
	Body = FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"sendTransaction","params":["%s",{"encoding": "base64"}]})"), Id, *Base64Transaction);
}
void FSolanaRpcBody::GetFeeForMessage(FString& Body, UINT Id, const FString Base64Transaction)
{
	Body = FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getFeeForMessage", "params":[%s,{"commitment":"processed"}]})"), Id, *Base64Transaction);
}


const FString& FSolanaJsonRpcGlobal::GetDefaultNetworkURL(ESolanaNetwork Network)
{
	switch (Network)
	{
	case ESolanaNetwork::MainNetBeta:
		return FSolanaJsonRpcGlobal::DefaultMainnetURL;
		break;
	case ESolanaNetwork::DevNet:
		return FSolanaJsonRpcGlobal::DefaultDevnetURL;
		break;
	case ESolanaNetwork::TestNet:
		return FSolanaJsonRpcGlobal::DefaultTestnetURL;
		break;
	}
	return FSolanaJsonRpcGlobal::DefaultEmptyURL;
}


template<class R>
FString FSolanaJsonRPCAsyncTask<R>::ToString() const
{
	return GetCallerString(TEXT("FSolanaJsonRPCAsyncTask"));
}


template<class R>
const FString& FSolanaJsonRPCAsyncTask<R>::GetRpcUrl() const
{
	const UFoundationSettings* SettingsPtr = GetDefault<UFoundationSettings>();
	if (SettingsPtr != nullptr) {

		const UFoundationSettings& Settings = (*SettingsPtr);
		const FString& Url = Settings.GetNetworkURL_Cpp();
		if (Url.IsEmpty())
			return FSolanaJsonRpcGlobal::GetDefaultNetworkURL(Settings.GetNetwork_Cpp());
		return Url;
	}
	return FJsonRPCAsyncTask::GetRpcUrl();
}