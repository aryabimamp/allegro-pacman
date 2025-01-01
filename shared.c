// [shared.c]
// you should define the shared variable declared in the header here.

#include "shared.h"
#include "utility.h"
#include "game.h"
// #include "scene_menu.h"

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_SAMPLE* themeMusic = NULL;
ALLEGRO_SAMPLE* themeVictory = NULL;
ALLEGRO_SAMPLE* LoseMusic = NULL;
ALLEGRO_SAMPLE* PACMAN_MOVESOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_EAT_GHOST = NULL;
ALLEGRO_FONT* menuFont = NULL;
ALLEGRO_FONT* menuFont1 = NULL;
ALLEGRO_SAMPLE* PACMAN_SPEED = NULL;
ALLEGRO_SAMPLE* settingsMusic = NULL;
ALLEGRO_SAMPLE* PACMAN_EATPUP = NULL;
int fontSize = 30;
float music_volume = 1;
float effect_volume = 1;
bool gameDone = false;

/*
	A way to accelerate is load assets once.
	And delete them at the end.
	This method does provide you better management of memory.

*/
void shared_init(void) {

	menuFont = load_font("Assets/Minecraft.ttf", fontSize);
	menuFont1 = load_font("Assets/Horror.ttf", 100);
	themeMusic = load_audio("Assets/Music/Pacman_theme_music.ogg");
	PACMAN_MOVESOUND = load_audio("Assets/Music/pacman_eatdot.ogg");
	PACMAN_SPEED = load_audio("Assets/Music/speed-up.ogg");
	PACMAN_EAT_GHOST = load_audio("Assets/Music/crunch.ogg");
	PACMAN_DEATH_SOUND = load_audio("Assets/Music/Man-im-dead.ogg");
	PACMAN_EATPUP = load_audio("Assets/Music/star_power_up.ogg");
	settingsMusic = load_audio( "Assets/Music/ori_wisps.ogg");
    themeVictory = load_audio( "Assets/Music/victory_music.ogg");
    LoseMusic = load_audio( "Assets/Music/Asgore.ogg");

}

void shared_destroy(void) {

	al_destroy_font(menuFont);
	al_destroy_font(menuFont1);
	al_destroy_sample(PACMAN_SPEED);
	al_destroy_sample(LoseMusic);
	al_destroy_sample(themeVictory);
	al_destroy_sample(themeMusic);
	al_destroy_sample(PACMAN_MOVESOUND);
	al_destroy_sample(PACMAN_DEATH_SOUND);
	al_destroy_sample(PACMAN_EATPUP);
    al_destroy_sample(settingsMusic);
	al_destroy_sample(PACMAN_EAT_GHOST);
}
