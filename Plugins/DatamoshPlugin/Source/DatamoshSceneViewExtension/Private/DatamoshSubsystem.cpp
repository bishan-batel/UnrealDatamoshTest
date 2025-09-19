// Fill out your copyright notice in the Description page of Project Settings.


#include "DatamoshSubsystem.h"

#include "FDatamoshSceneViewExtension.h"
#include "ParticleHelper.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

void UDatamoshSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	SceneExtension = FSceneViewExtensions::NewExtension<FDatamoshSceneViewExtension>();

	UE_LOG(LogTemp, Log, TEXT("Created DatamoshSceneViewExtension"));
}

void UDatamoshSubsystem::Deinitialize()
{
	{
		SceneExtension->IsActiveThisFrameFunctions.Empty();

		FSceneViewExtensionIsActiveFunctor IsActiveFunctor;

		IsActiveFunctor.IsActiveFunction = [](
			const ISceneViewExtension*,
			const FSceneViewExtensionContext& Context
		) -> TOptional<bool>
			{
				return TOptional{false};
			};

		SceneExtension->IsActiveThisFrameFunctions.Add(IsActiveFunctor);
	}

	SceneExtension.Reset();
	SceneExtension = nullptr;
}
