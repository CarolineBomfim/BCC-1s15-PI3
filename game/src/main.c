#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "global.h"
#include "gameMain.h"
#include "ranking.h"
#include "initialMenu.h"
#include "logger.h"

#define ESPACO_MEMORIA 100

void gameExit(global_var *global);

int main(int argc, char **argv) {
	al_init();

	if(!al_init_image_addon()) {
		erro("Falha ao carregar biblioteca de images.");
	}

	if(!al_init_acodec_addon()) {
		erro("Falha ao iniciar os codecs de audio");
	}

	if(!al_install_mouse()) {
		erro("Falha ao instalar mouse.");
	}

	if(!al_install_audio()) {
		erro("Falha ao instalar recursos de audio.");
	}

	if(!al_reserve_samples(1)) {
		erro("Falha ao reservar canal de audio");
	}
	config *configuracao = ler_arquivo_configuracao(ARQUIVO_CONFIG);
	global_var *global = malloc(sizeof(global_var));

	char aux[ESPACO_MEMORIA];
	int trying = 0;

	global->camera1 = camera_inicializa(0);

	sprintf(aux, "Camera: Altura= %d\n\t\t\tLargura= %d", 
	        global->camera1->altura, global->camera1->largura);
	logger(aux);
	sprintf(aux, "Config: Altura= %d\n\t\t\tLargura= %d", 
	        configuracao->altura, configuracao->largura);
	logger(aux);

	global->display = al_create_display(configuracao->largura, configuracao->altura);
	global->event_queue = al_create_event_queue();
	// global->ranking = ler_ranking(RANKING_FILE);
	// while(global->ranking[trying].record != NULL) {
	// 	printf("%s\n",global->ranking[trying].name);
	// 	trying++;
	// }
	if(!global->event_queue) {
		erro("Falha ao criar fila de eventos");
	}
	global->configure = configuracao;

	if( argc > 1) {
		if(argv[1][1] == 't') {
			global->gameMode = TESTING_MODE;
		} else if(argv[1][1] == 'd') {
			global->gameMode = DEBUG_MODE;
		}
	} else {
		global->gameMode = NORMAL_MODE;
	}

	if(!global->display) {
		erro("Falha ao criar arquivo display");
	}

	while(TRUE) {
		switch (initialMenu(trying, global)) {
			case 0:
				trying += startGame(global);
				fprintf(stderr, "Saiu.\n" );
				break;
			case 1:
				if(global->ranking) {
					showRanking(global->ranking);
				}
				break;
			default:
				gameExit(global);
		}
	}
	return 0;
}

void gameExit(global_var *global) {
	al_destroy_display(global->display);
	camera_finaliza(global->camera1);
	// clearRank(global->ranking, RANKING_FILE);
	free(global);
	logger("Sair do jogo");
	exit(EXIT_SUCCESS);
}
