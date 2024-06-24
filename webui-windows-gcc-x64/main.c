// Call C from JavaScript Example
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#include <dirent.h>

#include "webui.h"
#include "whereami.h"

#include "sqlite3.h"

#define KGFLAGS_IMPLEMENTATION
#include "kgflags.h"

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define PATH_SEPARATOR ';'
#define DIR_SEPARATOR '\\'
#else
#define PATH_SEPARATOR ':'
#define DIR_SEPARATOR '/'
#endif

#define PATH_MAX_CHARS 512

sqlite3 *db;
int changes;
char *buffer;
int pos;
#define BUF_SIZE 10485760

const char * icon = "<svg width=\"80.0000\" height=\"80.0000\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">"
"<path stroke=\"rgb(128, 128, 128)\" fill=\"none\" d=\"M2.863 41.8 L36.42 41.96 L26.32 31.86 L25.22 30.52 L24.28 29.06 L23.52 27.51 L22.95 25.87 L22.58 24.18 L22.41 22.45 L22.45 20.72 L22.69 19 L23.14 17.33 L23.78 15.72 L24.6 14.19 L25.6 12.78 L26.76 11.49 L28.07 10.35 L29.5 9.377 L31.04 8.577 L32.66 7.964 L34.34 7.549 L36.06 7.335 L37.8 7.327 L39.52 7.525 L41.2 7.925 L42.83 8.522 L44.37 9.308 L45.82 10.27 L47.13 11.4 L48.3 12.67 L49.32 14.08 L50.16 15.6 L50.81 17.2 L51.27 18.87 L51.53 20.59 L39.37 20.59 L39.12 20.57 L38.87 20.58 L38.63 20.59 L38.38 20.63 L38.13 20.67 L37.89 20.73 L37.65 20.81 L37.42 20.89 L37.19 21 L36.97 21.11 L36.76 21.24 L36.55 21.38 L36.35 21.53 L36.16 21.69 L35.98 21.86 L35.81 22.04 L35.66 22.24 L35.51 22.44 L35.37 22.65 L35.25 22.86 L35.14 23.09 L35.04 23.32 L34.96 23.55 L34.89 23.79 L34.83 24.03 L34.79 24.28 L34.77 24.53 L34.75 24.78 L34.75 25.02 L34.77 25.27 L34.8 25.52 L34.85 25.77 L51.56 42.67 L51.77 42.86 L51.97 43.04 L52.18 43.22 L52.4 43.39 L52.61 43.57 L52.84 43.73 L53.06 43.9 L53.29 44.05 L53.51 44.21 L53.75 44.36 L53.98 44.51 L54.22 44.65 L54.46 44.78 L54.7 44.92 L54.95 45.05 L55.19 45.17 L55.44 45.29 L55.7 45.4 L55.95 45.51 L56.21 45.62 L56.46 45.72 L56.72 45.81 L56.99 45.9 L57.25 45.99 L57.51 46.07 L57.78 46.14 L58.05 46.21 L58.32 46.28 L58.59 46.34 L58.86 46.4 L59.13 46.45 L59.4 46.49 L60.1 47.28 L60.75 48.11 L61.34 48.98 L61.87 49.9 L62.34 50.84 L62.74 51.82 L63.08 52.82 L63.35 53.84 L63.55 54.87 L63.67 55.92 L63.73 56.97 L63.72 58.03 L63.63 59.08 L63.48 60.12 L63.25 61.15 L62.95 62.16 L62.59 63.15 L62.16 64.12 L61.67 65.05 L61.11 65.95 L60.5 66.8 L59.83 67.62 L59.11 68.39 L58.33 69.1 L57.51 69.76 L56.65 70.37 L55.75 70.92 L54.81 71.4 L53.84 71.82 L52.85 72.17 L51.83 72.46 L50.8 72.67 L24.67 72.67 L23.39 72.39 L22.14 72.05 L20.9 71.64 L19.68 71.18 L18.48 70.67 L17.31 70.09 L16.17 69.46 L15.06 68.78 L13.98 68.04 L12.94 67.26 L11.94 66.43 L10.98 65.54 L10.06 64.62 L9.192 63.65 L8.367 62.64 L7.59 61.6 L6.863 60.51 L6.189 59.4 L5.569 58.25 L5.004 57.08 L4.496 55.88 L4.045 54.66 L3.653 53.41 L3.32 52.15 L3.048 50.88 L2.836 49.59 L2.686 48.3 L2.598 47 L2.571 45.7 L2.607 44.39 L2.704 43.09 L2.863 41.8 \"/>"
"<path stroke=\"rgb(255, 128, 0)\" fill=\"none\" d=\"M34.85 25.77 L34.8 25.52 L34.77 25.27 L34.75 25.02 L34.75 24.78 L34.77 24.53 L34.79 24.28 L34.83 24.03 L34.89 23.79 L34.96 23.55 L35.04 23.32 L35.14 23.09 L35.25 22.86 L35.37 22.65 L35.51 22.44 L35.66 22.24 L35.81 22.04 L35.98 21.86 L36.16 21.69 L36.35 21.53 L36.55 21.38 L36.76 21.24 L36.97 21.11 L37.19 21 L37.42 20.89 L37.65 20.81 L37.89 20.73 L38.13 20.67 L38.38 20.63 L38.63 20.59 L38.87 20.58 L39.12 20.57 L39.37 20.59 L72.91 20.59 L73.15 20.61 L73.38 20.65 L73.61 20.7 L73.83 20.75 L74.06 20.82 L74.28 20.9 L74.5 20.99 L74.71 21.09 L74.92 21.2 L75.12 21.33 L75.31 21.45 L75.5 21.59 L75.69 21.74 L75.86 21.9 L76.03 22.06 L76.19 22.23 L76.34 22.41 L76.48 22.6 L76.62 22.79 L76.74 22.99 L76.86 23.2 L76.96 23.41 L77.06 23.62 L77.14 23.84 L77.21 24.07 L77.28 24.29 L77.33 24.52 L77.37 24.75 L77.4 24.98 L77.42 25.22 L77.43 25.45 L77.43 25.69 L77.43 31.77 L77.35 32.87 L77.2 33.96 L76.97 35.03 L76.66 36.09 L76.28 37.12 L75.83 38.12 L75.3 39.09 L74.71 40.02 L74.06 40.9 L73.34 41.73 L72.57 42.52 L71.74 43.24 L70.86 43.9 L69.94 44.5 L68.98 45.04 L67.98 45.5 L66.95 45.89 L65.9 46.2 L64.83 46.45 L63.74 46.61 L62.64 46.7 L61.54 46.7 L60.45 46.63 L59.36 46.49 L58.28 46.26 L57.22 45.96 L56.19 45.58 L55.19 45.14 L54.22 44.62 L53.29 44.03 L52.4 43.38 L51.56 42.67 L34.85 25.77 \"/>"
"<path stroke=\"rgb(192, 192, 192)\" fill=\"none\" d=\"M15 46.91 L39.47 46.91 L40.33 47.07 L41.18 47.29 L42.01 47.59 L42.8 47.96 L43.56 48.41 L44.28 48.91 L44.95 49.48 L45.56 50.1 L46.13 50.78 L46.62 51.5 L47.06 52.26 L47.42 53.06 L47.72 53.89 L47.94 54.74 L48.08 55.6 L48.15 56.48 L48.14 57.36 L48.05 58.23 L47.88 59.09 L47.65 59.94 L47.33 60.76 L46.95 61.55 L46.5 62.3 L45.98 63.01 L45.41 63.67 L44.77 64.28 L44.09 64.83 L43.36 65.32 L42.59 65.74 L41.79 66.1 L40.96 66.38 L40.1 66.58 L38.98 66.85 L37.84 67.05 L36.7 67.19 L35.54 67.26 L34.39 67.26 L33.24 67.19 L32.09 67.06 L30.95 66.87 L29.83 66.6 L28.72 66.27 L27.63 65.88 L26.57 65.43 L25.54 64.91 L24.53 64.34 L23.57 63.71 L22.64 63.02 L21.75 62.29 L20.9 61.5 L20.11 60.66 L19.36 59.78 L18.67 58.86 L18.03 57.9 L17.44 56.9 L16.92 55.87 L16.45 54.81 L16.05 53.73 L15.71 52.63 L15.44 51.5 L15.23 50.37 L15.09 49.22 L15.01 48.07 L15 46.91 \"/>"
"<path stroke=\"rgb(254, 254, 254)\" fill=\"none\" d=\"M38.44 17.03 L38.44 17.13 L38.45 17.23 L38.47 17.33 L38.51 17.43 L38.55 17.52 L38.6 17.61 L38.66 17.69 L38.73 17.77 L38.8 17.84 L38.88 17.9 L38.97 17.96 L39.06 18 L39.16 18.04 L39.25 18.07 L39.35 18.08 L39.46 18.09 L39.56 18.09 L39.66 18.08 L39.76 18.05 L39.86 18.02 L39.95 17.98 L40.04 17.93 L40.12 17.87 L40.2 17.8 L40.27 17.73 L40.33 17.65 L40.39 17.56 L40.44 17.47 L40.47 17.37 L40.5 17.27 L40.52 17.17 L40.52 17.07 L40.52 16.97 L40.51 16.87 L40.48 16.77 L40.45 16.67 L40.41 16.58 L40.36 16.49 L40.3 16.4 L40.23 16.33 L40.16 16.26 L40.08 16.19 L39.99 16.14 L39.9 16.09 L39.8 16.06 L39.7 16.03 L39.6 16.01 L39.5 16 L39.4 16.01 L39.3 16.02 L39.2 16.04 L39.1 16.08 L39.01 16.12 L38.92 16.17 L38.83 16.23 L38.76 16.29 L38.69 16.37 L38.62 16.45 L38.57 16.54 L38.52 16.63 L38.49 16.72 L38.46 16.82 L38.44 16.92 L38.44 17.03 \"/>"
"<path fill=\"rgb(254, 254, 254)\" stroke=\"none\" d=\"M2.863 41.8 L36.42 41.96 L26.32 31.86 L25.22 30.52 L24.28 29.06 L23.52 27.51 L22.95 25.87 L22.58 24.18 L22.41 22.45 L22.45 20.72 L22.69 19 L23.14 17.33 L23.78 15.72 L24.6 14.19 L25.6 12.78 L26.76 11.49 L28.07 10.35 L29.5 9.377 L31.04 8.577 L32.66 7.964 L34.34 7.549 L36.06 7.335 L37.8 7.327 L39.52 7.525 L41.2 7.925 L42.83 8.522 L44.37 9.308 L45.82 10.27 L47.13 11.4 L48.3 12.67 L49.32 14.08 L50.16 15.6 L50.81 17.2 L51.27 18.87 L51.53 20.59 L39.37 20.59 L39.12 20.57 L38.87 20.58 L38.63 20.59 L38.38 20.63 L38.13 20.67 L37.89 20.73 L37.65 20.81 L37.42 20.89 L37.19 21 L36.97 21.11 L36.76 21.24 L36.55 21.38 L36.35 21.53 L36.16 21.69 L35.98 21.86 L35.81 22.04 L35.66 22.24 L35.51 22.44 L35.37 22.65 L35.25 22.86 L35.14 23.09 L35.04 23.32 L34.96 23.55 L34.89 23.79 L34.83 24.03 L34.79 24.28 L34.77 24.53 L34.75 24.78 L34.75 25.02 L34.77 25.27 L34.8 25.52 L34.85 25.77 L51.56 42.67 L51.77 42.86 L51.97 43.04 L52.18 43.22 L52.4 43.39 L52.61 43.57 L52.84 43.73 L53.06 43.9 L53.29 44.05 L53.51 44.21 L53.75 44.36 L53.98 44.51 L54.22 44.65 L54.46 44.78 L54.7 44.92 L54.95 45.05 L55.19 45.17 L55.44 45.29 L55.7 45.4 L55.95 45.51 L56.21 45.62 L56.46 45.72 L56.72 45.81 L56.99 45.9 L57.25 45.99 L57.51 46.07 L57.78 46.14 L58.05 46.21 L58.32 46.28 L58.59 46.34 L58.86 46.4 L59.13 46.45 L59.4 46.49 L60.1 47.28 L60.75 48.11 L61.34 48.98 L61.87 49.9 L62.34 50.84 L62.74 51.82 L63.08 52.82 L63.35 53.84 L63.55 54.87 L63.67 55.92 L63.73 56.97 L63.72 58.03 L63.63 59.08 L63.48 60.12 L63.25 61.15 L62.95 62.16 L62.59 63.15 L62.16 64.12 L61.67 65.05 L61.11 65.95 L60.5 66.8 L59.83 67.62 L59.11 68.39 L58.33 69.1 L57.51 69.76 L56.65 70.37 L55.75 70.92 L54.81 71.4 L53.84 71.82 L52.85 72.17 L51.83 72.46 L50.8 72.67 L24.67 72.67 L23.39 72.39 L22.14 72.05 L20.9 71.64 L19.68 71.18 L18.48 70.67 L17.31 70.09 L16.17 69.46 L15.06 68.78 L13.98 68.04 L12.94 67.26 L11.94 66.43 L10.98 65.54 L10.06 64.62 L9.192 63.65 L8.367 62.64 L7.59 61.6 L6.863 60.51 L6.189 59.4 L5.569 58.25 L5.004 57.08 L4.496 55.88 L4.045 54.66 L3.653 53.41 L3.32 52.15 L3.048 50.88 L2.836 49.59 L2.686 48.3 L2.598 47 L2.571 45.7 L2.607 44.39 L2.704 43.09 L2.863 41.8 M15 46.91 L39.47 46.91 L40.33 47.07 L41.18 47.29 L42.01 47.59 L42.8 47.96 L43.56 48.41 L44.28 48.91 L44.95 49.48 L45.56 50.1 L46.13 50.78 L46.62 51.5 L47.06 52.26 L47.42 53.06 L47.72 53.89 L47.94 54.74 L48.08 55.6 L48.15 56.48 L48.14 57.36 L48.05 58.23 L47.88 59.09 L47.65 59.94 L47.33 60.76 L46.95 61.55 L46.5 62.3 L45.98 63.01 L45.41 63.67 L44.77 64.28 L44.09 64.83 L43.36 65.32 L42.59 65.74 L41.79 66.1 L40.96 66.38 L40.1 66.58 L38.98 66.85 L37.84 67.05 L36.7 67.19 L35.54 67.26 L34.39 67.26 L33.24 67.19 L32.09 67.06 L30.95 66.87 L29.83 66.6 L28.72 66.27 L27.63 65.88 L26.57 65.43 L25.54 64.91 L24.53 64.34 L23.57 63.71 L22.64 63.02 L21.75 62.29 L20.9 61.5 L20.11 60.66 L19.36 59.78 L18.67 58.86 L18.03 57.9 L17.44 56.9 L16.92 55.87 L16.45 54.81 L16.05 53.73 L15.71 52.63 L15.44 51.5 L15.23 50.37 L15.09 49.22 L15.01 48.07 L15 46.91 M38.44 17.03 L38.44 17.13 L38.45 17.23 L38.47 17.33 L38.51 17.43 L38.55 17.52 L38.6 17.61 L38.66 17.69 L38.73 17.77 L38.8 17.84 L38.88 17.9 L38.97 17.96 L39.06 18 L39.16 18.04 L39.25 18.07 L39.35 18.08 L39.46 18.09 L39.56 18.09 L39.66 18.08 L39.76 18.05 L39.86 18.02 L39.95 17.98 L40.04 17.93 L40.12 17.87 L40.2 17.8 L40.27 17.73 L40.33 17.65 L40.39 17.56 L40.44 17.47 L40.47 17.37 L40.5 17.27 L40.52 17.17 L40.52 17.07 L40.52 16.97 L40.51 16.87 L40.48 16.77 L40.45 16.67 L40.41 16.58 L40.36 16.49 L40.3 16.4 L40.23 16.33 L40.16 16.26 L40.08 16.19 L39.99 16.14 L39.9 16.09 L39.8 16.06 L39.7 16.03 L39.6 16.01 L39.5 16 L39.4 16.01 L39.3 16.02 L39.2 16.04 L39.1 16.08 L39.01 16.12 L38.92 16.17 L38.83 16.23 L38.76 16.29 L38.69 16.37 L38.62 16.45 L38.57 16.54 L38.52 16.63 L38.49 16.72 L38.46 16.82 L38.44 16.92 L38.44 17.03 \"/>"
"<path fill=\"rgb(255, 128, 0)\" stroke=\"none\" d=\"M34.85 25.77 L34.8 25.52 L34.77 25.27 L34.75 25.02 L34.75 24.78 L34.77 24.53 L34.79 24.28 L34.83 24.03 L34.89 23.79 L34.96 23.55 L35.04 23.32 L35.14 23.09 L35.25 22.86 L35.37 22.65 L35.51 22.44 L35.66 22.24 L35.81 22.04 L35.98 21.86 L36.16 21.69 L36.35 21.53 L36.55 21.38 L36.76 21.24 L36.97 21.11 L37.19 21 L37.42 20.89 L37.65 20.81 L37.89 20.73 L38.13 20.67 L38.38 20.63 L38.63 20.59 L38.87 20.58 L39.12 20.57 L39.37 20.59 L72.91 20.59 L73.15 20.61 L73.38 20.65 L73.61 20.7 L73.83 20.75 L74.06 20.82 L74.28 20.9 L74.5 20.99 L74.71 21.09 L74.92 21.2 L75.12 21.33 L75.31 21.45 L75.5 21.59 L75.69 21.74 L75.86 21.9 L76.03 22.06 L76.19 22.23 L76.34 22.41 L76.48 22.6 L76.62 22.79 L76.74 22.99 L76.86 23.2 L76.96 23.41 L77.06 23.62 L77.14 23.84 L77.21 24.07 L77.28 24.29 L77.33 24.52 L77.37 24.75 L77.4 24.98 L77.42 25.22 L77.43 25.45 L77.43 25.69 L77.43 31.77 L77.35 32.87 L77.2 33.96 L76.97 35.03 L76.66 36.09 L76.28 37.12 L75.83 38.12 L75.3 39.09 L74.71 40.02 L74.06 40.9 L73.34 41.73 L72.57 42.52 L71.74 43.24 L70.86 43.9 L69.94 44.5 L68.98 45.04 L67.98 45.5 L66.95 45.89 L65.9 46.2 L64.83 46.45 L63.74 46.61 L62.64 46.7 L61.54 46.7 L60.45 46.63 L59.36 46.49 L58.28 46.26 L57.22 45.96 L56.19 45.58 L55.19 45.14 L54.22 44.62 L53.29 44.03 L52.4 43.38 L51.56 42.67 L34.85 25.77 \"/>"
"<path fill=\"rgb(192, 192, 192)\" stroke=\"none\" d=\"M15 46.91 L39.47 46.91 L40.33 47.07 L41.18 47.29 L42.01 47.59 L42.8 47.96 L43.56 48.41 L44.28 48.91 L44.95 49.48 L45.56 50.1 L46.13 50.78 L46.62 51.5 L47.06 52.26 L47.42 53.06 L47.72 53.89 L47.94 54.74 L48.08 55.6 L48.15 56.48 L48.14 57.36 L48.05 58.23 L47.88 59.09 L47.65 59.94 L47.33 60.76 L46.95 61.55 L46.5 62.3 L45.98 63.01 L45.41 63.67 L44.77 64.28 L44.09 64.83 L43.36 65.32 L42.59 65.74 L41.79 66.1 L40.96 66.38 L40.1 66.58 L38.98 66.85 L37.84 67.05 L36.7 67.19 L35.54 67.26 L34.39 67.26 L33.24 67.19 L32.09 67.06 L30.95 66.87 L29.83 66.6 L28.72 66.27 L27.63 65.88 L26.57 65.43 L25.54 64.91 L24.53 64.34 L23.57 63.71 L22.64 63.02 L21.75 62.29 L20.9 61.5 L20.11 60.66 L19.36 59.78 L18.67 58.86 L18.03 57.9 L17.44 56.9 L16.92 55.87 L16.45 54.81 L16.05 53.73 L15.71 52.63 L15.44 51.5 L15.23 50.37 L15.09 49.22 L15.01 48.07 L15 46.91 \"/>"
"<path fill=\"rgb(128, 128, 128)\" stroke=\"none\" d=\"M38.44 17.03 L38.44 17.13 L38.45 17.23 L38.47 17.33 L38.51 17.43 L38.55 17.52 L38.6 17.61 L38.66 17.69 L38.73 17.77 L38.8 17.84 L38.88 17.9 L38.97 17.96 L39.06 18 L39.16 18.04 L39.25 18.07 L39.35 18.08 L39.46 18.09 L39.56 18.09 L39.66 18.08 L39.76 18.05 L39.86 18.02 L39.95 17.98 L40.04 17.93 L40.12 17.87 L40.2 17.8 L40.27 17.73 L40.33 17.65 L40.39 17.56 L40.44 17.47 L40.47 17.37 L40.5 17.27 L40.52 17.17 L40.52 17.07 L40.52 16.97 L40.51 16.87 L40.48 16.77 L40.45 16.67 L40.41 16.58 L40.36 16.49 L40.3 16.4 L40.23 16.33 L40.16 16.26 L40.08 16.19 L39.99 16.14 L39.9 16.09 L39.8 16.06 L39.7 16.03 L39.6 16.01 L39.5 16 L39.4 16.01 L39.3 16.02 L39.2 16.04 L39.1 16.08 L39.01 16.12 L38.92 16.17 L38.83 16.23 L38.76 16.29 L38.69 16.37 L38.62 16.45 L38.57 16.54 L38.52 16.63 L38.49 16.72 L38.46 16.82 L38.44 16.92 L38.44 17.03 \"/>"
"<path stroke=\"rgb(0, 0, 0)\" fill=\"none\" d=\"M34.85 25.77 L77.43 25.69 \"/>"
"</svg>";
const char* icontype = "image/svg+xml";


