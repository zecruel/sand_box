#include "dxf_hatch.h"

int dxf_parse_patt(char *buf, struct h_pattern *ret){
	static char *line, *cur_line, *next_line, line_buf[DXF_MAX_CHARS];
	int line_size;
	
	cur_line = buf;
	
	char s[2] = ",";
	char *token;
	
	double ang, org_x, org_y, delta_x, delta_y, dashes[20];
	int num_dash;
	int pos = 0, init = 0, i;
	
	struct h_pattern *curr_h = ret;
	struct hatch_line **curr_l = NULL;
	
	while(cur_line){
		next_line = strchr(cur_line, '\n');
		if (next_line) {
			line_size = next_line - cur_line;
			if (line_size > DXF_MAX_CHARS - 1) line_size = DXF_MAX_CHARS - 1;
			memcpy(line_buf, cur_line, line_size);
			line_buf[line_size] = '\0';  /*terminate the current line*/
		}
		else{
			strncpy(line_buf, cur_line, DXF_MAX_CHARS);
		}
		
		/* trim leading/trailing whitespace of line */
		line = trimwhitespace(line_buf);
		s[0] = ','; s[1] = 0; /* standard separator*/
		
		if (line[0] == '*'){
			/* get the first token */
			token = strtok(line, s);
			pos = 0;
			init = 0;
			
			/* walk through other tokens */
			while( token != NULL ) {
				token = trimwhitespace(token);
				
				if (pos == 0) {
					if (curr_h){
						curr_h->next = malloc(sizeof(struct h_pattern));
						if (curr_h->next) {
							curr_h = curr_h->next;
							curr_h->next = NULL;
							curr_h->lines = NULL;
							curr_h->name[0] = 0; /*clear strings*/
							curr_h->descr[0] = 0;
							curr_h->num_lines = 0;
							
							curr_l = &(curr_h->lines);
							
							init = 1;
						}
					}
					if (init){
						token++; /*skip '*' in name */
						strncpy(curr_h->name, token, DXF_MAX_CHARS);
						s[0] = '\n'; s[1] = 0; /* change separator*/
					}
				}
				else {
					if (init) strncpy(curr_h->descr, token, DXF_MAX_CHARS);
				}

				token = strtok(NULL, s);
				pos++;
			}
		}
		else if((init) && ((isdigit(line[0])) || (line[0] == '.') || (line[0] == '-'))){
			/* get the first token */
			token = strtok(line, s);
			pos = 0;
			num_dash = 0;
			
			/* walk through other tokens */
			while( token != NULL ) {
				token = trimwhitespace(token);
				
				if (pos == 0) {
					if (curr_l){
						*curr_l = malloc(sizeof(struct hatch_line));
						if(*curr_l) (*curr_l)->next = NULL;
					}
					
					ang = atof(token);
				}
				else if (pos == 1) {
					org_x = atof(token);
				}
				else if (pos == 2) {
					org_y = atof(token);
				}
				else if (pos == 3) {
					delta_x = atof(token);
				}
				else if (pos == 4) {
					delta_y = atof(token);
				}
				else if ((pos > 4) && (num_dash < 20)){
					dashes[num_dash] = atof(token);
					//printf( "%0.2f, ", dashes[num_dash] );
					num_dash++;
				}

				token = strtok(NULL, s);
				pos++;
			}
			if(*curr_l){
				(*curr_l)->ang = ang;
				(*curr_l)->ox = org_x;
				(*curr_l)->oy = org_y;
				(*curr_l)->dx = delta_x;
				(*curr_l)->dy = delta_y;
				(*curr_l)->num_dash = num_dash;
				for (i = 0; i < num_dash; i++){
					(*curr_l)->dash[i] = dashes[i];
				}
				
				curr_h->num_lines++;
				
				curr_l = &((*curr_l)->next);
				//printf("%s|%s|%0.2f|%0.2f|%0.2f|%0.2f|%0.2f|%d\n", name, descr, ang, org_x, org_y, delta_x, delta_y, num_dash);
			}
		}
		else init = 0;
		
		cur_line = next_line ? (next_line+1) : NULL;
	}
	return 1;
}

int dxf_hatch_free (struct h_pattern *hatch){
	struct h_pattern *curr_h = NULL;
	struct h_pattern *next_h = NULL;
	struct hatch_line *curr_l = NULL;
	struct hatch_line *next_l = NULL;
	
	curr_h = hatch;
	
	while (curr_h){
		//printf("Name = %s, Descr: %s\n", curr_h->name, curr_h->descr);
		curr_l = curr_h->lines;
		
		while (curr_l){
			next_l = curr_l->next;
			free(curr_l);
			curr_l = next_l;
		}
		
		
		next_h = curr_h->next;
		free(curr_h);
		curr_h = next_h;
	}
	return 1;
}