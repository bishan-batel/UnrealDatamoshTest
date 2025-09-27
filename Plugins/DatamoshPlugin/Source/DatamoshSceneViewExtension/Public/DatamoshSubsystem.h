// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "DatamoshSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DATAMOSHPLUGIN_API UDatamoshSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

private:
	TSharedPtr<class FDatamoshSceneViewExtension, ESPMode::ThreadSafe> SceneExtension;
};
