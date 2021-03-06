#include "c_tile.h"
#include "c_config.h"
#include "s_map_block.h"
#include "console.h"

int get_border_offset_path(unsigned char in)
{
	if((in&(2|8|32|128)) == (2|8|32|128))
		return 10;
	else if((in&(2|8|128)) == (2|8|128))
		return 9;
	else if((in&(2|32|128)) == (2|32|128))
		return 8;
	else if((in&(8|32|128)) == (8|32|128))
		return 7;
	else if((in&(2|8|32)) == (2|8|32))
		return 6;
	else if((in&(2|128)) == (2|128))
		return 5;
	else if((in&(32|128)) == (32|128))
		return 4;
	else if((in&(8|32)) == (8|32))
		return 3;
	else if((in&(2|8)) == (2|8))
		return 2;
	else if((in&(8|32)) == (8|32))
		return 1;
	else if((in&(2|32)) == (2|32))
		return 0;
	else if((in&2) == 2)
		return 0;
	else if((in&8) == 8)
		return 1;
	else if((in&32) == 32)
		return 0;
	else if((in&128) == 128)
		return 1;
	return -1;
}

int get_border_offset_six(unsigned char in)
{
	if((in&(2|32)) == (2|32))
		return 0;
	else if((in&(8|128)) == (8|128))
		return 1;
	else if((in&(8|32)) == (8|32))
		return 2;
	else if((in&(32|128)) == (32|128))
		return 3;
	else if((in&(2|8)) == (2|8))
		return 4;
	else if((in&(2|128)) == (2|128))
		return 5;
	else if((in&2) == 2)
		return 0;
	else if((in&8) == 8)
		return 1;
	else if((in&32) == 32)
		return 0;
	else if((in&128) == 128)
		return 1;
	return -1;
}

int get_border_offset_four(unsigned char in)
{
	if(((in&2) == 2) && !((in&32) == 32))
		return 3;
	else if(((in&8) == 8) && !((in&128) == 128))
		return 0;
	else if(((in&32) == 32) && !((in&2) == 2))
		return 1;
	else if(((in&128) == 128) && !((in&8) == 8))
		return 2;
	return -1;
}

int get_border_offset_pair(unsigned char in)
{
	if((in&(2|32)) == (2|32))
		return 0;
	else if((in&(8|128)) == (8|128))
		return 1;
	else if((in&2) == 2)
		return 0;
	else if((in&8) == 8)
		return 1;
	else if((in&32) == 32)
		return 0;
	else if((in&128) == 128)
		return 1;
	return -1;
}

ALLEGRO_COLOR mix_colors( ALLEGRO_COLOR lhs, ALLEGRO_COLOR rhs)
{
	lhs.a *= rhs.a;
	lhs.r *= rhs.r;
	lhs.g *= rhs.g;
	lhs.b *= rhs.b;
	return lhs;
}

void draw_sprite(s_sprite sprite, s_map_block * block, float x, float y, int shrink, bool flip = 0, int offset = 0)
{
	if(offset < 0)
		return;
	int target_width = al_get_bitmap_width(al_get_target_bitmap());
	int target_height = al_get_bitmap_height(al_get_target_bitmap());
	if((x + sprite.origin_x > target_width) || (y + sprite.origin_y > target_height) || 
		((x + sprite.origin_x + sprite.width) < 0) || ((y + sprite.origin_y + (sprite.height - shrink)) < 0))return;
	int flags = 0;
	if(flip)
		flags = ALLEGRO_FLIP_HORIZONTAL;
	ALLEGRO_COLOR color;
	if(sprite.color_by == NONE)
		color = block->light;
	else if(sprite.color_by == INI)
		color = mix_colors(sprite.color, block->light) ;
	else if(sprite.color_by == BIOME)
		color = mix_colors(block->color, block->light);
	else color = block->light;
	al_draw_tinted_bitmap_region(
		imagelist.get_image(sprite.index),
		color,
		sprite.x + (sprite.width * offset),
		sprite.y + shrink,
		sprite.width,
		sprite.height - shrink,
		x + sprite.origin_x,
		y + sprite.origin_y,
		flags);
}

e_color_by get_color_selector(const char * text)
{
	if(strcmp(text, "none") == 0)
		return NONE;
	if(strcmp(text, "html") == 0)
		return INI;
	if(strcmp(text, "biome_map") == 0)
		return BIOME;
	return NONE;
}

