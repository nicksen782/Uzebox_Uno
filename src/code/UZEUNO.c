#include "UZEUNO.h"

// DEBUG
// Send 8-bit value to CUzeBox whisper port.
static void _emu_whisper(int port, u8 val) {
	if(port==0x39 || port == 0){ u8 volatile * const _whisper_pointer1 = (u8 *) 0x39; *_whisper_pointer1 = val; }
	if(port==0x3A || port == 1){ u8 volatile * const _whisper_pointer2 = (u8 *) 0x3A; *_whisper_pointer2 = val; }
}
// Clears the debug data (on the right side.)
static void clear_debug_showDebugData(){
#if VRAM_TILES_H == 32
	// Clear the debug section.

	u8 xx_start=28;
	u8 inrow=4;

	// u8 yellow_border = pgm_read_byte(&(border_yellow[2]));
	// u8 blue_border   = pgm_read_byte(&(border_blue  [2]));
	// u8 red_border    = pgm_read_byte(&(border_red   [2]));
	// u8 green_border  = pgm_read_byte(&(border_green [2]));
	// u8 black_border  = pgm_read_byte(&(border_black [2]));

	// u8 color_orange  = pgm_read_byte(&(orange_color [2]));
	u8 color_blue  = pgm_read_byte(&(blue_color [2]));

	Fill(xx_start,0, inrow, VRAM_TILES_V, color_blue);
#endif
}
// Shows debug data (on the right side.)
static void debug_showDebugData(){
#if VRAM_TILES_H == 32

	u8 xx_start=28;
	u8 inrow=4;

	u8 xx=xx_start;
	u8 yy=0;

	clear_debug_showDebugData();

	// Ram tile reservations.
	for(u8 i=0; i<reservedRamtiles-0; i+=1){
		// Go down to the next row?
		if(!(i%inrow) && i!=0) {
			xx=xx_start;
			yy++;
			// yy++;
		}

		// // Is this ram tile in use in the queue?
		// if(game.ramtile_ids_used[i] ==1) { N782_print(xx-2, yy, PSTR("1"), 0); }
		// // No?
		// else                             { N782_print(xx-2, yy, PSTR("0"), 0); }

		// Draw the ram tile. (One more than "i" since 0 is the ram tile for the cursor.)
		SetRamTile( xx+0,yy+0,1+i );
		// xx++;
		xx++;

	}

	// Card counts.
	yy+=1;
	yy+=1;
	xx_start+=1;
	// Count of the cards in the Draw Pile.
	N782_print(xx_start-1,yy, PSTR("DRAW"), 0);yy++;
	N782_print_u8(xx_start-1,yy, countDrawPile()    ) ; yy++;

	// Count of the cards in the Discard Pile.
	N782_print(xx_start-1,yy, PSTR("DISC"), 0);yy++;
	N782_print_u8(xx_start-1,yy, countDiscardPile() ) ; yy++;

	// Indicates which players are active.
	N782_print(xx_start-1,yy, PSTR("ACTV"), 0);yy++;
	N782_print_u8(xx_start-3, yy  , game.activePlayers[0]);
	N782_print_u8(xx_start-2, yy  , game.activePlayers[1]);
	N782_print_u8(xx_start-1, yy  , game.activePlayers[2]);
	N782_print_u8(xx_start+0, yy  , game.activePlayers[3]);
	yy++;

	// Number of players.
	N782_print(xx_start-1,yy, PSTR("NP"), 0);
	N782_print_u8(xx_start+0, yy  , game.numPlayers); yy++;

	// Current hand row for the active player.
	N782_print(xx_start-1,yy, PSTR("HR"), 0);
	N782_print_u8(xx_start, yy  , game.handRow); yy++;

	// Card indexes for the active player's displayed cards.
	// N782_print(xx_start-1,yy, PSTR("A"), 0); N782_print_u8(xx_start,yy, game.playerVisibleHand[0] ) ; yy++;
	// N782_print(xx_start-1,yy, PSTR("B"), 0); N782_print_u8(xx_start,yy, game.playerVisibleHand[1] ) ; yy++;
	// N782_print(xx_start-1,yy, PSTR("C"), 0); N782_print_u8(xx_start,yy, game.playerVisibleHand[2] ) ; yy++;
	// N782_print(xx_start-1,yy, PSTR("D"), 0); N782_print_u8(xx_start,yy, game.playerVisibleHand[3] ) ; yy++;
	// N782_print(xx_start-1,yy, PSTR("E"), 0); N782_print_u8(xx_start,yy, game.playerVisibleHand[4] ) ; yy++;

	// Card index for last discarded card.
	N782_print(xx_start-1,yy, PSTR("LAST"), 0);yy++;
	N782_print_u8(xx_start-0,yy, game.lastCardPlayed       ) ; yy++;

	// Random seed in use.
	// N782_print(xx_start-1,yy, PSTR("SEED"), 0);yy++;
	// N782_print_u16(xx_start-0,yy, game.randomSeed       ) ; yy++;

	//
#endif
}
// Error handler - Called upon reaching fatal errors.
static void errorHandler(enum errorTypes error){
	// EXAMPLE USAGE: errorHandler(RAMTILEUSAGE);
	// Intended for fatal errors.

	// Display the error text.
	switch(error){
		// If there are no more free reserved ramtiles.
		case RAMTILEUSAGE      : {
			N782_print( 0 , VRAM_TILES_V-1, S_RAMTILEUSAGE  , 0 ) ;
			break;
		}

		// If not enough cards are available.
		case NOTENOUGHCARDS    : {
			N782_print( 0 , VRAM_TILES_V-1, S_NOTENOUGHCARDS, 0 ) ;
			break;
		}

		// INVALID GAMESTATE 1.
		case INVALIDGAMESTATE1    : {
			N782_print( 0 , VRAM_TILES_V-1, S_INVALIDGAMESTATE1, 0 ) ;
			break;
		}

		// INVALID GAMESTATE 2.
		case INVALIDGAMESTATE2    : {
			N782_print( 0 , VRAM_TILES_V-1, S_INVALIDGAMESTATE2, 0 ) ;
			break;
		}

		default : { break; }
	}

	// Update the debug data.
	debug_showDebugData();

	// Blink the "!!" and do not let the program continue.
	while(1){
		for(u8 i=0; i<5; i+=1){
			N782_print( 0 , VRAM_TILES_V-2, S_DBLSPACE, 0 ) ;
			WaitVsync(20);
			N782_print( 0 , VRAM_TILES_V-2, S_DBLEXCLAMATION, 0 ) ;
			WaitVsync(20);
		}

		// WaitVsync(300); // 5 seconds
		WaitVsync(240); // 4 seconds
		// WaitVsync(180); // 3 seconds
		// WaitVsync(120); // 2 seconds
		// WaitVsync(60);  // 1 second

		SoftReset();
		// WaitVsync(1);
	}
}
// DEBUG

// LOW LEVEL - VSYNC COUNTERS
// Code that is run right before vsync.
static void pre_VsyncCallBack(){
}
// Increments timers (run from post vsync.)
static void updateIndividualTimers(){
	vsynccounter16b_1   ++ ;
	vsynccounter16b_2   ++ ;
	vsynccounter8b_gen1 ++ ;
	vsynccounter8b_gen2 ++ ;

	// vsynccounter8b_gen3 ++ ;
	// vsynccounter8b_gen4 ++ ;
	// counter8b_gen1      ++ ;
}
// Code that is run right after vsync.
static void post_VsyncCallBack(){
	updateIndividualTimers();
}
// Get gamepad inputs and store them in the game struct.
static void getInputs(){
	// Reads input as a combining ( 'OR' ) of gamepad 1 and 2.
	// Either gamepad can be used to control the game.

	// Get local inputs.
	ReadControllers();

	u16 inputs = joypad1_status_lo | joypad2_status_lo ;

	// Gamepad #1 and #2
	game.btnPrev1     = game.btnHeld1;
	game.btnHeld1     = inputs ; // joypad1_status_lo;
	game.btnPressed1  = game.btnHeld1 & (game.btnHeld1 ^ game.btnPrev1);
	game.btnReleased1 = game.btnPrev1 & (game.btnHeld1 ^ game.btnPrev1);

	// -------------
	// Gamepad #1
	// game.btnPrev1     = game.btnHeld1;
	// game.btnHeld1     = joypad1_status_lo;
	// game.btnPressed1  = game.btnHeld1 & (game.btnHeld1 ^ game.btnPrev1);
	// game.btnReleased1 = game.btnPrev1 & (game.btnHeld1 ^ game.btnPrev1);

	// Gamepad #2 - Separate gamepad.
	// game.btnPrev2     = game.btnHeld12
	// game.btnHeld2     = joypad2_status_lo;
	// game.btnPressed2  = game.btnHeld2 & (game.btnHeld2 ^ game.btnPrev2);
	// game.btnReleased2 = game.btnPrev2 & (game.btnHeld2 ^ game.btnPrev2);

	// Gamepad #2 - Set to the same inputs as gamepad #1
	// game.btnPrev2     = game.btnHeld1;
	// game.btnHeld2     = joypad1_status_lo;
	// game.btnPressed2  = game.btnHeld1 & (game.btnHeld1 ^ game.btnPrev1);
	// game.btnReleased2 = game.btnPrev1 & (game.btnHeld1 ^ game.btnPrev1);

	// Either gamepad is stored as gamepad #1.
	// if( joypad1_status_lo ){
	// 	game.btnPrev1     = game.btnHeld1;
	// 	game.btnHeld1     = joypad1_status_lo;
	// 	game.btnPressed1  = game.btnHeld1 & (game.btnHeld1 ^ game.btnPrev1);
	// 	game.btnReleased1 = game.btnPrev1 & (game.btnHeld1 ^ game.btnPrev1);
	// }
	// else if( joypad2_status_lo ) {
	// 	game.btnPrev1     = game.btnHeld2;
	// 	game.btnHeld1     = joypad2_status_lo;
	// 	game.btnPressed1  = game.btnHeld2 & (game.btnHeld2 ^ game.btnPrev2);
	// 	game.btnReleased1 = game.btnPrev2 & (game.btnHeld2 ^ game.btnPrev2);
	// }
}
// LOW LEVEL - VSYNC COUNTERS

// TEXT DISPLAY
// Displays text (allows for a limited fontset.)
static void N782_print(int x,int y,const char *string, u8 fromRam){
	// This works like the kernel print function but does not require a complete fontset.
	// Can save flash space by not requiring punctuation font tiles.
	// Punctuation characters can be added as needed.
	// Your fontset needs to have at least all capital letters, numbers.
	// This function forces any lowercase letter to use uppercase fonts.
	// A string will be passed along with the start x and y pos for it to be printed to.
	// IMPORTANT: A string that is NOT "null terminated" will cause this function to loop forever.

	// EXAMPLE USAGE:
	  // STRING FROM PROGMEM:
		// const char TESTSTRING[] PROGMEM = "TEST FLASH STRING";
		// N782_print(0, 0, TESTSTRING, 0);
		// OR
		// N782_print(0, 0, PSTR("TEST FLASH STRING"), 0);
	  // STRING FROM RAM:
		// const char TESTSTRING[] = "TEST RAM STRING";
		// N782_print(0, 0, TESTSTRING, 1);
		// OR
		// N782_print(0, 0, "TEST RAM STRING", 1);

	//
	int stringPos=0;
	char thischar;
	u8 charAsTileNum;
	u8 unknownFont_index = pgm_read_byte(&(blackTile[2]));
	// u8 unknownFont = pgm_read_byte(&(offBlackTile[2]));

	// Start indexes for letters, numbers.
	u8 letters_start_index=22;    // letters_start_index is the tile index of A. 65 is the ASCII value of A. 97 is the ASCII value of a.
	u8 numbers_start_index=9 ;    // numbers_start_index is the tile index of 0. 48 is the ASCII value of 0.

	// Start indexes for punctuation tiles.
	u8 space_start_index        = 0 ;
	u8 exclamation_start_index  = 2 ;
	u8 doubleQuote_start_index  = 3 ;
	u8 rightParen_start_index   = 4 ;
	u8 leftParen_start_index    = 5 ;
	u8 plus_start_index         = 6 ;
	u8 comma_start_index        = 7 ;
	u8 period_start_index       = 8 ;
	u8 colon_start_index        = 19;
	u8 question_start_index     = 20;
	u8 at_start_index           = 21;
	u8 underscore_start_index   = 48;

	// Keep printing the string until the string terminator (NULL) is found.
	while(1){
		// Read the next character in the string.
		if(fromRam==1){ thischar=string[stringPos++]; }
		else          { thischar=pgm_read_byte(&(string[stringPos++])); }

		// Is this character a NULL?
		if(thischar == 0){
			// Yes?
			// The string is over.
			break;
		}
		// Character was not a NULL. Print the character.
		else{
			// No?
			// Get the tile for the character.

			// Uppercase.
			if     (thischar >= 'A' && thischar <= 'Z') { charAsTileNum = letters_start_index + thischar-'A'; }

			// Force lowercase to uppercase.
			else if(thischar >= 'a' && thischar <= 'z') { charAsTileNum = letters_start_index + thischar-'a'; }

			// Numbers.
			else if(thischar >= '0' && thischar <= '9') { charAsTileNum = numbers_start_index + thischar-'0'; }

			// Punctuation.
			else if( thischar==' ' ) { charAsTileNum = space_start_index       ; }
			else if( thischar=='!' ) { charAsTileNum = exclamation_start_index ; }
			else if( thischar==')' ) { charAsTileNum = doubleQuote_start_index ; }
			else if( thischar=='"' ) { charAsTileNum = rightParen_start_index  ; }
			else if( thischar=='(' ) { charAsTileNum = leftParen_start_index   ; }
			else if( thischar=='+' ) { charAsTileNum = plus_start_index        ; }
			else if( thischar==',' ) { charAsTileNum = comma_start_index       ; }
			else if( thischar=='.' ) { charAsTileNum = period_start_index      ; }
			else if( thischar==':' ) { charAsTileNum = colon_start_index       ; }
			else if( thischar=='?' ) { charAsTileNum = question_start_index    ; }
			else if( thischar=='@' ) { charAsTileNum = at_start_index          ; }
			else if( thischar=='_' ) { charAsTileNum = underscore_start_index  ; }

			// Unmatched characters. Use the unknownFont tile.
			else                     { charAsTileNum = unknownFont_index ;  }

			// Print the character to the screen.
			SetTile(x++,y+0 , charAsTileNum);
		}
	}

}
// Displays u8 number display (allows for a limited fontset.)
static void N782_print_u8(int x,int y, u8 num){
	// Can print one number, 0-9.
	// u8 thischar;
	// u8 charAsTileNum;
	// u8 unknownFont_index = pgm_read_byte(&(blackTile[2]));
	// u8 unknownFont = pgm_read_byte(&(offBlackTile[2]));

	// Start indexes for letters, numbers.
	// u8 numbers_start_index=5;  // numbers_start_index is the tile index of 0.
	u8 start_i=0;

	u8 numbers_start_index=9;  // numbers_start_index is the tile index of 0.

	u8 cols[3] = {0};
	cols[0] = ( (num/100) + 0 ) ; num %= 100;
	cols[1] = ( (num/10)  + 0 ) ; num %= 10;
	cols[2] = ( (num/1)   + 0 ) ; num %= 1;

	// No leading zeros!
	if(cols[0] == 0)                                { start_i=1; }
	if(cols[0] == 0 && cols[1] == 0)                { start_i=2; }
	// if(cols[0] == 0 && cols[1] == 0 && cols[2] == 0){ start_i=3; }

	for(uint8_t i=start_i; i<3; i++){
		SetTile(
			x+i,
			y,
			cols[i]+numbers_start_index
		);
	}
}
static void N782_print_u16(int x,int y, u16 num){
	// Can print one number, 0-9.
	// u8 thischar;
	// u8 charAsTileNum;
	// u8 unknownFont_index = pgm_read_byte(&(blackTile[2]));
	// u8 unknownFont = pgm_read_byte(&(offBlackTile[2]));

	// Start indexes for letters, numbers.
	// u8 numbers_start_index=5;  // numbers_start_index is the tile index of 0.
	u8 start_i=0;

	u8 numbers_start_index=9;  // numbers_start_index is the tile index of 0.

	u8 cols[5] = {0};
	cols[0] = ( (num/10000) + 0 ) ; num %= 10000;
	cols[1] = ( (num/1000)  + 0 ) ; num %= 1000;
	cols[2] = ( (num/100)   + 0 ) ; num %= 100;
	cols[3] = ( (num/10)    + 0 ) ; num %= 10;
	cols[4] = ( (num/1)     + 0 ) ; num %= 1;

	// No leading zeros!
	// if(cols[0] == 0)                                { start_i=1; }
	// if(cols[0] == 0 && cols[1] == 0)                { start_i=2; }
	// if(cols[0] == 0 && cols[1] == 0 && cols[2] == 0){ start_i=3; }

	for(uint8_t i=start_i; i<5; i++){
		SetTile(
			x+i,
			y,
			cols[i]+numbers_start_index
		);
	}

}
// Message handler - Central place for drawing game messages.
static void msgHandler(enum msgTypes msg){
	// EXAMPLE USAGE: msgHandler(COULDNOTASSIGNALL);
	// EXAMPLE USAGE: msgHandler(GAMESTART_FIRST);
	// Intended for non-fatal errors.

	// clearbgmessage : Fill(8,17, 12,5, replacementTile);
	// Region starts at 8,17, has width of 12 and height of 5.

	Fill(8,17, 12,5, pgm_read_byte(&(blackTile[2]   )));

	// Display the msg text.
	switch(msg){
		// If not enough cards are available.
		case COULDNOTASSIGNALL : {
			N782_print(9 ,18, S_COULDNOTASSIGNALL1, 0);
			N782_print(8 ,19, S_COULDNOTASSIGNALL2, 0);
			N782_print(10,20, S_COULDNOTASSIGNALL3, 0);
			WaitVsync(100);
		}
		case GAMESTART_FIRST   : {
			N782_print   ( 9  , 18, S_PLAYER, 0);
			N782_print_u8( 14 , 18, game.activePlayer );
			N782_print   ( 9  , 19, S_PLAYSFIRST, 0);
			break;
		}
		case DRAW2_PLAYER      : {
			N782_print   ( 9  , 18, S_PLAYER   , 0 ) ;
			N782_print_u8( 14 , 18, game.activePlayer );
			N782_print   ( 9  , 19, S_DRAW2,0);
			N782_print   ( 9  , 20, S_LOSETURN,0);
			break;
		}
		case DRAW4_PLAYER      : {
			N782_print   ( 9  , 18, S_PLAYER   , 0 ) ;
			N782_print_u8( 14 , 18, game.activePlayer );
			N782_print   ( 9  , 19, S_DRAW4,0);
			N782_print   ( 9  , 20, S_LOSETURN,0);
			break;
		}
		case SKIP_PLAYER       : {
			N782_print   ( 9  , 18, S_PLAYER   , 0 ) ;
			N782_print_u8( 14 , 18, game.activePlayer );
			N782_print   ( 9  , 19, S_LOSETURN,0);
			break;
		}
		case REVERSE_PLAYER    : {
			N782_print   ( 9  , 19, S_REVERSE1,0);
			N782_print   ( 9  , 20, S_REVERSE2,0);
			break;
		}
		case INCORRECTCARD     : {
			N782_print( 9 , 18, S_INCORRECTYCARD  , 0 ) ;
			N782_print( 8 , 19, S_CHOSEANOTHER    , 0 ) ;
			break;
		}
		case PLAYORCANCEL      : {
			N782_print( 9 , 18, S_A_TO_PLAY  , 0 ) ;
			N782_print( 9 , 19, S_B_TO_CANCEL, 0 ) ;
			break;
		}
		case PASSORCANCEL      : {
			N782_print( 9 , 18, S_A_TO_PASS   , 0 ) ;
			N782_print( 9 , 19, S_B_TO_CANCEL , 0 ) ;
			break;
		}
		case GAMEWIN_PLAYER    : {
			N782_print   ( 10  , 18, S_PLAYER, 0);
			N782_print_u8( 15  , 18, game.activePlayer );
			N782_print   ( 10  , 19, S_WINSROUND1 , 0);
			N782_print   ( 11  , 20, S_WINSROUND2 , 0);
			break;
		}

		default : { break; }
	}

	// Update the debug data.
	debug_showDebugData();
}
// TEXT DISPLAY

