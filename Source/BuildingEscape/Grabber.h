// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGrabber();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    void FindPhysicsHandle();
    void FindPlayerController();
    void SetupInputComponent();
    void Grab();
    void Release();
    FHitResult GetFirstPhysicsBodyInReach() const;
private:
    const float Reach = 100.0f;
    UPhysicsHandleComponent* PhysicsHandle = nullptr;
    const APlayerController* PlayerController = nullptr;
    UInputComponent* Input = nullptr;
};
