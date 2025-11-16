#include "dialogue.h"
bool IsDialogueActive;

DialogueSet *currentDialogue;
int dialogueIndex;
int displayCharacter;

float dialogueTypeProgress;
float dialogueTypeSpeed = 0.2f;
int dialogueMessageLength;

const char *displayMessage;
float dialogueTween;
static void (*endFunction)(void);

void OnDialogueHide(void)
{
	IsDialogueActive = false;
	currentDialogue = NULL;
	dialogueIndex = 0;

	if (endFunction != NULL)
	{
		endFunction();
		endFunction = NULL;
	}
}

static void UpdateDialogue()
{
	const int minText = 1;
	const int maxText = 100;

	dialogueTypeProgress = 0.0f;
	displayCharacter = currentDialogue->Lines[dialogueIndex].CharacterID;
	displayMessage = currentDialogue->Lines[dialogueIndex].Message;
	dialogueMessageLength = strlen(displayMessage);
	dialogueTypeSpeed = Remap(Clamp(dialogueMessageLength, minText, maxText), minText, maxText, 0.95f, 0.5f);
	TraceLog(LOG_INFO, "%d %f", dialogueMessageLength, dialogueTypeSpeed);
}

void DialogueShow(const char *dialogueID, void (*onEnd)(void))
{
	endFunction = onEnd;
	IsDialogueActive = false;
	currentDialogue = NULL;
	dialogueIndex = 0;

	for (int i = 0; i < DIALOGUE_COUNT; i++)
	{
		DialogueSet *st = &Dialogues[i];
		if (TextIsEqual(st->ID, dialogueID))
		{
			currentDialogue = st;
			UpdateDialogue();
			break;
		}
	}

	if (currentDialogue == NULL)
	{
		endFunction = NULL;
		return;
	}

	IsDialogueActive = true;
	TweenManager_AddFloatFrom(&dialogueTween, 0, 1, 0.5f, EASING_EASEINQUAD, "Dialogue", NULL);
}

void DialogueUpdate(float dt)
{
	if (dialogueTween < 1 || dialogueIndex >= currentDialogue->Count)
		return;

	if (dialogueTypeProgress < 1.0f)
	{
		dialogueTypeProgress += dialogueTypeSpeed * dt;
		if (dialogueTypeProgress > 1.0f)
			dialogueTypeProgress = 1.0f;
	}

	if (IsPointerPressed())
	{
		if (dialogueTypeProgress >= 0.95f)
		{
			dialogueIndex++;
			if (dialogueIndex >= currentDialogue->Count)
			{
				TweenManager_AddFloatFrom(&dialogueTween, 1, 0, 0.5f, EASING_EASEINQUAD, "Dialogue", OnDialogueHide);
			}
			else
			{
				UpdateDialogue();
			}
		}
		else
		{
			dialogueTypeProgress = 5.0f;
		}
	}
}

void DialogueRender(float dt)
{
	DrawRectangle(0, 0, VIRTUAL_WIDTH, 120, (Color){10, 10, 10, 150 * dialogueTween});

	char typed[1024];
	GetTypedText(displayMessage, dialogueTypeProgress, typed, sizeof(typed));

	const char *dis = TextFormat("[c00FF00FF](%s)[r] %s", Characters[displayCharacter].ShortName, typed);
	DrawTextStyled(DefaultFont, dis, (Vector2){12, 12}, 35, 0, (Color){255, 255, 255, 255 * dialogueTween});
}

Character Characters[CHARACTER_COUNT] = {
	{.CharacterID = 0, .Name = "Hanaoka Hibiki", .ShortName = "Hibiki"},
	{.CharacterID = 1, .Name = "Matsuriya Riko", .ShortName = "Riko"},
};

