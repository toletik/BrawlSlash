// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::RestartLevelWithInitialValues()
{
	allDirectors.Empty();
	numberOfPlayerShieldPoints = 0;
	hasRestartLevel = false;
}