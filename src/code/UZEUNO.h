// ===== LIBRARIES =====
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include <uzebox.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <sdBase.h>
#include <spiram.h>
// ===== LIBRARIES =====

// ===== USER CODE INCLUDES =====
// #include "../assets/defineSR.def"             //
#include "../assets/bg_tiles_progmem.inc"     //
#include "../assets/bg_tiles2_progmem.inc"     //
#include "../assets/sprite_tiles_progmem.inc" //
#include "../assets/patches.inc" // SOUND PATCHES
// ===== USER CODE INCLUDES =====

// ===== DEFINES =====
// -- Defined pixel values.
#define ORANGE_VALUE      bg_tiles[ (TILE_WIDTH*TILE_HEIGHT * colorsTile[2]) + 0 ]
#define YELLOW_VALUE      bg_tiles[ (TILE_WIDTH*TILE_HEIGHT * colorsTile[2]) + 1 ]
#define BLUE_VALUE        bg_tiles[ (TILE_WIDTH*TILE_HEIGHT * colorsTile[2]) + 2 ]
#define RED_VALUE         bg_tiles[ (TILE_WIDTH*TILE_HEIGHT * colorsTile[2]) + 3 ]
#define GREEN_VALUE       bg_tiles[ (TILE_WIDTH*TILE_HEIGHT * colorsTile[2]) + 4 ]
#define BLACK_VALUE       bg_tiles[ (TILE_WIDTH*TILE_HEIGHT * colorsTile[2]) + 5 ]
#define OFFBLACK_VALUE    bg_tiles[ (TILE_WIDTH*TILE_HEIGHT * colorsTile[2]) + 6 ]
// -- Defined tile values.
#define TILE_RAMTILECLEAR blueStar[2]
// Flags and settings.
// ---- NAME ------------------------------------------ PROD VALUE --------------- DEV VALUE ---------- EXPLAINATION -----------------
#define DEBUG_IGNORE_CARD_CHECK 0                    // -- DEFAULT:  0             -- 0                 -- Ignores the card validity check when selecting a card.
#define DEBUG_AUTOSTARTNEXTGAME 0                    // -- DEFAULT:  0             -- 1                 -- Makes it so that user input is NOT required to start the next game.
#define ENDOFPLAY_RESRAMTILECHK 1                    // -- DEFAULT:  1             -- 1                 -- Allows the clearAllUnusedReservedRamtiles function to be defined.
#define ENDOFPLAY_RESRAMTILECHK_ERR 1                // -- DEFAULT:  0             -- 1                 -- Indicates if the clearAllUnusedReservedRamtiles should error out or clear the ramtile.
#define DEBUG_SHOWDATA          1                    // -- DEFAULT:  0             -- 1                 -- For dev. Shows lots of debug data on the right side of the screen.
// -----
#define TOTALCARDSINDECK        108                  // -- DEFAULT:  108           -- 108               -- Should always be 108.
#define RESERVEDRAMTILES        18                   // -- DEFAULT:  18            -- 18                -- Adjusts with need.
#define DEFAULTHANDSIZE         7                    // -- DEFAULT:  7             -- 7                 -- Number of cards dealt to each player at the start of the game.
#define INITIALDEALBASESPEED    9                    // -- DEFAULT:  9             -- 50                -- (higher is faster) Speed of the initial card dealing.
#define ENDOFROUNDDEALBASESPEED 8                    // -- DEFAULT:  8             -- 9                 -- (higher is faster) Speed of the card movement at the scoring screen.
#define CARDPLAYBASESPEED       2                    // -- DEFAULT:  2             -- 9                 -- (higher is faster) Speed the cards move when being played/cancelled/drawn.
#define CARDDRAWBASESPEED       3                    // -- DEFAULT:  3             -- 3                 -- (higher is faster) Speed the cards move when being played/cancelled/drawn.
#define BASECPU_THINKTIME       20                   // -- DEFAULT:  20            -- 1                 -- Minimum time before the CPU makes its play.
#define EXTRACPU_THINKTIME      60                   // -- DEFAULT:  60            -- 1                 -- Possible additional time before the CPU makes its play.
#define CPU_AFTERPLAYDELAY      50                   // -- DEFAULT:  50            -- 1                 -- Time before the CPU ends their turn after a play.
#define ENDOFTURNDELAY          75                   // -- DEFAULT:  75            -- 200               -- (higher is slower)
#define ENDOFROUNDSTARTDELAY    200                  // -- DEFAULT:  200           -- 200               -- (higher is slower)
#define ENDOFROUNDFINISHEDDELAY 150                  // -- DEFAULT:  150           -- 200               -- (higher is slower)
#define MSGTIMEDELAY            100                  // -- DEFAULT:  100           -- 100               -- (higher is slower)
#define CPUCHOOSECOLORDELAY     25                   // -- DEFAULT:  25            -- 10                -- (higher is slower)
#define PLAYER1_DEFAULTTYPE     CPU                  // -- DEFAULT:  CPU           -- CPU               -- Player type of player 1
#define PLAYER2_DEFAULTTYPE     HUMAN                // -- DEFAULT:  HUMAN         -- CPU               -- Player type of player 2
#define PLAYER3_DEFAULTTYPE     NONE                 // -- DEFAULT:  NONE          -- CPU               -- Player type of player 3
#define PLAYER4_DEFAULTTYPE     NONE                 // -- DEFAULT:  NONE          -- CPU               -- Player type of player 4
#define DEFAULTGS1              GS_TITLE             // -- DEFAULT:  GS_TITLE      -- GS_TITLE          -- Game state 1
#define DEFAULTGS2              GS_TITLE_UZEBOX      // -- DEFAULT:  GS_TITLE_N782 -- GS_TITLE_MAINMENU -- Game state 2
// ===== DEFINES =====