// INIT
static void getRandomSeedViaUserInput(){
	// Get a random source: Use the time it takes for the user to press and then release the button.
	vsynccounter8b_gen1=0;
	u8 frame=0;
	u8 x=(VRAM_TILES_H/2) - (12/2); // (12 is the length of the string.
	u8 y=(VRAM_TILES_V/2) - 1;
	ClearVram();

	N782_print( x, y, S_GAMEREADY , 0 ) ;

	// while(0)
	while(1)
	{
		getInputs();

		if     (vsynccounter8b_gen1<30){ frame=1; }
		else if(vsynccounter8b_gen1<40){ frame=2; }
		else                           { frame=1; vsynccounter8b_gen1=0; }

		// Tell the user to press START (blink this text.)
		if     (frame==1){ N782_print( x, y+2, S_PRESSSTART , 0 ) ; }
		else if(frame==2){ Fill(x,y+2, 12,1, 0x00); }

		// Did the user hold start?
		if( game.btnPressed1 ){
			// Reset the 8bit counter.
			vsynccounter8b_gen1=0;

			// Wait for the user to release all buttons.
			while(game.btnHeld1){ getInputs(); }

			// Generate a random number that uses the timer values.
			game.randomSeed = ( (vsynccounter16b_1 * vsynccounter8b_gen1) );

#if VRAM_TILES_H == 32
			// N782_print_u16(x,y+4, game.randomSeed);
			// N782_print_u16(x,y+5, vsynccounter16b_1);
			// N782_print_u16(x,y+6, vsynccounter8b_gen1);

			// // While no button is held...
			// while ( game.btnHeld1==0 ){ getInputs(); }
#endif
			// End the loop.
			break;
		}

		// Keep in sync while waiting for the user to hold a button then release it.
		WaitVsync(1);
	}
}
// Run once at program start. (SETUP.)
static void gameInit(){
	// System init.
	SetTileTable( bg_tiles );
	// for(u8 i=0;i<MAX_SPRITES; i++){sprites[i].x = OFF_SCREEN; }
	// memset(&ram_tiles, 0, RAM_TILES_COUNT*64);
	ClearVram();

	SetSpritesTileBank(0 , sprite_tiles   ); // 0 -- SPRITE_BANK0
	SetSpritesTileBank(1 , bg_tiles   );     // 1 -- SPRITE_BANK1

	// Init the music player.
	InitMusicPlayer(patches);
	SetMasterVolume(50);

	// Show the compile date.
	WaitVsync(50);
	N782_print( 0, 0, S_GAMENAME , 0 ) ;
	N782_print( 0, 1, S_MSG1 , 0) ; N782_print( 6, 1, S_MSG2 , 0) ;
	N782_print( 0, 7, S_MSG3 , 0) ;
	WaitVsync(50);
	ClearVram();

	// InitMusicPlayer(YourPatches);
	SetFontTilesIndex(1);
	SetUserRamTilesCount(reservedRamtiles+1); // Reserved ramtiles plus the compare.

	// Init the inital game settings.
	game.numPlayers    =  2                         ;
	game.activePlayers == game.numPlayers>0 ? 1 : 0 ;
	game.activePlayers == game.numPlayers>1 ? 1 : 0 ;
	game.activePlayers == game.numPlayers>2 ? 1 : 0 ;
	game.activePlayers == game.numPlayers>3 ? 1 : 0 ;
	game.activePlayer   = 255                       ;
	game.points_p1      = 0                         ;
	game.points_p2      = 0                         ;
	game.points_p3      = 0                         ;
	game.points_p4      = 0                         ;
	game.lastCardPlayed = 255                       ;
	game.direction      = FORWARD                   ;

	game.player_types[0]= HUMAN                     ;
	game.player_types[1]= CPU                       ;
	game.player_types[2]= NONE                      ;
	game.player_types[3]= NONE                      ;

	game.ai_type        = SIMPLE                    ;
	game.winStyle       = FIRSTTO500POINTS          ;
	game.drawStyle      = DRAW1                     ;

	// Normal starting gamestates.
	game.gamestate1   = GSTATE_TITLE      ;
	// game.gamestate2   = GSTATE_TITLE_N782 ;
	game.gamestate2   = GSTATE_TITLE_UZEBOX ;
	// game.gamestate2   = GSTATE_TITLE_MAINMENU ;

	// DEBUG starting gamestates.
	// game.gamestate1   = GSTATE_PLAYING      ;
	// game.gamestate2   = GSTATE_PLAYING_GAMESTART ;

	// while(true){
	// 	WaitVsync(1); _emu_whisper(0,5 ); WaitVsync(1); for(u8 ii=0; ii<1;ii+=1) { TriggerFx(5 , 255, true); } WaitVsync(100);
	// 	WaitVsync(1); _emu_whisper(0,6 ); WaitVsync(1); for(u8 ii=0; ii<1;ii+=1) { TriggerFx(6 , 255, true); } WaitVsync(100);
	// 	WaitVsync(1); _emu_whisper(0,7 ); WaitVsync(1); for(u8 ii=0; ii<1;ii+=1) { TriggerFx(7 , 255, true); } WaitVsync(100);
	// 	WaitVsync(1); _emu_whisper(0,8 ); WaitVsync(1); for(u8 ii=0; ii<1;ii+=1) { TriggerFx(8 , 255, true); } WaitVsync(100);
	// 	WaitVsync(1); _emu_whisper(0,9 ); WaitVsync(1); for(u8 ii=0; ii<1;ii+=1) { TriggerFx(9 , 255, true); } WaitVsync(100);
	// }

	// Init the rest of the game settings.
	// newGameInit();

	// Set the pre/post vsync callbacks.
	SetUserPreVsyncCallback ( &pre_VsyncCallBack  ); //
	SetUserPostVsyncCallback( &post_VsyncCallBack ); //

clearAllReservedRamtiles();
	for(u8 i=0; i<5; i+=1){
		game.playerVisibleHand[i]=255;
	}

	ClearVram();

	game.randomSeed=0;

	// getRandomSeedViaUserInput();
	// N782_print_u16(VRAM_TILES_H-5,27, game.randomSeed ) ;

	// Seed the random number generator using the values of the two timers multiplied together.
	srand(game.randomSeed);
	// srand(GetTrueRandomSeed());

	// Reset counters.
	vsynccounter16b_1   = 0;
	vsynccounter8b_gen1 = 0;
}
// --------------------------
static void drawGameBoard(){
	// How many players?
	game.numPlayers;

	if     (game.numPlayers==1){

	}
	else if(game.numPlayers==2){

	}
	else if(game.numPlayers==3){}
	else if(game.numPlayers==4){}
}
// INIT

// SOUND UTILITIES
static void playSFX(u8 patch){
	u8 volume   = 128  ;
	// bool retrig = true ;
	bool retrig = false ;

	// playSFX (SELECTCURSOR1);
	// playSFX (DRAWCARD);
	// playSFX (SELECTCARD);
	// playSFX (CANCELCARD);
	// playSFX (CARDPLAYED);

	switch(patch){
		// When a card is played.
		case CARDPLAYED          : { TriggerFx(5,  volume, retrig); break ; }
		// When a card is selected.
		case SELECTCARD          : { TriggerFx(6,  volume, retrig); break ; }
		// When a selected card is cancelled.
		case CANCELCARD          : { TriggerFx(7,  volume, retrig); break ; }
		// When the card/color select cursor is moved.
		case SELECTCURSOR1       : { TriggerFx(8,  volume, retrig); break ; }
		// When a card is drawn.
		case DRAWCARD            : { TriggerFx(9,  volume, retrig); break ; }
		//
		default                  : { break ; }
	};

	// TriggerFx(unsigned char patch,unsigned char volume, bool retrig);
	// TriggerFx(patch,  volume, retrig);
}
// SOUND UTILITIES

// RAMTILE UTILITIES
// Clears all reserved ramtiles (indexes and data.)
static void clearAllReservedRamtiles(){
	// Clear the ramtiles.
	clearRamtile(0, CLEAREDRAMTILECOLOR4);
	for(u8 i=0; i<reservedRamtiles; i+=1){
		clearRamtile(i+1, CLEAREDRAMTILECOLOR4);
		game.ramtile_ids_used[i]=0;
	}
}
static void clearAllRamtiles(){
	// Clear ALL ramtiles.
	for(u8 i=0; i<RAM_TILES_COUNT; i+=1){
		clearRamtile(i, CLEAREDRAMTILECOLOR4);
	}
}

// Rotates a ramtile in place negative 90 degrees.
static void rotateRamTile_n90(u8 mat[8][8]){
	// Based on: https://www.geeksforgeeks.org/inplace-rotate-square-matrix-by-90-degrees/
	// Rotates anti-clockwise 90 degrees (so, -90 degrees.)
	// Rotates in-place with no additional ramtile needed.
	// EXAMPLE USAGE: rotateRamTile_n90(&ram_tiles[(0*64)]); // n90 rotate ramtile 0.

	u8 N = 8;

	// Consider all squares one by one
	for (u8 x = 0; x < N / 2; x++) {
		// Consider elements in group of 4 in
		// current square
		for (u8 y = x; y < N-x-1; y++) {
			// store current cell in temp variable
			u8 temp = mat[x][y];

			// move values from right to top
			mat[x][y] = mat[y][N-1-x];

			// move values from bottom to right
			mat[y][N-1-x] = mat[N-1-x][N-1-y];

			// move values from left to bottom
			mat[N-1-x][N-1-y] = mat[N-1-y][x];

			// assign temp to left
			mat[N-1-y][x] = temp;
		}
	}
}
// Replaces all pixel values of the specified color to the new specified color.
static void swapColors_ramtile(u8 srcColor, u8 dstColor, u8 ramtile_id){
	// Use these colors - from a PROGMEM tile.
	u8 yellow_value = pgm_read_byte( &( &bg_tiles[ (u16) ( (TILE_WIDTH*TILE_HEIGHT) * pgm_read_byte(&(yellow_color[2])) ) ])[0] );
	u8 blue_value   = pgm_read_byte( &( &bg_tiles[ (u16) ( (TILE_WIDTH*TILE_HEIGHT) * pgm_read_byte(&(blue_color[2]  )) ) ])[0] );
	u8 red_value    = pgm_read_byte( &( &bg_tiles[ (u16) ( (TILE_WIDTH*TILE_HEIGHT) * pgm_read_byte(&(red_color[2]   )) ) ])[0] );
	u8 green_value  = pgm_read_byte( &( &bg_tiles[ (u16) ( (TILE_WIDTH*TILE_HEIGHT) * pgm_read_byte(&(green_color[2] )) ) ])[0] );
	u8 orange_value = pgm_read_byte( &( &bg_tiles[ (u16) ( (TILE_WIDTH*TILE_HEIGHT) * pgm_read_byte(&(orange_color[2])) ) ])[0] );
	u8 black_value  = pgm_read_byte( &( &bg_tiles[ (u16) ( (TILE_WIDTH*TILE_HEIGHT) * pgm_read_byte(&(blackTile[2]   )) ) ])[0] );

	// Adjust the srcColor to the actual value.
	switch(srcColor){
		case CARD_YELLOW : { srcColor = yellow_value; break; } // Yellow
		case CARD_BLUE   : { srcColor = blue_value;   break; } // Blue
		case CARD_RED    : { srcColor = red_value;    break; } // Red
		case CARD_GREEN  : { srcColor = green_value;  break; } // Green
		case CARD_ORANGE : { srcColor = orange_value; break; } // Orange
		case CARD_BLACK  : { srcColor = black_value;  break; }
		default  : { return; break; }
	};

	// Adjust the dstColor to the actual value.
	switch(dstColor){
		case CARD_YELLOW : { dstColor = yellow_value; break; } // Yellow
		case CARD_BLUE   : { dstColor = blue_value  ; break; } // Blue
		case CARD_RED    : { dstColor = red_value   ; break; } // Red
		case CARD_GREEN  : { dstColor = green_value ; break; } // Green
		case CARD_ORANGE : { dstColor = orange_value; break; } // Orange
		case CARD_BLACK  : { dstColor = black_value ; break; }
		default  : { return; break; }
	};

	// Use a pointer (easier to read this way.)
	u8 * ramTiles_ptr ;

	// Perform the color swap.
	for(u8 i=0; i<64; i+=1){
		// Set the address of the pointer.
		ramTiles_ptr = &ram_tiles[ (ramtile_id*64) +i ];

		// Is this value the same as srcColor? If so, then change it to the dstColor.
		if( *ramTiles_ptr == srcColor ) {
			*ramTiles_ptr = dstColor;
		}
	}
}
// Clears the pixel data from a ramtile (erases the ramtile data.)
static void clearRamtile( u8 id, u8 newColor){
	// Clears the pixel data in the specified ramtile.
	// NOTE: Assumes that you've already removed the ramtile from game.ramtile_ids_used.

	// Use a pointer (easier to read this way.)
	u8 * ramTiles_ptr ;

	// Ensure that this is a ramtile before clear it.
	// if(id <= reservedRamtiles ) {
	if(id < RAM_TILES_COUNT ) {
		// Set each pixel of the ramtile to the new value.
		for(u16 i=0; i<64; i+=1){
			// Set the address of the pointer.
			ramTiles_ptr = &ram_tiles[ ((id*64) +i) ];
			// Set the value for this pixel.
			*ramTiles_ptr = newColor;
		}
	}
}
// Provided a ramtile id and searches all ramtiles for an exact match.
u8 tileIsDuplicateOf(u8 ramtile_id){
	// Expected to specify ramtile 0 and to check it against all other used ramtiles.
	// Return the provided ramtile_id if the tile is NOT a duplicate.
	// (Should be 0 then since 0 not used for cards.)
	u8 leftPixel  ;
	u8 rightPixel ;
	u8 isMatch ;

	// Check rt 0 against rt 1, then 2, then 3 until the last reserved ramtile id is reached.
	for(
		u8 currentRamtileId = 1;    // Init.
		currentRamtileId<(reservedRamtiles-1); // Do while true.
		currentRamtileId+=1                    // Do at end of each iteration.
	){
		// Skip if the ramtile is set to available. (No point in searching ramtiles that are not in play.)
		if(game.ramtile_ids_used[currentRamtileId-1]==0){ continue; }

		//
		isMatch = true;

		// Check each pixel of both tiles and look for an all-match.
		for(u16 i=0; i<64; i+=1){
			// Get the pixels for comparison.
			leftPixel  = ram_tiles[ (int)(ramtile_id*64)       +i ];
			rightPixel = ram_tiles[ (int)(currentRamtileId*64) +i ];

			// Do they NOT match?
			if(leftPixel != rightPixel){ isMatch = false; break; }
		}

		//
		if(isMatch){
			return currentRamtileId;	// Will be truth-y.
		}

	}

	// Getting here means that the specified tile did NOT match any existing ram tiles.
	return ramtile_id; // Should be false-y if the specified ramtile was 0 (which is expected.)

}
// RAMTILE UTILITIES

// CARD UTILITIES
// Removes the card from the specified player and card position.
static void removeCard_sm(u8 player, u8 position){
	u8 x;
	u8 y;
	u8 w;
	u8 h;
	switch(player){
		case 1 : { x=pgm_read_byte(&(p1_pos[position][0])); y=pgm_read_byte(&(p1_pos[position][1])); w=2;h=3;break; }
		case 2 : { x=pgm_read_byte(&(p2_pos[position][0])); y=pgm_read_byte(&(p2_pos[position][1])); w=3;h=2;break; }
		case 3 : { x=pgm_read_byte(&(p3_pos[position][0])); y=pgm_read_byte(&(p3_pos[position][1])); w=2;h=3;break; }
		case 4 : { x=pgm_read_byte(&(p4_pos[position][0])); y=pgm_read_byte(&(p4_pos[position][1])); w=3;h=2;break; }
		default : { return; break; }
	};

	// Local variables.
	u8 map[ 2 + (w*h) ];
	// const u8 replacementTile = pgm_read_byte(&(wood_color2[2])) ;
	const u8 replacementTile = pgm_read_byte(&(offBlackTile[2])) ;
	// const u8 replacementTile = pgm_read_byte(&(wood_color1[2])) ;

	// Populate the tilemap by copying the tiles at the card position into an array.
	setVramTilemap( x,y , w, h, map );

	// Hide the tiles for the card's original position.
	Fill(x,y, w,h, replacementTile);

	// Clear any ramtiles that were used by the map.
	removeUnusedRamtilesInTilemap(map);
}
// Removes the large card (discard.)
static void removeCard_lg(){
	// Local variables.
	const u8 replacementTile = pgm_read_byte(&(wood_color1[2]));
	const u8 x = pgm_read_byte(&(discard_pos[0]));
	const u8 y = pgm_read_byte(&(discard_pos[1]));
	const u8 w = 3;
	const u8 h = 4;
	u8 map2[ 2 + (w*h) ];

	// Populate the tilemap by copying the tiles at the card position into an array.
	setVramTilemap( x,y , w, h, map2 );

	// Hide the tiles for the card's original position.
	Fill(x,y, w,h, replacementTile);

	// Clear any ramtiles that were used by the map.
	removeUnusedRamtilesInTilemap(map2);
}
// Removes and clears the ramtiles that are no longer in use by the specified map.
static void removeUnusedRamtilesInTilemap(u8 *map){
	// This function operates similar to removeCard_sm and removeCard_lg.
	 // Requires a tilemap that uses vram ids (and is stored in RAM, not PROGMEM.)
	// It assumes that that the tiles for the removed card have already been removed from vram.
	// It checks each tile in the tilemap for existance in vram.
	 // If the tile id is < RAM_TILES_COUNT then it is a ramtile and is subject to being cleared if the tile id is used nowhere else in vram.
	 // Flash tiles are ignored.

	// Clear the debug section.
	clear_debug_showDebugData();

	// Get the dimensions for this map.
	const u8 w=map[0];
	const u8 h=map[1];
	const u8 sizeOfMap = (w*h)+2;
	u16 tile;
	u8 clearThisRamtile=false;

	// For each tile in the array, look through vram.
	for(u16 ii=2; ii<sizeOfMap; ii+=1){
		// Get the tile id.
		tile = map[ii];

		// Must be a ramtile (and one of the ones that can be reserved.)
		if(tile <= reservedRamtiles ){
			// Set the clear flag.
			clearThisRamtile=true;

			// Go through vram.
			for(u16 i=0; i<(VRAM_TILES_H*VRAM_TILES_V); i+=1){
				// If at least one instance of the tile id is found then ignore this tile.
				if     (vram[i] == tile ){
					// Clear the clear flag.
					clearThisRamtile=false;

					// Quit looking in vram for this tile.
					break;
				}
			}

			// Is the clear flag still set? (If 0 instances of the tile id is found then clear it.)
			// NOTE: If a tile is cleared twice then that should not be a problem.
			if(clearThisRamtile){
				// Set the ramtile to be available for use.
				game.ramtile_ids_used[ tile -1] = 0;

				// Clear the data in that ramtile.
				clearRamtile(tile, CLEAREDRAMTILECOLOR4);
			}
		}
	}

	debug_showDebugData();

}
// Returns all discarded cards to the draw pile except the last played card.
static void returnDiscardsToDrawPile(){
	// Return all discards to the draw pile.
	for(u8 i=0; i<totalCardsInDeck; i+=1){
		if( cards[i].location==CARD_LOCATION_DISCARD ) {
			cards[i].location = CARD_LOCATION_DRAW ;
			debug_showDebugData();
		}
	}

	// Return the last card played to the discard pile since it should still be displayed.
	cards[game.lastCardPlayed].location = CARD_LOCATION_DISCARD;

	// Now adjust the "size" of each pile.

	// Draw pile (below.)
	redrawUnderDrawPile();

	// Discard pile (below.)
	redrawUnderDiscardPile();

	// debug_showDebugData();
}
// Returns ALL cards to the Draw Pile.
static void resetTheDeck(){
	// Return all cards to the draw pile.
	for(u8 i=0; i<totalCardsInDeck; i+=1){
		cards[i].location = CARD_LOCATION_DRAW ;
	}
}
// CARD UTILITIES

