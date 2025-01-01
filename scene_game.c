#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "scene_end.h"
#include "scene_settings.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"


// [HACKATHON 2-0] DONE
#define GHOST_NUM 4
/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
extern ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID;
extern ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID2;
extern ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID3;
extern ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID4;
extern ALLEGRO_SAMPLE* PACMAN_EAT_GHOST;
extern int basic_speed;
int game_main_Score = 0;
int game_high_Score = 0;
bool game_over = false;
bool game_victory = false;

/* Internal variables*/
static ALLEGRO_BITMAP* gameTitle = NULL;
static int gameTitleW ;
static int gameTitleH ;
ALLEGRO_TIMER* power_up_timer;
const int power_up_duration = 7;
ALLEGRO_TIMER* power_speed_timer;
const int power_speed_duration = 3;
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;
bool debug_mode = false;
bool cheat_mode = false;

/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);
static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void on_mouse_down(void);
static void render_init_screen(void);
static void draw_hitboxes(void);

static void init(void) {
    gameTitle = load_bitmap("Assets/game_bg.png");
	gameTitleW = al_get_bitmap_width(gameTitle);
	gameTitleH = al_get_bitmap_height(gameTitle);
	game_over = false;
	game_main_Score = 0;
	// create map
	// [TODO // DONE]
	// Create map from .txt file and design your own map !!
	basic_map = create_map("Assets/map_nthu.txt");
	if (!basic_map) {
		game_abort("error on creating map");
	}

	// create pacman
	pman = pacman_create();
	ghosts = (Ghost*)malloc(sizeof(Ghost) * GHOST_NUM); // [HACKATHON 2-1]
	if (!pman) {
		game_abort("error on creating pacman\n");
	}

	// allocate ghost memory
	// [HACKATHON 2-1] DONE
	// TODO: Allocate dynamic memory for ghosts array.

	else {
		// [HACKATHON 2-2] DONE
		// TODO: create a ghost.
		// Try to look the definition of ghost_create and figure out what should be placed here.
		for (int i = 0; i < GHOST_NUM; i++) {

			game_log("creating ghost %d\n", i);
			ghosts[i] = ghost_create(i);
			if (!ghosts[i])
				game_abort("error creating ghost\n");

		}
	}
	GAME_TICK = 0;

	render_init_screen();
	power_up_timer = al_create_timer(1.0f); // 1 tick / sec
	if (!power_up_timer)
		game_abort("Error on create timer\n");
    power_speed_timer = al_create_timer(1.0f);
    if (!power_speed_timer)
		game_abort("Error on create timer\n");


	return ;
}

static void step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}
static void checkItem(void) {
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	// [HACKATHON 1-3] DONE
	// TODO: check which item you are going to eat and use `pacman_eatItem` to deal with it.
	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]);
		basic_map->map[Grid_y][Grid_x] = ' ';
		game_main_Score += 10;
		basic_map->beansCount--;
		break;
	case 'P':
		pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]);
		basic_map->map[Grid_y][Grid_x] = ' ';
		game_main_Score += 30;
		for (int i = 0; i < GHOST_NUM; i++) {
			ghost_toggle_FLEE(ghosts[i], true);}
		game_log("power bean on");
		if (!al_get_timer_started(power_up_timer))
			al_start_timer(power_up_timer);
		else al_set_timer_count(power_up_timer, 0);
		break;
    case 'S':
		pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]);
		basic_map->map[Grid_y][Grid_x] = ' ';
		game_main_Score += 30;
		game_log("power speed on");
		pman->speed = 4.5;
		if (!al_get_timer_started(power_speed_timer))
			al_start_timer(power_speed_timer);
		else al_set_timer_count(power_speed_timer, 0);
		break;
	default:
		break;
		//Eating wall pacman
		//pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]);
		//basic_map->map[Grid_y][Grid_x+1] = ' ';
	}

	// [HACKATHON 1-4] DONE
	// erase the item you eat from map
	// becareful no erasing the wall block.
	/*
		basic_map->map...;
	*/
}
static void status_update(void) {
	for (int i = 0; i < GHOST_NUM; i++) {
		if (ghosts[i]->status == GO_IN)
			continue;
		// [TODO // DONE]
		// use `getDrawArea(..., GAME_TICK_CD)` and `RecAreaOverlap(..., GAME_TICK_CD)` functions to detect
		// if pacman and ghosts collide with each other.
		// And perform corresponding operations.
		if (!cheat_mode && RecAreaOverlap(getDrawArea(pman->objData, GAME_TICK_CD),
										getDrawArea(ghosts[i]->objData, GAME_TICK_CD))) {
			if (ghosts[i]->status == FLEE) {
				ghost_collided(ghosts[i]);
				play_audio(PACMAN_EAT_GHOST, effect_volume + 3);
				game_main_Score += 200;
			}
			else {
				game_log("collide with ghost\n");
				stop_bgm(PACMAN_MOVESOUND_ID);
				stop_bgm(PACMAN_MOVESOUND_ID2);
				stop_bgm(PACMAN_MOVESOUND_ID3);
				stop_bgm(PACMAN_MOVESOUND_ID4);
				al_rest(0.5);
				pacman_die();
				//game_high_Score = (game_main_Score >= game_high_Score) ? game_main_Score : game_high_Score;
				game_over = true;
				break;
			}
		}
		// [NOTE // DONE]
		// You should have some branch here if you want to implement power bean mode.
		// Uncomment Following Code
		/*
		if(!cheat_mode and collision of pacman and ghost)
		{
			game_log("collide with ghost\n");
			al_rest(1.0);
			pacman_die();
			game_over = true;
			break;
		}
		*/
	}
	if (basic_map->beansCount == 0) {
		game_log("Victory\n");
		stop_bgm(PACMAN_MOVESOUND_ID);
		stop_bgm(PACMAN_MOVESOUND_ID2);
		stop_bgm(PACMAN_MOVESOUND_ID3);
		stop_bgm(PACMAN_MOVESOUND_ID4);
		game_high_Score = (game_main_Score >= game_high_Score) ? game_main_Score : game_high_Score;
		game_victory = true;
		game_change_scene(scene_end_create());
	}
}

