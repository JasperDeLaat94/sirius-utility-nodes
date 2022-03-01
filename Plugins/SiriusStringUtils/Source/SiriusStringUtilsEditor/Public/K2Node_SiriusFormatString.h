// Copyright 2022-2022 Jasper de Laat. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_SiriusFormatString.generated.h"

class FBlueprintActionDatabaseRegistrar;
class UEdGraph;

UCLASS(MinimalAPI)
class UK2Node_SiriusFormatString : public UK2Node
{
	GENERATED_BODY()

public:
	explicit UK2Node_SiriusFormatString(const FObjectInitializer& ObjectInitializer);

	//~ Begin UObject Interface
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface

	//~ Begin UEdGraphNode Interface.
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	virtual void PinTypeChanged(UEdGraphPin* Pin) override;
	virtual FText GetTooltipText() const override;
	virtual FText GetPinDisplayName(const UEdGraphPin* Pin) const override;
	//~ End UEdGraphNode Interface.

	//~ Begin UK2Node Interface.
	virtual bool IsNodePure() const override { return true; }
	virtual bool NodeCausesStructuralBlueprintChange() const override { return true; }
	virtual void PostReconstructNode() override;
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual ERedirectType DoPinsMatchForReconstruction(const UEdGraphPin* NewPin, int32 NewPinIndex, const UEdGraphPin* OldPin, int32 OldPinIndex) const override;
	virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	//~ End UK2Node Interface.

	/** Returns Format pin */
	SIRIUSSTRINGUTILSEDITOR_API UEdGraphPin* GetFormatPin() const;

	/** Returns Result pin */
	SIRIUSSTRINGUTILSEDITOR_API UEdGraphPin* GetResultPin() const;

	/**
	 * Finds an argument pin by name, checking strings in a strict, case sensitive fashion
	 *
	 * @param InPinName		The pin name to check for
	 * @return				NULL if the pin was not found, otherwise the found pin.
	 */
	SIRIUSSTRINGUTILSEDITOR_API UEdGraphPin* FindArgumentPin(const FName InPinName) const;

	/** 
	 * Add a new wildcard argument to the node and create the associated pin. Intended for programmatic usage (e.g. K2Node expansion).
	 *
	 * @param InPinName		The pin name to add.
	 * @return				The newly created pin.
	 */
	SIRIUSSTRINGUTILSEDITOR_API UEdGraphPin* AddArgumentPin(const FName InPinName);

	/** Synchronize the type of the given argument pin with the type its connected to, or reset it to a wildcard pin if there's no connection */
	SIRIUSSTRINGUTILSEDITOR_API void SynchronizeArgumentPinType(UEdGraphPin* Pin) const;

private:
	static const FName FormatPinName;
	static const FName ResultPinName;
	
	/** When adding arguments to the node, their names are placed here and are generated as pins during construction */
	UPROPERTY()
	TArray<FName> PinNames;

	/** The "Format" input pin, always available on the node */
	UEdGraphPin* CachedFormatPin;

	/** Tooltip text for this node. */
	FText NodeTooltip;
};
