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

Author: Jon Sawler
Contributers: Daniele Calanna, Riccardo Torrisi, Federico Arona
*/
#pragma once

#include "Types.generated.h"

// |  ENT  | CS | ENT+CS |  MS  |
// +-------+----+--------+------+
// |  128  |  4 |   132  |  12  |
// |  160  |  5 |   165  |  15  |
// |  192  |  6 |   198  |  18  |
// |  224  |  7 |   231  |  21  |
// |  256  |  8 |   264  |  24  |

constexpr int PublicKeySize = 32;
constexpr int PrivateKeySize = 64;

constexpr int Base58PubKeySize = 44;
constexpr int Base58PrKeySize = 88;

constexpr int AccountDataSize = 165;

const FString TokenProgramId = "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA";

USTRUCT()
struct FBalanceContextJson
{
	GENERATED_BODY()

	UPROPERTY()	
	double slot;
	
	FBalanceContextJson() { slot = 0.0; }
	static FBalanceContextJson Empty;
};

USTRUCT()
struct FBalanceResultJson
{
	GENERATED_BODY()

	UPROPERTY()	
	FBalanceContextJson context;
	
	UPROPERTY()	
	double value;

	FBalanceResultJson() {
		context = FBalanceContextJson::Empty;
		value = 0.0;
	}
	static FBalanceResultJson Empty;
};

USTRUCT()
struct FAccountInfoJson
{
	GENERATED_BODY()

	UPROPERTY()	
	TArray<FString> data;
	
	UPROPERTY()	
	bool executable;
	
	UPROPERTY()	
	double lamports;
	
	UPROPERTY()	
	FString owner;
	
	UPROPERTY()	
	int32 rentEpoch;

	FAccountInfoJson() {
		data.Reset();
		executable = false;
		lamports = 0.0;
		owner.Empty();
		rentEpoch = 0;
	}

	static FAccountInfoJson Empty;
};

USTRUCT()
struct FProgramAccountJson
{
	GENERATED_BODY()

	UPROPERTY()	
	FString data;
	
	UPROPERTY()	
	bool executable;
	
	UPROPERTY()	
	double lamports;
	
	UPROPERTY()	
	FString owner;
	
	UPROPERTY()	
	double rentEpoch;

	FProgramAccountJson() {
		data.Empty();
		executable = false;
		lamports = 0.0;
		owner.Empty();
		rentEpoch = 0;
	}

	static FProgramAccountJson Empty;
};


USTRUCT()
struct FTokenUIBalanceJson
{
	GENERATED_BODY()

	UPROPERTY()	
	FString amount;
	
	UPROPERTY()	
	double decimals;
	
	UPROPERTY()	
	double uiAmount;
	
	UPROPERTY()	
	FString uiAmountString;

	FTokenUIBalanceJson()
	{
		amount.Empty();
		decimals = 0.0;
		uiAmount = 0.0;
		uiAmountString.Empty();
	}

	static FTokenUIBalanceJson Empty;
};

USTRUCT()
struct FTokenInfoJson
{
	GENERATED_BODY()

	UPROPERTY()	
	FTokenUIBalanceJson tokenAmount;
	
	UPROPERTY()	
	FString delegate;
	
	UPROPERTY()	
	FTokenUIBalanceJson delegatedAmount;
	
	UPROPERTY()	
	FString state;
	
	UPROPERTY()	
	bool isNative;
	
	UPROPERTY()	
	FString mint;
	
	UPROPERTY()	
	FString owner;

	FTokenInfoJson() {
		tokenAmount = FTokenUIBalanceJson::Empty;
		delegate.Empty();
		delegatedAmount = FTokenUIBalanceJson::Empty;
		state.Empty();
		isNative = false;
		mint.Empty();
		owner.Empty();
	}

	static FTokenInfoJson Empty;
};

USTRUCT()
struct FParsedTokenDataJson
{
	GENERATED_BODY()

	UPROPERTY()	
	FString accountType;
	
	UPROPERTY()	
	FTokenInfoJson info;
	
	UPROPERTY()	
	double space;

	FParsedTokenDataJson()
	{
		accountType.Empty();
		info = FTokenInfoJson::Empty;
		space = 0.0;
	}

	static FParsedTokenDataJson Empty;
};

USTRUCT()
struct FTokenDataJson
{
	GENERATED_BODY()

	UPROPERTY()	
	FString program;
	
	UPROPERTY()	
	FParsedTokenDataJson parsed;
	
	UPROPERTY()	
	double space;

	FTokenDataJson() {
		program.Empty();
		parsed = FParsedTokenDataJson::Empty;
		space = 0.0;
	}

	static FTokenDataJson Empty;
};

USTRUCT()
struct FTokenAccountDataJson
{
	GENERATED_BODY()

	UPROPERTY()	
	FTokenDataJson data;
	
	UPROPERTY()	
	bool executable;
	
	UPROPERTY()
	double lamports;
	
	UPROPERTY()	
	FString owner;
	
	UPROPERTY()	
	double rentEpoch;

	FTokenAccountDataJson()
	{

	}

	static FTokenAccountDataJson Empty;
};

USTRUCT()
struct FTokenBalanceDataJson
{
	GENERATED_BODY()

	UPROPERTY()	
	FTokenAccountDataJson account;
	
	UPROPERTY()	
	FString pubkey;

	FTokenBalanceDataJson() {
		account = FTokenAccountDataJson::Empty;
		pubkey.Reset();
	}
	static FTokenBalanceDataJson Empty;
};

USTRUCT()
struct FTokenAccountArrayJson
{
	GENERATED_BODY()

	UPROPERTY()	
	TArray<FTokenBalanceDataJson> value;

	FTokenAccountArrayJson() {
		value.Reset();
	}
	static FTokenAccountArrayJson Empty;
};