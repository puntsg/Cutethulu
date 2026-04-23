// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TutorialEntry.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETutorialContent : uint8 {
	TEXT UMETA(DisplayName = "Text"),
	IMAGE UMETA(DisplayName = "Image"),
	SPACER UMETA(DisplayName = "Spacer")
};

USTRUCT(BlueprintType)
struct YAKOTAKI_API FTutorialImageBlock
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FMargin padding;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TEnumAsByte<EHorizontalAlignment> horizontalAlignment;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TEnumAsByte<EVerticalAlignment> verticalAligment;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool forceNewLine;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UTexture2D* image;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector2D imageSize;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FLinearColor colourAndOpacity;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float renderOpacity;
};

USTRUCT(BlueprintType)
struct YAKOTAKI_API FTutorialSpacerBlock
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool fillEmptySpace;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector2D size;
};

USTRUCT(BlueprintType)
struct YAKOTAKI_API FTutorialTextBlock
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FMargin Padding;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool forceNewLine;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText text;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FTextBlockStyle textStyle;

	//UPROPERTY(BlueprintReadOnly, EditAnywhere)
	//ETextJustify justification;
};


USTRUCT(BlueprintType)
struct YAKOTAKI_API FTutorialContent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ETutorialContent tutorialContent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FTutorialTextBlock textContent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FTutorialImageBlock imageContent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FTutorialSpacerBlock spacer;
};


USTRUCT(BlueprintType)
struct YAKOTAKI_API FTutorialEntry : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString tutorialName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTutorialContent> tutorialContents;
};