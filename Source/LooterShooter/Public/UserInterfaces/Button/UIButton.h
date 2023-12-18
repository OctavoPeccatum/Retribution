// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBUIOnClickedSignature, class UUIButton*, Button);
/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API UUIButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Bind a function with the signature "void OnClicked(UBUIUWButton* Button);
	FBUIOnClickedSignature OnClickedDelegate;

protected:
	virtual void NativeConstruct() override;

	// In the Blueprint subclass, make sure you create aButton called "MainButton"
	UPROPERTY(meta = (BindWidget))
	class UButton* MainButton;

	UFUNCTION()
	void OnButtonClicked();
};
