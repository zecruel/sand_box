//shapping text using HarfBuzz
// compile: g++ shapping.c lib_harf.a -o teste.exe -I./harfbuzz_core/ -lm -static-libgcc -static-libstdc++ -lmingw32 -lm

#include <stdio.h>
#include "hb.h"

int main(int argc, char *argv[]){
  
  //Create a buffer and put your text in it.
  hb_buffer_t *buf;
  buf = hb_buffer_create();
  hb_buffer_add_utf8(buf, "ação", -1, 0, -1);

  //Set the script, language and direction of the buffer.

  // If you know the direction, script, and language
  //hb_buffer_set_direction(buf, HB_DIRECTION_LTR);
  //hb_buffer_set_script(buf, HB_SCRIPT_LATIN);
  //hb_buffer_set_language(buf, hb_language_from_string("en", -1));

  // If you don't know the direction, script, and language
  hb_buffer_guess_segment_properties(buf);


  //Create a face and a font from a font file.

  hb_blob_t *blob = hb_blob_create_from_file("GoNotoKurrent-Regular.ttf"); /* or hb_blob_create_from_file_or_fail() */
  hb_face_t *face = hb_face_create(blob, 0);
  hb_font_t *font = hb_font_create(face);

  //Shape!

  hb_shape(font, buf, NULL, 0);

  //Get the glyph and position information.

  unsigned int glyph_count;
  hb_glyph_info_t *glyph_info    = hb_buffer_get_glyph_infos(buf, &glyph_count);
  hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buf, &glyph_count);

  //Iterate over each glyph.

  hb_position_t cursor_x = 0;
  hb_position_t cursor_y = 0;
  for (unsigned int i = 0; i < glyph_count; i++) {
      hb_codepoint_t glyphid  = glyph_info[i].codepoint;
      hb_position_t x_offset  = glyph_pos[i].x_offset;
      hb_position_t y_offset  = glyph_pos[i].y_offset;
      hb_position_t x_advance = glyph_pos[i].x_advance;
      hb_position_t y_advance = glyph_pos[i].y_advance;
    /* draw_glyph(glyphid, cursor_x + x_offset, cursor_y + y_offset); */
      printf("cp = %d,\tofs = %d,%d,\tcur = %d,%d\n", glyphid, x_offset, y_offset, cursor_x, cursor_y);
      cursor_x += x_advance;
      cursor_y += y_advance;
  }


  //Tidy up.

  hb_buffer_destroy(buf);
  hb_font_destroy(font);
  hb_face_destroy(face);
  hb_blob_destroy(blob);
  
  return 0;
}