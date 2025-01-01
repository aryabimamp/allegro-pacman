#include <allegro5/allegro_primitives.h>
#include "pacman_obj.h"
#include "map.h"
/* Static variables */
static const int start_grid_x = 25, start_grid_y = 25;		// where to put pacman at the beginning
static const int fix_draw_pixel_offset_x = -3, fix_draw_pixel_offset_y = -3;  // draw offset
static const int draw_region = 30;							// pacman bitmap draw region

int animation = 0;
int animationash= 0;
int pacman_skin = 1;
ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID;
ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID2;
ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID3;
ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID4;
// [ NOTE - speed ]
// If you want to implement something regarding speed.
// You may have to modify the basic_speed here.
// But before you modify this, make sure you are
// totally understand the meaning of speed and function
// `step()` in `scene_game.c`, also the relationship between
// `speed`, `GAME_TICK`, `GAME_TICK_CD`, `objData->moveCD`.
static const int basic_speed = 2;

/* Shared variables */
extern ALLEGRO_SAMPLE* PACMAN_SPEED;
extern ALLEGRO_SAMPLE* PACMAN_MOVESOUND;
extern ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND;
extern ALLEGRO_SAMPLE* PACMAN_EATPUP;
extern uint32_t GAME_TICK;
extern uint32_t GAME_TICK_CD;
extern bool game_over;
extern float effect_volume;

/* Declare static function */
static bool pacman_movable(Pacman* pacman, Map* M, Directions targetDirec) {
	// [HACKATHON 1-2]
	// TODO: Determine if the current direction is movable.
	// That is to say, your pacman shouldn't penetrate 'wall' and 'room'
	// , where room is reserved for ghost to set up.
	// 1) For the current direction `targetDirec`, use pre-implemented function
	// `is_wall_block` and `is_room_block` to check if the block is wall or room. (they are both defined in map.c)
	// 2) the coordinate data of pacman is stored in pacman->objData.Coord
	// it is a self-defined pair IntInt type. Trace the code and utilize it.


	int x = pacman->objData.Coord.x; int y = pacman->objData.Coord.y;

	switch (targetDirec)
	{
	case UP:
		y=y-1;
		break;
	case DOWN:
		y=y+1;
		break;
	case LEFT:
		x=x-1;
		break;
	case RIGHT:
		x=x+1;
		break;
	default:
		// for none UP, DOWN, LEFT, RIGHT direction u should return false.
		return false;
	}
	if (is_wall_block(M, x, y) || is_room_block(M, x, y))
		return false;

	return true;
}

Pacman* pacman_create() {

	/*
		[TODO]
		Allocate dynamic memory for pman pointer;
	*/
	Pacman* pman = (Pacman*)malloc(sizeof(Pacman));
	if (!pman)
		return NULL;
	/*
		Pacman* pman = ...
		if(!pman)
			return NULL;
	*/
	/* Init pman data */
	/* set starting point, Size, */
	/* TODO? */
	/* hint / just put it */
	pman->objData.Coord.x = 24;
	pman->objData.Coord.y = 24;
	//pman->objData.Size.x = block_width;
	//pman->objData.Size.y = block_height;
	switch (pacman_skin)
	{
	case 1:
    pman->objData.Size.x = block_width;
	pman->objData.Size.y = block_height;
		break;
	case 2:
    pman->objData.Size.x = 33;
	pman->objData.Size.y = 40;
		break;

	default:
    pman->objData.Size.x = block_width;
	pman->objData.Size.y = block_height;
		break;
	}

	pman->objData.preMove = NONE;
	pman->objData.nextTryMove = NONE;
	pman->speed = basic_speed;

	pman->death_anim_counter = al_create_timer(1.0f / 64);
	pman->powerUp = false;
	/* load sprites */
	switch (pacman_skin)
	{
	case 1:
		pman->move_sprite = load_bitmap("Assets/pacman_move.png");
		break;
	case 2:
		pman->move_sprite = load_bitmap("Assets/ash_move.png");
		break;
	default:
		pman->move_sprite = load_bitmap("Assets/pacman_move.png");
		break;
	}
    switch (pacman_skin)
	{
	case 1:
	pman->die_sprite = load_bitmap("Assets/pacman_die.png");
	break;
	case 2:
	pman->die_sprite = load_bitmap("Assets/ash_die.png");
	break;
	default:
	pman->die_sprite = load_bitmap("Assets/pacman_die.png");
	break;
	}

	return pman;

}

void pacman_destroy(Pacman* pman) {
	al_stop_timer(pman->death_anim_counter);
	al_destroy_timer(pman->death_anim_counter);
	al_destroy_bitmap(pman->die_sprite);
	al_destroy_bitmap(pman->move_sprite);
	free(pman);

}


