// Fill out your copyright notice in the Description page of Project Settings.


#include "PrintScreen.h"

void UPrintScreen::PrintScreen(const FString& String, const FColor Color, const float Time)
{
	GEngine->AddOnScreenDebugMessage(-1, Time, Color, FString::Printf(TEXT("%s"), *String));
}
