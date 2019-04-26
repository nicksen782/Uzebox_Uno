// LIBRARIES
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
// LIBRARIES

// USER CODE INCLUDES
// #include "../assets/defineSR.def"             //
#include "../assets/bg_tiles_progmem.inc"     //
#include "../assets/bg_tiles2_progmem.inc"     //
#include "../assets/sprite_tiles_progmem.inc" //
#include "../assets/patches.inc" // SOUND PATCHES
// USER CODE INCLUDES

// STRINGS (PROGMEM)
const char S_GAMENAME[]       PROGMEM = _GAMENAME ; // STRING AT PROGRAM START
const char S_MSG1[]           PROGMEM = _MSG1     ; // STRING AT PROGRAM START
const char S_MSG2[]           PROGMEM = _MSG2     ; // STRING AT PROGRAM START
const char S_MSG3[]           PROGMEM = ""        ; // STRING AT PROGRAM START

const char S_PRESSSTART[]     PROGMEM = "PRESS START!"; // STRING AT GAME START

const char S_P1[]             PROGMEM = "P1"          ;
const char S_P2[]             PROGMEM = "P2"          ;
const char S_P3[]             PROGMEM = "P3"          ;
const char S_P4[]             PROGMEM = "P4"          ;
const char S_UNO[]            PROGMEM = "UNO!"      ; //
const char S_WIN[]            PROGMEM = "WIN!"      ; //
const char S_LOSE[]           PROGMEM = "LOSE"      ; //
const char S_CARDS[]          PROGMEM = "CARDS"     ; //

const char S_HUMAN[]          PROGMEM = "HUMAN"     ; //
const char S_CPU[]            PROGMEM = "  CPU"     ; //
const char S_NONE[]           PROGMEM = " NONE"     ; //

const char S_GAMEREADY[]      PROGMEM = "GAME READY!!"     ; //

const char S_DBLSPACE[]           PROGMEM = "  "                          ; // ERROR STRING
const char S_DBLEXCLAMATION[]     PROGMEM = "!!"                          ; // ERROR STRING
const char S_RAMTILEUSAGE[]       PROGMEM = "NOT ENOUGH RESERVED RAMTILES"; // ERROR STRING
const char S_NOTENOUGHCARDS[]     PROGMEM = "NOT ENOUGH CARDS"            ; // ERROR STRING
const char S_INVALIDGAMESTATE1[]  PROGMEM = "INVALID GAMESTATE1 "         ; // ERROR STRING
const char S_INVALIDGAMESTATE2[]  PROGMEM = "INVALID GAMESTATE2 "         ; // ERROR STRING

const char S_PLAYER[]             PROGMEM = "PLAYER"      ; // MESSAGE STRING
const char S_COULDNOTASSIGNALL1[] PROGMEM = "SOME CARDS"  ; // MESSAGE STRING
const char S_COULDNOTASSIGNALL2[] PROGMEM = "COULD NOT BE"; // MESSAGE STRING
const char S_COULDNOTASSIGNALL3[] PROGMEM = "ASSIGNED"    ; // MESSAGE STRING
const char S_A_TO_PLAY[]          PROGMEM = "A:  PLAY"    ; // MESSAGE STRING
const char S_A_TO_PASS[]          PROGMEM = "A:  PASS"    ; // MESSAGE STRING
const char S_B_TO_CANCEL[]        PROGMEM = "B:  CANCEL"  ; // MESSAGE STRING
const char S_INCORRECTYCARD[]     PROGMEM = "WRONG CARD"  ; // MESSAGE STRING
const char S_CHOSEANOTHER[]       PROGMEM = "PICK ANOTHER"; // MESSAGE STRING
const char S_PLAYSFIRST[]         PROGMEM = "PLAYS 1ST!"  ; // MESSAGE STRING
const char S_WINSROUND1[]         PROGMEM = "WINS THE"    ; // MESSAGE STRING
const char S_WINSROUND2[]         PROGMEM = "ROUND!"      ; // MESSAGE STRING
const char S_DRAW2[]              PROGMEM = "DRAW TWO!"   ; // MESSAGE STRING
const char S_DRAW4[]              PROGMEM = "DRAW FOUR!"  ; // MESSAGE STRING
const char S_LOSETURN[]           PROGMEM = "LOSE TURN!"  ; // MESSAGE STRING
const char S_REVERSE1[]           PROGMEM = "PLAY ORDER"  ; // MESSAGE STRING
const char S_REVERSE2[]           PROGMEM = "REVERSED!"   ; // MESSAGE STRING
const char S_CHOOSECOLOR[]        PROGMEM = "CHOOSE COLOR"; // MESSAGE STRING
const char S_PRESSA[]             PROGMEM = "PRESS A"     ; // MESSAGE STRING

