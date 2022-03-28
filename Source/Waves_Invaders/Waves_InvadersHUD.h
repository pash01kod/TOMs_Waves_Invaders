// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Waves_InvadersHUD.generated.h"

UCLASS()
class AWaves_InvadersHUD : public AHUD
{
	GENERATED_BODY()

public:
	AWaves_InvadersHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