void sqlite_get_changes(webui_event_t* e){
  changes = sqlite3_total_changes(db);
  webui_return_int(e, changes);
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
        
        if (i == 0) pos +=snprintf(buffer + pos, BUF_SIZE - pos, "\"%s\":\"", key);
        else pos +=snprintf(buffer + pos, BUF_SIZE - pos, ",\"%s\":\"", key);
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
            break;
          }
        }
        pos +=snprintf(buffer + pos, BUF_SIZE - pos, "\"");
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

int main(int argc, char** argv){
  
  char* path = NULL;
  int length, dirname_length;

  length = wai_getExecutablePath(NULL, 0, &dirname_length);
  if (length > 0)
  {
    path = (char*)malloc(length + 1);
    if (!path) return 1; /* fail */
    wai_getExecutablePath(path, length, &dirname_length);
    path[dirname_length] = '\0';
    chdir(path);
    free(path);
  }
  
  static char full_path[PATH_MAX_CHARS+1];
	full_path[0] = 0; /*init */
  
  const char *proj_path = NULL;  // guaranteed to be assigned only if kgflags_parse succeeds
	kgflags_string("proj", NULL, "Pasta do projeto.", true, &proj_path);
	if (!kgflags_parse(argc, argv)) {
    kgflags_print_errors();
    kgflags_print_usage();
    return 1;
  }
	//int kgflags_get_non_flag_args_count(void);
	const char * arg_file = kgflags_get_non_flag_arg(0);
  
  /* verifica se o diretorio eh valido */
  DIR* dir = opendir(proj_path);
	if (!dir) { 
		
		return 1; /* fail */
	}
	closedir(dir);
  //chdir(proj_path);
  strncpy(full_path, proj_path, PATH_MAX_CHARS);
  /* put dir separator at end of returnned string */
	int len = strlen (full_path);
	if (len < PATH_MAX_CHARS - 1){
		if (full_path[len - 1] != DIR_SEPARATOR){
			full_path[len] = DIR_SEPARATOR;
			full_path[len + 1]  = 0;
		}
	}else {
    return 1; /* fail */
  }
  len = strlen (full_path);
  strncat(full_path, "projeto.db", PATH_MAX_CHARS - len);
  
  sqlite3_open(full_path, &db);
  changes = 0;
  pos = 0;
  buffer = malloc(BUF_SIZE+1);

    
    // Create a window
    size_t my_window = webui_new_window();
    webui_set_icon(my_window, icon, icontype);

    // Bind HTML elements with C functions
    webui_bind(my_window, "Sqlite_get_changes", sqlite_get_changes);
    webui_bind(my_window, "Sqlite_exec", sqlite_exec);

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
