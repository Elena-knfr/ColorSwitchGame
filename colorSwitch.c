
	/* This files provides address values that exist in the system */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
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

int GROUND = 230;
#define GRAVITY 6
#define LIFT 20
#define	NUM_OBSTACLES 3

volatile int pixel_buffer_start; // global variable
volatile int * pixel_ctrl_ptr;
short int WHITE = 0xFFFF;

//void draw_circle();
void plot_pixel(int x, int y, short int line_color);
void swap(int* x, int* y);
void draw_line(int x0, int y0, int x1, int y1, short int color);
void clear_screen();
void wait_cycle();
//void draw_();
void draw_circ(int yc, int r, short int color);
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
void runGame(int y0, int y1, int y2, int r0, int r1, int r2, int y_player, int r_player);

int main(void)
{
    pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;

	  clear_screen();

	int y_obstacle [NUM_OBSTACLES] = {50, -70, -190}; //y coordinate of obstacle
	int r_obstacle [NUM_OBSTACLES] = {40, 30, 50}; //radius of obstacle

	int y_player = 200; //y coordinate of obstacle
	int r_player = 5; //radius of player
	int velY = 0;
	short int color_player = 0xffff;

	int game_over = 0; //initialize game over to be false

	volatile int * PS2_ptr = (int *) 0xFF200100;  // PS/2 port address
	int PS2_data, RVALID;
	int keyPress;
	char byte1 = 0, byte2 = 0;

    *(PS2_ptr) = 0xFF; //reset
/*
	right_down_arc(y_obstacle, r_obstacle, 0x10ff); //blue
	left_down_arc(y_obstacle, r_obstacle, 0xF800); //red
	left_up_arc(y_obstacle, r_obstacle, 0xC618); //grey
	right_up_arc(y_obstacle, r_obstacle, 0xFFE0); //yellow*/

  	while(!game_over) {
		PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port

		runGame(y_obstacle[0], y_obstacle[1], y_obstacle[2],
				r_obstacle[0], r_obstacle[1], r_obstacle[2], y_player, r_player);

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

		RVALID = PS2_data & 0x8000;
	  	if (RVALID){
			byte1 = byte2;
			byte2 = PS2_data & 0xFF;;
			if (byte2 == 0x29 && byte1 != 0xF0) { // if space is pressed
				velY = -LIFT;
			}
		}



	  /*
		if (y_obstacle + r_obstacle < 200 - 12) {
			draw_circ(y_obstacle, r_obstacle, 0x10ff);
			draw_circ(y_player, r_player, 0xffff);
			//draw_(y_obstacle);
			wait_cycle();
			//draw_black(y_obstacle);
			draw_circ(y_obstacle, r_obstacle, 0x0000);
			draw_circ(y_player, r_player, 0x0000);
		}
		else {
			draw_circ(y_obstacle, r_obstacle, 0x10ff);
			draw_circ(y_player, r_player, 0x10ff);
			wait_cycle();
			draw_circ(y_obstacle, r_obstacle, 0x0000);
			draw_circ(y_player, r_player, 0x0000);
		}*/
	}
}

