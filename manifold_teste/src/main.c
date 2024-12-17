#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "manifold/types.h"
#include "manifold/manifoldc.h"

void *alloc_manifold_buffer() { return malloc(manifold_manifold_size()); }

void *alloc_box_buffer() { return malloc(manifold_box_size()); }

void *alloc_meshgl_buffer() { return malloc(manifold_meshgl_size()); }

void *alloc_meshgl64_buffer() { return malloc(manifold_meshgl64_size()); }

void *alloc_simple_polygon_buffer() {
  return malloc(manifold_simple_polygon_size());
}

void *alloc_polygons_buffer() { return malloc(manifold_polygons_size()); }

void *alloc_manifold_vec_buffer() {
  return malloc(manifold_manifold_vec_size());
}

int main(int argc, char *argv[])
{
  printf("manifold test\n");
  
  int n = 25;
  ManifoldManifold *sphere = manifold_sphere(alloc_manifold_buffer(), 1.0, 4 * n);

  int tri = manifold_num_tri(sphere);
  
  printf("sphere: triangles = %d\n", tri);
  
  ManifoldManifold *cube = manifold_cube(alloc_manifold_buffer(),
    1.0, 2.0, 3.0, 0);
  
  ManifoldMeshGL *mesh =
      manifold_get_meshgl(alloc_meshgl_buffer(), cube);
  
  int prop = manifold_meshgl_num_prop(mesh);
  int vert = manifold_meshgl_num_vert(mesh);
  int trian = manifold_meshgl_num_tri(mesh);
  
  float *coord = manifold_meshgl_vert_properties(
    malloc(sizeof(float) * prop * vert), mesh);
  uint32_t *index = manifold_meshgl_tri_verts(
    malloc(sizeof(uint32_t) * 3 * trian), mesh);
  
  printf("cube: triangles = %d, vert = %d, prop = %d\n", trian, vert, prop);
  
  int i =0;
  for (i = 0; i < vert; i++){
    printf("v%d=(%.2f,%.2f,%.2f)\n", i, coord[prop*i], coord[prop*i+1], coord[prop*i+2]);
  }
  for (i = 0; i < trian; i++){
    printf("t%d=[%d,%d,%d]\n", i, index[3*i], index[3*i+1], index[3*i+2]);
  }
  
  manifold_destruct_meshgl(mesh);
  manifold_destruct_manifold(sphere);
  free(sphere);
  manifold_destruct_manifold(cube);
  free(cube);
  free(mesh);
  free (coord);
  free (index);
}