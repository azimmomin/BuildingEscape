// Fill out your copyright notice in the Description page of Project Settings.
#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Math/Color.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    const APlayerController* playerController = GetWorld()->GetFirstPlayerController();
    if (playerController == NULL)
    {
        UE_LOG(LogTemp, Error, TEXT("Could not find player controller in world. Make sure there is at least one."));
        return;
    }

    FRotator PlayerViewPointRotation;
    FVector PlayerViewPointPosition;
    playerController->GetPlayerViewPoint(OUT PlayerViewPointPosition, OUT PlayerViewPointRotation);

    DrawDebugLine(
        GetWorld(),
        PlayerViewPointPosition,
        PlayerViewPointPosition + (PlayerViewPointRotation.Vector() * Reach),
        FColor::Red,
        false,
        0.0f,
        0,
        5.0f
    );
    
}

