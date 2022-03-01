// Copyright 2022-2022 Jasper de Laat. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_SiriusPrintStringFormatted.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI)
class UK2Node_SiriusPrintStringFormatted : public UK2Node
{
	GENERATED_BODY()

public:
	UK2Node_SiriusPrintStringFormatted();

	//~ Begin UEdGraphNode Interface.
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetPinDisplayName(const UEdGraphPin* Pin) const override;
	virtual FText GetTooltipText() const override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	virtual void PinTypeChanged(UEdGraphPin* Pin) override;
	//~ End UEdGraphNode Interface.

	//~ Begin UK2Node Interface.
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	virtual bool NodeCausesStructuralBlueprintChange() const override { return true; }
	virtual void PostReconstructNode() override;
	//~ End UK2Node Interface.

private:
	UEdGraphPin* GetExecutePin() const;
	UEdGraphPin* GetThenPin() const;
	UEdGraphPin* GetFormatPin() const;
	UEdGraphPin* GetPrintScreenPin() const;
	UEdGraphPin* GetPrintLogPin() const;
	UEdGraphPin* GetTextColorPin() const;
	UEdGraphPin* GetDurationPin() const;

	UEdGraphPin* FindArgumentPin(const FName PinName) const;

	/** Synchronize the type of the given argument pin with the type its connected to, or reset it to a wildcard pin if there's no connection */
	void SynchronizeArgumentPinType(UEdGraphPin* Pin) const;

	static const FName ExecutePinName;
	static const FName ThenPinName;
	static const FName FormatPinName;
	static const FName PrintScreenPinName;
	static const FName PrintLogPinName;
	static const FName TextColorPinName;
	static const FName DurationPinName;

	/** When adding arguments to the node, their names are placed here and are generated as pins during construction */
	UPROPERTY()
	TArray<FName> PinNames;

	/** Tooltip text for this node. */
	FText NodeTooltip;

	TArray<UEdGraphPin*> CachedArgumentPins;
};
