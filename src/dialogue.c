#include "dialogue.h"
bool IsDialogueActive;

DialogueSet *currentDialogue;
int dialogueIndex;
int displayCharacter;
const char *displayMessage;
float dialogueTween;

void OnDialogueHide(const char *id)
{
	IsDialogueActive = false;
	currentDialogue = NULL;
	dialogueIndex = 0;
}

void DialogueShow(const char *dialogueID)
{
	IsDialogueActive = false;
	currentDialogue = NULL;
	dialogueIndex = 0;

	for (int i = 0; i < DIALOGUE_COUNT; i++)
	{
		DialogueSet *st = &Dialogues[i];
		if (TextIsEqual(st->ID, dialogueID))
		{
			currentDialogue = st;

			displayCharacter = currentDialogue->Lines[dialogueIndex].CharacterID;
			displayMessage = currentDialogue->Lines[dialogueIndex].Message;

			break;
		}
	}

	if (currentDialogue == NULL)
		return;

	IsDialogueActive = true;
	TweenManager_AddFloatFrom(&dialogueTween, 0, 1, 0.5f, EASING_EASEINQUAD, "Dialogue", NULL);
}

void DialogueUpdate(float dt)
{
	if (dialogueTween < 1 || dialogueIndex >= currentDialogue->Count)
		return;

	if (IsPointerPressed())
	{
		dialogueIndex++;
		if (dialogueIndex >= currentDialogue->Count)
		{
			TweenManager_AddFloatFrom(&dialogueTween, 1, 0, 0.5f, EASING_EASEINQUAD, "Dialogue", OnDialogueHide);
		}
		else
		{
			displayCharacter = currentDialogue->Lines[dialogueIndex].CharacterID;
			displayMessage = currentDialogue->Lines[dialogueIndex].Message;
		}
	}
}

void DialogueRender(float dt)
{
	DrawRectangle(0, 0, VIRTUAL_WIDTH, 100, (Color){10, 10, 10, 150 * dialogueTween});
	DrawText(displayMessage, 12, 12, 24, (Color){255, 255, 255, 255 * dialogueTween});
}



static const Dialogue tutorial_start_lines[] = {
    { .CharacterID = -1, .Message = "[Riko] Hibiki, you got a minute?\nIt's two days before the event, but wanna test out the\nfireworks dodgeball?" },
    { .CharacterID = -1, .Message = "[Hibiki] Huh, already!? I haven't even finished hearing all\nthe rules yet!" },
    { .CharacterID = -1, .Message = "[Riko] It's fiiine, it's fiiine! Even if you get hit,\nit's totally safety-approved!" },
    { .CharacterID = -1, .Message = "[Riko] They just flash really bright!" },
    { .CharacterID = -1, .Message = "[Hibiki] The way you said 'just' is not reassuring at all..." },
    { .CharacterID = -1, .Message = "[Riko] Well, if it does seem dangerous, as staff I'll put\na stop to it!\n...Probably! Uh, I'll try my best?" },
    { .CharacterID = -1, .Message = "[Hibiki] Can we ban words like 'probably' and 'try' from\nstaff vocabulary...?" },
    { .CharacterID = -1, .Message = "[Riko] You'll be fine! With your footwork, you can\ndodge fireworks and flower petals alike!" },
    { .CharacterID = -1, .Message = "[Riko] Let's practice here so you can be the\ncoolest 'Firework Dodger' at the festival!" },
    { .CharacterID = -1, .Message = "[Hibiki] ...If you put it like that, I guess I have no choice." },
    { .CharacterID = -1, .Message = "[Hibiki] All right, Miss Matsuriya, you'd better watch\nclosely, okay? Safety check and hype!" },
    { .CharacterID = -1, .Message = "[Riko] Leave it to me!" },
    { .CharacterID = -1, .Message = "[Riko] Safety check, good!" },
    { .CharacterID = -1, .Message = "[Riko] Hype level, good!" },
    { .CharacterID = -1, .Message = "[Riko] Then... Hanabisaki: Flowerburst tutorial...\nSTART!" }
};


DialogueSet Dialogues[DIALOGUE_COUNT] = 
{
	{
        .ID    = "tutorial-start",
        .Lines = (Dialogue*)tutorial_start_lines,
        .Count = arraySize(tutorial_start_lines)
    }
};