// ===== STRINGS (PROGMEM) =====
// ----- MESSAGE: INFO AT GAME INIT -----
const char S_GAMENAME[]           PROGMEM = _GAMENAME                      ;
const char S_MSG1[]               PROGMEM = _MSG1                          ;
const char S_MSG2[]               PROGMEM = _MSG2                          ;
const char S_MSG3[]               PROGMEM = ""                             ;
// ----- MESSAGE: INFO AT GAME INIT -----

// ----- ERROR STRINGS -----
const char S_ERROR[]              PROGMEM = "ERROR!"                        ;
const char S_DBLSPACE[]           PROGMEM = "  "                           ;
const char S_DBLEXCLAMATION[]     PROGMEM = "!!"                           ;
const char S_RAMTILEUSAGE[]       PROGMEM = "NOT ENOUGH RSRVD RAMTILES"    ;
const char S_UNCLEAREDRESERVEDRT[] PROGMEM = "UNCLEARED RESERVED RT"        ;
const char S_INVALIDGAMESTATE1[]  PROGMEM = "INVALID GAMESTATE1 "          ;
const char S_INVALIDGAMESTATE2[]  PROGMEM = "INVALID GAMESTATE2 "          ;
// ----- ERROR STRINGS -----

// ----- MESSAGE: PLAYER TYPES (OPTIONS MENU) -----
const char S_HUMAN[]              PROGMEM = "HUMAN"                        ;
const char S_CPU[]                PROGMEM = "  CPU"                        ;
const char S_NONE[]               PROGMEM = " NONE"                        ;
// ----- MESSAGE: PLAYER TYPES (OPTIONS MENU) -----

// ----- MESSAGE: FOR RANDOM SAMPLING FROM GAMEPAD -----
const char S_GAMEREADY[]          PROGMEM = "GAME READY!!"                 ;
const char S_PRESSSTART[]         PROGMEM = "PRESS START!"                 ;
// ----- MESSAGE: FOR RANDOM SAMPLING FROM GAMEPAD -----

// ----- MESSAGE: PLAYER INFO -----
const char S_P1[]                 PROGMEM = "P1"                           ;
const char S_P2[]                 PROGMEM = "P2"                           ;
const char S_P3[]                 PROGMEM = "P3"                           ;
const char S_P4[]                 PROGMEM = "P4"                           ;
const char S_CARDS[]              PROGMEM = "CARDS"                        ;
const char S_UNO[]                PROGMEM = "UNO!"                         ;
// ----- MESSAGE: PLAYER INFO -----

// ----- MESSAGE: VARIOUS -----
const char S_PLAYER[]             PROGMEM = "PLAYER"                       ;
// ----- MESSAGE: VARIOUS -----

// ----- MESSAGE: START OF ROUND -----
const char S_PLAYSFIRST[]         PROGMEM = "PLAYS FIRST!"                 ;
// ----- MESSAGE: START OF ROUND -----

// ----- MESSAGE: END OF ROUND -----
const char S_WINSROUND1[]         PROGMEM = "WINS THE"                     ;
const char S_WINSROUND2[]         PROGMEM = "ROUND!"                       ;
// ----- MESSAGE: END OF ROUND -----

// ----- MESSAGE: END OF GAME -----
const char S_WINSGAME1[]          PROGMEM = "WINS THE GAME!"               ;
const char S_CONGRATS[]           PROGMEM = "CONGRATULATIONS!"             ;
// ----- MESSAGE: END OF GAME -----

// ----- MESSAGE: END OF ROUND SCORING -----
const char S_SCORES[]             PROGMEM = "SCORES"                       ;
// ----- MESSAGE: END OF ROUND SCORING -----