// CARD DRAWING
// Populate the specified map array with vram tiles.
static void setVramTilemap(u8 x, u8 y, u8 w, u8 h, u8 *map){
	// Provide the origin x and y and the width and height of the tilemap.
	// Also apply a reference to an already defined and appropriately sized array.
	// The passed array will be adjusted in this function.
	// This function does not return anything.

	map[0] = w;
	map[1] = h;
	u8 i = 0;

	for(u8 dy=0;dy<h;dy++){
		for(u8 dx=0;dx<w;dx++){
			map[i+2] = vram[ ( (y+dy) * VRAM_TILES_H ) + x + dx ];
			i+=1;
		}
	}

}
// Draws a tilemap from the specified vram map.
static void DrawMap_vramtiles(u8 x,u8 y, u8 *map) {
	// Give x, y, and an array from ram for the tilemap to draw.
	const u8 mapWidth  = map[0];
	const u8 mapHeight = map[1];

	// Use a pointer (easier to read this way.)
	u8 * vram_ptr ;
	u8 tile ;

	for(u8 dy=0;dy<mapHeight;dy++){
		for(u8 dx=0;dx<mapWidth;dx++){
			// Set the address of the pointer.
			vram_ptr = &vram[ ( (y+dy) * VRAM_TILES_H ) + x+dx ];

			// Get the tile id specified by the map at this position.
			tile = map[ (dy*mapWidth)+dx+2 ];

			// Set the value for this vram tile.
			 *vram_ptr = tile ;
		}
	}
}
// Draws small and large cards. Reserves/deduplicates ramtiles used.
static void drawCard( u8 playerNum, u8 x, u8 y, u8 size, u8 value, u8 color){
	// IMPORTANT: Remove the card first BEFORE drawing the card.
	// removeCard_sm(1, 3); // player number, card position.

	// CARD UTILITIES
	void draw1tile_forCards(u8 x, u8 y, u8 tile, u8 color, u8 useRamtile, u8 rotations){
		// Read into ramtile 0.
			// Convert the orange to the specified color.
			// Determine if we already have this exact tile.
				// YES? Point instead to the found tile.
				// NO? Copy to next ram tile.

		u8 newRamtile_id ;

		if(useRamtile==1){
			CopyFlashTile(tile, 0);
			swapColors_ramtile(CARD_ORANGE, color, 0);
			newRamtile_id=tileIsDuplicateOf(0);

			// If tileIsDuplicateOf returns non-zero then the tile exists and that tile id was returned.
			// If rotations were specified then skip this check. It will be made again later after the rotations.
			if( newRamtile_id !=0 && !rotations ) { SetRamTile(x,y, newRamtile_id); }

			// If tileIsDuplicateOf returns 0 then the tile is new.
			else{
				// Is there an availale ram tile?
				if( countAvailableRamtile_ids() !=0 ) {
					// Yes? Determine which one.
					// Look in game.ramtile_ids_used for the first 0 value and use that index+1 as the new ramtile id.
					for(u8 i=0; i<reservedRamtiles; i+=1){
						// Found an index where the value set is 0?
						if(game.ramtile_ids_used[i]==0){
							// The current value of the i iterator will be the index and the next ram tile used.
							// Copy ramtile 0 to the id and then draw it.

							// Rotate the ramtile the number of times specified.
							while(rotations){
								rotateRamTile_n90(&ram_tiles[(0*64)]); // n90 rotate ramtile 0.
								rotations--;
							}

							// Is this rotated ramtile the same as another ramtile?
							newRamtile_id=tileIsDuplicateOf(0);
							if( newRamtile_id !=0 ) {
								// Duplicate! Don't use a new ramtile. Use the ramtile id that was returned from tileIsDuplicateOf.
								SetRamTile(x,y, newRamtile_id);
							}
							else{
								// Copy the ramtile to the available ramtile.
								CopyRamTile(0,   i+1);

								// Display the ramtile.
								SetRamTile (x,y, i+1);

								// Set the value to 1 (indicating "used".)
								game.ramtile_ids_used[i]=1;
							}

							// Done.
							break;
						}
					}
				}

				// Was an available id NOT found? // Print the warning.
				else {
					errorHandler(RAMTILEUSAGE);
				}
			}
		}
		else{
			SetTile(x,y,tile);
		}

	}
	// CARD UTILITIES

	const char * top    ;
	const char * base   ;
	// const char * bottom ;

	// Hide the cursor sprite.
	// sprites[0].x=OFF_SCREEN;

	// Disable the kernel processing of sprites.
	SetSpriteVisibility(false);

	// The number tiles are the same for large and small card.
	switch(value){
		case CARD_0          : { top=tile_0 ;         break; }
		case CARD_1          : { top=tile_1 ;         break; }
		case CARD_2          : { top=tile_2 ;         break; }
		case CARD_3          : { top=tile_3 ;         break; }
		case CARD_4          : { top=tile_4 ;         break; }
		case CARD_5          : { top=tile_5 ;         break; }
		case CARD_6          : { top=tile_6 ;         break; }
		case CARD_7          : { top=tile_7 ;         break; }
		case CARD_8          : { top=tile_8 ;         break; }
		case CARD_9          : { top=tile_9 ;         break; }
		default : { break; }
	}

	// Draw a 2x3 tile card?
	if(size==SMALL_CARD){
		// Action cards.
		switch(value){
			case CARD_DRAW2      : { top=map_sm_draw2 ;   break; }
			case CARD_SKIP       : { top=map_sm_skip ;    break; }
			case CARD_REV        : { top=map_sm_reverse ; break; }
			default : { break; }
		}

		switch(color){
			case CARD_YELLOW     : {
				if     (playerNum==1 || playerNum==3) { base=card_front_yellow_sm;       }
				else if(playerNum==2 || playerNum==4) { base=card_front_yellow_sm_90deg; }
				break;
			}
			case CARD_BLUE       : {
				if     (playerNum==1 || playerNum==3) { base=card_front_blue_sm;       }
				else if(playerNum==2 || playerNum==4) { base=card_front_blue_sm_90deg; }
				break;
			}
			case CARD_RED        : {
				if     (playerNum==1 || playerNum==3) { base=card_front_red_sm;       }
				else if(playerNum==2 || playerNum==4) { base=card_front_red_sm_90deg; }
				break;
			}
			case CARD_GREEN      : {
				if     (playerNum==1 || playerNum==3) { base=card_front_green_sm;       }
				else if(playerNum==2 || playerNum==4) { base=card_front_green_sm_90deg; }
				break;
			}
			default : { break; }
		}

		// If this is a wild card then no coloring is needed and no ramtiles are needed. Just draw the maps.
		if     (value==CARD_WILD || value==CARD_WILD_DRAW4){
			// Determine the correct map to draw and then draw it.
			switch(value){
				case CARD_WILD       : {
					if     (playerNum==1 || playerNum==3) { DrawMap2( x  , y   , card_wild_front_sm       ); }
					else if(playerNum==2 || playerNum==4) { DrawMap2( x  , y   , card_wild_front_sm_90neg ); }
				break;
			}
				case CARD_WILD_DRAW4 : {
					if     (playerNum==1 || playerNum==3) { DrawMap2( x  , y   , card_wild_d4front_sm       ); }
					else if(playerNum==2 || playerNum==4) { DrawMap2( x  , y   , card_wild_d4front_sm_90neg ); }
					break;
				}
			}
		}

		// All other cards require ramtiles and re-coloring.
		else{
			// Ramtiles are needed by the top-left, bottom-right corners only.
			// Use flash tiles for the rest.
			if     (playerNum==1 || playerNum==3) {
				// ROW #1
				draw1tile_forCards(x+0, y+0, pgm_read_word(&(top [2+0])) ,color, 1,0) ; // ROW #1: Left
				draw1tile_forCards(x+1, y+0, pgm_read_word(&(base[2+1])) ,color, 0,0) ; // ROW #1: Right

				// ROW #2
				draw1tile_forCards(x+0, y+1, pgm_read_word(&(base[2+2])) ,color, 0,0) ; // ROW #2: Left
				draw1tile_forCards(x+1, y+1, pgm_read_word(&(base[2+3])) ,color, 0,0) ; // ROW #2: Right

				// ROW #3
				draw1tile_forCards(x+0, y+2, pgm_read_word(&(base[2+4])) ,color, 0,0) ; // ROW #3: Left
				draw1tile_forCards(x+1, y+2, pgm_read_word(&(top [2+0])) ,color, 1,2) ; // ROW #3: Right
			}
			else if(playerNum==2 || playerNum==4) {
				// ROW #1
				draw1tile_forCards(x+0, y+0, pgm_read_word(&(top [0+2])) ,color, 1,1) ; // ROW #1: Left
				draw1tile_forCards(x+1, y+0, pgm_read_word(&(base[1+2])) ,color, 0,0) ; // ROW #1: Middle
				draw1tile_forCards(x+2, y+0, pgm_read_word(&(base[2+2])) ,color, 0,0) ; // ROW #1: Right

				// ROW #2
				draw1tile_forCards(x+0, y+1, pgm_read_word(&(base[3+2])) ,color, 0,0) ; // ROW #2: Left
				draw1tile_forCards(x+1, y+1, pgm_read_word(&(base[4+2])) ,color, 0,0) ; // ROW #2: Middle
				draw1tile_forCards(x+2, y+1, pgm_read_word(&(top [0+2])) ,color, 1,3) ; // ROW #2: Right
			}
		}
	}

	// Draw a3x4 tile card?
	else if(size==LARGE_CARD){
		// Action cards.
		switch(value){
			case CARD_DRAW2      : { top=map_lg_draw2 ;    break; }
			case CARD_SKIP       : { top=map_lg_skip ;     break; }
			case CARD_REV        : { top=map_lg_reverse ;  break; }
			default : { break; }
		}

		switch(color){
			case CARD_YELLOW     : { base=card_front_yellow_lg; break; }
			case CARD_BLUE       : { base=card_front_blue_lg;   break; }
			case CARD_RED        : { base=card_front_red_lg;    break; }
			case CARD_GREEN      : { base=card_front_green_lg;  break; }
			// case CARD_ORANGE     : { base=card_front_lg;        break; }
			// case CARD_BLACK      : { base=card_front_lg;       break; }
			default : { break; }
		}

		// If this is a wild card then no coloring is needed and no ramtiles are needed. Just draw the maps.
		if     (value==CARD_WILD || value==CARD_WILD_DRAW4){
			switch(value){
				case CARD_WILD       : { base=card_wild_front_lg;       break; }
				case CARD_WILD_DRAW4 : { base=card_wild_draw4_front_lg; break; }
			}
			DrawMap2( x  , y   , base   );
		}
		// All other cards require ramtiles and re-coloring.
		else{
			// Draw the base (flash tiles.) (Rows 2 and 3.)
			draw1tile_forCards(x+0, y+1, pgm_read_word(&( base [2+3])),  color, 0,0); // ROW #2 - LEFT
			draw1tile_forCards(x+1, y+1, pgm_read_word(&( base [2+4])),  color, 0,0); // ROW #2 - MIDDLE
			draw1tile_forCards(x+2, y+1, pgm_read_word(&( base [2+5])),  color, 0,0); // ROW #2 - RIGHT
			draw1tile_forCards(x+0, y+2, pgm_read_word(&( base [2+6])),  color, 0,0); // ROW #3 - LEFT
			draw1tile_forCards(x+1, y+2, pgm_read_word(&( base [2+7])),  color, 0,0); // ROW #3 - MIDDLE
			draw1tile_forCards(x+2, y+2, pgm_read_word(&( base [2+8])), color, 0,0); // ROW #3 - RIGHT

			// Draw the top.
			if     (value==CARD_DRAW2 || value==CARD_REV){
				draw1tile_forCards(x+0, y+0, pgm_read_word(&( top  [2+0])), color, 1,0); // ROW #1 - LEFT
				draw1tile_forCards(x+1, y+0, pgm_read_word(&( top  [2+1])), color, 1,0); // ROW #1 - MIDDLE
				draw1tile_forCards(x+2, y+0, pgm_read_word(&( top  [2+2])), color, 1,0); // ROW #1 - RIGHT
			}
			else if(value==CARD_SKIP){
				draw1tile_forCards(x+0, y+0, pgm_read_word(&( top  [2+0])), color, 1,0); // ROW #1 - LEFT
				draw1tile_forCards(x+1, y+0, pgm_read_word(&( top  [2+1])), color, 1,0); // ROW #1 - MIDDLE
				draw1tile_forCards(x+2, y+0, pgm_read_word(&( base [2+2])), color, 0,0); // ROW #1 - RIGHT
			}
			else{
				draw1tile_forCards(x+0, y+0, pgm_read_word(&( top  [2+0])), color, 1,0); // ROW #1 - LEFT
				draw1tile_forCards(x+1, y+0, pgm_read_word(&( base [2+1])), color, 0,0); // ROW #1 - MIDDLE
				draw1tile_forCards(x+2, y+0, pgm_read_word(&( base [2+2])), color, 0,0); // ROW #1 - RIGHT
			}

			// Draw the bottom.
			if     (value==CARD_DRAW2 || value==CARD_REV){
				draw1tile_forCards(x+0, y+3, pgm_read_word(&( top  [2+2])), color, 1,2); // ROW #4 - RIGHT
				draw1tile_forCards(x+1, y+3, pgm_read_word(&( top  [2+1])), color, 1,2); // ROW #4 - MIDDLE
				draw1tile_forCards(x+2, y+3, pgm_read_word(&( top  [2+0])), color, 1,2); // ROW #4 - LEFT
			}
			else if(value==CARD_SKIP){
				draw1tile_forCards(x+0, y+3, pgm_read_word(&( base [2+5])), color, 0,0); // ROW #4 - RIGHT
				draw1tile_forCards(x+1, y+3, pgm_read_word(&( top  [2+1])), color, 1,2); // ROW #4 - MIDDLE
				draw1tile_forCards(x+2, y+3, pgm_read_word(&( top  [2+0])), color, 1,2); // ROW #4 - LEFT
			}
			else{
				draw1tile_forCards(x+0, y+3, pgm_read_word(&( base [2+5])), color, 0,0); // ROW #4 - RIGHT
				draw1tile_forCards(x+1, y+3, pgm_read_word(&( base [2+1])), color, 0,0); // ROW #4 - MIDDLE
				draw1tile_forCards(x+2, y+3, pgm_read_word(&( top  [2+0])), color, 1,2); // ROW #4 - LEFT
			}

		}
	}

	// Clear the data in the comparison ramtile.
	clearRamtile(0, CLEAREDRAMTILECOLOR4);

	// Enable the kernel processing of sprites.
	SetSpriteVisibility(true);

	debug_showDebugData();
}
// Map a spritemap to sprites. Uses vram ids. Auto sets the SPRITE_RAM if the tile is a ramtile.
static void MapSprite2_nra( u8 startSprite, const char *map, u8 spriteFlags ) {
	// Similar syntax to MapSprite2. Supports all sprite flags such as: SPRITE_FLIP_X, SPRITE_FLIP_Y, SPRITE_OFF.
	 // The map uses vram ids (not the normal tile ids.)
	 // SPRITE_RAM is set automatically as determined by the value of the vram id.
	  // If an individual tile is < RAM_TILES_COUNT then SPRITE_RAM in spriteFlags will be set for that tile.
	// The map is expected to be from ram.
	// After being mapped, MoveSprite can be used normally.

	u8 mapWidth  = map[0]; // Tilemap width.
	u8 mapHeight = map[1]; // Tilemap height.
	u8 x;                  // Starting X origin.
	u8 y;                  // Starting Y origin.
	s8 dx;                 // Destination X position.
	s8 dy;                 // Destination t position.
	u8 t;                  // Tile id.

	// SPRITE_FLIP_X and SPRITE_FLIP_Y are used to determine the draw order of the map.
	// The kernel will take care of the actual sprite flipping on x and/or y.
	if(spriteFlags & SPRITE_FLIP_X){ x=(mapWidth-1); dx=-1; }
	else                           { x=0; dx=1; }
	if(spriteFlags & SPRITE_FLIP_Y){ y=(mapHeight-1); dy=-1; }
	else                           { y=0; dy=1; }

	// Go through the map and adjust the values in the sprites array.
	for(u8 cy=0;cy<mapHeight;cy++){
		for(u8 cx=0;cx<mapWidth;cx++){
			// Get the vram tile id.
			t=map[ (y * mapWidth) +x +2 ] ;

			// Set the tile id and the sprite flags.

			// Is this a ram tile?
			if( t < RAM_TILES_COUNT ){
				// Set the tile id to be the tileIndex of this sprite entry.
				sprites[startSprite].tileIndex=t;

				// Set the SPRITE_RAM flag in spriteFlags of this sprite entry based on the value of the tile id.
				// Make sure that a spritebank is NOT set for this.
				sprites[startSprite].flags =
					spriteFlags |
					SPRITE_RAM ;//& ~( SPRITE_BANK0 | SPRITE_BANK1 | SPRITE_BANK2 | SPRITE_BANK3 ) ;
			}
			// This is a flash tile. Use the specified spriteFlags (which should also include the SPRITE_BANK if it is not SPRITE_BANK0.)
			else{
				sprites[startSprite].tileIndex = t-RAM_TILES_COUNT;
				sprites[startSprite].flags     = spriteFlags ;
			}

			// Advance x.
			x+=dx;

			// Advance to the next sprite.
			startSprite++;
		}

		// Advance y.
		y+=dy;

		// Set the next x position.
		x = (spriteFlags & SPRITE_FLIP_X) ? (mapWidth-1) : (0);
	}

}
// CARD DRAWING

// UTILITIES
// Returns a random number within the range specified.
int getRandomNumber(int min, int max){
	// https://stackoverflow.com/a/18386648/2731377
	// return min + rand() / (RAND_MAX / (max - min + 1) + 1);
	return min + rand() / (32724 / (max - min + 1) + 1);

	// GetPrngNumber(GetTrueRandomSeed());

	// return min + GetPrngNumber( GetTrueRandomSeed() ) / (RAND_MAX / (max - min + 1) + 1);

}
// Returns the count of cards held by the specified player.
u8 countPlayerCards(u8 playerNum){
	u8 theCount=0;
	u8 location;

	// Determine the card location by the player number.
	switch(playerNum){
		case 1  : { location=CARD_LOCATION_PLAYER1; break; }
		case 2  : { location=CARD_LOCATION_PLAYER2; break; }
		case 3  : { location=CARD_LOCATION_PLAYER3; break; }
		case 4  : { location=CARD_LOCATION_PLAYER4; break; }
		default : {
			return 0;
			break;
		}
	}

	// Go through the cards array and count up all cards that match the player location.
	for(u8 i=0; i<totalCardsInDeck; i+=1){
		if     (cards[i].location==location){
			theCount++;
		}
	}

	// Return the count.
	return theCount;
}
// Returns the count of cards held by the Draw Pile.
u8 countDrawPile(){
	u8 theCount=0;
	// Go through the cards array and count up all cards that match the location.
	for(u8 i=0; i<totalCardsInDeck; i+=1){ if( cards[i].location==CARD_LOCATION_DRAW ) { theCount++;} }
	// Return the count.
	return theCount;
}
// Returns the count of cards held by the Discard Pile.
u8 countDiscardPile(){
	u8 theCount=0;
	// Go through the cards array and count up all cards that match the location.
	for(u8 i=0; i<totalCardsInDeck; i+=1){ if( cards[i].location==CARD_LOCATION_DISCARD ) { theCount++;} }
	// Return the count.
	return theCount;
}
// Returns the count of used ram ids.
u8 countUsedRamtile_ids(){
	u8 theCount=0;
	// Go through the reserved ramtiles array and count the used indexes.
	for(u8 i=0; i<reservedRamtiles; i+=1){ if( game.ramtile_ids_used[i] == 1 ) { theCount++; } }
	// Return the count.
	return theCount;
}
// Returns the count of unused ram ids.
u8 countAvailableRamtile_ids(){
	u8 theCount=0;
	// Go through the reserved ramtiles array and count the unused indexes.
	for(u8 i=0; i<reservedRamtiles; i+=1){ if( game.ramtile_ids_used[i] == 0 ) { theCount++; } }
	// Return the count.
	return theCount;
}
// UTILITIES

// Draws the "stack" under the Draw Pile based on the count of the Draw Pile.
static void redrawUnderDrawPile(){
	u8 theCount = countDrawPile() ;
	u8 x = pgm_read_byte(&(draw_pos_below[0]));
	u8 y = pgm_read_byte(&(draw_pos_below[1]));

	if     (theCount <  27 ) { DrawMap2(x,y, cardsBelow_lg_lt27) ;} // 27
	else if(theCount <  54 ) { DrawMap2(x,y, cardsBelow_lg_lt54) ;} // 54
	else if(theCount <  81 ) { DrawMap2(x,y, cardsBelow_lg_lt81) ;} // 81
	else if(theCount <= 108) { DrawMap2(x,y, cardsBelow_lg_lt108);} // 108
}
// Draws the "stack" under the Discard Pile based on the count of the Discard Pile.
static void redrawUnderDiscardPile(){
	u8 theCount = countDiscardPile() ;
	u8 x = pgm_read_byte(&(discard_pos_below[0]));
	u8 y = pgm_read_byte(&(discard_pos_below[1]));

	if     (theCount <  27 ) { DrawMap2(x,y, cardsBelow_lg_lt27) ;}
	else if(theCount <  54 ) { DrawMap2(x,y, cardsBelow_lg_lt54) ;}
	else if(theCount <  81 ) { DrawMap2(x,y, cardsBelow_lg_lt81) ;}
	else if(theCount <= 108) { DrawMap2(x,y, cardsBelow_lg_lt108);}
}
// Returns all cards for the specified player to the Draw Pile.
static void returnCardsForPlayer(u8 playerNum){
	u8 location;

	// Get location by player number.
	switch(playerNum){
		case 1  : { location=CARD_LOCATION_PLAYER1; break; }
		case 2  : { location=CARD_LOCATION_PLAYER2; break; }
		case 3  : { location=CARD_LOCATION_PLAYER3; break; }
		case 4  : { location=CARD_LOCATION_PLAYER4; break; }
		default : {
			// ClearVram(); WaitVsync(100);
			break;
		}
	}

	// Go through the cards array and return all matching location cards to the draw pile.
	for(u8 i=0; i<totalCardsInDeck; i+=1){
		if     (cards[i].location==location){
			cards[i].location=CARD_LOCATION_DRAW;
		}
	}
}
// Draws up to 5 cards from the specified player's hand. Can adjust the start position and draw speed.
static void displayCardsForPlayer(u8 playerNum, u8 startPos, u8 cardDelay){
	u8 location ;
	u8 cardsAssigned=0 ;
	u8 x;
	u8 y;
	u8 positionsToSkip=startPos;

	switch(playerNum){
		case 1  : { location=CARD_LOCATION_PLAYER1; break; }
		case 2  : { location=CARD_LOCATION_PLAYER2; break; }
		case 3  : { location=CARD_LOCATION_PLAYER3; break; }
		case 4  : { location=CARD_LOCATION_PLAYER4; break; }
		default : {
			// ClearVram(); WaitVsync(100);
			break;
		}
	}

	// 5 card positions.
	for(u8 i=0; i<totalCardsInDeck && (cardsAssigned<5); i+=1){
		// Does this card belong to the player?
		if( cards[i].location==location ) {
			// Do we skip this card?
			if(positionsToSkip) { positionsToSkip--; continue; }

			// Get the x and y tile coordinate for the card position.
			// Also, remove the card from that position.
			if     (location==CARD_LOCATION_PLAYER1){
				x=pgm_read_byte(&(p1_pos[cardsAssigned][0]));
				y=pgm_read_byte(&(p1_pos[cardsAssigned][1]));
				removeCard_sm(1, cardsAssigned);
			}
			else if(location==CARD_LOCATION_PLAYER2){
				x=pgm_read_byte(&(p2_pos[cardsAssigned][0]));
				y=pgm_read_byte(&(p2_pos[cardsAssigned][1]));
				removeCard_sm(2, cardsAssigned);
			}
			else if(location==CARD_LOCATION_PLAYER3){
				x=pgm_read_byte(&(p3_pos[cardsAssigned][0]));
				y=pgm_read_byte(&(p3_pos[cardsAssigned][1]));
				removeCard_sm(3, cardsAssigned);
			}
			else if(location==CARD_LOCATION_PLAYER4){
				x=pgm_read_byte(&(p4_pos[cardsAssigned][0]));
				y=pgm_read_byte(&(p4_pos[cardsAssigned][1]));
				removeCard_sm(4, cardsAssigned);
			}
			else{
				// This card is not assigned to a player. Continue.
				continue;
			}

			// A valid card was found.

			// Adjust the global variable for the player's hand.
			game.playerVisibleHand[cardsAssigned] = i;

			// Draw the card.
			drawCard(playerNum, x, y, SMALL_CARD, cards[i].value , cards[i].color );

			// Increment the number of cards assigned.
			cardsAssigned++;

			// Wait a short time.
			if(cardDelay){
				WaitVsync(cardDelay);
			}
		}
	}

	// Did we not assign 5 cards? Blank out the remaining card positions.
	while(cardsAssigned!=5){
		// const u8 replacementTile2 = pgm_read_byte(&(wood_color2[2])) ;
		const u8 replacementTile2 = pgm_read_byte(&(offBlackTile[2])) ;
		if     (location==CARD_LOCATION_PLAYER1){
			x=pgm_read_byte(&(p1_pos[cardsAssigned][0]));
			y=pgm_read_byte(&(p1_pos[cardsAssigned][1]));
			removeCard_sm(1, cardsAssigned);
			Fill(x,y,2,3,replacementTile2);
		}
		else if(location==CARD_LOCATION_PLAYER2){
			x=pgm_read_byte(&(p2_pos[cardsAssigned][0]));
			y=pgm_read_byte(&(p2_pos[cardsAssigned][1]));
			removeCard_sm(2, cardsAssigned);
			Fill(x,y,3,2,replacementTile2);
		}
		else if(location==CARD_LOCATION_PLAYER3){
			x=pgm_read_byte(&(p3_pos[cardsAssigned][0]));
			y=pgm_read_byte(&(p3_pos[cardsAssigned][1]));
			removeCard_sm(3, cardsAssigned);
			Fill(x,y,2,3,replacementTile2);
		}
		else if(location==CARD_LOCATION_PLAYER4){
			x=pgm_read_byte(&(p4_pos[cardsAssigned][0]));
			y=pgm_read_byte(&(p4_pos[cardsAssigned][1]));
			removeCard_sm(4, cardsAssigned);
			Fill(x,y,3,2,replacementTile2);
		}
		game.playerVisibleHand[cardsAssigned]=255;
		cardsAssigned++;
	}
}
// Remove all cards displayed for the specified player.
static void removePlayerCards(u8 playerNumber){
	// Player number must be valid! If not then nothing will happen.
	// Player 1 is 1. (not zero-indexed.)
	// Card positions ARE zero-indexed.
	removeCard_sm(playerNumber, 0);
	removeCard_sm(playerNumber, 1);
	removeCard_sm(playerNumber, 2);
	removeCard_sm(playerNumber, 3);
	removeCard_sm(playerNumber, 4);
}
// Clears the displayed message (seen when playing cards.)
static void clearbgmessage(){
	// const u8 replacementTile = pgm_read_byte(&(wood_color1[2])) ;
	const u8 replacementTile = pgm_read_byte(&(wood_color2[2])) ;
	Fill(8,17, 12,5, replacementTile);
}
// Assigns the specified number of cards to the specified player. (Not shown visually.)
u8 getCardFromDrawPile(u8 playerNum, u8 howMany){
	// Random card that has the location of CARD_LOCATION_DRAW.
	u8 cardIndex2;	// Used by this function.
	u8 cardsToAssign = howMany;	// Used by this function.
	u8 location;

	switch(playerNum){
		case 1  : { location=CARD_LOCATION_PLAYER1; break; }
		case 2  : { location=CARD_LOCATION_PLAYER2; break; }
		case 3  : { location=CARD_LOCATION_PLAYER3; break; }
		case 4  : { location=CARD_LOCATION_PLAYER4; break; }
		default : {
			return 255;
			break;
		}
	}

	u8 inDrawPile    = countDrawPile();
	u8 inDiscardPile = countDiscardPile();

	// Make sure enough cards exist in the draw pile!
	if( inDrawPile < howMany ) {
		// Return discarded cards to the Draw Pile. (Except: last played card.)
		returnDiscardsToDrawPile();

		// Update the displayed color, direction, draw pile, discard pile and last played card on discard pile.
		redrawMainInfoRegion();

		// Count the draw pile again.
		inDrawPile    = countDrawPile();

		// ERROR? - Fatal error.
		// if( inDrawPile < howMany ) {
			// Print the warning.
			// errorHandler(NOTENOUGHCARDS);
		// }

		// ERROR? - Just don't allow the card to be provided.
		//
	}

	u8 specifedCardsAssigned=false;
	while( cardsToAssign != 0 && countDrawPile() >=1 ){
	// Loop until all required cards have been assigned.

		// Generate random number (index into cards array.)
		cardIndex2 = getRandomNumber(0, totalCardsInDeck-1); // Normal range.
		// cardIndex2 = getRandomNumber(8, 20-1);               // Yellows
		// cardIndex2 = getRandomNumber(0, 8-1);                // Wilds

		// Is the card available?
		if( cards[cardIndex2].location==CARD_LOCATION_DRAW ) {
			// Assign the card to the player.
			cards[cardIndex2].location=location;

			// Decrement the remaining cards to assign.
			cardsToAssign--;

			if(cardsToAssign==0){ specifedCardsAssigned=true; }
		}
	}

	// Were all specified cards assigned?
	if(specifedCardsAssigned==false){
		msgHandler(COULDNOTASSIGNALL);

		// Clear the message. (The message at the lower half of the game board.)
		clearbgmessage();
	}

	// Return the value of the last cardIndex2.
	return cardIndex2;
}

