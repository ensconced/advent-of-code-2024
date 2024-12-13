#include "./lib.h"
#include "./parser.h"
#include <stdio.h>

#define MAX_REGIONS 1024
#define MAX_PLOTS_PER_REGION 512

typedef struct plot {
  int x;
  int y;
} plot;

typedef struct region {
  int id;
  char plant;
  plot *plots;
  size_t plot_count;
} region;

const int offsets[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

size_t region_perimeter(region reg, parsed_input input, int *region_map) {
  size_t result = 0;

  for (size_t i = 0; i < reg.plot_count; i++) {
    plot pl = reg.plots[i];
    for (int i = 0; i < 4; i++) {
      int neighbour_x = pl.x + offsets[i][0];
      int neighbour_y = pl.y + offsets[i][1];

      if (neighbour_x < 0 || neighbour_x >= input.width || neighbour_y < 0 ||
          neighbour_y >= input.height ||
          region_map[neighbour_y * input.height + neighbour_x] != reg.id) {
        result++;
      }
    }
  }

  return result;
}

void explore_region(int x, int y, parsed_input input, region *reg,
                    int *region_map) {
  region_map[y * input.height + x] = reg->id;
  if (reg->plot_count == MAX_PLOTS_PER_REGION) {
    fprintf(stderr, "too many plots\n");
    exit(1);
  }
  reg->plots[reg->plot_count++] = (plot){.x = x, .y = y};

  for (int i = 0; i < 4; i++) {
    int neighbour_x = x + offsets[i][0];
    int neighbour_y = y + offsets[i][1];
    if (neighbour_x >= 0 && neighbour_x < input.width && neighbour_y >= 0 &&
        neighbour_y < input.height &&
        input.rows[neighbour_y][neighbour_x] == reg->plant &&
        region_map[neighbour_y * input.height + neighbour_x] == -1) {
      explore_region(neighbour_x, neighbour_y, input, reg, region_map);
    }
  }
}

void include_plot(int x, int y, parsed_input input, int *region_map,
                  region *regions, size_t *regions_len) {
  if (region_map[y * input.height + x] == -1) {
    region *reg = malloc(sizeof(region));
    *reg = (region){
        .id = (int)(*regions_len),
        .plant = input.rows[y][x],
        .plots = malloc(sizeof(plot) * MAX_PLOTS_PER_REGION),
    };
    explore_region(x, y, input, reg, region_map);
    if (*regions_len == MAX_REGIONS) {
      fprintf(stderr, "too many regions\n");
      exit(1);
    }
    regions[(*regions_len)++] = *reg;
  }
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  int *region_map = malloc(sizeof(int) * (size_t)(input.height * input.width));
  for (int i = 0; i < input.height * input.width; i++) {
    region_map[i] = -1;
  }
  region *regions = malloc(sizeof(region) * MAX_REGIONS);
  size_t regions_len = 0;
  for (int y = 0; y < input.height; y++) {
    for (int x = 0; x < input.width; x++) {
      include_plot(x, y, input, region_map, regions, &regions_len);
    }
  }

  int result = 0;
  for (size_t i = 0; i < regions_len; i++) {
    region reg = regions[i];
    result += region_perimeter(reg, input, region_map) * reg.plot_count;
  }

  return result;
}

int part2(char *input_path) { return 0; }
