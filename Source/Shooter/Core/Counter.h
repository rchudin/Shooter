// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Counter.generated.h"


USTRUCT()
struct FCounterInt
{
	GENERATED_BODY()
	FCounterInt(): Count(int()) {}
	FCounterInt(const int X): Count(X) {}
	
	/*FAmmo operator=(const FAmmo& Other) {return  FAmmo(Other.Count); }*/
	FCounterInt& operator=(const int& Other) { Count = Other; return *this; }
	
	bool operator==(const FCounterInt& Other) const { return Count == Other.Count; }
	bool operator==(const int& Other) const { return Count == Other; }
	bool operator!=(const FCounterInt& Other) const { return Count != Other.Count; }
	bool operator!=(const int& Other) const { return Count != Other; }
	
	bool operator<=(const FCounterInt& Other) const { return Count <= Other.Count; }
	bool operator<=(const int& Other) const { return Count <= Other; }
	bool operator>=(const FCounterInt& Other) const { return Count >= Other.Count; }
	bool operator>=(const int& Other) const { return Count >= Other; }
	
	bool operator<(const FCounterInt& Other) const { return Count < Other.Count; }
	bool operator<(const int& Other) const { return Count < Other; }
	bool operator>(const FCounterInt& Other) const { return Count > Other.Count; }
	bool operator>(const int& Other) const { return Count > Other; }

	FCounterInt& operator+=(const FCounterInt& Other) { Count += Other.Count; OnUpdate(); return *this; }
	FCounterInt& operator+=(const int& Other) { Count += Other; OnUpdate(); return *this; }
	FCounterInt& operator-=(const FCounterInt& Other) { Count -= Other.Count;OnUpdate(); return *this; }
	FCounterInt& operator-=(const int& Other) { Count -= Other; OnUpdate(); return *this; }

	FCounterInt& operator++(){Count++; OnUpdate(); return *this;}
	FCounterInt operator++(int){const FCounterInt Temp = *this; ++*this; return Temp;}
	FCounterInt& operator--() { Count-- ; OnUpdate(); return *this;}
	FCounterInt operator--(int) {const FCounterInt Temp = *this; --*this; return Temp;}

	FCounterInt& operator+(const FCounterInt &Other) { Count += Other.Count; OnUpdate(); return *this; }
	FCounterInt& operator+(const int &Other) { Count += Other; OnUpdate(); return *this; }
	FCounterInt& operator-(const FCounterInt &Other) { Count -= Other.Count; OnUpdate(); return *this; }
	FCounterInt& operator-(const int &Other) { Count -= Other; OnUpdate(); return *this; }

	operator int() const {return  Count; }

	TFunction<void(const int& Value)> UpdateWidget;

	void OnUpdate() const { if (UpdateWidget) UpdateWidget(Count); }
private:
	UPROPERTY()
		int Count;
};


USTRUCT()
struct FCounterFloat
{
	GENERATED_BODY()
	FCounterFloat(): Count(float()) {}
	FCounterFloat(const float X): Count(X) {}
	
	/*FAmmo operator=(const FAmmo& Other) {return  FAmmo(Other.Count); }*/
	FCounterFloat& operator=(const float& Other) { Count = Other; return *this; }
	
	bool operator==(const FCounterFloat& Other) const { return Count == Other.Count; }
	bool operator==(const float& Other) const { return Count == Other; }
	bool operator!=(const FCounterFloat& Other) const { return Count != Other.Count; }
	bool operator!=(const float& Other) const { return Count != Other; }
	
	bool operator<=(const FCounterFloat& Other) const { return Count <= Other.Count; }
	bool operator<=(const float& Other) const { return Count <= Other; }
	bool operator>=(const FCounterFloat& Other) const { return Count >= Other.Count; }
	bool operator>=(const float& Other) const { return Count >= Other; }
	
	bool operator<(const FCounterFloat& Other) const { return Count < Other.Count; }
	bool operator<(const float& Other) const { return Count < Other; }
	bool operator>(const FCounterFloat& Other) const { return Count > Other.Count; }
	bool operator>(const float& Other) const { return Count > Other; }

	FCounterFloat& operator+=(const FCounterFloat& Other) { Count += Other.Count; OnUpdate(); return *this; }
	FCounterFloat& operator+=(const float& Other) { Count += Other; OnUpdate(); return *this; }
	FCounterFloat& operator-=(const FCounterFloat& Other) { Count -= Other.Count;OnUpdate(); return *this; }
	FCounterFloat& operator-=(const float& Other) { Count -= Other; OnUpdate(); return *this; }

	FCounterFloat& operator++(){Count++; OnUpdate(); return *this;}
	FCounterFloat operator++(int){const FCounterFloat Temp = *this; ++*this; return Temp;}
	FCounterFloat& operator--() { Count-- ; OnUpdate(); return *this;}
	FCounterFloat operator--(int) {const FCounterFloat Temp = *this; --*this; return Temp;}

	FCounterFloat& operator+(const FCounterFloat &Other) { Count += Other.Count; OnUpdate(); return *this; }
	FCounterFloat& operator+(const float &Other) { Count += Other; OnUpdate(); return *this; }
	FCounterFloat& operator-(const FCounterFloat &Other) { Count -= Other.Count; OnUpdate(); return *this; }
	FCounterFloat& operator-(const float &Other) { Count -= Other; OnUpdate(); return *this; }

	operator float() const {return  Count; }

	TFunction<void(const float& Value)> UpdateWidget;

	void OnUpdate() const { if (UpdateWidget) UpdateWidget(Count); }
private:
	UPROPERTY()
		float Count;
};
