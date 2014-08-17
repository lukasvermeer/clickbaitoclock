// One weird trick to tell time. You won't believe what happens next!
// Nothing could prepare you for this inconcievable innovation!
// This incredible program will totally blow your mind!
// We made a watchface that tells time in an amazing way. Here's how.
// Time Lords hate us!

// Hundreds of lines of code you won't believe actually work!
// The string concatenation and switch case statements below will shock you!

#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static GFont s_time_font;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  //tick_time->tm_min = 0; // HAX FOR TESTING
  //tick_time->tm_min = 1; // HAX FOR TESTING

  static char minute[] = "00";
  snprintf(minute, sizeof(minute), "%d", tick_time->tm_min);

  static char hour[] = "twelve";  
  switch(tick_time->tm_hour) {
    case 1  : strcpy(hour, "one"); break;
    case 2  : strcpy(hour, "two"); break;
    case 3  : strcpy(hour, "three"); break;
    case 4  : strcpy(hour, "four"); break;
    case 5  : strcpy(hour, "five"); break;
    case 6  : strcpy(hour, "six"); break;
    case 7  : strcpy(hour, "seven"); break;
    case 8  : strcpy(hour, "eight"); break;
    case 9  : strcpy(hour, "nine"); break;
    case 10 : strcpy(hour, "ten"); break;
    case 11 : strcpy(hour, "eleven"); break;
    case 12 : strcpy(hour, "twelve"); break;
    default : strcpy(hour, "unknown"); break;
  }
  
  // Create a long-lived buffer
  static char buffer[64];
  
  switch(tick_time->tm_min) {
    case 0:
      snprintf(buffer, 64, "It is %s o'clock and you won't believe what happens next!", hour); break;
    case 1:
      snprintf(buffer, 64, "%s minute you won't believe has passed since %s o'clock!", minute, hour); break;
    default :
      snprintf(buffer, 64, "%s minutes you won't believe have passed since %s o'clock!", minute, hour); break;
  }
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void main_window_load(Window *window) {
  //Create time TextLayer
  s_time_layer = text_layer_create(GRect(4, 4, 136, 160));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}