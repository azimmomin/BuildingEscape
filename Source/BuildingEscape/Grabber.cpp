// Fill out your copyright notice in the Description page of Project Settings.
#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Math/Color.h"

#define OUT

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

    FindPhysicsHandle();

    FindPlayerController();

    SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (!PhysicsHandle)
    {
        UE_LOG(LogTemp, Error, TEXT("%s does not have a UPhysicsHandleComponent attached."), *GetOwner()->GetName());
    }
}

void UGrabber::FindPlayerController()
{
    PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController == NULL)
    {
        UE_LOG(LogTemp, Error, TEXT("Could not find player controller in world. Make sure there is at least one."));
        return;
    }
}

void UGrabber::SetupInputComponent()
{
    Input = GetOwner()->FindComponentByClass<UInputComponent>();
    if (Input)
    {
        Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s does not have a UInputComponent attached."), *GetOwner()->GetName());
    }
}

void UGrabber::Grab()
{
    FHitResult Hit = GetFirstPhysicsBodyInReach();
    if (Hit.GetActor())
    {
        PhysicsHandle->GrabComponentAtLocation(Hit.GetComponent(), NAME_None, GetPlayerReach());
    }
}

void UGrabber::Release()
{
    if (PhysicsHandle->GrabbedComponent)
    {
        PhysicsHandle->ReleaseComponent();
    }
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (PhysicsHandle->GrabbedComponent)
    {
        PhysicsHandle->SetTargetLocation(GetPlayerReach());
    }
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    FRotator PlayerViewPointRotation;
    FVector PlayerViewPointPosition;
    PlayerController->GetPlayerViewPoint(OUT PlayerViewPointPosition, OUT PlayerViewPointRotation);
    
    FHitResult Hit;
    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
    GetWorld()->LineTraceSingleByObjectType(
        OUT Hit,
        PlayerViewPointPosition,
        GetPlayerReach(PlayerViewPointPosition, PlayerViewPointRotation),
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParams
    );

    return Hit;
}

const FVector UGrabber::GetPlayerReach() const
{
    FRotator PlayerViewPointRotation;
    FVector PlayerViewPointPosition;
    PlayerController->GetPlayerViewPoint(OUT PlayerViewPointPosition, OUT PlayerViewPointRotation);
    return GetPlayerReach(PlayerViewPointPosition, PlayerViewPointRotation);
}

const FVector UGrabber::GetPlayerReach(const FVector& PlayerViewPointPosition, const FRotator& PlayerViewPointRotation) const
{
    return PlayerViewPointPosition + (PlayerViewPointRotation.Vector() * Reach);
}