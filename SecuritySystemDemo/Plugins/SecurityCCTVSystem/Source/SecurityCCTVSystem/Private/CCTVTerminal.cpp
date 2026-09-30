// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTVTerminal.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
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

	MeshComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 1.0f));
	MeshComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/SecurityCCTVSystem/M_Screen.M_Screen"));
	if (MaterialFinder.Succeeded())
	{
		RenderMaterial = MaterialFinder.Object;
	}

#if 0
	InputMapping = NewObject<UCCTVInputMappingContext>();
	//UE_LOG(LogSecuritySystem, Log, TEXT("Creation: Mappings size: %d"), InputMapping->GetMappings().Num());
	UWorld* World = GetWorld();
	if (World)
	{
		UE_LOG(LogSecuritySystem, Log, TEXT("World found"));
		if (ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController())
		{
			UE_LOG(LogSecuritySystem, Log, TEXT("Local player found"));
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				UE_LOG(LogSecuritySystem, Log, TEXT("Input system found"));
				if (!InputMapping)
				{
					UE_LOG(LogSecuritySystem, Log, TEXT("Input mapping found"));
					InputSystem->AddMappingContext(InputMapping, 0);
					UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);

					//UE_LOG(LogSecuritySystem, Log, TEXT("Last: Mappings size: %d"), InputMapping->GetMappings().Num());

					//FEnhancedActionKeyMapping CCTVMapping = InputMapping->GetMapping(InputMapping->InteractKeyIndex);
					FEnhancedActionKeyMapping CCTVMapping = InputMapping->GetInteractKeyMapping();

					Input->BindAction(CCTVMapping.Action, ETriggerEvent::Triggered, this, &ACCTVTerminal::TurnOn);
				}
			}
		}
	}
#endif
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

	TurnOn(FInputActionValue());

	//InteractInputAction = NewObject<UCCTVInteractInputAction>();
	//InteractInputAction->ValueType = EInputActionValueType::Boolean;

	// Create a Hold Trigger
	//UInputTriggerHold* HoldTrigger = NewObject<UInputTriggerHold>();
	//HoldTrigger->HoldTimeThreshold = 0.5f; // Hold time in seconds

	// Add the trigger to the action
	//InteractInputAction->Triggers.Add(HoldTrigger);

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

	/*bBlockInput = false;
	EnableInput(GetWorld()->GetFirstPlayerController());
	UInputComponent* InputComponent = this->InputComponent;
	InputComponent->BindAction("InteractKey", IE_Pressed, this, &ACCTVTerminal::TurnOn);*/

	//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//PlayerController->InputComponent->BindAction("CCTVTerminalInteractKey", IE_Pressed, this, &ACCTVTerminal::TurnOn);


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


	/*UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	Subsystem->GetPlayerInput()->GetEnhancedActionMappings();
	Subsystem->GetPlayerInput()->AddActionMapping();
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);*/

#if 0
	UInputSettings* MyInputSettings = UInputSettings::GetInputSettings();
	FInputActionKeyMapping ActionMapping;

	FInputChord NewKey; // this value can be retrieved from key selector widget
	NewKey.Key = EKeys::E;

	ActionMapping.ActionName = FName("CCTVTerminalInteractAction");
	ActionMapping.Key = NewKey.Key;

	MyInputSettings->AddActionMapping(ActionMapping);
	MyInputSettings->SaveConfig();

	TArray<FInputActionKeyMapping> OutMappings;
	MyInputSettings->GetActionMappingByName("CCTVTerminalInteractAction", OutMappings);


	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	const UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	UEnhancedInputUserSettings* UserSettings = InputSubsystem->GetUserSettings();
	if (!UserSettings)
	{
		UE_LOG(LogSecuritySystem, Error, TEXT("UserSettings is NULL"));
		return;
	}
	//UserSettings->RegisterInputMappingContext();

	UEnhancedPlayerMappableKeyProfile* CurrentProfile = UserSettings->GetCurrentKeyProfile();