static const Dialogue tutorial_start_lines[] = {
	{.CharacterID = 1, .Message = "Hibiki, you got a minute?\nIt's two days before the event, but wanna test out the\nfireworks dodgeball?"},
	{.CharacterID = 0, .Message = "Huh, already!? I haven't even finished hearing all\nthe rules yet!"},
	{.CharacterID = 1, .Message = "It's fiiine, it's fiiine! Even if you get hit,\nit's totally safety-approved!"},
	{.CharacterID = 1, .Message = "They just flash really bright!"},
	{.CharacterID = 0, .Message = "The way you said 'just' is not reassuring at all..."},
	{.CharacterID = 1, .Message = "Well, if it does seem dangerous, as staff I'll put a\nstop to it!\n...Probably! Uh, I'll try my best?"},
	{.CharacterID = 0, .Message = "Can we ban words like 'probably' and 'try' from\nstaff vocabulary...?"},
	{.CharacterID = 1, .Message = "You'll be fine! With your footwork, you can\ndodge fireworks and flower petals alike!"},
	{.CharacterID = 1, .Message = "Let's practice here so you can be the coolest\n'Firework Dodger' at the festival!"},
	{.CharacterID = 0, .Message = "...If you put it like that, I guess I have no\nchoice."},
	{.CharacterID = 0, .Message = "All right, Miss Matsuriya, you'd better watch\nclosely, okay? Safety check and hype!"},
	{.CharacterID = 1, .Message = "Leave it to me!"},
	{.CharacterID = 1, .Message = "Safety check, good!"},
	{.CharacterID = 1, .Message = "Hype level, good!"},
	{.CharacterID = 1, .Message = "Then... Hanabisaki: Flowerburst tutorial...\nSTART!"}};

static const Dialogue tutorial_end_lines[] = {
	{.CharacterID = 0, .Message = "Haa... haa..."},
	{.CharacterID = 0, .Message = "This might be... more manageable than I\nthought... maybe."},
	{.CharacterID = 1, .Message = "Oooh! That's our Hibiki! You really dodged all the\nfireworks and petals so neatly!"},
	{.CharacterID = 0, .Message = "Honestly, at first I thought 'no way I can do\nthis'..."},
	{.CharacterID = 0, .Message = "but once I tried it... it's actually kind of fun...\nmaybe a little."},
	{.CharacterID = 1, .Message = "Right, right? In the real thing it'll be even more\ncolorful and over-the-top, you know!"},
	{.CharacterID = 0, .Message = "...Don't say something scary all of a sudden."},
	{.CharacterID = 0, .Message = "But... now that I've come this far,\nI'll see it through."},
};

static const Dialogue tutorial_retry_1[] = {
	{.CharacterID = 1, .Message = "Welcome back, Hibiki. Right this way to\nthe 'Courage to Try Again' lane~."},
	{.CharacterID = 0, .Message = "Don't give it weird names...\nBut, well, it's not over yet anyway."},
	{.CharacterID = 1, .Message = "Yep, that's the spirit. You can fall as many times\nas you like in practice!"},
	{.CharacterID = 1, .Message = "If you nail it on festival day, it all gets wiped\nclean!"},
	{.CharacterID = 0, .Message = "Then I'll just make up for every fall when the\nfestival comes."},
	{.CharacterID = 0, .Message = "...All right. One more round, please."},
	{.CharacterID = 1, .Message = "Roger! Fireworks, ready to launch! Dodgeball\nreflexes, ready too?"},
	{.CharacterID = 0, .Message = "...Probably."},
};

static const Dialogue tutorial_retry_2[] = {
	{.CharacterID = 1, .Message = "All right, one retry coming up~. This time,\nlet's go for no mistakes, okay?"},
	{.CharacterID = 0, .Message = "Easy for you to say... I'll do it though."},
};

DialogueSet Dialogues[DIALOGUE_COUNT] =
	{
		{.ID = "tutorial-start",
		 .Lines = (Dialogue *)tutorial_start_lines,
		 .Count = arraySize(tutorial_start_lines)},
		{.ID = "tutorial-end",
		 .Lines = (Dialogue *)tutorial_end_lines,
		 .Count = arraySize(tutorial_end_lines)},
		{.ID = "tutorial-retry-1",
		 .Lines = (Dialogue *)tutorial_retry_1,
		 .Count = arraySize(tutorial_retry_1)},
		{.ID = "tutorial-retry-2",
		 .Lines = (Dialogue *)tutorial_retry_2,
		 .Count = arraySize(tutorial_retry_2)}};