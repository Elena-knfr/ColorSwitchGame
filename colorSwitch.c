/* This files provides address values that exist in the system */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define BOARD                 "DE1-SoC"

/* VGA colors */
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x10FF
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00

/* Memory */
#define DDR_BASE              0x00000000
#define DDR_END               0x3FFFFFFF
#define A9_ONCHIP_BASE        0xFFFF0000
#define A9_ONCHIP_END         0xFFFFFFFF
#define SDRAM_BASE            0xC0000000
#define SDRAM_END             0xC3FFFFFF
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_ONCHIP_END       0xC803FFFF
#define FPGA_CHAR_BASE        0xC9000000
#define FPGA_CHAR_END         0xC9001FFF

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define JP1_BASE              0xFF200060
#define JP2_BASE              0xFF200070
#define PS2_BASE              0xFF200100
#define PS2_DUAL_BASE         0xFF200108
#define JTAG_UART_BASE        0xFF201000
#define JTAG_UART_2_BASE      0xFF201008
#define IrDA_BASE             0xFF201020
#define TIMER_BASE            0xFF202000
#define AV_CONFIG_BASE        0xFF203000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030
#define AUDIO_BASE            0xFF203040
#define VIDEO_IN_BASE         0xFF203060
#define ADC_BASE              0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE        0xFF709000
#define HPS_TIMER0_BASE       0xFFC08000
#define HPS_TIMER1_BASE       0xFFC09000
#define HPS_TIMER2_BASE       0xFFD00000
#define HPS_TIMER3_BASE       0xFFD01000
#define FPGA_BRIDGE           0xFFD0501C

/* ARM A9 MPCORE devices */
#define   PERIPH_BASE         0xFFFEC000    // base address of peripheral devices
#define   MPCORE_PRIV_TIMER   0xFFFEC600    // PERIPH_BASE + 0x0600

/* Interrupt controller (GIC) CPU interface(s) */
#define MPCORE_GIC_CPUIF      0xFFFEC100    // PERIPH_BASE + 0x100
#define ICCICR                0x00          // offset to CPU interface control reg
#define ICCPMR                0x04          // offset to interrupt priority mask reg
#define ICCIAR                0x0C          // offset to interrupt acknowledge reg
#define ICCEOIR               0x10          // offset to end of interrupt reg
/* Interrupt controller (GIC) distributor interface(s) */
#define MPCORE_GIC_DIST       0xFFFED000    // PERIPH_BASE + 0x1000
#define ICDDCR                0x00          // offset to distributor control reg
#define ICDISER               0x100         // offset to interrupt set-enable regs
#define ICDICER               0x180         // offset to interrupt clear-enable regs
#define ICDIPTR               0x800         // offset to interrupt processor targets regs
#define ICDICFR               0xC00         // offset to interrupt configuration regs

#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

int GROUND = 240;
#define GRAVITY 5
#define LIFT 20
#define	NUM_OBSTACLES 3
#define ROTATION_SPEED 0.02908882 //angle change every 1/60 sec

volatile int pixel_buffer_start; // global variable
volatile char* character_buffer = (char*) 0xC9000000;
volatile int * pixel_ctrl_ptr;
short int WHITE = 0xFFFF;
char startArray1[] = "COLOR SWITCH";
char startArray2[] = "Press space to start";
char startArray3[] = "Total Stars:";
char startArray4[] = "Score: ";
char startArray5[] = "GAME OVER!"; ////////////////
char startArray6[] = "Your Score: "; /////////////////
char s[4];
char s1[4];/////////////

int obstacle_array1[232][3];
int obstacle_array2[288][3];
int obstacle_array3[176][3];
short int obstacle_color [3][4] = {{RED, CYAN, YELLOW, MAGENTA},
								   {RED, CYAN, YELLOW, MAGENTA},
								   {RED, CYAN, YELLOW, MAGENTA},};
int y_obstacle [NUM_OBSTACLES] = {50, -70, -190}; //y coordinate of obstacle
int r_obstacle [NUM_OBSTACLES] = {40, 50, 30}; //radius of obstacle

int closest_obstacle_i = 0;

//int player_array[24][2];
int y_player = 200; //y coordinate of player
short int color_player = RED;
int r_player = 4; //radius of player

//void draw_circle();
void plot_pixel(int x, int y, short int line_color);
void swap(int* x, int* y);
void draw_line(int x0, int y0, int x1, int y1, short int color);
void clear_screen();
void wait_cycle();
//void draw_();
void draw_circ(int yc, int r, short int color);
draw_filled_circle (int yc, int r, short int color);
void right_down_arc(int yc, int r, short int color);
void right_up_arc(int yc, int r, short int color);
void left_down_arc(int yc, int r, short int color);
void left_up_arc(int yc, int r, short int color);
void left_arc(int yc, int r, short int color);
void right_arc(int yc, int r, short int color);
void bottom_arc(int yc, int r, short int color);
void top_arc(int yc, int r, short int color);
void delay(float number_of_seconds);
bool is_inside_screen(int x, int y);
void draw_circle_obstacle(int y, int r, short int color1, short int color2, short int color3, short int color4);
void draw_circle_obstacle_shifted_color(int y, int r, short int color1, short int color2, short int color3, short int color4);
void spinningCircles(int y, int r, short int color1, short int color2, short int color3, short int color4);
void runGame(int y0, int y1, int y2, int r0, int r1, int r2);
void startGame();
void eraseMessage();
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3);

