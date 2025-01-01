#include "scene_end.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include "scene_menu_object.h"
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"

static ALLEGRO_SAMPLE_ID endBGM;
static Button btnSettings;
extern ALLEGRO_SAMPLE* settingsMusic;
extern ALLEGRO_SAMPLE* LoseMusic;
extern ALLEGRO_SAMPLE* themeVictory;
static ALLEGRO_BITMAP* pika = NULL;
static ALLEGRO_BITMAP* ghst3 = NULL;
static ALLEGRO_BITMAP* ghst2 = NULL;
static ALLEGRO_BITMAP* ghst1 = NULL;
static ALLEGRO_BITMAP* ghst = NULL;
extern int game_main_Score;
extern int game_high_Score;
bool game_over;
bool game_victory;
ALLEGRO_TIMER* anima_timer;
static int anima = 0;
static int anima1 = 60;
static int animaSize = 60;
static int animaSize2 = 40;

//----------

static void init() {
	stop_bgm(endBGM);
	if (!game_over) {
    endBGM = play_bgm(themeVictory, music_volume);
	}
	else{
    endBGM = play_bgm(LoseMusic, music_volume);
	}
	btnSettings = button_create(730, 20, 50, 50, "Assets/settings.png", "Assets/settings2.png");
	pika = load_bitmap("Assets/pikachu.png");
	ghst3 = load_bitmap("Assets/ghost_move_pink.png");
	ghst2 = load_bitmap("Assets/ghost_move_orange.png");
	ghst1 = load_bitmap("Assets/ghost_move_red.png");
	ghst = load_bitmap("Assets/ghost_move_blue.png");

	anima_timer = al_create_timer(0.1f);
	if (!anima_timer)
		game_abort("Error on create timer\n");
	al_start_timer(anima_timer);
}

static void draw_lose(void ){
	al_clear_to_color(al_map_rgb(30, 20, 40));

	drawButton(btnSettings);

	al_draw_text(
		menuFont1,
		al_map_rgb(255, 0, 0),
		SCREEN_W / 2,
		SCREEN_H / 2 - 50,
		ALLEGRO_ALIGN_CENTER,
		"YOU DIED!");

	/*al_draw_textf(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 130,
		ALLEGRO_ALIGN_CENTER,
		"Your Score : %d", game_main_Score);

	al_draw_textf(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 100,
		ALLEGRO_ALIGN_CENTER,
		"High Score : %d", game_high_Score);*/

	al_draw_text(
		menuFont,
		al_map_rgb(180, 180, 180),
		SCREEN_W / 2,
		SCREEN_H - 50,
		ALLEGRO_ALIGN_CENTER,
		"(press esc to go back to menu)");
}

static void draw_win(void) {
	al_clear_to_color(al_map_rgb(20, 15, 30));

	drawButton(btnSettings);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H / 2 - 50,
		ALLEGRO_ALIGN_CENTER,
		"YOU WIN!");

	al_draw_textf(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 130,
		ALLEGRO_ALIGN_CENTER,
		"Your Score : %d", game_main_Score);

	al_draw_textf(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 100,
		ALLEGRO_ALIGN_CENTER,
		"High Score : %d", game_high_Score);

	al_draw_text(
		menuFont,
		al_map_rgb(180, 180, 180),
		SCREEN_W / 2,
		SCREEN_H - 50,
		ALLEGRO_ALIGN_CENTER,
		"(press esc to go back to menu)");

	anima = (al_get_timer_count(anima_timer) % 2) ? 0 : 16;
	anima1 = (al_get_timer_count(anima_timer) % 2) ? 0 : 60;

	al_draw_scaled_bitmap(
		ghst1,
		0 + anima, 0,
		16, 16,
		al_get_timer_count(anima_timer)*4 - 70, (SCREEN_H / 2 - 100),
		animaSize, animaSize,
		0
	);

	al_draw_scaled_bitmap(
		ghst,
		0 + anima, 0,
		16, 16,
		al_get_timer_count(anima_timer) * 4 - 160, (SCREEN_H / 2 - 100),
		animaSize, animaSize,
		0
	);

		al_draw_scaled_bitmap(
		ghst2,
		0 + anima, 0,
		16, 16,
		al_get_timer_count(anima_timer) * 4 - 250, (SCREEN_H / 2 - 100),
		animaSize, animaSize,
		0
	);

		al_draw_scaled_bitmap(
		ghst3,
		0 + anima, 0,
		16, 16,
		al_get_timer_count(anima_timer) * 4 - 340, (SCREEN_H / 2 - 100),
		animaSize, animaSize,
		0
	);

	al_draw_scaled_bitmap(
		pika,
		0 + anima1, 0,
		60, 40,
		al_get_timer_count(anima_timer) * 4 - 450, (SCREEN_H / 2 - 100),
		80, 60,
		0
	);


}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	btnSettings.hovered = buttonHover(btnSettings, mouse_x, mouse_y);
}

static void on_mouse_down() {
	if (btnSettings.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		game_change_scene(scene_settings_create());
}


static void destroy() {
	stop_bgm(endBGM);
	al_destroy_bitmap(btnSettings.default_img);
	al_destroy_bitmap(btnSettings.hovered_img);
    al_destroy_bitmap(pika);
	al_destroy_bitmap(ghst3);
	al_destroy_bitmap(ghst2);
	al_destroy_bitmap(ghst1);
	al_destroy_bitmap(ghst);
	al_stop_timer(anima_timer);
	al_destroy_timer(anima_timer);
}

static void on_key_down(int keycode) {
	switch (keycode) {
	case ALLEGRO_KEY_ESCAPE:
		game_change_scene(scene_menu_create());
		break;
	default:
		break;
	}
}


// The only function that is shared across files.
Scene scene_end_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "End";
	scene.initialize = &init;
	scene.draw = (game_over)? &draw_lose : &draw_win;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("End scene created");
	return scene;
}
