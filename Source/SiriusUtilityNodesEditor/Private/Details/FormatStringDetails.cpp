// Copyright 2022-2022 Jasper de Laat. All Rights Reserved.

#include "FormatStringDetails.h"

#include "Widgets/SWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "UObject/Package.h"
#include "EditorStyleSet.h"
#include "PropertyHandle.h"
#include "IDetailChildrenBuilder.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "K2Node_SiriusFormatString.h"
#include "PropertyCustomizationHelpers.h"

#define LOCTEXT_NAMESPACE "FormatStringDetails"

void FFormatStringArgumentLayout::GenerateHeaderRowContent(FDetailWidgetRow& NodeRow)
{
	const int32 ArgumentCount = TargetNode->GetArgumentCount();
	const bool bIsMoveUpEnabled = ArgumentCount != 1 && ArgumentIndex != 0;
	const bool bIsMoveDownEnabled = ArgumentCount != 1 && ArgumentIndex < ArgumentCount - 1;

	const TSharedRef<SWidget> ClearButton = PropertyCustomizationHelpers::MakeClearButton(FSimpleDelegate::CreateSP(this, &FFormatStringArgumentLayout::OnArgumentRemove));

	NodeRow
		.WholeRowWidget
		[
			SNew(SHorizontalBox)
			.IsEnabled(this, &FFormatStringArgumentLayout::CanEditArguments)

			+ SHorizontalBox::Slot()
			[
				SAssignNew(ArgumentNameWidget, SEditableTextBox)
				.OnTextCommitted(this, &FFormatStringArgumentLayout::OnArgumentNameCommitted)
				.OnTextChanged(this, &FFormatStringArgumentLayout::OnArgumentNameChanged)
				.Text(this, &FFormatStringArgumentLayout::GetArgumentName)
			]

			+ SHorizontalBox::Slot()
			  .AutoWidth()
			  .Padding(2, 0)
			[
				SNew(SButton)
				.ContentPadding(0)
				.OnClicked(this, &FFormatStringArgumentLayout::OnMoveArgumentUp)
				.IsEnabled(bIsMoveUpEnabled)
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush("BlueprintEditor.Details.ArgUpButton"))
				]
			]
			+ SHorizontalBox::Slot()
			  .AutoWidth()
			  .Padding(2, 0)
			[
				SNew(SButton)
				.ContentPadding(0)
				.OnClicked(this, &FFormatStringArgumentLayout::OnMoveArgumentDown)
				.IsEnabled(bIsMoveDownEnabled)
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush("BlueprintEditor.Details.ArgDownButton"))
				]
			]

			+ SHorizontalBox::Slot()
			  .AutoWidth()
			  .Padding(2, 0)
			[
				ClearButton
			]
		];
}

FText FFormatStringArgumentLayout::GetArgumentName() const
{
	return TargetNode->GetArgumentName(ArgumentIndex);
}

void FFormatStringArgumentLayout::OnArgumentRemove() const
{
	TargetNode->RemoveArgument(ArgumentIndex);
}

FReply FFormatStringArgumentLayout::OnMoveArgumentUp() const
{
	TargetNode->SwapArguments(ArgumentIndex, ArgumentIndex - 1);
	return FReply::Handled();
}

FReply FFormatStringArgumentLayout::OnMoveArgumentDown() const
{
	TargetNode->SwapArguments(ArgumentIndex, ArgumentIndex + 1);
	return FReply::Handled();
}

bool FFormatStringArgumentLayout::CanEditArguments() const
{
	return TargetNode->CanEditArguments();
}

struct FScopeTrue
{
	bool& bRef;

	explicit FScopeTrue(bool& bInRef) : bRef(bInRef)
	{
		ensure(!bRef);
		bRef = true;
	}

	~FScopeTrue()
	{
		ensure(bRef);
		bRef = false;
	}
};