// TEST GAME.
static void test_testgame(){
	/*
	Determine first player:
	 Assign one card to each player.
	 Display each player's card.
	 Highest value goes first.
	 Reset the deck.

	Determine first discard:
	 Take one card from the Draw Pile to the Discard Pile.
	  Repeat if the card was a Wild Draw 4.
	 If the card is an action card then the action will happen to the first player.
	  Including wild. The user wilfl have the option to choose what starting color.
	*/

	// Draw the gameboard.

	// Populate the Draw Pile.

	// Reset the deck (assign all cards to the Draw Pile.)

	// Determine who goes first.
		// Each player is provided a temporary card that will be drawn at their first card position.
		// Highest card wins.

	// Deal cards to all active players (assign location values in the cards array.)
		// All cards are initially face down.

	// Put initial card onto the discard pile.
		// If it is a Reverse then reverse the player order.
		// If it is a Draw 2 then the first player draws two more cards and does not play this round.
		// If it is a Skip then the first player does not play this round.
		// If it is a Wild then the first player gets to choose the color.
		// If it is a Wild Draw 4 then it is ignored and another card is dealt.

	// GAME PLAY:

	// Players must discard from their hand a card that matches either the value or the color of the card on the discard pile.
		// If they cannot then they must draw from the Draw Pile.
			// If that card is playable then they are allowed the choice to immediately play that card.

		// A card is playable if the card value matches the discard card value, the card color matches, or the card is a Wild Card (or Wild Draw 4.)
			// If the card is a Wild Draw 4 then the player must choose the color. The next player must draw 4 cards and then lose their turn.
				// The Wild Draw 4 can only be used if it is the player's only playable card.
			// If the card is a Wild then the player must choose the new color.
			// If the card is a Draw 2 then the next player must draw two cards from the Draw Pile and then lose their turn.
			// If the card is a Skip then the next player loses their turn.
			// If the card is a Reverse then the player order reverses.
			// If the card value or color matches the discard card value or color then the player's turn ends and it is the next player's turn.

		// If a player only has one card remaining then they must declare "UNO!"
			// If this is applicable then the game will provide a short window where the player can choose to declare "UNO!"
				// If this window is missed then the player must draw two cards from the Draw Pile.

		// The game ends when the a player no longer has any cards in their hand.
			// Points are now added. The remaining player's cards are added up and that score is added to the winning player's score.
				// "Number Card"    - Number value of the card is number of points.
				// "Draw Two"       - 20 Points
				// "Reverse"        - 20 Points
				// "Skip"           - 20 Points
				// "Wild"           - 50 Points
				// "Wild Draw Four" - 50 Points
			// If a player reaches 500 points then they win.

	while(1){
		WaitVsync(1);
	}

}
// TEST GAME.

static void dealSpecifiedCard_anim(u8 playerNum, u8 cardIndex, u8 cardPos, u8 cardDelay, u8 option){
	// Draw one card from the Draw Pile and assign to the specified player.
	// Get the last card assigned's index into the cards array.
	// NOTE: This function assumes that the cards have been removed first.
	const char * card ;
	u8 location;

	// Card sprite.
	// Contains the tilemap (vram tiles) used by the selected card sprite. (2x3 tilemaps.)
	u8 map1_2x3 [ 2 + (2*3) ];

	// Contains the tilemap (vram tiles) used by the selected card sprite. (3x2 tilemaps.)
	u8 map2_3x2 [ 2 + (3*2) ];

	// Pointer to the tilemap arrayy
	u8 * map ;

	// Dimensions for the selected card map.
	u8 w;
	u8 h;

	s8 xdir;
	s8 ydir;

	u8 tile_x=pgm_read_byte(&(draw_pos[0]));
	u8 tile_y=pgm_read_byte(&(draw_pos[1]));
	u8 sprite_x = tile_x << 3;
	u8 sprite_y = tile_y << 3;

	u8 dst_x;
	u8 dst_y;

	u8 counter;

	// Get the proper card tilemap, position, and dimensions.
	switch(playerNum){
		case 1 : {
			card=card_back_sm_0deg;
			w=2; h=3;
			map = &map1_2x3;
			dst_x=pgm_read_byte(&(p1_pos[cardPos][0]));
			dst_y=pgm_read_byte(&(p1_pos[cardPos][1]));
			location=CARD_LOCATION_PLAYER1;
			break;
		}
		case 2 : {
			card=card_back_sm_neg90deg;
			w=3; h=2;
			map = &map2_3x2;
			dst_x=pgm_read_byte(&(p2_pos[cardPos][0]));
			dst_y=pgm_read_byte(&(p2_pos[cardPos][1]));
			location=CARD_LOCATION_PLAYER2;
			break;
		}
		case 3 : {
			card=card_back_sm_360deg;
			w=2; h=3;
			map = &map1_2x3;
			dst_x=pgm_read_byte(&(p3_pos[cardPos][0]));
			dst_y=pgm_read_byte(&(p3_pos[cardPos][1]));
			location=CARD_LOCATION_PLAYER3;
			break;
		}
		case 4 : {
			card=card_back_sm_90deg;
			w=3; h=2;
			map = &map2_3x2;
			dst_x=pgm_read_byte(&(p4_pos[cardPos][0]));
			dst_y=pgm_read_byte(&(p4_pos[cardPos][1]));
			location=CARD_LOCATION_PLAYER4;
			break;
		}
		default : { return; break; }
	};

	dst_x = dst_x<<3;
	dst_y = dst_y<<3;

	// If the card index is 255 then actually get a new card and use the new index.
	// Otherwise, use the specified card index.
	if(cardIndex==255){
		cardIndex = getCardFromDrawPile(playerNum, 1);
	}


	// Draw card face down.
	if     (option==CARDS_FACEDOWN){
		// Draw the properly rotated facedown card.
		DrawMap2(tile_x, tile_y, card);
	}
	// Draw card face up.
	else if(option==CARDS_FACEUP  ){
		// Draw the card. Will be rotated correctly for the player.
		drawCard(
			playerNum                     , // u8 playerNum
			tile_x                        , // u8 x
			tile_y                        , // u8 y
			SMALL_CARD                    , // u8 size
			cards[cardIndex].value        , // u8 value
			cards[cardIndex].color          // u8 color
		);
	}

	// Get the tilemap for the new card.
	setVramTilemap( tile_x, tile_y , w, h, map );

	// Redraw the Draw Pile.
	DrawMap2(tile_x, tile_y, card_back_lg);

	// Redraw the Draw pile stack.
	redrawUnderDrawPile();

	// Map the card to a spritemap.
	MapSprite2_nra( 1, map, SPRITE_BANK1 );

	// Set the xdir for the card movement.
	if     (sprite_x<dst_x){ xdir =  1; }
	else if(sprite_x>dst_x){ xdir = -1; }
	else                   { xdir =  0; }

	// Set the ydir for the card movement.
	if     (sprite_y<dst_y){ ydir =  1; }
	else if(sprite_y>dst_y){ ydir = -1; }
	else                   { ydir =  0; }

	// Reset the counter. (Is used to speed-limit the card movement.)
	counter=0;

	// Move the card.
	while(1){
		// Determine if an x and/or y movement is required this iteration.
		if(sprite_x != dst_x){ sprite_x += xdir; }
		if(sprite_y != dst_y){ sprite_y += ydir; }

		// Redraw the sprite at the new location.
		MoveSprite(1, sprite_x, sprite_y, map[0], map[1]);

		// If both the x and the y values match the discard values then break.
		if( sprite_x==dst_x && sprite_y==dst_y ) { break; }

		// Do a vsync wait every other time.
		// if(counter & 1) { WaitVsync(1); }
		if(counter % cardDelay == 0 ){ WaitVsync(1); }

		// Increment the counter.
		counter+=1;
	}

	// Remove the card that the sprite is now overlapping.
	removeCard_sm(playerNum, cardPos);

	// Hide the small card spritemap.
	MapSprite2_nra( 1, map, SPRITE_OFF );

	// Use the spritemap to draw the card tiles back into vram.
	DrawMap_vramtiles( dst_x>>3, dst_y>>3, map );

	// Remove the tiles used by the small card (if they were the only instances of those tiles.)
	removeUnusedRamtilesInTilemap(map);

	// Update the displayed player data.
	updatePlayerDisplayedData();

	// Update the displayed color, direction, draw pile, discard pile and last played card on discard pile.
	redrawMainInfoRegion();

	return;
}

static void redrawMainInfoRegion(){
	u8 * colorTile;

	Fill(9,10, 10, 7, pgm_read_byte(&(wood_color1 [2])));

	// Redraw the Draw Pile.
	DrawMap2(pgm_read_byte(&(draw_pos[0])), pgm_read_byte(&(draw_pos[1])), card_back_lg);

	// Redraw the Draw pile stack.
	redrawUnderDrawPile();

	// Redraw the last discard if it was a valid card.
	if(game.lastCardPlayed != 255){
		// Remove the discard card. (Always do this first.)
		removeCard_lg();
		// Redraw the Discard Card.
		drawCard(
			255                              ,
			pgm_read_byte(&(discard_pos[0])) ,
			pgm_read_byte(&(discard_pos[1])) ,
			LARGE_CARD                       ,
			cards[game.lastCardPlayed].value ,
			cards[game.lastCardPlayed].color
		);
		// Redraw the Discard pile stack.
		redrawUnderDiscardPile();
	}

	// Play direction.
	if     (game.direction==FORWARD ) {
		DrawMap2(5 ,20,b_l_corner_fwd);
		DrawMap2(5 ,5 ,t_l_corner_fwd);
		DrawMap2(20,5 ,t_r_corner_fwd);
		DrawMap2(20,20,b_r_corner_fwd);
	}
	else if(game.direction==BACKWARD) {
		DrawMap2(5 ,20,b_l_corner_bkd);
		DrawMap2(5 ,5 ,t_l_corner_bkd);
		DrawMap2(20,5 ,t_r_corner_bkd);
		DrawMap2(20,20,b_r_corner_bkw);
	}

	// Active color
	if     (cards[game.lastCardPlayed].color==CARD_YELLOW){ colorTile=pgm_read_byte(&(border_yellow[2])) ; }
	else if(cards[game.lastCardPlayed].color==CARD_BLUE  ){ colorTile=pgm_read_byte(&(border_blue  [2])) ; }
	else if(cards[game.lastCardPlayed].color==CARD_RED   ){ colorTile=pgm_read_byte(&(border_red   [2])) ; }
	else if(cards[game.lastCardPlayed].color==CARD_GREEN ){ colorTile=pgm_read_byte(&(border_green [2])) ; }
	else                                                  { colorTile=pgm_read_byte(&(wood_color2 [2])) ; }

	void clearBorderColor(){
		const u8 replacementTile = pgm_read_byte(&(wood_color2 [2]));
		Fill(8 ,22,12,1 ,replacementTile); // Player 1 side.
		Fill(5 ,8 ,1 ,12,replacementTile); // Player 2 side.
		Fill(8 ,5 ,12,1 ,replacementTile); // Player 3 side.
		Fill(22,8 ,1 ,12,replacementTile); // Player 4 side.
	}

	// Clear the borders.
	clearBorderColor();

	// Draw the colored border only for the active player.
	if     (game.activePlayer==1){ Fill(8 ,22,12,1 ,colorTile); } // Player 1 side.
	else if(game.activePlayer==2){ Fill(5 ,8 ,1 ,12,colorTile); } // Player 2 side.
	else if(game.activePlayer==3){ Fill(8 ,5 ,12,1 ,colorTile); } // Player 3 side.
	else if(game.activePlayer==4){ Fill(22,8 ,1 ,12,colorTile); } // Player 4 side.

	debug_showDebugData();
}

static void get_cardDims_byPlayerAndCardPosition(u8 playerNum, u8 position, u8 *x, u8 *y){
	switch(playerNum){
		case 1 : {
			*x=pgm_read_byte(&(p1_pos[position][0]));
			*y=pgm_read_byte(&(p1_pos[position][1]));
			break;
		}
		case 2 : {
			*x=pgm_read_byte(&(p2_pos[position][0]));
			*y=pgm_read_byte(&(p2_pos[position][1]));
			break;
		}
		case 3 : {
			*x=pgm_read_byte(&(p3_pos[position][0]));
			*y=pgm_read_byte(&(p3_pos[position][1]));
			break;
		}
		case 4 : {
			*x=pgm_read_byte(&(p4_pos[position][0]));
			*y=pgm_read_byte(&(p4_pos[position][1]));
			break;
		}
		default : { return; break; }
	};

}
static void updatePlayerDisplayedData(){
	// Count all player cards.
	u8 cardCount_p1 = countPlayerCards(1);
	u8 cardCount_p2 = countPlayerCards(2);
	u8 cardCount_p3 = countPlayerCards(3);
	u8 cardCount_p4 = countPlayerCards(4);

	// Show player designations, arrows, and card counts.

	// P1
	if(game.activePlayers[0]==1){
		cardCount_p1 = countPlayerCards(1);
		// Draw name, cards, card count.
		DrawMap2      ( 23 , VRAM_TILES_V-3 , arrow_yellow);
		DrawMap2      ( 24 , VRAM_TILES_V-3 , arrow_yellow);
		N782_print    ( 26 , VRAM_TILES_V-3 , S_P1   , 0 ) ;
		N782_print    ( 23 , VRAM_TILES_V-2 , S_CARDS, 0 ) ;
		Fill(25 , VRAM_TILES_V-1, 3,1, 0);
		N782_print_u8 ( 25 , VRAM_TILES_V-1 , cardCount_p1) ;

		// Clear the UNO indicator.
		Fill(24 , VRAM_TILES_V-4, 4,1, 0x00);

		// Set the UNO indicator where applicable.
		if( cardCount_p1==1 ) { N782_print( 24 , VRAM_TILES_V-4, S_UNO , 0 ) ; }
	}
	// P2
	if(game.activePlayers[1]==1){
		cardCount_p2 = countPlayerCards(2);
		N782_print    ( 3 , VRAM_TILES_V-3 , S_P2   , 0 ) ;
		DrawMap2      ( 0 , VRAM_TILES_V-3 , arrow_blue  );
		DrawMap2      ( 1 , VRAM_TILES_V-3 , arrow_blue  );
		N782_print    ( 0 , VRAM_TILES_V-2 , S_CARDS, 0 ) ;
		Fill(2 , VRAM_TILES_V-1, 3,1, 0);
		N782_print_u8 ( 2 , VRAM_TILES_V-1 , cardCount_p2) ;
		Fill(1  , VRAM_TILES_V-4, 4,1, 0x00);
		if( cardCount_p2==1 ) { N782_print( 1  , VRAM_TILES_V-4, S_UNO , 0 ) ; }
	}
	// P3
	if(game.activePlayers[2]==1){
		cardCount_p3 = countPlayerCards(3);
		N782_print    ( 0 , 0 , S_P3   ,0 ) ;
		DrawMap2      ( 3 , 0 , arrow_red   );
		DrawMap2      ( 4 , 0 , arrow_red   );
		N782_print    ( 0 , 1 , S_CARDS,0)  ;
		Fill(2 , 2, 3,1, 0);
		N782_print_u8 ( 2 , 2 , cardCount_p3) ;
		Fill(1  , 3             , 4,1, 0x00);
		if( cardCount_p3==1 ) { N782_print( 1  , 3             , S_UNO , 0 ) ; }
	}
	// P4
	if(game.activePlayers[3]==1){
		cardCount_p4 = countPlayerCards(4);
		N782_print    ( 23 , 0 , S_P4   ,0 ) ;
		DrawMap2      ( 26 , 0 , arrow_green );
		DrawMap2      ( 27 , 0 , arrow_green );
		N782_print    ( 23 , 1 , S_CARDS,0 ) ;
		Fill(25 , 2, 3,1, 0);
		N782_print_u8 ( 25 , 2 , cardCount_p4) ;
		Fill(24 , 3             , 4,1, 0x00);
		if( cardCount_p4==1 ) { N782_print( 24 , 3             , S_UNO , 0 ) ; }
	}

}

