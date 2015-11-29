#include <pebble.h>

Window *window;
TextLayer *NormTime_layer;
TextLayer *date_layer;
TextLayer *ArmyTime_layer;
//InverterLayer *inverter_layer;

/*
* This is the event handler for timechanges
*/
void handle_timechanges(struct tm *tick_time, TimeUnits units_changed)
{
	// Buffers used for size of date and time
  static char NormalTime_buffer[40];
	static char ArmyTime_buffer[12];
  static char date_buffer[22];
	// orignial of 12
	// Normal Time
  strftime(NormalTime_buffer, sizeof(NormalTime_buffer), "%I:%M ", tick_time);
  text_layer_set_text(NormTime_layer, NormalTime_buffer);
	
	//Army Time
	strftime(ArmyTime_buffer, sizeof(ArmyTime_buffer), "%H:%M:%S %p\n", tick_time);
  text_layer_set_text(ArmyTime_layer, ArmyTime_buffer);
	
  //
  strftime(date_buffer, sizeof(date_buffer), "%a, %b %e\n%x", tick_time);
  text_layer_set_text(date_layer, date_buffer);
}
void handle_init(void) {
	// Create a window and text layer
	window = window_create();
	
	// ArmyTime_layer, creating, setting font, aligning it, adding to window
	ArmyTime_layer = text_layer_create(GRect(0, 0, 144, 30));
	text_layer_set_font(ArmyTime_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(ArmyTime_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(ArmyTime_layer));

	// NormTime_layer, creating, setting font, aligning it, adding to window
	NormTime_layer = text_layer_create(GRect(0, 30, 144, 100));
	//RESOURCE_ID_FONT_OSP_DIN_70 
	//RESOURCE_ID_FONT_DATA_LATIN_55
	GFont custom_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DATA_LATIN_55));
	text_layer_set_font(NormTime_layer, custom_font);
	text_layer_set_text_alignment(NormTime_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(NormTime_layer));
	
  // date_layer, creating, setting font, aligning, adding to window.
  date_layer = text_layer_create(GRect(0, 105, 144, 60));
	
	text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
	
	// This is for use of the inverter
	//inverter_layer = inverter_layer_create(GRect(0, 30, 144, 80));
	//layer_add_child(window_get_root_layer(window), inverter_layer_get_layer(inverter_layer));

	// TickTimer subscribing
  tick_timer_service_subscribe(SECOND_UNIT, handle_timechanges);
  
	//graphics_draw_line(window_get_root_layer(window), GPoint p0, GPoint p1)
	
	// Push the window
	window_stack_push(window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Watchface properly displayed.");
}

void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(NormTime_layer);
  text_layer_destroy(date_layer);
	text_layer_destroy(ArmyTime_layer);
	//inverter_layer_destroy(inverter_layer);
	
	// Destroy the window
	window_destroy(window);
  
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
