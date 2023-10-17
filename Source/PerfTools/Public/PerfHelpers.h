// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

#include "PerfHelpers.generated.h"

UCLASS()
class PERFTOOLS_API APerfHelpers : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APerfHelpers();

	UFUNCTION(BlueprintCallable)
	static void ExecuteStatCommands(UWorld* World);

	UFUNCTION(BlueprintCallable)
	static void ClearStatCommands(UWorld* World);

	UFUNCTION(BlueprintPure)
	static bool IsEditorOrDevBuild();


private:
	static bool bStatsOn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