void draw_obstacle_new(int y, int r, double start_angle, short int color1, short int color2, short int color3, short int color4);
void draw_circle_angle_helper (int xc, int yc, int x, int y, double start_angle, short int color1, short int color2, short int color3, short int color4);
float myAtan2(int a, int b);

void load_circle(int r, int obstacle_index, int array[][3], int circumference);
//void load_circle(int r, int obstacle_index);
int find_pixel_circumference(int r);
int min (int a, int b);

int score;
void get_score(int score);
void erase_score();
void display_score();
void erase_score_count();
void gameover(); ////////////////////////////////////////////////
void draw_symb(int yc, int xc, int r, short int color); ///////////
void top_arc_symb(int yc, int xc, int r, short int color); ///////////////
void bottom_arc_symb(int yc, int xc, int r, short int color);
void erase_gameover(); ////////////////////////////////////////////////////////////////////////////////////////////

bool collided (int closest);
float two_dis(float x1, float y1, float x2, float y2);

int main(void)
{
    pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;

	clear_screen();

    eraseMessage();
	startGame();

	bool run_game = false;

	volatile int * PS2_ptr = (int *) 0xFF200100;  // PS/2 port address
	int PS2_data, RVALID;
	int keyPress;
	char byte1 = 0, byte2 = 0;

    *(PS2_ptr) = 0xFF; //reset
	while(1) {
		PS2_data = *(PS2_ptr);
		RVALID = PS2_data & 0x8000;
	  	if (RVALID){
			byte1 = byte2;
			byte2 = PS2_data & 0xFF;;
			if (byte2 == 0x29 && byte1 != 0xF0) { // if space is pressed
				run_game = true;
				break;
			}
		}
	}

	clear_screen();
	eraseMessage();
	erase_score();/////////////////////

	if(run_game) {

		display_score();//////////////////

		int rotate_speed [NUM_OBSTACLES] = {1, -1, 1};// 0.02908882, 0.02908882
		int start_shrift [NUM_OBSTACLES] = {0,35,20};
		int pixel_shrift [NUM_OBSTACLES] = {0,0,0};
		short int temp_color;

		int velY = 0;

		int game_over = 0; //initialize game over to be false

		/*volatile int * PS2_ptr = (int *) 0xFF200100;  // PS/2 port address
		int PS2_data, RVALID;
		int keyPress;
		char byte1 = 0, byte2 = 0;*/

		//*(PS2_ptr) = 0xFF; //reset



		load_circle (r_obstacle[0], 0, obstacle_array1, 232);
		load_circle (r_obstacle[1], 1, obstacle_array2, 288);
		load_circle (r_obstacle[2], 2, obstacle_array3, 176);
		while(!game_over) {

			/*printf("%d ", find_pixel_circumference(40));
			printf("%d ", find_pixel_circumference(50));
printf("%d ", find_pixel_circumference(30));*/

		/*if(ABS(y_obstacle[2] - y_player) < r_player + r_obstacle[2])
			draw_filled_circle(y_player, r_player, RED);
		else if(ABS(y_obstacle[1] - y_player) < r_player + r_obstacle[1])
			draw_filled_circle(y_player, r_player, YELLOW);
		else if(ABS(y_obstacle[0] - y_player) < r_player + r_obstacle[0])
			draw_filled_circle(y_player, r_player, CYAN);
		else
			draw_filled_circle(y_player, r_player, 0xffff);*/

			PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port

			//draw_circ(y_player, r_player, 0xffff);
			/*
			if (y_player + r_player > y_obstacle[0] + r_obstacle[0])
				draw_filled_circle(y_player, r_player, 0xffff);
			else if (y_player + r_player > y_obstacle[1] + r_obstacle[1] && y_player + r_player < y_obstacle[0] + r_obstacle[0])
				draw_filled_circle(y_player, r_player, RED);
			else if (y_player + r_player > y_obstacle[2] + r_obstacle[2] && y_player + r_player < y_obstacle[1] + r_obstacle[1])
				draw_filled_circle(y_player, r_player, YELLOW);
			else {
				draw_filled_circle(y_player, r_player, 0xFC18);
				//color_player = 0xFC18;
			}*/
			draw_filled_circle(y_player, r_player, color_player);

			//draw the obstacles
			//for (int j=0; j<NUM_OBSTACLES; j++ ){
			for (int i=0; i<sizeof(obstacle_array1)/sizeof(obstacle_array1[0]); i++){
				plot_pixel(150+obstacle_array1[i][0], y_obstacle[0]+obstacle_array1[i][1], obstacle_array1[i][2]);
			}
			for (int i=0; i<sizeof(obstacle_array2)/sizeof(obstacle_array2[0]); i++){
				plot_pixel(150+obstacle_array2[i][0], y_obstacle[1]+obstacle_array2[i][1], obstacle_array2[i][2]);
			}
			for (int i=0; i<sizeof(obstacle_array3)/sizeof(obstacle_array3[0]); i++){
				plot_pixel(150+obstacle_array3[i][0], y_obstacle[2]+obstacle_array3[i][1], obstacle_array3[i][2]);
			}
			//}

			wait_cycle();
			//erase old frame
			draw_filled_circle(y_player, r_player, 0x0000);
			for (int i=0; i<NUM_OBSTACLES; i++ ){
				draw_circ(y_obstacle[i], r_obstacle[i], 0x0000);
			}




			/* shifting color elements */
			temp_color =obstacle_array1[sizeof(obstacle_array1)/sizeof(obstacle_array1[0])-1][2];
			for(int i=sizeof(obstacle_array1)/sizeof(obstacle_array1[0])-1; i>0; i -= rotate_speed [0])
			{
				obstacle_array1[i][2]= obstacle_array1[i-rotate_speed [0]][2];;
			}
			obstacle_array1[0][2]=temp_color;

			temp_color =obstacle_array2[sizeof(obstacle_array2)/sizeof(obstacle_array2[0])-1][2];
			for(int i=sizeof(obstacle_array2)/sizeof(obstacle_array2[0])-1; i>0; i -= rotate_speed [0])
			{
				obstacle_array2[i][2]= obstacle_array2[i-rotate_speed [0]][2];;
			}
			obstacle_array2[0][2]=temp_color;

			temp_color =obstacle_array3[sizeof(obstacle_array3)/sizeof(obstacle_array3[0])-1][2];
			for(int i=sizeof(obstacle_array3)/sizeof(obstacle_array3[0])-1; i>0; i -= rotate_speed [0])
			{
				obstacle_array3[i][2]= obstacle_array3[i-rotate_speed [0]][2];;
			}
			obstacle_array3[0][2]=temp_color;



			/*for (int i=0; i<NUM_OBSTACLES; i++ ){


				//obstacle_array[i] = y_obstacle[(i+2)%NUM_OBSTACLES] - 120;
				pixel_shrift [i] += rotate_speed [i];

				if (pixel_shrift[i] >= 58){
					pixel_shrift[i] = 0;
					if (spin_cycle == 3)
						spin_cycle = 0;
					else {
						spin_cycle++;
					}
				}
			}*/

			//once the player jumps high enough
			if (y_player + velY + GRAVITY > 120){
				y_player += velY + GRAVITY;
				velY = velY*0.8;
				if (y_player >= GROUND)
					y_player = GROUND;
			} else {
				//move all obstacles down
				for (int i=0; i<NUM_OBSTACLES; i++ ){
					y_obstacle[i] += 120 - (y_player + velY + GRAVITY);
					if (y_obstacle[i] - r_obstacle[i] > RESOLUTION_Y)
						y_obstacle[i] = y_obstacle[(i+2)%NUM_OBSTACLES] - 120;
				}
				GROUND += 120 - (y_player + velY + GRAVITY);
				velY = velY*0.8;
			}

			if (y_player + r_player + 5 < y_obstacle[closest_obstacle_i] - r_obstacle[closest_obstacle_i]){
				closest_obstacle_i = (closest_obstacle_i+1)%NUM_OBSTACLES;
				score++;
				get_score(score);
			}


			//check for collision
			if (collided(closest_obstacle_i)){
				game_over == true;
				break;
			}


			RVALID = PS2_data & 0x8000;
			if (RVALID){
				byte1 = byte2;
				byte2 = PS2_data & 0xFF;;
				if (byte2 == 0x29 && byte1 != 0xF0) { // if space is pressed
					velY = -LIFT;
				}
			}
		}
	}
	gameover();
	volatile int * PS2_ptrr = (int *) 0xFF200100;  // PS/2 port address
	int PS2_dataa, RVALIDD;
	int keyPresss;
	char bytee1 = 0, bytee2 = 0;

    *(PS2_ptrr) = 0xFF; //reset
	while(1) {
		PS2_dataa = *(PS2_ptrr);
		RVALIDD = PS2_dataa & 0x8000;
	  	if (RVALIDD){
			bytee1 = bytee2;
			bytee2 = PS2_dataa & 0xFF;;
			if (bytee2 == 0x29 && bytee1 != 0xF0) { // if space is pressed
				delay(2);
				erase_gameover();
				break;
			}
		}
	}

}

