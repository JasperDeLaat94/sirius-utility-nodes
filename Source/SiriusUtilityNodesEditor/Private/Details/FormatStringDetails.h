// Copyright 2022-2022 Jasper de Laat. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "IDetailCustomization.h"
#include "IDetailCustomNodeBuilder.h"

class FDetailWidgetRow;
class IDetailChildrenBuilder;
class IDetailLayoutBuilder;
class SEditableTextBox;
class UK2Node_SiriusFormatString;

/** Custom struct for each group of arguments in the function editing details */
class FFormatStringArgumentLayout final : public IDetailCustomNodeBuilder, public TSharedFromThis<FFormatStringArgumentLayout>
{
public:
	FFormatStringArgumentLayout(UK2Node_SiriusFormatString* InTargetNode, int32 InArgumentIndex)
		: TargetNode(InTargetNode),
		  ArgumentIndex(InArgumentIndex),
		  bCausedChange(false)
	{
	}

	bool CausedChange() const { return bCausedChange; }

private:
	/** IDetailCustomNodeBuilder Interface*/
	virtual void SetOnRebuildChildren(FSimpleDelegate InOnRegenerateChildren) override
	{
	};
	virtual void GenerateHeaderRowContent(FDetailWidgetRow& NodeRow) override;

	virtual void GenerateChildContent(IDetailChildrenBuilder& ChildrenBuilder) override
	{
	};

	virtual void Tick(float DeltaTime) override
	{
	}

	virtual bool RequiresTick() const override { return false; }
	virtual FName GetName() const override { return NAME_None; }
	virtual bool InitiallyCollapsed() const override { return false; }

	/** Retrieves the argument's name */
	FText GetArgumentName() const;

	/** Moves the argument up in the list */
	FReply OnMoveArgumentUp() const;

	/** Moves the argument down in the list */
	FReply OnMoveArgumentDown() const;

	/** Deletes the argument */
	void OnArgumentRemove() const;

	/** Callback when the argument's name is committed */
	void OnArgumentNameCommitted(const FText& NewText, ETextCommit::Type InTextCommit);

	/** Callback when changing the argument's name to verify the name */
	void OnArgumentNameChanged(const FText& NewText) const;

	/** 
	 * Helper function to validate the argument's name
	 *
	 * @param InNewText		The name to validate
	 *
	 * @return				Returns true if the name is valid
	 */
	bool IsValidArgumentName(const FText& InNewText) const;

	bool CanEditArguments() const;

	/** The target node that this argument is on */
	UK2Node_SiriusFormatString* TargetNode;

	/** Index of argument */
	int32 ArgumentIndex;

	/** The argument's name widget, used for setting a argument's name */
	TWeakPtr<SEditableTextBox> ArgumentNameWidget;

	bool bCausedChange;
};

/** Custom struct for each group of arguments in the function editing details */
class FFormatStringLayout final : public IDetailCustomNodeBuilder, public TSharedFromThis<FFormatStringLayout>
{
public:
	explicit FFormatStringLayout(UK2Node_SiriusFormatString* InTargetNode)
		: TargetNode(InTargetNode)
	{
	}

	void Refresh() const
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		OnRebuildChildren.ExecuteIfBound();
	}

	bool CausedChange() const;

private:
	/** IDetailCustomNodeBuilder Interface*/
	virtual void SetOnRebuildChildren(FSimpleDelegate InOnRegenerateChildren) override { OnRebuildChildren = InOnRegenerateChildren; }

	virtual void GenerateHeaderRowContent(FDetailWidgetRow& NodeRow) override
	{
	}

	virtual void GenerateChildContent(IDetailChildrenBuilder& ChildrenBuilder) override;

	virtual void Tick(float DeltaTime) override
	{
	}

	virtual bool RequiresTick() const override { return false; }
	virtual FName GetName() const override { return NAME_None; }
	virtual bool InitiallyCollapsed() const override { return false; }

	FSimpleDelegate OnRebuildChildren;

	/** The target node that this argument is on */
	UK2Node_SiriusFormatString* TargetNode;

	TArray<TWeakPtr<FFormatStringArgumentLayout>> Children;
};

/** Details customization for the "Format String" node */
class FFormatStringDetails final : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShareable(new FFormatStringDetails);
	}

	FFormatStringDetails() : TargetNode(nullptr)
	{
	}

	virtual ~FFormatStringDetails() override;

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;

	/** Forces a refresh on the details customization */
	void OnForceRefresh() const;

private:
	/** Handles new argument request */
	FReply OnAddNewArgument() const;

	/** Callback whenever a package is marked dirty, will refresh the node being represented by this details customization */
	void OnEditorPackageModified(UPackage* Package) const;

	bool CanEditArguments() const;

	TSharedPtr<FFormatStringLayout> Layout;
	/** The target node that this argument is on */
	UK2Node_SiriusFormatString* TargetNode;
};
