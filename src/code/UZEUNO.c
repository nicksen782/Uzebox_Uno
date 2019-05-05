#include "UZEUNO.h"

// ===== DEBUG =====
// Send 8-bit value to CUzeBox whisper port.
static void _emu_whisper(int port, u8 val) {
	if(port==0x39 || port == 0){ u8 volatile * const _whisper_pointer1 = (u8 *) 0x39; *_whisper_pointer1 = val; }
	if(port==0x3A || port == 1){ u8 volatile * const _whisper_pointer2 = (u8 *) 0x3A; *_whisper_pointer2 = val; }
}
// Clears the debug data (on the right side.)
static void clear_debug_showDebugData(){
#if DEBUG_SHOWDATA == 1 && VRAM_TILES_H == 32
	// Clear the debug section.

	u8 xx_start=28;
	u8 inrow=4;

	u8 color = pgm_read_byte(&(blackTile [2]));

	Fill(xx_start,0, inrow, VRAM_TILES_V, color);
#endif
}
// Shows debug data (on the right side.)
static void debug_showDebugData(){
#if DEBUG_SHOWDATA == 1 && VRAM_TILES_H == 32
// clearAllUnusedReservedRamtiles();
	u8 xx_start=28;
	u8 inrow=4;

	u8 xx=xx_start;
	u8 yy=0;

	clear_debug_showDebugData();

	// Show ALL ramtiles. Separate the reserved from the non-reserved.
	xx=xx_start;
	SetRamTile( xx,yy++, 0 );
	u8 drawn=0;
	for(u8 i=1; i<RAM_TILES_COUNT; i+=1){
		// Draw the ram tile.
		SetRamTile( xx,yy, i );

		if(i==RESERVEDRAMTILES+0){
			xx=xx_start;
			yy++;
			yy++;
			drawn=0;
		}

		// Go down to the next row?
		else if(drawn==inrow-1){
			xx=xx_start;
			yy++;
			drawn=0;
		}
		else{
			xx++;
			drawn++;
		}

	}
	yy++;

	// Ram tile reservations (reserved ids)
	xx=xx_start;
	for(u8 i=0; i<RESERVEDRAMTILES; i+=1){
		// Go down to the next row?
		if(!(i%inrow) && i!=0) {
			xx=xx_start;
			yy++;
		}

		// Is this ram tile in use in the queue?
		if(game.ramtile_ids_used[i] ==1) { N782_print(xx, yy+0, PSTR("1"), 0); }
		// No?
		else                             { N782_print(xx, yy+0, PSTR("0"), 0); }

		xx++;
	}
	yy++;

	xx_start+=1;
	yy++;

	// Count of handsPlayed.
	N782_print(xx_start-1,yy, PSTR("HAND"), 0);yy++;
	N782_print_u8(xx_start-0,yy, game.handsPlayed       ) ; yy++;

	// Count of Ramtiles.
	// N782_print(xx_start-1,yy, PSTR("RTC"), 0);yy++;
	// N782_print_u8(xx_start-0,yy, RAM_TILES_COUNT       ) ; yy++;

	// Count of RESERVEDRAMTILES.
	// N782_print(xx_start-1,yy, PSTR("RRTC"), 0);yy++;
	// N782_print_u8(xx_start-0,yy, RESERVEDRAMTILES       ) ; yy++;

	// Count of non-reserved ramtiles.
	// N782_print(xx_start-1,yy, PSTR("NRTC"), 0);yy++;
	// N782_print_u8(xx_start-0,yy, RAM_TILES_COUNT-RESERVEDRAMTILES       ) ; yy++;

	// Card counts.
	// Count of the cards in the Draw Pile.
	// N782_print(xx_start-1,yy, PSTR("DRAW"), 0);yy++;
	// N782_print_u8(xx_start-1,yy, countDrawPile()    ) ; yy++;
	// yy++;

	// Count of the cards in the Discard Pile.
	// N782_print(xx_start-1,yy, PSTR("DISC"), 0);yy++;
	// N782_print_u8(xx_start-1,yy, countDiscardPile() ) ; yy++;
	// yy++;

	// Max used ramtiles.
	// u8 temp = countUsedRamtile_ids();
	// if(temp>game.mostUsedRamtiles){ game.mostUsedRamtiles=temp; }
	// N782_print(xx_start-1,yy, PSTR("MXRT"), 0); yy++;
	// N782_print_u8(xx_start+0, yy  , game.mostUsedRamtiles); yy++;
	// yy++;

	// Indicates which players are active.
	// N782_print(xx_start-1,yy, PSTR("ACTV"), 0);yy++;
	// N782_print_u8(xx_start-3, yy  , game.activePlayers[0]);
	// N782_print_u8(xx_start-2, yy  , game.activePlayers[1]);
	// N782_print_u8(xx_start-1, yy  , game.activePlayers[2]);
	// N782_print_u8(xx_start+0, yy  , game.activePlayers[3]);
	// yy++;

	// Number of players.
	// N782_print(xx_start-1,yy, PSTR("NP"), 0);
	// N782_print_u8(xx_start+0, yy  , game.numPlayers); yy++;

	// Show vsync counters:
	// N782_print(xx_start-1,yy, PSTR("V161"), 0); yy++;
	// N782_print_u8(xx_start+0, yy  , vsynccounter16b_1); yy++;
	// yy++;
	// N782_print(xx_start-1,yy, PSTR("VS_1"), 0); yy++;
	// N782_print_u8(xx_start+0, yy  , vsynccounter8b_gen1); yy++;
	// yy++;
	// N782_print(xx_start-1,yy, PSTR("VS_2"), 0); yy++;
	// N782_print_u8(xx_start+0, yy  , vsynccounter8b_gen2); yy++;
	// yy++;
	// N782_print(xx_start-1,yy, PSTR("VS_3"), 0); yy++;
	// N782_print_u8(xx_start+0, yy  , vsynccounter8b_gen3); yy++;
	// yy++;

	// Show player points.
	// N782_print(xx_start-1,yy, PSTR("P1PT"), 0); yy++; N782_print_u8(xx_start+0, yy  , game.points_p1); yy++;
	// N782_print(xx_start-1,yy, PSTR("P2PT"), 0); yy++; N782_print_u8(xx_start+0, yy  , game.points_p2); yy++;
	// N782_print(xx_start-1,yy, PSTR("P3PT"), 0); yy++; N782_print_u8(xx_start+0, yy  , game.points_p3); yy++;
	// N782_print(xx_start-1,yy, PSTR("P4PT"), 0); yy++; N782_print_u8(xx_start+0, yy  , game.points_p4); yy++;

	// // Counts of card colors/values for the active player's hand.
	// countColorsValuesMatches(game.activePlayer);
	// N782_print(xx_start-1,yy, PSTR("WD"), 0); N782_print_u8(xx_start+0,yy,counts.wild            ); yy++;
	// N782_print(xx_start-1,yy, PSTR("W4"), 0); N782_print_u8(xx_start+0,yy,counts.wildd4          ); yy++;
	// N782_print(xx_start-1,yy, PSTR("BK"), 0); N782_print_u8(xx_start+0,yy,counts.black           ); yy++;
	// yy++;
	// N782_print(xx_start-1,yy, PSTR("YE"), 0); N782_print_u8(xx_start+0,yy,counts.yellow          ); yy++;
	// N782_print(xx_start-1,yy, PSTR("BL"), 0); N782_print_u8(xx_start+0,yy,counts.blue            ); yy++;
	// N782_print(xx_start-1,yy, PSTR("RE"), 0); N782_print_u8(xx_start+0,yy,counts.red             ); yy++;
	// N782_print(xx_start-1,yy, PSTR("GR"), 0); N782_print_u8(xx_start+0,yy,counts.green           ); yy++;
	// yy++;
	// N782_print(xx_start-1,yy, PSTR("DC"), 0); N782_print_u8(xx_start+0,yy,counts.sameDiscardColor); yy++;
	// N782_print(xx_start-1,yy, PSTR("DV"), 0); N782_print_u8(xx_start+0,yy,counts.sameDiscardValue); yy++;
	// yy++;
	// N782_print(xx_start-1,yy, PSTR("MC"), 0); N782_print_u8(xx_start+0,yy,counts.hasMostColorOf); yy++;
	// yy++;
	// N782_print(xx_start-1,yy, PSTR("NCRD"), 0); yy++;
	// N782_print_u8(xx_start+0,yy,counts.numCardsInHand); yy++;
	// yy++;

	// Count of button presses.
	// N782_print(xx_start-1,yy, PSTR("BCNT"), 0);yy++;
	// N782_print_u8(xx_start-0,yy, game.buttonPresses       ) ; yy++;

	// Gamestate 1.
	// N782_print(xx_start-1,yy, PSTR("G1"), 0);
	// N782_print_u8(xx_start+0, yy  , game.gamestate1); yy++;

	// Gamestate 2.
	// N782_print(xx_start-1,yy, PSTR("G2"), 0);
	// N782_print_u8(xx_start+0, yy  , game.gamestate2); yy++;

	// Current hand row for the active player.
	// N782_print(xx_start-1,yy, PSTR("HR"), 0);
	// N782_print_u8(xx_start, yy  , game.handRow); yy++;

	// Card indexes for the active player's displayed cards.
	// N782_print(xx_start-1,yy, PSTR("A"), 0); N782_print_u8(xx_start,yy, game.playerVisibleHand[0] ) ; yy++;
	// N782_print(xx_start-1,yy, PSTR("B"), 0); N782_print_u8(xx_start,yy, game.playerVisibleHand[1] ) ; yy++;
	// N782_print(xx_start-1,yy, PSTR("C"), 0); N782_print_u8(xx_start,yy, game.playerVisibleHand[2] ) ; yy++;
	// N782_print(xx_start-1,yy, PSTR("D"), 0); N782_print_u8(xx_start,yy, game.playerVisibleHand[3] ) ; yy++;
	// N782_print(xx_start-1,yy, PSTR("E"), 0); N782_print_u8(xx_start,yy, game.playerVisibleHand[4] ) ; yy++;

	// Card index for last discarded card.
	// N782_print(xx_start-1,yy, PSTR("LAST"), 0);yy++;
	// N782_print_u8(xx_start-0,yy, game.lastCardPlayed       ) ; yy++;

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

	// Error color.
	u8 color = pgm_read_byte(&(wood_color2   [2]));
	// Fill(1,9, 27, 1, color);

	// Display the error text.
	switch(error){
		// If there are no more free reserved ramtiles.
		case RAMTILEUSAGE      : {
			N782_print( 1,9 , S_RAMTILEUSAGE  , 0 ) ;
			break;
		}

		case UNCLEAREDRESERVEDRT      : {
			N782_print( 1,9 , S_UNCLEAREDRESERVEDRT  , 0 ) ;
			break;
		}

		// INVALID GAMESTATE 1.
		case INVALIDGAMESTATE1    : {
			N782_print( 1,9 , S_INVALIDGAMESTATE1, 0 ) ;
			break;
		}

		// INVALID GAMESTATE 2.
		case INVALIDGAMESTATE2    : {
			N782_print( 1,9 , S_INVALIDGAMESTATE2, 0 ) ;
			break;
		}

		default : { break; }
	}

	// Update the debug data.
	debug_showDebugData();

	while(1){
		if(game.btnHeld1==0){ break; }
		getInputs();
	}

	// Blink the error warning.
	while(1){
		for(u8 i=0; i<5; i+=1){
			Fill(9 , 6, 10, 3, color);
			WaitVsync(20);
			DrawMap2(9,6,errorImg);
			WaitVsync(20);

			getInputs();
			if(game.btnHeld1){ SoftReset(); }
		}

	}
}
// ===== DEBUG =====

// -- LOW LEVEL - VSYNC COUNTERS =====
// Code that is run right before vsync.
static void pre_VsyncCallBack(){
}
// Increments timers (run from post vsync.)
static void updateIndividualTimers(){
	vsynccounter16b_1   ++ ;
	vsynccounter8b_gen1 ++ ;
	vsynccounter8b_gen2 ++ ;
	vsynccounter8b_gen3 ++ ;
}
// Code that is run right after vsync.
static void post_VsyncCallBack(){
	updateIndividualTimers();

	void hidePlayerArrows(){
		Fill(22, 24, 1, 3, 0); // 1
		Fill(1 , 22, 3, 1, 0); // 2
		Fill(5 , 1 , 1, 3, 0); // 3
		Fill(24, 5 , 3, 1, 0); // 4
	}

	void showPlayerArrows(u8 playerNum){
		if     (playerNum==1){
			if     (game.playerArrowFrame==0){
				DrawMap2( 22 , 24+0 , arrow_yellow_f1 );
				DrawMap2( 22 , 24+1 , arrow_yellow_f1 );
				DrawMap2( 22 , 24+2 , arrow_yellow_f1 );
			}
			else if(game.playerArrowFrame==1){
				DrawMap2( 22 , 24+0 , arrow_yellow_f2 );
				DrawMap2( 22 , 24+1 , arrow_yellow_f2 );
				DrawMap2( 22 , 24+2 , arrow_yellow_f2 );
			}
		}
		else if(playerNum==2){
			if     (game.playerArrowFrame==0){
				DrawMap2( 1+0  , 22 , arrow_blue_f1   );
				DrawMap2( 1+1  , 22 , arrow_blue_f1   );
				DrawMap2( 1+2  , 22 , arrow_blue_f1   );
			}
			else if(game.playerArrowFrame==1){
				DrawMap2( 1+0  , 22 , arrow_blue_f2   );
				DrawMap2( 1+1  , 22 , arrow_blue_f2   );
				DrawMap2( 1+2  , 22 , arrow_blue_f2   );
			}
		}
		else if(playerNum==3){
			if     (game.playerArrowFrame==0){
				DrawMap2( 5  , 1+0              , arrow_red_f1    );
				DrawMap2( 5  , 1+1              , arrow_red_f1    );
				DrawMap2( 5  , 1+2              , arrow_red_f1    );
			}
			else if(game.playerArrowFrame==1){
				DrawMap2( 5  , 1+0              , arrow_red_f2    );
				DrawMap2( 5  , 1+1              , arrow_red_f2    );
				DrawMap2( 5  , 1+2              , arrow_red_f2    );
			}
		}
		else if(playerNum==4){
			if     (game.playerArrowFrame==0){
				DrawMap2( 24+0 , 5              , arrow_green_f1  );
				DrawMap2( 24+1 , 5              , arrow_green_f1  );
				DrawMap2( 24+2 , 5              , arrow_green_f1  );
			}
			else if(game.playerArrowFrame==1){
				DrawMap2( 24+0 , 5              , arrow_green_f2  );
				DrawMap2( 24+1 , 5              , arrow_green_f2  );
				DrawMap2( 24+2 , 5              , arrow_green_f2  );
			}
		}
	}

	// If gs1 == GS_PLAYING and gs2 == GS_PLAYING_PLAYING.
		// Get Uzenet updates.
		//

		// Does a player have UNO? Alternate the UNO graphic.
		//

		// Alternate the player arrows that point to their hand.
		if( game.gamestate1 == GS_PLAYING && game.gamestate2 == GS_PLAYING_PLAYING	 ){
			if     (vsynccounter8b_gen3<20){ game.playerArrowFrame=0; }
			else if(vsynccounter8b_gen3<40){ game.playerArrowFrame=1; }
			else                           { game.playerArrowFrame=0; vsynccounter8b_gen3=0; }

			hidePlayerArrows();
			showPlayerArrows(game.activePlayer);

			// debug_showDebugData();
		}
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

	// Count new button presses.
	if(game.btnPressed1){ game.buttonPresses++; }
}
// ===== LOW LEVEL - VSYNC COUNTERS =====

// ===== TEXT DISPLAY =====
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
// Displays u16 number display (allows for a limited fontset.)
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
// Clears the displayed message (seen when playing cards.)
static void clearbgmessage(){
	// Fill(8,18, 12,1, pgm_read_byte(&(wood_color1 [2] )));
	// Fill(8,19, 12,1, pgm_read_byte(&(yellow_color[2] )));
	// Fill(8,20, 12,1, pgm_read_byte(&(blue_color  [2] )));

	// const u8 replacementTile = pgm_read_byte(&(wood_color1[2])) ;
	const u8 replacementTile = pgm_read_byte(&(wood_color2[2])) ;
	Fill(7,18, 14,3, replacementTile);
}
// Message handler - Central place for drawing game messages.
static void msgHandler(enum msgTypes msg){
	// EXAMPLE USAGE: msgHandler(GAMESTART_FIRST);
	// Intended for non-fatal errors.

	// Clear the background (with the black tile.)
	Fill(7,18, 14,3, pgm_read_byte(&(blackTile[2]   )));

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
			N782_print   ( 10  , 18, S_PLAYER, 0);
			N782_print_u8( 15 , 18, game.activePlayer );
			N782_print   ( 8  , 20, S_PLAYSFIRST, 0);
			break;
		}
		case DRAW2_PLAYER      : {
			N782_print   ( 10  , 18, S_PLAYER   , 0 ) ;
			N782_print_u8( 15 , 18, game.activePlayer );
			N782_print   ( 9  , 19, S_DRAW2,0);
			N782_print   ( 9  , 20, S_LOSETURN,0);
			break;
		}
		case DRAW4_PLAYER      : {
			N782_print   ( 9  , 18, S_PLAYER   , 0 ) ;
			N782_print_u8( 15 , 18, game.activePlayer );
			N782_print   ( 9  , 19, S_DRAW4,0);
			N782_print   ( 9  , 20, S_LOSETURN,0);
			break;
		}
		case SKIP_PLAYER       : {
			N782_print   ( 10  , 18, S_PLAYER   , 0 ) ;
			N782_print_u8( 15 , 18, game.activePlayer );
			N782_print   ( 11  , 19, S_SKIP,0);
			N782_print   ( 9  , 20, S_LOSETURN,0);
			break;
		}
		case REVERSE_PLAYER    : {
			N782_print   ( 9  , 18, S_REVERSE1,0);
			N782_print   ( 9  , 20, S_REVERSE2,0);
			break;
		}
		case INCORRECTCARD     : {
			N782_print( 9 , 18, S_INCORRECTYCARD  , 0 ) ;
			N782_print( 8 , 20, S_CHOSEANOTHER    , 0 ) ;
			break;
		}
		case PLAYORCANCEL      : {
			N782_print( 9 , 18, S_A_TO_PLAY  , 0 ) ;
			N782_print( 9 , 20, S_B_TO_CANCEL, 0 ) ;
			break;
		}
		case PASSORCANCEL      : {
			N782_print( 9 , 18, S_A_TO_PASS   , 0 ) ;
			N782_print( 9 , 20, S_B_TO_CANCEL , 0 ) ;
			break;
		}
		case ROUNDWIN_PLAYER    : {
			N782_print   ( 10  , 18, S_PLAYER, 0);
			N782_print_u8( 15  , 18, game.activePlayer );
			N782_print   ( 10  , 19, S_WINSROUND1 , 0);
			N782_print   ( 11  , 20, S_WINSROUND2 , 0);
			break;
		}

		// Does not require the same dimensions as the other messages.
		case GAMEWIN_PLAYER    : {
			N782_print   ( 10  , 17-2, S_PLAYER, 0);
			N782_print_u8( 15  , 17-2, game.activePlayer );
			N782_print   ( 7   , 19-2, S_WINSGAME1 , 0);
			for(u8 i=0;i<10;i+=1){
				Fill(6,21-1, 16,1, 0);
				WaitVsync(10);
				N782_print   ( 6   , 21-1, S_CONGRATS  , 0);
				WaitVsync(10);
			}
			break;
		}

		default : { break; }
	}

	// Update the debug data.
	debug_showDebugData();
}
// ===== TEXT DISPLAY =====