// DONE
static void gstate_title_n782(){
	ClearVram();

	// nicksen782 lense flare graphic.
	ClearVram();
	SetTileTable( bg_tiles2 );
	DrawMap2(11,9,N782_F1);WaitVsync(10);
	DrawMap2(11,9,N782_F2);WaitVsync(10);
	DrawMap2(11,9,N782_F3);WaitVsync(10);
	DrawMap2(11,9,N782_F4);WaitVsync(10);
	DrawMap2(11,9,N782_F5);WaitVsync(10);
	DrawMap2(11,9,N782_F4);WaitVsync(8);
	DrawMap2(11,9,N782_F3);WaitVsync(8);
	DrawMap2(11,9,N782_F2);WaitVsync(8);
	DrawMap2(11,9,N782_F1);WaitVsync(8);
	WaitVsync(25);
	ClearVram();
	SetTileTable( bg_tiles );

	while(game.gamestate2==GSTATE_TITLE_N782    ){
		break;
	}

	game.gamestate2=GSTATE_TITLE_UZEBOX;
}
static void gstate_title_uzebox(){
	ClearVram();
	SetTileTable( bg_tiles );

	N782_print( (VRAM_TILES_H/2) - (6/2), (VRAM_TILES_V/2) - 1 ,PSTR("UZEBOX"), 0);

	while(game.gamestate2==GSTATE_TITLE_UZEBOX  ){
		break;
	}

	WaitVsync(50);
	game.gamestate2=GSTATE_TITLE_MAINMENU;
}
static void gstate_title_mainmenu(){
	ClearVram();
	SetTileTable( bg_tiles );

	DrawMap2(0, 0, titlescreen_28x28);

	// Color flash setup.
	u8 * colorTimer = &vsynccounter8b_gen2;
	u8 currentTile = 1;
	u8 color_yellow = pgm_read_byte(&(border_yellow[2]));
	u8 color_blue   = pgm_read_byte(&(border_blue  [2]));
	u8 color_red    = pgm_read_byte(&(border_red   [2]));
	u8 color_green  = pgm_read_byte(&(border_green [2]));
	u8 color_black  = pgm_read_byte(&(border_black [2]));
	*colorTimer = 5;
	// Copy the tile to a ramtile. Replace instances of that tile in vram with the ramtile.
	CopyFlashTile(color_yellow, 2);
	for(u8 y=0; y<VRAM_TILES_V; y+=1){
		for(u8 x=0; x<VRAM_TILES_H; x+=1){
			if(vram[(y*VRAM_TILES_H)+x]==color_yellow+RAM_TILES_COUNT){
				vram[(y*VRAM_TILES_H)+x]=2;
			}
		}
	}
	CopyFlashTile(color_green, 2);

	// Cursor config.
	u8 * cursorTimer = &vsynccounter8b_gen1;
	u8 currentCursor = 0;
	u8 *cursor1map;
	u8 *cursor2map;
	u8 cursorIndex;
	u8 cursor_x=4;
	u8 cursor_y=16;
	u8 y_rows[] = {
		16,18,20,22
	};
	u8 y_rows_index = 0;
	cursor1map=cursor_f1_r;
	cursor2map=cursor_f2_r;
	*cursorTimer=15;

	while(game.gamestate1==GSTATE_TITLE){
		// Read the gamepad.
		getInputs();

		// Check for softReset request.
		if( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }

		// Move cursor.
		if( (game.btnHeld1 & BTN_UP) ||  (game.btnHeld1 & BTN_DOWN) ) {
			if( (game.btnHeld1 & BTN_UP  ) ){
				if(y_rows_index !=0) { y_rows_index--; }
				else{ y_rows_index = sizeof(y_rows)-1; }
			}

			if( (game.btnHeld1 & BTN_DOWN) ){
				if(y_rows_index !=sizeof(y_rows)-1) { y_rows_index++; }
				else{ y_rows_index = 0; }
			}

			cursor_y = y_rows[y_rows_index] ;
			MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);

			while ( game.btnHeld1 ){
				getInputs();
				WaitVsync(1);
			}

		}

		// Make selection.
		if( (game.btnHeld1 & BTN_A) || (game.btnHeld1 & BTN_START)){
			// Game setup (local).
			if     (y_rows_index==0){
				game.gamestate1=GSTATE_OPTIONS;
				game.gamestate2=GSTATE_OPTIONS_2;
			}

			// Game setup (Uzenet).
			else if(y_rows_index==1){
				game.gamestate1=GSTATE_OPTIONS;
				game.gamestate2=GSTATE_OPTIONS_1;
			}

			// Rules
			else if(y_rows_index==2){
				game.gamestate1=GSTATE_RULES;
				game.gamestate2=GSTATE_RULES_1;
			}

			// Credits
			else if(y_rows_index==3){
				game.gamestate1=GSTATE_CREDITS;
				game.gamestate2=GSTATE_CREDITS_1;
			}

			// Hide the cursor.
			MapSprite2( 0, cursor1map, SPRITE_OFF);

			// Wait for the player to release the button.
			while ( game.btnHeld1 ){ getInputs(); WaitVsync(1); }

			break;
		}

		// Update the cursor.
		if(*cursorTimer > 10 ){
			// Change to which map?
			if(currentCursor==0){ MapSprite2( 0, cursor1map, SPRITE_BANK0 ); }
			else                { MapSprite2( 0, cursor2map, SPRITE_BANK0 ); }

			// Redraw the sprite.
			MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);

			// Flip the cursor frame.
			currentCursor = !currentCursor;

			// Reset the counter.
			*cursorTimer=0;
		}

		// Update the color flash.
		if(*colorTimer > 10 ){
			// Change to which map?
			switch(currentTile){
				case 0  : { CopyFlashTile(color_yellow, 2); currentTile++; break; }
				case 1  : { CopyFlashTile(color_blue  , 2); currentTile++; break; }
				case 2  : { CopyFlashTile(color_red   , 2); currentTile++; break; }
				case 3  : { CopyFlashTile(color_green , 2); currentTile=0; break; }
				default : { break; }
			};

			// Reset the counter.
			*colorTimer=0;
		}

		WaitVsync(1);
	}
}
static void gstate_title(){
	while(game.gamestate1==GSTATE_TITLE){
		if     (game.gamestate2==GSTATE_TITLE_N782    ){
			gstate_title_n782();
		}
		else if(game.gamestate2==GSTATE_TITLE_UZEBOX  ){
			gstate_title_uzebox();
		}
		else if(game.gamestate2==GSTATE_TITLE_MAINMENU){
			gstate_title_mainmenu();
		}
		else{
			errorHandler(INVALIDGAMESTATE2);
		}
	}
}

//
static void gstate_options1(){
	// Uzenet setup
	DrawMap2(0,0,options1_28x28);
	while(game.gamestate2==GSTATE_OPTIONS_1){
		getInputs();
		if( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }
		if( (game.btnHeld1) ) {
			while ( game.btnHeld1 ){ getInputs(); WaitVsync(1); }
			game.gamestate1=GSTATE_OPTIONS;
			game.gamestate2=GSTATE_OPTIONS_2;
		}
	}

}
static void gstate_options2(){
	DrawMap2(0,0,options2_28x28);

	// Four boxes.
		// Player setup
			// 4 rows.
			// Each row can be adjusted to have the value of "HUMAN", "CPU", "NONE".
		// CPU AI Setup
			// 3 rows.
			// Each row can be adjusted to have the value of "SIMPLE", "HARDER", "EXPERT".
		// Win Style
			// 2 rows.
			// Each row can be adjusted to have the value of "FIRST TO ZERO CARDS", "FIRST TO 500 POINTS".
		// No Playable Card.
			// 2 rows.
			// Each row can be adjusted to have the value of "DRAW ONLY ONE CARD", "DRAW UNTIL PLAYABLE".
	// DPAD changes the cursor. DPAD changes the option for the Players box.
	// A accepts the selection(s) and moves to the next box.
		// On the last accepted box, the game starts.
	// B moves back to the previous box. Settings remain the same until changed.

	u8 currentBox=0;

	u8 box1_y_index=0;
	u8 box2_y_index=0;
	u8 box3_y_index=0;
	u8 box4_y_index=0;

	u8 box1_y_rows[4] = { 5 , 6  ,7, 8 };
	u8 box2_y_rows[3] = { 5 , 6  ,7 };
	u8 box3_y_rows[2] = { 14, 15 };
	u8 box4_y_rows[2] = { 21, 22 };

	u8 cursor_box1_x=3;
	u8 cursor_box1_y=box1_y_rows[box1_y_index];

	u8 cursor_box2_x=16;
	u8 cursor_box2_y=box2_y_rows[box2_y_index];

	u8 cursor_box3_x=3;
	u8 cursor_box3_y=box3_y_rows[box3_y_index];

	u8 cursor_box4_x=3;
	u8 cursor_box4_y=box4_y_rows[box4_y_index];

	// Cursor timer and current frame.
	u8 * cursorTimer = &vsynccounter8b_gen1;
	*cursorTimer=15;
	u8 currentCursor = 0;

	u8 nextX    ;
	u8 nextY    ;
	u8 nextType ;

	// These values will be copied to the game struct.
	u8 p1_type   = game.player_types[0] ;
	u8 p2_type   = game.player_types[1] ;
	u8 p3_type   = game.player_types[2] ;
	u8 p4_type   = game.player_types[3] ;
	u8 ai_type   = game.ai_type         ;
	u8 winStyle  = game.winStyle        ;
	u8 drawStyle = game.drawStyle       ;

	// Game setup
	while(game.gamestate2==GSTATE_OPTIONS_2){
		// Change the cursor frame?
		if(*cursorTimer > 15 ){
			// Hide all cursors.
			MapSprite2( 0, cursor_f1_r, SPRITE_OFF );
			MapSprite2( 1, cursor_f1_r, SPRITE_OFF );
			MapSprite2( 2, cursor_f1_r, SPRITE_OFF );
			MapSprite2( 3, cursor_f1_r, SPRITE_OFF );

			if     (currentBox==0){
				if     (currentCursor==0){
					MapSprite2( currentBox, cursor_f1_r, SPRITE_BANK0 );
					MoveSprite( currentBox, cursor_box1_x<<3 , cursor_box1_y<<3, 1, 1);
				}
				else                     {
					MapSprite2( currentBox, cursor_f2_r, SPRITE_BANK0 );
					MoveSprite( currentBox, cursor_box1_x<<3 , cursor_box1_y<<3, 1, 1);
				}
			}
			else if(currentBox==1){
				if     (currentCursor==0){
					MapSprite2( currentBox, cursor_f1_r, SPRITE_BANK0 );
					MoveSprite( currentBox, cursor_box2_x<<3 , cursor_box2_y<<3, 1, 1);
				}
				else                     {
					MapSprite2( currentBox, cursor_f2_r, SPRITE_BANK0 );
					MoveSprite( currentBox, cursor_box2_x<<3 , cursor_box2_y<<3, 1, 1);
				}
			}
			else if(currentBox==2){
				if     (currentCursor==0){
					MapSprite2( currentBox, cursor_f1_r, SPRITE_BANK0 );
					MoveSprite( currentBox, cursor_box3_x<<3 , cursor_box3_y<<3, 1, 1);
				}
				else                     {
					MapSprite2( currentBox, cursor_f2_r, SPRITE_BANK0 );
					MoveSprite( currentBox, cursor_box3_x<<3 , cursor_box3_y<<3, 1, 1);
				}
			}
			else if(currentBox==3){
				if     (currentCursor==0){
					MapSprite2( currentBox, cursor_f1_r, SPRITE_BANK0 );
					MoveSprite( currentBox, cursor_box4_x<<3 , cursor_box4_y<<3, 1, 1);
				}
				else                     {
					MapSprite2( currentBox, cursor_f1_r, SPRITE_BANK0 );
					MoveSprite( currentBox, cursor_box4_x<<3 , cursor_box4_y<<3, 1, 1);
				}
			}

			// Flip the cursor frame.
			currentCursor = !currentCursor;

			// Reset the counter.
			*cursorTimer=0;
		}

		// Read gamepad.
		getInputs();

		// Look for the softReset command.
		if( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }

		// If button is held...
		if( (game.btnHeld1) ) {
			// "PLAYERS" box.
			if     (currentBox==0){
				// Move the cursor up or down.
				if( (game.btnHeld1 & BTN_UP) || (game.btnHeld1 & BTN_DOWN) ) {
					// Move cursor UP (do bounds check.)
					if( (game.btnHeld1 & BTN_UP   ) ) {
						box1_y_index = box1_y_index==0                     ? sizeof(box1_y_rows)-1 : box1_y_index-1;
					}
					// Move cursor DOWN (do bounds check.)
					if( (game.btnHeld1 & BTN_DOWN ) ) {
						box1_y_index = box1_y_index==sizeof(box1_y_rows)-1 ? 0                     : box1_y_index+1;
					}
					// Adjust cursor y position.
					cursor_box1_y=box1_y_rows[box1_y_index];
					// Redraw cursor.
					MoveSprite( 0 , cursor_box1_x<<3 , cursor_box1_y<<3, 1, 1);
				}
				// Change the selected option.
				else if( (game.btnHeld1 & BTN_LEFT) || (game.btnHeld1 & BTN_RIGHT) ) {
					if     (box1_y_index==0){
						// Set the new player type. (Do bounds check.)
						p1_type = p1_type==0 ? 2 : p1_type-1;
						// Set the nextX to print from.
						nextX=cursor_box1_x+4;
						// Set the nextY to print from.
						nextY=box1_y_rows[box1_y_index];
						// Set the nextType to print.
						nextType=p1_type;
					}
					else if(box1_y_index==1){
						p2_type = p2_type==0 ? 2 : p2_type-1;
						nextX=cursor_box1_x+4;
						nextY=box1_y_rows[box1_y_index];
						nextType=p2_type;
					}
					else if(box1_y_index==2){
						p3_type = p3_type==0 ? 2 : p3_type-1;
						nextX=cursor_box1_x+4;
						nextY=box1_y_rows[box1_y_index];
						nextType=p3_type;
					}
					else if(box1_y_index==3){
						p4_type = p4_type==0 ? 2 : p4_type-1;
						nextX=cursor_box1_x+4;
						nextY=box1_y_rows[box1_y_index];
						nextType=p4_type;
					}

					// Erase the area that is about to be printed to.
					Fill(cursor_box1_x+4, box1_y_rows[box1_y_index], 5,1,0x00);
					// Print the player type.
					switch(nextType){
						case HUMAN : { N782_print(nextX, nextY, S_HUMAN, 0); break; }
						case CPU   : { N782_print(nextX, nextY, S_CPU  , 0); break; }
						case NONE  : { N782_print(nextX, nextY, S_NONE , 0); break; }
					};
				}
				// Confirm selection, move to the next box.
				if( (game.btnHeld1 & BTN_A    ) ) {
					currentBox++;
				}
				// Return to the title screen.
				if( (game.btnHeld1 & BTN_B    ) ) {
					// Go back to the title screen.
					game.gamestate1=GSTATE_TITLE;
					game.gamestate2=GSTATE_TITLE_MAINMENU;
				}
			}
			// "CPU AI" box.
			else if(currentBox==1){
				// Move the cursor up or down.
				if( (game.btnHeld1 & BTN_UP) || (game.btnHeld1 & BTN_DOWN) ) {
					// Move cursor UP (do bounds check.)
					if( (game.btnHeld1 & BTN_UP   ) ) {
						box2_y_index = box2_y_index==0                     ? sizeof(box2_y_rows)-1 : box2_y_index-1;
					}
					// Move cursor DOWN (do bounds check.)
					if( (game.btnHeld1 & BTN_DOWN ) ) {
						box2_y_index = box2_y_index==sizeof(box2_y_rows)-1 ? 0                     : box2_y_index+1;
					}
					//
					ai_type=box2_y_index;
					// Adjust cursor y position.
					cursor_box2_y=box2_y_rows[box2_y_index];
					// Redraw cursor.
					MoveSprite( 1 , cursor_box2_x<<3 , cursor_box2_y<<3, 1, 1);
				}
				// Confirm selection, move to the next box.
				if( (game.btnHeld1 & BTN_A    ) ) {
					currentBox++;
				}
				// Move to the previous box.
				if( (game.btnHeld1 & BTN_B    ) ) {
					currentBox--;
				}
			}
			// "WIN STYLE" box.
			else if(currentBox==2){
				// winStyle
				// Move the cursor up or down.
				if( (game.btnHeld1 & BTN_UP) || (game.btnHeld1 & BTN_DOWN) ) {
					// Move cursor UP (do bounds check.)
					if( (game.btnHeld1 & BTN_UP   ) ) {
						box3_y_index = box3_y_index==0                     ? sizeof(box3_y_rows)-1 : box3_y_index-1;
					}
					// Move cursor DOWN (do bounds check.)
					if( (game.btnHeld1 & BTN_DOWN ) ) {
						box3_y_index = box3_y_index==sizeof(box3_y_rows)-1 ? 0                     : box3_y_index+1;
					}
					//
					winStyle=box3_y_index;
					// Adjust cursor y position.
					cursor_box3_y=box3_y_rows[box3_y_index];
					// Redraw cursor.
					MoveSprite( 1 , cursor_box3_x<<3 , cursor_box3_y<<3, 1, 1);
				}
				// Confirm selection, move to the next box.
				if( (game.btnHeld1 & BTN_A    ) ) {
					currentBox++;
				}
				// Move to the previous box.
				if( (game.btnHeld1 & BTN_B    ) ) {
					currentBox--;
				}
			}
			// "DRAW STYLE" box.
			else if(currentBox==3){
				// Move the cursor up or down.
				if( (game.btnHeld1 & BTN_UP) || (game.btnHeld1 & BTN_DOWN) ) {
					// Move cursor UP (do bounds check.)
					if( (game.btnHeld1 & BTN_UP   ) ) {
						box4_y_index = box4_y_index==0                     ? sizeof(box4_y_rows)-1 : box4_y_index-1;
					}
					// Move cursor DOWN (do bounds check.)
					if( (game.btnHeld1 & BTN_DOWN ) ) {
						box4_y_index = box4_y_index==sizeof(box4_y_rows)-1 ? 0                     : box4_y_index+1;
					}
					//
					drawStyle=box4_y_index;
					// Adjust cursor y position.
					cursor_box4_y=box4_y_rows[box4_y_index];
					// Redraw cursor.
					MoveSprite( 1 , cursor_box4_x<<3 , cursor_box4_y<<3, 1, 1);
				}
				// Confirm selection. Save settings and start the game.
				if( (game.btnHeld1 & BTN_A    ) ) {
					// Hide all cursors.
					MapSprite2( 0, cursor_f1_r, SPRITE_OFF );
					MapSprite2( 1, cursor_f1_r, SPRITE_OFF );
					MapSprite2( 2, cursor_f1_r, SPRITE_OFF );
					MapSprite2( 3, cursor_f1_r, SPRITE_OFF );

					// Save the settings to the game struct.

					// Count and set the active players. (Either HUMAN or CPU.)
					game.numPlayers    =  0                         ;
					if( p1_type==HUMAN || p1_type==CPU ) { game.numPlayers+=1; game.activePlayers[0]=1; } else{ game.activePlayers[0]=0 ; }
					if( p2_type==HUMAN || p2_type==CPU ) { game.numPlayers+=1; game.activePlayers[1]=1; } else{ game.activePlayers[1]=0 ; }
					if( p3_type==HUMAN || p3_type==CPU ) { game.numPlayers+=1; game.activePlayers[2]=1; } else{ game.activePlayers[2]=0 ; }
					if( p4_type==HUMAN || p4_type==CPU ) { game.numPlayers+=1; game.activePlayers[3]=1; } else{ game.activePlayers[3]=0 ; }

					// Set the player types.
					game.player_types[0] = p1_type;
					game.player_types[1] = p2_type;
					game.player_types[2] = p3_type;
					game.player_types[3] = p4_type;

					// Set the ai_type, winStyle, drawStyle.
					game.ai_type        = ai_type   ;
					game.winStyle       = winStyle  ;
					game.drawStyle      = drawStyle ;

					// Reset accumulated points.
					game.points_p1      = 0         ;
					game.points_p2      = 0         ;
					game.points_p3      = 0         ;
					game.points_p4      = 0         ;

					// Set active player to invalid, last card played to invalid, direction to forward.
					game.activePlayer   = 255       ;
					game.lastCardPlayed = 255       ;
					game.direction      = FORWARD   ;

					// Start a new game.
					game.gamestate1=GSTATE_PLAYING;
					game.gamestate2=GSTATE_PLAYING_GAMESTART;

#if VRAM_TILES_H == 32
					// DEBUG -
					ClearVram();
					u8 x=(VRAM_TILES_H/2)-13/2;
					u8 y=(VRAM_TILES_V/2)-17/2;
					// DEBUG -

					// DEBUG - "PLAYERS" box.
					N782_print(x,y , PSTR("p1_type:   "), 0); N782_print_u8(x+10,y,game.player_types[0]);y++;
					N782_print(x,y , PSTR("p2_type:   "), 0); N782_print_u8(x+10,y,game.player_types[1]);y++;
					N782_print(x,y , PSTR("p3_type:   "), 0); N782_print_u8(x+10,y,game.player_types[2]);y++;
					N782_print(x,y , PSTR("p4_type:   "), 0); N782_print_u8(x+10,y,game.player_types[3]);y++;
					y++;
					// DEBUG - "PLAYERS" box.

					// DEBUG - "CPU AI" box.
					N782_print(x,y , PSTR("ai_type:   "), 0); N782_print_u8(x+10,y,ai_type);y++;
					y++;
					// DEBUG - "CPU AI" box.

					// DEBUG - "WIN STYLE" box.
					N782_print(x,y , PSTR("winStyle:  "), 0); N782_print_u8(x+10,y,winStyle);y++;
					y++;
					// DEBUG - "WIN STYLE" box.

					// DEBUG - "DRAW STYLE" box.
					N782_print(x,y , PSTR("drawStyle: "), 0); N782_print_u8(x+10,y,drawStyle);y++;
					y++;
					// DEBUG - "DRAW STYLE" box.

					// DEBUG - Number of players.
					N782_print(x,y , PSTR("numPlayers:"), 0); N782_print_u8(x+10,y,game.numPlayers);y++;
					y++;
					// DEBUG - Number of players.

					// DEBUG - Active players.
					N782_print(x,y , PSTR("p1 active: "), 0); N782_print_u8(x+10,y,game.activePlayers[0]);y++;
					N782_print(x,y , PSTR("p2 active: "), 0); N782_print_u8(x+10,y,game.activePlayers[1]);y++;
					N782_print(x,y , PSTR("p3 active: "), 0); N782_print_u8(x+10,y,game.activePlayers[2]);y++;
					N782_print(x,y , PSTR("p4 active: "), 0); N782_print_u8(x+10,y,game.activePlayers[3]);y++;
					y++;
					// DEBUG - Active players.

					// While a button is still held...
					while ( game.btnHeld1 ){ getInputs(); }

					// While no button is held...
					while ( game.btnHeld1==0 ){ getInputs(); }

					// While a button is still held...
					while ( game.btnHeld1 ){ getInputs(); }
#endif

					// ClearVram();
					// btnPressed1
					getRandomSeedViaUserInput();
					srand(game.randomSeed);

					break;
				}
				// Move to the previous box.
				if( (game.btnHeld1 & BTN_B    ) ) {
					currentBox--;
				}
			}

			// While a button is still held...
			while ( game.btnHeld1 ){ getInputs(); }
		}

	}
}

static void gstate_options(){
	while(game.gamestate1==GSTATE_OPTIONS){
		if     (game.gamestate2==GSTATE_OPTIONS_1){ gstate_options1(); }
		else if(game.gamestate2==GSTATE_OPTIONS_2){ gstate_options2(); }
		else{
			errorHandler(INVALIDGAMESTATE2);
		}
	}
}

// DONE
static void gstate_credits1(){
	DrawMap2(0,0,credits1_28x28);
	while(game.gamestate2==GSTATE_CREDITS_1){
		getInputs();
		if( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }
		if( (game.btnHeld1) ) {
			while ( game.btnHeld1 ){ getInputs(); WaitVsync(1); }
			game.gamestate1=GSTATE_CREDITS;
			game.gamestate2=GSTATE_CREDITS_2;
		}
	}

}
static void gstate_credits2(){
	DrawMap2(0,0,credits2_28x28);

	// Game setup
	while(game.gamestate2==GSTATE_CREDITS_2){
		getInputs();
		if( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }
		if( (game.btnHeld1) ) {
			while ( game.btnHeld1 ){ getInputs(); WaitVsync(1); }
			game.gamestate1=GSTATE_TITLE;
			game.gamestate2=GSTATE_TITLE_MAINMENU;
		}
	}

}
static void gstate_credits(){
	while(game.gamestate1==GSTATE_CREDITS){
		if     (game.gamestate2==GSTATE_CREDITS_1){ gstate_credits1(); }
		else if(game.gamestate2==GSTATE_CREDITS_2){ gstate_credits2(); }
		else{
			errorHandler(INVALIDGAMESTATE2);
		}
	}
}