terrain_type get_terrain_type(const char * text)
{
	if(strcmp(text, "any") == 0)
		return TERRAIN_ANY;
	if(strcmp(text, "none") == 0)
		return TERRAIN_NONE;
	if(strcmp(text, "ocean") == 0)
		return TERRAIN_OCEAN;
	if(strcmp(text, "river") == 0)
		return TERRAIN_RIVER;
	if(strcmp(text, "brook") == 0)
		return TERRAIN_STREAM;
	if(strcmp(text, "swamp") == 0)
		return TERRAIN_SWAMP;
	if(strcmp(text, "marsh") == 0)
		return TERRAIN_SWAMP;
	if(strcmp(text, "grassland_temperate") == 0)
		return TERRAIN_GRASS_TEMP;
	if(strcmp(text, "grassland_tropical") == 0)
		return TERRAIN_GRASS_TROP;
	if(strcmp(text, "forest_temperate") == 0)
		return TERRAIN_FOREST_TEMP;
	if(strcmp(text, "forest_tropical") == 0)
		return TERRAIN_FOREST_TROP;
	if(strcmp(text, "badlands") == 0)
		return TERRAIN_BADLANDS;
	if(strcmp(text, "rock") == 0)
		return TERRAIN_ROCK;
	if(strcmp(text, "beach") == 0)
		return TERRAIN_BEACH;
	if(strcmp(text, "taiga") == 0)
		return TERRAIN_TAIGA;
	if(strcmp(text, "glacier") == 0)
		return TERRAIN_GLACIER;
	if(strcmp(text, "tundra") == 0)
		return TERRAIN_TUNDRA;
	if(strcmp(text, "mountain") == 0)
		return TERRAIN_MOUNTAIN;
	if(strcmp(text, "mountain_tall") == 0)
		return TERRAIN_MOUNTAIN_TALL;

	if(strcmp(text, "village") == 0)
		return TERRAIN_VILLAGE;
	if(strcmp(text, "farm_planted") == 0)
		return TERRAIN_FARM_PLANTED;
	if(strcmp(text, "farm_fallow") == 0)
		return TERRAIN_FARM_FALLOW;
	if(strcmp(text, "farm_pasture") == 0)
		return TERRAIN_FARM_PASTURE;
	if(strcmp(text, "fort_keep") == 0)
		return TERRAIN_FORT_KEEP;
	if(strcmp(text, "fort_wall") == 0)
		return TERRAIN_FORT_WALL;
	if(strcmp(text, "fort_gate") == 0)
		return TERRAIN_FORT_GATE;
	if(strcmp(text, "fort_court") == 0)
		return TERRAIN_FORT_COURT;
	if(strcmp(text, "road") == 0)
		return TERRAIN_ROAD;
	if(strcmp(text, "bridge") == 0)
		return TERRAIN_BRIDGE;
	if(strcmp(text, "tunnel") == 0)
		return TERRAIN_TUNNEL;

	if(strcmp(text, "dwarven_fortress_center") == 0)
		return TERRAIN_DORF_CENTER;
	if(strcmp(text, "dwarven_fortress_walls") == 0)
		return TERRAIN_DORF_EDGE;

	if(strcmp(text, "goblin_fortress_center") == 0)
		return TERRAIN_GOBLIN_CENTER;
	if(strcmp(text, "goblin_fortress_walls") == 0)
		return TERRAIN_GOBLIN_EDGE;

	if(strcmp(text, "elven_retreat_center") == 0)
		return TERRAIN_ELF_CENTER;
	if(strcmp(text, "elven_retreat_walls") == 0)
		return TERRAIN_ELF_EDGE;

	return TERRAIN_ANY;
}

e_offset_type get_offset_type(const char * text)
{
	if(!text)
		return OFFSET_NONE;
	if(strcmp(text, "none") == 0)
		return OFFSET_NONE;
	if(strcmp(text, "path") == 0)
		return OFFSET_PATH;
	if(strcmp(text, "pair") == 0)
		return OFFSET_PAIR;
	if(strcmp(text, "six") == 0)
		return OFFSET_SIX;
	if(strcmp(text, "four") == 0)
		return OFFSET_FOUR;
	if(strcmp(text, "random") == 0)
		return OFFSET_RANDOM;
	return OFFSET_NONE;
}

int get_offset(e_offset_type type, char borders, s_map_block * block, unsigned char amount)
{
	if(type == OFFSET_PATH)
		return get_border_offset_path(borders);
	else if(type == OFFSET_PAIR)
		return get_border_offset_pair(borders);
	else if(type == OFFSET_SIX)
		return get_border_offset_six(borders);
	else if(type == OFFSET_FOUR)
		return get_border_offset_four(borders);
	else if(type == OFFSET_RANDOM)
	{
		int off = amount*block->random;
		if(off >= amount)
			off=0;
		return off;
	}
	return 0;
}

s_sprite::s_sprite(void)
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	index = -1;
	origin_x = 0;
	origin_y = 0;
	column_height = 0;
	color_by=NONE;
	offset_type =  OFFSET_NONE;
	offset_amount = 0;
	border_terrain = TERRAIN_ANY;
}

