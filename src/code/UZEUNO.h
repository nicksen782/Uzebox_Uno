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
const char S_PLAYER[]         PROGMEM = "PLAYER"      ;
const char S_1[]              PROGMEM = "1"           ;
const char S_2[]              PROGMEM = "2"           ;
const char S_3[]              PROGMEM = "3"           ;
const char S_4[]              PROGMEM = "4"           ;

const char S_PLAYSFIRST[]     PROGMEM = "PLAYS FIRST!"; //

const char S_DIRECTION[]      PROGMEM = "DIR"       ; //
const char S_FORWARD[]        PROGMEM = "FORWARD"   ; //
const char S_BACKWARD[]       PROGMEM = "BACKWARD"  ; //

const char S_COLOR[]          PROGMEM = "COL"       ; //
const char S_YELLOW[]         PROGMEM = "YELLOW"    ; //
const char S_BLUE[]           PROGMEM = "BLUE"      ; //
const char S_RED[]            PROGMEM = "RED"       ; //
const char S_GREEN[]          PROGMEM = "GREEN"     ; //
const char S_BLACK[]          PROGMEM = "BLACK"     ; //

const char S_UNO[]            PROGMEM = "UNO!"      ; //
const char S_WIN[]            PROGMEM = "WIN!"      ; //
const char S_LOSE[]           PROGMEM = "LOSE"      ; //
const char S_CARDS[]          PROGMEM = "CARDS"     ; //

const char S_A_TO_PLAY[]      PROGMEM = "A:  PLAY"  ; // STRING FOR SELECTING A CARD
const char S_A_TO_PASS[]      PROGMEM = "A:  PASS"  ; //
const char S_B_TO_CANCEL[]    PROGMEM = "B:  CANCEL"; // STRING FOR SELECTING A CARD

const char S_INCORRECTYCARD[] PROGMEM = "INCORRECT CARD" ; //
const char S_CHOSEANOTHER[]   PROGMEM = "CHOSE ANOTHER" ; //

const char S_WINSROUND[]      PROGMEM = "WINS ROUND!" ; //

const char S_DRAW2[]          PROGMEM = "DRAW TWO!"  ; // CARD INFO
const char S_DRAW4[]          PROGMEM = "DRAW FOUR!" ; // CARD INFO
const char S_LOSETURN[]       PROGMEM = "LOSE TURN!" ; // CARD INFO
const char S_REVERSE[]        PROGMEM = "REVERSED!" ; // CARD INFO

const char S_CHOOSECOLOR[]    PROGMEM = "CHOOSE COLOR" ; //
const char S_PRESSA[]         PROGMEM = "PRESS A" ;        //

// const char S_NEXT_PLAYER[]    PROGMEM = "NEXT PLAYER:"   ; // CARD INFO
// const char S_DRAW2[]          PROGMEM = " DRAWS 2 CARDS"  ; // CARD INFO
// const char S_DRAW4[]          PROGMEM = " DRAWS 4 CARDS"  ; // CARD INFO
// const char S_LOSESTURN[]      PROGMEM = " LOSES TURN"     ; // CARD INFO
// const char S_REVERSE[]        PROGMEM = "REVERSES ORDER" ; // CARD INFO

const char S_DBLSPACE[]       PROGMEM = "  "                          ; // ERROR STRING
const char S_DBLEXCLAMATION[] PROGMEM = "!!"                          ; // ERROR STRING
const char S_RAMTILEUSAGE[]   PROGMEM = "BAD RESERVED RAMTILES COUNT" ; // ERROR STRING
const char S_NOTENOUGHCARDS[] PROGMEM = "BAD DRAW PILE COUNT"         ; // ERROR STRING
// STRINGS (PROGMEM)

// HOLDS THE CURRENT START POSITION FOR THE SCROLLABLE VIEW.
// u8 p1_card_scroll_pos=0;
// u8 p2_card_scroll_pos=0;
// u8 p3_card_scroll_pos=0;
// u8 p4_card_scroll_pos=0;
// HOLDS THE CURRENT START POSITION FOR THE SCROLLABLE VIEW.

// POSITIONS (PROGMEM)
const u8 p1_pos[][2]          PROGMEM = { { 7    , 24   }, { 10   , 24   }, { 13   , 24   }, { 16   , 24   }, { 19   , 24   } };
const u8 p2_pos[][2]          PROGMEM = { { 1    , 7    }, { 1    , 10   }, { 1    , 13   }, { 1    , 16   }, { 1    , 19   } };
const u8 p3_pos[][2]          PROGMEM = { { 7    , 1    }, { 10   , 1    }, { 13   , 1    }, { 16   , 1    }, { 19   , 1    } };
const u8 p4_pos[][2]          PROGMEM = { { 24   , 7    }, { 24   , 10   }, { 24   , 13   }, { 24   , 16   }, { 24   , 19   } };