const char S_PAUSEMENU[]          PROGMEM = "PAUSE MENU"; // MESSAGE STRING
const char S_ASET_BCANCEL[]       PROGMEM = "B:CANCEL-A:SET"; // MESSAGE STRING
// STRINGS (PROGMEM)

// HOLDS THE CURRENT START POSITION FOR THE SCROLLABLE VIEW.
// u8 p1_card_scroll_pos=0;
// u8 p2_card_scroll_pos=0;
// u8 p3_card_scroll_pos=0;
// u8 p4_card_scroll_pos=0;
// HOLDS THE CURRENT START POSITION FOR THE SCROLLABLE VIEW.

// POSITIONS (PROGMEM)
const u8 p1_pos[][2]           PROGMEM = { { 7    , 24   }, { 10   , 24   }, { 13   , 24   }, { 16   , 24   }, { 19   , 24   } };
const u8 p2_pos[][2]           PROGMEM = { { 1    , 7    }, { 1    , 10   }, { 1    , 13   }, { 1    , 16   }, { 1    , 19   } };
const u8 p3_pos[][2]           PROGMEM = { { 7    , 1    }, { 10   , 1    }, { 13   , 1    }, { 16   , 1    }, { 19   , 1    } };
const u8 p4_pos[][2]           PROGMEM = { { 24   , 7    }, { 24   , 10   }, { 24   , 13   }, { 24   , 16   }, { 24   , 19   } };

const u8 p1_pos_cursor[][2]    PROGMEM = { { 7    , 24-1 }, { 10   , 24-1 }, { 13   , 24-1 }, { 16   , 24-1 }, { 19   , 24-1 } };
const u8 p2_pos_cursor[][2]    PROGMEM = { { 1+3  , 7    }, { 1+3  , 10   }, { 1+3  , 13   }, { 1+3  , 16   }, { 1+3  , 19   } };
const u8 p3_pos_cursor[][2]    PROGMEM = { { 7    , 1+3  }, { 10   , 1+3  }, { 13   , 1+3  }, { 16   , 1+3  }, { 19   , 1+3  } };
const u8 p4_pos_cursor[][2]    PROGMEM = { { 24-1 , 7    }, { 24-1 , 10   }, { 24-1 , 13   }, { 24-1 , 16   }, { 24-1 , 19   } };

//
const u8 wild_pos_cursor[][2]  PROGMEM = { { 9    , 16 }, { 12   , 16 }, { 15   , 16 }, { 18   , 16 }, };
//
const u8 pause_pos_cursor[][2] PROGMEM = {
	{ 8 , 12 },
	{ 8 , 13 },
	{ 8 , 14 },
	{ 8 , 15 },
};
//

const u8 draw_pos[2]           PROGMEM =   { 9+1 , 12-1 };
const u8 discard_pos[2]        PROGMEM =   { 16-1, 12-1 };
const u8 draw_pos_below[2]     PROGMEM =   { 9+1 , 16-1 };
const u8 discard_pos_below[2]  PROGMEM =   { 16-1, 16-1 };
// POSITIONS (PROGMEM)

