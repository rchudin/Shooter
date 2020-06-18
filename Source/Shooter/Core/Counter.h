// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Counter.generated.h"


USTRUCT()
struct FCounter
{
	GENERATED_BODY()
	FCounter(): Count(int()) {}
	FCounter(const int X): Count(X) {}
	
	/*FAmmo operator=(const FAmmo& Other) {return  FAmmo(Other.Count); }*/
	FCounter& operator=(const int& Other) { Count = Other; return *this; }
	
	bool operator==(const FCounter& Other) const { return Count == Other.Count; }
	bool operator==(const int& Other) const { return Count == Other; }
	bool operator!=(const FCounter& Other) const { return Count != Other.Count; }
	bool operator!=(const int& Other) const { return Count != Other; }
	
	bool operator<=(const FCounter& Other) const { return Count <= Other.Count; }
	bool operator<=(const int& Other) const { return Count <= Other; }
	bool operator>=(const FCounter& Other) const { return Count >= Other.Count; }
	bool operator>=(const int& Other) const { return Count >= Other; }
	
	bool operator<(const FCounter& Other) const { return Count < Other.Count; }
	bool operator<(const int& Other) const { return Count < Other; }
	bool operator>(const FCounter& Other) const { return Count > Other.Count; }
	bool operator>(const int& Other) const { return Count > Other; }

	FCounter& operator+=(const FCounter& Other) { Count += Other.Count; OnUpdate(); return *this; }
	FCounter& operator+=(const int& Other) { Count += Other; OnUpdate(); return *this; }
	FCounter& operator-=(const FCounter& Other) { Count -= Other.Count;OnUpdate(); return *this; }
	FCounter& operator-=(const int& Other) { Count -= Other; OnUpdate(); return *this; }

	FCounter& operator++(){Count++; OnUpdate(); return *this;}
	FCounter operator++(int){const FCounter Temp = *this; ++*this; return Temp;}
	FCounter& operator--() { Count-- ; OnUpdate(); return *this;}
	FCounter operator--(int) {const FCounter Temp = *this; --*this; return Temp;}

	FCounter& operator+(FCounter &Other) { Count += Other.Count; OnUpdate(); return *this; }
	FCounter& operator+(int &Other) { Count += Other; OnUpdate(); return *this; }
	FCounter& operator-(FCounter &Other) { Count -= Other.Count; OnUpdate(); return *this; }
	FCounter& operator-(int &Other) { Count -= Other; OnUpdate(); return *this; }

	operator int() const {return  Count; }

	TFunction<void(const int& Value)> UpdateWidget;

	void OnUpdate() const { if (UpdateWidget) UpdateWidget(Count); }
private:
	UPROPERTY()
		int Count;
};
