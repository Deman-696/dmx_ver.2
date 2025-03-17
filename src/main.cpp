#include "settings.h"
byte Ethernet::buffer[500];
uint8_t pinTx = 8;

uint8_t adr_head_1 = 20;//adress 1 
uint8_t adr_head_2 = 40;//adress 2
uint8_t adr_head_3 = 60;//adress 3 
uint8_t adr_head_4 = 2;//adress 4
uint8_t mas_head[4] = { adr_head_1, adr_head_2, adr_head_3, adr_head_4};
enum eName { HEAD_1, HEAD_2, HEAD_3, HEAD_4 };
enum eState {NONE_S, FUN_FACT_S, WAIT_ANSWER_S, RESULTS_S};
int state = NONE_S;
bool flagFirstFocus = false;

// const int adr = adr_head_1;//for debug

uint64_t timer_pulse = 0;
uint8_t pulse_period = 2;

// const int offset_rgb_dimmer = 0;
// const int offset_red_dimmer = 1;
// const int offset_green_dimmer = 2;
// const int offset_blue_dimmer = 3;
// const int offset_function_choose = 5;
// const int offset_function_speed = 6;

void setup() {
  Serial.begin(9600);
  dmx_setup();
  lan_init();
  timer_pulse = millis();
  Serial.println("____________________________");
  /*DmxSimple.write(mas_head[HEAD_1]+OFFSET_RED_DIMMER, 66); 
  while (1)
  {
    
    for (uint8_t i = 0; i < 250; i++){
    //set_collor_lightness(HEAD_1, i, i, i);
    DmxSimple.write(mas_head[HEAD_1]+OFFSET_RED_DIMMER, value); 
    DmxSimple.write(mas_head[HEAD_1]+OFFSET_RED_DIMMER, value); 
    //delay(100);
    }
    
    
  }*/
  
}

void loop() {
  ether.packetLoop(ether.packetReceive());

  //----------------------------TESTS----------------------------
  // DmxSimple.write(adr+offset_rgb_dimmer, UINT8_MAX);
  // DmxSimple.write(adr+offset_red_dimmer, UINT8_MAX); 
  // DmxSimple.write(adr+offset_green_dimmer, UINT8_MAX); 
  // DmxSimple.write(adr+offset_blue_dimmer, UINT8_MAX); 
  // DmxSimple.write(adr+offset_function_choose, 51); //51 - резкое включение, плавное затухание
  // DmxSimple.write(adr+offset_function_choose, 101); //101 - медлленное переключение между цветами, 150 -быстрое
  // DmxSimple.write(adr+offset_function_choose, 151);// 151 - быстрое градиентное включение и затухание разных сочетаний цветов по очереди 
  // DmxSimple.write(adr+offset_function_choose, 200);
  // DmxSimple.write(adr+offset_function_speed, 20);
  // delay(5000);
  // DmxSimple.write(adr+offset_function_speed, 100);
  // delay(5000);
  // DmxSimple.write(adr+offset_function_speed, 200);
  // delay(5000);
  // DmxSimple.write(adr+offset_function_speed, 250);
  // delay(5000);


//----------------------------state machine----------------------------
switch(state) {
    case NONE_S:
      break;
    case FUN_FACT_S:
      pulse();
      break;
    case WAIT_ANSWER_S:
      pulse();
      break;
    case RESULTS_S:
      pulse();
      break;
    default:
      break;  
  }
}

void dmx_setup()  {
  DmxSimple.usePin(pinTx);  //пин передачи данных
  DmxSimple.maxChannel(100); //максимальное количество каналов уменьшено, ускоряет передачу данных
  for(int i= 0; i< HEADS_COUNT; i++)
  {
    set_master_lightness(mas_head[i], MAX_LIGHTNESS);
    set_rgb(i, 0);
    // set_function(i, 0);
    // set_speed(i, 0);
  }
}