bool collided (int closest) {
	float distance;

	switch (closest){
		case 0:
			for (int i=sizeof(obstacle_array1)/sizeof(obstacle_array1[0])/4-8; i<sizeof(obstacle_array1)/sizeof(obstacle_array1[0])/4+8; i++){
				distance = two_dis(150+obstacle_array1[i][0], y_obstacle[0] + obstacle_array1[i][1], 150, y_player);
				if (distance <= r_player && obstacle_array1[i][2] != color_player){
					return true;
				}
			}
			for (int i=3*sizeof(obstacle_array1)/sizeof(obstacle_array1[0])/4-8; i<3*sizeof(obstacle_array1)/sizeof(obstacle_array1[0])/4+8; i++){
				distance = two_dis(150+obstacle_array1[i][0], y_obstacle[0] + obstacle_array1[i][1], 150, y_player);
				if (distance <= r_player && obstacle_array1[i][2] != color_player){
					return true;
				}
			}
			break;
		case 1:
			for (int i=sizeof(obstacle_array2)/sizeof(obstacle_array2[0])/4-8; i<sizeof(obstacle_array2)/sizeof(obstacle_array2[0])/4+8; i++){
				distance = two_dis(150+obstacle_array2[i][0], y_obstacle[1] + obstacle_array2[i][1], 150, y_player);
				if (distance <= r_player && obstacle_array2[i][2] != color_player){
					return true;
				}
			}
			for (int i=3*sizeof(obstacle_array2)/sizeof(obstacle_array2[0])/4-8; i<3*sizeof(obstacle_array2)/sizeof(obstacle_array2[0])/4+8; i++){
				distance = two_dis(150+obstacle_array2[i][0], y_obstacle[1] + obstacle_array2[i][1], 150, y_player);
				if (distance <= r_player && obstacle_array2[i][2] != color_player){
					return true;
				}
			}
			break;
		case 2:
			for (int i=sizeof(obstacle_array3)/sizeof(obstacle_array3[0])/4-8; i<sizeof(obstacle_array3)/sizeof(obstacle_array3[0])/4+8; i++){
				distance = two_dis(150+obstacle_array3[i][0], y_obstacle[2] + obstacle_array3[i][1], 150, y_player);
				if (distance <= r_player && obstacle_array3[i][2] != color_player){
					return true;
				}
			}
			for (int i=3*sizeof(obstacle_array3)/sizeof(obstacle_array3[0])/4-8; i<3*sizeof(obstacle_array3)/sizeof(obstacle_array3[0])/4+8; i++){
				distance = two_dis(150+obstacle_array3[i][0], y_obstacle[2] + obstacle_array3[i][1], 150, y_player);
				if (distance <= r_player && obstacle_array3[i][2] != color_player){
					return true;
				}
			}
			break;
	}



	return false;
}