// ----- MESSAGE: IN GAME PLAY -----
const char S_COULDNOTASSIGNALL1[] PROGMEM = "SOME CARDS"                   ;
const char S_COULDNOTASSIGNALL2[] PROGMEM = "COULD NOT BE"                 ;
const char S_COULDNOTASSIGNALL3[] PROGMEM = "ASSIGNED"                     ;
const char S_A_TO_PLAY[]          PROGMEM = "A:  PLAY"                     ;
const char S_A_TO_PASS[]          PROGMEM = "A:  PASS"                     ;
const char S_B_TO_CANCEL[]        PROGMEM = "B:  CANCEL"                   ;
const char S_INCORRECTYCARD[]     PROGMEM = "WRONG CARD"                   ;
const char S_CHOSEANOTHER[]       PROGMEM = "PICK ANOTHER"                 ;
const char S_DRAW2[]              PROGMEM = "DRAW TWO !"                   ;
const char S_DRAW4[]              PROGMEM = "DRAW FOUR!"                   ;
const char S_SKIP[]               PROGMEM = "SKIP !"                       ;
const char S_LOSETURN[]           PROGMEM = "LOSE TURN!"                   ;
const char S_REVERSE1[]           PROGMEM = "PLAY ORDER"                   ;
const char S_REVERSE2[]           PROGMEM = "REVERSED !"                   ;
const char S_CHOOSECOLOR[]        PROGMEM = "CHOOSE COLOR"                 ;
const char S_PRESSA[]             PROGMEM = "PRESS A"                      ;
const char S_PAUSEMENU[]          PROGMEM = "PAUSE MENU"                   ;
const char S_BCANCEL_ASET[]       PROGMEM = "B:CANCEL-A:SET"               ;
// ----- MESSAGE: IN GAME PLAY -----

// ----- MESSAGE: IN PAUSE MENU -----
const char S_RESETROUND[]         PROGMEM = "RESET ROUND"                  ;
const char S_EXITGAME[]           PROGMEM = "EXIT GAME"                    ;
const char S_AUTOPLAY[]           PROGMEM = "AUTO PLAY"                    ;
const char S_NORMALPLAY[]         PROGMEM = "NORMAL PLAY"                  ;
const char S_CANCEL[]             PROGMEM = "CANCEL"                       ;
// ----- MESSAGE: IN PAUSE MENU -----
// ===== STRINGS (PROGMEM) =====

// ===== POSITIONS (PROGMEM) =====
const u8 p1_pos[][2]           PROGMEM = { {7 ,24},{10,24},{13,24},{16,24},{19,24} }; // CARDS
const u8 p2_pos[][2]           PROGMEM = { {1 ,7 },{1 ,10},{1 ,13},{1 ,16},{1 ,19} }; // CARDS
const u8 p3_pos[][2]           PROGMEM = { {7 ,1 },{10,1 },{13,1 },{16,1 },{19,1 } }; // CARDS
const u8 p4_pos[][2]           PROGMEM = { {24,7 },{24,10},{24,13},{24,16},{24,19} }; // CARDS
const u8 p1_pos_cursor[][2]    PROGMEM = { {7 ,23},{10,23},{13,23},{16,23},{19,23} }; // CARD CURSORS
const u8 p2_pos_cursor[][2]    PROGMEM = { {4 ,7 },{4 ,10},{4 ,13},{4 ,16},{4 ,19} }; // CARD CURSORS
const u8 p3_pos_cursor[][2]    PROGMEM = { {7 ,4 },{10,4 },{13,4 },{16,4 },{19,4 } }; // CARD CURSORS
const u8 p4_pos_cursor[][2]    PROGMEM = { {23,7 },{23,10},{23,13},{23,16},{23,19} }; // CARD CURSORS
const u8 wild_pos_cursor[][2]  PROGMEM = { {9 ,15},{12,15},{15,15},{18,15},};         // CURSORS IN WILD MENU
const u8 pause_pos_cursor[][2] PROGMEM = { {8 ,12},{8 ,13},{8 ,14},{8 ,15},};         // CURSORS IN PAUSE MENU
const u8 draw_pos[2]           PROGMEM =   {10,11};                                   // Draw Pile
const u8 draw_pos_below[2]     PROGMEM =   {10,15};                                   // Under Draw Pile
const u8 discard_pos[2]        PROGMEM =   {15,11};                                   // Discard Pile
const u8 discard_pos_below[2]  PROGMEM =   {15,15};                                   // Under Discard Pile
// ===== POSITIONS (PROGMEM) =====

