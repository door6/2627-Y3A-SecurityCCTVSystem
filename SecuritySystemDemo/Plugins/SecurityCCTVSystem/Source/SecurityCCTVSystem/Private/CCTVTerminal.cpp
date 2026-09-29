// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTVTerminal.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GameplayStatics.h"
//#include "Components/EnhancedInputComponent.h"
//#include "InputTriggers.h"
#include "SecuritySystemLog.h"

// Sets default values
ACCTVTerminal::ACCTVTerminal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//so actor can be moved in the editor
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetMobility(EComponentMobility::Static);
	SetRootComponent(Root);

	//add a quad mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	UStaticMesh* Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'")).Object;
	MeshComponent->SetStaticMesh(Mesh);
	MeshComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	//MeshComponent->SetupAttachment(Root);

	MeshComponent->SetRelativeScale3D(FVector(2.0f, 3.0f, 1.0f));
	MeshComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/SecurityCCTVSystem/M_Screen.M_Screen"));
	if (MaterialFinder.Succeeded())
	{
		RenderMaterial = MaterialFinder.Object;
	}
}

// Called when the game starts or when spawned
void ACCTVTerminal::BeginPlay()
{
	Super::BeginPlay();

	//TextureRenderTarget->ConstructTexture2D(this, "RenderTexture", RF_NoFlags);
	TextureRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, 1024, 1024, ETextureRenderTargetFormat::RTF_RGBA32f, FLinearColor::Black, false);

	//DynamicMaterialInstance = UMaterialInstanceDynamic::Create(ParentMaterial, this);
	//DynamicMaterialInstance = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
	//DynamicMaterialInstance->SetTextureParameterValue(FName("ScreenTexture"), TextureRenderTarget);

	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(RenderMaterial, this);
	MeshComponent->SetMaterial(0, DynamicMaterialInstance);
	DynamicMaterialInstance->SetTextureParameterValue(FName("ScreenTexture"), TextureRenderTarget);

	//material->SetTextureParameterValue(TEXT("TextureInput"), RenderTarget2D);
	//MeshComponent->SetMaterial(0, material);

	//RenderMaterial = new UMaterial();
	//UMaterialInstanceDynamic* ExampleMID = UMaterialInstanceDynamic::Create(ExampleMaterial);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASecurityCamera::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ASecurityCamera* SecurityCamera = Cast<ASecurityCamera>(Actor);
		SecurityCamera->SceneCaptureComponent->TextureTarget = TextureRenderTarget;	//can have only one texture target, if there are multiple cctvs it will be displayed only to the last one
		
		SecurityCameras.AddUnique(SecurityCamera);
	}
	//CurrentCamera = SecurityCameras[0];

	TurnOn();

	/*bBlockInput = false;
	EnableInput(GetWorld()->GetFirstPlayerController());
	UInputComponent* InputComponent = this->InputComponent;
	InputComponent->BindAction("InteractKey", IE_Pressed, this, &ACCTVTerminal::TurnOn);*/

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->InputComponent->BindAction("CCTVTerminalInteractKey", IE_Pressed, this, &ACCTVTerminal::TurnOn);

	/*APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		EnableInput(PlayerController);
	}
	if (InputComponent)
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
		EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Triggered, this, &ACCTVTerminal::TurnOn);
	}*/
	
}

// Called every frame
void ACCTVTerminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InUse)
		return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//FVector PlayerLocation = PlayerController->GetOwner()->GetActorLocation();
	FVector PlayerLocation = PlayerController->LastSpectatorSyncLocation;

	double SquaredDistance = FVector::DistSquared(PlayerLocation, GetActorLocation());
	if (SquaredDistance < FMath::Square(InteractRadius)) {}
		//if (PlayerController->)

}

void ACCTVTerminal::TurnOn()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	TerminalWidget = CreateWidget<UCCTVTerminalWidget>(PlayerController, UCCTVTerminalWidget::StaticClass());
	if (!TerminalWidget)
		UE_LOG(LogSecuritySystem, Error, TEXT("TerminalWidget is NULL"));
	if (!PlayerController)
		UE_LOG(LogSecuritySystem, Error, TEXT("PlayerController is NULL"));

	TerminalWidget->CCTVTerminal = this;	//bind delegates to widget buttons after this
	FInputModeGameAndUI Mode;
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	Mode.SetHideCursorDuringCapture(false);
	PlayerController->SetInputMode(Mode);
	TerminalWidget->AddToViewport(); // Z-order, this just makes it render on the very top.	9999
	UE_LOG(LogSecuritySystem, Log, TEXT("AddToViewport called, IsInViewport: %s"), TerminalWidget->IsInViewport() ? TEXT("true") : TEXT("false"));

	InUse = true;
}

void ACCTVTerminal::TurnOff()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//TerminalWidget->RemoveFromViewport();		//maybe pass widget as parameter?
	TerminalWidget->RemoveFromParent();
	TerminalWidget = nullptr;
	FInputModeGameOnly GameMode;
	PlayerController->SetInputMode(GameMode);
	//FSlateApplication::Get().SetFocusToGameViewport();
	//bShowMouseCursor = false;

	InUse = false;
}

void ACCTVTerminal::SwitchToNextCamera()
{
	if (++CurrentCameraIndex >= SecurityCameras.Num())
		CurrentCameraIndex = 0;

	UE_LOG(LogSecuritySystem, Log, TEXT("CCTVTerminal: SwitchToNextCamera"));
}

void ACCTVTerminal::SwitchToPreviousCamera()
{
	if (--CurrentCameraIndex <= -1)
		CurrentCameraIndex = SecurityCameras.Num() - 1;

	UE_LOG(LogSecuritySystem, Log, TEXT("CCTVTerminal: SwitchToPreviousCamera"));
}

void ACCTVTerminal::TriggerResponders()
{
	UDetector* DetectorComponent = SecurityCameras[CurrentCameraIndex]->DetectorComponent;

	if (!DetectorComponent)
		UE_LOG(LogSecuritySystem, Error, TEXT("ACCTVTerminal: DetectorComponent is NULL"));

	ESecurityState NewSecurityState;
	switch (DetectorComponent->CurrentState)
	{
	case ESecurityState::Neutral:
		NewSecurityState = ESecurityState::Alarm;
		break;
	case ESecurityState::Alarm:
		NewSecurityState = ESecurityState::Neutral;
		break;
	}
	DetectorComponent->CurrentState = NewSecurityState;

	UE_LOG(LogSecuritySystem, Log, TEXT("CCTVTerminal: TriggerResponders"));

	DetectorComponent->TriggerResponders(NewSecurityState);
}
