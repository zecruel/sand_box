// Call C from JavaScript Example

#include "webui.h"

#include "sqlite3.h"

sqlite3 *db;
int changes;
char *buffer;
int pos;
#define BUF_SIZE 10485760

void my_function_string(webui_event_t* e) {

    // JavaScript:
    // webui_fn('MyID_One', 'Hello');

    const char* str = webui_get_string(e);
    printf("my_function_string: %s\n", str); // Hello

    // Need Multiple Arguments?
    //
    // WebUI support only one argument. To get multiple arguments
    // you can send a JSON string from JavaScript then decode it.
    // Example:
    //
    // my_json = my_json_decoder(str);
    // foo = my_json[0];
    // bar = my_json[1];
}

void my_function_integer(webui_event_t* e) {

    // JavaScript:
    // webui_fn('MyID_Two', 123456789);

    long long number = webui_get_int(e);
    printf("my_function_integer: %lld\n", number); // 123456789
}

void my_function_boolean(webui_event_t* e) {

    // JavaScript:
    // webui_fn('MyID_Three', true);

    bool status = webui_get_bool(e); // True
    if(status)
        printf("my_function_boolean: True\n");
    else
        printf("my_function_boolean: False\n");
}

void my_function_with_response(webui_event_t* e) {

    // JavaScript:
    // const result = webui_fn('MyID_Four', number);

    long long number = webui_get_int(e);
    number = number * 2;
    printf("my_function_with_response: %lld\n", number);

    // Send back the response to JavaScript
    webui_return_int(e, number);
}

void sqlite_exec(webui_event_t* e){
	
	const char* st = webui_get_string(e);
	
	/* verify passed arguments */
	buffer[0] = 0;
  pos = 0;
	sqlite3_stmt *stmt;
	
	/* sqlite full statement cycle - prepare/step(s)/finalize */
	sqlite3_prepare_v2(db, st, -1, &stmt, NULL);
  
  pos +=snprintf(buffer + pos, BUF_SIZE - pos, "{");
	int data = 0;
	int err = sqlite3_step(stmt);
  if (err == SQLITE_ROW) {
    data = 1;
    pos +=snprintf(buffer + pos, BUF_SIZE - pos, "\"data\":[");
  }
	
	while (err != SQLITE_DONE) {
		if (err ==  SQLITE_ERROR) break;
    if (err == SQLITE_ROW) {
      int i, n = sqlite3_data_count(stmt);
      if (data == 1) {
        pos +=snprintf(buffer + pos, BUF_SIZE - pos, "{");
        data = 2;
      }
      else pos +=snprintf(buffer + pos, BUF_SIZE - pos, ",{");
      for (i = 0; i < n; i++) {
        const char* key = sqlite3_column_name(stmt, i);
        
        if (i == 0) pos +=snprintf(buffer + pos, BUF_SIZE - pos, "\"%s\":", key);
        else pos +=snprintf(buffer + pos, BUF_SIZE - pos, ",\"%s\":", key);
        switch (sqlite3_column_type(stmt, i)) {
          case SQLITE_INTEGER:
            //lua_pushinteger(L, sqlite3_column_int64(stmt, i));
            pos +=snprintf(buffer + pos, BUF_SIZE - pos, "%lld", sqlite3_column_int64(stmt, i));
            break;
          case SQLITE_FLOAT:
            //lua_pushnumber(L, sqlite3_column_double(stmt, i));
            pos +=snprintf(buffer + pos, BUF_SIZE - pos, "%f", sqlite3_column_double(stmt, i));
            break;
          case SQLITE_TEXT:
          case SQLITE_BLOB: {
            pos +=snprintf(buffer + pos, BUF_SIZE - pos, "\"");
            const char *p = sqlite3_column_blob(stmt, i);
            if (p) {
              int len = sqlite3_column_bytes(stmt, i), j;
              if (len + pos < BUF_SIZE){
                for (j = 0; j<len; j++){
                  if (p[j] == '\\' && pos < BUF_SIZE - 1){
                    buffer[pos] = '\\';
                    pos++;
                    buffer[pos] = '\\';
                  }
                  else if (p[j] == '"' && pos < BUF_SIZE - 1){
                    buffer[pos] = '\\';
                    pos++;
                    buffer[pos] = '"';
                  }
                  else buffer[pos] = p[j];
                  pos++;
                }
              }
              //if (len + pos < BUF_SIZE) pos +=snprintf(buffer + pos, len, "%s", p);
              //lua_pushlstring(L, p, sqlite3_column_bytes(stmt, i));
            }
            pos +=snprintf(buffer + pos, BUF_SIZE - pos, "\"");
            break;
          }
          case SQLITE_NULL:
          default:
            pos +=snprintf(buffer + pos, BUF_SIZE - pos, "\"\"");
          break;
        }
      }
      pos +=snprintf(buffer + pos, BUF_SIZE - pos, "}");
    }
    err = sqlite3_step(stmt);
	}
	sqlite3_finalize(stmt);
  if (data) pos +=snprintf(buffer + pos, BUF_SIZE - pos, "]");
	
	if (err == SQLITE_DONE || err == SQLITE_OK) {
		/* return number of current changes */
		changes = sqlite3_total_changes(db);
    if (data) pos +=snprintf(buffer + pos, BUF_SIZE - pos, ",\"changes\":%d", changes);
    else pos +=snprintf(buffer + pos, BUF_SIZE - pos, "\"changes\":%d", changes);
    data = 3;
		
	} else {
		if (data) pos +=snprintf(buffer + pos, BUF_SIZE - pos, ",\"error\":\"error\"");
    else pos +=snprintf(buffer + pos, BUF_SIZE - pos, "\"error\":\"error\"");
	}
  
  pos +=snprintf(buffer + pos, BUF_SIZE - pos, "}");
	webui_return_string(e, buffer);
}