void set_speed(uint8_t head_num, uint8_t value)  {
  DmxSimple.write(mas_head[head_num]+OFFSET_FUNCTION_SPEED, value);
  delay(2);
}

void set_master_lightness(uint8_t head_num, uint8_t value)  {
  DmxSimple.write(mas_head[head_num]+OFFSET_RGB_DIMMER, value);
  // delay(2);
}

void set_collor_lightness(uint8_t head_num, uint8_t R_value, uint8_t G_value, uint8_t B_value)  {
  DmxSimple.write(mas_head[head_num]+OFFSET_RED_DIMMER, R_value); 
  DmxSimple.write(mas_head[head_num]+OFFSET_GREEN_DIMMER, G_value); 
  DmxSimple.write(mas_head[head_num]+OFFSET_BLUE_DIMMER, B_value); 
  // delay(2);
}

void set_red_lightness(uint8_t head_num, uint8_t value)  {
  DmxSimple.write(mas_head[head_num]+OFFSET_RED_DIMMER, value); 
}

void set_yellow_lightness(uint8_t head_num, uint8_t value)  {
  DmxSimple.write(mas_head[head_num]+OFFSET_RED_DIMMER, value); 
  DmxSimple.write(mas_head[head_num]+OFFSET_GREEN_DIMMER, value); 
  DmxSimple.write(mas_head[head_num]+OFFSET_BLUE_DIMMER, 0); 
  // delay(2);
}

void set_green_lightness(uint8_t head_num, uint8_t value)  {
  DmxSimple.write(mas_head[head_num]+OFFSET_GREEN_DIMMER, value); 
  // delay(2);
}

void set_blue_lightness(uint8_t head_num, uint8_t value) {
  DmxSimple.write(mas_head[head_num]+OFFSET_BLUE_DIMMER, value); 
  // delay(2);
}

void set_function(uint8_t head_num, uint8_t value) { 
  DmxSimple.write(mas_head[head_num]+OFFSET_FUNCTION_CHOOSE, value); 
  // delay(2);
}

void set_rgb(uint8_t head_num, uint8_t value) {//head_adress -> head_num?
  set_red_lightness(head_num, value);
  set_green_lightness(head_num, value);
  set_blue_lightness(head_num, value);
}

void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len) {
  char str[len + 1];
  strncpy(str, data, len);
  str[len] = 0; //null-terminator: end string

  //-------------------------- LIGHT --------------------------------
  //Serial.println(str);
  if (strcmp(str, "light=0") == 0)  heads_off();    //off all heads
  // if (strcmp(str, "light=1") == 0)  ;
  // if (strcmp(str, "light=2") == 0)  ;
  if (strcmp(str, "light=3") == 0)  heads_off();// wait_answer();

  // if (strcmp(str, "light=4") == 0)  results();    //itogi
  if (strcmp(str, "light=5") == 0)  mini_game();  //todo mini game
  if (strcmp(str, "light=6") == 0)  boss();       //todo boss
  if (strcmp(str, "light=7") == 0)  fun_fact();   //todo fun fact
  
  if (strcmp(str, "test") == 0)  test();  //todo mini game
  if (strcmp(str, "test=40") == 0)  test40();
  if (strcmp(str, "test=60") == 0)  test60();
  if (strcmp(str, "test=101") == 0)  test101();
  if (strcmp(str, "test=128") == 0)  test128();
  if (strcmp(str, "test=151") == 0)  test151();
  if (strcmp(str, "test=181") == 0)  test181();
  //-------------------------- SPOTS --------------------------------
  if (strcmp(str, "spot=1") == 0)   tumba_light(HEAD_1, YELLOW_COLOR);
  if (strcmp(str, "spot=2") == 0)   tumba_light(HEAD_2, YELLOW_COLOR);
  if (strcmp(str, "spot=3") == 0)   tumba_light(HEAD_3, YELLOW_COLOR);
  if (strcmp(str, "spot=4") == 0)   tumba_light(HEAD_4, YELLOW_COLOR);

  if (strcmp(str, "focus=1") == 0)   tumba_add_light(HEAD_1, YELLOW_COLOR);
  if (strcmp(str, "focus=2") == 0)   tumba_add_light(HEAD_2, YELLOW_COLOR);
  if (strcmp(str, "focus=3") == 0)   tumba_add_light(HEAD_3, YELLOW_COLOR);
  if (strcmp(str, "focus=4") == 0)   tumba_add_light(HEAD_4, YELLOW_COLOR);
}

