#include "ghost.h"
#include "pacman_obj.h"
#include "map.h"
/* Shared variables */
int GO_OUT_TIME = 256;
extern uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
extern const int cage_grid_x, cage_grid_y;

/* Declare static function prototypes */
static void ghost_red_move_script_FREEDOM(Ghost* ghost, Map* M);
static void ghost_red_move_script_BLOCKED(Ghost* ghost, Map* M);
static void ghost_pink_move_script_BLOCKED(Ghost* ghost, Map* M);
static void ghost_blue_move_script_BLOCKED(Ghost* ghost, Map* M);
static void ghost_orange_move_script_BLOCKED(Ghost* ghost, Map* M);


static void ghost_red_move_script_FREEDOM(Ghost* ghost, Map* M) {
	// [HACKATHON 2-4 // DONE]
	// Uncomment the following code and finish pacman picking random direction.

	static Directions proba[4] = {0,0,0,0}; // possible movement
	Directions k = 0;
	Directions j = ghost->objData.facing;
	switch (j)
	{
	case 1:
		j = 4;
		break;
	case 2:
		j = 3;
		break;
	case 3:
		j = 2;
		break;
	case 4:
		j = 1;
		break;
	default:
		j = 0;
		break;
	}
	int cnt = 0;
	for (Directions i = 1; i <= 4; i++)
		if (ghost_movable(ghost, M, i, true)) 	proba[cnt++] = i;
	if (cnt == 0) ghost_NextMove(ghost, UP);
	if (cnt == 1) {
		ghost_NextMove(ghost, proba[0]); return;
	}
	while (k == 0 || k == j) { k = proba[rand() % 4];}
	ghost_NextMove(ghost, k);

	// [TODO] (Not in Hackathon)
	// Description:
	// For red(Blinky) ghost, we ask you to implement an random strategy ghost,
	// which means moving in random direction.
	// But your random strategy have to designed carefully so that ghost won't walk back and forth.
	// (The code here DO perform walking back and forth.)

}
static void ghost_orange_move_script_FREEDOM(Ghost* ghost, Map* M, const Pacman* const pacman) {
	static Directions proba[4] = { 0,0,0,0 };
	Directions k = 0;
	Directions j = ghost->objData.facing;
	Directions shortestDirection = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y,
														pacman->objData.Coord.x, pacman->objData.Coord.y);
	switch (j)
	{
	case 1:
		j = 4;
		break;
	case 2:
		j = 3;
		break;
	case 3:
		j = 2;
		break;
	case 4:
		j = 1;
		break;
	default:
		j = 0;
		break;
	}
	int cnt = 0;
	if (ghost_movable(ghost, M, shortestDirection, true)) {
		ghost_NextMove(ghost, shortestDirection); return;
	}
	else {
		int cnt = 0;
		for (Directions i = 1; i <= 4; i++)
			if (ghost_movable(ghost, M, i, true)) 	proba[cnt++] = i;
		if (cnt == 0) ghost_NextMove(ghost, UP);
		if (cnt == 1) {
			ghost_NextMove(ghost, proba[0]); return;
		}
		while (k == 0 || k == j) { k = proba[rand() % 4]; }
		ghost_NextMove(ghost, k);
	}
}

