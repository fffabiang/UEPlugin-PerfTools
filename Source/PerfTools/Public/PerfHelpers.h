// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameMode.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameUserSettings.h"

#include "PerfHelpers.generated.h"


UENUM(BlueprintType)
enum class ESettingsPreset : uint8 {
	Custom	UMETA(DisplayName = "Custom"),
	Low		UMETA(DisplayName = "Low"),
	Medium	UMETA(DisplayName = "Medium"),
	High	UMETA(DisplayName = "High"),
	Epic	UMETA(DisplayName = "Epic"),
	Unavailable UMETA(DisplayName = "Unavailable"),
};


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

	UFUNCTION(BlueprintPure)
	static ESettingsPreset GetSettingsPreset();

	UFUNCTION(BlueprintCallable)
	static void SetSettingsPreset(ESettingsPreset value);

	UFUNCTION(BlueprintPure)
	static float GetLastGPUBenchmark();

	UFUNCTION(BlueprintPure)
	static float GetLastCPUBenchmark();

	UFUNCTION(BlueprintCallable)
	static void RunBenchmarkAndUpdate(UWorld* World);


private:
	static bool bStatsOn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