void heads_off() {
  state = NONE_S;
  for (uint8_t i = 0; i < HEADS_COUNT; i++) {
    set_rgb(i, 0);
    set_function(i, 50);
    set_speed(i, 0);
    set_master_lightness(i, MAX_LIGHTNESS);
  }
  flagFirstFocus = false;
    
  // DmxSimple.write(mas_head[0]+OFFSET_RED_DIMMER, 0); 
  // DmxSimple.write(mas_head[0]+OFFSET_GREEN_DIMMER, 0);
  // DmxSimple.write(mas_head[0]+OFFSET_BLUE_DIMMER, 0);
  
  // DmxSimple.write(mas_head[1]+OFFSET_RED_DIMMER, 0);
  // DmxSimple.write(mas_head[1]+OFFSET_GREEN_DIMMER, 0);
  // DmxSimple.write(mas_head[1]+OFFSET_BLUE_DIMMER, 0);
  
  // DmxSimple.write(mas_head[2]+OFFSET_RED_DIMMER, 0);
  // DmxSimple.write(mas_head[2]+OFFSET_GREEN_DIMMER, 0);
  // DmxSimple.write(mas_head[2]+OFFSET_BLUE_DIMMER, 0);
  
  // DmxSimple.write(mas_head[3]+OFFSET_RED_DIMMER, 0);
  // DmxSimple.write(mas_head[3]+OFFSET_GREEN_DIMMER, 0);
  // DmxSimple.write(mas_head[3]+OFFSET_BLUE_DIMMER, 0);

  // DmxSimple.write(mas_head[0]+OFFSET_FUNCTION_CHOOSE, 0);
  // DmxSimple.write(mas_head[1]+OFFSET_FUNCTION_CHOOSE, 0);
  // DmxSimple.write(mas_head[2]+OFFSET_FUNCTION_CHOOSE, 0);
  // DmxSimple.write(mas_head[3]+OFFSET_FUNCTION_CHOOSE, 0);
}

void tumba_light(uint8_t head_num, uint8_t color)
{
  state = NONE_S;
  heads_off();
  
  switch (color) {
    case RED_COLOR:
      set_red_lightness(head_num, MAX_LIGHTNESS);
      break;
    case GREEN_COLOR:
      set_green_lightness(head_num, MAX_LIGHTNESS);
      break;
    case BLUE_COLOR:
      set_blue_lightness(head_num, MAX_LIGHTNESS);
      break;
    case YELLOW_COLOR:
      set_yellow_lightness(head_num, MAX_LIGHTNESS);
    break;
      default:
    break;
  }  
}

void tumba_add_light(uint8_t head_num, uint8_t color)
{
  if (flagFirstFocus == false) {
    heads_off();
    flagFirstFocus = true;
  }
  state = NONE_S;
  for(uint8_t i = 0; i < HEADS_COUNT; i++) // обнуляем режимы
  {
    set_function(i, 50);
    set_speed(i, 0);
    //set_master_lightness(i, MAX_LIGHTNESS);
  }
  
  switch (color) {
    case RED_COLOR:
      set_red_lightness(head_num, MAX_LIGHTNESS);
      break;
    case GREEN_COLOR:
      set_green_lightness(head_num, MAX_LIGHTNESS);
      break;
    case BLUE_COLOR:
      set_blue_lightness(head_num, MAX_LIGHTNESS);
      break;
    case YELLOW_COLOR:
      set_yellow_lightness(head_num, MAX_LIGHTNESS);
    break;
      default:
    break;
  }  
}