// ===== EXTERNAL GLOBALS VARIABLES =====
extern u8           ram_tiles[]          ; // Kernel: Ram tile array.
extern u8           vram[]               ; // Kernel: Video ram array.
extern u8           masterVolume         ; // Kernel: Holds the current volume value.
extern volatile u16 joypad1_status_lo    ; // Kernel: Gamepad 1
extern volatile u16 joypad2_status_lo    ; // Kernel: Gamepad 2
extern u8           user_ram_tiles_c_tmp ; // Kernel: Value set by SetUserRamTilesCount.
extern u8           free_tile_index      ; // Kernel: Next available ram tile index (used by the sprite blitter.)
// ===== EXTERNAL GLOBALS VARIABLES =====


// ===== ? =====
// ===== ? =====

// ===== ENUMS =====
enum playerTypes          {
	NONE  ,
	HUMAN ,
	CPU   ,
};
enum winTypes             {
	FIRSTTO500POINTS = 0 ,
	FIRSTTO0CARDS    = 1 ,
};
enum drawTypes            {
	DRAW1          = 0 ,
	DRAWREPEATEDLY = 1 ,
};
enum errorTypes           {
	RAMTILEUSAGE        ,
	UNCLEAREDRESERVEDRT ,
	INVALIDGAMESTATE1   ,
	INVALIDGAMESTATE2   ,
};
enum msgTypes             {
	COULDNOTASSIGNALL ,
	GAMESTART_FIRST   ,
	DRAW2_PLAYER      ,
	DRAW4_PLAYER      ,
	SKIP_PLAYER       ,
	REVERSE_PLAYER    ,
	INCORRECTCARD     ,
	PLAYORCANCEL      ,
	PASSORCANCEL      ,
	ROUNDWIN_PLAYER   ,
	GAMEWIN_PLAYER    ,
};
enum gamestates1          {
	GS_TITLE   ,
	GS_OPTIONS ,
	GS_RULES   ,
	GS_CREDITS ,
	GS_PLAYING ,
	GS_UZENET  ,
	GS_MUSIC   ,
};
enum gamestates2          {
	// TITLE
	GS_TITLE_N782        , // Nicksen782 logo.
	GS_TITLE_UZEBOX      , // Uzebox logo.
	GS_TITLE_MAINMENU    , // Main title screen.

	// GAME SETUP
	GS_OPTIONS_1         , // Network setup.
	GS_OPTIONS_2         , // Normal setup.

	// GAME RULES
	GS_RULES_1           , // Rules page 1
	GS_RULES_2           , // Rules page 2

	// CREDITS
	GS_CREDITS_1         , // Credits page 1
	GS_CREDITS_2         , // Credits page 2

	// PLAYING
	GS_PLAYING_GAMESTART , // New round initialization.
	GS_PLAYING_PLAYING   , // Main game play state.
	GS_PLAYING_ROUNDEND  , // .

	// MUSIC TESTS
	GS_MUSIC_SCREEN1     ,
};
enum playDirection        {
	FORWARD  ,
	BACKWARD ,
};
enum cardFaceShown        {
	CARDS_FACEDOWN ,
	CARDS_FACEUP   ,
};
enum card_sizes           {
	SMALL_CARD ,
	LARGE_CARD ,
};
enum card_location        {
	CARD_LOCATION_PLAYER1 = 0,
	CARD_LOCATION_PLAYER2 = 1,
	CARD_LOCATION_PLAYER3 = 2,
	CARD_LOCATION_PLAYER4 = 3,
	CARD_LOCATION_DRAW    = 4,
	CARD_LOCATION_DISCARD = 5,
};
enum card_values          {
	CARD_0          = 0  ,
	CARD_1          = 1  ,
	CARD_2          = 2  ,
	CARD_3          = 3  ,
	CARD_4          = 4  ,
	CARD_5          = 5  ,
	CARD_6          = 6  ,
	CARD_7          = 7  ,
	CARD_8          = 8  ,
	CARD_9          = 9  ,
	CARD_DRAW2      = 10 ,
	CARD_SKIP       = 11 ,
	CARD_REV        = 12 ,
	CARD_WILD       = 13 ,
	CARD_WILD_DRAW4 = 14 ,
};
enum card_colors          {
	CARD_YELLOW ,
	CARD_BLUE   ,
	CARD_RED    ,
	CARD_GREEN  ,
	CARD_BLACK  ,
	CARD_ORANGE ,
};
enum soundSFXs            {
	SELECTCURSOR1 ,
	SELECTCARD    ,
	CANCELCARD    ,
	DRAWCARD      ,
	CARDPLAYED    ,
};
enum NPN_nextPlayerNumber {
	NPN_GET  ,
	NPN_SET  ,
	NPN_NEXT ,
};
enum SNC_choices          {
	SNC_PLAYERCHOICE  ,
	SNC_CPUAUTOCHOICE ,
};
// ===== ENUMS =====

