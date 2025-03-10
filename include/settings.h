#ifndef SETTINGS_H
#define SETTINGS_H

#include <DmxSimple.h>
#include <EEPROM.h>
#include <EtherCard.h>
#include <IPAddress.h>

static byte mymac[] = { 0x00, 0x57, 0x1E, 0xDE, 0xCE, 0x57 };
static byte myip[] = { 192,168,0,120 };
static byte gwip[] = { 192,168,0,1 };


// #define clr_WHITE 0
// #define clr_RED 10
// #define clr_GREEN 20
// #define clr_BLUE 30
// #define clr_YELLOW 40
// #define clr_ORANGE 50
// #define clr_INDIGO 60
// #define clr_PINK 70

#define OFFSET_RGB_DIMMER     0
#define OFFSET_RED_DIMMER     1
#define OFFSET_GREEN_DIMMER   2
#define OFFSET_BLUE_DIMMER    3
#define OFFSET_FUNCTION_CHOOSE  5
#define OFFSET_FUNCTION_SPEED   6

#define HEADS_COUNT     4
#define MAX_LIGHTNESS   254

#define RED_COLOR       66
#define GREEN_COLOR     77
#define BLUE_COLOR      88
#define YELLOW_COLOR    99



//prototypes
void print();
void test_center();

void lan_init();
void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, char *data, uint16_t len);
void write_eeprom();
void read_eeprom();

void set_speed(uint8_t head_num, uint8_t value);
void set_master_lightness(uint8_t head_num, uint8_t value);
void set_red_lightness(uint8_t head_num, uint8_t value);
void set_green_lightness(uint8_t head_num, uint8_t value);
void set_blue_lightness(uint8_t head_num, uint8_t value);
void set_rgb(uint8_t head_num, uint8_t value);
void set_function(uint8_t head_num, uint8_t value);
void dmx_setup();
void pulse();
void pulse(uint8_t _prd);

void cmd_off();
void heads_off();
void wait_answer();
void results();
void mini_game();
void boss();
void fun_fact();

void tumba_light(uint8_t head_num, uint8_t color);
void tumba_add_light(uint8_t head_num, uint8_t color);
void set_red_lightness(uint8_t head_num, uint8_t value);
void set_blue_lightness(uint8_t head_num, uint8_t value);
void set_green_lightness(uint8_t head_num, uint8_t value);
void set_yellow_lightness(uint8_t head_num, uint8_t value);
//сброс всех цветов
void reset_colors(uint8_t head_num);


void test();
void test40();
void test60();
void test101();
void test128();
void test151();
void test181();
#endif