// ===== INIT =====
// Displays message and then wait for user input. Uses the button press/release timing as a random seed.
static void getRandomSeedViaUserInput(){
	// Get a random source: Use the time it takes for the user to press and then release the button.
	vsynccounter8b_gen1=0;
	u8 frame=0;
	u8 x=(VRAM_TILES_H/2) - (12/2); // (12 is the length of the string.
	u8 y=(VRAM_TILES_V/2) - 1;
	ClearVram();

	N782_print( x, y, S_GAMEREADY , 0 ) ;

	while(1){
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

			// End the loop.
			break;
		}

		// Keep in sync while waiting for the user to hold a button then release it.
		WaitVsync(1);
	}
}
// Run once at program start. (SETUP)
static void gameInit(){
	// System init.
	SetTileTable( bg_tiles );
	ClearVram();

	// Specify values for the sprite banks.
	SetSpritesTileBank(0 , sprite_tiles   ); // 0 -- SPRITE_BANK0
	SetSpritesTileBank(1 , bg_tiles   );     // 1 -- SPRITE_BANK1

	// Init the music player.
	InitMusicPlayer(patches);
	SetMasterVolume(50);

	SetUserRamTilesCount(RESERVEDRAMTILES+1); // Reserved ramtiles plus the compare.

	// Set the pre/post vsync callbacks.
	SetUserPreVsyncCallback ( &pre_VsyncCallBack  ); //
	SetUserPostVsyncCallback( &post_VsyncCallBack ); //

	// Show the compile date.
	WaitVsync(50);
	N782_print( 0, 0, S_GAMENAME , 0 ) ;
	N782_print( 0, 1, S_MSG1 , 0) ; N782_print( 6, 1, S_MSG2 , 0) ;
	N782_print( 0, 7, S_MSG3 , 0) ;
	WaitVsync(50);
	ClearVram();

	// Init the inital game settings.
	// game.numPlayers     =  2                          ;
	// game.activePlayers[0]  = game.numPlayers > 0 ? 1 : 0 ;
	// game.activePlayers[1]  = game.numPlayers > 1 ? 1 : 0 ;
	// game.activePlayers[2]  = game.numPlayers > 2 ? 1 : 0 ;
	// game.activePlayers[3]  = game.numPlayers > 3 ? 1 : 0 ;
	game.activePlayer   = 255                       ;
	game.pointsForWin   = 500                       ;
	game.points_p1      = 0                         ;
	game.points_p2      = 0                         ;
	game.points_p3      = 0                         ;
	game.points_p4      = 0                         ;
	game.lastCardPlayed = 255                       ;
	game.direction      = FORWARD                   ;
	game.player_types[0]= PLAYER1_DEFAULTTYPE       ; // #define
	game.player_types[1]= PLAYER2_DEFAULTTYPE       ; // #define
	game.player_types[2]= PLAYER3_DEFAULTTYPE       ; // #define
	game.player_types[3]= PLAYER4_DEFAULTTYPE       ; // #define
	game.winStyle       = FIRSTTO500POINTS          ;
	game.drawStyle      = DRAW1                     ;
	game.buttonPresses  = 0;
	game.playerVisibleHand[0]=255;
	game.playerVisibleHand[1]=255;
	game.playerVisibleHand[2]=255;
	game.playerVisibleHand[3]=255;
	game.playerVisibleHand[4]=255;
	game.randomSeed=0;
	game.playerArrowFrame=0;

	// Normal starting gamestates.
	game.gamestate1   = DEFAULTGS1 ; // #define
	game.gamestate2   = DEFAULTGS2 ; // #define

	// SOUND TEST
	// while(true){
	// 	WaitVsync(1); _emu_whisper(0,5 ); WaitVsync(1); for(u8 ii=0; ii<1;ii+=1) { TriggerFx(5 , 255, true); } WaitVsync(100);
	// 	WaitVsync(1); _emu_whisper(0,6 ); WaitVsync(1); for(u8 ii=0; ii<1;ii+=1) { TriggerFx(6 , 255, true); } WaitVsync(100);
	// 	WaitVsync(1); _emu_whisper(0,7 ); WaitVsync(1); for(u8 ii=0; ii<1;ii+=1) { TriggerFx(7 , 255, true); } WaitVsync(100);
	// 	WaitVsync(1); _emu_whisper(0,8 ); WaitVsync(1); for(u8 ii=0; ii<1;ii+=1) { TriggerFx(8 , 255, true); } WaitVsync(100);
	// 	WaitVsync(1); _emu_whisper(0,9 ); WaitVsync(1); for(u8 ii=0; ii<1;ii+=1) { TriggerFx(9 , 255, true); } WaitVsync(100);
	// }

	clearAllReservedRamtiles();
	// clearAllRamtiles();

	ClearVram();

	// Reset counters.
	vsynccounter16b_1   = 0;
	vsynccounter8b_gen1 = 0;
	vsynccounter8b_gen2 = 0;
	vsynccounter8b_gen3 = 0;
}
// ===== INIT =====

// ===== SOUND UTILITIES =====
// Plays the specified sound effect.
static void playSFX(u8 patch){
	u8 volume   = 128  ;
	// bool retrig = true ;
	bool retrig = true ;

	// playSFX (SELECTCURSOR1);
	// playSFX (DRAWCARD);
	// playSFX (SELECTCARD);
	// playSFX (CANCELCARD);
	// playSFX (CARDPLAYED);

	// 5 'tiuck'
	// 6 'thuck'
	// 7 'torltorl'
	// 8 'tink'
	// 9 'tiiiigh' is high pitched, annoying.

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
		case DRAWCARD            : { TriggerFx(7,  volume, retrig); break ; } //
		//
		default                  : { break ; }
	};

	// TriggerFx(unsigned char patch,unsigned char volume, bool retrig);
	// TriggerFx(patch,  volume, retrig);
}
// ===== SOUND UTILITIES =====

// ===== RAMTILE UTILITIES =====
// CHECK: Clear any unused reserved ramtile if it is not in use in vram.
static void clearAllUnusedReservedRamtiles(){
	#if ENDOFPLAY_RESRAMTILECHK==1
	// Should be played at the start of each hand while the player's cards are still face-down.
	u8 tile;
	u8 * vram_row_ptr;

	// Go through vram once. Check each value in vram that is a ram tile.
	// Then use that value as an index into the game.ramtile_ids_used array.
	// Keep track of all tiles that match one of the reserved ram tiles.
	// Just a flag that it was seen.
	// At the end, go through the list and clear any ramtile that was NOT seen.

	// 0 is good meaning unseen. 1 means seen but not reserved.
	u8 ramtileids[ RESERVEDRAMTILES+1 ] = { 0 };
	ramtileids[0]=0;
	// u8 temp;

	__asm__ __volatile__ ("wdr");
	for(u8 y=0; y<VRAM_TILES_V; y+=1){
		// Get vram address to this row.
		vram_row_ptr = &vram[ (y*VRAM_TILES_H)+0 ];
		// Only go up to 28 (since anything beyond that is the debug region.)
		for(u8 x=0; x<28; x+=1){
			// Get the vram tile id.
			tile = (*(vram_row_ptr+x));

			// Is the id at vram the id of one of the reserved ramtiles?
			if(
				(tile !=0 && tile < RESERVEDRAMTILES)
				&& game.ramtile_ids_used[tile-1]==0
			){
				ramtileids[ tile-1 ] = 1;

				// for(u8 i=0; i<10; i+=1){
				// 	temp=tile;
				// 	(*(vram_row_ptr+x))=0;
				// 	WaitVsync(2);
				// 	(*(vram_row_ptr+x))=temp;
				// 	WaitVsync(2);
				// }

				// N782_print(0,0,PSTR("BAD "), 0);
				// WaitVsync(100);
				goto label_breakoutofloop;
			}
			// N782_print(0,0,PSTR("GOOD"), 0);
		}
	}
	__asm__ __volatile__ ("wdr");

	// This label can be called by the previous for loop to break out of the nexted loops.
	label_breakoutofloop:

	// Clear any reserved ramtiles that were NOT seen.
	for(u8 i=1; i<sizeof(ramtileids+1); i+=1){
		if(ramtileids[i-1]==1 && i!=0){

#if ENDOFPLAY_RESRAMTILECHK_ERR==1
			// Error out if a ramtile was found to still be reserved but isn't on screen.
			_emu_whisper(0, i);
			_emu_whisper(1, i-1);
			debug_showDebugData();
			errorHandler(UNCLEAREDRESERVEDRT);
#else
			// Set the ramtile to be available for use.
			game.ramtile_ids_used[ i-1] = 0;

			// Clear the data in that ramtile.
			clearRamtile(i, TILE_RAMTILECLEAR+1);
#endif
		}
	}
	#endif
}
// Clears all reserved ramtiles (indexes and data.)
static void clearAllReservedRamtiles(){
	// Clear the ramtiles.
	clearRamtile(0, TILE_RAMTILECLEAR);
	for(u8 i=0; i<RESERVEDRAMTILES; i+=1){
		// Reset
		clearRamtile(i+1, TILE_RAMTILECLEAR);
		game.ramtile_ids_used[i]=0;
	}
}

