
#include "FoundationSettings.h"

ESolanaNetwork UFoundationSettings::GetNetwork() const
{
	return GetNetwork_Cpp();
}
ESolanaNetwork UFoundationSettings::GetNetwork_Cpp() const
{
	return Network;
}
FString UFoundationSettings::GetNetworkURL() const
{
	return GetNetworkURL_Cpp();
}

FString UFoundationSettings::EmptyNetworkUrl;
const FString& UFoundationSettings::GetNetworkURL_Cpp() const
{
	const FString* NetworkURLPtr = NetworkURLs.Find(GetNetwork_Cpp());
	return (NetworkURLPtr != nullptr) ? (*NetworkURLPtr) : UFoundationSettings::EmptyNetworkUrl;
}