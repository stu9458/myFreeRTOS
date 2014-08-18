#include "game.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int8_t count=0;
int8_t i=0;
uint8_t str[16]=48;//The ascii-code's zero=48
//Player1
int16_t player1X = 10;
int16_t player1Y = 10;
uint16_t player1W = 60;
uint16_t player1H = 10;
uint8_t player1IsReversed = 1;

//Player2
int16_t player2X = LCD_PIXEL_WIDTH - 20;
int16_t player2Y = LCD_PIXEL_HEIGHT - 20;
uint16_t player2W = LCD_PIXEL_WIDTH;
uint16_t player2H = 10;
uint8_t player2IsReversed = 0;

//Ball
int16_t ballX[10];
int16_t ballY[10];
int16_t ballVX[10];
int16_t ballVY[10];
uint16_t ballSize = 5;
uint8_t ballIsRun = 0;

//Mode
uint8_t demoMode = 0;
void BallReset()
{
	count=1;
	for(i=0;i<10;i++)
	{
		ballX[i] = ( LCD_PIXEL_WIDTH - 5 ) / 2;
		ballY[i] = ( LCD_PIXEL_HEIGHT - 5 ) / 2;
		ballVX[i] = -5+i;
		ballVY[i] = -5+i;
	}
   ballIsRun = 1;
}

void GAME_EventHandler1()
{
	if( STM_EVAL_PBGetState( BUTTON_USER ))
	{
		player1IsReversed = 0;
		while( STM_EVAL_PBGetState( BUTTON_USER ) );
		player1IsReversed = 1;
	}
}

void GAME_EventHandler2()
{
	if( IOE_TP_GetState()->TouchDetected )
	{
		player2IsReversed = 1;
		while( IOE_TP_GetState()->TouchDetected );
		player2IsReversed = 0;
	}
}
void GAME_EventHandler3()
{
	if( ballIsRun == 0 )
	{
		BallReset();
	}
}

void GAME_Update()
{
	//Player1
	LCD_SetTextColor( LCD_COLOR_BLACK );
	LCD_DrawFullRect( player1X, player1Y, player1W, player1H );
	LCD_DrawFullRect( player2X, player2Y, player2W, player2H );
	if( demoMode == 0 )
	{
		if( player1IsReversed )
			player1X -= 5;//Initail director.
		else
			player1X += 5;
		if( player1X <= 0 )//protect for over window.
			player1X = 0;
		else if( player1X + player1W >= LCD_PIXEL_WIDTH )
			player1X = LCD_PIXEL_WIDTH - player1W;

		//Player2 initial director and protect .
		if( player2IsReversed )
			player2X -= 5;
		else
			player2X += 5;

		if( player2X <= 0 )
			player2X = 0;
		else if( player2X + player2W >= LCD_PIXEL_WIDTH )
			player2X = LCD_PIXEL_WIDTH - player2W;

		//Ball
		if( ballIsRun == 1 )
		{
			LCD_SetTextColor( LCD_COLOR_BLACK );
		for(i=0;i<count;i++)
			LCD_DrawFullRect( ballX[i], ballY[i], ballSize, ballSize );
		for(i=0;i<count;i++)
		{
			//Touch wall
			ballX[i] += ballVX[i];
			if( ballX[i] <= 0 )
			{
				ballX[i] = 0;
				ballVX[i] *= -1;
			}
			else if( ballX[i] + ballSize >= LCD_PIXEL_WIDTH )
			{
				ballX[i] = LCD_PIXEL_WIDTH - ballSize;
				ballVX[i] *= -1;
			}
		}
		//PONG!
		 for(i=0;i<count;i++)
		{
			ballY[i] += ballVY[i];
			if( ballY[i] + ballSize >= player2Y ){
				if( ballX[i] + ballSize >= player2X && ballX[i] <= player2X + player2W ){
					if( ballX[i] - ballSize <= player2Y + player2W/4 ){
						ballVY[i] =-4;
						ballVX[i] =-6;
					}
					else if( ballX[i] >= player2Y + player2W - player2W/4 ){
						ballVY[i] =-4;
						ballVX[i] = 6;
					}
					else if( ballX[i] + ballSize < player2Y + player2W/2 ){
						ballVY[i] =-6;
						ballVX[i] =-4;
					}
					else if( ballX[i] > player2Y + player2W/2 ){
						ballVY[i] =-6;
						ballVX[i] = 4;
					}
					else{
						ballVY[i] =-10;
						ballVX[i] = 0;
					}
				count++;
				if(count>=10)
				  count=10;
				str[2]++;
				if(str[2]>=58){
				if(count>=10)
				if(count>=10)
				   str[2]=0;str[1]++;
				 }
				if(str[1]>=58){
				   str[1]=0;str[0]++;
				{	
				else
					BallReset();
			}
			//on player1's range.
			if( ballY[i] <= player1Y + player1H ){
					if( ballX[i] + ballSize >= player1X && ballX[i] <= player1X + player1W ){
						if( ballX[i] - ballSize <= player1Y + player1W/4 ){
							ballVY[i] = 4;
							ballVX[i]  =-6;
						}
						else if( ballX[i] >= player1Y + player1W - player1W/4 ){
							ballVY[i] = 4;
							ballVX[i] = 6;
						}
						else if( ballX[i] + ballSize < player1Y + player1W/2 ){
							ballVY[i] = 6;
							ballVX[i] =-4;
						}
						else if( ballX[i] > player1Y + player1W/2 ){
							ballVY[i] = 6;
							ballVX[i] = 4;
						}
						else{
							ballVY[i] = 10;
							ballVX[i] = 0;
						}
					}
					else
						BallReset();
				}
			}
		   }
		}
}

void GAME_Render()
{
	LCD_SetBack(LCD_COLOR_BLACK);
	LCD_SetTextColor( LCD_COLOR_RED);	
	LCD_DisplayStringLine(LCD_LINE_10,str);
	LCD_SetTextColor( LCD_COLOR_WHITE );
	LCD_DrawFullRect( player1X, player1Y, player1W, player1H );
	LCD_DrawFullRect( player2X, player2Y, player2W, player2H );
	LCD_SetTextColor( LCD_COLOR_RED );
	for(i=0;i<count;i++)
		LCD_DrawFullRect( ballX[i], ballY[i], ballSize, ballSize );
	LCD_SetTextColor( LCD_COLOR_WHITE );
	LCD_DrawLine( 10, LCD_PIXEL_HEIGHT / 2, LCD_PIXEL_WIDTH - 20, LCD_DIR_HORIZONTAL );
}