// ===== STRUCTS =====
struct cardData_ {
	u8 player     ; // Player.
	u8 tile_x     ; // Start tile x.
	u8 tile_y     ; // Start tile y.
	u8 dst_x      ; // Destination tile x.
	u8 dst_y      ; // Destination tile y.
	u8 cardSize   ; // Card size: SMALL_CARD, LARGE_CARD
	u8 value      ; // Value of the card.
	u8 color      ; // Color of the card.
	u8 w          ;
	u8 h          ;
	u8 *map       ;
	u8 fillTile   ;
	// pgm_read_byte(&(offBlackTile[2]))
	// pgm_read_byte(&(wood_color2[2]))
	// pgm_read_byte(&(blackTile[2]))
}; // __attribute__ ((__packed__))
struct counts_   {
	u8 wild             ;
	u8 wildd4           ;
	u8 black            ;
	u8 yellow           ;
	u8 blue             ;
	u8 red              ;
	u8 green            ;
	u8 sameDiscardColor ;
	u8 sameDiscardValue ;
	u8 hasMostColorOf   ;
	u8 numCardsInHand   ;
};   // __attribute__ ((__packed__))
struct cards_    {
	// (10 bits, takes 2 bytes per entry.)
	enum card_location location :3 ; // 6/7 values.  Can change the value.
	enum card_values   value    :4 ; // 15/16 values. Never changes value.
	enum card_colors   color    :3 ; // 6/7 values.  Never changes value.
};    // __attribute__ ((__packed__))
struct game_     {
	enum gamestates1   gamestate1      ;
	enum gamestates2   gamestate2      ;
	enum playDirection direction       ;
	enum playerTypes   player_types[4] ;
	enum playerTypes   ai_type         ;
	enum winTypes      winStyle        ;
	enum drawTypes     drawStyle       ;

	u8 numPlayers                         ; // 1, 2, 3, 4.
	u8 activePlayers[4]                   ; // FLAGS: index 0 is player 1, index 1 is player 2, etc.
	u8 activePlayer                       ; // Currently active player. (1, 2, 3, 4)

	u8 handRow                            ; // Stores the displayed row of cards for the player (5 cards at a time.)

	u8 ramtile_ids_used[RESERVEDRAMTILES] ; // Array containing ramtile id reservations. (1 or 0 for availability.)
	u8 playerVisibleHand[5]               ; // Active player's visible hand. (Values are indexes into cards array struct.)
	u8 lastCardPlayed                     ; // Card at the top of the Discard Pile. (Index into cards array struct.)

	u16 pointsForWin                      ; // Points required to win the game. (Default is 500.)
	u16 points_p1                         ; // A win is reached when points get to pointsForWin.
	u16 points_p2                         ; // A win is reached when points get to pointsForWin.
	u16 points_p3                         ; // A win is reached when points get to pointsForWin.
	u16 points_p4                         ; // A win is reached when points get to pointsForWin.

	// Stores the game pad state: player 1
	u16 btnHeld1                          ; // Buttons held     on the last gamepad read.
	u16 btnPressed1                       ; // Buttons pressed  on the last gamepad read.
	u16 btnPrev1                          ; // Buttons held     on the previous to last gamepad read.
	u16 btnReleased1                      ; // Buttons released on the last gamepad read.

	u16 randomSeed                        ; // Stores the randomly generated number used for the random seed.

	u8  playerArrowFrame                  ; // Stores the frame of the player arrow (not the cursor.)

	u8  buttonPresses                     ; // DEBUG.
	u8  autoPlay                          ; // DEBUG.
	u8  handsPlayed                       ; // DEBUG.
	u8  mostUsedRamtiles                  ; // DEBUG.

};     // __attribute__ ((__packed__))
// ===== STRUCTS =====

// ===== GLOBAL VARIABLES =====
// ----- DEBUG -----
// ----- DEBUG -----

// ----- HOLDS THE CURRENT START POSITION FOR THE SCROLLABLE VIEW. -----
// u8 p1_card_scroll_pos=0;
// u8 p2_card_scroll_pos=0;
// u8 p3_card_scroll_pos=0;
// u8 p4_card_scroll_pos=0;
// ----- HOLDS THE CURRENT START POSITION FOR THE SCROLLABLE VIEW. -----

// ----- COUNTERS/TIMERS -----
u16 vsynccounter16b_1   = 0;
u8  vsynccounter8b_gen1 = 0;
u8  vsynccounter8b_gen2 = 0;
u8  vsynccounter8b_gen3 = 0;
// ----- COUNTERS/TIMERS -----

// ----- GLOBAL STRUCT INSTANCES -----
struct counts_ counts ;
struct game_   game ;
struct cards_  cards[TOTALCARDSINDECK] = {
	// WILD
	{ .location=CARD_LOCATION_DRAW , .value=CARD_WILD       , .color=CARD_BLACK } , // index 0
	{ .location=CARD_LOCATION_DRAW , .value=CARD_WILD       , .color=CARD_BLACK } , // index 1
	{ .location=CARD_LOCATION_DRAW , .value=CARD_WILD       , .color=CARD_BLACK } , // index 2
	{ .location=CARD_LOCATION_DRAW , .value=CARD_WILD       , .color=CARD_BLACK } , // index 3