//function to find distance between 2 points
float two_dis(float x1, float y1, float x2, float y2) {
   float dis = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
   return dis;
}

void startGame()
{
	erase_gameover();
	erase_score_count();
	erase_score();

	int x = 30;
	int y = 2;
	char startArray1[] = "COLOR SWITCH";
	for (int i = 0; i < sizeof(startArray1); i++) {
		*(char *) (character_buffer + (y << 7) + x) = startArray1[i];
		x++;
	}

	x = 30;
	y = 4;
	char startArray2[] = "Press space to start";
	for (int i = 0; i < sizeof(startArray2); i++) {
		*(char *) (character_buffer + (y << 7) + x) = startArray2[i];
		x++;
	}

	for(int i = 50; i > 45; i--) {
  		draw_circle_obstacle(120, i, BLUE, PINK, CYAN, MAGENTA);
  	}

  	for(int i = 40; i > 35; i--) {
  		draw_circle_obstacle(120, i, MAGENTA, BLUE, PINK, CYAN);
  	}

  	for(int i = 30; i > 25; i--) {
  		draw_circle_obstacle(120, i, CYAN, MAGENTA, BLUE, PINK);
  	}

  	for(int i = 20; i > 15; i--) {
  		draw_circle_obstacle(120, i, PINK, CYAN, MAGENTA, BLUE);
  	}

  	//draw_triangle(145, 115, 155, 120, 145, 125);
  	draw_triangle(145, 110, 163, 120, 145, 130);

  	x = 7;
  	y = 35;
  	char startArray3[] = "Total Stars:";
  	for (int i = 0; i < sizeof(startArray3); i++) {
  		*(char *) (character_buffer + (y << 7) + x) = startArray3[i];
  		x++;
  	}

  	// middle star
  	draw_line(48, 150, 42, 160, 0xFFEF);
  	draw_line(48, 150, 54, 160, 0xFFEF);
  	draw_line(40, 153, 54, 160, 0xFFEF);
  	draw_line(56, 153, 42, 160, 0xFFEF);
  	draw_line(56, 153, 40, 153, 0xFFEF);

  	// left star
  	draw_line(29, 150, 23, 160, 0xFFEF);
  	draw_line(29, 150, 35, 160, 0xFFEF);
  	draw_line(21, 153, 35, 160, 0xFFEF);
  	draw_line(37, 153, 23, 160, 0xFFEF);
  	draw_line(37, 153, 21, 153, 0xFFEF);

  	// right star
  	draw_line(67, 150, 61, 160, 0xFFEF);
  	draw_line(67, 150, 73, 160, 0xFFEF);
  	draw_line(59, 153, 73, 160, 0xFFEF);
  	draw_line(75, 153, 61, 160, 0xFFEF);
  	draw_line(75, 153, 59, 153, 0xFFEF);

    for(int i = 40; i < 45; i++) {
		draw_line(i, 40, i+10, 60, BLUE);
		draw_line(240+i, 40, 230+i, 60, BLUE);
	}

	for(int i = 50; i < 55; i++) {
		draw_line(i, 60, i+10, 80, CYAN);
		draw_line(220+i, 60, 210+i, 80, CYAN);
	}

	for(int i = 65; i < 70; i++) {
		draw_line(i, 50, i-15, 60, PINK);
		draw_line(190+i, 50, 205+i, 60, PINK);
	}

	for(int i = 50; i < 55; i++) {
		draw_line(i, 60, i-15, 70, MAGENTA);
		draw_line(220+i, 60, 235+i, 70, MAGENTA);
	}

  for(int i = 8; i > 0; i--) {
		draw_circle_obstacle(210, i, YELLOW, YELLOW, YELLOW, YELLOW);
	}

}

