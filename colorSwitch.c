
	/* This files provides address values that exist in the system */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BOARD                 "DE1-SoC"

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

#define GROUND 200
#define GRAVITY 5.5
#define LIFT 30

short int circle_array[15][30] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x08, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x62, 0x10, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x62, 0x10, 0x00, 0x00,
  0x41, 0x08, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x41, 0x08,
  0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
  0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xdf, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
  0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x82, 0x10,
  0x00, 0x00, 0x62, 0x10, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x62, 0x10, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x62, 0x10, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x08, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

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

int main(void)
{
    pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;

	clear_screen();

	int y_obstacle = 50; //y coordinate of obstacle
	//int y1 = 50;
	int r_obstacle = 40; //radius of obstacle


	int y_player = 200; //y coordinate of obstacle
	int r_player = 5; //radius of player
	int velY = 0;

	int game_over = 0; //initialize game over to be false

  	//int dir = 1;

	volatile int * PS2_ptr = (int *) 0xFF200100;  // PS/2 port address
	int PS2_data, RVALID;
	int keyPress;
	char byte1 = 0, byte2 = 0;

    *(PS2_ptr) = 0xFF; //reset


  	while(!game_over) {
		PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port


		//draw_circ(y_obstacle, r_obstacle, 0x10ff);
		right_down_arc(y_obstacle, r_obstacle, 0x10ff);
		right_up_arc(y_obstacle, r_obstacle, 0xFFE0);
		left_down_arc(y_obstacle, r_obstacle, 0xF800);
		left_up_arc(y_obstacle, r_obstacle, 0xC618);
		if (y_player + r_player > y_obstacle + r_obstacle)
			draw_circ(y_player, r_player, 0xffff);
		else
			draw_circ(y_player, r_player, 0xFC18);
		//draw_(y_obstacle);
		wait_cycle();
		//draw_black(y_obstacle);
		//draw_circ(y_obstacle, r_obstacle, 0x0000);
		right_down_arc(y_obstacle, r_obstacle, 0x0000);
		right_up_arc(y_obstacle, r_obstacle, 0x0000);
		left_down_arc(y_obstacle, r_obstacle, 0x0000);
		left_up_arc(y_obstacle, r_obstacle, 0x0000);
		draw_circ(y_player, r_player, 0x0000);


		y_player += velY + GRAVITY;
		velY = velY*0.8;
		if (y_player >= GROUND)
			y_player = GROUND;


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

void plot_pixel(int x, int y, short int line_color)
{
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

/*
void draw_circ(int yc, int r, short int color) {
	int d=3-2*r;
	int x=0;
	int y=r;
	int xc = 150;
	//int yc = 100;
	while(1){
		while(x<=y)
		{
			//plot_pixel(xc+x,yc+y,color1);
			plot_pixel(xc-y,yc-x,color2);
			plot_pixel(xc+y,yc-x,color3);
			plot_pixel(xc-y,yc+x,color4);
			//plot_pixel(xc+y,yc+x,color5);
			plot_pixel(xc-x,yc-y,color6);
			plot_pixel(xc+x,yc-y,color7);
			plot_pixel(xc-x,yc+y,color8);
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
}
*/