static void ghost_red_move_script_BLOCKED(Ghost* ghost, Map* M) {

	switch (ghost->objData.preMove)
	{
	case UP:
		if (ghost->objData.Coord.y == 14)
			ghost_NextMove(ghost, DOWN);
		else
			ghost_NextMove(ghost, UP);
		break;
	case DOWN:
		if (ghost->objData.Coord.y == 16)
			ghost_NextMove(ghost, UP);
		else
			ghost_NextMove(ghost, DOWN);
		break;
	default:
		ghost_NextMove(ghost, UP);
		break;
	}
}
static void ghost_pink_move_script_BLOCKED(Ghost* ghost, Map* M) {

	switch (ghost->objData.preMove)
	{
	case LEFT:
		if (ghost->objData.Coord.x == 17)
			ghost_NextMove(ghost, RIGHT);
		else
			ghost_NextMove(ghost, LEFT);
		break;
	case RIGHT:
		if (ghost->objData.Coord.x == 19)
			ghost_NextMove(ghost, LEFT);
		else
			ghost_NextMove(ghost, RIGHT);
		break;
	default:
		ghost_NextMove(ghost, LEFT);
		break;
	}
}
static void ghost_blue_move_script_BLOCKED(Ghost* ghost, Map* M) {

	switch (ghost->objData.preMove)
	{
    case DOWN:
		if (ghost->objData.Coord.y == 16)
			ghost_NextMove(ghost, UP);
		else
			ghost_NextMove(ghost, DOWN);
		break;
	case RIGHT:
		if (ghost->objData.Coord.x == 19)
			ghost_NextMove(ghost, LEFT);
		else
			ghost_NextMove(ghost, RIGHT);
		break;
	case LEFT:
		if (ghost->objData.Coord.x == 17)
			ghost_NextMove(ghost, RIGHT);
		else
			ghost_NextMove(ghost, LEFT);
		break;
	default:
		ghost_NextMove(ghost, RIGHT);
		break;
	}
}
static void ghost_orange_move_script_BLOCKED(Ghost* ghost, Map* M) {

	switch (ghost->objData.preMove)
	{
	case DOWN:
		if (ghost->objData.Coord.y == 16)
			ghost_NextMove(ghost, UP);
		else
			ghost_NextMove(ghost, DOWN);
		break;
	case UP:
		if (ghost->objData.Coord.y == 14)
			ghost_NextMove(ghost, DOWN);
		else
			ghost_NextMove(ghost, UP);
		break;
	default:
		ghost_NextMove(ghost, DOWN);
		break;
	}
}

void ghost_red_move_script(Ghost* ghost, Map* M, Pacman* pacman) {
	if (!movetime(ghost->speed))
		return;
		switch (ghost->status)
		{
		case BLOCKED:
			ghost_red_move_script_BLOCKED(ghost, M);
			if (al_get_timer_count(game_tick_timer) > GO_OUT_TIME)
				ghost->status = GO_OUT;
			break;
		case FREEDOM:
			ghost_red_move_script_FREEDOM(ghost, M);
			break;
		case GO_OUT:
			ghost_move_script_GO_OUT(ghost, M);
			break;
		case GO_IN:
			ghost_move_script_GO_IN(ghost, M);
			if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
				GO_OUT_TIME = (al_get_timer_count(game_tick_timer) + 256);
				ghost->status = BLOCKED;
				ghost->speed = 2;
			}
			break;
		case FLEE:
			ghost_move_script_FLEE(ghost, M, pacman);
			break;
		default:
			break;
		}

		if(ghost_movable(ghost, M, ghost->objData.nextTryMove, false)){
			ghost->objData.preMove = ghost->objData.nextTryMove;
			ghost->objData.nextTryMove = NONE;
		}
		else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
			return;

		switch (ghost->objData.preMove) {
		case RIGHT:
			ghost->objData.Coord.x += 1;
			break;
		case LEFT:
			ghost->objData.Coord.x -= 1;
			break;
		case UP:
			ghost->objData.Coord.y -= 1;
			break;
		case DOWN:
			ghost->objData.Coord.y += 1;
			break;
		default:
			break;
		}
		ghost->objData.facing = ghost->objData.preMove;
		ghost->objData.moveCD = GAME_TICK_CD;
}

void ghost_pink_move_script(Ghost* ghost, Map* M, Pacman* pacman) {
	if (!movetime(ghost->speed))
		return;
	switch (ghost->status)
	{
	case BLOCKED:
		ghost_pink_move_script_BLOCKED(ghost, M);
		if (al_get_timer_count(game_tick_timer) > GO_OUT_TIME + 256)
			ghost->status = GO_OUT;
		break;
	case FREEDOM:
		ghost_red_move_script_FREEDOM(ghost, M);
		break;
	case GO_OUT:
		ghost_move_script_GO_OUT(ghost, M);
		break;
	case GO_IN:
		ghost_move_script_GO_IN(ghost, M);
		if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
			GO_OUT_TIME = (al_get_timer_count(game_tick_timer) + 256);
			ghost->status = BLOCKED;
			ghost->speed = 2;
		}
		break;
	case FLEE:
		ghost_move_script_FLEE(ghost, M, pacman);
		break;
	default:
		break;
	}

	if (ghost_movable(ghost, M, ghost->objData.nextTryMove, false)) {
		ghost->objData.preMove = ghost->objData.nextTryMove;
		ghost->objData.nextTryMove = NONE;
	}
	else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
		return;

	switch (ghost->objData.preMove) {
	case RIGHT:
		ghost->objData.Coord.x += 1;
		break;
	case LEFT:
		ghost->objData.Coord.x -= 1;
		break;
	case UP:
		ghost->objData.Coord.y -= 1;
		break;
	case DOWN:
		ghost->objData.Coord.y += 1;
		break;
	default:
		break;
	}
	ghost->objData.facing = ghost->objData.preMove;
	ghost->objData.moveCD = GAME_TICK_CD;
}