	// WILD DRAW FOUR
	{ .location=CARD_LOCATION_DRAW , .value=CARD_WILD_DRAW4 , .color=CARD_BLACK } , // index 4
	{ .location=CARD_LOCATION_DRAW , .value=CARD_WILD_DRAW4 , .color=CARD_BLACK } , // index 5
	{ .location=CARD_LOCATION_DRAW , .value=CARD_WILD_DRAW4 , .color=CARD_BLACK } , // index 6
	{ .location=CARD_LOCATION_DRAW , .value=CARD_WILD_DRAW4 , .color=CARD_BLACK } , // index 7

	// YELLOW
	{ .location=CARD_LOCATION_DRAW , .value=CARD_0     , .color=CARD_YELLOW } , // index 8
	{ .location=CARD_LOCATION_DRAW , .value=CARD_1     , .color=CARD_YELLOW } , // index 9
	{ .location=CARD_LOCATION_DRAW , .value=CARD_1     , .color=CARD_YELLOW } , // index 10
	{ .location=CARD_LOCATION_DRAW , .value=CARD_2     , .color=CARD_YELLOW } , // index 11
	{ .location=CARD_LOCATION_DRAW , .value=CARD_2     , .color=CARD_YELLOW } , // index 12
	{ .location=CARD_LOCATION_DRAW , .value=CARD_3     , .color=CARD_YELLOW } , // index 13
	{ .location=CARD_LOCATION_DRAW , .value=CARD_3     , .color=CARD_YELLOW } , // index 14
	{ .location=CARD_LOCATION_DRAW , .value=CARD_4     , .color=CARD_YELLOW } , // index 15
	{ .location=CARD_LOCATION_DRAW , .value=CARD_4     , .color=CARD_YELLOW } , // index 16
	{ .location=CARD_LOCATION_DRAW , .value=CARD_5     , .color=CARD_YELLOW } , // index 17
	{ .location=CARD_LOCATION_DRAW , .value=CARD_5     , .color=CARD_YELLOW } , // index 18
	{ .location=CARD_LOCATION_DRAW , .value=CARD_6     , .color=CARD_YELLOW } , // index 19
	{ .location=CARD_LOCATION_DRAW , .value=CARD_6     , .color=CARD_YELLOW } , // index 20
	{ .location=CARD_LOCATION_DRAW , .value=CARD_7     , .color=CARD_YELLOW } , // index 21
	{ .location=CARD_LOCATION_DRAW , .value=CARD_7     , .color=CARD_YELLOW } , // index 22
	{ .location=CARD_LOCATION_DRAW , .value=CARD_8     , .color=CARD_YELLOW } , // index 23
	{ .location=CARD_LOCATION_DRAW , .value=CARD_8     , .color=CARD_YELLOW } , // index 24
	{ .location=CARD_LOCATION_DRAW , .value=CARD_9     , .color=CARD_YELLOW } , // index 25
	{ .location=CARD_LOCATION_DRAW , .value=CARD_9     , .color=CARD_YELLOW } , // index 26
	{ .location=CARD_LOCATION_DRAW , .value=CARD_DRAW2 , .color=CARD_YELLOW } , // index 27
	{ .location=CARD_LOCATION_DRAW , .value=CARD_DRAW2 , .color=CARD_YELLOW } , // index 28
	{ .location=CARD_LOCATION_DRAW , .value=CARD_SKIP  , .color=CARD_YELLOW } , // index 29
	{ .location=CARD_LOCATION_DRAW , .value=CARD_SKIP  , .color=CARD_YELLOW } , // index 30
	{ .location=CARD_LOCATION_DRAW , .value=CARD_REV   , .color=CARD_YELLOW } , // index 31
	{ .location=CARD_LOCATION_DRAW , .value=CARD_REV   , .color=CARD_YELLOW } , // index 32

