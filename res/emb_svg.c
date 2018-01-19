#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_PATH_LEN 512

char * get_suffix(char *file){
	int i = 0;
	const char *s = file;
	static char suffix[4];
	int found = 0;
	memset(suffix, 0, sizeof(suffix));

	/* extract suffix .xxx from file */
	while (*s++ != '\0') {
		if (found && i < 3) suffix[i++] = *s;

		if (*s == '.') {
			if (found){
				found = 0;
				break;
			}
			found = 1;
		}
	}
	if (!found) suffix[0] = 0;
	return suffix;
}

int listdir(char *dir, char *type) {
	struct dirent *entry;
	DIR *work, *subdir;
	char buffer[MAX_PATH_LEN], name[MAX_PATH_LEN], *suffix, ch, *end;
	FILE *ret_file, *curr_file;

	work = opendir(dir);
	if (work == NULL) {
		perror("opendir\n");
		return 0;
	}
	
	ret_file = fopen("svg_media.h", "w");
	if (ret_file == NULL){
		perror("Failed to open return file\n");
		return 0;
	}
	
	strncpy(buffer, dir, MAX_PATH_LEN);
	int n = strlen(buffer);
	if (n > 0 && (buffer[n-1] != '/')) buffer[n++] = '/';
	
	fprintf (ret_file, "#ifndef _SVG_MEDIA\n#define _SVG_MEDIA\n\n");

	fprintf (ret_file, "enum {\n");
	
	while((entry = readdir(work))){
		strncpy(buffer + n, entry->d_name, MAX_PATH_LEN-n);
		subdir = opendir(buffer);
		if (subdir != NULL) closedir(subdir);
		else{
			suffix = get_suffix(entry->d_name);
			if(strcmp (type, suffix) == 0){
				curr_file = fopen(buffer, "r");
				if (curr_file){
					
					/*strip the suffix of file name*/
					strncpy(name, entry->d_name, MAX_PATH_LEN);
					end = strstr(name, suffix);
					*(end-1) = 0;
					
					/*capitalize name*/
					end = name;
					while (*end= toupper(*end)) end++;
					
					fprintf (ret_file, "    SVG_%s,\n", name);
					fclose(curr_file);
				}
			}
		}
		
	}
	
	fprintf (ret_file, "    SVG_MEDIA_SIZE\n} svg_list;\n\n");
	
	fprintf (ret_file, "char * svg_data[] = {\n");
	
	rewinddir(work);
	while((entry = readdir(work))){
		strncpy(buffer + n, entry->d_name, MAX_PATH_LEN-n);
		subdir = opendir(buffer);
		if (subdir != NULL) closedir(subdir);
		else{
			suffix = get_suffix(entry->d_name);
			if(strcmp (type, suffix) == 0){
				curr_file = fopen(buffer, "r");
				if (curr_file){
					
					/*strip the suffix of file name*/
					strncpy(name, entry->d_name, MAX_PATH_LEN);
					end = strstr(name, suffix);
					*(end-1) = 0;
					
					/*capitalize name*/
					end = name;
					while (*end= toupper(*end)) end++;
					
					fprintf (ret_file, "\t[SVG_%s] = (char[]){\"", name);
					while( ( ch = fgetc(curr_file) ) != EOF ){
						if (ch == '\"') fprintf (ret_file, "\\\"");
						else if (ch == '\n') fprintf (ret_file, "\"\n\t\"");
						else fputc(ch, ret_file);
					}
					fprintf (ret_file, "\"},\n\n");
					fclose(curr_file);
				}
			}
		}
	}
	fprintf (ret_file, "};\n#endif");
	fclose(ret_file);
	closedir(work);
	return 1;
}

int main(int argc, char **argv) {
	if (listdir(".", "svg")){
		printf("OK\n");
	}

	return 0;
}