// EXTERNAL GLOBALS VARIABLES
extern u8                    ram_tiles[]  ; // Kernel: Ram tile array.
extern u8                    vram[]               ; // Kernel: Video ram array.
extern u8                    masterVolume         ; // Kernel: Holds the current volume value.
extern volatile u16 joypad1_status_lo    ; // Kernel: Gamepad 1
extern volatile u16 joypad2_status_lo    ; // Kernel: Gamepad 2
extern u8                    user_ram_tiles_c_tmp ; // Kernel: Value set by SetUserRamTilesCount.
extern u8                    free_tile_index      ; // Kernel: Next available ram tile index (used by the sprite blitter.)
// EXTERNAL GLOBALS VARIABLES

// DEFINES
#define totalCardsInDeck 108
#define reservedRamtiles 16
#define defaultHandSize 7
// DEFINES

// GLOBAL VARIABLES
// GLOBAL VARIABLES

// COUNTERS/TIMERS
u16 vsynccounter16b_1   = 0;
u16 vsynccounter16b_2   = 0;
u8  vsynccounter8b_gen1 = 0;
u8  vsynccounter8b_gen2 = 0;
u8  counter8b_gen1      = 0;
// COUNTERS/TIMERS

// ENUMS
enum playerTypes    {
	NONE  ,
	HUMAN ,
	CPU   ,
};
enum AI_Types    {
	SIMPLE ,
	HARDER ,
	EXPERT ,
};
enum winTypes    {
	FIRSTTO500POINTS ,
	FIRSTTO0CARDS    ,
};
enum drawTypes    {
	DRAW1          ,
	DRAWREPEATEDLY ,
};
enum errorTypes    {
	RAMTILEUSAGE      ,
	NOTENOUGHCARDS    ,
	INVALIDGAMESTATE1 ,
	INVALIDGAMESTATE2 ,
};
enum msgTypes      {
	COULDNOTASSIGNALL ,
	GAMESTART_FIRST   ,
	DRAW2_PLAYER      ,
	DRAW4_PLAYER      ,
	SKIP_PLAYER       ,
	REVERSE_PLAYER    ,
	INCORRECTCARD     ,
	PLAYORCANCEL      ,
	PASSORCANCEL      ,
	GAMEWIN_PLAYER    ,

};
enum ramtileColors {
	// CLEAREDRAMTILECOLOR0 = 0B00000110 , // INITIAL COLOR. A color that looks like RED.
	// CLEAREDRAMTILECOLOR1 = 0B01011001 , // UNUSED.        A color that looks like slate gray.
	// CLEAREDRAMTILECOLOR2 = 0B11011011 , // ERASED COLOR.  A color that looks like violet.

	// CLEAREDRAMTILECOLOR3 = 0B11011011 , // Black.
	// CLEAREDRAMTILECOLOR4 = 0B11011011 , // Brown.

	CLEAREDRAMTILECOLOR4 = 0x09 , // Brown.

};
enum gamestates1   {
	GSTATE_TITLE   ,

	GSTATE_OPTIONS ,

	GSTATE_RULES   ,

	GSTATE_CREDITS ,

	GSTATE_PLAYING ,

	GSTATE_UZENET  ,
};
enum gamestates2   {
	GSTATE_TITLE_N782        , // Nicksen782 logo.
	GSTATE_TITLE_UZEBOX      , // Uzebox logo.
	GSTATE_TITLE_MAINMENU    , // Main title screen.

	GSTATE_OPTIONS_1         , // Network setup.
	GSTATE_OPTIONS_2         , // Normal setup.

	GSTATE_RULES_1           , // Rules page 1
	GSTATE_RULES_2           , // Rules page 2

	GSTATE_CREDITS_1         , // Credits page 1
	GSTATE_CREDITS_2         , // Credits page 2

