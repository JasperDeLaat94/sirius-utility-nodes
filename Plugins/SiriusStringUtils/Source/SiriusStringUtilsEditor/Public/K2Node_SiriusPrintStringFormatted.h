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
	virtual ERedirectType DoPinsMatchForReconstruction(const UEdGraphPin* NewPin, int32 NewPinIndex, const UEdGraphPin* OldPin, int32 OldPinIndex) const override;
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	virtual bool NodeCausesStructuralBlueprintChange() const override { return true; }
	virtual void PostReconstructNode() override;
	//~ End UK2Node Interface.

private:
	/** returns Format pin */
	UEdGraphPin* GetFormatPin() const;

	/** Finds an argument pin by name, checking strings in a strict, case sensitive fashion
	 * @param InPinName		The pin name to check for
	 * @return				nullptr if the pin was not found, otherwise the found pin.
	 */
	UEdGraphPin* FindArgumentPin(const FName InPinName) const;

	/** Synchronize the type of the given argument pin with the type its connected to, or reset it to a wildcard pin if there's no connection */
	void SynchronizeArgumentPinType(UEdGraphPin* Pin) const;
	
	/** When adding arguments to the node, their names are placed here and are generated as pins during construction */
	UPROPERTY()
	TArray<FName> PinNames;

	/** The "Format" input pin, always available on the node */
	UEdGraphPin* CachedFormatPin = nullptr;

	/** Tooltip text for this node. */
	FText NodeTooltip;
};
