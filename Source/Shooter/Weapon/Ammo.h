// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Ammo.generated.h"


USTRUCT()
struct FAmmo
{
	GENERATED_BODY()
	FAmmo(): Count(int()) {}
	FAmmo(int X): Count(X) {}
	
	/*FAmmo operator=(const FAmmo& Other) {return  FAmmo(Other.Count); }*/
	FAmmo& operator=(const int& Other) { Count = Other; return *this; }
	
	bool operator==(const FAmmo& Other) const { return Count == Other.Count; }
	bool operator==(const int& Other) const { return Count == Other; }
	bool operator!=(const FAmmo& Other) const { return !(*this == Other); }
	bool operator!=(const int& Other) const { return Count != Other; }
	
	bool operator<=(const FAmmo& Other) const { return Count <= Other.Count; }
	bool operator<=(const int& Other) const { return Count <= Other; }
	bool operator>=(const FAmmo& Other) const { return Count >= Other.Count; }
	bool operator>=(const int& Other) const { return Count >= Other; }
	
	bool operator<(const FAmmo& Other) const { return Count < Other.Count; }
	bool operator<(const int& Other) const { return Count < Other; }
	bool operator>(const FAmmo& Other) const { return Count > Other.Count; }
	bool operator>(const int& Other) const { return Count > Other; }

	FAmmo& operator+=(const FAmmo& Other) { Count += Other.Count; OnUpdate(); return *this; }
	FAmmo& operator+=(const int& Other) { Count += Other; OnUpdate(); return *this; }
	FAmmo& operator-=(const FAmmo& Other) { Count -= Other.Count;OnUpdate(); return *this; }
	FAmmo& operator-=(const int& Other) { Count -= Other; OnUpdate(); return *this; }

	FAmmo& operator++(){Count++; OnUpdate(); return *this;}
	FAmmo operator++(int){const FAmmo Temp = *this; ++*this; return Temp;}
	FAmmo& operator--() { Count-- ; OnUpdate(); return *this;}
	FAmmo operator--(int) {const FAmmo Temp = *this; --*this; return Temp;}

	FAmmo& operator+(FAmmo &Other) { Count += Other.Count; OnUpdate(); return *this; }
	FAmmo& operator+(int &Other) { Count += Other; OnUpdate(); return *this; }
	FAmmo& operator-(FAmmo &Other) { Count -= Other.Count; OnUpdate(); return *this; }
	FAmmo& operator-(int &Other) { Count -= Other; OnUpdate(); return *this; }

	operator int() const {return  Count; }

	TFunction<void(const int& Value)> UpdateWidget;

	void OnUpdate() const { if (UpdateWidget) UpdateWidget(Count); }
private:
	UPROPERTY()
		int Count;
};
