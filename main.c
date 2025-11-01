#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "agent.h"

int find_neighbour(agent_info_t info, cell_t type)
{
    coords_t center = {VIEW_DISTANCE, VIEW_DISTANCE};

    for(int dir = 0 ; dir < 8 ; dir++)
    {
        coords_t coords = direction_to_coords(center, dir);
        cell_t neighbour = info.cells[coords.row][coords.col];
        if (neighbour == type)
        {
            return dir;
        }
    }

    return -1;
}

command_t think(agent_info_t info)
{
    cell_t bee = info.cells[VIEW_DISTANCE][VIEW_DISTANCE];
	int player = info.player;

	if (is_bee_with_flower(bee))
	{
		int hive_dir = find_neighbour(info, hive_cell(info.player));
		if (player == 1)
		{
			if (info.col < 28 && hive_dir < 0)
				return (command_t){.action = MOVE, .direction = E};
			if (info.row <= 13 && hive_dir < 0)
				return (command_t){.action = MOVE, .direction = S};
			if (info.row > 13 && hive_dir < 0)
				return (command_t){.action = MOVE, .direction = N};
		}
		else
		{
			if (info.col > 2 && hive_dir < 0)
				return (command_t){.action = MOVE, .direction = W};
			if (info.row <= 13 && hive_dir < 0)
				return (command_t){.action = MOVE, .direction = S};
			if (info.row > 13 && hive_dir < 0)
				return (command_t){.action = MOVE, .direction = N};
		}
		if (hive_dir >= 0)
		{
			return (command_t) {
				.action = FORAGE,
				.direction = hive_dir
			};
		}
	}
	else
	{
		int flower_dir = find_neighbour(info, FLOWER);
		if (flower_dir >= 0)
		{
			return (command_t) {
				.action = FORAGE,
				.direction = flower_dir
			};
		}
	}

    return (command_t) {
        .action = MOVE,
        .direction = rand() % 8
    };
}

int main(int argc, char **argv)
{
    if (argc < 3)
        panic("Usage: ./agent arena_host arena_ip");

    srand(time(NULL) + getpid());

    char *host = argv[1];
    int port = atoi(argv[2]);
    char *team_name = "embarassedB";

    agent_main(host, port, team_name, think);
}
