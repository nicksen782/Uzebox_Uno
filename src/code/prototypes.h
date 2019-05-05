static void _emu_whisper(int port, u8 val);
static void clear_debug_showDebugData();
static void debug_showDebugData();
static void errorHandler(enum errorTypes error);
static void pre_VsyncCallBack();
static void updateIndividualTimers();
static void post_VsyncCallBack();
static void getInputs();
static void N782_print(int x,int y,const char *string, u8 fromRam);
static void N782_print_u8(int x,int y, u8 num);
static void N782_print_u16(int x,int y, u16 num);
static void gameInit();
static void rotateRamTile_n90(u8 (mat)[8][8]);
static void swapColors_ramtile(u8 srcColor, u8 dstColor, u8 ramtile_id);
static void clearRamtile( u8 id, u8 newColor);
static void clearAllReservedRamtiles();
static void clearAllUnusedReservedRamtiles();
static void removeCard_sm(u8 player, u8 position);
static void removeCard_lg();
static void removeUnusedRamtilesInTilemap(u8 *map);
static void returnDiscardsToDrawPile();
static void resetTheDeck();
static void setVramTilemap(u8 x, u8 y, u8 w, u8 h, u8 *map);
static void DrawMap_vramtiles(u8 x,u8 y, u8 *map);
static void drawCard( u8 playerNum, u8 x, u8 y, u8 size, u8 value, u8 color);
static void MapSprite2_nra( u8 startSprite, const char *map, u8 spriteFlags );
static void redrawUnderDrawPile();
static void redrawUnderDiscardPile();
static void returnCardsForPlayer(u8 playerNum);
static void displayCardsForPlayer(u8 playerNum, u8 startPos, u8 cardDelay);
static void removePlayerCards(u8 playerNumber);
static void clearbgmessage();
static void dealSpecifiedCard_anim(u8 playerNum, u8 cardIndex, u8 cardPos, u8 cardDelay, u8 option);
static void newGameSetup();
static void redrawMainInfoRegion();
static void get_cardDims_byPlayerAndCardPosition(u8 playerNum, u8 position, u8 *x, u8 *y);
static void updatePlayerDisplayedData();
static void playSFX(u8 patch);
static void gstate_title();
static void gstate_title_n782();
static void gstate_title_uzebox();
static void gstate_title_mainmenu();
static void gstate_options();
static void gstate_options1();
static void gstate_options2();
static void gstate_credits1();
static void gstate_credits();
static void gstate_credits2();
static void gstate_rules_1();
static void gstate_rules_2();
static void gstate_rules();
static void gstate_playing();
static void getRandomSeedViaUserInput();
static void drawFaceDownCard(u8 playerNum, u8 position);
static void displayCards_byPlayer(u8 playerNum, u8 option);
static void getCursorPos(u8 playerNum, u8 cursorIndex, u8 *x, u8 *y);
static void askUserToSetNewColor(enum SNC_choices howToChoose);
static void moveCard( struct cardData_ *cardData, u8 autoRemove, u8 autoRemoveDelay, u8 cardDelay);
static void showPoints();
static void setPoints(u8 winner);
static void unoCheck();;
static void winRound(u8 winnerOfTheRound);
static void countColorsValuesMatches(u8 playerNum);
void main() __attribute__ ((noreturn));

u8 nextPlayerNumber(u8 playerNum, enum NPN_nextPlayerNumber options);
u8 pause();
u8 getMatchingCardValue(u8 playerNum, u8 value);
u8 getMatchingCardColor(u8 playerNum, u8 color);
u8 canCardBePlayed(u8 cardIndex);
u8 tileIsDuplicateOf(u8 ramtile_id);
u8 countPlayerCards(u8 playerNum);
u8 countDrawPile();
u8 countDiscardPile();
u8 countUsedRamtile_ids();
u8 countAvailableRamtile_ids();
u8 getCardFromDrawPile(u8 playerNum, u8 howMany);
u16 getRandomNumber(int min, int max);