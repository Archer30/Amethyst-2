#include "stdafx.h"
#include "majaczek.h"


extern int all_creatures;
extern MONSTER_PROP new_monsters[MONSTERS_AMOUNT];

void drop_html_creature_list(const char const *path) {
	if ( auto target = fopen(path, "w"))
	{
		fprintf(target,"<table border=2 width=100%%> \n");

		for (int i = 0; i <= all_creatures; i++)
			fprintf(target,"<tr><td> %i <td> %s \n",i, 
				new_monsters[i].name ? new_monsters[i].name : "NO_NAME");

		fprintf(target, "</table> \n");
		fclose(target);
	}
}