static void update(void) {
	if (game_over) {
		/*
			[TODO // DONE]
			start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
			game_change_scene(...);
		*/
		game_log("death animation timer Start\n");
		al_start_timer(pman->death_anim_counter);
		if (al_get_timer_count(pman->death_anim_counter) > 96) {
			game_change_scene(scene_end_create());}
		return;
	}
	if (al_get_timer_count(power_up_timer) > power_up_duration) {
		al_stop_timer(power_up_timer);
		al_set_timer_count(power_up_timer, 0);
		for (int i = 0; i < GHOST_NUM; i++) {
			ghost_toggle_FLEE(ghosts[i], false);
		}
		game_log("power bean off");
	}
	if (al_get_timer_count(power_speed_timer) > power_speed_duration) {
		al_stop_timer(power_speed_timer);
		al_set_timer_count(power_speed_timer, 0);
		pman->speed = 2;
		game_log("power speed off");

	}


	step();
	checkItem();
	status_update();
	pacman_move(pman, basic_map);
	for (int i = 0; i < GHOST_NUM; i++)
		ghosts[i]->move_script(ghosts[i], basic_map, pman);
}

static void draw(void) {

    al_clear_to_color(al_map_rgb(0, 0, 0));

    const float scale = 0.7;
	const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * gameTitleW;
	const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * gameTitleH;

	al_draw_scaled_bitmap(
		gameTitle,
		0, 0,
		gameTitleW, gameTitleH,
		offset_w, offset_h,
		gameTitleW * scale, gameTitleH * scale,
		0
	);


	//	[TODO]
	//	Draw scoreboard, something your may need is sprinf
	// char str[30];
	// sprintf(str, "Score: %d", (int)10);
	// al_draw_text(menuFont, al_map_rgb(255, 255, 255), 0, 0, ALLEGRO_ALIGN_LEFT, str);
	al_draw_textf(menuFont, al_map_rgb(255, 255, 255), 0, 0,
		ALLEGRO_ALIGN_LEFT, "Score: %d", game_main_Score);

	draw_map(basic_map);

	pacman_draw(pman);
	if (game_over)
		return;
	// no drawing below when game over
	for (int i = 0; i < GHOST_NUM; i++)
		ghost_draw(ghosts[i]);

	//debugging mode
	if (debug_mode) {
		draw_hitboxes();
	}

}

static void draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea(pman->objData, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}

static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}


static void destroy(void) {
	/*
		[TODO]
		free map array, Pacman and ghosts
	*/
	/*for (int i = 0; i < GHOST_NUM; i++) {
		free(ghosts[i]);
	}*/
	//free(ghosts);
	free(basic_map->map);
	free(basic_map);
	//free(pman);
	stop_bgm(PACMAN_MOVESOUND_ID2);
	stop_bgm(PACMAN_MOVESOUND_ID3);
	stop_bgm(PACMAN_MOVESOUND_ID4);
	al_stop_timer(power_speed_timer);
	al_destroy_timer(power_speed_timer);
    al_stop_timer(power_up_timer);
	al_destroy_timer(power_up_timer);
}

static void on_key_down(int key_code) {
	switch (key_code)
	{
		// [HACKATHON 1-1]
		// TODO: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
		// we provided you a function `pacman_NextMove` to set the pacman's next move direction.
		case ALLEGRO_KEY_W:
			pacman_NextMove(pman, UP);
			break;
		case ALLEGRO_KEY_A:
			pacman_NextMove(pman, LEFT);
			break;
		case ALLEGRO_KEY_S:
			pacman_NextMove(pman, DOWN);
			break;
		case ALLEGRO_KEY_D:
			pacman_NextMove(pman, RIGHT);
			break;
		case ALLEGRO_KEY_C:
			cheat_mode = !cheat_mode;
			if (cheat_mode)
				printf("cheat mode on\n");
			else
				printf("cheat mode off\n");
			break;
		case ALLEGRO_KEY_G:
			debug_mode = !debug_mode;
			break;
		case ALLEGRO_KEY_I:
			printinfo();
			break;
		case ALLEGRO_KEY_V:
			basic_map->beansCount = 0;
			break;
	default:
		break;
	}

}

static void on_mouse_down(void) {
	// nothing here

}

static void render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_map(basic_map);
	pacman_draw(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);

	al_flip_display();
	al_rest(2.0);

}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
	return scene;
}