c_tile::c_tile(void)
{
	height_max = 999;
	height_min = -999;
	rain_min = -999;
	rain_max = 999;
	special_terrain = TERRAIN_ANY;
	priority = 0;
}

c_tile::~c_tile(void)
{
}

void c_tile::draw(float x, float y, int height, int bottom, int surface, s_map_block * block, bool flip)
{
	if ((height-bottom) <= 0)
	{
		for(unsigned int i = 0; i < top_sprites.size(); i++)
		{
			int offset = get_offset(
				top_sprites.at(i).offset_type,
				block->terrain_borders[top_sprites.at(i).border_terrain?top_sprites.at(i).border_terrain:block->terrain],
				block,
				top_sprites.at(i).offset_amount);
			draw_sprite(
				top_sprites.at(i),
				block,
				x,
				y - height,
				0,
				flip,
				offset);
		}
	}
	else
	{
		for(unsigned int i = 0; i < bottom_sprites.size(); i++)
		{
			int num_sections = (height - bottom) / bottom_sprites.at(i).column_height;
			int bottom_section_height = (height - bottom) % bottom_sprites.at(i).column_height;
			int offset = get_offset(
				bottom_sprites.at(i).offset_type,
				block->terrain_borders[bottom_sprites.at(i).border_terrain?bottom_sprites.at(i).border_terrain:block->terrain],
				block,
				bottom_sprites.at(i).offset_amount);
			if(bottom_section_height)
			{
				draw_sprite(
					bottom_sprites.at(i), 
					block,
					x, 
					y - height + (num_sections * bottom_sprites.at(i).column_height),
					bottom_sprites.at(i).column_height - bottom_section_height,
					flip,
					offset);
			}
			for( int sec = 0; sec < num_sections; sec++)
			{
				draw_sprite(
					bottom_sprites.at(i), 
					block,
					x, 
					y - height + (sec * bottom_sprites.at(i).column_height),
					0,
					flip,
					offset);
			}
		}
		for(unsigned int i = 0; i < top_sprites.size(); i++)
		{
			int offset = get_offset(
				top_sprites.at(i).offset_type,
				block->terrain_borders[top_sprites.at(i).border_terrain?top_sprites.at(i).border_terrain:block->terrain],
				block,
				top_sprites.at(i).offset_amount);
			draw_sprite(
				top_sprites.at(i),
				block,
				x,
				y - height,
				0,
				flip,
				offset);
		}
	}
	if(height < surface)
	{
		for(unsigned int i = 0; i < intermediate_sprites.size(); i++)
		{
			int num_sections = (surface - height) / intermediate_sprites.at(i).column_height;
			int bottom_section_height = (surface - height) % intermediate_sprites.at(i).column_height;
			int offset = get_offset(
				intermediate_sprites.at(i).offset_type,
				block->terrain_borders[intermediate_sprites.at(i).border_terrain?intermediate_sprites.at(i).border_terrain:block->terrain],
				block,
				intermediate_sprites.at(i).offset_amount);
			if(bottom_section_height)
			{
				draw_sprite(
					intermediate_sprites.at(i), 
					block,
					x, 
					y - surface + (num_sections * intermediate_sprites.at(i).column_height),
					intermediate_sprites.at(i).column_height - bottom_section_height,
					flip,
					offset);
			}
			for( int sec = 0; sec < num_sections; sec++)
			{
				draw_sprite(
					intermediate_sprites.at(i), 
					block,
					x, 
					y - surface + (sec * intermediate_sprites.at(i).column_height),
					0,
					flip,
					offset);
			}
		}
		for(unsigned int i = 0; i < surface_sprites.size(); i++)
		{
			int offset = get_offset(
				surface_sprites.at(i).offset_type,
				block->terrain_borders[surface_sprites.at(i).border_terrain?surface_sprites.at(i).border_terrain:block->terrain],
				block,
				surface_sprites.at(i).offset_amount);
			draw_sprite(
				surface_sprites.at(i),
				block,
				x,
				y-surface,
				0,
				flip,
				offset);
		}
	}
}