void runGame(int y0, int y1, int y2, int r0, int r1, int r2, int y_player, int r_player) {

		draw_circle_obstacle(y0, r0, BLUE, RED, GREY, YELLOW);
		draw_circle_obstacle(y1, r1, BLUE, RED, GREY, YELLOW);
		draw_circle_obstacle(y2, r2, BLUE, RED, GREY, YELLOW);
/*
		if (y_player + r_player > y_obstacle[0] + r_obstacle[0])
			draw_circ(y_player, r_player, 0xffff);
		else if (y_player + r_player > y_obstacle[1] + r_obstacle[1] && y_player + r_player < y_obstacle[0] + r_obstacle[0])
			draw_circ(y_player, r_player, RED);
		else if (y_player + r_player > y_obstacle[2] + r_obstacle[2] && y_player + r_player < y_obstacle[1] + r_obstacle[1])
			draw_circ(y_player, r_player, YELLOW);
		else {
			draw_circ(y_player, r_player, 0xFC18);
			//color_player = 0xFC18;
		}
*/

		wait_cycle();

		draw_circle_obstacle(y0, r0, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle(y1, r1, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle(y2, r2, 0x0000, 0x0000, 0x0000, 0x0000);

		//draw objects black to erase them

		draw_circ(y_player, r_player, 0x0000);

		wait_cycle();
		delay(0.01);

		draw_circle_obstacle_shifted_color(y0, r0, BLUE, RED, GREY, YELLOW);
		draw_circle_obstacle_shifted_color(y1, r1, BLUE, RED, GREY, YELLOW);
		draw_circle_obstacle_shifted_color(y2, r2, BLUE, RED, GREY, YELLOW);

		wait_cycle();
		//delay(0.01);

		draw_circle_obstacle_shifted_color(y0, r0, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle_shifted_color(y1, r1, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle_shifted_color(y2, r2, 0x0000, 0x0000, 0x0000, 0x0000);



		wait_cycle();
		delay(0.01);

		draw_circle_obstacle(y0, r0, YELLOW, BLUE, RED, GREY);
		draw_circle_obstacle(y1, r1, YELLOW, BLUE, RED, GREY);
		draw_circle_obstacle(y2, r2, YELLOW, BLUE, RED, GREY);

		wait_cycle();
		//delay(0.01);

		draw_circle_obstacle(y0, r0, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle(y1, r1, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle(y2, r2, 0x0000, 0x0000, 0x0000, 0x0000);

		wait_cycle();
		delay(0.01);

		draw_circle_obstacle_shifted_color(y0, r0, YELLOW, BLUE, RED, GREY);
		draw_circle_obstacle_shifted_color(y1, r1, YELLOW, BLUE, RED, GREY);
		draw_circle_obstacle_shifted_color(y2, r2, YELLOW, BLUE, RED, GREY);

		wait_cycle();
		//delay(0.01);

		draw_circle_obstacle_shifted_color(y0, r0, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle_shifted_color(y1, r1, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle_shifted_color(y2, r2, 0x0000, 0x0000, 0x0000, 0x0000);


		//delay(0.1);
		wait_cycle();
		delay(0.01);

		draw_circle_obstacle(y0, r0, GREY, YELLOW, BLUE, RED);
		draw_circle_obstacle(y1, r1, GREY, YELLOW, BLUE, RED);
		draw_circle_obstacle(y2, r2, GREY, YELLOW, BLUE, RED);

		wait_cycle();
		//delay(0.01);

		draw_circle_obstacle(y0, r0, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle(y1, r1, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle(y2, r2, 0x0000, 0x0000, 0x0000, 0x0000);

		wait_cycle();
		delay(0.01);

		draw_circle_obstacle_shifted_color(y0, r0, GREY, YELLOW, BLUE, RED);
		draw_circle_obstacle_shifted_color(y1, r1, GREY, YELLOW, BLUE, RED);
		draw_circle_obstacle_shifted_color(y2, r2, GREY, YELLOW, BLUE, RED);

		wait_cycle();
		//delay(0.01);

		draw_circle_obstacle_shifted_color(y0, r0, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle_shifted_color(y1, r1, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle_shifted_color(y2, r2, 0x0000, 0x0000, 0x0000, 0x0000);

		//delay(0.1);
		wait_cycle();
		delay(0.01);

		draw_circle_obstacle(y0, r0, RED, GREY, YELLOW, BLUE);
		draw_circle_obstacle(y1, r1, RED, GREY, YELLOW, BLUE);
		draw_circle_obstacle(y2, r2, RED, GREY, YELLOW, BLUE);

		wait_cycle();
		//delay(0.01);

		draw_circle_obstacle(y0, r0, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle(y1, r1, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle(y2, r2, 0x0000, 0x0000, 0x0000, 0x0000);

		wait_cycle();
		delay(0.01);

		draw_circle_obstacle_shifted_color(y0, r0, RED, GREY, YELLOW, BLUE);
		draw_circle_obstacle_shifted_color(y1, r1, RED, GREY, YELLOW, BLUE);
		draw_circle_obstacle_shifted_color(y2, r2, RED, GREY, YELLOW, BLUE);

		wait_cycle();
		//delay(0.01);

		draw_circle_obstacle_shifted_color(y0, r0, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle_shifted_color(y1, r1, 0x0000, 0x0000, 0x0000, 0x0000);
		draw_circle_obstacle_shifted_color(y2, r2, 0x0000, 0x0000, 0x0000, 0x0000);
		//wait_cycle();
		//delay(0.1);
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

void spinningCircles(int y, int r, short int color1, short int color2, short int color3, short int color4) {
	draw_circle_obstacle(y, r, color1, color2, color3, color4);
	delay(0.1);
	draw_circle_obstacle_shifted_color(y, r, color1, color2, color3, color4);
	delay(0.1);

	draw_circle_obstacle(y, r, color4, color1, color2, color3);
	delay(0.1);
	draw_circle_obstacle_shifted_color(y, r, color4, color1, color2, color3);
	delay(0.1);

	draw_circle_obstacle(y, r, color3, color4, color1, color2);
	delay(0.1);
	draw_circle_obstacle_shifted_color(y, r, color3, color4, color1, color2);
	delay(0.1);

	draw_circle_obstacle(y, r, color2, color3, color4, color1);
	delay(0.1);
	draw_circle_obstacle_shifted_color(y, r, color2, color3, color4, color1);
	delay(0.1);
}