	// BLUE
	{ .location=CARD_LOCATION_DRAW , .value=CARD_0     , .color=CARD_BLUE } , // index 33
	{ .location=CARD_LOCATION_DRAW , .value=CARD_1     , .color=CARD_BLUE } , // index 34
	{ .location=CARD_LOCATION_DRAW , .value=CARD_1     , .color=CARD_BLUE } , // index 35
	{ .location=CARD_LOCATION_DRAW , .value=CARD_2     , .color=CARD_BLUE } , // index 36
	{ .location=CARD_LOCATION_DRAW , .value=CARD_2     , .color=CARD_BLUE } , // index 37
	{ .location=CARD_LOCATION_DRAW , .value=CARD_3     , .color=CARD_BLUE } , // index 38
	{ .location=CARD_LOCATION_DRAW , .value=CARD_3     , .color=CARD_BLUE } , // index 39
	{ .location=CARD_LOCATION_DRAW , .value=CARD_4     , .color=CARD_BLUE } , // index 40
	{ .location=CARD_LOCATION_DRAW , .value=CARD_4     , .color=CARD_BLUE } , // index 41
	{ .location=CARD_LOCATION_DRAW , .value=CARD_5     , .color=CARD_BLUE } , // index 42
	{ .location=CARD_LOCATION_DRAW , .value=CARD_5     , .color=CARD_BLUE } , // index 43
	{ .location=CARD_LOCATION_DRAW , .value=CARD_6     , .color=CARD_BLUE } , // index 44
	{ .location=CARD_LOCATION_DRAW , .value=CARD_6     , .color=CARD_BLUE } , // index 45
	{ .location=CARD_LOCATION_DRAW , .value=CARD_7     , .color=CARD_BLUE } , // index 46
	{ .location=CARD_LOCATION_DRAW , .value=CARD_7     , .color=CARD_BLUE } , // index 47
	{ .location=CARD_LOCATION_DRAW , .value=CARD_8     , .color=CARD_BLUE } , // index 48
	{ .location=CARD_LOCATION_DRAW , .value=CARD_8     , .color=CARD_BLUE } , // index 49
	{ .location=CARD_LOCATION_DRAW , .value=CARD_9     , .color=CARD_BLUE } , // index 50
	{ .location=CARD_LOCATION_DRAW , .value=CARD_9     , .color=CARD_BLUE } , // index 51
	{ .location=CARD_LOCATION_DRAW , .value=CARD_DRAW2 , .color=CARD_BLUE } , // index 52
	{ .location=CARD_LOCATION_DRAW , .value=CARD_DRAW2 , .color=CARD_BLUE } , // index 53
	{ .location=CARD_LOCATION_DRAW , .value=CARD_SKIP  , .color=CARD_BLUE } , // index 54
	{ .location=CARD_LOCATION_DRAW , .value=CARD_SKIP  , .color=CARD_BLUE } , // index 55
	{ .location=CARD_LOCATION_DRAW , .value=CARD_REV   , .color=CARD_BLUE } , // index 56
	{ .location=CARD_LOCATION_DRAW , .value=CARD_REV   , .color=CARD_BLUE } , // index 57

	// RED
	{ .location=CARD_LOCATION_DRAW , .value=CARD_0     , .color=CARD_RED } , // index 58
	{ .location=CARD_LOCATION_DRAW , .value=CARD_1     , .color=CARD_RED } , // index 59
	{ .location=CARD_LOCATION_DRAW , .value=CARD_1     , .color=CARD_RED } , // index 60
	{ .location=CARD_LOCATION_DRAW , .value=CARD_2     , .color=CARD_RED } , // index 61
	{ .location=CARD_LOCATION_DRAW , .value=CARD_2     , .color=CARD_RED } , // index 62
	{ .location=CARD_LOCATION_DRAW , .value=CARD_3     , .color=CARD_RED } , // index 63
	{ .location=CARD_LOCATION_DRAW , .value=CARD_3     , .color=CARD_RED } , // index 64
	{ .location=CARD_LOCATION_DRAW , .value=CARD_4     , .color=CARD_RED } , // index 65
	{ .location=CARD_LOCATION_DRAW , .value=CARD_4     , .color=CARD_RED } , // index 66
	{ .location=CARD_LOCATION_DRAW , .value=CARD_5     , .color=CARD_RED } , // index 67
	{ .location=CARD_LOCATION_DRAW , .value=CARD_5     , .color=CARD_RED } , // index 68
	{ .location=CARD_LOCATION_DRAW , .value=CARD_6     , .color=CARD_RED } , // index 69
	{ .location=CARD_LOCATION_DRAW , .value=CARD_6     , .color=CARD_RED } , // index 70
	{ .location=CARD_LOCATION_DRAW , .value=CARD_7     , .color=CARD_RED } , // index 71
	{ .location=CARD_LOCATION_DRAW , .value=CARD_7     , .color=CARD_RED } , // index 72
	{ .location=CARD_LOCATION_DRAW , .value=CARD_8     , .color=CARD_RED } , // index 73
	{ .location=CARD_LOCATION_DRAW , .value=CARD_8     , .color=CARD_RED } , // index 74
	{ .location=CARD_LOCATION_DRAW , .value=CARD_9     , .color=CARD_RED } , // index 75
	{ .location=CARD_LOCATION_DRAW , .value=CARD_9     , .color=CARD_RED } , // index 76
	{ .location=CARD_LOCATION_DRAW , .value=CARD_DRAW2 , .color=CARD_RED } , // index 77
	{ .location=CARD_LOCATION_DRAW , .value=CARD_DRAW2 , .color=CARD_RED } , // index 78
	{ .location=CARD_LOCATION_DRAW , .value=CARD_SKIP  , .color=CARD_RED } , // index 79
	{ .location=CARD_LOCATION_DRAW , .value=CARD_SKIP  , .color=CARD_RED } , // index 80
	{ .location=CARD_LOCATION_DRAW , .value=CARD_REV   , .color=CARD_RED } , // index 81
	{ .location=CARD_LOCATION_DRAW , .value=CARD_REV   , .color=CARD_RED } , // index 82

