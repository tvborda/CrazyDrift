// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/WheeledVehicle.h"
#include "PlayerWheeledVehicle.generated.h"

/**
 * 
 */
UCLASS()
class CRAZYDRIFT_API APlayerWheeledVehicle : public AWheeledVehicle
{
	GENERATED_BODY()
	
public:
	static const TCHAR* VehicleSkeletalMeshPath;
	static const TCHAR* VehicleAnimClassPath;
	static const TCHAR* VehicleWheelFrontClassPath;
	static const TCHAR* VehicleWheelBackClassPath;

	APlayerWheeledVehicle();

	/** Returns SpringArm subobject **/
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; };
	/** Returns Camera subobject **/
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; };

protected:
	/** Pawn interface */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/** Handle pressing forwards */
	void MoveForward(float Val);
	/** Handle pressing right */
	void MoveRight(float Val);
	/** Handle handbrake pressed */
	void OnHandbrakePressed();
	/** Handle handbrake released */
	void OnHandbrakeReleased();

private:
	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	void InitSkeletalMesh();
	void InitAnimClass();
	void InitVehicleSetup();
	//void InitMechanicalSetup();
	//void InitSteeringSetup();
};
