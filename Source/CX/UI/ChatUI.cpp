// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatUI.h"
#include "CXPlayerController.h"	

void UChatUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &UChatUI::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &UChatUI::OnChatInputTextCommitted);
	}
}

void UChatUI::NativeDestruct()
{

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &UChatUI::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &UChatUI::OnChatInputTextCommitted);
	}

	Super::NativeDestruct();
}

void UChatUI::OnNewChatMessage(const FString& InSender, const FString& InMessage)
{
	FString NewMessage = InSender + TEXT(": ") + InMessage;

	FString OldMessage = MultilineEditableTextBox_ChatLogs->GetText().ToString();
	OldMessage += '\n';
	FText Message = FText::FromString(OldMessage + NewMessage);

	MultilineEditableTextBox_ChatLogs->SetText(Message);
}

void UChatUI::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if(CommitMethod == ETextCommit::OnEnter)
	{
		if(!Text.IsEmpty())
		{
			FString ChatMessage = Text.ToString();

			EditableTextBox_ChatInput->SetText(FText::GetEmpty());

			ACXPlayerController* PC = GetOwningPlayer<ACXPlayerController>();
			if (PC)
			{
				UE_LOG(LogTemp, Warning, TEXT("[ChatUI] PC 찾음! CTSChatMessage 호출"));
				PC->CTSChatMessage(ChatMessage);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[ChatUI] GetOwningPlayer가 nullptr입니다!"));
				// 임시 대안: GetWorld()->GetFirstPlayerController<ACXPlayerController>()
			}
		}
	}
}