	// GREEN
	{ .location=CARD_LOCATION_DRAW , .value=CARD_0     , .color=CARD_GREEN } , // index 83
	{ .location=CARD_LOCATION_DRAW , .value=CARD_1     , .color=CARD_GREEN } , // index 84
	{ .location=CARD_LOCATION_DRAW , .value=CARD_1     , .color=CARD_GREEN } , // index 85
	{ .location=CARD_LOCATION_DRAW , .value=CARD_2     , .color=CARD_GREEN } , // index 86
	{ .location=CARD_LOCATION_DRAW , .value=CARD_2     , .color=CARD_GREEN } , // index 87
	{ .location=CARD_LOCATION_DRAW , .value=CARD_3     , .color=CARD_GREEN } , // index 88
	{ .location=CARD_LOCATION_DRAW , .value=CARD_3     , .color=CARD_GREEN } , // index 89
	{ .location=CARD_LOCATION_DRAW , .value=CARD_4     , .color=CARD_GREEN } , // index 90
	{ .location=CARD_LOCATION_DRAW , .value=CARD_4     , .color=CARD_GREEN } , // index 91
	{ .location=CARD_LOCATION_DRAW , .value=CARD_5     , .color=CARD_GREEN } , // index 92
	{ .location=CARD_LOCATION_DRAW , .value=CARD_5     , .color=CARD_GREEN } , // index 93
	{ .location=CARD_LOCATION_DRAW , .value=CARD_6     , .color=CARD_GREEN } , // index 94
	{ .location=CARD_LOCATION_DRAW , .value=CARD_6     , .color=CARD_GREEN } , // index 95
	{ .location=CARD_LOCATION_DRAW , .value=CARD_7     , .color=CARD_GREEN } , // index 96
	{ .location=CARD_LOCATION_DRAW , .value=CARD_7     , .color=CARD_GREEN } , // index 97
	{ .location=CARD_LOCATION_DRAW , .value=CARD_8     , .color=CARD_GREEN } , // index 98
	{ .location=CARD_LOCATION_DRAW , .value=CARD_8     , .color=CARD_GREEN } , // index 99
	{ .location=CARD_LOCATION_DRAW , .value=CARD_9     , .color=CARD_GREEN } , // index 100
	{ .location=CARD_LOCATION_DRAW , .value=CARD_9     , .color=CARD_GREEN } , // index 101
	{ .location=CARD_LOCATION_DRAW , .value=CARD_DRAW2 , .color=CARD_GREEN } , // index 102
	{ .location=CARD_LOCATION_DRAW , .value=CARD_DRAW2 , .color=CARD_GREEN } , // index 103
	{ .location=CARD_LOCATION_DRAW , .value=CARD_SKIP  , .color=CARD_GREEN } , // index 104
	{ .location=CARD_LOCATION_DRAW , .value=CARD_SKIP  , .color=CARD_GREEN } , // index 105
	{ .location=CARD_LOCATION_DRAW , .value=CARD_REV   , .color=CARD_GREEN } , // index 106
	{ .location=CARD_LOCATION_DRAW , .value=CARD_REV   , .color=CARD_GREEN } , // index 107
} ;
// ----- GLOBAL STRUCT INSTANCES -----
// ===== GLOBAL VARIABLES =====

// ===== PROTOTYPES =====
#include "prototypes.h"
// ===== PROTOTYPES =====




// ===== TEST FUNCTIONS =====

/*
static void rotateArray(u8 *ptr, u8 times90, u8 w, u8 h);
static void rotateArray(u8 *ptr, u8 times90, u8 w, u8 h){
}

// Clears ALL ramtile data.
static void clearAllRamtiles();
static void clearAllRamtiles(){
	// Clear ALL ramtiles.
	for(u8 i=0; i<RAM_TILES_COUNT; i+=1){
		// clearRamtile(i, BLACK_VALUE);
		// clearRamtile(i, TILE_RAMTILECLEAR);
		CopyFlashTile(TILE_RAMTILECLEAR, i);
	}
}
*/

// ===== TEST FUNCTIONS =====