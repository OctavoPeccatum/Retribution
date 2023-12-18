// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterfaces/Button/UIButton.h"
#include "Components/Button.h"

void UUIButton::NativeConstruct()
{
	Super::NativeConstruct();

	MainButton->OnClicked.AddUniqueDynamic(this, &UUIButton::OnButtonClicked);
}

void UUIButton::OnButtonClicked()
{
	OnClickedDelegate.Broadcast(this);
}
