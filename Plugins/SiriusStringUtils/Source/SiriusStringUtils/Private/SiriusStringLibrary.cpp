// Copyright 2022-2022 Jasper de Laat. All Rights Reserved.

#include "SiriusStringLibrary.h"

#include "Misc/StringFormatter.h"
#include "UObject/EditorObjectVersion.h"

void FSiriusStringFormatArgument::ResetValue()
{
	ArgumentValueType = ESiriusStringFormatArgumentType::String;
	ArgumentValue = FString();
	ArgumentValueInt = 0;
	ArgumentValueInt64 = 0;
	ArgumentValueFloat = 0.0f;
}

FStringFormatArg FSiriusStringFormatArgument::ToEngineFormatArg() const
{
	switch (ArgumentValueType)
	{
	case ESiriusStringFormatArgumentType::Int:
		return FStringFormatArg(ArgumentValueInt);
	case ESiriusStringFormatArgumentType::Int64:
		return FStringFormatArg(ArgumentValueInt64);
	case ESiriusStringFormatArgumentType::Float:
		return FStringFormatArg(ArgumentValueFloat);
	case ESiriusStringFormatArgumentType::String:
		return FStringFormatArg(ArgumentValue);
	default:
		break;
	}
	return FStringFormatArg(TEXT(""));
}

void operator<<(FStructuredArchive::FSlot Slot, FSiriusStringFormatArgument& Value)
{
	FArchive& UnderlyingArchive = Slot.GetUnderlyingArchive();
	FStructuredArchive::FRecord Record = Slot.EnterRecord();

	UnderlyingArchive.UsingCustomVersion(FEditorObjectVersion::GUID);
	
	Record << SA_VALUE(TEXT("ArgumentName"), Value.ArgumentName);

	uint8 TypeAsByte = static_cast<uint8>(Value.ArgumentValueType);
	if (UnderlyingArchive.IsLoading())
	{
		Value.ResetValue();
	}
	Record << SA_VALUE(TEXT("Type"), TypeAsByte);

	Value.ArgumentValueType = static_cast<ESiriusStringFormatArgumentType>(TypeAsByte);
	switch (Value.ArgumentValueType)
	{
	case ESiriusStringFormatArgumentType::Int:
		Record << SA_VALUE(TEXT("Value"), Value.ArgumentValueInt);
		break;
	case ESiriusStringFormatArgumentType::Int64:
		Record << SA_VALUE(TEXT("Value"), Value.ArgumentValueInt64);
		break;
	case ESiriusStringFormatArgumentType::Float:
		Record << SA_VALUE(TEXT("Value"), Value.ArgumentValueFloat);
		break;
	case ESiriusStringFormatArgumentType::String:
		Record << SA_VALUE(TEXT("Value"), Value.ArgumentValue);
		break;
	default:
		break;
	}
}

FString USiriusStringLibrary::Format(const FString& InPattern, const TArray<FSiriusStringFormatArgument>& InArgs)
{
	TMap<FString, FStringFormatArg> Args;
	for (const FSiriusStringFormatArgument& Arg : InArgs)
	{
		Args.Emplace(Arg.ArgumentName, Arg.ToEngineFormatArg());
	}
	return FString::Format(*InPattern, Args);
}