void pacman_draw(Pacman* pman) {
	/*
		[HW-TODO ]
		Draw Pacman and animations
		hint: use pman->objData.moveCD to determine which frame of the animation to draw, you may refer to discription in ghost_draw in ghost.c
	*/
	RecArea drawArea = getDrawArea(pman->objData, GAME_TICK_CD);

	int offset = 0;

	if (game_over) {
		int frame = al_get_timer_count(pman->death_anim_counter) / 7;
		game_log("Pacman dead animation start\n");
		switch (pacman_skin)
	{
        case 1:
        al_draw_scaled_bitmap(pman->die_sprite, frame * 16, 0, 16, 16,
			drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
			draw_region, draw_region, 0);
        break;
        case 2:
        al_draw_scaled_bitmap(pman->die_sprite, 0, frame * 47, 32, 47,
			drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
			draw_region, draw_region, 0);
        break;
        default:
        al_draw_scaled_bitmap(pman->die_sprite, frame * 16, 0, 16, 16,
			drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
			draw_region, draw_region, 0);
        break;
	}


		/*hint: instead of using pman->objData.moveCD, use Pacman's death_anim_counter to create animation*/
	}
	else {
		if (pman->objData.moveCD % 64 <= 32) {animation = 0; animationash = 0;}
		else {animation = 16; animationash = 33;}
		switch (pacman_skin)
        {
        case 1:
        switch (pman->objData.facing) {
		case RIGHT:
			al_draw_scaled_bitmap(pman->move_sprite, 0 + animation, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
			break;
		case LEFT:
			al_draw_scaled_bitmap(pman->move_sprite, 16 * 2 + animation, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
			break;
		case UP:
			al_draw_scaled_bitmap(pman->move_sprite, 16 * 4 + animation, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
			break;
		case DOWN:
			al_draw_scaled_bitmap(pman->move_sprite, 16 * 6 + animation, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
			break;
		default:
			al_draw_scaled_bitmap(pman->move_sprite, 0, 0,
				16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
			break;
		}
        break;
        case 2:
        switch (pman->objData.facing) {
		case RIGHT:
			al_draw_scaled_bitmap(pman->move_sprite, 0 + animationash, 0, 33, 40,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
			break;
		case LEFT:
			al_draw_scaled_bitmap(pman->move_sprite, 33 * 2 + animationash, 0, 33, 40,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
			break;
		case UP:
			al_draw_scaled_bitmap(pman->move_sprite, 33 * 4 + animationash, 0, 33, 40,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
			break;
		case DOWN:
			al_draw_scaled_bitmap(pman->move_sprite, 33 * 6 + animationash, 0, 33, 40,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
			break;
		default:
			al_draw_scaled_bitmap(pman->move_sprite, 0, 0,
				16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
			break;
		}
        break;
        default:
        switch (pman->objData.facing) {
		case RIGHT:
			al_draw_scaled_bitmap(pman->move_sprite, 0 + animation, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
			break;
		case LEFT:
			al_draw_scaled_bitmap(pman->move_sprite, 16 * 2 + animation, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
			break;
		case UP:
			al_draw_scaled_bitmap(pman->move_sprite, 16 * 4 + animation, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
			break;
		case DOWN:
			al_draw_scaled_bitmap(pman->move_sprite, 16 * 6 + animation, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
			break;
		default:
			al_draw_scaled_bitmap(pman->move_sprite, 0, 0,
				16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
			break;
		}
        break;
        }

	}
}
void pacman_move(Pacman* pacman, Map* M) {
	if (!movetime(pacman->speed))
		return;
	if (game_over)
		return;

	int probe_x = pacman->objData.Coord.x, probe_y = pacman->objData.Coord.y;
	if (pacman_movable(pacman, M, pacman->objData.nextTryMove))
		pacman->objData.preMove = pacman->objData.nextTryMove;
	else if (!pacman_movable(pacman, M, pacman->objData.preMove))
		return;

	switch (pacman->objData.preMove)
	{
	case UP:
		pacman->objData.Coord.y -= 1;
		pacman->objData.preMove = UP;
		break;
	case DOWN:
		pacman->objData.Coord.y += 1;
		pacman->objData.preMove = DOWN;
		break;
	case LEFT:
		pacman->objData.Coord.x -= 1;
		pacman->objData.preMove = LEFT;
		break;
	case RIGHT:
		pacman->objData.Coord.x += 1;
		pacman->objData.preMove = RIGHT;
		break;
	default:
		break;
	}
	pacman->objData.facing = pacman->objData.preMove;
	pacman->objData.moveCD = GAME_TICK_CD;
}
void pacman_eatItem(Pacman* pacman, const char Item) {
	switch (Item)
	{
	case '.':
		stop_bgm(PACMAN_MOVESOUND_ID);
		PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND, effect_volume);
		break;
        case 'P':
		stop_bgm(PACMAN_MOVESOUND_ID);
		stop_bgm(PACMAN_MOVESOUND_ID2);
		PACMAN_MOVESOUND_ID2 = play_audio(PACMAN_EATPUP, effect_volume + 1);
		break;
		case 'S':
		stop_bgm(PACMAN_MOVESOUND_ID);
		stop_bgm(PACMAN_MOVESOUND_ID4);
		PACMAN_MOVESOUND_ID4 = play_audio(PACMAN_SPEED, effect_volume);
		break;
	default:
		break;
	}
}

void pacman_NextMove(Pacman* pacman, Directions next) {
	pacman->objData.nextTryMove = next;
}

void pacman_die() {
	stop_bgm(PACMAN_MOVESOUND_ID);
	PACMAN_MOVESOUND_ID = play_audio(PACMAN_DEATH_SOUND, effect_volume);
}