// DONE
static void gstate_rules_1(){
	DrawMap2(0,0,rules1_28x28);
	while(game.gamestate2==GSTATE_RULES_1){
		getInputs();
		if     ( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }
		else if( (game.btnHeld1) ) {
			while ( game.btnHeld1 ){ getInputs(); WaitVsync(1); }
			game.gamestate1=GSTATE_RULES;
			game.gamestate2=GSTATE_RULES_2;
		}
	}
}
static void gstate_rules_2(){
	DrawMap2(0,0,rules2_28x28);
	while(game.gamestate2==GSTATE_RULES_2){
		getInputs();
		if     ( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }
		else if( (game.btnHeld1) ) {
			while ( game.btnHeld1 ){ getInputs(); WaitVsync(1); }
			game.gamestate1=GSTATE_TITLE;
			game.gamestate2=GSTATE_TITLE_MAINMENU;
		}
	}
}
static void gstate_rules(){
	while(game.gamestate1==GSTATE_RULES){
		if     (game.gamestate2==GSTATE_RULES_1){ gstate_rules_1(); }
		else if(game.gamestate2==GSTATE_RULES_2){ gstate_rules_2(); }
		else{
			errorHandler(INVALIDGAMESTATE2);
		}
	}
}

// Resets the game for a new round. (Does NOT impact scores.)
static void newGameSetup(){
	// Clear all reserved ramtile data.
	clearAllReservedRamtiles();

	u8 playerWithHighestCard(u8 *p_cards){
		// Reset the deck.
		resetTheDeck();

		// Assign 1 card to each player.
		if(game.activePlayers[0]==1){ getCardFromDrawPile(1, 1); }
		if(game.activePlayers[1]==1){ getCardFromDrawPile(2, 1); }
		if(game.activePlayers[2]==1){ getCardFromDrawPile(3, 1); }
		if(game.activePlayers[3]==1){ getCardFromDrawPile(4, 1); }

		u8 p1_card;
		u8 p2_card;
		u8 p3_card;
		u8 p4_card;

		u8 highest     = 0  ;
		u8 winnerCount = 0  ;
		u8 winner      = 255;

		// Go through all the cards and determine which card is held by each player.
		for(u8 i=0; i<totalCardsInDeck; i+=1){
			switch(cards[i].location){
				case CARD_LOCATION_PLAYER1 : { p1_card=cards[i].value; p_cards[0]=i; break; }
				case CARD_LOCATION_PLAYER2 : { p2_card=cards[i].value; p_cards[1]=i; break; }
				case CARD_LOCATION_PLAYER3 : { p3_card=cards[i].value; p_cards[2]=i; break; }
				case CARD_LOCATION_PLAYER4 : { p4_card=cards[i].value; p_cards[3]=i; break; }
				default :{ break; }
			};

		}

		// Each player draws a card.

		// Any card that is not a number card is to be interpreted as 0.
		if(game.activePlayers[0]==1){ if( p1_card >= CARD_DRAW2 && p1_card <= CARD_WILD_DRAW4 ){ p1_card = CARD_0; } }
		if(game.activePlayers[1]==1){ if( p2_card >= CARD_DRAW2 && p2_card <= CARD_WILD_DRAW4 ){ p2_card = CARD_0; } }
		if(game.activePlayers[2]==1){ if( p3_card >= CARD_DRAW2 && p3_card <= CARD_WILD_DRAW4 ){ p3_card = CARD_0; } }
		if(game.activePlayers[3]==1){ if( p4_card >= CARD_DRAW2 && p4_card <= CARD_WILD_DRAW4 ){ p4_card = CARD_0; } }

		// Determine which card was the highest.
		if(game.activePlayers[0]==1){ if( p1_card > highest ){ highest = p1_card; } }
		if(game.activePlayers[1]==1){ if( p2_card > highest ){ highest = p2_card; } }
		if(game.activePlayers[2]==1){ if( p3_card > highest ){ highest = p3_card; } }
		if(game.activePlayers[3]==1){ if( p4_card > highest ){ highest = p4_card; } }

		// How many players have the highest card?
		if(game.activePlayers[0]==1){ if( p1_card == highest ){ winner = 1; winnerCount+=1; } }
		if(game.activePlayers[1]==1){ if( p2_card == highest ){ winner = 2; winnerCount+=1; } }
		if(game.activePlayers[2]==1){ if( p3_card == highest ){ winner = 3; winnerCount+=1; } }
		if(game.activePlayers[3]==1){ if( p4_card == highest ){ winner = 4; winnerCount+=1; } }

		// The player that draws the highest number is the first player.

		// There should only be one winner. Check the second index.
		// If it is non-zero then this test is invalid.

		// Valid! Return the winner.
		if( winnerCount == 1 && winner != 0 ){
			game.activePlayer = winner;
			return winner ;
		}
		else{
			return 255;
		}

	}
	void placeInitialDiscardCard(){
		// Determine first discard:
		//  Take one card from the Draw Pile to the Discard Pile.
		//   Repeat if the card was a Wild Draw 4.
		//  If the card is an action card then the action will happen to the first player.
		//   Including wild. The user will have the option to choose what starting color.
		u8 cardIndex2;

		while(1){
			// Generate random number (index into cards array.)
			cardIndex2 = getRandomNumber(0, totalCardsInDeck-1);
			// cardIndex2=56;

			// Is the card available?
			if( cards[cardIndex2].location==CARD_LOCATION_DRAW ) {
				// Is it the Wild Draw 4 card? (If so, then skip.)
				if(cards[cardIndex2].value==CARD_WILD_DRAW4){ continue; }

				// Assign the card to the Discard Pile.
				else{
					// Assign the card to the Discard Pile.
					cards[cardIndex2].location=CARD_LOCATION_DISCARD;

					// Remove the discard card. (Always do this first.)
					removeCard_lg();

					// Set as the last played card.
					game.lastCardPlayed                 = cardIndex2;

					// DEBUG
					// game.lastCardPlayed                 = 106; // REV   - GREEN
					// game.lastCardPlayed                 = 27 ; // DRAW2 - YELLOW
					// game.lastCardPlayed                 = 54 ; // SKIP  - BLUE
					// game.lastCardPlayed                 = 0  ; // WILD
					// game.lastCardPlayed                 = 4  ; // WILD DRAW 4
					// DEBUG

					// Update the played card's position in the cards array.
					cards[game.lastCardPlayed].location = CARD_LOCATION_DISCARD;

					// Update the game direction.
					if(cards[game.lastCardPlayed].value==CARD_REV){
						// game.direction = !game.direction;
					}

					// Update the discard color.
					// game.discardColor = cards[game.lastCardPlayed].color;

					// Draw the card.
					drawCard(
						255                              ,
						pgm_read_byte(&(discard_pos[0])) ,
						pgm_read_byte(&(discard_pos[1])) ,
						LARGE_CARD                       ,
						cards[game.lastCardPlayed].value ,
						cards[game.lastCardPlayed].color
					);

					// Redraw the stack under the card.
					redrawUnderDiscardPile();

					// End the loop.
					break;
				}
			}
		}

		redrawUnderDrawPile();
	}

	// Draw the game board.
	DrawMap2(0, 0, board_28x28);

	game.lastCardPlayed=255;

	redrawMainInfoRegion();

	u8 p_cards[4] = { 255 } ;
	u8 pos;

	// Determine which player goes first. (Assign one card to each player.)
	// Make sure that players cards cannot match.
	while(1){
		//
		playerWithHighestCard(&p_cards);

		// Valid response?
		if( game.activePlayer != 255 ){
			WaitVsync(20);

			// dealSpecifiecCard_anim(u8 playerNum, u8 cardIndex, u8 cardPos, u8 cardDelay);

			if(game.activePlayers[0]==1){ dealSpecifiedCard_anim(1, p_cards[0], 2, 5, CARDS_FACEUP); }
			if(game.activePlayers[1]==1){ dealSpecifiedCard_anim(2, p_cards[1], 2, 5, CARDS_FACEUP); }
			if(game.activePlayers[2]==1){ dealSpecifiedCard_anim(3, p_cards[2], 2, 5, CARDS_FACEUP); }
			if(game.activePlayers[3]==1){ dealSpecifiedCard_anim(4, p_cards[3], 2, 5, CARDS_FACEUP); }

			debug_showDebugData();

			// Display who plays first!
			msgHandler(GAMESTART_FIRST);

			break;
		}

		// Invalid response. Loop again.
	}

	// Wait a moment to allow the user to read the displayed message.
	WaitVsync(125);

	// Clear the message. (The message at the lower half of the game board.)
	clearbgmessage();

	// Remove the cards temporarily assigned to each player. (ram tiles and display only.)
	if(game.activePlayers[0]==1){ removePlayerCards(1); }
	if(game.activePlayers[1]==1){ removePlayerCards(2); }
	if(game.activePlayers[2]==1){ removePlayerCards(3); }
	if(game.activePlayers[3]==1){ removePlayerCards(4); }

	// Reset the deck. (Return all cards back to the CARD_LOCATION_DRAW location.)
	resetTheDeck();

	// Assign cards to each player. (Does not display the cards.)
	for(u8 i=0; i<defaultHandSize; i+=1){
		if(i%5==0) { pos=0; }

		// Assign a card to player 1.
		if(game.activePlayers[0]==1){ dealSpecifiedCard_anim(1, 255, pos, 10, CARDS_FACEDOWN); }
		// Assign a card to player 2.
		if(game.activePlayers[1]==1){ dealSpecifiedCard_anim(2, 255, pos, 10, CARDS_FACEDOWN); }
		// Assign a card to player 3.
		if(game.activePlayers[2]==1){ dealSpecifiedCard_anim(3, 255, pos, 10, CARDS_FACEDOWN); }
		// Assign a card to player 4.
		if(game.activePlayers[3]==1){ dealSpecifiedCard_anim(4, 255, pos, 15, CARDS_FACEDOWN); }

		updatePlayerDisplayedData();

		pos++;

	}

	// Place an initial discard card and then redraw the stack under the Discard Pile.
	placeInitialDiscardCard();

	// Update the displayed player data.
	updatePlayerDisplayedData();

	// Update the displayed color, direction, draw pile, discard pile and last played card on discard pile.
	redrawMainInfoRegion();

	WaitVsync(50);
}