void wait_answer()
{
  heads_off();
  state = RESULTS_S;
  for (int i = 0; i < HEADS_COUNT; i++) { //яркость всех тумб в 255
    //добавить мягкую пульсацию
    set_master_lightness(i, MAX_LIGHTNESS);
    set_yellow_lightness(i, MAX_LIGHTNESS);
  }
}  

void results() {
  heads_off();
  state = RESULTS_S;
  for (int i = 0; i < HEADS_COUNT; i++) { //яркость всех тумб в 255
    set_master_lightness(i, MAX_LIGHTNESS);
    set_red_lightness(i, 0);
    set_green_lightness(i, 0);
    set_blue_lightness(i, MAX_LIGHTNESS);
  }
}

void mini_game()
{
  heads_off();
  state = NONE_S;
  for (int i = 0; i < HEADS_COUNT; i++) { //яркость всех тумб в 255
    set_master_lightness(i, MAX_LIGHTNESS);
    set_red_lightness(i, MAX_LIGHTNESS/2);
    set_blue_lightness(i, MAX_LIGHTNESS/2);
    set_green_lightness(i, 0);
  }
}

void reset_colors(uint8_t head_num)
{
  set_rgb(head_num, 0);
}

void boss()
{
  heads_off();
  state = NONE_S;
  set_function(HEAD_1, 150);
  set_function(HEAD_2, 150);
  set_function(HEAD_3, 150);
  set_function(HEAD_4, 150);
  set_speed(HEAD_1, 150);
  set_speed(HEAD_2, 150);
  set_speed(HEAD_3, 150);
  set_speed(HEAD_4, 150);

  // set_red_lightness(HEAD_1, MAX_LIGHTNESS);
  // set_red_lightness(HEAD_2, MAX_LIGHTNESS);
  // set_red_lightness(HEAD_3, MAX_LIGHTNESS);
  DmxSimple.write(mas_head[0]+OFFSET_RED_DIMMER, MAX_LIGHTNESS); 
  DmxSimple.write(mas_head[0]+OFFSET_GREEN_DIMMER, 0);
  DmxSimple.write(mas_head[0]+OFFSET_BLUE_DIMMER, 0);
  
  DmxSimple.write(mas_head[1]+OFFSET_RED_DIMMER, MAX_LIGHTNESS);
  DmxSimple.write(mas_head[1]+OFFSET_GREEN_DIMMER, 0);
  DmxSimple.write(mas_head[1]+OFFSET_BLUE_DIMMER, 0);
  
  DmxSimple.write(mas_head[2]+OFFSET_RED_DIMMER, MAX_LIGHTNESS);
  DmxSimple.write(mas_head[2]+OFFSET_GREEN_DIMMER, 0);
  DmxSimple.write(mas_head[2]+OFFSET_BLUE_DIMMER, 0);
  
  DmxSimple.write(mas_head[3]+OFFSET_RED_DIMMER, MAX_LIGHTNESS);
  DmxSimple.write(mas_head[3]+OFFSET_GREEN_DIMMER, 0);
  DmxSimple.write(mas_head[3]+OFFSET_BLUE_DIMMER, 0);
}

void fun_fact()
{
  heads_off();
  state = FUN_FACT_S;
  for(int i=0; i< HEADS_COUNT; i++)
  {
    // DmxSimple.write(i+OFFSET_FUNCTION_CHOOSE, 120); 
    set_master_lightness(i, MAX_LIGHTNESS/2);
    set_blue_lightness(i, MAX_LIGHTNESS);
    // set_function(i, 0);
    // set_rgb(i, 150);
    // set_red_lightness(i, MAX_LIGHTNESS);
  }
}