void ghost_blue_move_script(Ghost* ghost, Map* M, Pacman* pacman) {
	if (!movetime(ghost->speed))
		return;
	switch (ghost->status)
	{
	case BLOCKED:
		ghost_blue_move_script_BLOCKED(ghost, M);
		if (al_get_timer_count(game_tick_timer) > GO_OUT_TIME + 256 * 2)
			ghost->status = GO_OUT;
		break;
	case FREEDOM:
		ghost_red_move_script_FREEDOM(ghost, M);
		break;
	case GO_OUT:
		ghost_move_script_GO_OUT(ghost, M);
		break;
	case GO_IN:
		ghost_move_script_GO_IN(ghost, M);
		if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
			GO_OUT_TIME = (al_get_timer_count(game_tick_timer) + 256);
			ghost->status = BLOCKED;
			ghost->speed = 2;
		}
		break;
	case FLEE:
		ghost_move_script_FLEE(ghost, M, pacman);
		break;
	default:
		break;
	}

	if (ghost_movable(ghost, M, ghost->objData.nextTryMove, false)) {
		ghost->objData.preMove = ghost->objData.nextTryMove;
		ghost->objData.nextTryMove = NONE;
	}
	else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
		return;

	switch (ghost->objData.preMove) {
	case RIGHT:
		ghost->objData.Coord.x += 1;
		break;
	case LEFT:
		ghost->objData.Coord.x -= 1;
		break;
	case UP:
		ghost->objData.Coord.y -= 1;
		break;
	case DOWN:
		ghost->objData.Coord.y += 1;
		break;
	default:
		break;
	}
	ghost->objData.facing = ghost->objData.preMove;
	ghost->objData.moveCD = GAME_TICK_CD;
}

void ghost_orange_move_script(Ghost* ghost, Map* M, Pacman* pacman) {
	if (!movetime(ghost->speed))
		return;
	switch (ghost->status)
	{
	case BLOCKED:
		ghost_orange_move_script_BLOCKED(ghost, M);
		if (al_get_timer_count(game_tick_timer) > GO_OUT_TIME + 256* 3)
			ghost->status = GO_OUT;
		break;
	case FREEDOM:
		ghost_orange_move_script_FREEDOM(ghost, M, pacman);
		break;
	case GO_OUT:
		ghost_move_script_GO_OUT(ghost, M);
		break;
	case GO_IN:
		ghost_move_script_GO_IN(ghost, M);
		if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
			GO_OUT_TIME = (al_get_timer_count(game_tick_timer) + 256);
			ghost->status = BLOCKED;
			ghost->speed = 2;
		}
		break;
	case FLEE:
		ghost_move_script_FLEE(ghost, M, pacman);
		break;
	default:
		break;
	}

	if (ghost_movable(ghost, M, ghost->objData.nextTryMove, false)) {
		ghost->objData.preMove = ghost->objData.nextTryMove;
		ghost->objData.nextTryMove = NONE;
	}
	else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
		return;

	switch (ghost->objData.preMove) {
	case RIGHT:
		ghost->objData.Coord.x += 1;
		break;
	case LEFT:
		ghost->objData.Coord.x -= 1;
		break;
	case UP:
		ghost->objData.Coord.y -= 1;
		break;
	case DOWN:
		ghost->objData.Coord.y += 1;
		break;
	default:
		break;
	}
	ghost->objData.facing = ghost->objData.preMove;
	ghost->objData.moveCD = GAME_TICK_CD;
}
