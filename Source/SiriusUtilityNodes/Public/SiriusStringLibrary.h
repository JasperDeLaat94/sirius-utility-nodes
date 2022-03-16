// Copyright 2022-2022 Jasper de Laat. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SiriusStringLibrary.generated.h"

UENUM(BlueprintType)
enum class ESiriusStringFormatArgumentType : uint8
{
	Int,
	Int64,
	Float,
	String,
};

/** Used to pass argument/value pairs into FString::Format. */
USTRUCT(NoExport, BlueprintInternalUseOnly)
struct FSiriusStringFormatArgument
{
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=ArgumentName)
	FString ArgumentName;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=ArgumentValue)
	ESiriusStringFormatArgumentType ArgumentValueType;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=ArgumentValue)
	FString ArgumentValue;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=ArgumentValue)
	int32 ArgumentValueInt;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=ArgumentValue)
	int64 ArgumentValueInt64;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=ArgumentValue)
	float ArgumentValueFloat;

	FSiriusStringFormatArgument()
	{
		ResetValue();
	}

	void ResetValue();
	
	FStringFormatArg ToEngineFormatArg() const;

	friend void operator<<(FStructuredArchive::FSlot Slot, FSiriusStringFormatArgument& Value);
};

UCLASS(meta=(BlueprintThreadSafe, ScriptName="SiriusStringLibrary"))
class SIRIUSUTILITYNODES_API USiriusStringLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/* Used for formatting a string using the FString::Format function and utilized by the UK2Node_SiriusFormatString */
	UFUNCTION(BlueprintPure, meta=(BlueprintInternalUseOnly = "true"))
	static FString Format(const FString& InPattern, const TArray<FSiriusStringFormatArgument>& InArgs);
};