const u8 p1_pos_cursor[][2]   PROGMEM = { { 7    , 24-2 }, { 10   , 24-2 }, { 13   , 24-2 }, { 16   , 24-2 }, { 19   , 24-2 } };
const u8 p2_pos_cursor[][2]   PROGMEM = { { 1+4  , 7    }, { 1+4  , 10   }, { 1+4  , 13   }, { 1+4  , 16   }, { 1+4  , 19   } };
const u8 p3_pos_cursor[][2]   PROGMEM = { { 7    , 1+4  }, { 10   , 1+4  }, { 13   , 1+4  }, { 16   , 1+4  }, { 19   , 1+4  } };
const u8 p4_pos_cursor[][2]   PROGMEM = { { 24-2 , 7    }, { 24-2 , 10   }, { 24-2 , 13   }, { 24-2 , 16   }, { 24-2 , 19   } };

//
const u8 wild_pos_cursor[][2] PROGMEM = { { 9    , 16 }, { 12   , 16 }, { 15   , 16 }, { 18   , 16 }, };
//

const u8 draw_pos[2]          PROGMEM =   { 9 , 12 };
const u8 discard_pos[2]       PROGMEM =   { 16, 12 };
const u8 draw_pos_below[2]    PROGMEM =   { 9 , 16 };
const u8 discard_pos_below[2] PROGMEM =   { 16, 16 };
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
// DEFINES

// GLOBAL VARIABLES
// GLOBAL VARIABLES

// COUNTERS/TIMERS
u16 vsynccounter16b_1 = 0;
u8 vsynccounter8b_gen1    = 0;
u8 vsynccounter8b_gen2    = 0;
u8 counter8b_gen1         = 0;
// COUNTERS/TIMERS

// ENUMS
enum errorTypes {
	RAMTILEUSAGE  ,
	NOTENOUGHCARDS ,
};
enum ramtileColors   {
	// CLEAREDRAMTILECOLOR0 = 0B00000110 , // INITIAL COLOR. A color that looks like RED.
	// CLEAREDRAMTILECOLOR1 = 0B01011001 , // UNUSED.        A color that looks like slate gray.
	// CLEAREDRAMTILECOLOR2 = 0B11011011 , // ERASED COLOR.  A color that looks like violet.

	// CLEAREDRAMTILECOLOR3 = 0B11011011 , // Black.
	// CLEAREDRAMTILECOLOR4 = 0B11011011 , // Brown.

	CLEAREDRAMTILECOLOR4 = 0x09 , // Brown.

};
enum gamestates1   {
	GSTATE_TITLE1        ,
	GSTATE_TITLE2        ,
	GSTATE_TITLE_MAIN    ,
	GSTATE_PLAYING       ,
	GSTATE_UZENET_SETUP1 ,
	GSTATE_UZENET_SETUP2 ,
	GSTATE_LOCAL_PREGAME ,
};
enum playDirection   {
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
	CARD_LOCATION_DRAW    ,
	CARD_LOCATION_DISCARD ,
	CARD_LOCATION_PLAYER1 ,
	CARD_LOCATION_PLAYER2 ,
	CARD_LOCATION_PLAYER3 ,
	CARD_LOCATION_PLAYER4 ,
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
	u8 gamestate2                         ; // Secondary game state. Coin room, normal game.

	u8 numPlayers                         ; // 1, 2, 3, 4
	u8 activePlayers[4]                   ; // FLAGS: index 0 is player 1, index 1 is player 2, etc.
	u8 activePlayer                       ; // Currently active player.

	// enum card_values   discardColor       ; // :4 ;
	// enum card_colors   discardValue       ; // :3 ;
	enum playDirection direction          ;

	u8 ramtile_ids_used[reservedRamtiles] ;
	u8 playerVisibleHand[5]               ; // Active player's hand. (Indexes into cards array struct.)
	u8 lastCardPlayed                     ; // Card at the top of the Discard Pile. (Index into cards array struct.)

	u16 points_p1                         ; // A win is reached when points get to 500.
	u16 points_p2                         ; // A win is reached when points get to 500.
	u16 points_p3                         ; // A win is reached when points get to 500.
	u16 points_p4                         ; // A win is reached when points get to 500.

	// Stores the game pad state: player 1
	u16 btnHeld1                          ;
	u16 btnPressed1                       ;
	u16 btnPrev1                          ;
	u16 btnReleased1                      ;

	// Stores the game pad state: player 2
	u16 btnHeld2                          ;
	u16 btnPressed2                       ;
	u16 btnPrev2                          ;
	u16 btnReleased2                      ;

	u16 randomSeed                        ;
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