void test()
{
  for(int i=0; i< HEADS_COUNT; i++)
  {
    set_master_lightness(i, MAX_LIGHTNESS);
    set_red_lightness(i, MAX_LIGHTNESS);
  }
 delay(5000);
 for(int i=0; i< HEADS_COUNT; i++)
 {
   set_rgb(i, 0);
   set_master_lightness(i, MAX_LIGHTNESS);
   set_green_lightness(i, MAX_LIGHTNESS);
  }
delay(5000);
for(int i=0; i< HEADS_COUNT; i++)
{
  set_rgb(i, 0);
  set_master_lightness(i, MAX_LIGHTNESS);
  set_blue_lightness(i, MAX_LIGHTNESS);
}
delay(5000);

for(int i=0; i< HEADS_COUNT; i++) {
  set_rgb(i, 255);
  set_function(i, 40);//0-50 stroboflas and light dimmer
  set_speed(i, 50);
  delay(5000);

  set_function(i, 120);//101-150 gradients change
  delay(5000);
  
  set_function(i, 200);//151-200 pulse change
  delay(5000);
  }
}


void lan_init() {
  Serial.println("LAN init..");
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
  /*if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));*/
  
  ether.staticSetup(myip, gwip);
  ether.printIp("IP:  ", ether.myip);
  ether.udpServerListenOnPort(&udpSerialPrint, 4444);
}

void test40()
{
  for(int i=0; i< HEADS_COUNT; i++)
  {
    set_master_lightness(i, MAX_LIGHTNESS);
    set_rgb(i, MAX_LIGHTNESS);
    set_function(i, 40);//0-50 stroboflas and light dimmer
    set_speed(i, 150);
  }
}

void test60()
{
  for(int i=0; i< HEADS_COUNT; i++)
  {
    set_master_lightness(i, MAX_LIGHTNESS);
    set_rgb(i, MAX_LIGHTNESS);
    set_function(i, 60);//0-50 stroboflas and light dimmer
    set_speed(i, 150);
  }
}

void test101()
{
  for(int i=0; i< HEADS_COUNT; i++)
  {
    set_master_lightness(i, MAX_LIGHTNESS);
    set_rgb(i, MAX_LIGHTNESS);
    set_function(i, 101);//101-150 gradients change
    set_speed(i, 150);
  }
}

void test128()
{
  for(int i=0; i< HEADS_COUNT; i++)
  {
    set_master_lightness(i, MAX_LIGHTNESS);
    set_rgb(i, MAX_LIGHTNESS);
    set_function(i, 128);//101-150 gradients change
    set_speed(i, 150);
  }
}

void test151()
{
  for(int i=0; i< HEADS_COUNT; i++)
  {
    set_master_lightness(i, MAX_LIGHTNESS);
    set_rgb(i, MAX_LIGHTNESS);
    set_function(i, 151);//101-150 gradients change
    set_speed(i, 150);
  }
}

void test181()
{
  for(int i=0; i< HEADS_COUNT; i++)
  {
    set_master_lightness(i, MAX_LIGHTNESS);
    set_rgb(i, MAX_LIGHTNESS);
    set_function(i, 181);//101-150 gradients change
    set_speed(i, 150);
  }
}

void pulse()
{
  static uint8_t brightness = 0;
  static bool dir = 1;
  if(millis() > timer_pulse + pulse_period)
  {
    dir ? brightness++ : brightness--;

    if (brightness >= MAX_LIGHTNESS || brightness == 0) 
      dir = !dir;
    
    for(int i=0; i< HEADS_COUNT; i++)
      set_master_lightness(i, brightness);

    timer_pulse = millis();
  }
}

void pulse(uint8_t _prd)
{
  static uint8_t brightness = 0;
  static bool dir = 1;
  if(millis() > timer_pulse + _prd)
  {
    dir ? brightness++ : brightness--;

    if (brightness >= MAX_LIGHTNESS || brightness == 0) 
      dir = !dir;
    
    for(int i=0; i< HEADS_COUNT; i++)
      set_master_lightness(i, brightness);

    timer_pulse = millis();
  }
}