int main() {
  
  sqlite3_open("projeto.db", &db);
  changes = 0;
  pos = 0;
  buffer = malloc(BUF_SIZE+1);

    /* HTML
    const char* my_html = 
    "<html>"
    "  <head>"
    "    <title>Call C from JavaScript Example</title>"
    "    <style>"
    "      body {"
    "        color: white;"
    "        background: #0F2027;"
    "        text-align: center;"
    "        font-size: 16px;"
    "        font-family: sans-serif;"
    "      }"
    "    </style>"
    "  </head>"
    "  <body>"
    "    <h2>WebUI - Call C from JavaScript Example</h2>"
    "    <p>Call C function with argument (<em>See the logs in your terminal</em>)</p>"
    "    <br>"
    "    <button onclick=\"webui_fn('MyID_One', 'Hello');\">Call my_function_string()</button>"
    "    <br>"
    "    <br>"
    "    <button onclick=\"webui_fn('MyID_Two', 123456789);\">Call my_function_integer()</button>"
    "    <br>"
    "    <br>"
    "    <button onclick=\"webui_fn('MyID_Three', true);\">Call my_function_boolean()</button>"
    "    <br>"
    "    <br>"
    "    <p>Call C function and wait for the response</p>"
    "    <br>"
    "    <button onclick=\"MyJS();\">Call my_function_with_response()</button>"
    "    <br>"
    "    <br>"
    "    <input type=\"text\" id=\"MyInputID\" value=\"2\">"
    "    <script>"
    "      function MyJS() {"
    "        const MyInput = document.getElementById('MyInputID');"
    "        const number = MyInput.value;"
    "        webui_fn('MyID_Four', number).then((response) => {"
    "            MyInput.value = response;"
    "        });"
    "      }"
    "    </script>"
    "  </body>"
    "</html>";
    */
    // Create a window
    size_t my_window = webui_new_window();

    // Bind HTML elements with C functions
    webui_bind(my_window, "MyID_One", my_function_string);
    webui_bind(my_window, "MyID_Two", my_function_integer);
    webui_bind(my_window, "MyID_Three", my_function_boolean);
    webui_bind(my_window, "MyID_Four", my_function_with_response);
    webui_bind(my_window, "Sqlite", sqlite_exec);

    // Show the window
    webui_show(my_window, "test.html"); // webui_show_browser(my_window, my_html, Chrome);

    // Wait until all windows get closed
    webui_wait();
    sqlite3_close_v2(db);
    free(buffer);
    return 0;
}

#if defined(_MSC_VER)
    int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
        return main();
    }
#endif
