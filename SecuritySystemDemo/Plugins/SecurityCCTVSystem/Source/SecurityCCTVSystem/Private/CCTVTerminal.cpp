// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTVTerminal.h"
#include "Kismet/KismetRenderingLibrary.h"

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
}

// Called when the game starts or when spawned
void ACCTVTerminal::BeginPlay()
{
	Super::BeginPlay();

	//TextureRenderTarget->ConstructTexture2D(this, "RenderTexture", RF_NoFlags);
	TextureRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, 1024, 1024, ETextureRenderTargetFormat::RTF_RGBA32f, FLinearColor::Black, false);

	//DynamicMaterialInstance = UMaterialInstanceDynamic::Create(ParentMaterial, this);
	DynamicMaterialInstance = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
	DynamicMaterialInstance->SetTextureParameterValue(FName("ScreenTexture"), TextureRenderTarget);

	//material->SetTextureParameterValue(TEXT("TextureInput"), RenderTarget2D);
	//MeshComponent->SetMaterial(0, material);

	//RenderMaterial = new UMaterial();
	//UMaterialInstanceDynamic* ExampleMID = UMaterialInstanceDynamic::Create(ExampleMaterial);
	
}

// Called every frame
void ACCTVTerminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

