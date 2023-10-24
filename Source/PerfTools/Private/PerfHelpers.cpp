// Fill out your copyright notice in the Description page of Project Settings.


#include "PerfHelpers.h"
#include "Kismet/GameplayStatics.h"


bool APerfHelpers::bStatsOn = false;

// Sets default values
APerfHelpers::APerfHelpers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APerfHelpers::ExecuteStatCommands(UWorld* World)
{

	if (bStatsOn) 
	{
		UE_LOG(LogTemp, Log, TEXT("Stats already displayed."));
		return;
	}


	if (World)
	{ 
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
		if (!PlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("No player controller available."));
			return;
		}
		 
		if (!IsEditorOrDevBuild())
		{
			UE_LOG(LogTemp, Warning, TEXT("Not executing stat commands because not in EDITOR nor DEV BUILD."));
			return;
		}


		UE_LOG(LogTemp, Log, TEXT("Executing performance stat commands."));

		// Execute 'stat fps' command
		TArray<FString> ConsoleCommands = {
			"stat fps",
			"stat Unit"
		};

		// Execute each console command from the array
		for (const FString& Command : ConsoleCommands)
		{
			PlayerController->ConsoleCommand(*Command);
		}

		bStatsOn = true;

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid world reference to ExecuteStatCommands"));
	}
}

void APerfHelpers::ClearStatCommands(UWorld* World)
{
	if (bStatsOn)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
		if (!PlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("No player controller available."));
			return;
		}
		else
		{
			FString Command = "stat none";
			PlayerController->ConsoleCommand(*Command);
			bStatsOn = false;
			UE_LOG(LogTemp, Log, TEXT("Cleared stat commands"));
		}
	}
}

bool APerfHelpers::IsEditorOrDevBuild()
{
	#if WITH_EDITOR
		return true;
	#else
		return !UKismetSystemLibrary::IsPackagedForDistribution();
	#endif
}
 

// Called when the game starts or when spawned
void APerfHelpers::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APerfHelpers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ESettingsPreset APerfHelpers::GetSettingsPreset()
{
	
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogTemp, Error, TEXT("No user settings available."));
		return ESettingsPreset::Unavailable;
	}

	int32 level = Settings->GetOverallScalabilityLevel();
	UE_LOG(LogTemp, Log, TEXT("GetOverallScalabilityLevel (%d)"), (int) level);
	switch (level)
	{
		case -1:
			return ESettingsPreset::Custom;
		case 0:
			return ESettingsPreset::Low;
		case 1:
			return ESettingsPreset::Medium;
		case 2:
			return ESettingsPreset::High;
		case 3:
			return ESettingsPreset::Epic;
		default:
			return ESettingsPreset::Unavailable;
	}
}

void APerfHelpers::SetSettingsPreset(ESettingsPreset value)
{

	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogTemp, Error, TEXT("No user settings available."));
		return ;
	}

	bool updateLevel = true;
	int32 newLevel = 0;

	switch (value)
	{
		case ESettingsPreset::Custom:
			newLevel = -1;
			break;
		case ESettingsPreset::Low:
			newLevel = 0;
			break;
		case ESettingsPreset::Medium:
			newLevel = 1;
			break;
		case ESettingsPreset::High:
			newLevel = 2;
			break;
		case ESettingsPreset::Epic:
			newLevel = 3;
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Invalid value passed to SetSettingsPreset"));
			updateLevel = false;
			break;
	}

	if (updateLevel)
	{
		Settings->SetOverallScalabilityLevel(newLevel);
		Settings->ApplySettings(true);
		UE_LOG(LogTemp, Log, TEXT("SetOverallScalabilityLevel to %d"), (int)newLevel);
	}

}

float APerfHelpers::GetLastGPUBenchmark()
{

	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogTemp, Error, TEXT("No user settings available."));
		return 0;
	}

	return Settings->ScalabilityQuality.GPUBenchmarkResults;
}

float APerfHelpers::GetLastCPUBenchmark()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogTemp, Error, TEXT("No user settings available."));
		return 0;
	}

	return Settings->ScalabilityQuality.CPUBenchmarkResults;
}

void APerfHelpers::RunBenchmarkAndUpdate(UWorld* World)
{

	UE_LOG(LogTemp, Log, TEXT("RunBenchmarkAndUpdate Started"));

	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogTemp, Error, TEXT("No user settings available."));
		return;
	}
	
	if (World)
	{
		float LastWorldDeltaMS = UGameplayStatics::GetWorldDeltaSeconds(World) * 1000;
		int ResolutionX = Settings->GetScreenResolution().X;
		int ResolutionY = Settings->GetScreenResolution().Y;

		UE_LOG(LogTemp, Log, TEXT("LastDelta (%lf) Resolution X(%d) Y(%d)"), LastWorldDeltaMS, ResolutionX, ResolutionY);

		Settings->RunHardwareBenchmark();
		Settings->ApplyHardwareBenchmarkResults();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No valid GameMode object."));
	}

	UE_LOG(LogTemp, Log, TEXT("RunBenchmarkAndUpdate Completed"));

}