static void gstate_playing(){
	ClearVram();
	game.gamestate1=GSTATE_PLAYING;

	//
	void drawFaceDownCard(u8 playerNum, u8 position){
		// This function assumes that the cards have been removed first.
		u8 x,y;
		const char * card ;

		// Get the x,y,w,h values.
		switch(playerNum){
			case 1 : {
				x=pgm_read_byte(&(p1_pos[position][0]));
				y=pgm_read_byte(&(p1_pos[position][1]));
				card=card_back_sm_0deg;
				break;
			}
			case 2 : {
				x=pgm_read_byte(&(p2_pos[position][0]));
				y=pgm_read_byte(&(p2_pos[position][1]));
				card=card_back_sm_neg90deg;
				break;
			}
			case 3 : {
				x=pgm_read_byte(&(p3_pos[position][0]));
				y=pgm_read_byte(&(p3_pos[position][1]));
				card=card_back_sm_360deg;
				break;
			}
			case 4 : {
				x=pgm_read_byte(&(p4_pos[position][0]));
				y=pgm_read_byte(&(p4_pos[position][1]));
				card=card_back_sm_90deg;
				break;
			}
			default : { return; break; }
		};

		DrawMap2(x, y, card);
	}
	// Only works with the cards of the specified player.
	void displayCards_byPlayer(u8 playerNum, u8 option){
		// Get the count of this player's cards.
		u8 numCards = countPlayerCards(playerNum) ;

		if( option==CARDS_FACEDOWN ){
			for(u8 pos=0; pos<5; pos+=1){
				// Remove the existing card.
				removeCard_sm(playerNum, pos);

				// Draw the card face down.
				if(numCards>pos){ drawFaceDownCard(playerNum, pos); }

				// Remove and hide the card.
				else            { removeCard_sm(playerNum, pos); }
			}
		}

		if( option==CARDS_ACTIVEPLAYERFACEUP || option==CARDS_FACEUP ){
			// This does remove the existing card before drawing the new card.
			displayCardsForPlayer(game.activePlayer, game.handRow*5, 1);
		}

	}
	//
	void getCursorPos(u8 playerNum, u8 cursorIndex, u8 *x, u8 *y){
		if     (playerNum==1){
			*x = pgm_read_byte(&(p1_pos_cursor[cursorIndex][0]));
			*y = pgm_read_byte(&(p1_pos_cursor[cursorIndex][1]));
		}
		else if(playerNum==2){
			*x = pgm_read_byte(&(p2_pos_cursor[cursorIndex][0]));
			*y = pgm_read_byte(&(p2_pos_cursor[cursorIndex][1]));
		}
		else if(playerNum==3){
			*x = pgm_read_byte(&(p3_pos_cursor[cursorIndex][0]));
			*y = pgm_read_byte(&(p3_pos_cursor[cursorIndex][1]));
		}
		else if(playerNum==4){
			*x = pgm_read_byte(&(p4_pos_cursor[cursorIndex][0]));
			*y = pgm_read_byte(&(p4_pos_cursor[cursorIndex][1]));
		}

		// _emu_whisper(0,*x<<3);
		// _emu_whisper(1,*y<<3);
		// WaitVsync(75);
	}
	//
	void nextPlayerNumber(u8 playerNum, enum NPN_nextPlayerNumber options){
		// This function will set the next value for game.activePlayer.

		// Allows you to directly specify the next player (ignoring order.)
		if(playerNum!=255 && options==NPN_SET){
			game.activePlayers[game.activePlayer-1]==playerNum-1;
			return;
		}

		u8 activePlayer = game.activePlayer;

		// Who is the next player?
		while(1){
			if(options==NPN_NEXT){
				if     (game.direction==FORWARD) {
					// Moving forward... in bounds? Increment.
					if( activePlayer+1 !=5 ){ activePlayer++; }

					// Reset to player 1.
					else                      { activePlayer=1; }
				}
				else if(game.direction==BACKWARD){
					// Moving backward... in bounds? Decrement.
					if( activePlayer-1!=0){ activePlayer--; }

					// Reset to player 4.
					else                      { activePlayer=4; }
				}

				// End the loop if the next player is active.

				if( game.activePlayers[activePlayer-1] == 1 ){
					// Do we SET the NEXT value?
					if     (options==NPN_NEXT){
						game.activePlayer=activePlayer;
						return game.activePlayer ;
					}
					// Do we only GET the next value?
					else if(options==NPN_GET){
						return activePlayer ;
					}
				}

				// Otherwise, loop again.
			}
		}

	}
	//
	void askUserToSetNewColor(){
		// Ask the user to chose the new color.
		// Use the message area rows and the draw/discard rows.
		// The erased rows will be drawn after the user makes their selection.

		const u8 replacementTile = pgm_read_byte(&(wood_color2 [2]));
		const u8 yellow_value    = pgm_read_byte(&(yellow_color[2]));
		const u8 blue_value      = pgm_read_byte(&(blue_color  [2]));
		const u8 red_value       = pgm_read_byte(&(red_color   [2]));
		const u8 green_value     = pgm_read_byte(&(green_color [2]));
		const u8 black_value     = pgm_read_byte(&(blackTile   [2]));

		// Cursor timer and current frame.
		u8 * cursorTimer = &vsynccounter8b_gen1;
		*cursorTimer=0;
		u8 currentCursor = 0;
		u8 cursor_x=pgm_read_byte(&(wild_pos_cursor[0][0]));;  // Can change.
		u8 cursor_y=pgm_read_byte(&(wild_pos_cursor[0][1]));; // Stays the same.
		// const char *cursor1map=cursor_f1_u;
		// const char *cursor2map=cursor_f2_u;
		const char *cursor1map=cursor_f1_u;
		const char *cursor2map=cursor_f2_u;
		u8 cursorIndex=0;
		MapSprite2( 0, cursor1map, SPRITE_BANK0 );
		MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);
		SetSpriteVisibility(false);

		u8 numRowsToDraw=8;
		const u8 rowWidth=14;

		u8 startx = 7;
		u8 starty = 9;
		u8 x      = startx;
		u8 y      = starty;
		u16 * srcVram_ptr ;
		u16 * dstVram_ptr ;

		// // Remove the discard card.
		// removeCard_lg();
		// Draw the top and the bottom row first.
		Fill(startx,starty  ,rowWidth,1, black_value);
		Fill(startx,starty+1,rowWidth,1, black_value);
		N782_print(startx+1, starty  , S_CHOOSECOLOR, 0);
		N782_print(startx+3, starty+1, S_PRESSA     , 0);

		// Move down to the next row.
		y++;

		// Open the window.
		for(u8 i=0; i<numRowsToDraw; i+=1){
			//
			WaitVsync(3);

			// Set pointers for memmove.
			srcVram_ptr= &(vram[((y)   * VRAM_TILES_H)+x]);
			dstVram_ptr= &(vram[((y+1) * VRAM_TILES_H)+x]);

			// Move the last row down 1.
			memmove( dstVram_ptr, srcVram_ptr, rowWidth );

			// Replace the old row with black.
			Fill(startx,y,rowWidth,1, black_value);

			// Set the left tile.
			// SetTile(startx, y, blue_value);

			// Set the right tile.
			// SetTile(startx+rowWidth-1, y, green_value);

			// Increment y.
			y++;
		}

		// Draw the color options.
		Fill(startx+2-1   ,starty+4, 3, 2, yellow_value);
		Fill(startx+5-1   ,starty+4, 3, 2, blue_value  );
		Fill(startx+8-1   ,starty+4, 3, 2, red_value   );
		Fill(startx+11-1  ,starty+4, 3, 2, green_value );

		SetSpriteVisibility(true);

		// Loop until a choice has been made.
		while(1){
			getInputs();
			// Move cursor left or right. Perform bounds check.
			if( (game.btnHeld1 & BTN_LEFT) ||  (game.btnHeld1 & BTN_RIGHT) ) {
				playSFX(SELECTCURSOR1);

				// Set new cursor index.
				if( (game.btnHeld1 & BTN_LEFT) ){
					cursorIndex=cursorIndex !=0 ? cursorIndex-1 : cursorIndex;
				}
				// Set new cursor index.
				if( (game.btnHeld1 & BTN_RIGHT) ){
					cursorIndex=cursorIndex !=3 ? cursorIndex+1 : cursorIndex;
				}
				// Set new cursor x and y.
				cursor_x = pgm_read_byte(&(wild_pos_cursor[cursorIndex][0]));
				cursor_y = pgm_read_byte(&(wild_pos_cursor[cursorIndex][1]));
				// Move the cursor sprite to the new location.
				MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);


				// Wait for the user to release all buttons.
				getInputs();
				while(game.btnHeld1){ getInputs(); }
			}

			// Did the user press A?
			if( (game.btnHeld1 & BTN_A) ){
				// Set the new discard color as selected by the user.
				switch(cursorIndex){
					case CARD_YELLOW: { cards[game.lastCardPlayed].color=CARD_YELLOW; break; }
					case CARD_BLUE  : { cards[game.lastCardPlayed].color=CARD_BLUE;   break; }
					case CARD_RED   : { cards[game.lastCardPlayed].color=CARD_RED;    break; }
					case CARD_GREEN : { cards[game.lastCardPlayed].color=CARD_GREEN;  break; }
					default : { break; }
				};

				// Hide the sprite.
				MapSprite2( 0, cursor1map, SPRITE_OFF );
				// MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);
				// MoveSprite(0, OFF_SCREEN, OFF_SCREEN, 1, 1);
				WaitVsync(1);
				SetSpriteVisibility(false);

				// Black out the title bars.
				Fill(startx,starty ,rowWidth,1, black_value);
				Fill(startx,y      ,rowWidth,1, black_value);

				// Close the window.
				for(u8 i=0; i<numRowsToDraw+2; i+=1){
					//
					WaitVsync(2);

					// Set pointers for memmove.
					srcVram_ptr= &vram[(y)  * VRAM_TILES_H]+x;
					dstVram_ptr= &vram[(y-1)  * VRAM_TILES_H]+x;

					// Move the last row down 1.
					memmove( dstVram_ptr, srcVram_ptr, rowWidth );

					// Replace the old row with the replacementTile.
					Fill(startx,y,rowWidth,1, replacementTile);

					// Decrement y.
					y--;
				}

				// Remove the last line of the window.
				Fill(startx,y,rowWidth,1, replacementTile);

				SetSpriteVisibility(true);

				// End the loop.
				break;
			}

			// Change the cursor frame?
			if(*cursorTimer > 15 ){
				// Change to which map?
				if(currentCursor==0){ MapSprite2( 0, cursor1map, SPRITE_BANK0 ); }
				else                { MapSprite2( 0, cursor2map, SPRITE_BANK0 ); }

				// Redraw the sprite.
				MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);

				// Flip the cursor frame.
				currentCursor = !currentCursor;

				// Reset the counter.
				*cursorTimer=0;
			}

			// Keep in sync.
			WaitVsync(1);
		}

	}
	//
	u8 pause(){
		// Ask the user to chose the new color.
		// Use the message area rows and the draw/discard rows.
		// The erased rows will be drawn after the user makes their selection.

		const u8 replacementTile = pgm_read_byte(&(wood_color2 [2]));
		const u8 yellow_value    = pgm_read_byte(&(yellow_color[2]));
		const u8 blue_value      = pgm_read_byte(&(blue_color  [2]));
		const u8 red_value       = pgm_read_byte(&(red_color   [2]));
		const u8 green_value     = pgm_read_byte(&(green_color [2]));
		const u8 black_value     = 0x01;

		// Cursor timer and current frame.
		u8 * cursorTimer = &vsynccounter8b_gen1;
		*cursorTimer=0;
		u8 currentCursor = 0;
		const char *cursor1map=cursor_f1_r;
		const char *cursor2map=cursor_f2_r;
		u8 cursorIndex=3;
		u8 cursor_x=pgm_read_byte(&(pause_pos_cursor[cursorIndex][0]));;  // Can change.
		u8 cursor_y=pgm_read_byte(&(pause_pos_cursor[cursorIndex][1]));; // Stays the same.
		MapSprite2( 0, cursor1map, SPRITE_BANK0 );
		MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);
		SetSpriteVisibility(false);

		u8 numRowsToDraw=10;
		const u8 rowWidth=14;

		u8 startx = 7;
		u8 starty = 8;
		u8 x      = startx;
		u8 y      = starty;
		u16 * srcVram_ptr ;
		u16 * dstVram_ptr ;

		// // Remove the discard card.
		removeCard_lg();

		// Draw the top and the bottom row first.
		Fill(startx,starty  ,rowWidth,1, black_value);
		Fill(startx,starty+1,rowWidth,1, black_value);
		N782_print(startx+2, starty  , S_PAUSEMENU, 0);
		N782_print(startx+0, starty+1, S_ASET_BCANCEL     , 0);

		// Move down to the next row.
		y++;

		// Open the window.
		for(u8 i=0; i<numRowsToDraw; i+=1){
			//
			WaitVsync(3);

			// Set pointers for memmove.
			srcVram_ptr= &(vram[((y)   * VRAM_TILES_H)+x]);
			dstVram_ptr= &(vram[((y+1) * VRAM_TILES_H)+x]);

			// Move the last row down 1.
			memmove( dstVram_ptr, srcVram_ptr, rowWidth );

			// Replace the old row with black.
			Fill(startx,y,rowWidth,1, black_value);

			// Increment y.
			y++;
		}

		// Draw the options.
		N782_print(pgm_read_byte(&(pause_pos_cursor[3][0]))+1,pgm_read_byte(&(pause_pos_cursor[0][1])),PSTR("RESET ROUND"), 0);
		N782_print(pgm_read_byte(&(pause_pos_cursor[3][0]))+1,pgm_read_byte(&(pause_pos_cursor[1][1])),PSTR("EXIT GAME"), 0);
		N782_print(pgm_read_byte(&(pause_pos_cursor[3][0]))+1,pgm_read_byte(&(pause_pos_cursor[2][1])),PSTR("AUTO PLAY"), 0);
		N782_print(pgm_read_byte(&(pause_pos_cursor[3][0]))+1,pgm_read_byte(&(pause_pos_cursor[3][1])),PSTR("CANCEL"), 0);

		SetSpriteVisibility(true);

		// Loop until a choice has been made.
		while(1){
			getInputs();
			// Move cursor left or right. Perform bounds check.
			if( (game.btnHeld1 & BTN_UP) ||  (game.btnHeld1 & BTN_DOWN) ) {
				playSFX(SELECTCURSOR1);

				// Set new cursor index.
				if( (game.btnHeld1 & BTN_UP) ){
					cursorIndex=cursorIndex !=0 ? cursorIndex-1 : cursorIndex;
				}
				// Set new cursor index.
				if( (game.btnHeld1 & BTN_DOWN) ){
					cursorIndex=cursorIndex !=3 ? cursorIndex+1 : cursorIndex;
				}
				// Set new cursor x and y.
				cursor_x = pgm_read_byte(&(pause_pos_cursor[cursorIndex][0]));
				cursor_y = pgm_read_byte(&(pause_pos_cursor[cursorIndex][1]));
				// Move the cursor sprite to the new location.
				MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);


				// Wait for the user to release all buttons.
				getInputs();
				while(game.btnHeld1){ getInputs(); }
			}

			// Did the user press A or START?
			if( (game.btnHeld1 & BTN_A) || (game.btnHeld1 & BTN_START)){
				//
				// game.gamestate1=GSTATE_PLAYING;
				// game.gamestate2=GSTATE_PLAYING_PLAYING;
				// game.gamestate2=GSTATE_PLAYING_GAMESTART;

				switch(cursorIndex){
					// RESET ROUND.
					case 0 : {
						game.gamestate1=GSTATE_PLAYING;
						game.gamestate2=GSTATE_PLAYING_GAMESTART;
						break;
					}
					// EXIT GAME.
					case 1 : {
						game.gamestate1=GSTATE_TITLE;
						game.gamestate2=GSTATE_TITLE_MAINMENU;
						break;
					}
					// AUTO PLAY.
					case 2 : {
						// game.gamestate1=GSTATE_PLAYING;
						// game.gamestate2=GSTATE_PLAYING_GAMESTART;
						break;
					}
					// CANCEL
					case 3 : {
						// game.gamestate1=GSTATE_PLAYING;
						// game.gamestate2=GSTATE_PLAYING_GAMESTART;
						break;
					}
					default: { break; }
				};

				// Hide the sprite.
				MapSprite2( 0, cursor1map, SPRITE_OFF );
				WaitVsync(1);
				SetSpriteVisibility(false);

				// Black out the title bars.
				Fill(startx,starty ,rowWidth,1, black_value);
				Fill(startx,y      ,rowWidth,1, black_value);

				// Close the window.
				for(u8 i=0; i<numRowsToDraw+2; i+=1){
					//
					WaitVsync(2);

					// Set pointers for memmove.
					srcVram_ptr= &vram[(y)  * VRAM_TILES_H]+x;
					dstVram_ptr= &vram[(y-1)  * VRAM_TILES_H]+x;

					// Move the last row down 1.
					memmove( dstVram_ptr, srcVram_ptr, rowWidth );

					// Replace the old row with the replacementTile.
					Fill(startx,y,rowWidth,1, replacementTile);

					// Decrement y.
					y--;
				}

				// Remove the last line of the window.
				Fill(startx,y,rowWidth,1, replacementTile);

				SetSpriteVisibility(true);

				// End the loop.
				break;
			}

			// Change the cursor frame?
			if(*cursorTimer > 15 ){
				// Change to which map?
				if(currentCursor==0){ MapSprite2( 0, cursor1map, SPRITE_BANK0 ); }
				else                { MapSprite2( 0, cursor2map, SPRITE_BANK0 ); }

				// Redraw the sprite.
				MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);

				// Flip the cursor frame.
				currentCursor = !currentCursor;

				// Reset the counter.
				*cursorTimer=0;
			}

			// Keep in sync.
			WaitVsync(1);
		}

		switch(cursorIndex){
			// RESET ROUND.
			case 0 : { return 1; break; }
			// EXIT GAME.
			case 1 : { return 1; break; }
			// AUTO PLAY.
			case 2 : { return 2; break; }
			// CANCEL
			case 3 : { return 0; break; }
			default: { break; }
		};

	}
	//
	void showPoints(){
		// Show existing points for all players.

		// ClearVram();

		if(game.activePlayers[0]==1){
			N782_print    ( 7+0  , 10+0 , S_PLAYER       , 0 ); // Player 1
			N782_print_u8 ( 7+5  , 10+0 , 1                  ); // Player 1
			N782_print_u16( 7+10 , 10+0 , game.points_p1     ); // Player 1
		}

		if(game.activePlayers[1]==1){
			N782_print    ( 7+0  , 10+3 , S_PLAYER       , 0 ); // Player 2
			N782_print_u8 ( 7+5  , 10+3 , 2                  ); // Player 2
			N782_print_u16( 7+10 , 10+3 , game.points_p2     ); // Player 2
		}

		if(game.activePlayers[2]==1){
			N782_print    ( 7+0  , 10+6 , S_PLAYER       , 0 ); // Player 3
			N782_print_u8 ( 7+5  , 10+6 , 3                  ); // Player 3
			N782_print_u16( 7+10 , 10+6 , game.points_p3     ); // Player 3
		}

		if(game.activePlayers[3]==1){
			N782_print    ( 7+0  , 10+9 , S_PLAYER       , 0 ); // Player 4
			N782_print_u8 ( 7+5  , 10+9 , 4                  ); // Player 4
			N782_print_u16( 7+10 , 10+9 , game.points_p4     ); // Player 4
		}

	}
	//
	void setPoints(u8 winner){
		// Tally up the points.

		// Points are now added. The remaining player's cards are added up and that score is added to the winning player's score.
			// "Number Card"    - Number value of the card is number of points.
			// "Draw Two"       - 20 Points
			// "Reverse"        - 20 Points
			// "Skip"           - 20 Points
			// "Wild"           - 50 Points
			// "Wild Draw Four" - 50 Points
		// If a player reaches 500 points then they win.

	Fill(0, 0 , VRAM_TILES_H, VRAM_TILES_V, 1);

	showPoints();

	// Contains the tilemap (vram tiles) used by the selected card sprite. (3x2 tilemaps.)
	u8 map_2x3 [ 2 + (2*3) ];
	u8 map_3x2 [ 2 + (3*2) ];
	map_2x3[0]=2; map_2x3[1]=3;
	map_3x2[0]=3; map_3x2[1]=2;
	// Pointer to the tilemap arrayy
	u8 * map;
	// Dimensions for the selected card map.
	u8 w;
	u8 h;
	s8 xdir;
	s8 ydir;
	u8 dst_x;
	u8 dst_y;
	u8 counter;
	// u8 rand_y[] = {12,14,16,18,};
	u8 tile_x;
	u8 tile_y;
	u8 sprite_x ;//= tile_x << 3;
	u8 sprite_y ;//= tile_y << 3;
	u8 cardDelay=4;

		// Only the winner of the round will get points.
		u8 location;
		u16 * points_slot;
		switch(winner){
			case 1  : {
				location=CARD_LOCATION_PLAYER1;
				points_slot=&game.points_p1;
				dst_x=19;
				dst_y=10;
				break;
			}
			case 2  : {
				location=CARD_LOCATION_PLAYER2;
				points_slot=&game.points_p2;
				dst_x=19;
				dst_y=14;
				break;
			}
			case 3  : {
				location=CARD_LOCATION_PLAYER3;
				points_slot=&game.points_p3;
				dst_x=19;
				dst_y=16;
				break;
			}
			case 4  : {
				location=CARD_LOCATION_PLAYER4;
				points_slot=&game.points_p4;
				dst_x=19;
				dst_y=19;
				break;
			}
			default : {
				return ;
				break;
			}
		}

		clearAllRamtiles();
		u8 pos=0;
		u8 cardsSent=0;
		for(u8 i=0; i<totalCardsInDeck; i+=1){
			// Only add the points for cards that are NOT in the player's hand, draw, or discard.
			if(
				cards[i].location!=location              &&
				cards[i].location!=CARD_LOCATION_DRAW    &&
				cards[i].location!=CARD_LOCATION_DISCARD
			) {
				if(cardsSent%5==0){ pos=0; }
				cardsSent++;

				// Wilds: 50 points.
				if     (cards[i].color==CARD_BLACK)                             { *points_slot+=50; }

				// Number cards: Face value for points.
				else if(cards[i].value>=CARD_0 && cards[i].value<=CARD_9 )      { *points_slot+=cards[i].value; }

				// Action cards: 20 points.
				else if(cards[i].value>=CARD_DRAW2 && cards[i].value<=CARD_REV ){ *points_slot+=50; }

				// Send the card from one side of the screen to the other.

				// tile_y=rand_y[getRandomNumber(12, 18) | 0x01]; // Force it to be even. ;

				switch(cards[i].location){
					case CARD_LOCATION_PLAYER1  : {
						map = &map_2x3 ;
						w=map_2x3[0];h=map_2x3[1];
						tile_x=pgm_read_byte(&(p1_pos[pos][0]));
						tile_y=pgm_read_byte(&(p1_pos[pos][1]));
						break;
					}
					case CARD_LOCATION_PLAYER2  : {
						map = &map_3x2 ;
						w=map_3x2[0];h=map_3x2[1];
						tile_x=pgm_read_byte(&(p2_pos[pos][0]));
						tile_y=pgm_read_byte(&(p2_pos[pos][1]));
						break;
					}
					case CARD_LOCATION_PLAYER3  : {
						map = &map_2x3 ;
						w=map_2x3[0];h=map_2x3[1];
						tile_x=pgm_read_byte(&(p3_pos[pos][0]));
						tile_y=pgm_read_byte(&(p3_pos[pos][1]));
						break;
					}
					case CARD_LOCATION_PLAYER4  : {
						map = &map_3x2 ;
						w=map_3x2[0];h=map_3x2[1];
						tile_x=pgm_read_byte(&(p4_pos[pos][0]));
						tile_y=pgm_read_byte(&(p4_pos[pos][1]));
						break;
					}
					default : {
						return ;
						break;
					}
				}

				drawCard(
					cards[i].location+1 ,
					// 1 ,
					tile_x         , // pgm_read_byte(&(discard_pos[0])) ,
					tile_y         , // pgm_read_byte(&(discard_pos[1])) ,
					SMALL_CARD     ,
					cards[i].value ,
					cards[i].color
				);

				// Get the tilemap for the new card.
				setVramTilemap( tile_x, tile_y , w, h, map );

				// Hide the vram card.
				Fill(tile_x, tile_y , w, h, 1);

				// Map the card to a spritemap.
				MapSprite2_nra( 0, map, SPRITE_BANK1 );

				sprite_x = tile_x << 3;
				sprite_y = tile_y << 3;
				// dst_x    = dst_x  << 3;
				// dst_y    = dst_y  << 3;

				// Set the xdir for the card movement.
				if     (sprite_x<dst_x<< 3){ xdir =  1; }
				else if(sprite_x>dst_x<< 3){ xdir = -1; }
				else                       { xdir =  0; }

				// Set the ydir for the card movement.
				if     (sprite_y<dst_y<< 3){ ydir =  1; }
				else if(sprite_y>dst_y<< 3){ ydir = -1; }
				else                       { ydir =  0; }

				// Reset the counter. (Is used to speed-limit the card movement.)
				counter=0;

				// Move the card.
				while(1){
					// Determine if an x and/or y movement is required this iteration.
					if(sprite_x != dst_x<< 3){ sprite_x += xdir; }
					if(sprite_y != dst_y<< 3){ sprite_y += ydir; }

					// Redraw the sprite at the new location.
					MoveSprite(0, sprite_x, sprite_y, map[0], map[1]);

					// If both the x and the y values match the discard values then break.
					if( sprite_x==dst_x<< 3 && sprite_y==dst_y<< 3 ) { break; }

					// Do a vsync wait every other time.
					// if(counter & 1) { WaitVsync(1); }
					if(counter % cardDelay == 0 ){ WaitVsync(1); }
					// if(counter & 1 ){ WaitVsync(1); }
					// WaitVsync(1);

					// Increment the counter.
					counter+=1;
				}

				// Hide the card spritemap.
				MapSprite2_nra( 0, map, SPRITE_OFF );

				// Remove the tiles used by the small card (if they were the only instances of those tiles.)
				removeUnusedRamtilesInTilemap(map);

				showPoints();

				pos++;

				// WaitVsync(5);
			}


		}
		clearAllRamtiles();

		// Did the player win the game by getting enough points?
		if(*points_slot>=game.pointsForWin){
			//
		}

	}

	void unoCheck(){
		// Does the player now have UNO?
		if     (countPlayerCards(game.activePlayer)==1){
			// Make the user confirm their UNO within a short window of time.

			// If confirmed... good. End the check.

			// If unconfirmed within the window of time then the player must draw 1 card.
		}
	}

	void winGame(u8 winnerOfTheRound){
		// Win! End round, count points, start new round.
			// WIN ROUND!!!

			game.activePlayer = winnerOfTheRound;

			// Update the displayed player data.
			updatePlayerDisplayedData();

			// Update the displayed color, direction, draw pile, discard pile and last played card on discard pile.
			redrawMainInfoRegion();

			msgHandler(GAMEWIN_PLAYER);
			WaitVsync(200);

			// Clear all reserved ramtile data.
			clearAllReservedRamtiles();

			// Tally up the points and then show the points screen.
			ClearVram();
			setPoints(game.activePlayer);
			showPoints();
			WaitVsync(200);

			// game.gamestate1=GSTATE_PLAYING;
			// game.gamestate2=GSTATE_PLAYING_PLAYING;
			game.gamestate2=GSTATE_PLAYING_GAMESTART;

			// SoftReset();
	}

	// Replacement tile used when hiding a card in the player's hand.
	// const u8 replacementTile = pgm_read_byte(&(wood_color2[2])) ;
	const u8 replacementTile = pgm_read_byte(&(offBlackTile[2])) ;

	// Cursor values.
	u8 * cursorTimer = &vsynccounter8b_gen1;
	*cursorTimer=0;

	u8 * msgTimer = &vsynccounter8b_gen2;
	*msgTimer=0;
	u8 msgDisplayed=0;

	u8 currentCursor = 0;
	u8 *cursor1map; cursor1map=cursor2_f1_d;
	u8 *cursor2map; cursor2map=cursor2_f2_d;
	u8 cursorIndex;
	u8 cursor_x;
	u8 cursor_y;

	// Player hand.
	u8 cardSelected=0;

	// Card sprite.
	// Contains the tilemap (vram tiles) used by the selected card sprite. (2x3 tilemaps.)
	u8 map1_2x3 [ 2 + (2*3) ];

	// Contains the tilemap (vram tiles) used by the selected card sprite. (3x2 tilemaps.)
	u8 map2_3x2 [ 2 + (3*2) ];

	// Pointer to the tilemap arrayy
	u8 * map ;

	// Dimensions for the selected card map.
	u8 w;
	u8 h;

	// Player hand - row position.
	u8 handRow = 0;

	//
	u8 counter;
	u8 tile_x;
	u8 tile_y;

	u8 sprite_x;
	u8 sprite_y;

	s8 xdir;
	s8 ydir;

	// Discard Pile.
	u8 discard_x = pgm_read_byte(&(discard_pos[0])) << 3 ; // sprite values.
	u8 discard_y = pgm_read_byte(&(discard_pos[1])) << 3 ; // sprite values.

	// End of turn flags.
	u8 endOfTurnFlag = 0;
	u8 playerDraws2  = 0;
	u8 playerSkipped = 0;
	u8 playerDraws4  = 0;
	u8 playerReverse = 0;
	u8 initialIteration=1;
	u8 thisPlayer;
	u8 maxHandRows;
	u8 playerCards;
	u8 winnerOfTheRound=255;
	u8 retval;

	void prePlay(){
		// PRETURN SETUP:
		// Clear the endOfTurnFlag.
		// Determine active player.
		// Hide the cursor.
		// Handle if this is the initial play.
		// Handle flags set by previous play or initial discard.
		// Reset the flags.
		// Redraw player data (card count, uno.)
		// Redraw play color, direction, draw and discard piles.
		// Should the turn end early (due to previously set flags?)

		// TURN SETUP:
		// Determine cursor coords, colors for the active player.
		// Display the cursor.
		// Reset handRow to 0.
		// Flip up the active player's cards.

		// ACTIVE TURN:
		// Get gamepad input.
		// NO CARD SELECTED?
			// Directional pad buttons will change the cursor and the displayed "row" of cards.

			// "A" will select a card.
				// Make sure card is valid, is wild, color match, or value match.
					// Set the info message flag if the user tries to select an invalid card.
				// Hide the cursor.
				// Copy vram for the selected card into ram map.
				// Erase vram card.
				// Use ram map to create a sprite out of the original vram tiles.
				// Move the card (smoothly) in the correct direction 32 pixels (4 tiles.)
				// Clear any info messages.
				// Set the cardSelected flag.
				// Ask the user to confirm the play or to cancel the selection.
					// END OF BRANCH

			// "B" will pass the player's turn.
				// Clear any info messages.
				// Ask the user to confirm the pass or to cancel the pass.
					// Read gamepad.
						// Pressed "A":
							// Clear any info messages.
							// Deal a card to the player.
							// Set the end of turn flag.

						// Pressed "B":
							// Clear any info messages.
					// If the end of turn flag is set then break the while loop for this play.
				// If button was pressed wait until it has been released before continuing.

		// CARD IS SELECTED?
			// Pressed "A":
				// Hide the cursor sprite.
				// Determine which directions (for x and y) that the card will need to move in order to get to the discard pile.
				// Clear any info messages.
				// Move the card smoothly from its position toward the discard pile.
				// Hide the card sprite.
				// Remove the ramtiles associated with the card sprite (if applicable.)
				// Clear the cardSelected flag.
				// Update the game.lastCardPlayed with the selected card.
				// Set the location of the selected card to CARD_LOCATION_DISCARD.
				// Remove the large discard in vram and ramtiles (if applicable.)
				// Set flags for the next play if the card was an action or wild card.
					// If the card was a wild card then immediately ask the player to choose the new play color.
				// Redraw player data (card count, uno.)
				// Redraw play color, direction, draw and discard piles.
				// Set the endOfTurnFlag.
				// Break the while loop for this play.

			// Pressed "B":
				// Clear any info messages.
				// Smoothly move the card sprite back to its original position.
				// Hide the card spritemap.
				// Use the card sprite map to draw the card back into vram.
				// Clear the cardSelected flag.
				// Unhide the cursor.

		// Cursor map update.
			// If the timer is high enough and a card is not selected then update the cursor map.
			// Map and move the cursor sprite.
			// Toggle the currentCursor.
			// Reset the cursorTimer.

		// Info message timer update (possible erase.)
			// If the timer is high enough and msgDisplayed is set.
			// Clear any info messages.
			// Clear the msgDisplayed flag.
			// Reset the msgTimer.

		// POST ACTIVE TURN:
			// Check if the current player has "UNO."
				//
			// Check if the current player now has 0 cards.
				// Yes? Win!
					// Show the win message.
					// Count points to assign to the winning player.
					// Show the points of all players.
					// END OF ROUND.
			// Flip the player's cards face down (clear ramtile usages where applicable.)

			// Determine and set the next active player.

	}

	while(game.gamestate1==GSTATE_PLAYING){

		// Game round initialization.
		// Resets the game for a new round. (Does NOT impact scores.)
		if     (game.gamestate2==GSTATE_PLAYING_GAMESTART){
			newGameSetup();
			redrawMainInfoRegion();

			initialIteration = 1;
			endOfTurnFlag    = 0;
			playerDraws2     = 0;
			playerSkipped    = 0;
			playerDraws4     = 0;
			playerReverse    = 0;
			winnerOfTheRound = 255;

			game.gamestate2=GSTATE_PLAYING_PLAYING;
			// game.gamestate2=GSTATE_PLAYING_GAMESTART;
			continue;
		}

		// Game in progress.
		else if(game.gamestate2==GSTATE_PLAYING_PLAYING          ){
			// Update the displayed color, direction, draw pile, discard pile and last played card on discard pile.
			redrawMainInfoRegion();

			// Display all cards face-down.
			if(game.activePlayers[0]==1){ displayCards_byPlayer(1, CARDS_FACEDOWN); }
			if(game.activePlayers[1]==1){ displayCards_byPlayer(2, CARDS_FACEDOWN); }
			if(game.activePlayers[2]==1){ displayCards_byPlayer(3, CARDS_FACEDOWN); }
			if(game.activePlayers[3]==1){ displayCards_byPlayer(4, CARDS_FACEDOWN); }

			// Main game loop. The program stays within this loop until the round is over or the gamestate2 changes.
			while(game.gamestate2==GSTATE_PLAYING_PLAYING){
				// Clear the endOfTurnFlag.
				endOfTurnFlag=0;

				//
				thisPlayer=game.activePlayer;

				// Hide the cursor.
				MapSprite2(0, cursor1map, SPRITE_OFF );
				// MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);
				WaitVsync(1);

				// Update the displayed player data.
				updatePlayerDisplayedData();

				// Update the displayed color, direction, draw pile, discard pile and last played card on discard pile.
				redrawMainInfoRegion();

				// Handle initial iteration. (Initial discard.)
				if(initialIteration==1){
					if(cards[game.lastCardPlayed].color==CARD_BLACK     ) { askUserToSetNewColor(); }
					if(cards[game.lastCardPlayed].value==CARD_DRAW2     ) { playerDraws2 =1; }
					if(cards[game.lastCardPlayed].value==CARD_SKIP      ) { playerSkipped=1; }
					if(cards[game.lastCardPlayed].value==CARD_REV       ) { playerReverse=1; }
					if(cards[game.lastCardPlayed].value==CARD_WILD_DRAW4) { playerDraws4 =1; }
				}

#if VRAM_TILES_H == 32
// N782_print_u8(8+0 ,6,playerDraws2);
// N782_print_u8(8+2 ,6,playerSkipped);
// N782_print_u8(8+4 ,6,playerDraws4);
// N782_print_u8(8+6 ,6,playerReverse);

// N782_print_u8(8+0 ,7,game.numPlayers);
// N782_print_u8(8+2 ,7,thisPlayer);
// N782_print_u8(8+4 ,7,initialIteration);
// WaitVsync(50);
#endif
				// Were some flags set at the end of the last turn?
				if(playerDraws2 || playerSkipped || playerDraws4 || playerReverse){
					if     ( playerDraws2  ){
						msgHandler(DRAW2_PLAYER);
						endOfTurnFlag=1;
						// Draw 2
						for(u8 i=0; i<2; i+=1){
							if(game.player_types[thisPlayer-1]!=HUMAN){
								dealSpecifiedCard_anim(game.activePlayer, 255, 2, 3, CARDS_FACEDOWN);
							}
							else{
								dealSpecifiedCard_anim(game.activePlayer, 255, 2, 3, CARDS_FACEUP);
							}
							playSFX(DRAWCARD);
						}
					}
					else if( playerSkipped ){
						msgHandler(SKIP_PLAYER);
						endOfTurnFlag=1;
					}
					else if( playerDraws4  ){
						msgHandler(DRAW4_PLAYER);
						endOfTurnFlag=1;
						// Draw 4
						for(u8 i=0; i<4; i+=1){
							if(game.player_types[thisPlayer-1]!=HUMAN){
								dealSpecifiedCard_anim(game.activePlayer, 255, 2, 3, CARDS_FACEDOWN);
							}
							else{
								dealSpecifiedCard_anim(game.activePlayer, 255, 2, 3, CARDS_FACEUP);
							}

							playSFX(DRAWCARD);
						}
					}
					else if( playerReverse  ){
						// Change the direction.
						game.direction = !game.direction;

						// Handle the initial deal differently.
						if( initialIteration==1){
							if(game.numPlayers == 2){
								endOfTurnFlag = 0 ;
								*msgTimer     = 0 ;
								msgDisplayed  = 1 ;
							}
							else{
								// Skip to the next player.
								nextPlayerNumber(255, NPN_NEXT);
								endOfTurnFlag=1;
							}
						}

						// Number of players is 2?
						else if( game.numPlayers == 2 ){
							// End this turn.
							endOfTurnFlag=1;
						}
						// Default actions.
						else{
							// Skip to the next player.
							nextPlayerNumber(255, NPN_NEXT);
							endOfTurnFlag=1;
						}

						msgHandler(REVERSE_PLAYER);
					}
				}

				// Reset those flags.
				initialIteration = 0;
				playerDraws2     = 0;
				playerSkipped    = 0;
				playerDraws4     = 0;
				playerReverse    = 0;

				// Update the displayed player data.
				updatePlayerDisplayedData();

				// Update the displayed color, direction, draw pile, discard pile and last played card on discard pile.
				redrawMainInfoRegion();

				// winnerOfTheRound=1;

				// Do we have a winner??
				if(winnerOfTheRound!=255){
					winGame(winnerOfTheRound);
					continue;
				}

				// If the end of turn flag is set then end this iteration. (Also, set the value for next player.)
				if( endOfTurnFlag==1 ) {
					WaitVsync(100);

					// Flip down this player's cards. (Clear ramtile usages.)
					displayCards_byPlayer(thisPlayer, CARDS_FACEDOWN);

					clearbgmessage();
					nextPlayerNumber(255, NPN_NEXT);
					clearbgmessage();
					continue;
				}

				// Setup the cursor for this player.
				SetSpriteVisibility(false);
				cursorIndex=0;
				currentCursor=0;
				*cursorTimer=0;
				getCursorPos(game.activePlayer, cursorIndex, &cursor_x, &cursor_y);
				if     (game.activePlayer==1){
					cursor1map=cursor2_f1_d; // Cursor tilemap used.
					cursor2map=cursor2_f2_d; // Cursor tilemap used.
					map = &map1_2x3;         // vram tilemap for the card sprites.
					w=2; h=3;                // Dimensions for the cards sprites.
				}
				else if(game.activePlayer==2){
					cursor1map=cursor2_f1_l;
					cursor2map=cursor2_f2_l;
					map = &map2_3x2;
					w=3; h=2;
				}
				else if(game.activePlayer==3){
					cursor1map=cursor2_f1_u;
					cursor2map=cursor2_f2_u;
					map = &map1_2x3;
					w=2; h=3;
				}
				else if(game.activePlayer==4){
					cursor1map=cursor2_f1_r;
					cursor2map=cursor2_f2_r;
					map = &map2_3x2;
					w=3; h=2;
				}

				// Display the cursor.
				MapSprite2(0, cursor1map, SPRITE_BANK0 );
				MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);
				SetSpriteVisibility(true);

				game.handRow=0;

				// Flip up this player's cards. (Uses ramtiles.)
				displayCards_byPlayer(thisPlayer, CARDS_FACEUP);

				// Loop until the player's turn ends.
				while(endOfTurnFlag==0){
					// Get gamepad inputs. (All human players can be controlled via either gamepad.)
					getInputs();

					// Pause?
					if( (game.btnHeld1 & BTN_START)) {
						retval=pause();
						redrawMainInfoRegion();
						// Cancel.
						if     ( retval == 0 ){
						}
						// Restart round or exit game.
						else if( retval == 1 ){ break; }
						// Auto play.
						else if( retval == 2 ){
							// break;
						}
					}

					// Do a soft-reset if both L and R shoulder buttons are pressed.
					if( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }

					// CARD NOT YET SELECTED: Allow cursor movement and for the user to choose a card or pass their turn.
					if(!cardSelected){
						// Move cursor left or right. Perform bounds check.
						if     (game.activePlayer==1 || game.activePlayer==3){
							// Adjust the cursor position over card.
							if( (game.btnHeld1 & BTN_LEFT) ||  (game.btnHeld1 & BTN_RIGHT) ) {
								playSFX(SELECTCURSOR1);

								if( game.btnHeld1 & (BTN_LEFT ) ){
									cursorIndex=cursorIndex !=0 ? cursorIndex-1 : cursorIndex;
								}
								if( game.btnHeld1 & (BTN_RIGHT) ){
									cursorIndex=cursorIndex !=4 ? cursorIndex+1 : cursorIndex;
								}
								// Redraw the cursor if moved.
								getCursorPos(game.activePlayer, cursorIndex, &cursor_x, &cursor_y);
								// Redraw the sprite.
								MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);
							}

							// Adjust the handRow value and then redisplay cards.
							if( (game.btnHeld1 & BTN_UP) ||  (game.btnHeld1 & BTN_DOWN) ) {
								playSFX(SELECTCURSOR1);

								if( game.btnHeld1 & (BTN_DOWN )  ) {
									game.handRow = game.handRow <= (countPlayerCards(game.activePlayer)/5)-1 ? game.handRow+1 : game.handRow;

								}
								if( game.btnHeld1 & (BTN_UP) ) {
									game.handRow = game.handRow == 0 ? game.handRow : game.handRow-1;
								}

								// Flip up this player's cards. (Uses ramtiles.)
								displayCards_byPlayer(thisPlayer, CARDS_FACEUP);
							}
						}
						else if(game.activePlayer==2 || game.activePlayer==4){
							// Adjust the cursor position over card.
							if( (game.btnHeld1 & BTN_UP) ||  (game.btnHeld1 & BTN_DOWN) ) {
								playSFX(SELECTCURSOR1);

								if( game.btnHeld1 & (BTN_UP )  ){
									cursorIndex=cursorIndex !=0 ? cursorIndex-1 : cursorIndex;
								}
								if( game.btnHeld1 & (BTN_DOWN) ){
									cursorIndex=cursorIndex !=4 ? cursorIndex+1 : cursorIndex;
								}
								// Redraw the cursor if moved.
								getCursorPos(game.activePlayer, cursorIndex, &cursor_x, &cursor_y);
								// Redraw the sprite.
								MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);
							}

							// Adjust the handRow value and then redisplay cards.
							if( (game.btnHeld1 & BTN_LEFT) ||  (game.btnHeld1 & BTN_RIGHT) ) {
								playSFX(SELECTCURSOR1);

								if( game.btnHeld1 & (BTN_RIGHT )  ) {
									game.handRow = game.handRow <= (countPlayerCards(game.activePlayer)/5)-1 ? game.handRow+1 : game.handRow;

								}
								if( game.btnHeld1 & (BTN_LEFT) ) {
									game.handRow = game.handRow == 0 ? game.handRow : game.handRow-1;
								}

								// Flip up this player's cards. (Uses ramtiles.)
								displayCards_byPlayer(thisPlayer, CARDS_FACEUP);
							}
						}

						// Select the card.
						if     ( game.btnHeld1 & (BTN_A   )  ){
							// Do not allow the card to be selected if it is invalid.
							if(game.playerVisibleHand[cursorIndex]==255) { continue; }

							// Do not allow the card to be selected if it is invalid.
							if(
								!(
									// Not a wild?
									   cards[ game.playerVisibleHand[cursorIndex] ].color == CARD_BLACK

									// Not a matching color to the last played card?
									|| cards[ game.playerVisibleHand[cursorIndex] ].color == cards[game.lastCardPlayed].color

									// Not a matching value to the last played card?
									|| cards[ game.playerVisibleHand[cursorIndex] ].value == cards[game.lastCardPlayed].value
								)
							){
								// Change the color if the card was a different color that the discard but had a matching value.
								msgHandler(INCORRECTCARD);
								msgDisplayed=1;
								*msgTimer=0;
								continue;
							}
							else { msgDisplayed=0; }
							/*
							msgDisplayed=0;
							*/

							// Set the cardSelected flag. (Disallows further cursor movements.)
							playSFX(SELECTCARD);
							cardSelected=true;

							// Get new x and y.
							get_cardDims_byPlayerAndCardPosition(game.activePlayer, cursorIndex, &tile_x, &tile_y);
							sprite_x=tile_x << 3 ;
							sprite_y=tile_y << 3 ;

							// Hide the cursor.
							MapSprite2( 0, cursor1map, SPRITE_OFF);
							WaitVsync(1);

							// Get the tilemap for the selected card.
							// map[0], map[1]
							// setVramTilemap( tile_x, tile_y , pgm_read_byte(&(map)[1]), pgm_read_byte(&(map)[1]), map );
							setVramTilemap( tile_x, tile_y , w, h, map );

							// Clear the card in vram.
							Fill(tile_x,tile_y, w, h, replacementTile);

							// Map the card to a spritemap.
							MapSprite2_nra( 1, map, SPRITE_BANK1 );

							// (smoothly) Move the card into the selected position.
							for(u8 i=0; i<8*4; i+=1){
								// Adjust x or y depending on the active player.
								if     (game.activePlayer==1){ sprite_y-=1; }
								else if(game.activePlayer==2){ sprite_x+=1; }
								else if(game.activePlayer==3){ sprite_y+=1; }
								else if(game.activePlayer==4){ sprite_x-=1; }

								MoveSprite(1, sprite_x, sprite_y, map[0], map[1]);

								if(i & 2) { WaitVsync(1); }
							}

							//
							clearbgmessage();

							msgHandler(PLAYORCANCEL);
						}

						// Give up turn.
						else if( game.btnHeld1 & (BTN_B   )  ){
							clearbgmessage();

							// Wait for the user to release the button.
							while ( game.btnHeld1 ){
								getInputs();
								WaitVsync(1);
							}

							msgHandler(PASSORCANCEL);

							// Wait for the user to make a choice.
							while(1){
								getInputs();

								// Confirm - pass.
								if( game.btnHeld1 & (BTN_A ) ){
									// Clear the message.
									clearbgmessage();

									// Player draws 1 card.
									// getCardFromDrawPile(game.activePlayer, 1);

									// If this is a CPU player or UZENET player then don't show the new card.
									// Show the card for HUMAN players only.

									if(game.player_types[thisPlayer-1]!=HUMAN){
										dealSpecifiedCard_anim(game.activePlayer, 255, 2, 3, CARDS_FACEDOWN);
									}
									else{
										dealSpecifiedCard_anim(game.activePlayer, 255, 2, 3, CARDS_FACEUP);
									}

									playSFX(DRAWCARD);

									// Set the endOfTurnFlag.
									endOfTurnFlag=1;

									break;
								}

								// Cancel - pass.
								if( game.btnHeld1 & (BTN_B ) ){
									// Clear the message.
									clearbgmessage();

									// Just break out of this while. Nothing else special to do.
									break;
								}
							}

							// End of turn?
							if(endOfTurnFlag){ break; }
						}

						// If a button was pressed then wait for the button to be released.
						else if( game.btnHeld1 ){
							while ( game.btnHeld1 ){
								getInputs();
								// Do a soft-reset if both L and R shoulder buttons are pressed.
								if( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }
								WaitVsync(1);
							}
						}
					}

					// CARD SELECTED: Allow user to play the card or cancel the request.
					else{
						// PLAY CARD: Confirm the selection and play the card.
						if( game.btnHeld1 & (BTN_A   )  ){
							// playSFX(CARDPLAYED);

							// Play the card.

							// Hide the cursor sprite.
							MapSprite2( 0, cursor1map, SPRITE_OFF );
							WaitVsync(1);

							// Set the xdir for the card movement.
							if     (sprite_x<discard_x){ xdir =  1; }
							else if(sprite_x>discard_x){ xdir = -1; }
							else                       { xdir =  0; }

							// Set the ydir for the card movement.
							if     (sprite_y<discard_y){ ydir =  1; }
							else if(sprite_y>discard_y){ ydir = -1; }
							else                       { ydir =  0; }

							// Reset the counter. (Is used to speed-limit the card movement.)
							counter=0;

							// Clear the confirmation message from the lower board region.
							clearbgmessage();

							// Move the card.
							while(1){
								// Determine if an x and/or y movement is required this iteration.
								if(sprite_x != discard_x){ sprite_x += xdir; }
								if(sprite_y != discard_y){ sprite_y += ydir; }

								// Redraw the sprite at the new location.
								MoveSprite(1, sprite_x, sprite_y, map[0], map[1]);

								// If both the x and the y values match the discard values then break.
								if( sprite_x==discard_x && sprite_y==discard_y ) { break; }

								// Do a vsync wait every other time.
								if(counter & 1) { WaitVsync(1); }

								// Increment the counter.
								counter+=1;
							}

							// Hide the small card spritemap.
							MapSprite2_nra( 1, map, SPRITE_OFF );
							WaitVsync(1);

							// Remove the tiles used by the small card (if they were the only instances of those tiles.)
							removeUnusedRamtilesInTilemap(map);

							// Clear the cardSelected flag. (Allows cursor movements.)
							cardSelected=false;

							// Update the played card's position in the cards array.
							game.lastCardPlayed                 = game.playerVisibleHand[cursorIndex];
							cards[game.lastCardPlayed].location = CARD_LOCATION_DISCARD;

							// Remove the discard card.
							removeCard_lg();

							// Set flags that will be used on the next player.

							// Was the last card played Wild Draw 4?
							if(cards[game.lastCardPlayed].value==CARD_WILD_DRAW4){ playerDraws4=1;  }
							// Was the last card played a Draw Two?
							if(cards[game.lastCardPlayed].value==CARD_DRAW2)     { playerDraws2=1;  }
							// Was the last card played a Skip?
							if(cards[game.lastCardPlayed].value==CARD_SKIP)      { playerSkipped=1; }
							// Was the last card played a Reverse?
							if(cards[game.lastCardPlayed].value==CARD_REV)       { playerReverse=1; }

							// Was the played card a Wild or Wild Draw 4 card?
							// If so, then offer ask the player to choose the new color.
							if(cards[game.lastCardPlayed].color==CARD_BLACK)     { askUserToSetNewColor(); }

							// End player's turn. Start next player's turn.
							// Done! Start over at the beginning.

							// Update the displayed player data.
							updatePlayerDisplayedData();

							// Update the displayed color, direction, draw pile, discard pile and last played card on discard pile.
							redrawMainInfoRegion();

							endOfTurnFlag=1;
							break;

							// continue;
						}

						// CANCEL SELECTION: Cancel the selection and return the card to it's previous position.
						if( game.btnHeld1 & (BTN_B   )  ){
							playSFX(CANCELCARD);

							// Clear the confirmation message from the lower board region.
							clearbgmessage();

							// (smoothly) Move the small card back to its prior position.
							for(u8 i=0; i<8*4; i+=1){
								// Adjust x or y depending on the active player.
								if     (game.activePlayer==1){ sprite_y+=1; }
								else if(game.activePlayer==2){ sprite_x-=1; }
								else if(game.activePlayer==3){ sprite_y-=1; }
								else if(game.activePlayer==4){ sprite_x+=1; }

								MoveSprite(1, sprite_x, sprite_y, map[0], map[1]);

								if(i & 2) { WaitVsync(1); }
							}

							// Hide the spritemap for the selected card.
							MapSprite2_nra( 1, map, SPRITE_OFF);
							WaitVsync(1);

							// Use the spritemap to draw the card tiles back into vram.
							DrawMap_vramtiles( tile_x, tile_y, map );

							// Clear the cardSelected flag. (Allows cursor movements.)
							cardSelected=false;

							// Unhide the cursor.
							MapSprite2(0, cursor1map, SPRITE_BANK0 );
							MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);
						}
					}

					// CURSOR UPDATE: Change the cursor map based on a counter. (Only occurs if a card is not yet selected.)
					if(*cursorTimer > 15 && !cardSelected){
						// Change to which map?
						if(currentCursor==0){ MapSprite2( 0, cursor1map, SPRITE_BANK0 ); }
						else                { MapSprite2( 0, cursor2map, SPRITE_BANK0 ); }

						// Redraw the sprite.
						MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);

						// Flip the cursor frame.
						currentCursor = !currentCursor;

						// Reset the counter.
						*cursorTimer=0;
					}

					// MESSAGE TIMER UPDATE: Remove the message after the timer reaches its limit.
					if(*msgTimer > 100 && msgDisplayed){
						// Clear the message area.
						clearbgmessage();

						// Reset the message displayed flag.
						msgDisplayed=0;

						// Reset the counter.
						*msgTimer=0;
					}

					// Keep in sync.
					WaitVsync(1);

				}

				// End of turn.

				// Does the player now have UNO?
				unoCheck();

				// Does the player now have zero cards?
				if(countPlayerCards(game.activePlayer)==0){
					// Set the value for the winner of this round. It will be checked after any action cards have run their course.
					winnerOfTheRound=game.activePlayer;
					// continue;
				}

				// Flip down this player's cards. (Clear ramtile usages.)
				displayCards_byPlayer(thisPlayer, CARDS_FACEDOWN);

				// Find and set the next active player number.
				nextPlayerNumber(255, NPN_NEXT);

				WaitVsync(1);
			}
		}
	}
}

// MAIN
int main(){
	// Initialize the game.
	gameInit();

	// Main loop.
	while(1){
		// Run the correct function based on what the current game state is.
		// Go to that function and remain there until the gamestate1 changes.
		if      (game.gamestate1==GSTATE_TITLE    ) { gstate_title()   ; }
		else if (game.gamestate1==GSTATE_OPTIONS  ) { gstate_options() ; }
		else if (game.gamestate1==GSTATE_RULES    ) { gstate_rules()   ; }
		else if (game.gamestate1==GSTATE_CREDITS  ) { gstate_credits() ; }
		else if (game.gamestate1==GSTATE_PLAYING  ) { gstate_playing() ; }

		// This shouldn't happen. There should always be a matching gamestate.
		else{
			errorHandler(INVALIDGAMESTATE1);
		}

	}

	// __asm__ __volatile__ ("wdr");
	// __asm__ __volatile__ ("wdr");
}
// MAIN

// OLD - UNUSED

// OLD - UNUSED