// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include "scene_settings.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include "scene_menu_object.h"
#include "scene_end.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.
static int anim = 0;
static int animash = 0;
static ALLEGRO_BITMAP* gameTitle = NULL;
static int gameTitleW ;
static int gameTitleH ;
static Button btnToMenu;
static Button btnBGML;
static Button btnBGMR;
static Button btnBGML2;
static Button btnBGMR2;
static Button btnSkin1;
static Button btnSkin2;
static Button btnApply;
static ALLEGRO_SAMPLE_ID settingsBGM;
static ALLEGRO_SAMPLE_ID sfxTest;
static ALLEGRO_BITMAP* skin1 = NULL;
static ALLEGRO_BITMAP* skin2 = NULL;
static ALLEGRO_BITMAP* skin3 = NULL;
static int animX = 385;
static int animY = 500;
static int animSize = 60;
static int textX = 320;
static int textY = 450;

extern ALLEGRO_SAMPLE* settingsMusic;
extern ALLEGRO_SAMPLE* PACMAN_EATPUP;
ALLEGRO_TIMER* anim_timer;
extern int pacman_skin;

static void init() {
	btnToMenu = button_create(730, 20, 50, 50, "Assets/settings.png", "Assets/settings2.png");
	btnBGML = button_create(197, SCREEN_H/2-332, 90, 90, "Assets/left_arrow_btn.png", "Assets/left_arrow_btn_hover.png");
	btnBGMR = button_create(530, SCREEN_H/2-332, 90, 90, "Assets/right_arrow_btn.png", "Assets/right_arrow_btn_hover.png");
	btnBGML2 = button_create(197, SCREEN_H/2-183, 90, 90, "Assets/left_arrow_btn.png", "Assets/left_arrow_btn_hover.png");
	btnBGMR2 = button_create(530, SCREEN_H/2-183, 90, 90, "Assets/right_arrow_btn.png", "Assets/right_arrow_btn_hover.png");
	btnSkin1 = button_create(197, 415, 90, 90, "Assets/left_arrow_btn.png", "Assets/left_arrow_btn_hover.png");
	btnSkin2 = button_create(530, 415, 90, 90, "Assets/right_arrow_btn.png", "Assets/right_arrow_btn_hover.png");
	btnApply = button_create(SCREEN_W-180, SCREEN_H-180, 180, 180, "Assets/apply_btn.png", "Assets/apply_btn_hover.png");
	skin1 = load_bitmap("Assets/pacman_move.png");
	skin2 = load_bitmap("Assets/ash_move.png");
	stop_bgm(settingsBGM);
	settingsBGM = play_bgm(settingsMusic, music_volume + 2);

    gameTitle = load_bitmap("Assets/groudonvkyogre.png");
	gameTitleW = al_get_bitmap_width(gameTitle);
	gameTitleH = al_get_bitmap_height(gameTitle);

	anim_timer = al_create_timer(1.0f);
	if (!anim_timer)
		game_abort("Error on create timer\n");
	al_start_timer(anim_timer);
}