	GSTATE_PLAYING_GAMESTART , // New round initialization.
	GSTATE_PLAYING_PLAYING   , // Main game play state.
};
enum playDirection {
	FORWARD  ,
	BACKWARD ,
};
enum cardFaceShown {
	CARDS_FACEDOWN ,
	CARDS_FACEUP   ,
	CARDS_ACTIVEPLAYERFACEUP   ,
};
enum card_sizes    {
	SMALL_CARD ,
	LARGE_CARD ,
};
enum card_location {
	CARD_LOCATION_PLAYER1 = 0,
	CARD_LOCATION_PLAYER2 = 1,
	CARD_LOCATION_PLAYER3 = 2,
	CARD_LOCATION_PLAYER4 = 3,
	CARD_LOCATION_DRAW    = 4,
	CARD_LOCATION_DISCARD = 5,
};
enum card_values   {
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
enum card_colors   {
	CARD_YELLOW = 0,
	CARD_BLUE   = 1,
	CARD_RED    = 2,
	CARD_GREEN  = 3,
	CARD_BLACK  = 4,
	CARD_ORANGE = 5,
};
enum soundSFXs     {
	SELECTCURSOR1 , //
	SELECTCARD    , //
	CANCELCARD    , //
	DRAWCARD      , //
	CARDPLAYED    , //
};
enum NPN_nextPlayerNumber {
	NPN_GET  ,
	NPN_SET  ,
	NPN_NEXT ,
};
// enum SP_showPoints   {
// 	ONLYWINNER ,
// 	ALLPLAYERS ,
// }
// ENUMS

// NOTE: Using bitfields saves ram!
struct __attribute__ ((__packed__)) cards_ {
	// (10 bits, takes 2 bytes per entry.)
	enum card_location location :3 ; // 6/7 values.  Can change the value.
	enum card_values   value    :4 ; // 15/16 values. Never changes value.
	enum card_colors   color    :3 ; // 6/7 values.  Never changes value.
};
struct __attribute__ ((__packed__)) game_ {
	enum gamestates1 gamestate1           ; //
	enum gamestates2 gamestate2           ; //

	u8 numPlayers                         ; // 1, 2, 3, 4
	u8 activePlayers[4]                   ; // FLAGS: index 0 is player 1, index 1 is player 2, etc.
	u8 activePlayer                       ; // Currently active player.

	u8 handRow ;

	enum playDirection direction          ;

	u8 ramtile_ids_used[reservedRamtiles] ; // Array containing ramtile id reservations. (1 or 0 for availability.)
	u8 playerVisibleHand[5]               ; // Active player's hand. (Indexes into cards array struct.)
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

	// Stores the game pad state: player 2
	// u16 btnHeld2                          ; // Buttons held     on the last gamepad read.
	// u16 btnPressed2                       ; // Buttons pressed  on the last gamepad read.
	// u16 btnPrev2                          ; // Buttons held     on the previous to last gamepad read.
	// u16 btnReleased2                      ; // Buttons released on the last gamepad read.

	u16 randomSeed                        ; // Stores the randomly generated number used for the random seed.

	enum playerTypes player_types[4];
	enum playerTypes ai_type;
	enum winTypes    winStyle;
	enum drawTypes   drawStyle;
};
struct game_ game ;

// struct cards_ cards[totalCardsInDeck];
struct cards_ cards[totalCardsInDeck] = {
	// WILDS
	{ .location=CARD_LOCATION_DRAW , .value=CARD_WILD       , .color=CARD_BLACK } , // index 0
	{ .location=CARD_LOCATION_DRAW , .value=CARD_WILD       , .color=CARD_BLACK } , // index 1
	{ .location=CARD_LOCATION_DRAW , .value=CARD_WILD       , .color=CARD_BLACK } , // index 2
	{ .location=CARD_LOCATION_DRAW , .value=CARD_WILD       , .color=CARD_BLACK } , // index 3
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

// PROTOTYPES
#include "prototypes.h"
// PROTOTYPES

