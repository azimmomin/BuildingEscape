// Fill out your copyright notice in the Description page of Project Settings.
#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Math/Color.h"

#define OUT

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (!PhysicsHandle)
    {
        UE_LOG(LogTemp, Error, TEXT("%s does not have a UPhysicsHandleComponent attached."), *GetOwner()->GetName());
    }

    Input = GetOwner()->FindComponentByClass<UInputComponent>();
    if (Input)
    {
        Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s does not have a UInputComponent attached."), *GetOwner()->GetName());
    }
}

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
    FVector LineTraceEnd;
    
    playerController->GetPlayerViewPoint(OUT PlayerViewPointPosition, OUT PlayerViewPointRotation);
    LineTraceEnd = PlayerViewPointPosition + (PlayerViewPointRotation.Vector() * Reach);

    DrawDebugLine(
        GetWorld(),
        PlayerViewPointPosition,
        LineTraceEnd,
        FColor::Red,
        false,
        0.0f,
        0,
        5.0f
    );
    
    FHitResult Hit;
    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
    GetWorld()->LineTraceSingleByObjectType(
        OUT Hit,
        PlayerViewPointPosition,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParams
    );

    const AActor* ActorHit = Hit.GetActor();
    if (ActorHit)
    {
      UE_LOG(LogTemp, Warning, TEXT("Trace hit: %s"), *(ActorHit->GetName()));
    }
}

void UGrabber::Grab()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab."));
}