static void draw(void ){
	al_clear_to_color(al_map_rgb(0, 0, 0));

	const float scale = 0.6;
	const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * gameTitleW;
	const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * gameTitleH;

	//draw title image
	al_draw_scaled_bitmap(
		gameTitle,
		0, 0,
		gameTitleW, gameTitleH,
		0, 400,
		gameTitleW * scale, gameTitleH * scale,
		0
	);

	al_draw_textf(
		menuFont,
		al_map_rgb(255, 255, 255),
		287,
		SCREEN_H/2-300,
		ALLEGRO_ALIGN_LEFT,
		"BGM Volume : %0.1f", music_volume);

	al_draw_textf(
		menuFont,
		al_map_rgb(255, 255, 255),
		290,
		SCREEN_H/2-150,
		ALLEGRO_ALIGN_LEFT,
		"SFX Volume : %0.1f", effect_volume);

	drawButton(btnToMenu);
	drawButton(btnBGML);
	drawButton(btnBGMR);
	drawButton(btnBGML2);
	drawButton(btnBGMR2);
	drawButton(btnSkin1);
	drawButton(btnSkin2);
	drawButton(btnApply);

	anim = (al_get_timer_count(anim_timer) % 2) ? 0 : 16;
	animash = (al_get_timer_count(anim_timer) % 2) ? 0 : 33;
	switch (pacman_skin)
	{
	case 1:
		al_draw_scaled_bitmap(
			skin1,
			0 + anim, 0,
			16, 16,
			animX, animY,
			animSize, animSize,
			0
		);
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			textX, textY,
			ALLEGRO_ALIGN_LEFT,
			"Default skin");
		break;
	case 2:
		al_draw_scaled_bitmap(
			skin2,
			0 + animash, 0,
			33, 40,
			animX, animY,
			animSize, animSize,
			0
		);
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			textX, textY,
			ALLEGRO_ALIGN_LEFT,
			"Ash Ketchum");
		break;
	default:
		break;
	}
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	btnToMenu.hovered = buttonHover(btnToMenu, mouse_x, mouse_y);
	btnBGML.hovered = buttonHover(btnBGML, mouse_x, mouse_y);
	btnBGMR.hovered = buttonHover(btnBGMR, mouse_x, mouse_y);
	btnBGML2.hovered = buttonHover(btnBGML2, mouse_x, mouse_y);
	btnBGMR2.hovered = buttonHover(btnBGMR2, mouse_x, mouse_y);
	btnSkin1.hovered = buttonHover(btnSkin1, mouse_x, mouse_y);
	btnSkin2.hovered = buttonHover(btnSkin2, mouse_x, mouse_y);
	btnApply.hovered = buttonHover(btnApply, mouse_x, mouse_y);
}

static void on_mouse_down() {
	if (btnToMenu.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		game_change_scene(scene_menu_create());
	if (btnApply.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		stop_bgm(settingsBGM);
		settingsBGM = play_bgm(settingsMusic, music_volume);}
	if (btnBGML.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		if (music_volume != 0) music_volume -= 1;
		game_log("music down\n");}
	if (btnBGMR.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		if (music_volume != 10) music_volume += 1;
		game_log("music up\n");}
	if (btnBGML2.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		if (effect_volume != 0) effect_volume -= 1;
		game_log("effect down\n");
		stop_bgm(sfxTest);
		sfxTest = play_audio(PACMAN_EATPUP, effect_volume);}
	if (btnBGMR2.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		if (effect_volume != 10) effect_volume += 1;
		stop_bgm(sfxTest);
		game_log("effect up\n");
		sfxTest = play_audio(PACMAN_EATPUP, effect_volume);}
	if (btnSkin1.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		game_log("Skin default\n");
		if (pacman_skin == 1) pacman_skin = 2;
		else pacman_skin -= 1;
		stop_bgm(sfxTest);
		sfxTest = play_audio(PACMAN_MOVESOUND, effect_volume);}
	if (btnSkin2.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		game_log("Skin default\n");
		if (pacman_skin == 2) pacman_skin = 1;
		else pacman_skin += 1;
		stop_bgm(sfxTest);
		sfxTest = play_audio(PACMAN_MOVESOUND, effect_volume);}
}


static void destroy() {
	stop_bgm(settingsBGM);
    al_destroy_bitmap(btnToMenu.default_img);
	al_destroy_bitmap(gameTitle);
	al_destroy_bitmap(btnToMenu.hovered_img);
	al_destroy_bitmap(btnBGML.default_img);
	al_destroy_bitmap(btnBGML.hovered_img);
	al_destroy_bitmap(btnBGMR.default_img);
	al_destroy_bitmap(btnBGMR.hovered_img);
	al_destroy_bitmap(btnBGML2.default_img);
	al_destroy_bitmap(btnBGML2.hovered_img);
	al_destroy_bitmap(btnBGMR2.default_img);
	al_destroy_bitmap(btnBGMR2.hovered_img);
	al_destroy_bitmap(btnSkin1.default_img);
	al_destroy_bitmap(btnSkin1.hovered_img);
	al_destroy_bitmap(btnSkin2.default_img);
	al_destroy_bitmap(btnSkin2.hovered_img);
	al_destroy_bitmap(btnApply.default_img);
	al_destroy_bitmap(btnApply.hovered_img);
	al_destroy_bitmap(skin1);
	al_destroy_bitmap(skin2);
	al_stop_timer(anim_timer);
	al_destroy_timer(anim_timer);
}

static void on_key_down(int keycode) {
	switch (keycode) {
	case ALLEGRO_KEY_BACKSPACE:
		game_change_scene(scene_menu_create());
		break;
	default:
		break;
	}
}


// The only function that is shared across files.
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Settings";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}
