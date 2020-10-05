// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface_Highlightable.h"




IInterface_Highlightable::IInterface_Highlightable()
{
}

void IInterface_Highlightable::SetIfNeedToGlow(bool value)
{
	if(matDynamic != nullptr)
		matDynamic->SetScalarParameterValue("needToGlow", value ? 1 : 0);
}