void FFormatStringArgumentLayout::OnArgumentNameCommitted(const FText& NewText, ETextCommit::Type /*InTextCommit*/)
{
	if (IsValidArgumentName(NewText))
	{
		FScopeTrue ScopeTrue(bCausedChange);
		TargetNode->SetArgumentName(ArgumentIndex, *NewText.ToString());
	}
	ArgumentNameWidget.Pin()->SetError(FString());
}

void FFormatStringArgumentLayout::OnArgumentNameChanged(const FText& NewText) const
{
	// ReSharper disable once CppExpressionWithoutSideEffects
	IsValidArgumentName(NewText);
}

bool FFormatStringArgumentLayout::IsValidArgumentName(const FText& InNewText) const
{
	if (TargetNode->FindArgumentPin(*InNewText.ToString()))
	{
		ArgumentNameWidget.Pin()->SetError(LOCTEXT("UniqueName_Error", "Name must be unique."));
		return false;
	}
	ArgumentNameWidget.Pin()->SetError(FString());
	return true;
}

bool FFormatStringLayout::CausedChange() const
{
	for (const auto Child : Children)
	{
		if (Child.IsValid() && Child.Pin()->CausedChange())
		{
			return true;
		}
	}
	return false;
}

void FFormatStringLayout::GenerateChildContent(IDetailChildrenBuilder& ChildrenBuilder)
{
	Children.Empty();
	for (int32 ArgIdx = 0; ArgIdx < TargetNode->GetArgumentCount(); ++ArgIdx)
	{
		TSharedRef<FFormatStringArgumentLayout> ArgumentIndexLayout = MakeShareable(new FFormatStringArgumentLayout(TargetNode, ArgIdx));
		ChildrenBuilder.AddCustomBuilder(ArgumentIndexLayout);
		Children.Add(ArgumentIndexLayout);
	}
}

FFormatStringDetails::~FFormatStringDetails()
{
	UPackage::PackageDirtyStateChangedEvent.RemoveAll(this);
}

void FFormatStringDetails::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	const TArray<TWeakObjectPtr<UObject>>& Objects = DetailLayout.GetSelectedObjects();
	check(Objects.Num() > 0);

	if (Objects.Num() == 1)
	{
		TargetNode = CastChecked<UK2Node_SiriusFormatString>(Objects[0].Get());
		TSharedRef<IPropertyHandle> PropertyHandle = DetailLayout.GetProperty(FName("PinNames"), UK2Node_SiriusFormatString::StaticClass());

		IDetailCategoryBuilder& InputsCategory = DetailLayout.EditCategory("Arguments", LOCTEXT("DetailsArguments", "Arguments"));

		InputsCategory.AddCustomRow(LOCTEXT("FunctionNewInputArg", "New"))
		[
			SNew(SBox)
			.HAlign(HAlign_Right)
			[
				SNew(SButton)
					.Text(LOCTEXT("FunctionNewInputArg", "New"))
					.OnClicked(this, &FFormatStringDetails::OnAddNewArgument)
					.IsEnabled(this, &FFormatStringDetails::CanEditArguments)
			]
		];

		Layout = MakeShareable(new FFormatStringLayout(TargetNode));
		InputsCategory.AddCustomBuilder(Layout.ToSharedRef());
	}

	UPackage::PackageDirtyStateChangedEvent.AddSP(this, &FFormatStringDetails::OnEditorPackageModified);
}

void FFormatStringDetails::OnForceRefresh() const
{
	Layout->Refresh();
}

FReply FFormatStringDetails::OnAddNewArgument() const
{
	TargetNode->AddArgumentPin();
	OnForceRefresh();
	return FReply::Handled();
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void FFormatStringDetails::OnEditorPackageModified(UPackage* Package) const
{
	if (TargetNode &&
		Package &&
		Package->IsDirty() &&
		Package == TargetNode->GetOutermost() &&
		(!Layout.IsValid() || !Layout->CausedChange()))
	{
		OnForceRefresh();
	}
}

bool FFormatStringDetails::CanEditArguments() const
{
	return TargetNode->CanEditArguments();
}

#undef LOCTEXT_NAMESPACE
