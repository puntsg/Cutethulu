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
struct YAKOTAKI_API FInputActionImage {
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMargin padding;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* image;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D imageSize;

	FInputActionImage(){}
};

USTRUCT(BlueprintType)
struct YAKOTAKI_API FTutorialImageBlock
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EHorizontalAlignment> horizontalAlignment;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EVerticalAlignment> verticalAligment;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool forceNewLine;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor colourAndOpacity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FInputActionImage keyboardImage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FInputActionImage controllerImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float renderOpacity;
};

USTRUCT(BlueprintType)
struct YAKOTAKI_API FTutorialSpacerBlock
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool fillEmptySpace;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D size;
};

USTRUCT(BlueprintType)
struct YAKOTAKI_API FTutorialTextBlock
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMargin Padding;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool forceNewLine;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText text;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTextBlockStyle textStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ETextJustify::Type>justification;
};


USTRUCT(BlueprintType)
struct YAKOTAKI_API FTutorialContent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETutorialContent tutorialContent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "tutorialContent == ETutorialContent::TEXT", EditConditionHides))
	FTutorialTextBlock textContent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "tutorialContent == ETutorialContent::IMAGE", EditConditionHides))
	FTutorialImageBlock imageContent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "tutorialContent == ETutorialContent::SPACER", EditConditionHides))
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