// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTVTerminal.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "SecuritySystemLog.h"

ACCTVTerminal::ACCTVTerminal()
{
	PrimaryActorTick.bCanEverTick = false;

	//so actor can be moved in the editor
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetMobility(EComponentMobility::Static);
	SetRootComponent(Root);

	//add a quad mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	UStaticMesh* Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'")).Object;
	MeshComponent->SetStaticMesh(Mesh);
	MeshComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	MeshComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 1.0f));
	MeshComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/SecurityCCTVSystem/M_Screen.M_Screen"));
	if (MaterialFinder.Succeeded())
	{
		RenderMaterial = MaterialFinder.Object;
	}

	// set default values
	InteractRadius = 500.f;
}

void ACCTVTerminal::BeginPlay()
{
	Super::BeginPlay();

	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), this->StaticClass());
	if (FoundActor && FoundActor != this)
	{
		TObjectPtr<ACCTVTerminal> OtherCCTVTerminal = Cast<ACCTVTerminal>(FoundActor);
		TextureRenderTarget = OtherCCTVTerminal->TextureRenderTarget;
	}
	else
		TextureRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, 1024, 1024, ETextureRenderTargetFormat::RTF_RGBA32f, FLinearColor::Black, false);

	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(RenderMaterial, this);
	MeshComponent->SetMaterial(0, DynamicMaterialInstance);
	DynamicMaterialInstance->SetTextureParameterValue(FName("ScreenTexture"), TextureRenderTarget);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASecurityCamera::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ASecurityCamera* SecurityCamera = Cast<ASecurityCamera>(Actor);

		if (!SecurityCamera->DetectorComponent)
			UE_LOG(LogSecuritySystem, Error, TEXT("%s has NO detector component"), *SecurityCamera->GetActorLabel());
		
		SecurityCameras.AddUnique(SecurityCamera);
	}

	SecurityCameras[CurrentCameraIndex]->SceneCaptureComponent->TextureTarget = TextureRenderTarget;

	if (!TerminalWidgetClass)
	{
		UE_LOG(LogSecuritySystem, Error, TEXT("TerminalWidgetClass is NULL"));
		return;
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	TerminalWidget = CreateWidget<UUserWidget>(PlayerController, TerminalWidgetClass);	
}


const bool ACCTVTerminal::IsInUse()
{
	return InUse;
}

void ACCTVTerminal::TurnOn()
{
	if (InUse)
		return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
	{
		UE_LOG(LogSecuritySystem, Error, TEXT("PlayerController is NULL"));
		return;
	}

	//check whether player is inside terminals interact radius
	FVector PlayerLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	FVector PlayerToTerminalVector = this->GetActorLocation() - PlayerLocation;
	double SquaredDistance = PlayerToTerminalVector.SizeSquared();
	if (SquaredDistance >= FMath::Square(InteractRadius))
	{
		UE_LOG(LogSecuritySystem, Log, TEXT("CCTVTerminal too far away from the player"));
		return;
	}
	//dot product check	
	PlayerToTerminalVector.Normalize();
	FVector PlayerForwardVector = PlayerController->PlayerCameraManager->GetActorForwardVector();
	if (FVector::DotProduct(PlayerForwardVector, PlayerToTerminalVector) <= 0.8f)
	{
		UE_LOG(LogSecuritySystem, Log, TEXT("CCTVTerminal not in front of the player"));
		return;
	}

	InUse = true;
	UE_LOG(LogSecuritySystem, Log, TEXT("%s: InUse"), *GetActorLabel());
	
	if (!TerminalWidget)
	{
		UE_LOG(LogSecuritySystem, Error, TEXT("TerminalWidget is NULL"));
		return;
	}

	PlayerController->FlushPressedKeys();
	PlayerController->bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(TerminalWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	PlayerController->SetInputMode(InputMode);
	TerminalWidget->AddToViewport();
	UE_LOG(LogSecuritySystem, Log, TEXT("AddToViewport called, IsInViewport: %s"), TerminalWidget->IsInViewport() ? TEXT("true") : TEXT("false"));
}

void ACCTVTerminal::TurnOff()
{
	if (!TerminalWidget)
	{
		UE_LOG(LogSecuritySystem, Error, TEXT("TerminalWidget is NULLPTR"));
		return;
	}

	if (!InUse)
	{
		UE_LOG(LogSecuritySystem, Error, TEXT("Terminal NOT InUse"));
		return;
	}

	InUse = false;
	UE_LOG(LogSecuritySystem, Log, TEXT("Terminal turned off"));

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	TerminalWidget->RemoveFromParent();
	FInputModeGameOnly GameMode;
	PlayerController->SetInputMode(GameMode);
	PlayerController->bShowMouseCursor = false;
}

void ACCTVTerminal::SwitchToNextCamera()
{
	SecurityCameras[CurrentCameraIndex]->SceneCaptureComponent->TextureTarget = nullptr;

	if (++CurrentCameraIndex >= SecurityCameras.Num())
		CurrentCameraIndex = 0;

	SecurityCameras[CurrentCameraIndex]->SceneCaptureComponent->TextureTarget = TextureRenderTarget;

	UE_LOG(LogSecuritySystem, Log, TEXT("CCTVTerminal: SwitchToNextCamera"));
}

void ACCTVTerminal::SwitchToPreviousCamera()
{
	SecurityCameras[CurrentCameraIndex]->SceneCaptureComponent->TextureTarget = nullptr;

	if (--CurrentCameraIndex <= -1)
		CurrentCameraIndex = SecurityCameras.Num() - 1;

	SecurityCameras[CurrentCameraIndex]->SceneCaptureComponent->TextureTarget = TextureRenderTarget;

	UE_LOG(LogSecuritySystem, Log, TEXT("CCTVTerminal: SwitchToPreviousCamera"));
}

void ACCTVTerminal::TriggerResponders()
{
	UDetectorComponent* DetectorComponent = SecurityCameras[CurrentCameraIndex]->DetectorComponent;

	if (!DetectorComponent)
	{
		UE_LOG(LogSecuritySystem, Error, TEXT("ACCTVTerminal: DetectorComponent is NULL"));
		return;
	}

	UE_LOG(LogSecuritySystem, Log, TEXT("CCTVTerminal: TriggerResponders"));

	switch (DetectorComponent->GetCurrentState())
	{
	case ESecurityState::Neutral:
		DetectorComponent->CheckCanStateBeChanged(ESecurityState::Alarm);
		break;
	case ESecurityState::Alarm:
		DetectorComponent->CheckCanStateBeChanged(ESecurityState::Neutral);
		break;
	}
}