#endif

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	InputMapping = NewObject<UCCTVInputMappingContext>();
	if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController())
	{
		UE_LOG(LogSecuritySystem, Log, TEXT("Local player found"));
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			UE_LOG(LogSecuritySystem, Log, TEXT("Input system found"));
			if (InputMapping)
			{
				UE_LOG(LogSecuritySystem, Log, TEXT("Input mapping found"));
				InputSystem->AddMappingContext(InputMapping, 0);
				UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

				//FEnhancedActionKeyMapping CCTVMapping = InputMapping->GetMapping(InputMapping->InteractKeyIndex);
				FEnhancedActionKeyMapping& CCTVMapping = InputMapping->GetInteractKeyMapping();
				if (!CCTVMapping.Action)
					UE_LOG(LogSecuritySystem, Error, TEXT("CCTVMapping.Action is NULL"));

				Input->BindAction(CCTVMapping.Action, ETriggerEvent::Triggered, this, &ACCTVTerminal::TurnOn);
			}
		}
	}
}

// Called every frame
void ACCTVTerminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACCTVTerminal::TurnOn(const FInputActionValue& Value)
{
	if (InUse)
		return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//check whether player is inside terminals interact radius
	FVector PlayerLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	//radius check
	/*UE_LOG(LogSecuritySystem, Log, TEXT("PlayerLocation: %f %f %f"), PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z);
	double SquaredDistance = FVector::DistSquared(PlayerLocation, this->GetActorLocation());
	UE_LOG(LogSecuritySystem, Log, TEXT("TerminalLocation: %f %f %f"), this->GetActorLocation().X, this->GetActorLocation().Y, this->GetActorLocation().Z);
	if (SquaredDistance >= FMath::Square(InteractRadius))
		return;*/

	//interact radius check	
	FVector PlayerToTerminalVector = this->GetActorLocation() - PlayerLocation;
	double SquaredDistance = PlayerToTerminalVector.SizeSquared();
	if (SquaredDistance >= FMath::Square(InteractRadius))
		return;
	//dot product check	
	PlayerToTerminalVector.Normalize();
	FVector PlayerForwardVector = PlayerController->PlayerCameraManager->GetActorForwardVector();
	if (FVector::DotProduct(PlayerForwardVector, PlayerToTerminalVector) <= 0.8f)
		return;
	

	TerminalWidget = CreateWidget<UCCTVTerminalWidget>(PlayerController, UCCTVTerminalWidget::StaticClass());
	if (!TerminalWidget)
		UE_LOG(LogSecuritySystem, Error, TEXT("TerminalWidget is NULL"));
	if (!PlayerController)
		UE_LOG(LogSecuritySystem, Error, TEXT("PlayerController is NULL"));

	TerminalWidget->CCTVTerminal = this;
	PlayerController->FlushPressedKeys();
	PlayerController->bShowMouseCursor = true;
	//FInputModeGameAndUI Mode;
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(TerminalWidget->TakeWidget());
	//Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	//Mode.SetHideCursorDuringCapture(false);
	PlayerController->SetInputMode(InputMode);
	TerminalWidget->AddToViewport();
	UE_LOG(LogSecuritySystem, Log, TEXT("AddToViewport called, IsInViewport: %s"), TerminalWidget->IsInViewport() ? TEXT("true") : TEXT("false"));

	InUse = true;
}

void ACCTVTerminal::TurnOff()
{
	if (!InUse)
		return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//TerminalWidget->RemoveFromViewport();		//maybe pass widget as parameter?
	TerminalWidget->RemoveFromParent();
	TerminalWidget = nullptr;
	FInputModeGameOnly GameMode;
	PlayerController->SetInputMode(GameMode);
	//FSlateApplication::Get().SetFocusToGameViewport();
	PlayerController->bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);

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
