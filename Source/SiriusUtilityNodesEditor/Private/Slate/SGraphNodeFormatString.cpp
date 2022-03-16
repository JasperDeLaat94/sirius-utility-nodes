// Copyright 2022-2022 Jasper de Laat. All Rights Reserved.

#include "SGraphNodeFormatString.h"
#include "Widgets/SBoxPanel.h"
#include "GraphEditorSettings.h"
#include "K2Node_SiriusFormatString.h"
#include "NodeFactory.h"

#define LOCTEXT_NAMESPACE "FormatStringNode"

void SGraphNodeFormatString::Construct(const FArguments& InArgs, UK2Node_SiriusFormatString* InNode)
{
	this->GraphNode = InNode;
	this->SetCursor(EMouseCursor::CardinalCross);
	this->UpdateGraphNode();
}

void SGraphNodeFormatString::CreatePinWidgets()
{
	for (UEdGraphPin* Pin : GraphNode->Pins)
	{
		if (!Pin->bHidden)
		{
			TSharedPtr<SGraphPin> NewPin = FNodeFactory::CreatePinWidget(Pin);
			check(NewPin.IsValid());
			AddPin(NewPin.ToSharedRef());
		}
	}
}

void SGraphNodeFormatString::CreateInputSideAddButton(const TSharedPtr<SVerticalBox> InputBox)
{
	const TSharedRef<SWidget> AddPinButton = AddPinButtonContent(
		LOCTEXT("AddPinButton", "Add pin"),
		LOCTEXT("AddPinButton_Tooltip", "Adds an argument to the node"),
		false);

	FMargin AddPinPadding = Settings->GetInputPinPadding();
	AddPinPadding.Top += 6.0f;

	InputBox->AddSlot()
	        .AutoHeight()
	        .VAlign(VAlign_Center)
	        .Padding(AddPinPadding)
	[
		AddPinButton
	];
}

EVisibility SGraphNodeFormatString::IsAddPinButtonVisible() const
{
	EVisibility VisibilityState = EVisibility::Collapsed;
	if (const UK2Node_SiriusFormatString* FormatNode = Cast<UK2Node_SiriusFormatString>(GraphNode))
	{
		VisibilityState = SGraphNode::IsAddPinButtonVisible();
		if (VisibilityState == EVisibility::Visible)
		{
			VisibilityState = FormatNode->CanEditArguments() ? EVisibility::Visible : EVisibility::Collapsed;
		}
	}
	return VisibilityState;
}

FReply SGraphNodeFormatString::OnAddPin()
{
	if (UK2Node_SiriusFormatString* FormatText = Cast<UK2Node_SiriusFormatString>(GraphNode))
	{
		FormatText->AddArgumentPin();
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
