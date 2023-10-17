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

