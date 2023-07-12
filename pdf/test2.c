#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
  
  if (argc < 3) {
    perror("Error: ");
    return -1;
  }
  int i = 0;
  
  if(strcmp(argv[1], "a") == 0){
    if (argc < 4) {
      perror("Error: ");
      return -1;
    }
    
  } else if(strcmp(argv[1], "e") == 0){
    
  } else {
    perror("Error: ");
    return -1;
  }
  
  char buffer[256], *end, root[256], info[256], id[2560];
  
  long int pos = 254, size = 0, prev = 0, len;
  
  FILE *in_file = fopen(argv[2], "rb");
	if(in_file == NULL){
		return 0;
	}
  
  fseek(in_file, -255, SEEK_END);
  
  fread(buffer, 1, 255, in_file);
  for (i = 0; i < 255; i++){
    if (buffer[i] < 9 || buffer[i]>126)
      buffer[i] = ' ';
  }
  
  buffer[255] = 0;
  
  
  char *ref = strstr(buffer, "trailer");
  if (ref) ref += 8; else goto quit_in_file;
  char *value = strstr(ref, "/Size");
  if (value) value = strpbrk(value, "0123456789"); else goto quit_in_file;
  if (value) size = atoi(value); else goto quit_in_file;
  if (!size) goto quit_in_file;
  
  value = strstr(ref, "/Root");
  if (value) end = strchr(value + 5, 'R'); else goto quit_in_file;
  if (end) len = end - value + 1; else goto quit_in_file;
  if (len < 2) goto quit_in_file;
  if (len > 255) goto quit_in_file;
  strncpy(root, value, len);
  root[len]=0;
  
  info[0] = 0;
  value = strstr(ref, "/Info");
  if (value) {
    end = strchr(value + 5, 'R');
    if (end) {
      len = end - value + 1;
      if (len > 1 && len < 255)
        strncpy(info, value, len);
        info[len] = 0;
    }
  }
  
  id[0] = 0;
  value = strstr(ref, "/ID");
  if (value) {
    end = strchr(value + 3, ']');
    if (end) {
      len = end - value + 1;
      if (len > 1 && len < 255)
        strncpy(id, value, len);
        id[len] = 0;
    }
  }
  
  ref = strstr(ref, "startxref");
  if (ref) ref += 10; else goto quit_in_file;
  
  ref = strpbrk(ref, "0123456789");
  
  if (!ref) goto quit_in_file;
  
  prev = atoi(ref);
  if (!prev) goto quit_in_file;
  
  fseek(in_file, 0, SEEK_SET);
  
  
  
  
  if(strcmp(argv[1], "a") == 0){
    
    FILE *a_file = fopen(argv[3], "rb");
    if(a_file == NULL){
      goto quit_in_file;
    }
    
    FILE *out_file = fopen("out.pdf", "wb");
    if(out_file == NULL){
      goto quit_in_file;
    }
    
    
    fseek(a_file, 0, SEEK_END);
    long int stream_len = ftell(a_file);
    fseek(a_file, 0, SEEK_SET);
    
    int c;
    
    while (1){
      c = fgetc(in_file);
      if (feof(in_file)) break;
      fputc(c, out_file);
      
    }
  
    long int obj_pos = ftell(out_file);
    
    fprintf(out_file, "%d 0 obj\r\n", size);
    fprintf(out_file, "<</Type /Grafics /Subtype /Image /Filter /JBIG2Decode /Height 561 /Width 370 /Length %d >>stream\r\n", stream_len);
    
    while (1){
      c = fgetc(a_file);
      if (feof(a_file)) break;
      fputc(c, out_file);
      
    }
    fclose(a_file);
    
    fprintf(out_file, "\r\nendstream\r\n");
    fprintf(out_file, "endobj\r\n");
    long int xref_pos = ftell(out_file);
    fprintf(out_file, "xref\r\n");
    fprintf(out_file, "0 1\r\n0000000000 65535 f\r\n");
    fprintf(out_file, "%d 1\r\n%010ld 00000 n\r\n", size, obj_pos);
    size++;
    fprintf(out_file, "trailer\r\n<<\r\n");
    fprintf(out_file, "/Size %d\r\n", size);
    fprintf(out_file, "%s\r\n", root);
    if (strlen (info)) fprintf(out_file, "%s\r\n", info);
    if (strlen (id)) fprintf(out_file, "%s\r\n", id);
    fprintf(out_file, "/Prev %ld\r\n", prev);
    fprintf(out_file, ">>\r\nstartxref\r\n%ld\r\n%%%%EOF\r\n", xref_pos);
    
    fclose(out_file);
    
    printf("out size = %d\n", size);
  }
  else
  {
    ref = strstr(buffer, "trailer");
    if (ref) *ref = 0; else goto quit_in_file;
    ref = strstr(buffer, " 00000 n");
    if (ref) *ref = 0; else goto quit_in_file;
    ref = strrchr(buffer, '\n');
    if (!ref) goto quit_in_file;
    
    prev = atoi(ref);
    if (!prev) goto quit_in_file;
    
    fseek(in_file, prev, SEEK_SET);
    
    fread(buffer, 1, 255, in_file);
    for (i = 0; i < 255; i++){
      if (buffer[i] < 9 || buffer[i]>126)
        buffer[i] = ' ';
    }
    
    buffer[255] = 0;
    
    ref = strstr(buffer, "7z");
    if (!ref) goto quit_in_file;
    
    prev += ref - buffer;
    
    ref = strstr(buffer, "/Length");
    if (ref) ref += 7; else goto quit_in_file;
    
    ref = strpbrk(ref, "0123456789");
    
    if (!ref) goto quit_in_file;
    
    len = atoi(ref);
    if (!len) goto quit_in_file;
    
    fseek(in_file, prev, SEEK_SET);
    
    
    FILE *out_file = fopen("out.7z", "wb");
    if(out_file == NULL){
      goto quit_in_file;
    }
    
    int c;
    
    for (i = 0; i < len; i++){
      c = fgetc(in_file);
      if (feof(in_file)) break;
      fputc(c, out_file);
      
    }
    fclose(out_file);
    
    printf("out size = %d\n", len);
  }

  fclose(in_file);
  return 0;

quit_in_file:
  fclose(in_file);
  perror("Error: ");
  return -1;
}