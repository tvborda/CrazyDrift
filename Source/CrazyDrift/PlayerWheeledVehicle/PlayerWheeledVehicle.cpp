// Fill out your copyright notice in the Description page of Project Settings.

#include "CrazyDrift.h"
#include "PlayerWheeledVehicle.h"
#include "Vehicles/WheeledVehicleMovementComponent.h"
#include "Vehicles/WheeledVehicleMovementComponent4W.h"
#include "Vehicles/VehicleWheel.h"

const TCHAR* APlayerWheeledVehicle::VehicleSkeletalMeshPath = TEXT("SkeletalMesh'/Game/Vehicles/SimpleCar/Models/SimpleCar.SimpleCar'");
const TCHAR* APlayerWheeledVehicle::VehicleAnimClassPath = TEXT("AnimBlueprint'/Game/Animations/SimpleCar_AnimBlueprint.SimpleCar_AnimBlueprint_C'");
const TCHAR* APlayerWheeledVehicle::VehicleWheelFrontClassPath = TEXT("Blueprint'/Game/Blueprints/PlayerWheeledVehicle/BP_VehicleWheel_Front.BP_VehicleWheel_Front_C'");
const TCHAR* APlayerWheeledVehicle::VehicleWheelBackClassPath = TEXT("Blueprint'/Game/Blueprints/PlayerWheeledVehicle/BP_VehicleWheel_Back.BP_VehicleWheel_Back_C'");

APlayerWheeledVehicle::APlayerWheeledVehicle()
{
	PrimaryActorTick.bCanEverTick = true;

	/** Initialization of skeletal mesh */
	InitSkeletalMesh();
	/** Initialization of animation class */
	InitAnimClass();
	/** Initialization of vehicle setup */
	InitVehicleSetup();
	/** Initialization of mechanical setup */
	//InitMechanicalSetup();
	/** Initialization of steering setup */
	//InitSteeringSetup();

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a spring arm component for our chase camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	SpringArm->TargetArmLength = 7500.f;
	SpringArm->RelativeRotation = FRotator(-30.f, 45.f, 0.f);
	SpringArm->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create the chase camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetProjectionMode(ECameraProjectionMode::Perspective);
	Camera->SetFieldOfView(30.0f);
	//Camera->SetOrthoWidth(7500.0f);
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}


void APlayerWheeledVehicle::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerWheeledVehicle::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerWheeledVehicle::MoveRight);
	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &APlayerWheeledVehicle::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &APlayerWheeledVehicle::OnHandbrakeReleased);
}

void APlayerWheeledVehicle::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void APlayerWheeledVehicle::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void APlayerWheeledVehicle::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void APlayerWheeledVehicle::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void APlayerWheeledVehicle::InitSkeletalMesh()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VehicleSkeletalMesh(VehicleSkeletalMeshPath);
	if (VehicleSkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(VehicleSkeletalMesh.Object);
	}
	else
	{
		UE_LOG(LogCrazyDrift, Warning, TEXT("PlayerWheeledVehicle - Failed to initialize skeletal mesh: %s"), VehicleSkeletalMeshPath);
	}
}

void APlayerWheeledVehicle::InitAnimClass()
{
	static ConstructorHelpers::FClassFinder<UObject> VehicleAnimClass(VehicleAnimClassPath);
	if (VehicleAnimClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(VehicleAnimClass.Class);
	}
	else
	{
		UE_LOG(LogCrazyDrift, Warning, TEXT("PlayerWheeledVehicle - Failed to initialize animation: %s"), VehicleAnimClassPath);
	}
}

void APlayerWheeledVehicle::InitVehicleSetup()
{
	UWheeledVehicleMovementComponent4W* VehicleMovement4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
	VehicleMovement4W->bConstrainToPlane = true;
	VehicleMovement4W->bSnapToPlaneAtStart = true;

	check(VehicleMovement4W->WheelSetups.Num() == 4);

	// Front Wheels
	static ConstructorHelpers::FClassFinder<UVehicleWheel> VehicleWheelFrontClass(VehicleWheelFrontClassPath);
	if (VehicleWheelFrontClass.Succeeded())
	{
		VehicleMovement4W->WheelSetups[0].WheelClass = VehicleWheelFrontClass.Class;
		VehicleMovement4W->WheelSetups[1].WheelClass = VehicleWheelFrontClass.Class;
	}
	else
	{
		UE_LOG(LogCrazyDrift, Warning, TEXT("PlayerWheeledVehicle - Failed to initialize front wheels: %s"), VehicleWheelFrontClassPath);
	}
	VehicleMovement4W->WheelSetups[0].BoneName = FName("Wheel_FL");
	VehicleMovement4W->WheelSetups[0].AdditionalOffset = FVector(0.f, 0.f, 0.f);
	VehicleMovement4W->WheelSetups[1].BoneName = FName("Wheel_FR");
	VehicleMovement4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 0.f, 0.f);

	// Back Wheels
	static ConstructorHelpers::FClassFinder<UVehicleWheel> VehicleWheelBackClass(VehicleWheelBackClassPath);
	if (VehicleWheelBackClass.Succeeded())
	{
		VehicleMovement4W->WheelSetups[2].WheelClass = VehicleWheelBackClass.Class;
		VehicleMovement4W->WheelSetups[3].WheelClass = VehicleWheelBackClass.Class;
	}
	else
	{
		UE_LOG(LogCrazyDrift, Warning, TEXT("PlayerWheeledVehicle - Failed to initialize back wheels: %s"), VehicleWheelFrontClassPath);
	}
	VehicleMovement4W->WheelSetups[2].BoneName = FName("Wheel_BL");
	VehicleMovement4W->WheelSetups[2].AdditionalOffset = FVector(0.f, 0.f, 0.f);
	VehicleMovement4W->WheelSetups[3].BoneName = FName("Wheel_BR");
	VehicleMovement4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 0.f, 0.f);

	//VehicleMovement4W->InertiaTensorScale = FVector(1.0f, 1.333f, 1.2f);
	//VehicleMovement4W->MinNormalizedTireLoad = 0.0f;
	//VehicleMovement4W->MinNormalizedTireLoadFiltered = 0.2f;
	//VehicleMovement4W->MaxNormalizedTireLoad = 2.0f;
	//VehicleMovement4W->MaxNormalizedTireLoadFiltered = 2.0f;
}
