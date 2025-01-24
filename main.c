#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <SDL2/SDL.h>

#include "./style.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define CELL_WIDTH ((float) SCREEN_WIDTH / BOARD_WIDTH)
#define CELL_HEIGHT ((float) SCREEN_HEIGHT / BOARD_HEIGHT)

#define AGENT_COUNT 5

int scc(int code) {
	if (code < 0) {
		fprintf(stderr, "ERROR: sdl error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	return code;
}

void *scp(void *ptr) {
	if (ptr == NULL) {
		fprintf(stderr, "ERROR: sdl error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	return ptr;
}

void sdl_set_color_hex(SDL_Renderer *renderer, Uint32 hex) {
	scc(SDL_SetRenderDrawColor(
			renderer,
			(hex >> (3 * 8)) & 0xFF,
			(hex >> (2 * 8)) & 0xFF,
			(hex >> (1 * 8)) & 0xFF,
			(hex >> (0 * 8)) & 0xFF));

}

typedef enum {
	DIR_RIGHT = 0,
	DIR_UP,
	DIR_LEFT,
	DIR_DOWN,
} Dir;

typedef struct {
	int pos_x, pos_y;
	Dir dir;
	int hunger;
	int health;
} Agent;

typedef enum {
	ACTION_NOP = 0,
	ACTION_STEP,
	ACTION_EAT,
	ACTION_ATTACK,
} Agent_Action;

Agent agents[AGENT_COUNT];

void render_board_grid(SDL_Renderer *renderer) {
	sdl_set_color_hex(renderer, GRID_COLOR);
	for (int x = 1; x < BOARD_WIDTH; ++x) {
		scc(SDL_RenderDrawLine(
				renderer,
				x * CELL_WIDTH,
				0,
				x * CELL_WIDTH,
				SCREEN_HEIGHT));
	}

	for (int y = 1; y < BOARD_HEIGHT; ++y) {
		scc(SDL_RenderDrawLine(
				renderer,
				0,
				y * CELL_HEIGHT,
				SCREEN_WIDTH,
				y * CELL_HEIGHT));
	}
}

int random_int_range(int low, int high) {
	return low + rand() % (high - low);
}

Dir random_dir(void) {
	return (Dir) random_int_range(0, 4);
}

Agent random_agent(void) {
	Agent agent = { 0 };
	agent.pos_x = random_int_range(0, BOARD_WIDTH);
	agent.pos_y = random_int_range(0, BOARD_HEIGHT);
	agent.dir = random_dir();
	agent.hunger = 100;
	agent.health = 100;
	return agent;
}

void init_agents(void) {
	for (size_t i = 0; i < AGENT_COUNT; ++i) {
		agents[i] = random_agent();
	}
}

void render_agent(SDL_Renderer *renderer, Agent agent) {
	sdl_set_color_hex(renderer, AGENT_COLOR);

	SDL_Rect rect = {
		(int) floorf(agent.pos_x * CELL_WIDTH),
		(int) floorf(agent.pos_y * CELL_HEIGHT),
		(int) floorf(CELL_WIDTH),
		(int) floorf(CELL_HEIGHT),
	};

	scc(SDL_RenderFillRect(renderer, &rect));
}

void render_all_agents(SDL_Renderer *renderer) {
	for (size_t i = 0; i < AGENT_COUNT; ++i) {
		render_agent(renderer, agents[i]);
	}
}

int main(int argc, char *argv[]) {
	init_agents();

	scc(SDL_Init(SDL_INIT_VIDEO));
	SDL_Window *const window = scp(SDL_CreateWindow(
		"Hunger Games?",
		0, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_RESIZABLE));

	SDL_Renderer *const renderer = scp(SDL_CreateRenderer(
						window, -1,
						SDL_RENDERER_ACCELERATED));

	scc(SDL_RenderSetLogicalSize(renderer, (int) SCREEN_WIDTH, SCREEN_HEIGHT));

	int quit = 0;
	while (!quit) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			}
		}

		sdl_set_color_hex(renderer, BACKGROUND_COLOR);
		scc(SDL_RenderClear(renderer));

		render_board_grid(renderer);
		render_all_agents(renderer);
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();
	return 0;
}