void c_tile::load_ini(ALLEGRO_PATH * path)
{
	ALLEGRO_CONFIG * config = 0;

	const char * thepath = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);
	log_printf("Loading tile at: %s\n", thepath);
	config = al_load_config_file(thepath);
	if(!config)
	{
		DisplayErr("Cannot load tile: %s\n", thepath);
		exit(1);
	}
	char buffer[256];

	height_max = get_config_int(config, "SPRITE", "height_max", 280);
	height_min = get_config_int(config, "SPRITE", "height_min", 0);

	temp_max = get_config_int(config, "SPRITE", "temperature_max", 256);
	temp_min = get_config_int(config, "SPRITE", "temperature_min", 0);

	rain_max = get_config_int(config, "SPRITE", "rainfall_max", 256);
	rain_min = get_config_int(config, "SPRITE", "rainfall_min", 0);

	drain_max = get_config_int(config, "SPRITE", "drainage_max", 256);
	drain_min = get_config_int(config, "SPRITE", "drainage_min", 0);

	sav_max = get_config_int(config, "SPRITE", "savagery_max", 256);
	sav_min = get_config_int(config, "SPRITE", "savagery_min", 0);

	vol_max = get_config_int(config, "SPRITE", "volcanism_max", 256);
	vol_min = get_config_int(config, "SPRITE", "volcanism_min", 0);

	veg_max = get_config_int(config, "SPRITE", "vegetation_max", 256);
	veg_min = get_config_int(config, "SPRITE", "vegetation_min", 0);

	evil_max = get_config_int(config, "SPRITE", "evil_max", 256);
	evil_min = get_config_int(config, "SPRITE", "evil_min", 0);

	sal_max = get_config_int(config, "SPRITE", "salinity_max", 256);
	sal_min = get_config_int(config, "SPRITE", "salinity_min", 0);

	priority = get_config_int(config, "SPRITE", "priority", 0);

	const char * terra = al_get_config_value(config, "SPRITE", "special_terrain");
	if(terra)
		special_terrain = get_terrain_type(terra);

	s_sprite temp;
	size_t cap_layers = get_config_int(config, "SPRITE", "cap_layers");
	for(size_t i = 0; i < cap_layers; i++)
	{
		sprintf(buffer, "CAP_IMAGE_%d", i);
		temp = get_from_ini(config, buffer, path);
		if(temp.index >= 0)
			top_sprites.push_back(temp);
	}

	size_t column_layers = get_config_int(config, "SPRITE", "column_layers");
	for(size_t i = 0; i < column_layers; i++)
	{
		sprintf(buffer, "COLUMN_IMAGE_%d", i);
		temp = get_from_ini(config, buffer, path);
		if(temp.index >= 0)
			bottom_sprites.push_back(temp);
	}

	size_t surface_layers = get_config_int(config, "SPRITE", "surface_layers");
	for(size_t i = 0; i < surface_layers; i++)
	{
		sprintf(buffer, "SURFACE_IMAGE_%d", i);
		temp = get_from_ini(config, buffer, path);
		if(temp.index >= 0)
			surface_sprites.push_back(temp);
	}

	size_t intermediate_layers = get_config_int(config, "SPRITE", "intermediate_layers");
	for(size_t i = 0; i < intermediate_layers; i++)
	{
		sprintf(buffer, "INTERMEDIATE_IMAGE_%d", i);
		temp = get_from_ini(config, buffer, path);
		if(temp.index >= 0)
			intermediate_sprites.push_back(temp);
	}

}

s_sprite c_tile::get_from_ini(ALLEGRO_CONFIG *config, const char * section, ALLEGRO_PATH * base_path)
{
	s_sprite temp;

	const char * buffer_file = al_get_config_value(config, section, "image_file");
	if(!buffer_file)
	{
		temp.index = -1;
		return temp;
	}
	ALLEGRO_PATH * imagepath = al_create_path(buffer_file);
	al_rebase_path(base_path, imagepath);
	temp.index = imagelist.load_image(al_path_cstr(imagepath, ALLEGRO_NATIVE_PATH_SEP));

	temp.x = get_config_int(config, section, "x");
	temp.y = get_config_int(config, section, "y");

	temp.width = get_config_int(config, section, "width");
	temp.height = get_config_int(config, section, "height");

	temp.origin_x = get_config_int(config, section, "origin_x");
	temp.origin_y = get_config_int(config, section, "origin_y");

	temp.origin_x = 0 - temp.origin_x;
	temp.origin_y = 0 - temp.origin_y;

	temp.column_height = get_config_int(config, section, "column_height");

	const char * color_selection = al_get_config_value(config, section, "color_source");
	if(color_selection)
		temp.color_by = get_color_selector(color_selection);

	const char * color = al_get_config_value(config, section, "color_html");
	if(color)
		temp.color = color_html(color);

	const char * off = al_get_config_value(config, section, "offset_type");
	if(off)
		temp.offset_type = get_offset_type(off);

	const char * neigh = al_get_config_value(config, section, "border_terrain");
	if(neigh)
		temp.border_terrain = get_terrain_type(neigh);

	temp.offset_amount = get_config_int(config, section, "offset_amount");

	al_destroy_path(imagepath);
	return temp;
}