void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
	draw_line(x1, y1, x2, y2, GREY);
  	draw_line(x2, y2, x3, y3, GREY);
  	draw_line(x3, y3, x1, y1, GREY);
}


void eraseMessage()
{
	int x = 30;
	int y = 2;
	//char startArray1[] = "";
	for (int i = 0; i < sizeof(startArray1); i++) {
		startArray1[i] = '\0';
		*(char *) (character_buffer + (y << 7) + x) = startArray1[i];
		x++;
	}

	x = 30;
	y = 4;
	//char startArray2[] = "";
	for (int i = 0; i < sizeof(startArray2); i++) {
		startArray2[i] = '\0';
		*(char *) (character_buffer + (y << 7) + x) = startArray2[i];
		x++;
	}

	x = 7;
	y = 35;
	for (int i = 0; i < sizeof(startArray3); i++) {
		startArray3[i] = '\0';
		*(char *) (character_buffer + (y << 7) + x) = startArray3[i];
		x++;
	}

}

void plot_pixel(int x, int y, short int line_color)
{
	if (is_inside_screen(x, y)) //only plot pixel if the xy coordinates are within screen bounds
    	*(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void swap(int* x, int* y)
{
    *x = *x + *y;
    *y = *x - *y;
    *x = *x - *y;
}

void draw_line(int x0, int y0, int x1, int y1, short int color)
{
	int is_steep = ABS(y1 - y0) > ABS(x1 - x0);
	if(is_steep) {
		swap(&x0, &y0);
		swap(&x1, &y1);
	}

	if(x0 > x1) {
		swap(&x0, &x1);
		swap(&y0, &y1);
	}

	int deltax = x1 - x0;
	int deltay = ABS(y1 - y0);
	int error = -(deltax / 2);

	int y = y0;
	int y_step = 0;
	if(y0 < y1) {
		y_step = 1;
	}
	else {
		y_step = -1;
	}

	for(int x = x0; x <= x1; ++x) {
		if(is_steep) {
			plot_pixel(y, x, color);
		}
		else {
			plot_pixel(x, y, color);
		}

		error = error + deltay;
		if(error >= 0 ) {
			y = y + y_step;
			error = error - deltax;
		}
	}
}

void clear_screen()
{
    int x = 0, y = 0;

    for (x = 0; x < RESOLUTION_X; ++x)
    {
        for (y = 0; y < RESOLUTION_Y; ++y)
        {
            plot_pixel(x, y, 0x0000);
        }
    }
}

void wait_cycle()
{
	volatile int* pixel_ctrl_ptr = (int *)0xFF203020; //pixel controller
	register int status;

    *pixel_ctrl_ptr = 1; //start the synchronization process

	status = *(pixel_ctrl_ptr + 3);
    while((status & 0x01) != 0)
    {
       status = *(pixel_ctrl_ptr + 3); // keep reading the status
    }

    return; //exit when s equals to 1
}

void draw_circ(int yc, int r, short int color) {
	int d=3-2*r;
	int x=0;
	int y=r;
	int xc = 150;
	//int yc = 100;
	while(x<=y)
	{
		plot_pixel(xc+x,yc+y,color);
		plot_pixel(xc-y,yc-x,color);
		plot_pixel(xc+y,yc-x,color);
		plot_pixel(xc-y,yc+x,color);
		plot_pixel(xc+y,yc+x,color);
		plot_pixel(xc-x,yc-y,color);
		plot_pixel(xc+x,yc-y,color);
		plot_pixel(xc-x,yc+y,color);
		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
	}
}

draw_filled_circle (int yc, int r, short int color) {
	int d=3-2*r;
	int x=0;
	int y=r;
	int xc = 150;
	//int yc = 100;
	while(x<=y)
	{
		draw_line(xc+x, yc+y, xc-x,yc+y, color);
		draw_line(xc-y,yc-x, xc+y,yc-x, color);
		draw_line(xc-y,yc+x, xc+y,yc+x, color);
		draw_line(xc-x,yc-y, xc+x,yc-y, color);

		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
	}
}

int find_pixel_circumference(int r){
	int d=3-2*r;
	int x=0;
	int y=r;
	int xc = 150;
	int count = 0;
	while(x<=y)
	{
		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
		count += 8;
	}
	printf("%d ", count);
	return count;
}

void load_circle(int r, int obstacle_index, int array[][3], int circumference){
	int d=3-2*r;
	int x=0;
	int y=r;
	int xc = 150;
	int count = 0;
	int one_eighth_circumf = circumference / 8;

	while(x<=y)
	{
		array[count][0]=y;
		array[count][1]=x;
		array[count][2]=obstacle_color[obstacle_index][0];

		array[2*one_eighth_circumf-count-1][0]=x;
		array[2*one_eighth_circumf-count-1][1]=y;
		array[2*one_eighth_circumf-count-1][2]=obstacle_color[obstacle_index][0];

		array[count+2*one_eighth_circumf][0]=-x;
		array[count+2*one_eighth_circumf][1]=y;
		array[count+2*one_eighth_circumf][2]=obstacle_color[obstacle_index][1];

		array[4*one_eighth_circumf-count-1][0]=-y;
		array[4*one_eighth_circumf-count-1][1]=x;
		array[4*one_eighth_circumf-count-1][2]=obstacle_color[obstacle_index][1];

		array[count+4*one_eighth_circumf][0]=-y;
		array[count+4*one_eighth_circumf][1]=-x;
		array[count+4*one_eighth_circumf][2]=obstacle_color[obstacle_index][2];

		array[6*one_eighth_circumf-count-1][0]=-x;
		array[6*one_eighth_circumf-count-1][1]=-y;
		array[6*one_eighth_circumf-count-1][2]=obstacle_color[obstacle_index][2];

		array[count+6*one_eighth_circumf][0]=x;
		array[count+6*one_eighth_circumf][1]=-y;
		array[count+6*one_eighth_circumf][2]=obstacle_color[obstacle_index][3];

		array[8*one_eighth_circumf-count-1][0]=y;
		array[8*one_eighth_circumf-count-1][1]=-x;
		array[8*one_eighth_circumf-count-1][2]=obstacle_color[obstacle_index][3];

		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
		count++;
	}
}

/*void load_circle(int r, int obstacle_index, int* array, int circumference){
	int d=3-2*r;
	int x=0;
	int y=r;
	int xc = 150;
	int count = 0;
	int one_eighth_circumf = circumference / 8;

	while(x<=y)
	{
		array[count][0]=y;
		array[count][1]=x;
		array[count][2]=obstacle_color[obstacle_index][0];

		array[2*-count-1][0]=x;
		array[58-count-1][1]=y;
		array[58-count-1][2]=obstacle_color[obstacle_index][0];

		array[count+2*29][0]=-x;
		array[count+2*29][1]=y;
		array[count+2*29][2]=obstacle_color[obstacle_index][1];

		array[4*29-count-1][0]=-y;
		array[4*29-count-1][1]=x;
		array[4*29-count-1][2]=obstacle_color[obstacle_index][1];

		array[count+4*29][0]=-y;
		array[count+4*29][1]=-x;
		array[count+4*29][2]=obstacle_color[obstacle_index][2];

		array[6*29-count-1][0]=-x;
		array[6*29-count-1][1]=-y;
		array[6*29-count-1][2]=obstacle_color[obstacle_index][2];

		array[count+6*29][0]=x;
		array[count+6*29][1]=-y;
		array[count+6*29][2]=obstacle_color[obstacle_index][3];

		array[8*29-count-1][0]=y;
		array[8*29-count-1][1]=-x;
		array[8*29-count-1][2]=obstacle_color[obstacle_index][3];

		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
		count++;
	}
}*/

void right_down_arc(int yc, int r, short int color) {
	int d=3-2*r;
	int x=0;
	int y=r;
	int xc = 150;
	//int yc = 100;
	while(x<=y)
	{
		plot_pixel(xc+x,yc+y,color);
		plot_pixel(xc+y,yc+x,color);

		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
	}
}

void right_up_arc(int yc, int r, short int color) {
	int d=3-2*r;
	int x=0;
	int y=r;
	int xc = 150;
	//int yc = 100;
	while(x<=y)
	{
		plot_pixel(xc+y,yc-x,color);
		plot_pixel(xc+x,yc-y,color);

		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
	}
}

void left_down_arc(int yc, int r, short int color) {
	int d=3-2*r;
	int x=0;
	int y=r;
	int xc = 150;
	//int yc = 100;
	while(x<=y)
	{
		plot_pixel(xc-x,yc+y,color);
		plot_pixel(xc-y,yc+x,color);

		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
	}
}

void left_up_arc(int yc, int r, short int color) {
	int d=3-2*r;
	int x=0;
	int y=r;
	int xc = 150;
	//int yc = 100;
	while(x<=y)
	{
		plot_pixel(xc-x,yc-y,color);
		plot_pixel(xc-y,yc-x,color);

		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
	}
}

void bottom_arc(int yc, int r, short int color) {
	int d=3-2*r;
	int x=0;
	int y=r;
	int xc = 150;
	//int yc = 100;
	while(x<=y)
	{
		plot_pixel(xc+x,yc+y,color);
		plot_pixel(xc-x,yc+y,color);

		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
	}
}

void right_arc(int yc, int r, short int color) {
	int d=3-2*r;
	int x=0;
	int y=r;
	int xc = 150;
	//int yc = 100;
	while(x<=y)
	{
		plot_pixel(xc+y,yc+x,color);
		plot_pixel(xc+y,yc-x,color);

		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
	}
}

void top_arc(int yc, int r, short int color) {
	int d=3-2*r;
	int x=0;
	int y=r;
	int xc = 150;
	//int yc = 100;
	while(x<=y)
	{
		plot_pixel(xc+x,yc-y,color);
		plot_pixel(xc-x,yc-y,color);

		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
	}
}

void left_arc(int yc, int r, short int color) {
	int d=3-2*r;
	int x=0;
	int y=r;
	int xc = 150;
	//int yc = 100;
	while(x<=y)
	{
		plot_pixel(xc-y,yc-x,color);
		plot_pixel(xc-y,yc+x,color);

		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
	}
}

void delay(float number_of_seconds)
{
    // Converting time into milli_seconds
    float milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

//function that checks if a set of x y coordinates are within the screen limits
//returns true or false
bool is_inside_screen(int x, int y){
	return (x>0 && x<RESOLUTION_X && y>0 && y<RESOLUTION_Y);
}

//wrapper function that calls all 4 draw arc functions to draw a full circle
void draw_circle_obstacle(int y, int r, short int color1, short int color2, short int color3, short int color4){
	right_down_arc(y, r, color1);
	left_down_arc(y, r, color2);
	left_up_arc(y, r, color3);
	right_up_arc(y, r, color4);
}

void draw_circle_obstacle_shifted_color(int y, int r, short int color1, short int color2, short int color3, short int color4) {
	bottom_arc(y, r, color1);
	left_arc(y, r, color2);
	top_arc(y, r, color3);
	right_arc(y, r, color4);
}

void draw_obstacle_using_angle (int center_y, int r, double start_angle, short int color1, short int color2, short int color3, short int color4){
	const float theta_spacing = 0.07;
	float costheta, sintheta;
	int circlex, circley;
	for (float theta=start_angle; theta < start_angle + 2*M_PI; theta += theta_spacing) {
		// precompute sines and cosines of theta
    	costheta = cos(theta);
		sintheta = sin(theta);

		circlex = (int)(RESOLUTION_X/2 + r*costheta);
      	circley = (int)(center_y + r*sintheta);

		if (theta >= start_angle && theta < start_angle + M_PI/2)
			plot_pixel(circlex,circley,color1);
		else if (theta >= start_angle + M_PI/2 && theta < start_angle + M_PI)
			plot_pixel(circlex,circley,color2);
		else if (theta >= start_angle + M_PI && theta < start_angle + 3*M_PI/2)
			plot_pixel(circlex,circley,color3);
		else if (theta >= start_angle + 2*M_PI/3 && theta < start_angle + 2*M_PI)
			plot_pixel(circlex,circley,color4);
	}

}

void draw_obstacle_new(int yc, int r, double start_angle, short int color1, short int color2, short int color3, short int color4) {
	int d=3-2*r;
	int x=0;
	int y=r;
	int xc = 150;

	while(x<=y)
	{
		draw_circle_angle_helper (xc, yc, x, y, start_angle, color1, color2, color3, color4);
		draw_circle_angle_helper (xc, yc, -y, -x, start_angle, color1, color2, color3, color4);
		draw_circle_angle_helper (xc, yc, +y, -x, start_angle, color1, color2, color3, color4);
		draw_circle_angle_helper (xc, yc, -y, +x, start_angle, color1, color2, color3, color4);
		draw_circle_angle_helper (xc, yc, y, x, start_angle, color1, color2, color3, color4);
		draw_circle_angle_helper (xc, yc, -x, -y, start_angle, color1, color2, color3, color4);
		draw_circle_angle_helper (xc, yc, +x, -y, start_angle, color1, color2, color3, color4);
		draw_circle_angle_helper (xc, yc, -x, +y, start_angle, color1, color2, color3, color4);


		/*
		plot_pixel(xc+x,yc+y,color);
		plot_pixel(xc-y,yc-x,color);
		plot_pixel(xc+y,yc-x,color);
		plot_pixel(xc-y,yc+x,color);
		plot_pixel(xc+y,yc+x,color);
		plot_pixel(xc-x,yc-y,color);
		plot_pixel(xc+x,yc-y,color);
		plot_pixel(xc-x,yc+y,color);*/

		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
	}
}


void draw_circle_angle_helper (int xc, int yc, int x, int y, double start_angle, short int color1, short int color2, short int color3, short int color4){
	//float start_angle_tan = tan(start_angle);
	float theta =atan2(y,x) + M_PI; //myAtan2(y,x);//M_PI;  //y/x;
	// atan2(y,x) + M_PI; //
	/*
	if (theta >= start_angle && theta < start_angle + M_PI/2)
		plot_pixel(xc+x,yc+y,color1);
	else if (theta >= start_angle + M_PI/2 && theta < start_angle + M_PI)
		plot_pixel(xc+x,yc+y,color2);
	else if (theta >= start_angle - M_PI && theta < start_angle - M_PI/2)
		plot_pixel(xc+x,yc+y,color3);
	else if (theta >= start_angle - M_PI/2 && theta < start_angle)
		plot_pixel(xc+x,yc+y,color4);
	*/

	if (theta >= 0 && theta < start_angle)
		plot_pixel(xc+x,yc+y,color4);
	else if (theta >= start_angle && theta < start_angle + M_PI/2)
		plot_pixel(xc+x,yc+y,color1);
	else if (theta >= start_angle + M_PI/2 && theta < start_angle + M_PI)
		plot_pixel(xc+x,yc+y,color2);
	else if (theta >= start_angle + M_PI && theta < start_angle + 3*M_PI/2)
		plot_pixel(xc+x,yc+y,color3);
	else if (theta >= start_angle + 3*M_PI/2 && theta < 2*M_PI)
		plot_pixel(xc+x,yc+y,color4);

}

float myAtan2(int a, int b) {
    float atan2val;
    if (b > 0) {
        atan2val = atan(a/b);
    }
    else if ((b < 0) && (a >= 0)) {
        atan2val = atan(a/b) + M_PI;
    }
    else if ((b < 0) && (a < 0)) {
        atan2val = atan(a/b) - M_PI;
    }
    else if ((b = 0) && (a > 0)) {
        atan2val = M_PI / 2;
    }
    else if ((b = 0) && (a < 0)) {
        atan2val = 0 - M_PI / 2;
    }
    else if ((b = 0) && (a = 0)) {
        atan2val = 1000;               //represents undefined
    }
    return atan2val;
}

int min (int a, int b) {
	if (a<b)
		return a;
	else
		return b;
}

void get_score(int score)
{
	//char s[4];
	sprintf(s,"%d",score); // convert int to string

	int x = 12;
	int y = 2;
	for (int i = 0; i < sizeof(s); i++) {
		*(char *) (character_buffer + (y << 7) + x) = s[i];
		x++;
	}

}

void erase_score() {
	int x = 5;
	int y = 2;
	for (int i = 0; i < sizeof(startArray4); i++) {
		startArray4[i] = '\0';
		*(char *) (character_buffer + (y << 7) + x) = startArray4[i];
		x++;
	}
}

void erase_score_count() {
	int x = 12;
	int y = 2;
	for (int i = 0; i < sizeof(s); i++) {
		s[i] = '\0';
		*(char *) (character_buffer + (y << 7) + x) = s[i];
		x++;
	}
}

void display_score() {
	int x = 5;
	int y = 2;
	char startArray4[] = "Score: ";
	for (int i = 0; i < sizeof(startArray4); i++) {
		*(char *) (character_buffer + (y << 7) + x) = startArray4[i];
		x++;
	}
}

void gameover() {
	clear_screen();
	erase_score();
	erase_score_count();

	int x = 160;
	int y = 20;
	char startArray5[] = "GAME OVER";
	for (int i = 0; i < sizeof(startArray5); i++) {
		*(char *) (character_buffer + (y << 7) + x) = startArray5[i];
		x++;
	}

	x = 160;
	y = 25;
	char startArray6[] = "Your Score: ";
	for (int i = 0; i < sizeof(startArray6); i++) {
		*(char *) (character_buffer + (y << 7) + x) = startArray6[i];
		x++;
	}

	char s1[4];
	sprintf(s1,"%d",score); // convert int to string

	x = 173;
	y = 25;
	for (int i = 0; i < sizeof(s1); i++) {
		*(char *) (character_buffer + (y << 7) + x) = s1[i];
		x++;
	}

	draw_symb(100, 60, 20, BLUE);
	draw_symb(90, 66, 2, GREEN);
	draw_symb(90, 54, 2, GREEN);
	top_arc_symb(113, 60, 10, MAGENTA);


	bottom_arc_symb(140, 40, 40, YELLOW);
	bottom_arc_symb(140, 200, 40, YELLOW);

	top_arc_symb(200, 120, 40, PINK);
	top_arc_symb(200, 280, 40, PINK);

	bottom_arc_symb(10, 40, 40, YELLOW);
	bottom_arc_symb(10, 200, 40, YELLOW);

	top_arc_symb(70, 120, 40, PINK);
	top_arc_symb(70, 280, 40, PINK);

}

void erase_gameover() {
	clear_screen();

	int x = 160;
	int y = 20;
	//char startArray5[] = "GAME OVER";
	for (int i = 0; i < sizeof(startArray5); i++) {
		startArray5[i] = '\0';
		*(char *) (character_buffer + (y << 7) + x) = startArray5[i];
		x++;
	}

	x = 160;
	y = 25;
	//char startArray6[] = "Your Score: ";
	for (int i = 0; i < sizeof(startArray6); i++) {
		startArray6[i] = '\0';
		*(char *) (character_buffer + (y << 7) + x) = startArray6[i];
		x++;
	}

	//char s1[4];
	//sprintf(s1,"%d",score); // convert int to string

	x = 173;
	y = 25;
	for (int i = 0; i < sizeof(s1); i++) {
		s1[i] = '\0';
		*(char *) (character_buffer + (y << 7) + x) = s1[i];
		x++;
	}
}

void draw_symb(int yc, int xc, int r, short int color) {
	int d=3-2*r;
	int x=0;
	int y=r;
	//int xc = 150;
	//int yc = 100;
	while(x<=y)
	{
		plot_pixel(xc+x,yc+y,color);
		plot_pixel(xc-y,yc-x,color);
		plot_pixel(xc+y,yc-x,color);
		plot_pixel(xc-y,yc+x,color);
		plot_pixel(xc+y,yc+x,color);
		plot_pixel(xc-x,yc-y,color);
		plot_pixel(xc+x,yc-y,color);
		plot_pixel(xc-x,yc+y,color);
		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
	}
}

void top_arc_symb(int yc, int xc, int r, short int color) {
	int d=3-2*r;
	int x=0;
	int y=r;
	//int xc = 150;
	//int yc = 100;
	while(x<=y)
	{
		plot_pixel(xc+x,yc-y,color);
		plot_pixel(xc-x,yc-y,color);

		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
	}
}

void bottom_arc_symb(int yc, int xc, int r, short int color) {
	int d=3-2*r;
	int x=0;
	int y=r;
	//int xc = 150;
	//int yc = 100;
	while(x<=y)
	{
		plot_pixel(xc+x,yc+y,color);
		plot_pixel(xc-x,yc+y,color);

		if(d<=0)
		{
			d=d+4*x+6;
		}
		else
		{
			d=d+4*x-4*y+10;
			y=y-1;
		}
		x=x+1;
	}
}