// Rotates a ramtile in place negative 90 degrees.
static void rotateRamTile_n90(u8 mat[][8]){
	// Based on: https://www.geeksforgeeks.org/inplace-rotate-square-matrix-by-90-degrees/
	// Rotates anti-clockwise 90 degrees (so, -90 degrees.)
	// Rotates in-place with no additional ramtile needed.
	// EXAMPLE USAGE: rotateRamTile_n90( (u8 (*)[8]) &ram_tiles[0*64] ); // n90 rotate ramtile 0.

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
	// u8 YELLOW_VALUE = pgm_read_byte( &( &bg_tiles[ (u16) ( (TILE_WIDTH*TILE_HEIGHT) * pgm_read_byte(&(yellow_color[2])) ) ])[0] );
	// u8 BLUE_VALUE   = pgm_read_byte( &( &bg_tiles[ (u16) ( (TILE_WIDTH*TILE_HEIGHT) * pgm_read_byte(&(blue_color[2]  )) ) ])[0] );
	// u8 RED_VALUE    = pgm_read_byte( &( &bg_tiles[ (u16) ( (TILE_WIDTH*TILE_HEIGHT) * pgm_read_byte(&(red_color[2]   )) ) ])[0] );
	// u8 GREEN_VALUE  = pgm_read_byte( &( &bg_tiles[ (u16) ( (TILE_WIDTH*TILE_HEIGHT) * pgm_read_byte(&(green_color[2] )) ) ])[0] );
	// u8 ORANGE_VALUE = pgm_read_byte( &( &bg_tiles[ (u16) ( (TILE_WIDTH*TILE_HEIGHT) * pgm_read_byte(&(orange_color[2])) ) ])[0] );
	// u8 BLACK_VALUE  = pgm_read_byte( &( &bg_tiles[ (u16) ( (TILE_WIDTH*TILE_HEIGHT) * pgm_read_byte(&(blackTile[2]   )) ) ])[0] );

	// Adjust the srcColor to the actual value.
	switch(srcColor){
		case CARD_YELLOW : { srcColor = YELLOW_VALUE; break; } // Yellow
		case CARD_BLUE   : { srcColor = BLUE_VALUE;   break; } // Blue
		case CARD_RED    : { srcColor = RED_VALUE;    break; } // Red
		case CARD_GREEN  : { srcColor = GREEN_VALUE;  break; } // Green
		case CARD_ORANGE : { srcColor = ORANGE_VALUE; break; } // Orange
		case CARD_BLACK  : { srcColor = BLACK_VALUE;  break; }
		default  : { return; break; }
	};

	// Adjust the dstColor to the actual value.
	switch(dstColor){
		case CARD_YELLOW : { dstColor = YELLOW_VALUE; break; } // Yellow
		case CARD_BLUE   : { dstColor = BLUE_VALUE  ; break; } // Blue
		case CARD_RED    : { dstColor = RED_VALUE   ; break; } // Red
		case CARD_GREEN  : { dstColor = GREEN_VALUE ; break; } // Green
		case CARD_ORANGE : { dstColor = ORANGE_VALUE; break; } // Orange
		case CARD_BLACK  : { dstColor = BLACK_VALUE ; break; }
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
	// if(id <= RESERVEDRAMTILES ) {
	// if(id < RAM_TILES_COUNT ) {
		if(newColor==TILE_RAMTILECLEAR){
			// Replace the ram tile data with one from flash.
			// CopyFlashTile(TILE_RAMTILECLEAR, id);
			CopyFlashTile(TILE_RAMTILECLEAR, id);
		}
		else{
			// Set each pixel of the ramtile to the new value.
			for(u16 i=0; i<64; i+=1){
				// Set the address of the pointer.
				ramTiles_ptr = &ram_tiles[ ((id*64) +i) ];
				// Set the value for this pixel.
				*ramTiles_ptr = newColor;
			}
		}
	// }
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
		currentRamtileId<(RESERVEDRAMTILES-1); // Do while true.
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
// ===== RAMTILE UTILITIES =====

// ===== CARD UTILITIES =====
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
	// clearAllUnusedReservedRamtiles();
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
	// clearAllUnusedReservedRamtiles();
}
// Removes and clears the ramtiles that are no longer in use by the specified map.
static void removeUnusedRamtilesInTilemap(u8 *map){
	// return  ;
	// This function operates similar to removeCard_sm and removeCard_lg.
	 // Requires a tilemap that uses vram ids (and is stored in RAM, not PROGMEM.)
	// It assumes that that the tiles for the removed card have already been removed from vram.
	// It checks each tile in the tilemap for existance in vram.
	 // If the tile id is < RAM_TILES_COUNT then it is a ramtile and is subject to being cleared if the tile id is used nowhere else in vram.
	 // Flash tiles are ignored.

	// Clear the debug section.
	// clear_debug_showDebugData();

	// Get the dimensions for this map.
	const u8 w=map[0];
	const u8 h=map[1];
	const u8 sizeOfMap = (w*h)+2;
	u16 tile;
	u8 clearThisRamtile=false;

	u8 * vram_row_ptr;

	// For each tile in the array, look through vram.
	for(u16 ii=2; ii<sizeOfMap; ii+=1){
		// Get the tile id.
		tile = map[ii];

		// Must be a ramtile (and one of the ones that can be reserved.)
		if(tile <= RESERVEDRAMTILES ){
			// Set the clear flag.
			clearThisRamtile=true;

			// Go through vram.
			// for(u16 i=0; i<(VRAM_TILES_H*VRAM_TILES_V); i+=1){
			for(u8 y=0; y<VRAM_TILES_V; y+=1){
				// Set the address of the pointer.
				vram_row_ptr = &vram[ (y*VRAM_TILES_H)+0 ];

				for(u8 x=0; x<28; x+=1){
					// If at least one instance of the tile id is found then ignore this tile.
					if( (*(vram_row_ptr+x)) == tile ){
						// Clear the clear flag.
						clearThisRamtile=false;

						// Quit looking in vram for this tile.
						break;
					}
				}
			}

			// Is the clear flag still set? (If 0 instances of the tile id is found then clear it.)
			// NOTE: If a tile is cleared twice then that should not be a problem.
			if(clearThisRamtile){
				// Set the ramtile to be available for use.
				game.ramtile_ids_used[ tile -1] = 0;

				// Clear the data in that ramtile.
				// clearRamtile(tile, BLACK_VALUE);
				clearRamtile(tile, TILE_RAMTILECLEAR);
			}
		}
	}

	debug_showDebugData();

}
// Returns all discarded cards to the draw pile except the last played card.
static void returnDiscardsToDrawPile(){
	// Return all discards to the draw pile.
	for(u8 i=0; i<TOTALCARDSINDECK; i+=1){
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
	for(u8 i=0; i<TOTALCARDSINDECK; i+=1){
		cards[i].location = CARD_LOCATION_DRAW ;
	}
}
// ===== CARD UTILITIES =====

// ===== CARD DRAWING =====
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
					for(u8 i=0; i<RESERVEDRAMTILES; i+=1){
						// Found an index where the value set is 0?
						if(game.ramtile_ids_used[i]==0){
							// The current value of the i iterator will be the index and the next ram tile used.
							// Copy ramtile 0 to the id and then draw it.

							// Rotate the ramtile the number of times specified.
							while(rotations){
								// rotateRamTile_n90( &ram_tiles[(0*64)]); // n90 rotate ramtile 0.
								rotateRamTile_n90( (u8 (*)[8]) &ram_tiles[0*64] ); // n90 rotate ramtile 0.
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
	// clearRamtile(0, BLACK_VALUE);
	clearRamtile(0, TILE_RAMTILECLEAR);

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
// ===== CARD DRAWING =====

// ===== UTILITIES =====
// Returns a random number within the range specified.
u16 getRandomNumber(int min, int max){
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
	for(u8 i=0; i<TOTALCARDSINDECK; i+=1){
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
	for(u8 i=0; i<TOTALCARDSINDECK; i+=1){ if( cards[i].location==CARD_LOCATION_DRAW ) { theCount++;} }
	// Return the count.
	return theCount;
}
// Returns the count of cards held by the Discard Pile.
u8 countDiscardPile(){
	u8 theCount=0;
	// Go through the cards array and count up all cards that match the location.
	for(u8 i=0; i<TOTALCARDSINDECK; i+=1){ if( cards[i].location==CARD_LOCATION_DISCARD ) { theCount++;} }
	// Return the count.
	return theCount;
}
// Returns the count of used ram ids.
u8 countUsedRamtile_ids(){
	u8 theCount=0;
	// Go through the reserved ramtiles array and count the used indexes.
	for(u8 i=0; i<RESERVEDRAMTILES; i+=1){ if( game.ramtile_ids_used[i] == 1 ) { theCount++; } }
	// Return the count.
	return theCount;
}
// Returns the count of unused ram ids.
u8 countAvailableRamtile_ids(){
	u8 theCount=0;
	// Go through the reserved ramtiles array and count the unused indexes.
	for(u8 i=0; i<RESERVEDRAMTILES; i+=1){ if( game.ramtile_ids_used[i] == 0 ) { theCount++; } }
	// Return the count.
	return theCount;
}
// Gets counts for each type of card in a player's hand.
void countColorsValuesMatches(u8 playerNum){
	// Determine card location by player number.
	u8 location=playerNum-1;

	// Reset the struct values.
	counts.wild             = 0 ;
	counts.wildd4           = 0 ;
	counts.black            = 0 ;
	counts.yellow           = 0 ;
	counts.blue             = 0 ;
	counts.red              = 0 ;
	counts.green            = 0 ;
	counts.sameDiscardColor = 0 ;
	counts.sameDiscardValue = 0 ;
	counts.hasMostColorOf   = 0 ;
	counts.numCardsInHand   = 0 ;

	// Go through the cards array and count up all cards that match the player location.
	for(u8 i=0; i<TOTALCARDSINDECK; i+=1){
		if     (cards[i].location==location){
			// Update the card count in the player's hand.
			counts.numCardsInHand++;

			// Count the card colors.
			if(cards[i].color==CARD_BLACK ){ counts.black ++; }
			if(cards[i].color==CARD_YELLOW){ counts.yellow++; }
			if(cards[i].color==CARD_BLUE  ){ counts.blue  ++; }
			if(cards[i].color==CARD_RED   ){ counts.red   ++; }
			if(cards[i].color==CARD_GREEN ){ counts.green ++; }

			// Count the card if Wild.
			if(cards[i].value==CARD_WILD      ){ counts.wild  ++; }
			// Count the card if Wild Draw 4.
			if(cards[i].value==CARD_WILD_DRAW4){ counts.wildd4++; }

			// Count if value is same as discard.
			if(cards[i].color==cards[ game.lastCardPlayed ].color){ counts.sameDiscardColor++; }
			// Count if color is same as discard.
			if(cards[i].value==cards[ game.lastCardPlayed ].value){ counts.sameDiscardValue++; }
		}
	}

	// Determine which card color the player has the most of.
	// The default is yellow and it can be superseded by greater counts.
	counts.hasMostColorOf = CARD_YELLOW;
	if(counts.blue >counts.yellow){ counts.hasMostColorOf = CARD_BLUE;  }
	if(counts.red  >counts.blue  ){ counts.hasMostColorOf = CARD_RED;   }
	if(counts.green>counts.red   ){ counts.hasMostColorOf = CARD_GREEN; }
}
// ===== UTILITIES =====

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
	for(u8 i=0; i<TOTALCARDSINDECK; i+=1){
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

	// for(u8 pos=0; pos<5; pos+=1){
	// 	// Remove the existing card.
	// 	removeCard_sm(playerNum, pos);
	// }

	// 5 card positions.
	for(u8 i=0; i<TOTALCARDSINDECK && (cardsAssigned<5); i+=1){
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

			// Remove the card in this position if one is already there.
			removeCard_sm(playerNum, cardsAssigned);

			// Draw the card.
			drawCard(
				playerNum      ,
				x              ,
				y              ,
				SMALL_CARD     ,
				cards[i].value ,
				cards[i].color
			);

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
	// u8 inDiscardPile = countDiscardPile();

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
		cardIndex2 = getRandomNumber(0, TOTALCARDSINDECK-1); // Normal range.
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

		// Return the value of the last cardIndex2.
		return 255;
	}
	else{
		// Return the value of the last cardIndex2.
		return cardIndex2;
	}

}
// Draw and animate a card from the Draw pile into a player's hand.
static void dealSpecifiedCard_anim(u8 playerNum, u8 cardIndex, u8 cardPos, u8 cardDelay, u8 option){
	// Draw one card from the Draw Pile and assign to the specified player.
	// Get the last card assigned's index into the cards array.
	// NOTE: This function assumes that the cards have been removed first.
	const char * card ;
	// u8 location;

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
			map = map1_2x3;
			dst_x=pgm_read_byte(&(p1_pos[cardPos][0]));
			dst_y=pgm_read_byte(&(p1_pos[cardPos][1]));
			// location=CARD_LOCATION_PLAYER1;
			break;
		}
		case 2 : {
			card=card_back_sm_neg90deg;
			w=3; h=2;
			map = map2_3x2;
			dst_x=pgm_read_byte(&(p2_pos[cardPos][0]));
			dst_y=pgm_read_byte(&(p2_pos[cardPos][1]));
			// location=CARD_LOCATION_PLAYER2;
			break;
		}
		case 3 : {
			card=card_back_sm_360deg;
			w=2; h=3;
			map = map1_2x3;
			dst_x=pgm_read_byte(&(p3_pos[cardPos][0]));
			dst_y=pgm_read_byte(&(p3_pos[cardPos][1]));
			// location=CARD_LOCATION_PLAYER3;
			break;
		}
		case 4 : {
			card=card_back_sm_90deg;
			w=3; h=2;
			map = map2_3x2;
			dst_x=pgm_read_byte(&(p4_pos[cardPos][0]));
			dst_y=pgm_read_byte(&(p4_pos[cardPos][1]));
			// location=CARD_LOCATION_PLAYER4;
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

	// // If either pile has cards... or something...
	// if( countDrawPile() || countDiscardPile() ){
	// }

	// If the cardIndex is 255 (returned from function) then there was not a card available.
	if(cardIndex==255){
		return;
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
	// clearAllUnusedReservedRamtiles();

	// Update the displayed player data.
	updatePlayerDisplayedData();

	// Update the displayed color, direction, draw pile, discard pile and last played card on discard pile.
	redrawMainInfoRegion();

	return;
}
// Get the X and the Y for a player's card and card position.
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
// Redraws center of board, Draw and Discard Piles, play direction, active color.
static void redrawMainInfoRegion(){
	u8 colorTile;

	// Remove the discard card. (Always do this first.)
	removeCard_lg();

	// Redraw the center.
	// Fill(9,10, 10, 7, pgm_read_byte(&(wood_color1 [2])));
	DrawMap2(9,10,board_center_12x10);

	// Redraw the Draw Pile.
	DrawMap2(pgm_read_byte(&(draw_pos[0])), pgm_read_byte(&(draw_pos[1])), card_back_lg);

	// Redraw the Draw pile stack.
	redrawUnderDrawPile();

	// Redraw the last discard if it was a valid card.
	if(game.lastCardPlayed != 255){
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
		DrawMap2(5 ,20+2, b_l_corner_h_fwd);
		DrawMap2(5 ,20  , b_l_corner_v_fwd);
		DrawMap2(5 ,5   , t_l_corner_h_fwd);
		DrawMap2(5 ,5   , t_l_corner_v_fwd);
		DrawMap2(20,5   , t_r_corner_h_fwd);
		DrawMap2(20+2,5 , t_r_corner_v_fwd);
		DrawMap2(20,20+2, b_r_corner_h_fwd);
		DrawMap2(20+2,20, b_r_corner_v_fwd);
	}
	else if(game.direction==BACKWARD) {
		DrawMap2(5 ,20+2, b_l_corner_h_bkd);
		DrawMap2(5 ,20  , b_l_corner_v_bkd);
		DrawMap2(5 ,5   , t_l_corner_h_bkd);
		DrawMap2(5 ,5   , t_l_corner_v_bkd);
		DrawMap2(20,5   , t_r_corner_h_bkd);
		DrawMap2(20+2,5 , t_r_corner_v_bkd);
		DrawMap2(20,20+2, b_r_corner_h_bkw);
		DrawMap2(20+2,20, b_r_corner_v_bkw);
	}

	// Active color
	if     (cards[game.lastCardPlayed].color==CARD_YELLOW){ colorTile=pgm_read_byte(&(border_yellow[2])) ; }
	else if(cards[game.lastCardPlayed].color==CARD_BLUE  ){ colorTile=pgm_read_byte(&(border_blue  [2])) ; }
	else if(cards[game.lastCardPlayed].color==CARD_RED   ){ colorTile=pgm_read_byte(&(border_red   [2])) ; }
	else if(cards[game.lastCardPlayed].color==CARD_GREEN ){ colorTile=pgm_read_byte(&(border_green [2])) ; }
	else                                                  { colorTile=pgm_read_byte(&(wood_color2  [2])) ; }

	void clearBorderColor(){
		const u8 replacementTile = pgm_read_byte(&(wood_color1 [2]));
		// const u8 replacementTile = 0;
		// Player 1 side.
		Fill(8 ,22,12,1 ,replacementTile);
		// Player 2 side.
		Fill(5 ,8 ,1 ,12,replacementTile);
		// Player 3 side.
		Fill(8 ,5 ,12,1 ,replacementTile);
		// Player 4 side.
		Fill(22,8 ,1 ,12,replacementTile);
	}

	// Clear the borders.
	clearBorderColor();

	if(game.gamestate2==GS_PLAYING_PLAYING){
		// Draw the colored border only for the active player.
		if     (game.activePlayer==1){ Fill(8 ,22,12,1 ,colorTile); }
		else if(game.activePlayer==2){ Fill(5 ,8 ,1 ,12,colorTile); }
		else if(game.activePlayer==3){ Fill(8 ,5 ,12,1 ,colorTile); }
		else if(game.activePlayer==4){ Fill(22,8 ,1 ,12,colorTile); }
	}

	// debug_showDebugData();
}
// Displays player name, card count, uno.
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
		N782_print    ( 23 , VRAM_TILES_V-3 , S_P1   , 0 ) ;
		N782_print    ( 23 , VRAM_TILES_V-2 , S_CARDS, 0 ) ;
		Fill(25 , VRAM_TILES_V-1, 3,1, 0);
		N782_print_u8 ( 23 , VRAM_TILES_V-1 , cardCount_p1) ;

		// Clear the UNO indicator.
		Fill(24 , VRAM_TILES_V-4, 4,1, 0x00);

		// Set the UNO indicator where applicable.
		if( cardCount_p1==1 ) { N782_print( 24 , VRAM_TILES_V-4, S_UNO , 0 ) ; }
	}
	// P2
	if(game.activePlayers[1]==1){
		cardCount_p2 = countPlayerCards(2);
		N782_print    ( 0 , VRAM_TILES_V-3 , S_P2   , 0 ) ;
		N782_print    ( 0 , VRAM_TILES_V-2 , S_CARDS, 0 ) ;
		Fill(2 , VRAM_TILES_V-1, 3,1, 0);
		N782_print_u8 ( 0 , VRAM_TILES_V-1 , cardCount_p2) ;
		Fill(1  , VRAM_TILES_V-4, 4,1, 0x00);
		if( cardCount_p2==1 ) { N782_print( 1  , VRAM_TILES_V-4, S_UNO , 0 ) ; }
	}
	// P3
	if(game.activePlayers[2]==1){
		cardCount_p3 = countPlayerCards(3);
		N782_print    ( 0 , 0 , S_P3   ,0 ) ;
		N782_print    ( 0 , 1 , S_CARDS,0)  ;
		Fill(2 , 2, 3,1, 0);
		N782_print_u8 ( 0 , 2 , cardCount_p3) ;
		Fill(1  , 3             , 4,1, 0x00);
		if( cardCount_p3==1 ) { N782_print( 1  , 3             , S_UNO , 0 ) ; }
	}
	// P4
	if(game.activePlayers[3]==1){
		cardCount_p4 = countPlayerCards(4);
		N782_print    ( 23 , 0 , S_P4   ,0 ) ;
		N782_print    ( 23 , 1 , S_CARDS,0 ) ;
		Fill(25 , 2, 3,1, 0);
		N782_print_u8 ( 23 , 2 , cardCount_p4) ;
		Fill(24 , 3             , 4,1, 0x00);
		if( cardCount_p4==1 ) { N782_print( 24 , 3             , S_UNO , 0 ) ; }
	}

}

// DONE -- gstate_title
static void gstate_title_n782(){
	// nicksen782 lense flare animation.

	ClearVram();

	// Draw functions.
	void face_frame1(){ DrawMap2((VRAM_TILES_H/2) - (7/2), 11,N782_FACE1_F1); }
	void face_frame2(){ DrawMap2((VRAM_TILES_H/2) - (7/2), 11,N782_FACE1_F2); }
	void face_frame3(){ DrawMap2((VRAM_TILES_H/2) - (7/2), 11,N782_FACE1_F3); }
	void face_frame4(){ DrawMap2((VRAM_TILES_H/2) - (7/2), 11,N782_FACE1_F4); }
	void face_frame5(){ DrawMap2((VRAM_TILES_H/2) - (7/2), 11,N782_FACE1_F5); }
	void face_frame6(){ DrawMap2((VRAM_TILES_H/2) - (7/2), 11,N782_FACE1_F1); }

	// Arrays of function pointers.
	void (*face_frames[6])() = { face_frame1, face_frame2, face_frame3, face_frame4, face_frame5, face_frame1 };

	// Different timers for the top and bottom.
	u8 * faceTimer = &vsynccounter8b_gen1;
	* faceTimer=0;
	s8 currentFace = 0;
	u8 faceAnimations=0;

	ClearVram();
	SetTileTable( bg_tiles2 );

	WaitVsync(25);

	DrawMap2((VRAM_TILES_H/2) - (7/2), 11,N782_FACE1_F1);
	DrawMap2((VRAM_TILES_H/2) - (7/2), 16 ,N782_TEXT1_F1);

	while(game.gamestate2==GS_TITLE_N782    ){
		if(*faceTimer > 12 && faceAnimations < 2){
			face_frames[currentFace]();
			currentFace+=1;
			if     (currentFace==5) {
				currentFace=0;
				faceAnimations++;
			}
			// Reset the counter.
			*faceTimer=0;
		}

		if(*faceTimer>25 && faceAnimations==2){
			// WaitVsync(50);
			DrawMap2((VRAM_TILES_H/2) - (7/2), 11,N782_FACE1_F1);
			DrawMap2((VRAM_TILES_H/2) - (7/2), 16,N782_TEXT1_F1);
			WaitVsync(50);
			break;
		}
	}

	ClearVram();
	SetTileTable( bg_tiles );

	game.gamestate2=GS_TITLE_UZEBOX;
}
static void gstate_title_uzebox(){
	ClearVram();
	SetTileTable( bg_tiles );
	u8 centered_x=(VRAM_TILES_H/2) - (9/2);
	u8 centered_y=(VRAM_TILES_V/2) - 3/2;

	// N782_print( (VRAM_TILES_H/2) - (6/2), (VRAM_TILES_V/2) - 1 ,PSTR("UZEBOX"), 0);
	DrawMap2(centered_x,centered_y,UZEBOX_TEXT1_F1);

	u8 * textTimer = &vsynccounter8b_gen2;
	* textTimer=0;
	s8 currentText = 0;
	u8 textTurns=0;

	void text_frame1() { DrawMap2( (VRAM_TILES_H/2) - (9/2), (VRAM_TILES_V/2) - 3/2, UZEBOX_TEXT1_F1 ); }
	void text_frame2() { DrawMap2( (VRAM_TILES_H/2) - (9/2), (VRAM_TILES_V/2) - 3/2, UZEBOX_TEXT1_F2 ); }
	void text_frame3() { DrawMap2( (VRAM_TILES_H/2) - (9/2), (VRAM_TILES_V/2) - 3/2, UZEBOX_TEXT1_F3 ); }
	void text_frame4() { DrawMap2( (VRAM_TILES_H/2) - (9/2), (VRAM_TILES_V/2) - 3/2, UZEBOX_TEXT1_F4 ); }
	void text_frame5() { DrawMap2( (VRAM_TILES_H/2) - (9/2), (VRAM_TILES_V/2) - 3/2, UZEBOX_TEXT1_F5 ); }
	void text_frame6() { DrawMap2( (VRAM_TILES_H/2) - (9/2), (VRAM_TILES_V/2) - 3/2, UZEBOX_TEXT1_F6 ); }
	void text_frame7() { DrawMap2( (VRAM_TILES_H/2) - (9/2), (VRAM_TILES_V/2) - 3/2, UZEBOX_TEXT1_F7 ); }
	void text_frame8() { DrawMap2( (VRAM_TILES_H/2) - (9/2), (VRAM_TILES_V/2) - 3/2, UZEBOX_TEXT1_F8 ); }
	void text_frame9() { DrawMap2( (VRAM_TILES_H/2) - (9/2), (VRAM_TILES_V/2) - 3/2, UZEBOX_TEXT1_F9 ); }
	void text_frame10(){ DrawMap2( (VRAM_TILES_H/2) - (9/2), (VRAM_TILES_V/2) - 3/2, UZEBOX_TEXT1_F10); }
	void (*text_frames[9])() = {
		/*text_frame1,*/
		text_frame2 ,
		text_frame3 ,
		text_frame4 ,
		text_frame5 ,
		text_frame6 ,
		text_frame7 ,
		text_frame8 ,
		text_frame9 ,
		text_frame10
	};

	while(game.gamestate2==GS_TITLE_UZEBOX  ){
		// break;

		if(*textTimer > 2 ){
			text_frames[currentText]();
			currentText+=1;
			if     (currentText==9) { currentText=0; textTurns++; }
			// Reset the counter.
			*textTimer=0;
		}

		if(textTurns>=4){ break; }
	}

	DrawMap2(centered_x,centered_y,UZEBOX_TEXT1_F1);

	WaitVsync(50);
	game.gamestate2=GS_TITLE_MAINMENU;
}
static void gstate_title_mainmenu(){
	ClearVram();
	debug_showDebugData();
	SetTileTable( bg_tiles );

	DrawMap2(0, 0, titlescreen_28x28);

	// sideCardTimer setup.
	// u8 sideCardTimer=0;

	// Color flash setup.
	u8 * colorTimer = &vsynccounter8b_gen2;
	u8 currentTile = 1;
	u8 color_yellow = pgm_read_byte(&(border_yellow[2]));
	u8 color_blue   = pgm_read_byte(&(border_blue  [2]));
	u8 color_red    = pgm_read_byte(&(border_red   [2]));
	u8 color_green  = pgm_read_byte(&(border_green [2]));
	// u8 color_black  = pgm_read_byte(&(border_black [2]));
	*colorTimer = 5;
	// Copy the tile to a ramtile. Replace instances of that tile in vram with the ramtile.
	CopyFlashTile(color_yellow, 0);
	for(u8 y=0; y<VRAM_TILES_V; y+=1){
		for(u8 x=0; x<VRAM_TILES_H; x+=1){
			if(vram[(y*VRAM_TILES_H)+x]==color_yellow+RAM_TILES_COUNT){
				vram[(y*VRAM_TILES_H)+x]=0;
			}
		}
	}
	CopyFlashTile(color_green, 0);

	// Cursor config.
	u8 * cursorTimer = &vsynccounter8b_gen1;
	u8 currentCursor = 0;
	const u8 * cursor1map;
	const u8 *cursor2map;
	// u8 cursorIndex;
	u8 cursor_x=4;
	u8 cursor_y=16;
	u8 y_rows[] = {
		16,18,20,22
	};
	u8 y_rows_index = 0;
	cursor1map=cursor_f1_r;
	cursor2map=cursor_f2_r;
	*cursorTimer=15;

	while(game.gamestate1==GS_TITLE){
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
				game.gamestate1=GS_OPTIONS;
				game.gamestate2=GS_OPTIONS_2;
			}

			// Game setup (Uzenet).
			else if(y_rows_index==1){
				game.gamestate1=GS_OPTIONS;
				game.gamestate2=GS_OPTIONS_1;
			}

			// Rules
			else if(y_rows_index==2){
				game.gamestate1=GS_RULES;
				game.gamestate2=GS_RULES_1;
			}

			// Credits
			else if(y_rows_index==3){
				game.gamestate1=GS_CREDITS;
				game.gamestate2=GS_CREDITS_1;
			}

			// Hide the cursor.
			MapSprite2( 0, cursor1map, SPRITE_OFF);

			// Wait for the player to release the button.
			while ( game.btnHeld1 ){ getInputs(); WaitVsync(1); }

			break;
		}

		// Update the cursor.
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

		// Update the color flash.
		if(*colorTimer > 15 ){
			// Change to which map?
			switch(currentTile){
				case 0  : { CopyFlashTile(color_yellow, 0); currentTile++; break; }
				case 1  : { CopyFlashTile(color_blue  , 0); currentTile++; break; }
				case 2  : { CopyFlashTile(color_red   , 0); currentTile++; break; }
				case 3  : { CopyFlashTile(color_green , 0); currentTile=0; break; }
				default : { break; }
			};

			// Reset the counter.
			*colorTimer=0;
		}

		// Update the side card animation.
		// if(*sideCardTimer > 10 ){
		// }

		WaitVsync(1);
	}
}
static void gstate_title(){
	while(game.gamestate1==GS_TITLE){
		if     (game.gamestate2==GS_TITLE_N782    ){ gstate_title_n782()    ; }
		else if(game.gamestate2==GS_TITLE_UZEBOX  ){ gstate_title_uzebox()  ; }
		else if(game.gamestate2==GS_TITLE_MAINMENU){ gstate_title_mainmenu(); }
		else{
			errorHandler(INVALIDGAMESTATE2);
		}
	}
}

// DONE -- gstate_options
static void gstate_options1(){
	// Uzenet setup
	// NOTHING HERE YET!
	DrawMap2(0,0,options1_28x28);
	while(game.gamestate2==GS_OPTIONS_1){
		getInputs();
		if( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }
		if( (game.btnHeld1) ) {
			while ( game.btnHeld1 ){ getInputs(); WaitVsync(1); }
			game.gamestate1=GS_OPTIONS;
			game.gamestate2=GS_OPTIONS_2;
		}
	}

}
// static void gstate_options2(){
// 	DrawMap2(0,0,options2_28x28);

// 	// Four boxes.
// 		// Player setup
// 			// 4 rows.
// 			// Each row can be adjusted to have the value of "HUMAN", "CPU", "NONE".
// 		// Win Style
// 			// 2 rows.
// 			// Each row can be adjusted to have the value of "FIRST TO ZERO CARDS", "FIRST TO 500 POINTS".
// 		// No Playable Card.
// 			// 2 rows.
// 			// Each row can be adjusted to have the value of "DRAW ONLY ONE CARD", "DRAW UNTIL PLAYABLE".
// 	// DPAD changes the cursor. DPAD changes the option for the Players box.
// 	// A accepts the selection(s) and moves to the next box.
// 		// On the last accepted box, the game starts.
// 	// B moves back to the previous box. Settings remain the same until changed.

// 	u8 currentBox=0;

// 	u8 box1_y_index=0;
// 	u8 box3_y_index=0;
// 	u8 box4_y_index=0;

// 	u8 box1_y_rows[4] = { 5 , 6  ,7, 8 };
// 	u8 box3_y_rows[2] = { 14, 15 };
// 	u8 box4_y_rows[2] = { 21, 22 };

// 	u8 cursor_box1_x=3;
// 	u8 cursor_box1_y=box1_y_rows[box1_y_index];

// 	u8 cursor_box3_x=3;
// 	u8 cursor_box3_y=box3_y_rows[box3_y_index];

// 	u8 cursor_box4_x=3;
// 	u8 cursor_box4_y=box4_y_rows[box4_y_index];

// 	// Cursor timer and current frame.
// 	u8 * cursorTimer = &vsynccounter8b_gen1;
// 	*cursorTimer=15;
// 	u8 currentCursor = 0;

// 	u8 nextX    ;
// 	u8 nextY    ;
// 	u8 nextType ;

// 	// These values will be copied to the game struct.
// 	u8 p1_type   = game.player_types[0] ;
// 	u8 p2_type   = game.player_types[1] ;
// 	u8 p3_type   = game.player_types[2] ;
// 	u8 p4_type   = game.player_types[3] ;
// 	u8 ai_type   = game.ai_type         ;
// 	u8 winStyle  = game.winStyle        ;
// 	u8 drawStyle = game.drawStyle       ;

// 	// PRINT THE CURRENT (default) PLAYER TYPES.

// 	void printPlayerType(u8 x, u8 y, enum playerTypes type){
// 		// Erase the area that is about to be printed to.
// 		Fill(x, y, 5,1, 0x00);
// 		// Print the player type.
// 		switch(type){
// 			case HUMAN : { N782_print(x, y, S_HUMAN, 0); break; }
// 			case CPU   : { N782_print(x, y, S_CPU  , 0); break; }
// 			case NONE  : { N782_print(x, y, S_NONE , 0); break; }
// 		};
// 	}
// 	// Display the default player types.
// 	printPlayerType( (cursor_box1_x+4) , box1_y_rows[0], game.player_types[0] );
// 	printPlayerType( (cursor_box1_x+4) , box1_y_rows[1], game.player_types[1] );
// 	printPlayerType( (cursor_box1_x+4) , box1_y_rows[2], game.player_types[2] );
// 	printPlayerType( (cursor_box1_x+4) , box1_y_rows[3], game.player_types[3] );

// 	// Game setup
// 	while(game.gamestate2==GS_OPTIONS_2){
// 		// Change the cursor frame?
// 		if(*cursorTimer > 15 ){
// 			// Hide all cursors.

// 			// "PLAYERS" box.
// 			MapSprite2( 0, cursor_f1_r, SPRITE_OFF );
// 			// "WIN STYLE" box.
// 			MapSprite2( 1, cursor_f1_r, SPRITE_OFF );
// 			// "DRAW STYLE" box.
// 			MapSprite2( 2, cursor_f1_r, SPRITE_OFF );

// 			// "PLAYERS" box.
// 			if     (currentBox==0){
// 				if     (currentCursor==0){
// 					MapSprite2( currentBox, cursor_f1_r, SPRITE_BANK0 );
// 					MoveSprite( currentBox, cursor_box1_x<<3 , cursor_box1_y<<3, 1, 1);
// 				}
// 				else                     {
// 					MapSprite2( currentBox, cursor_f2_r, SPRITE_BANK0 );
// 					MoveSprite( currentBox, cursor_box1_x<<3 , cursor_box1_y<<3, 1, 1);
// 				}
// 			}

// 			// "WIN STYLE" box.
// 			else if(currentBox==1){
// 				if     (currentCursor==0){
// 					MapSprite2( currentBox, cursor_f1_r, SPRITE_BANK0 );
// 					MoveSprite( currentBox, cursor_box3_x<<3 , cursor_box3_y<<3, 1, 1);
// 				}
// 				else                     {
// 					MapSprite2( currentBox, cursor_f2_r, SPRITE_BANK0 );
// 					MoveSprite( currentBox, cursor_box3_x<<3 , cursor_box3_y<<3, 1, 1);
// 				}
// 			}

// 			// "DRAW STYLE" box.
// 			else if(currentBox==2){
// 				if     (currentCursor==0){
// 					MapSprite2( currentBox, cursor_f1_r, SPRITE_BANK0 );
// 					MoveSprite( currentBox, cursor_box4_x<<3 , cursor_box4_y<<3, 1, 1);
// 				}
// 				else                     {
// 					MapSprite2( currentBox, cursor_f1_r, SPRITE_BANK0 );
// 					MoveSprite( currentBox, cursor_box4_x<<3 , cursor_box4_y<<3, 1, 1);
// 				}
// 			}

// 			// Flip the cursor frame.
// 			currentCursor = !currentCursor;

// 			// Reset the counter.
// 			*cursorTimer=0;
// 		}

// 		// Read gamepad.
// 		getInputs();

// 		// Look for the softReset command.
// 		if( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }

// 		// If button is held...
// 		if( (game.btnHeld1) ) {
// 			// "PLAYERS" box.
// 			if     (currentBox==0){
// 				// Move the cursor up or down.
// 				if( (game.btnHeld1 & BTN_UP) || (game.btnHeld1 & BTN_DOWN) ) {
// 					// Move cursor UP (do bounds check.)
// 					if( (game.btnHeld1 & BTN_UP   ) ) {
// 						box1_y_index = box1_y_index==0                     ? sizeof(box1_y_rows)-1 : box1_y_index-1;
// 					}
// 					// Move cursor DOWN (do bounds check.)
// 					if( (game.btnHeld1 & BTN_DOWN ) ) {
// 						box1_y_index = box1_y_index==sizeof(box1_y_rows)-1 ? 0                     : box1_y_index+1;
// 					}
// 					// Adjust cursor y position.
// 					cursor_box1_y=box1_y_rows[box1_y_index];
// 					// Redraw cursor.
// 					MoveSprite( 0 , cursor_box1_x<<3 , cursor_box1_y<<3, 1, 1);
// 				}
// 				// Change the selected option.
// 				else if( (game.btnHeld1 & BTN_LEFT) || (game.btnHeld1 & BTN_RIGHT) ) {
// 					if     (box1_y_index==0){
// 						// Set the new player type. (Do bounds check.)
// 						p1_type = p1_type==0 ? 2 : p1_type-1;
// 						// Set the nextX to print from.
// 						nextX=cursor_box1_x+4;
// 						// Set the nextY to print from.
// 						nextY=box1_y_rows[box1_y_index];
// 						// Set the nextType to print.
// 						nextType=p1_type;
// 					}
// 					else if(box1_y_index==1){
// 						p2_type = p2_type==0 ? 2 : p2_type-1;
// 						nextX=cursor_box1_x+4;
// 						nextY=box1_y_rows[box1_y_index];
// 						nextType=p2_type;
// 					}
// 					else if(box1_y_index==2){
// 						p3_type = p3_type==0 ? 2 : p3_type-1;
// 						nextX=cursor_box1_x+4;
// 						nextY=box1_y_rows[box1_y_index];
// 						nextType=p3_type;
// 					}
// 					else if(box1_y_index==3){
// 						p4_type = p4_type==0 ? 2 : p4_type-1;
// 						nextX=cursor_box1_x+4;
// 						nextY=box1_y_rows[box1_y_index];
// 						nextType=p4_type;
// 					}

// 					printPlayerType( nextX , nextY, nextType );

// 				}
// 				// Confirm selection, move to the next box.
// 				if( (game.btnHeld1 & BTN_A || game.btnHeld1 & BTN_START  ) ) {
// 					currentBox++;
// 				}
// 				// Return to the title screen.
// 				if( (game.btnHeld1 & BTN_B    ) ) {
// 					// Go back to the title screen.
// 					game.gamestate1=GS_TITLE;
// 					game.gamestate2=GS_TITLE_MAINMENU;
// 				}
// 			}

// 			// "WIN STYLE" box.
// 			else if(currentBox==1){
// 				// winStyle
// 				// Move the cursor up or down.
// 				if( (game.btnHeld1 & BTN_UP) || (game.btnHeld1 & BTN_DOWN) ) {
// 					// Move cursor UP (do bounds check.)
// 					if( (game.btnHeld1 & BTN_UP   ) ) {
// 						box3_y_index = box3_y_index==0                     ? sizeof(box3_y_rows)-1 : box3_y_index-1;
// 					}
// 					// Move cursor DOWN (do bounds check.)
// 					if( (game.btnHeld1 & BTN_DOWN ) ) {
// 						box3_y_index = box3_y_index==sizeof(box3_y_rows)-1 ? 0                     : box3_y_index+1;
// 					}
// 					// FIRSTTO500POINTS is 0, FIRSTTO0CARDS is 1.
// 					winStyle=box3_y_index;
// 					// Adjust cursor y position.
// 					cursor_box3_y=box3_y_rows[box3_y_index];
// 					// Redraw cursor.
// 					MoveSprite( 1 , cursor_box3_x<<3 , cursor_box3_y<<3, 1, 1);
// 				}
// 				// Confirm selection, move to the next box.
// 				if( (game.btnHeld1 & BTN_A || game.btnHeld1 & BTN_START  ) ) {
// 					currentBox++;
// 				}
// 				// Move to the previous box.
// 				if( (game.btnHeld1 & BTN_B    ) ) {
// 					currentBox--;
// 				}
// 			}

// 			// "DRAW STYLE" box.
// 			else if(currentBox==2){
// 				// Move the cursor up or down.
// 				if( (game.btnHeld1 & BTN_UP) || (game.btnHeld1 & BTN_DOWN) ) {
// 					// Move cursor UP (do bounds check.)
// 					if( (game.btnHeld1 & BTN_UP   ) ) {
// 						box4_y_index = box4_y_index==0                     ? sizeof(box4_y_rows)-1 : box4_y_index-1;
// 					}
// 					// Move cursor DOWN (do bounds check.)
// 					if( (game.btnHeld1 & BTN_DOWN ) ) {
// 						box4_y_index = box4_y_index==sizeof(box4_y_rows)-1 ? 0                     : box4_y_index+1;
// 					}
// 					//
// 					drawStyle=box4_y_index;
// 					// Adjust cursor y position.
// 					cursor_box4_y=box4_y_rows[box4_y_index];
// 					// Redraw cursor.
// 					MoveSprite( 1 , cursor_box4_x<<3 , cursor_box4_y<<3, 1, 1);
// 				}
// 				// Confirm selection. Save settings and start the game.
// 				if( (game.btnHeld1 & BTN_A || game.btnHeld1 & BTN_START  ) ) {
// 					// Hide all cursors.
// 					MapSprite2( 0, cursor_f1_r, SPRITE_OFF );
// 					MapSprite2( 1, cursor_f1_r, SPRITE_OFF );
// 					MapSprite2( 2, cursor_f1_r, SPRITE_OFF );
// 					MapSprite2( 3, cursor_f1_r, SPRITE_OFF );

// 					// Save the settings to the game struct.

// 					// Count and set the active players. (Either HUMAN or CPU.)
// 					game.numPlayers    =  0                         ;
// 					if( p1_type==HUMAN || p1_type==CPU ) { game.numPlayers+=1; game.activePlayers[0]=1; } else{ game.activePlayers[0]=0 ; }
// 					if( p2_type==HUMAN || p2_type==CPU ) { game.numPlayers+=1; game.activePlayers[1]=1; } else{ game.activePlayers[1]=0 ; }
// 					if( p3_type==HUMAN || p3_type==CPU ) { game.numPlayers+=1; game.activePlayers[2]=1; } else{ game.activePlayers[2]=0 ; }
// 					if( p4_type==HUMAN || p4_type==CPU ) { game.numPlayers+=1; game.activePlayers[3]=1; } else{ game.activePlayers[3]=0 ; }

// 					// Make sure there is at least 2 players.
// 					if(game.numPlayers<2){
// 						currentBox=0;
// 						// Wait for the player to release the button.
// 						while ( game.btnHeld1 ){ getInputs(); WaitVsync(1); }
// 						continue;
// 					}

// 					// Set the player types.
// 					game.player_types[0] = p1_type;
// 					game.player_types[1] = p2_type;
// 					game.player_types[2] = p3_type;
// 					game.player_types[3] = p4_type;

// 					// Set the ai_type, winStyle, drawStyle.
// 					game.ai_type        = ai_type   ;
// 					game.winStyle       = winStyle  ;
// 					if     (game.winStyle==FIRSTTO500POINTS){ game.pointsForWin=500; }
// 					else if(game.winStyle==FIRSTTO0CARDS   ){ game.pointsForWin=0;   }

// 					game.drawStyle      = drawStyle ;

// 					// Reset accumulated points.
// 					game.points_p1      = 0         ;
// 					game.points_p2      = 0         ;
// 					game.points_p3      = 0         ;
// 					game.points_p4      = 0         ;

// 					// Set active player to invalid, last card played to invalid, direction to forward.
// 					game.activePlayer   = 255       ;
// 					game.lastCardPlayed = 255       ;
// 					game.direction      = FORWARD   ;

// 					// DEBUG
// 					game.autoPlay=0;
// 					game.buttonPresses=0;
// 					game.handsPlayed=0;
// 					// DEBUG

// 					// Start a new game.
// 					game.gamestate1=GS_PLAYING;
// 					game.gamestate2=GS_PLAYING_GAMESTART;

// 					// ClearVram();
// 					// btnPressed1
// 					// getRandomSeedViaUserInput();
// 					game.randomSeed=0;
// 					srand(game.randomSeed);

// 					break;
// 				}
// 				// Move to the previous box.
// 				if( (game.btnHeld1 & BTN_B    ) ) {
// 					currentBox--;
// 				}
// 			}

// 			// While a button is still held...
// 			while ( game.btnHeld1 ){ getInputs(); }
// 		}

// 	}
// }
static void gstate_options2(){
	DrawMap2(0,0,options2_28x28);

	// Four boxes.
		// Player setup
			// 4 rows.
			// Each row can be adjusted to have the value of "HUMAN", "CPU", "NONE".
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
	u8 box3_y_index=0;
	u8 box4_y_index=0;

	u8 box1_y_rows[4] = { 5+7 , 6+7  ,7+7, 8+7 };
	u8 box3_y_rows[2] = { 14, 15 };
	u8 box4_y_rows[2] = { 21, 22 };

	u8 cursor_box1_x=3;
	u8 cursor_box1_y=box1_y_rows[box1_y_index];

	u8 cursor_box3_x=3+13;
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

	// PRINT THE CURRENT (default) PLAYER TYPES.

	void printPlayerType(u8 x, u8 y, enum playerTypes type){
		// Erase the area that is about to be printed to.
		Fill(x, y, 5,1, 0x00);
		// Print the player type.
		switch(type){
			case HUMAN : { N782_print(x, y, S_HUMAN, 0); break; }
			case CPU   : { N782_print(x, y, S_CPU  , 0); break; }
			case NONE  : { N782_print(x, y, S_NONE , 0); break; }
		};
	}
	// Display the default player types.
	printPlayerType( (cursor_box1_x+4) , box1_y_rows[0], game.player_types[0] );
	printPlayerType( (cursor_box1_x+4) , box1_y_rows[1], game.player_types[1] );
	printPlayerType( (cursor_box1_x+4) , box1_y_rows[2], game.player_types[2] );
	printPlayerType( (cursor_box1_x+4) , box1_y_rows[3], game.player_types[3] );

	// Game setup
	while(game.gamestate2==GS_OPTIONS_2){
		// Change the cursor frame?
		if(*cursorTimer > 15 ){
			// Hide all cursors.
			// N782_print_u8(0,0,currentBox);
			// "PLAYERS" box.
			// MapSprite2( 0, cursor_f1_r, SPRITE_OFF );
			// // "WIN STYLE" box.
			// MapSprite2( 1, cursor_f1_r, SPRITE_OFF );
			// // "DRAW STYLE" box.
			// MapSprite2( 2, cursor_f1_r, SPRITE_OFF );

			// "PLAYERS" box.
			if     (currentBox==0){
				MapSprite2( currentBox, cursor_f1_r, SPRITE_OFF );
				if     (currentCursor==0){
					MapSprite2( currentBox, cursor_f1_r, SPRITE_BANK0 );
					MoveSprite( currentBox, cursor_box1_x<<3 , cursor_box1_y<<3, 1, 1);
				}
				else                     {
					MapSprite2( currentBox, cursor_f2_r, SPRITE_BANK0 );
					MoveSprite( currentBox, cursor_box1_x<<3 , cursor_box1_y<<3, 1, 1);
				}
			}

			// "WIN STYLE" box.
			else if(currentBox==1){
				MapSprite2( currentBox, cursor_f1_r, SPRITE_OFF );
				if     (currentCursor==0){
					MapSprite2( currentBox, cursor_f1_r, SPRITE_BANK0 );
					MoveSprite( currentBox, cursor_box3_x<<3 , cursor_box3_y<<3, 1, 1);
				}
				else                     {
					MapSprite2( currentBox, cursor_f2_r, SPRITE_BANK0 );
					MoveSprite( currentBox, cursor_box3_x<<3 , cursor_box3_y<<3, 1, 1);
				}
			}

			// "DRAW STYLE" box.
			else if(currentBox==2){
				MapSprite2( currentBox, cursor_f1_r, SPRITE_OFF );
				if     (currentCursor==0){
					MapSprite2( currentBox, cursor_f1_r, SPRITE_BANK0 );
					MoveSprite( currentBox, cursor_box4_x<<3 , cursor_box4_y<<3, 1, 1);
				}
				else                     {
					MapSprite2( currentBox, cursor_f2_r, SPRITE_BANK0 );
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

					printPlayerType( nextX , nextY, nextType );

				}
				// Confirm selection, move to the next box.
				if( (game.btnHeld1 & BTN_A || game.btnHeld1 & BTN_START  ) ) {
					currentBox++;
				}
				// Return to the title screen.
				if( (game.btnHeld1 & BTN_B    ) ) {
					// Go back to the title screen.
					game.gamestate1=GS_TITLE;
					game.gamestate2=GS_TITLE_MAINMENU;
				}
			}

			// "WIN STYLE" box.
			else if(currentBox==1){
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
					// FIRSTTO500POINTS is 0, FIRSTTO0CARDS is 1.
					winStyle=box3_y_index;
					// Adjust cursor y position.
					cursor_box3_y=box3_y_rows[box3_y_index];
					// Redraw cursor.
					MoveSprite( 1 , cursor_box3_x<<3 , cursor_box3_y<<3, 1, 1);
				}
				// Confirm selection, move to the next box.
				if( (game.btnHeld1 & BTN_A || game.btnHeld1 & BTN_START  ) ) {
					currentBox++;
				}
				// Move to the previous box.
				if( (game.btnHeld1 & BTN_B    ) ) {
					currentBox--;
				}
			}

			// "DRAW STYLE" box.
			else if(currentBox==2){
				// Move the cursor up or down.
				if( (game.btnHeld1 & BTN_UP) || (game.btnHeld1 & BTN_DOWN) ) {
					// Move cursor UP (do bounds check.)
					if( (game.btnHeld1 & BTN_UP   ) ) {
						box4_y_index = box4_y_index==0 ? sizeof(box4_y_rows)-1 : box4_y_index-1;
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
					MoveSprite( 2 , cursor_box4_x<<3 , cursor_box4_y<<3, 1, 1);
				}
				// Confirm selection. Save settings and start the game.
				if( (game.btnHeld1 & BTN_A || game.btnHeld1 & BTN_START  ) ) {
					// Hide all cursors.
					MapSprite2( 0, cursor_f1_r, SPRITE_OFF );
					MapSprite2( 1, cursor_f1_r, SPRITE_OFF );
					MapSprite2( 2, cursor_f1_r, SPRITE_OFF );

					// Save the settings to the game struct.

					// Count and set the active players. (Either HUMAN or CPU.)
					game.numPlayers    =  0                         ;
					if( p1_type==HUMAN || p1_type==CPU ) { game.numPlayers+=1; game.activePlayers[0]=1; } else{ game.activePlayers[0]=0 ; }
					if( p2_type==HUMAN || p2_type==CPU ) { game.numPlayers+=1; game.activePlayers[1]=1; } else{ game.activePlayers[1]=0 ; }
					if( p3_type==HUMAN || p3_type==CPU ) { game.numPlayers+=1; game.activePlayers[2]=1; } else{ game.activePlayers[2]=0 ; }
					if( p4_type==HUMAN || p4_type==CPU ) { game.numPlayers+=1; game.activePlayers[3]=1; } else{ game.activePlayers[3]=0 ; }

					// Make sure there is at least 2 players.
					if(game.numPlayers<2){
						currentBox=0;
						// Wait for the player to release the button.
						while ( game.btnHeld1 ){ getInputs(); WaitVsync(1); }
						continue;
					}

					// Set the player types.
					game.player_types[0] = p1_type;
					game.player_types[1] = p2_type;
					game.player_types[2] = p3_type;
					game.player_types[3] = p4_type;

					// Set the ai_type, winStyle, drawStyle.
					game.ai_type        = ai_type   ;
					game.winStyle       = winStyle  ;
					if     (game.winStyle==FIRSTTO500POINTS){ game.pointsForWin=500; }
					else if(game.winStyle==FIRSTTO0CARDS   ){ game.pointsForWin=0;   }

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

					// DEBUG
					game.autoPlay=0;
					game.buttonPresses=0;
					game.handsPlayed=0;
					// DEBUG

					// Start a new game.
					game.gamestate1=GS_PLAYING;
					game.gamestate2=GS_PLAYING_GAMESTART;

					// ClearVram();
					// btnPressed1
					getRandomSeedViaUserInput();
					// game.randomSeed=1;
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
	while(game.gamestate1==GS_OPTIONS){
		if     (game.gamestate2==GS_OPTIONS_1){ gstate_options1(); }
		else if(game.gamestate2==GS_OPTIONS_2){ gstate_options2(); }
		else{
			errorHandler(INVALIDGAMESTATE2);
		}
	}
}

// DONE -- gstate_credits
static void gstate_credits1(){
	DrawMap2(0,0,credits1_28x28);
	while(game.gamestate2==GS_CREDITS_1){
		getInputs();
		if( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }
		if( (game.btnHeld1) ) {
			while ( game.btnHeld1 ){ getInputs(); WaitVsync(1); }
			game.gamestate1=GS_CREDITS;
			game.gamestate2=GS_CREDITS_2;
		}
	}

}
static void gstate_credits2(){
	DrawMap2(0,0,credits2_28x28);

	// Game setup
	while(game.gamestate2==GS_CREDITS_2){
		getInputs();
		if( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }
		if( (game.btnHeld1) ) {
			while ( game.btnHeld1 ){ getInputs(); WaitVsync(1); }
			game.gamestate1=GS_TITLE;
			game.gamestate2=GS_TITLE_MAINMENU;
		}
	}

}
static void gstate_credits(){
	while(game.gamestate1==GS_CREDITS){
		if     (game.gamestate2==GS_CREDITS_1){ gstate_credits1(); }
		else if(game.gamestate2==GS_CREDITS_2){ gstate_credits2(); }
		else{
			errorHandler(INVALIDGAMESTATE2);
		}
	}
}

// DONE -- gstate_rules
static void gstate_rules_1(){
	DrawMap2(0,0,rules1_28x28);
	while(game.gamestate2==GS_RULES_1){
		getInputs();
		if     ( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }
		else if( (game.btnHeld1) ) {
			while ( game.btnHeld1 ){ getInputs(); WaitVsync(1); }
			game.gamestate1=GS_RULES;
			game.gamestate2=GS_RULES_2;
		}
	}
}
static void gstate_rules_2(){
	DrawMap2(0,0,rules2_28x28);
	while(game.gamestate2==GS_RULES_2){
		getInputs();
		if     ( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }
		else if( (game.btnHeld1) ) {
			while ( game.btnHeld1 ){ getInputs(); WaitVsync(1); }
			game.gamestate1=GS_TITLE;
			game.gamestate2=GS_TITLE_MAINMENU;
		}
	}
}
static void gstate_rules(){
	while(game.gamestate1==GS_RULES){
		if     (game.gamestate2==GS_RULES_1){ gstate_rules_1(); }
		else if(game.gamestate2==GS_RULES_2){ gstate_rules_2(); }
		else{
			errorHandler(INVALIDGAMESTATE2);
		}
	}
}

// Resets the game for a new round. (Does NOT impact scores.)
static void newGameSetup(){
	// Clear all reserved ramtile data.
	clearAllReservedRamtiles();

	u8 playerWithHighestCard(u8 p_cards[4]){
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
		for(u8 i=0; i<TOTALCARDSINDECK; i+=1){
			switch(cards[i].location){
				case CARD_LOCATION_PLAYER1 : { p1_card=cards[i].value; (p_cards[0])=i; break; }
				case CARD_LOCATION_PLAYER2 : { p2_card=cards[i].value; (p_cards[1])=i; break; }
				case CARD_LOCATION_PLAYER3 : { p3_card=cards[i].value; (p_cards[2])=i; break; }
				case CARD_LOCATION_PLAYER4 : { p4_card=cards[i].value; (p_cards[3])=i; break; }
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
			// return winner ;
		}
		else{
			game.activePlayer = 255;
			// return 255;
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
			cardIndex2 = getRandomNumber(0, TOTALCARDSINDECK-1);
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
	game.activePlayer = 255 ;
	while( game.activePlayer == 255 ){
		//
		playerWithHighestCard(p_cards);

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

			// break;
		}

		// Invalid response. Loop again.
	}

	// Wait a moment to allow the user to read the displayed message.
	WaitVsync(125);

	// Clear the message. (The message at the lower half of the game board.)
	clearbgmessage();

	// Remove from the screen the cards temporarily assigned to each player. (ram tiles and display only.)
	if(game.activePlayers[0]==1){ removePlayerCards(1); }
	if(game.activePlayers[1]==1){ removePlayerCards(2); }
	if(game.activePlayers[2]==1){ removePlayerCards(3); }
	if(game.activePlayers[3]==1){ removePlayerCards(4); }

	// Reset the deck. (Return all cards back to the CARD_LOCATION_DRAW location.)
	resetTheDeck();

	// Assign cards to each player. (Does not display the cards.)
	for(u8 i=0; i<DEFAULTHANDSIZE; i+=1){
		// If at the first position or the last then reset to the first position.
		if(i%5==0) { pos=0; }

		// Assign a card to player 1.
		if(game.activePlayers[0]==1){ dealSpecifiedCard_anim(1, 255, pos, INITIALDEALBASESPEED+1, CARDS_FACEDOWN); }
		// Assign a card to player 2.
		if(game.activePlayers[1]==1){ dealSpecifiedCard_anim(2, 255, pos, INITIALDEALBASESPEED+0, CARDS_FACEDOWN); }
		// Assign a card to player 3.
		if(game.activePlayers[2]==1){ dealSpecifiedCard_anim(3, 255, pos, INITIALDEALBASESPEED+0, CARDS_FACEDOWN); }
		// Assign a card to player 4.
		if(game.activePlayers[3]==1){ dealSpecifiedCard_anim(4, 255, pos, INITIALDEALBASESPEED+5, CARDS_FACEDOWN); }

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

// ===== GS_PLAYING UTILITIES =====
// Draws a card face down for a player at the specified position (remove fully the card first!)
static void drawFaceDownCard(u8 playerNum, u8 position){
	// This function assumes that the cards have been removed first.
	// :: From the screen
	// :: From ramtile reservations (important!)
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
static void displayCards_byPlayer(u8 playerNum, u8 option){
	if( option==CARDS_FACEDOWN ){
		// Get the count of this player's cards.
		u8 numCards = countPlayerCards(playerNum) ;

		for(u8 pos=0; pos<5; pos+=1){
			// Remove the existing card.
			removeCard_sm(playerNum, pos);

			// Draw the card face down.
			if(numCards>pos){ drawFaceDownCard(playerNum, pos); }

			// Remove and hide the card.
			else            { removeCard_sm(playerNum, pos); }
		}
	}

	if( option==CARDS_FACEUP ){
		// This DOES remove the existing card before drawing the new card.
		// Also will remove any cards NOT drawn (EX: If there were 4 and now there are 3 then the 4th would be removed too.)
		// displayCardsForPlayer(game.activePlayer, game.handRow*5, 1);
		displayCardsForPlayer(playerNum, game.handRow*5, 1);
	}
}
// Determine where the card select cursor should be based on cursor index and player number.
static void getCursorPos(u8 playerNum, u8 cursorIndex, u8 *x, u8 *y){
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
// Allows the user to choose a new color when using a Wild card.
static void askUserToSetNewColor(enum SNC_choices howToChoose){
	// Ask the user to chose the new color.
	// Use the message area rows and the draw/discard rows.
	// The erased rows will be drawn after the user makes their selection.

	const u8 replacementTile = pgm_read_byte(&(wood_color2 [2]));
	// const u8 YELLOW_VALUE    = pgm_read_byte(&(yellow_color[2]));
	// const u8 BLUE_VALUE      = pgm_read_byte(&(blue_color  [2]));
	// const u8 RED_VALUE       = pgm_read_byte(&(red_color   [2]));
	// const u8 GREEN_VALUE     = pgm_read_byte(&(green_color [2]));
	// const u8 BLACK_VALUE     = pgm_read_byte(&(blackTile   [2]));

	u8 * cpu_autoConfirmTimer = &vsynccounter8b_gen2;
	// *cpu_autoConfirmTimer=0;

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

	u8 numRowsToDraw=7;
	const u8 rowWidth=14;

	u8 startx = 7;
	u8 starty = 9;
	u8 x      = startx;
	u8 y      = starty;
	u8 * srcVram_ptr ;
	u8 * dstVram_ptr ;
	// u16 * srcVram_ptr ;
	// u16 * dstVram_ptr ;

	// // Remove the discard card.
	removeCard_lg();
	// Draw the top and the bottom row first.
	Fill(startx,starty  ,rowWidth,1, BLACK_VALUE);
	Fill(startx,starty+1,rowWidth,1, BLACK_VALUE);
	N782_print(startx+1, starty  , S_CHOOSECOLOR, 0);
	N782_print(startx+3, starty+1, S_PRESSA     , 0);

	// Move down to the next row.
	y++;

	// Open the window.
	for(u8 i=0; i<numRowsToDraw; i+=1){
		//
		WaitVsync(3);

		// Set pointers for memmove.
		srcVram_ptr = &(vram[((y)   * VRAM_TILES_H)+x]);
		dstVram_ptr = &(vram[((y+1) * VRAM_TILES_H)+x]);

		// Move the last row down 1.
		memmove( dstVram_ptr, srcVram_ptr, rowWidth );

		// Replace the old row with black.
		Fill(startx,y,rowWidth,1, BLACK_VALUE);

		// Set the left tile.
		// SetTile(startx, y, BLUE_VALUE);

		// Set the right tile.
		// SetTile(startx+rowWidth-1, y, GREEN_VALUE);

		// Increment y.
		y++;
	}

	// Draw the color options (use ram tiles from the end which are normally used for sprites.)
	clearRamtile( RAM_TILES_COUNT-1, YELLOW_VALUE);
	clearRamtile( RAM_TILES_COUNT-2, BLUE_VALUE  );
	clearRamtile( RAM_TILES_COUNT-3, RED_VALUE   );
	clearRamtile( RAM_TILES_COUNT-4, GREEN_VALUE );
	Fill(startx+2-1   ,starty+3, 3, 2, (RAM_TILES_COUNT-1)-RAM_TILES_COUNT);
	Fill(startx+5-1   ,starty+3, 3, 2, (RAM_TILES_COUNT-2)-RAM_TILES_COUNT);
	Fill(startx+8-1   ,starty+3, 3, 2, (RAM_TILES_COUNT-3)-RAM_TILES_COUNT);
	Fill(startx+11-1  ,starty+3, 3, 2, (RAM_TILES_COUNT-4)-RAM_TILES_COUNT);

	SetSpriteVisibility(true);

	u8 colorIsSelected=0;

	// Loop until a choice has been made.
	while(1){
		if     (howToChoose == SNC_CPUAUTOCHOICE && colorIsSelected==0){
			WaitVsync(15);
			// Set the new discard color as selected by the CPU.
			switch(counts.hasMostColorOf){
				case CARD_YELLOW: { cards[game.lastCardPlayed].color=CARD_YELLOW; cursorIndex=0; break; }
				case CARD_BLUE  : { cards[game.lastCardPlayed].color=CARD_BLUE;   cursorIndex=1; break; }
				case CARD_RED   : { cards[game.lastCardPlayed].color=CARD_RED;    cursorIndex=2; break; }
				case CARD_GREEN : { cards[game.lastCardPlayed].color=CARD_GREEN;  cursorIndex=3; break; }
				default : { break; }
			};
			cursor_x = pgm_read_byte(&(wild_pos_cursor[cursorIndex][0]));
			cursor_y = pgm_read_byte(&(wild_pos_cursor[cursorIndex][1]));
			colorIsSelected=1;
			playSFX(SELECTCURSOR1);
			*cpu_autoConfirmTimer=0;
		}
		else if(howToChoose == SNC_PLAYERCHOICE ){
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
				colorIsSelected=1;
				*cpu_autoConfirmTimer=CPUCHOOSECOLORDELAY;
			}
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

		// Was a selection made?
		if(colorIsSelected==1 && *cpu_autoConfirmTimer>=CPUCHOOSECOLORDELAY){
			// Hide the sprite.
			MapSprite2( 0, cursor1map, SPRITE_OFF );
			// MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);
			// MoveSprite(0, OFF_SCREEN, OFF_SCREEN, 1, 1);
			WaitVsync(1);
			SetSpriteVisibility(false);

			// Black out the title bars.
			Fill(startx,starty ,rowWidth,1, BLACK_VALUE);
			Fill(startx,y      ,rowWidth,1, BLACK_VALUE);

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

		// Keep in sync.
		WaitVsync(1);
	}

}
// Draws and moves a card as a sprite from point A to point B.
static void moveCard( struct cardData_ *cardData, u8 autoRemove, u8 autoRemoveDelay, u8 cardDelay){
	// Reset the counter. (Is used to speed-limit the card movement.)
	u8 counter=0;
	// u8 cardDelay=ENDOFROUNDDEALBASESPEED;
	u8 sprite_x;
	u8 sprite_y;
	s8 xdir;
	s8 ydir;

	// Draw the card.
	drawCard(
		cardData->player   ,
		cardData->tile_x   ,
		cardData->tile_y   ,
		cardData->cardSize ,
		cardData->value    ,
		cardData->color
	);

	// Get the tilemap for the new card.
	setVramTilemap( cardData->tile_x, cardData->tile_y , cardData->w, cardData->h, (cardData->map) );

	// Hide the vram card.
	Fill(cardData->tile_x, cardData->tile_y , cardData->w, cardData->h, cardData->fillTile);

	// Map the card to a spritemap.
	MapSprite2_nra( 0, (cardData->map), SPRITE_BANK1 );

	sprite_x = cardData->tile_x << 3;
	sprite_y = cardData->tile_y << 3;

	// Set the xdir for the card movement.
	if     (sprite_x<cardData->dst_x<< 3){ xdir =  1; }
	else if(sprite_x>cardData->dst_x<< 3){ xdir = -1; }
	else                                 { xdir =  0; }

	// Set the ydir for the card movement.
	if     (sprite_y<cardData->dst_y<< 3){ ydir =  1; }
	else if(sprite_y>cardData->dst_y<< 3){ ydir = -1; }
	else                                 { ydir =  0; }

	// Move the card.
	while(1){
		// Determine if an x and/or y movement is required this iteration.
		if(sprite_x != cardData->dst_x<< 3){ sprite_x += xdir; }
		if(sprite_y != cardData->dst_y<< 3){ sprite_y += ydir; }

		// Redraw the sprite at the new location.
		MoveSprite(0, sprite_x, sprite_y,  (cardData->map)[0],  (cardData->map)[1]);

		// If both the x and the y values match the discard values then break.
		if( sprite_x==cardData->dst_x<< 3 && sprite_y==cardData->dst_y<< 3 ) { break; }

		// Do a vsync wait every other time.
		if(counter % cardDelay == 0 ){ WaitVsync(1); }

		// Increment the counter.
		counter+=1;
	}

	// Do we remove the card?
	if(autoRemove){
		// Is there a delay on the removal?
		if(autoRemoveDelay){ WaitVsync(autoRemoveDelay); }

		// Hide the card spritemap.
		MapSprite2_nra( 0,  (cardData->map), SPRITE_OFF );

		// Remove the tiles used by the card (if they were the only instances of those tiles.)
		removeUnusedRamtilesInTilemap( (cardData->map) );
		// clearAllUnusedReservedRamtiles();
	}
}
// (End of round) Displays the points for all active players.
static void showPoints(){
	// Show existing points for all players.
	u8 startX=7;
	u8 startY=7;

	N782_print    ( startX+0 , startY+0 , S_SCORES       , 0 );

	startY+=2;

	if(game.activePlayers[0]==1){
		N782_print    ( startX+0 , startY+0 , S_PLAYER       , 0 ); // Player 1
		N782_print_u8 ( startX+5 , startY+0 , 1                  ); // Player 1
		N782_print_u16( startX+9 , startY+0 , game.points_p1     ); // Player 1
	}

	if(game.activePlayers[1]==1){
		N782_print    ( startX+0 , startY+1 , S_PLAYER       , 0 ); // Player 2
		N782_print_u8 ( startX+5 , startY+1 , 2                  ); // Player 2
		N782_print_u16( startX+9 , startY+1 , game.points_p2     ); // Player 2
	}

	if(game.activePlayers[2]==1){
		N782_print    ( startX+0 , startY+2 , S_PLAYER       , 0 ); // Player 3
		N782_print_u8 ( startX+5 , startY+2 , 3                  ); // Player 3
		N782_print_u16( startX+9 , startY+2 , game.points_p3     ); // Player 3
	}

	if(game.activePlayers[3]==1){
		N782_print    ( startX+0 , startY+3 , S_PLAYER       , 0 ); // Player 4
		N782_print_u8 ( startX+5 , startY+3 , 4                  ); // Player 4
		N782_print_u16( startX+9 , startY+3 , game.points_p4     ); // Player 4
	}

}
// (End of round) Adds points to the winner's score. Shows animation show of the point tallys.
static void setPoints(u8 winner){
	// Tally up the points.

	// Points are now added. The remaining player's cards are added up and that score is added to the winning player's score.
		// "Number Card"    - Number value of the card is number of points.
		// "Draw Two"       - 20 Points
		// "Reverse"        - 20 Points
		// "Skip"           - 20 Points
		// "Wild"           - 50 Points
		// "Wild Draw Four" - 50 Points
	// If a player reaches 500 points then they win.

	// Fill(0, 0 , VRAM_TILES_H, VRAM_TILES_V, 1);
	Fill(6, 6 , 16, 16, 0);

	showPoints();
	WaitVsync(100);

	// Contains the tilemap (vram tiles) used by the selected card sprite. (3x2 tilemaps.)
	u8 map_2x3 [ 2 + (2*3) ]; // 2+6
	u8 map_3x2 [ 2 + (3*2) ]; // 2+6
	u8 map_3x4 [ 2 + (3*4) ]; // 2+12
	map_2x3[0]=2; map_2x3[1]=3;
	map_3x2[0]=3; map_3x2[1]=2;
	map_3x4[0]=3; map_3x4[1]=4;

	// Pointer to the tilemap arrayy
	u8 * map;
	// Dimensions for the selected card map.
	u8 w;
	u8 h;
	// s8 xdir;
	// s8 ydir;
	u8 dst_x;
	u8 dst_y;
	u8 tile_x;
	u8 tile_y;
	// u8 sprite_x ;
	// u8 sprite_y ;

	// Only the winner of the round will get points.
	// u8 location;
	u16 * points_slot;

	switch(winner){
		case 1  : {
			// location=CARD_LOCATION_PLAYER1;
			points_slot=&game.points_p1;
			dst_x=28-3;
			dst_y=28-3;
			break;
		}
		case 2  : {
			// location=CARD_LOCATION_PLAYER2;
			points_slot=&game.points_p2;
			dst_x=0;
			dst_y=28-3;
			break;
		}
		case 3  : {
			// location=CARD_LOCATION_PLAYER3;
			points_slot=&game.points_p3;
			dst_x=0;
			dst_y=0;
			break;
		}
		case 4  : {
			// location=CARD_LOCATION_PLAYER4;
			points_slot=&game.points_p4;
			dst_x=28-3;
			dst_y=0;
			break;
		}
		default : {
			return ;
			break;
		}
	}

	u8 pos=0;
	u8 cardsSent=0;
	u8 playerCardCount;
	// u8 player_cardPosition;
	u8 onLast5Cards=0;
	// u8 debugValue=0;
	u8 newPoints=0;

	// DEBUG
	returnCardsForPlayer(winner);
	removePlayerCards(winner);
	// DEBUG

	for(u8 player=1; player<4+1; player+=1){
		// Is this player an active player?
		if(game.activePlayers[player-1]==0){ continue; }

		// Resets.
		// clearAllRamtiles();
		clearAllReservedRamtiles();
		game.handRow=0;
		pos=0;
		cardsSent=0;
		updatePlayerDisplayedData();

		// Does this player have any cards?
		playerCardCount=countPlayerCards(player);
		if(playerCardCount==0) { continue; }

		// Is this player the winner?
		if(player==winner){ continue; }

		// Show this player's cards.
		displayCards_byPlayer(player, CARDS_FACEUP);

		// Go through the deck.
		for(u8 i=0; i<TOTALCARDSINDECK; i+=1){
			// CHECK FOR 5 CARD CYCLE RESTART.
			// Count the cards for this player.
			playerCardCount=countPlayerCards(player);
			// Have 5 cards already been sent?
			if(cardsSent==5){
				cardsSent=0;
				pos=0;

				// This is done earlier ?????
				displayCards_byPlayer(player, CARDS_FACEUP);
			}
			// Do no more cards remain for this player?
			if(playerCardCount == 0 ){
				displayCards_byPlayer(player, CARDS_FACEUP);
				break;
			}
			// Reset cardsSent and pos one more time if on the last 5 cards.
			if( (playerCardCount <5 && onLast5Cards==false) ){
				cardsSent=0;
				pos=0;
				onLast5Cards=true;
				displayCards_byPlayer(player, CARDS_FACEUP);
			}
			// CHECK FOR 5 CARD CYCLE RESTART.

			// Find the next card that is "owned" by the player.
			if(
				cards[i].location == player-1                 // Card held by the current player of the outer for loop.
			) {
				// Wilds: 50 points.
				if     (cards[i].color==CARD_BLACK)                             { newPoints=50; }
				// Number cards: Face value for points.
				else if(cards[i].value>=CARD_0 && cards[i].value<=CARD_9 )      { newPoints=cards[i].value; }
				// Action cards: 20 points.
				else if(cards[i].value>=CARD_DRAW2 && cards[i].value<=CARD_REV ){ newPoints=20; }

				// Set the map, w, h, tile_x, tile_y for the card that will move.
				switch(cards[i].location){
					case CARD_LOCATION_PLAYER1  : {
						map = map_2x3 ;
						w=map_2x3[0];h=map_2x3[1];
						tile_x=pgm_read_byte(&(p1_pos[pos][0]));
						tile_y=pgm_read_byte(&(p1_pos[pos][1]));
						break;
					}
					case CARD_LOCATION_PLAYER2  : {
						map = map_3x2 ;
						w=map_3x2[0];h=map_3x2[1];
						tile_x=pgm_read_byte(&(p2_pos[pos][0]));
						tile_y=pgm_read_byte(&(p2_pos[pos][1]));
						break;
					}
					case CARD_LOCATION_PLAYER3  : {
						map = map_2x3 ;
						w=map_2x3[0];h=map_2x3[1];
						tile_x=pgm_read_byte(&(p3_pos[pos][0]));
						tile_y=pgm_read_byte(&(p3_pos[pos][1]));
						break;
					}
					case CARD_LOCATION_PLAYER4  : {
						map = map_3x2 ;
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

				// Add the new points.
				*points_slot+=newPoints;

				// Update the display points.
				showPoints();

				// Erase the area where the card info will be displayed.
				Fill(6,14, 16, 4, 0x00);

				// Display the named color of the card.
				if(cards[i].color==CARD_YELLOW){ N782_print(7,14,PSTR("YELLOW"), 0); }
				if(cards[i].color==CARD_BLUE)  { N782_print(7,14,PSTR("BLUE"  ), 0); }
				if(cards[i].color==CARD_RED)   { N782_print(7,14,PSTR("RED"   ), 0); }
				if(cards[i].color==CARD_GREEN) { N782_print(7,14,PSTR("GREEN" ), 0); }

				// Display named value of the card
				if     (cards[i].value==CARD_0)          { N782_print(7,15,PSTR("ZERO" ), 0); }
				else if(cards[i].value==CARD_1)          { N782_print(7,15,PSTR("ONE"  ), 0); }
				else if(cards[i].value==CARD_2)          { N782_print(7,15,PSTR("TWO"  ), 0); }
				else if(cards[i].value==CARD_3)          { N782_print(7,15,PSTR("THREE"), 0); }
				else if(cards[i].value==CARD_4)          { N782_print(7,15,PSTR("FOUR" ), 0); }
				else if(cards[i].value==CARD_5)          { N782_print(7,15,PSTR("FIVE" ), 0); }
				else if(cards[i].value==CARD_6)          { N782_print(7,15,PSTR("SIX"  ), 0); }
				else if(cards[i].value==CARD_7)          { N782_print(7,15,PSTR("SEVEN"), 0); }
				else if(cards[i].value==CARD_8)          { N782_print(7,15,PSTR("EIGHT"), 0); }
				else if(cards[i].value==CARD_9)          { N782_print(7,15,PSTR("NINE" ), 0); }
				else if(cards[i].value==CARD_DRAW2)      { N782_print(7,15,PSTR("DRAW 2")    , 0); }
				else if(cards[i].value==CARD_SKIP)       { N782_print(7,15,PSTR("SKIP")      , 0); }
				else if(cards[i].value==CARD_REV)        { N782_print(7,15,PSTR("REVERSE")   , 0); }
				else if(cards[i].value==CARD_WILD)       { N782_print(7,15,PSTR("WILD")      , 0); }
				else if(cards[i].value==CARD_WILD_DRAW4) { N782_print(7,15,PSTR("W. DRAW 4") , 0); }

				// Display value of the card
				N782_print   (7,16,PSTR("POINTS:"), 0);
				N782_print_u8(7+7,16,newPoints);

				dst_x=13+5;
				dst_y=15;

				// Set the struct data.
				struct cardData_ cardData;
				cardData.player   = player         ;
				cardData.tile_x   = tile_x         ;
				cardData.tile_y   = tile_y         ;
				cardData.dst_x    = dst_x          ;
				cardData.dst_y    = dst_y          ;
				cardData.cardSize = SMALL_CARD     ;
				cardData.value    = cards[i].value ;
				cardData.color    = cards[i].color ;
				cardData.w        = w              ;
				cardData.h        = h              ;
				cardData.map      = map            ;
				cardData.fillTile = pgm_read_byte(&(offBlackTile[2])) ;

				// Move the card to the center then remove the card after a delay.
				moveCard(&cardData, 1, 1, ENDOFROUNDDEALBASESPEED);

				// Set the card location to the Draw Pile.
				cards[i].location=CARD_LOCATION_DRAW;

				// Update the displayed card counts.
				updatePlayerDisplayedData();

				// Draw the card.
				drawCard(
					255            ,
					dst_x          ,
					dst_y-1        ,
					LARGE_CARD     ,
					cards[i].value ,
					cards[i].color
				);

				// Get the tilemap for the new card.
				map = map_3x4 ;
				w=map_3x4[0];h=map_3x4[1];
				setVramTilemap( dst_x, dst_y-1 , w, h, (map) );

				// Increment cardsSent and pos
				cardsSent++;
				pos++;

				// Have 5 cards already been sent?
				if(cardsSent==5){
					// displayCards_byPlayer(player, CARDS_FACEUP);
				}

				// Wait.
				WaitVsync(15);

				// Clear the vram where the large card is.
				Fill(dst_x, dst_y-1 , w, h, 0x00);
				// Remove the tiles used by the large card (if they were the only instances of those tiles.)
				removeUnusedRamtilesInTilemap( (map) );
				// clearAllUnusedReservedRamtiles();
				// Erase the area where the card info will be displayed.
				Fill(6,14, 16, 4, 0x00);
			}

		}

		// Erase the area where the card info was be displayed.
		Fill(6,14, 16, 4, 0x00);

		// Update the displayed card counts.
		updatePlayerDisplayedData();

		// clearAllRamtiles();
		clearAllReservedRamtiles();
	}

}
// (UNFINISHED) Checks for UNO at the end of a play. Requires user to acknowledge the UNO or they are forced to draw a card.
static void unoCheck(){
	// Does the player now have UNO?
	if     (countPlayerCards(game.activePlayer)==1){
		// Make the user confirm their UNO within a short window of time.

		// If confirmed... good. End the check.

		// If unconfirmed within the window of time then the player must draw 1 card.
	}
}
// Handles end of round.
static void winRound(u8 winnerOfTheRound){
	// Win! End round, count points, start new round.
	game.activePlayer = winnerOfTheRound;

	// Update the displayed player data.
	updatePlayerDisplayedData();

	// Make sure that any remaining cards are displayed face down.
	displayCards_byPlayer(1, CARDS_FACEDOWN);
	displayCards_byPlayer(2, CARDS_FACEDOWN);
	displayCards_byPlayer(3, CARDS_FACEDOWN);
	displayCards_byPlayer(4, CARDS_FACEDOWN);

	game.gamestate2=GS_PLAYING_ROUNDEND;

	// Update the displayed color, direction, draw pile, discard pile and last played card on discard pile.
	redrawMainInfoRegion();

	msgHandler(ROUNDWIN_PLAYER);
	WaitVsync(ENDOFROUNDSTARTDELAY);

	// Clear all reserved ramtile data.
	clearAllReservedRamtiles();

	// Tally up the points and then show the points screen.
	// ClearVram();
	setPoints(game.activePlayer);
	showPoints();
	WaitVsync(ENDOFROUNDFINISHEDDELAY);

	// Did a player win the game by getting enough points?

	u8 gameIsWon=false;
	if     (game.points_p1 >=game.pointsForWin ){ gameIsWon=true; }
	else if(game.points_p2 >=game.pointsForWin ){ gameIsWon=true; }
	else if(game.points_p3 >=game.pointsForWin ){ gameIsWon=true; }
	else if(game.points_p4 >=game.pointsForWin ){ gameIsWon=true; }

	if(gameIsWon){
		// Win the game.

		// Present the game win message.
		// Uses the activePlayer, which should have been the last player to get points and would be the winner.
		msgHandler(GAMEWIN_PLAYER);

		if(DEBUG_AUTOSTARTNEXTGAME==1){
			// DEBUG
			game.points_p1=0;
			game.points_p2=0;
			game.points_p3=0;
			game.points_p4=0;
			game.gamestate1=GS_PLAYING;
			game.gamestate2=GS_PLAYING_GAMESTART;
			// DEBUG
		}
		else{
			getInputs();

			// Wait until the button is pressed.
			while ( game.btnHeld1==0 ){ getInputs(); }

			// Wait until the button is released.
			while ( game.btnHeld1 )   { getInputs(); }

			// Return to the title screens.
			game.gamestate1=GS_TITLE;
			game.gamestate2=GS_TITLE_MAINMENU;
		}
	}
	else{
		// Start a new round.
		game.gamestate1=GS_PLAYING;
		game.gamestate2=GS_PLAYING_GAMESTART;
	}

	// SoftReset();
}
// Determine what the next player number will be or specifically set it.
u8 nextPlayerNumber(u8 playerNum, enum NPN_nextPlayerNumber options){
	// This function will set the next value for game.activePlayer.

	// Allows you to directly specify the next player (ignoring order.)
	if(playerNum!=255 && options==NPN_SET){
		game.activePlayers[game.activePlayer-1]==playerNum-1;
		return 255;
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
// Pauses the game and offers some options such as restarts and auto-play
u8 pause(){
	clearbgmessage();
	// Ask the user to chose the new color.
	// Use the message area rows and the draw/discard rows.
	// The erased rows will be drawn after the user makes their selection.

	const u8 replacementTile = pgm_read_byte(&(wood_color2 [2]));
	// const u8 YELLOW_VALUE    = pgm_read_byte(&(yellow_color[2]));
	// const u8 BLUE_VALUE      = pgm_read_byte(&(blue_color  [2]));
	// const u8 RED_VALUE       = pgm_read_byte(&(red_color   [2]));
	// const u8 GREEN_VALUE     = pgm_read_byte(&(green_color [2]));
	// const u8 BLACK_VALUE     = 0x01;

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
	u8 * srcVram_ptr ;
	u8 * dstVram_ptr ;
	// u16 * srcVram_ptr ;
	// u16 * dstVram_ptr ;

	// Remove the discard card.
	removeCard_lg();

	// Draw the top and the bottom row first.
	Fill(startx,starty  ,rowWidth,1, BLACK_VALUE);
	Fill(startx,starty+1,rowWidth,1, BLACK_VALUE);
	N782_print(startx+2, starty  , S_PAUSEMENU, 0);
	N782_print(startx+0, starty+1, S_BCANCEL_ASET, 0);

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
		Fill(startx,y,rowWidth,1, BLACK_VALUE);

		// Increment y.
		y++;
	}

	// Draw the options.
	N782_print( pgm_read_byte(&(pause_pos_cursor[3][0]))+1, pgm_read_byte(&(pause_pos_cursor[2][1]))-2, S_RESETROUND, 0 );
	N782_print( pgm_read_byte(&(pause_pos_cursor[3][0]))+1, pgm_read_byte(&(pause_pos_cursor[2][1]))-1, S_EXITGAME, 0 );
	if(game.autoPlay==1){
		N782_print( pgm_read_byte(&(pause_pos_cursor[3][0]))+1, pgm_read_byte(&(pause_pos_cursor[2][1]))+0, S_NORMALPLAY, 0 );
	}
	else{
		N782_print( pgm_read_byte(&(pause_pos_cursor[3][0]))+1, pgm_read_byte(&(pause_pos_cursor[2][1]))+0, S_AUTOPLAY, 0 );
	}
	N782_print( pgm_read_byte(&(pause_pos_cursor[3][0]))+1, pgm_read_byte(&(pause_pos_cursor[2][1]))+1, S_CANCEL, 0 );


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
			// game.gamestate1=GS_PLAYING;
			// game.gamestate2=GS_PLAYING_PLAYING;
			// game.gamestate2=GS_PLAYING_GAMESTART;

			switch(cursorIndex){
				// RESET ROUND.
				case 0 : {
					game.gamestate1=GS_PLAYING;
					game.gamestate2=GS_PLAYING_GAMESTART;
					break;
				}
				// EXIT GAME.
				case 1 : {
					game.gamestate1=GS_TITLE;
					game.gamestate2=GS_TITLE_MAINMENU;
					break;
				}
				// AUTO PLAY.
				case 2 : {
					if(game.autoPlay==1){
						removePlayerCards(game.activePlayer);
						displayCards_byPlayer(game.activePlayer, CARDS_FACEUP);
						game.autoPlay=0;
					}
					else                {
						removePlayerCards(game.activePlayer);
						displayCards_byPlayer(game.activePlayer, CARDS_FACEDOWN);
						game.autoPlay=1;
					}

					break;
				}
				// CANCEL
				case 3 : {
					// game.gamestate1=GS_PLAYING;
					// game.gamestate2=GS_PLAYING_GAMESTART;
					break;
				}
				default: { break; }
			};

			// Hide the sprite.
			MapSprite2( 0, cursor1map, SPRITE_OFF );
			WaitVsync(1);
			SetSpriteVisibility(false);

			// Black out the title bars.
			Fill(startx,starty ,rowWidth,1, BLACK_VALUE);
			Fill(startx,y      ,rowWidth,1, BLACK_VALUE);

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
// Finds the first card in the player's hand that matches the specified value.
u8 getMatchingCardValue(u8 playerNum, u8 value){
	u8 location = playerNum-1;

	for(u8 i=0; i<TOTALCARDSINDECK; i+=1){
		if( cards[i].location==location && cards[i].value==value) { return i; }
	}

	return 255;
}
// Finds the first card in the player's hand that matches the specified color.
u8 getMatchingCardColor(u8 playerNum, u8 color){
	u8 location = playerNum-1;

	for(u8 i=0; i<TOTALCARDSINDECK; i+=1){
		if( cards[i].location==location && cards[i].color==color) { return i; }
	}

	return 255;
}
u8 canCardBePlayed(u8 cardIndex){
	// cards[ game.playerVisibleHand[cursorIndex] ]
	// u8 color = cards[ cardIndex ].color;
	// u8 value = cards[ cardIndex ].value;
	// u8 hasColorMatchInHand=getMatchingCardColor(game.activePlayer, cards[game.lastCardPlayed].color);

	// Wild color?
	if     (cards[ cardIndex ].value == CARD_WILD){ return true; }

	// Wild Draw Four value?
	else if(cards[ cardIndex ].value == CARD_WILD_DRAW4){
		// Only allow this card to be played if the player does not already have a matching color in their hand as the discard color.

		// Does the player have a color match against the discard color?
		if(getMatchingCardColor(game.activePlayer, cards[game.lastCardPlayed].color) != 255 ? true : false){
			// Card CAN be played.
			return false;
		}
		// No match to the discard color.
		else{
			// Card CANNOT be played.
			return true;
		}
	}

	// Matching color to the last played card?
	else if(cards[ cardIndex ].color == cards[game.lastCardPlayed].color){ return true; }

	// Matching value to the last played card?
	else if(cards[ cardIndex ].value == cards[game.lastCardPlayed].value){ return true; }

	else {
		return false;
	}
}
// ===== GS_PLAYING UTILITIES =====

static void gstate_playing(){
	ClearVram();
	game.gamestate1=GS_PLAYING;

	// Replacement tile used when hiding a card in the player's hand.
	const u8 replacementTile = pgm_read_byte(&(offBlackTile[2])) ;

	// Cursor values.
	u8 * cursorTimer = &vsynccounter8b_gen1;
	*cursorTimer=0;

	u8 * msgTimer = &vsynccounter8b_gen2;
	*msgTimer=0;
	u8 msgDisplayed=0;

	u8 currentCursor = 0;
	const u8 *cursor1map; cursor1map=cursor2_f1_d;
	const u8 *cursor2map; cursor2map=cursor2_f2_d;
	u8 cursorIndex;
	u8 cursor_x;
	u8 cursor_y;

	// Player hand.
	u8 cardSelected=0;

	// Card sprite.
	// Contains the tilemap (vram tiles) used by the selected card sprite. (2x3 tilemaps.)
	u8 map1_2x3 [ 2 + (2*3) ];
	map1_2x3[0]=2; map1_2x3[1]=3;

	// Contains the tilemap (vram tiles) used by the selected card sprite. (3x2 tilemaps.)
	u8 map2_3x2 [ 2 + (3*2) ];
	map2_3x2[0]=3; map2_3x2[1]=2;

	// Pointer to the tilemap array
	u8 * map ;

	// Dimensions for the selected card map.
	u8 w;
	u8 h;

	//
	// u8 counter;
	u8 tile_x;
	u8 tile_y;

	// u8 sprite_x;
	// u8 sprite_y;

	// s8 xdir;
	// s8 ydir;

	// Discard Pile.
	// u8 discard_x = pgm_read_byte(&(discard_pos[0])) << 3 ; // sprite values.
	// u8 discard_y = pgm_read_byte(&(discard_pos[1])) << 3 ; // sprite values.

	// End of turn flags.
	u8 endOfTurnFlag = 0;
	u8 playerDraws2  = 0;
	u8 playerSkipped = 0;
	u8 playerDraws4  = 0;
	u8 playerReverse = 0;
	u8 initialIteration=1;
	u8 thisPlayer;
	// u8 maxHandRows;
	// u8 playerCards;
	u8 winnerOfTheRound=255;
	u8 retval;
	u8 cpu_card1=255;
	u8 cpu_card2=255;
	u8 cpu_card3=255;
	u8 cpu_card4=255;
	u8 cpu_played=0;
	u8 randPos;

	struct cardData_ cardData;

	while(game.gamestate1==GS_PLAYING){
		// Resets the game for a new round. (Does NOT impact scores.)
		if     (game.gamestate2==GS_PLAYING_GAMESTART){
			newGameSetup();
			redrawMainInfoRegion();

			// Reset the flags that are checked at the beginning of each hand.
			initialIteration = 1;
			endOfTurnFlag    = 0;
			playerDraws2     = 0;
			playerSkipped    = 0;
			playerDraws4     = 0;
			playerReverse    = 0;

			// Set the winner value to the invalid value.
			winnerOfTheRound = 255;
			// winnerOfTheRound = 1;

			// Change the gamestate2.
			game.gamestate2=GS_PLAYING_PLAYING;
			continue;
		}

		// Game in progress.
		else if(game.gamestate2==GS_PLAYING_PLAYING          ){
			// // Update the displayed color, direction, draw pile, discard pile and last played card on discard pile.
			// redrawMainInfoRegion();

			// // Update the displayed player data.
			// updatePlayerDisplayedData();

			// Display all cards face-down.
			if(game.activePlayers[0]==1){ displayCards_byPlayer(1, CARDS_FACEDOWN); }
			if(game.activePlayers[1]==1){ displayCards_byPlayer(2, CARDS_FACEDOWN); }
			if(game.activePlayers[2]==1){ displayCards_byPlayer(3, CARDS_FACEDOWN); }
			if(game.activePlayers[3]==1){ displayCards_byPlayer(4, CARDS_FACEDOWN); }

			// Main game loop. The program stays within this loop until the round is over or the gamestate2 changes.
			while(game.gamestate2==GS_PLAYING_PLAYING){
				clearAllUnusedReservedRamtiles();

				// DEBUG
				game.handsPlayed++;
				// DEBUG

				// Keep in sync.
				WaitVsync(1);

				// Clear the endOfTurnFlag.
				endOfTurnFlag=0;

				// Used where the player number needs to change but the old value is still required.
				thisPlayer=game.activePlayer;

				// Hide the cursor.
				MapSprite2(0, cursor1map, SPRITE_OFF );

				// Update the displayed color, direction, draw pile, discard pile and last played card on discard pile.
				redrawMainInfoRegion();

				// Update the displayed player data.
				updatePlayerDisplayedData();

				// Handle initial iteration flags. (Initial discard for when the round is started.)
				if(initialIteration==1){
					// Show the player's hand?
					if(game.player_types[thisPlayer-1]!=HUMAN){
						// Display the cards face-down.
						displayCards_byPlayer(thisPlayer, CARDS_FACEDOWN);
					}
					else{
						// Flip up this player's cards. (Uses ramtiles.)
						displayCards_byPlayer(thisPlayer, CARDS_FACEUP);
					}

					if     (cards[game.lastCardPlayed].color==CARD_BLACK     ) {
						if(game.player_types[thisPlayer-1]!=HUMAN){ askUserToSetNewColor(SNC_CPUAUTOCHOICE); }
						else                                      { askUserToSetNewColor(SNC_PLAYERCHOICE);  }
					}
					else if(cards[game.lastCardPlayed].value==CARD_DRAW2     ) { playerDraws2 =1; }
					else if(cards[game.lastCardPlayed].value==CARD_SKIP      ) { playerSkipped=1; }
					else if(cards[game.lastCardPlayed].value==CARD_REV       ) { playerReverse=1; }
					else if(cards[game.lastCardPlayed].value==CARD_WILD_DRAW4) { playerDraws4 =1; }
				}

				// Were some flags set at the end of the last turn?
				if(playerDraws2 || playerSkipped || playerDraws4 || playerReverse){
					// A Draw 2 card was played.
					if     ( playerDraws2  ){
						// Display the Draw 2 message.
						msgHandler(DRAW2_PLAYER);
						// Set the end of turn flag.
						endOfTurnFlag=1;
						// Draw 2 cards.
						for(u8 i=0; i<2; i+=1){
							// Pick a card position where a card should NOT already be.
							// Or just default to the last position.
							countColorsValuesMatches(game.activePlayer);
							if(counts.numCardsInHand<5){ randPos = counts.numCardsInHand; }
							else                       { randPos = 4; }

							// Non-human cards are dealt face-down.
							if(game.player_types[thisPlayer-1]!=HUMAN){
								dealSpecifiedCard_anim(game.activePlayer, 255, randPos, CARDDRAWBASESPEED, CARDS_FACEDOWN);
							}
							// Human cards are dealt face-up.
							else{
								dealSpecifiedCard_anim(game.activePlayer, 255, randPos, CARDDRAWBASESPEED, CARDS_FACEUP);
							}
							// Play the sound effect.
							playSFX(DRAWCARD);
						}
					}
					// A Skip card was played.
					else if( playerSkipped ){
						// Display the Skip message.
						msgHandler(SKIP_PLAYER);
						// Set the end of turn flag.
						endOfTurnFlag=1;
					}
					// A Wild Draw 4 card was played.
					else if( playerDraws4  ){
						// Display the Draw 4 message.
						msgHandler(DRAW4_PLAYER);
						// Set the end of turn flag.
						endOfTurnFlag=1;
						// Draw 4
						for(u8 i=0; i<4; i+=1){
							countColorsValuesMatches(game.activePlayer);
							if(counts.numCardsInHand<5){ randPos = counts.numCardsInHand; }
							else                       { randPos = 4; }

							// Non-human cards are dealt face-down.
							if(game.player_types[thisPlayer-1]!=HUMAN){
								dealSpecifiedCard_anim(game.activePlayer, 255, randPos, CARDDRAWBASESPEED, CARDS_FACEDOWN);
							}
							// Human cards are dealt face-up.
							else{
								dealSpecifiedCard_anim(game.activePlayer, 255, randPos, CARDDRAWBASESPEED, CARDS_FACEUP);
							}
							// Play the sound effect.
							playSFX(DRAWCARD);
						}
					}
					else if( playerReverse  ){
						// Change the direction.
						game.direction = !game.direction;

						// Handle the initial deal differently.
						if( initialIteration==1){
							if(game.numPlayers == 2){
								// DON'T end this turn.
								endOfTurnFlag = 0 ;
								// Reset the message timer.
								*msgTimer     = 0 ;
								// Indicate that a message is displayed (it will clear later after the timer finishes.)
								msgDisplayed  = 1 ;
							}
							else{
								// Skip to the next player.
								nextPlayerNumber(255, NPN_NEXT);
								// Set the end of turn flag.
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

						// Display the Reverse message.
						msgHandler(REVERSE_PLAYER);
					}
				}

				// Reset those flags.
				initialIteration = 0;
				playerDraws2     = 0;
				playerSkipped    = 0;
				playerDraws4     = 0;
				playerReverse    = 0;

				// Get new counts so that we can make a decision on what card to play. (Updates the counts struct.)
				countColorsValuesMatches(game.activePlayer);

				// Reset the current handRow flag.
				game.handRow=0;

				// Update the displayed player data.
				updatePlayerDisplayedData();

				// Update the displayed color, direction, draw pile, discard pile and last played card on discard pile.
				redrawMainInfoRegion();

				// Do we have a winner??
				if(winnerOfTheRound!=255){
					// All cards should already be flipped face-down.
					winRound(winnerOfTheRound);
					// Don't complete the rest of this iteration.
					continue;
				}

				// If the end of turn flag is set then end this iteration. (Also, set the value for next player.)
				if( endOfTurnFlag==1 ) {
					WaitVsync(ENDOFTURNDELAY);

					// Flip down this player's cards. (Clear ramtile usages.)
					displayCards_byPlayer(thisPlayer, CARDS_FACEDOWN);

					clearbgmessage();
					nextPlayerNumber(255, NPN_NEXT);
					clearbgmessage();
					break;
				}

				// Setup and Display the cursor for this player.
				SetSpriteVisibility(false);
				cursorIndex   = 0;
				currentCursor = 0;
				*cursorTimer  = 0;
				getCursorPos(game.activePlayer, cursorIndex, &cursor_x, &cursor_y);
				if     (game.activePlayer==1){
					cursor1map=cursor2_f1_d;     // Cursor tilemap used.
					cursor2map=cursor2_f2_d;     // Cursor tilemap used.
					map = map1_2x3;             // vram tilemap for the card sprites.
					w=map1_2x3[0];h=map1_2x3[1]; // Dimensions for the cards sprites.
					// w=2; h=3;
				}
				else if(game.activePlayer==2){
					cursor1map=cursor2_f1_l;
					cursor2map=cursor2_f2_l;
					map = map2_3x2;
					w=map2_3x2[0];h=map2_3x2[1];
					// w=3; h=2;
				}
				else if(game.activePlayer==3){
					cursor1map=cursor2_f1_u;
					cursor2map=cursor2_f2_u;
					map = map1_2x3;
					w=map1_2x3[0];h=map1_2x3[1];
					// w=2; h=3;
				}
				else if(game.activePlayer==4){
					cursor1map=cursor2_f1_r;
					cursor2map=cursor2_f2_r;
					map = map2_3x2;
					w=map2_3x2[0];h=map2_3x2[1];
					// w=3; h=2;
				}
				SetSpriteVisibility(true);

				if      (game.player_types[game.activePlayer-1]==HUMAN || game.autoPlay==1){
					MapSprite2(0, cursor1map, SPRITE_BANK0 );
					MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);

					// Flip up this player's cards. (Uses ramtiles.)
					displayCards_byPlayer(thisPlayer, CARDS_FACEUP);
					// displayCards_byPlayer(thisPlayer, CARDS_FACEDOWN);
				}
				else if(game.player_types[thisPlayer-1]==CPU){
					// Display the cards face-down.
					// displayCards_byPlayer(thisPlayer, CARDS_FACEUP);
					displayCards_byPlayer(thisPlayer, CARDS_FACEDOWN);
				}

					/*
					// Show the player's hand?
					if(game.player_types[thisPlayer-1]!=HUMAN){
						// Display the cards face-down.
						displayCards_byPlayer(thisPlayer, CARDS_FACEDOWN);
					}
					else{
						// Flip up this player's cards. (Uses ramtiles.)
						displayCards_byPlayer(thisPlayer, CARDS_FACEUP);
					}
					*/

				// Loop until the player's turn ends.
				while(endOfTurnFlag==0){
					// Get gamepad inputs. (All human players can be controlled via either gamepad.)
					getInputs();

					// Pause?
					if( (game.btnHeld1 & BTN_START)) {
						// Run the Pause function and receive a value back.
						retval=pause();
						// Clear and redraw the area where the Pause menu was drawn.
						redrawMainInfoRegion();

						// Check the Pause function return value.

						// Cancel.
						if     ( retval == 0 ){ }
						// Restart round or exit game.
						else if( retval == 1 ){ break; }
						// Auto play.
						else if( retval == 2 ){
							continue;
						}
					}

					// Do a soft-reset if both L and R shoulder buttons are pressed.
					if( (game.btnHeld1 & BTN_SR) && (game.btnHeld1 & BTN_SL) ) { SoftReset(); }

					// Is the player a CPU?
					if     (game.player_types[game.activePlayer-1]==CPU || game.autoPlay==1){
						randPos = (getRandomNumber(BASECPU_THINKTIME, BASECPU_THINKTIME+EXTRACPU_THINKTIME)/10)*10 ;
						// // DEBUG
						// N782_print_u8(28, 27, randPos);
						// // DEBUG
						WaitVsync( randPos );

						// Get a card from the hand with a matching color.
						cpu_card1=getMatchingCardColor(game.activePlayer, cards[game.lastCardPlayed].color);
						// Get a card from the hand with a matching value.
						cpu_card2=getMatchingCardValue(game.activePlayer, cards[game.lastCardPlayed].value);
						// Get a card from the hand that is the Wild card.
						cpu_card3=getMatchingCardValue(game.activePlayer, CARD_WILD);
						// Get a card from the hand that is the Wild Draw 4 card.
						cpu_card4=getMatchingCardValue(game.activePlayer, CARD_WILD_DRAW4);
						// Reset the cpu_played flag.
						cpu_played=0;

						// If a matching card color is found in the hand then play the first matching card.
						if     (cpu_card1!=255){ game.lastCardPlayed=cpu_card1; cpu_played=1; }
						// No matching color? Try to match the value. If a match exists then use the first found match.
						else if(cpu_card2!=255){ game.lastCardPlayed=cpu_card2; cpu_played=1; }
						// Is there a Wild card available?
						else if(cpu_card3!=255){ game.lastCardPlayed=cpu_card3; cpu_played=1; }
						// Last playable option is the Wild Draw 4 card. Does it exist?
						else if(cpu_card4!=255){ game.lastCardPlayed=cpu_card4; cpu_played=1; }
						// No plays available. Pass turn.
						else                   { cpu_played=0; }

						// Card was played. Set flags if applicable and end the turn.
						if( cpu_played==1 ) {

							playSFX(SELECTCARD);

							// Hide the cursor sprite.
							MapSprite2( 0, cursor1map, SPRITE_OFF );

							// Pick a card position where a card should already be.
							randPos = getRandomNumber(0, counts.numCardsInHand >= 5 ? 4 : counts.numCardsInHand);

							// Set the map, w, h, tile_x, tile_y for the card that will move.
							switch(game.activePlayer){
								case 1  : {
									map = map1_2x3 ;
									w=map1_2x3[0];h=map1_2x3[1];
									// w=2; h=3;
									tile_x=pgm_read_byte(&(p1_pos[randPos][0]));
									tile_y=pgm_read_byte(&(p1_pos[randPos][1]));
									break;
								}
								case 2  : {
									map = map2_3x2 ;
									w=map2_3x2[0];h=map2_3x2[1];
									// w=3; h=2;
									tile_x=pgm_read_byte(&(p2_pos[randPos][0]));
									tile_y=pgm_read_byte(&(p2_pos[randPos][1]));
									break;
								}
								case 3  : {
									map = map1_2x3 ;
									w=map1_2x3[0];h=map1_2x3[1];
									// w=2; h=3;
									tile_x=pgm_read_byte(&(p3_pos[randPos][0]));
									tile_y=pgm_read_byte(&(p3_pos[randPos][1]));
									break;
								}
								case 4  : {
									map = map2_3x2 ;
									w=map2_3x2[0];h=map2_3x2[1];
									// w=3; h=2;
									tile_x=pgm_read_byte(&(p4_pos[randPos][0]));
									tile_y=pgm_read_byte(&(p4_pos[randPos][1]));

									break;
								}
								default : {
									// return ;
									break;
								}
							}

							// Set the struct data.
							struct cardData_ cardData;
							cardData.player   = game.activePlayer     ;
							cardData.tile_x   = tile_x         ;
							cardData.tile_y   = tile_y         ;
							cardData.dst_x    = pgm_read_byte(&(discard_pos[0])) ;
							cardData.dst_y    = pgm_read_byte(&(discard_pos[1])) ;
							cardData.cardSize = SMALL_CARD     ;
							cardData.value    = cards[game.lastCardPlayed].value ;
							cardData.color    = cards[game.lastCardPlayed].color ;
							cardData.w        = w              ;
							cardData.h        = h              ;
							cardData.map      = map            ;
							cardData.fillTile = pgm_read_byte(&(offBlackTile[2])) ;

							// Remove the card that is in the position we are playing from.
							removeCard_sm(game.activePlayer, randPos);

							// WaitVsync(50);

							// Flip card hand over.
							displayCards_byPlayer(game.activePlayer, CARDS_FACEDOWN);

							// Set the played card to be part of the Discard Pile.
							cards[game.lastCardPlayed].location = CARD_LOCATION_DISCARD;

							// WaitVsync(50);
							// Move the card to the discard pile and then remove the card after a delay.
							moveCard(&cardData, 1, 1, CARDPLAYBASESPEED);
							// WaitVsync(50);

							// Remove the discard card.
							removeCard_lg();

							// Set action-card flags for the next turn.
							if     (cards[game.lastCardPlayed].value==CARD_WILD_DRAW4){
								playerDraws4=1;
								askUserToSetNewColor(SNC_CPUAUTOCHOICE);
							}
							else if(cards[game.lastCardPlayed].value==CARD_WILD)  { askUserToSetNewColor(SNC_CPUAUTOCHOICE); }
							else if(cards[game.lastCardPlayed].value==CARD_DRAW2) { playerDraws2=1;  }
							else if(cards[game.lastCardPlayed].value==CARD_SKIP)  { playerSkipped=1; }
							else if(cards[game.lastCardPlayed].value==CARD_REV)   { playerReverse=1; }
						}

						// Card was NOT played. End turn and draw a card.
						else{
							playSFX(DRAWCARD);

							// Pick a card position where a card should NOT already be.
							// Or just default to the last position.
							if(counts.numCardsInHand<5){ randPos = counts.numCardsInHand; }
							else                       { randPos = 4; }

							// Flip card hand over.
							displayCards_byPlayer(game.activePlayer, CARDS_FACEDOWN);

							dealSpecifiedCard_anim(game.activePlayer, 255, randPos, CARDDRAWBASESPEED, CARDS_FACEDOWN);
						}

						// Do this at the end of the card choose phase.
						updatePlayerDisplayedData();
						redrawMainInfoRegion();
						endOfTurnFlag=1;

						// Flip card hand over.
						displayCards_byPlayer(game.activePlayer, CARDS_FACEDOWN);

						WaitVsync(CPU_AFTERPLAYDELAY);

						break;
					}

					// Is the player a HUMAN?
					else if(game.player_types[game.activePlayer-1]==HUMAN){
						// CARD NOT YET SELECTED: Allow cursor movement and for the user to choose a card or pass their turn.
						if(!cardSelected){
							_emu_whisper(0,game.playerVisibleHand[cursorIndex]);
							_emu_whisper(1,cursorIndex);

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
									displayCards_byPlayer(game.activePlayer, CARDS_FACEUP);
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
									displayCards_byPlayer(game.activePlayer, CARDS_FACEUP);
								}
							}

							// Select the card.
							if     ( game.btnHeld1 & (BTN_A   )  ){
								// Do not allow the card to be selected if it is invalid.
								if(game.playerVisibleHand[cursorIndex]==255) { continue; }

#if DEBUG_IGNORE_CARD_CHECK==1
								msgDisplayed=0;
#else
								// Do not allow the card to be selected if it is invalid.
								if( ! canCardBePlayed(game.playerVisibleHand[cursorIndex]) ){
									// Change the color if the card was a different color that the discard but had a matching value.
									msgHandler(INCORRECTCARD);
									msgDisplayed=1;
									*msgTimer=0;
									continue;
								}
								else {
									_emu_whisper(0,0);
									_emu_whisper(1,0);
									msgDisplayed=0;
								}
#endif

								// Set the cardSelected flag. (Disallows further cursor movements.)
								playSFX(SELECTCARD);
								cardSelected=true;

								// Hide the cursor.
								MapSprite2( 0, cursor1map, SPRITE_OFF);

								// Get new x and y.
								get_cardDims_byPlayerAndCardPosition(game.activePlayer, cursorIndex, &tile_x, &tile_y);

								// Clear the card in vram.
								Fill(tile_x,tile_y, w, h, replacementTile);

								// Use the new card moving function.

								// Get the new dst_x and dst_y.
								switch(game.activePlayer){
									case 1  : {
										cardData.dst_x=tile_x;
										cardData.dst_y=tile_y-5;
										break;
									}
									case 2  : {
										cardData.dst_x=tile_x+5;
										cardData.dst_y=tile_y;
										break;
									}
									case 3  : {
										cardData.dst_x=tile_x+0;
										cardData.dst_y=tile_y+5;
										break;
									}
									case 4  : {
										cardData.dst_x=tile_x-5;
										cardData.dst_y=tile_y;
										break;
									}
									default : {
										// return ;
										break;
									}
								}

								// Set the struct data.
								cardData.player   = game.activePlayer     ;
								cardData.tile_x   = tile_x         ;
								cardData.tile_y   = tile_y         ;
								// cardData.dst_x    = cardData.dst_x; // pgm_read_byte(&(discard_pos[0])) ;
								// cardData.dst_y    = cardData.dst_y; // pgm_read_byte(&(discard_pos[1])) ;
								cardData.cardSize = SMALL_CARD     ;
								cardData.value    = cards[ game.playerVisibleHand[cursorIndex] ].value ;
								cardData.color    = cards[ game.playerVisibleHand[cursorIndex] ].color ;
								cardData.w        = w              ;
								cardData.h        = h              ;
								cardData.map      = map            ;
								// cardData.fillTile = pgm_read_byte(&(wood_color2[2])) ;
								cardData.fillTile = pgm_read_byte(&(offBlackTile[2])) ;

								// Move the card to the discard pile and then remove the card after a delay.
								moveCard(&cardData, 0, 0, CARDPLAYBASESPEED);

								clearbgmessage();

								msgHandler(PLAYORCANCEL);
							}

							// Give up turn.
							else if( game.btnPressed1 & (BTN_B   )  ){
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
									if( game.btnPressed1 & (BTN_A ) ){
										// Clear the message.
										clearbgmessage();

										// If this is a CPU player or UZENET player then don't show the new card.
										// Show the card for HUMAN players only.

										if(game.player_types[game.activePlayer-1]!=HUMAN){
											dealSpecifiedCard_anim(game.activePlayer, 255, 2, CARDDRAWBASESPEED, CARDS_FACEDOWN);
										}
										else{
											dealSpecifiedCard_anim(game.activePlayer, 255, 2, CARDDRAWBASESPEED, CARDS_FACEUP);
										}

										playSFX(DRAWCARD);

										// Set the endOfTurnFlag.
										endOfTurnFlag=1;

										// Wait until the button is released.
										while ( game.btnHeld1 ){ getInputs(); }

										break;
									}

									// Cancel - pass.
									if( game.btnPressed1 & (BTN_B ) ){
										// Wait until the button is released.
										while ( game.btnHeld1 ){ getInputs(); }
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
							else if( game.btnPressed1 ){
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

								// Clear the confirmation message from the lower board region.
								clearbgmessage();

								// Set the struct data.
								// struct cardData_ cardData;
								cardData.player   = game.activePlayer     ;
								cardData.tile_x   = cardData.dst_x        ;
								cardData.tile_y   = cardData.dst_y        ;
								cardData.dst_x    = pgm_read_byte(&(discard_pos[0])); // tile_x;
								cardData.dst_y    = pgm_read_byte(&(discard_pos[1])); // tile_y;
								cardData.cardSize = SMALL_CARD     ;
								cardData.value    = cards[ game.playerVisibleHand[cursorIndex] ].value ;
								cardData.color    = cards[ game.playerVisibleHand[cursorIndex] ].color ;
								cardData.w        = w              ;
								cardData.h        = h              ;
								cardData.map      = map            ;
								cardData.fillTile = pgm_read_byte(&(wood_color2[2])) ;
								// cardData.fillTile = pgm_read_byte(&(offBlackTile[2])) ;

								// Draw and move the card to the discard pile and then remove the card after a delay.
								moveCard(&cardData, 0, 0, CARDPLAYBASESPEED);

								// ClearVram();

								// Hide the card spritemap.
								MapSprite2_nra( 0,  map, SPRITE_OFF );

								// Use the spritemap to draw the card tiles back into vram.
								// DrawMap_vramtiles( tile_x, tile_y, map );

								// Remove the tiles used by the existing card (if they were the only instances of those tiles.)
								removeUnusedRamtilesInTilemap( map );
								// clearAllUnusedReservedRamtiles();

								// Clear the cardSelected flag. (Allows cursor movements.)
								cardSelected=false;

								// Unhide the cursor.
								MapSprite2(0, cursor1map, SPRITE_BANK0 );
								MoveSprite(0, cursor_x<<3, cursor_y<<3, 1, 1);

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
								if(cards[game.lastCardPlayed].color==CARD_BLACK)     { askUserToSetNewColor(SNC_PLAYERCHOICE); }

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
								// Remove the tiles used by the existing card (if they were the only instances of those tiles.)
								// removeUnusedRamtilesInTilemap( map );
								// clearAllUnusedReservedRamtiles();

								// Clear the card in vram.
								Fill(cardData.dst_x,cardData.dst_y, w, h, pgm_read_byte(&(wood_color1[2])) );

								// ClearVram();

								// Clear the confirmation message from the lower board region.
								clearbgmessage();

								playSFX(CANCELCARD);

								// Set the struct data.
								// struct cardData_ cardData;
								cardData.player   = game.activePlayer     ;
								cardData.tile_x   = cardData.dst_x        ;
								cardData.tile_y   = cardData.dst_y        ;
								cardData.dst_x    = tile_x;
								cardData.dst_y    = tile_y;
								cardData.cardSize = SMALL_CARD     ;
								cardData.value    = cards[ game.playerVisibleHand[cursorIndex] ].value ;
								cardData.color    = cards[ game.playerVisibleHand[cursorIndex] ].color ;
								cardData.w        = w              ;
								cardData.h        = h              ;
								cardData.map      = map            ;
								cardData.fillTile = pgm_read_byte(&(wood_color2[2])) ;
								// cardData.fillTile = pgm_read_byte(&(offBlackTile[2])) ;

								// Draw and move the card to the discard pile and then remove the card after a delay.
								moveCard(&cardData, 0, 0, CARDPLAYBASESPEED);

								// Hide the card spritemap.
								MapSprite2_nra( 0,  map, SPRITE_OFF );

								// Use the spritemap to draw the card tiles back into vram.
								DrawMap_vramtiles( tile_x, tile_y, map );

								// Remove the tiles used by the existing card (if they were the only instances of those tiles.)
								removeUnusedRamtilesInTilemap( map );
								// clearAllUnusedReservedRamtiles();

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
						if(*msgTimer > MSGTIMEDELAY && msgDisplayed){
							// Clear the message area.
							clearbgmessage();

							// Reset the message displayed flag.
							msgDisplayed=0;

							// Reset the counter.
							*msgTimer=0;
						}
					}
				}

				// End of turn.

				// Does the player now have UNO?
				unoCheck();

				// Does the player now have zero cards?
				if(countPlayerCards(game.activePlayer)==0){
					// Set the value for the winner of this round.
					// It will be checked after any action cards have run their course.
					winnerOfTheRound=game.activePlayer;
				}

				// Flip down this player's cards. (Clear ramtile usages.)
				displayCards_byPlayer(game.activePlayer, CARDS_FACEDOWN);

				// Find and set the next active player number.
				nextPlayerNumber(255, NPN_NEXT);

				// Keep in sync.
				WaitVsync(1);
			}
		}
	}
}

// ===== MAIN =====
void main(){
	// Initialize the game.
	gameInit();

	// Main loop.
	while(1){
		// Run the correct function based on what the current game state is.
		// Go to that function and remain there until the gamestate1 changes.

		// switch(game.gamestate1){
		// 	case GS_TITLE   : { gstate_title()   ; break; }
		// 	case GS_OPTIONS : { gstate_options() ; break; }
		// 	case GS_RULES   : { gstate_rules()   ; break; }
		// 	case GS_CREDITS : { gstate_credits() ; break; }
		// 	case GS_PLAYING : { gstate_playing() ; break; }
		// 	default  : {
		// 		// This shouldn't happen. There should always be a matching gamestate.
		// 		errorHandler(INVALIDGAMESTATE1);
		// 		break;
		// 	}
		// };

		if      (game.gamestate1==GS_TITLE    ) { gstate_title()   ; }
		else if (game.gamestate1==GS_OPTIONS  ) { gstate_options() ; }
		else if (game.gamestate1==GS_RULES    ) { gstate_rules()   ; }
		else if (game.gamestate1==GS_CREDITS  ) { gstate_credits() ; }
		else if (game.gamestate1==GS_PLAYING  ) { gstate_playing() ; }
		else{
			// This shouldn't happen. There should always be a matching gamestate.
			errorHandler(INVALIDGAMESTATE1);
		}

	}

	// __asm__ __volatile__ ("wdr");
	// __asm__ __volatile__ ("wdr");
}
// ===== MAIN =====

// OLD - UNUSED

// OLD - UNUSED