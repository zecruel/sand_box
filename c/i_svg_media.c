#include "i_svg_media.h"

//char ** svg_media(void){
	static char * svg_data[] = {
		[SVG_ARC] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <rect fill=\"none\" stroke-dasharray=\"null\" stroke-linejoin=\"null\" stroke-linecap=\"null\" x=\"18.46743\" y=\"13.75821\" width=\"2.32293\" height=\"2.2807\" id=\"svg_6\" stroke=\"#F9F9F9\"/>"
		"  <rect id=\"svg_9\" fill=\"none\" stroke-dasharray=\"null\" stroke-linejoin=\"null\" stroke-linecap=\"null\" x=\"3.30502\" y=\"13.39921\" width=\"2.32293\" height=\"2.2807\" stroke=\"#F9F9F9\"/>"
		"  <path id=\"svg_10\" d=\"m4.29754,13.25139c0.22525,-4.1672 4.16719,-6.89841 8.10914,-6.75762c3.94195,0.14079 7.26445,3.18171 7.05327,7.0955\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" stroke=\"#F9F9F9\" fill=\"none\"/>"
		"</svg>"},

		[SVG_BRICK] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path stroke-width=\"0\" stroke=\"#F9F9F9\" fill=\"#F9F9F9\" id=\"svg_1\" d=\"m9.916,8.195l-0.013,0l0,0.961c-0.034,1.598 4.213,1.601 4.161,0l0,-0.96c-0.123,-1.511 -4.042,-1.52 -4.148,-0.001zm2.08,0.71c-0.723,0 -1.311,-0.253 -1.311,-0.564c0,-0.312 0.588,-0.564 1.311,-0.564c0.724,0 1.311,0.253 1.311,0.564c0,0.311 -0.587,0.564 -1.311,0.564zm6.421,-2.155l0,-0.96c-0.124,-1.511 -4.042,-1.52 -4.148,-0.001l-0.013,0l0,0.961c-0.034,1.599 4.214,1.602 4.161,0zm-2.067,-1.379c0.723,0 1.311,0.253 1.311,0.564s-0.589,0.565 -1.311,0.565c-0.724,0 -1.311,-0.253 -1.311,-0.564s0.587,-0.565 1.311,-0.565zm-10.797,0.418l-0.013,0l0,0.961c-0.034,1.598 4.213,1.601 4.161,0l0,-0.96c-0.123,-1.511 -4.042,-1.519 -4.148,-0.001zm2.08,0.711c-0.723,0 -1.311,-0.253 -1.311,-0.564s0.588,-0.565 1.311,-0.565c0.724,0 1.311,0.253 1.311,0.564s-0.588,0.565 -1.311,0.565zm2.283,-2.988l-0.013,0.201l0,0.759c-0.034,1.598 4.214,1.602 4.161,0l0,-0.959c-0.124,-1.512 -4.042,-1.52 -4.148,-0.001zm3.392,0.145c0,0.311 -0.588,0.564 -1.311,0.564c-0.724,0 -1.311,-0.253 -1.311,-0.564s0.587,-0.564 1.311,-0.564c0.723,0 1.311,0.253 1.311,0.564zm-1.308,-3.657l-11,6l0.009,0.019l-0.009,-0.005l0,12.118l11,5.868l11,-5.869l0,-12.055l-11,-6.076zm-1,21l-8,-4.268l0,-7.133l8,4.401l0,7zm-8.885,-14.464l9.882,-5.396l9.917,5.458l-9.896,5.385l-9.903,-5.447zm10.885,7.464l8,-4.353l0,7.085l-8,4.268l0,-7z\"/>"
		"</svg>"},

		[SVG_CIRCLE] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <circle stroke=\"#F9F9F9\" stroke-width=\"2\" fill=\"#F9F9F9\" stroke-dasharray=\"null\" stroke-linejoin=\"null\" stroke-linecap=\"null\" fill-opacity=\"0\" cx=\"11.96246\" cy=\"11.92803\" r=\"10.87535\" id=\"svg_1\"/>"
		"</svg>"},

		[SVG_COPY] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path stroke-width=\"0\" stroke=\"#F9F9F9\" fill=\"#F9F9F9\" id=\"svg_1\" d=\"m15.143,13.244l0.837,-2.244l2.698,5.641l-5.678,2.502l0.805,-2.23s-8.055,-3.538 -7.708,-10.913c2.715,5.938 9.046,7.244 9.046,7.244zm8.857,-7.244l0,18l-18,0l0,-6l-6,0l0,-18l18,0l0,6l6,0zm-2,2l-12.112,0c-0.562,-0.578 -1.08,-1.243 -1.521,-2l7.633,0l0,-4l-14,0l0,14l4,0l0,-3.124c0.6,0.961 1.287,1.823 2,2.576l0,6.548l14,0l0,-14z\"/>"
		"</svg>"},

		[SVG_CURSOR] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path fill=\"#F9F9F9\" d=\"m4.83033,-0.0246l16,12.279l-6.951,1.17l4.325,8.817l-3.596,1.734l-4.35,-8.879l-5.428,4.702l0,-19.823z\" id=\"path4529\"/>"
		"</svg>"},

		[SVG_CUT] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path stroke-width=\"0\" stroke=\"#F9F9F9\" fill=\"#F9F9F9\" id=\"svg_1\" d=\"m12.026,14.116c-3.475,1.673 -7.504,3.619 -8.484,4.09c-1.848,0.889 -3.542,-1.445 -3.542,-1.445l8.761,-4.226l3.265,1.581zm7.93,6.884c-0.686,0 -1.393,-0.154 -2.064,-0.479c-1.943,-0.941 -2.953,-3.001 -2.498,-4.854c0.26,-1.057 -0.296,-1.201 -1.145,-1.612l-14.189,-6.866s1.7,-2.329 3.546,-1.436c1.134,0.549 5.689,2.747 9.614,4.651l0.985,-0.474c0.85,-0.409 1.406,-0.552 1.149,-1.609c-0.451,-1.855 0.564,-3.913 2.51,-4.848c0.669,-0.321 1.373,-0.473 2.054,-0.473c2.311,0 4.045,1.696 4.045,3.801c0,1.582 -0.986,3.156 -2.613,3.973c-1.625,0.816 -2.765,0.18 -4.38,0.965l-0.504,0.245l0.552,0.27c1.613,0.789 2.754,0.156 4.377,0.976c1.624,0.819 2.605,2.392 2.605,3.97c0,2.108 -1.739,3.8 -4.044,3.8zm-2.555,-12.815c0.489,1.022 1.876,1.378 3.092,0.793c1.217,-0.584 1.809,-1.893 1.321,-2.916c-0.489,-1.022 -1.876,-1.379 -3.093,-0.794s-1.808,1.894 -1.32,2.917zm-3.643,3.625c0,-0.414 -0.335,-0.75 -0.75,-0.75c-0.414,0 -0.75,0.336 -0.75,0.75s0.336,0.75 0.75,0.75s0.75,-0.336 0.75,-0.75zm6.777,3.213c-1.215,-0.588 -2.604,-0.236 -3.095,0.786c-0.491,1.022 0.098,2.332 1.313,2.919c1.215,0.588 2.603,0.235 3.094,-0.787c0.492,-1.021 -0.097,-2.33 -1.312,-2.918z\"/>"
		"</svg>"},

		[SVG_CZ] = (char[]){"<svg width=\"159.375\" height=\"159.375\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path id=\"svg_2\" stroke-width=\"2.4365\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#008000\" fill=\"#008000\" d=\"m19.717,1.22001l119.94,0c10.248,0 18.499,8.2505 18.499,18.499l0,119.94c0,10.248 -8.2505,18.499 -18.499,18.499l-119.94,0c-10.248,0 -18.499,-8.2505 -18.499,-18.499l0,-119.94c0,-10.248 8.2505,-18.499 18.499,-18.499z\"/>"
		"  <path fill=\"#ffffff\" id=\"svg_4\" d=\"m53.18069,111.00471q-9.6438,0 -17.83534,-3.24783q-8.13347,-3.24783 -14.00109,-9.67726q-5.86762,-6.42943 -9.12101,-16.04017q-3.19533,-9.61075 -3.19533,-22.20435q0,-11.7319 3.07896,-21.2764q3.07896,-9.5445 8.94682,-16.37166q5.63537,-6.56191 13.94303,-10.14124q8.36572,-3.57932 18.24201,-3.57932q5.46094,0 9.81823,0.72911q4.41535,0.66282 8.13347,1.78961q3.89231,1.25936 7.02958,2.8501q3.19533,1.52447 5.57731,2.8501l0,23.92777l-2.55617,0q-1.62667,-1.59077 -4.1248,-3.77805q-2.44004,-2.18729 -5.57731,-4.30827q-3.19533,-2.12102 -6.91345,-3.57932q-3.71812,-1.4582 -7.95905,-1.4582q-4.70566,0 -8.94682,1.72331q-4.24092,1.65704 -7.84292,5.56772q-3.42758,3.77805 -5.57731,10.00848q-2.09145,6.23042 -2.09145,15.11222q0,9.27953 2.26574,15.50995q2.32382,6.23042 5.80956,9.80975q3.54394,3.64557 7.90099,5.23623q4.35729,1.52447 8.59821,1.52447q4.06674,0 8.01711,-1.39193q4.00867,-1.39193 7.37819,-3.77805q2.84672,-1.92218 5.28676,-4.10954t4.00867,-3.77805l2.32382,0l0,23.59628q-3.25339,1.65704 -6.21623,3.11535q-2.96284,1.4582 -6.21623,2.51872q-4.24092,1.39193 -7.95905,2.12102t-10.2249,0.72911l0,0.00038z\"/>"
		"  <path fill=\"#ffffff\" id=\"svg_6\" stroke-width=\"0.99609px\" d=\"m150.68777,147.69152l-60.44588,0l0,-16.68789l35.85193,-55.14824l-34.11192,0l0,-17.22639l56.7735,0l0,16.98718l-35.8034,54.84895l37.73576,0l0,17.22639z\"/>"
		"</svg>"},

		[SVG_DOWN] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path stroke-width=\"0\" stroke=\"#F9F9F9\" fill=\"#F9F9F9\" id=\"svg_1\" d=\"m24,12c0,-6.627 -5.373,-12 -12,-12s-12,5.373 -12,12s5.373,12 12,12s12,-5.373 12,-12zm-17,1l4,0l0,-8l2,0l0,8l4,0l-5,6l-5,-6z\"/>"
		"</svg>"},

		[SVG_DUPLI] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <polygon stroke=\"#F9F9F9\" id=\"svg_27\" fill=\"none\" points=\"11.22015559273109,6.564429988540724 7.3665972478891035,11.868398356550983 1.13140869140625,9.842461853727968 1.13140869140625,3.2863981233534787 7.3665972478891035,1.260462760925293 11.22015559273109,6.564429988540724 \" stroke-width=\"2\"/>"
		"  <path stroke=\"#F9F9F9\" fill=\"none\" stroke-width=\"2\" stroke-dasharray=\"null\" stroke-linejoin=\"null\" stroke-linecap=\"null\" d=\"m1.12991,12.74926c1.00308,4.01937 3.87068,6.12674 7.83461,5.59615l-2.03119,-2.47885\" id=\"svg_34\"/>"
		"  <polygon id=\"svg_2\" stroke=\"#F9F9F9\" fill=\"none\" points=\"22.71637535095215,17.33411979675293 18.862817764282227,22.63808822631836 12.627628326416016,20.612152099609375 12.627628326416016,14.056087493896484 18.862817764282227,12.030152320861816 22.71637535095215,17.33411979675293 \" stroke-width=\"2\"/>"
		"</svg>"},

		[SVG_ELIPSE] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <ellipse stroke=\"#F9F9F9\" ry=\"6.40723\" rx=\"10.83722\" id=\"svg_1\" cy=\"12.08009\" cx=\"12.00834\" fill-opacity=\"0\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" fill=\"#F9F9F9\"/>"
		"</svg>"},

		[SVG_INFO] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path stroke-width=\"0\" stroke=\"#F9F9F9\" fill=\"#F9F9F9\" id=\"svg_1\" d=\"m12,0c-6.627,0 -12,5.373 -12,12s5.373,12 12,12s12,-5.373 12,-12s-5.373,-12 -12,-12zm-2.033,16.01c0.564,-1.789 1.632,-3.932 1.821,-4.474c0.273,-0.787 -0.211,-1.136 -1.74,0.209l-0.34,-0.64c1.744,-1.897 5.335,-2.326 4.113,0.613c-0.763,1.835 -1.309,3.074 -1.621,4.03c-0.455,1.393 0.694,0.828 1.819,-0.211c0.153,0.25 0.203,0.331 0.356,0.619c-2.498,2.378 -5.271,2.588 -4.408,-0.146zm4.742,-8.169c-0.532,0.453 -1.32,0.443 -1.761,-0.022c-0.441,-0.465 -0.367,-1.208 0.164,-1.661c0.532,-0.453 1.32,-0.442 1.761,0.022c0.439,0.466 0.367,1.209 -0.164,1.661z\"/>"
		"</svg>"},

		[SVG_LAYERS] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path stroke-width=\"0\" stroke=\"#F9F9F9\" fill=\"#F9F9F9\" id=\"svg_1\" d=\"m4,17.162l-2,0.838l0,-12.972l12,-5.028l0,2.507l-10,4.19l0,10.465zm18,-11.162l-12,5.028l0,12.972l12,-5.028l0,-12.972zm-14,3.697l10,-4.19l0,-2.507l-12,5.028l0,12.972l2,-0.838l0,-10.465z\"/>"
		"</svg>"},

		[SVG_LEFT] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path stroke-width=\"0\" stroke=\"#F9F9F9\" fill=\"#F9F9F9\" id=\"svg_1\" d=\"m12,24c6.627,0 12,-5.373 12,-12s-5.373,-12 -12,-12s-12,5.373 -12,12s5.373,12 12,12zm-1,-17l0,4l8,0l0,2l-8,0l0,4l-6,-5l6,-5z\"/>"
		"</svg>"},

		[SVG_LINE] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <line stroke=\"#F9F9F9\" stroke-width=\"2\" id=\"svg_2\" y2=\"3.95966\" x2=\"19.96678\" y1=\"19.50219\" x1=\"4.46648\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" fill=\"none\"/>"
		"  <rect stroke=\"#F9F9F9\" id=\"svg_6\" height=\"2.2807\" width=\"2.32293\" y=\"2.05908\" x=\"19.65001\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" fill=\"none\"/>"
		"  <rect id=\"svg_7\" stroke=\"#F9F9F9\" height=\"2.2807\" width=\"2.32293\" y=\"19.18543\" x=\"2.41808\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" fill=\"none\"/>"
		"</svg>"},

		[SVG_MAGNET] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path stroke-width=\"0\" stroke=\"#F9F9F9\" d=\"m17.374,20.235c2.444,-2.981 6.626,-8.157 6.626,-8.157l-3.846,-3.092s-2.857,3.523 -6.571,8.097c-4.312,5.312 -11.881,-2.41 -6.671,-6.671c4.561,-3.729 8.097,-6.57 8.097,-6.57l-3.092,-3.842s-5.173,4.181 -8.157,6.621c-2.662,2.175 -3.76,4.749 -3.76,7.24c0,5.254 4.867,10.139 10.121,10.139c2.487,0 5.064,-1.095 7.253,-3.765zm4.724,-7.953l-1.699,2.111l-1.74,-1.397l1.701,-2.114l1.738,1.4zm-10.386,-10.385l1.4,1.738l-2.113,1.701l-1.397,-1.74l2.11,-1.699z\" id=\"svg_1\" fill=\"#F9F9F9\"/>"
		"</svg>"},

		[SVG_MIRROR] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <polygon stroke=\"#F9F9F9\" stroke-width=\"2\" points=\"11.220155715942383,7.165107727050781 7.3665971755981445,12.469076156616211 1.13140869140625,10.44313907623291 1.13140869140625,3.887075662612915 7.3665971755981445,1.861140251159668 11.220155715942383,7.165107727050781 \" fill=\"none\" id=\"svg_27\"/>"
		"  <polygon stroke=\"#F9F9F9\" stroke-width=\"2\" points=\"12.504858016967773,16.751941680908203 16.45220375061035,11.31888484954834 22.83914566040039,13.394128799438477 22.83914566040039,20.109752655029297 16.45220375061035,22.184999465942383 12.504858016967773,16.751941680908203 \" fill=\"none\" id=\"svg_2\"/>"
		"  <line stroke=\"#F9F9F9\" id=\"svg_1\" y2=\"23.16259\" x2=\"3.59051\" y1=\"20.26599\" x1=\"5.793\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" fill=\"none\"/>"
		"  <line id=\"svg_4\" stroke=\"#F9F9F9\" y2=\"18.24431\" x2=\"7.23523\" y1=\"15.34771\" x1=\"9.43771\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" fill=\"none\"/>"
		"  <line id=\"svg_5\" stroke=\"#F9F9F9\" y2=\"13.13855\" x2=\"11.03952\" y1=\"10.24195\" x1=\"13.242\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" fill=\"none\"/>"
		"  <line id=\"svg_6\" stroke=\"#F9F9F9\" y2=\"8.33313\" x2=\"14.64358\" y1=\"5.43653\" x1=\"16.84607\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" fill=\"none\"/>"
		"  <line id=\"svg_7\" stroke=\"#F9F9F9\" y2=\"3.62783\" x2=\"18.04742\" y1=\"0.73123\" x1=\"20.24991\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" fill=\"none\"/>"
		"</svg>"},

		[SVG_MOVE] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <polygon opacity=\"0.4\" stroke=\"#F9F9F9\" id=\"svg_27\" fill=\"none\" points=\"11.22015559273109,6.564429988540724 7.3665972478891035,11.868398356550983 1.13140869140625,9.842461853727968 1.13140869140625,3.2863981233534787 7.3665972478891035,1.260462760925293 11.22015559273109,6.564429988540724 \" stroke-width=\"2\"/>"
		"  <path stroke=\"#F9F9F9\" fill=\"none\" stroke-width=\"2\" stroke-dasharray=\"null\" stroke-linejoin=\"null\" stroke-linecap=\"null\" d=\"m1.12991,12.74926c1.00308,4.01937 3.87068,6.12674 7.83461,5.59615l-2.03119,-2.47885\" id=\"svg_34\"/>"
		"  <polygon id=\"svg_2\" stroke=\"#F9F9F9\" fill=\"none\" points=\"22.71637535095215,17.33411979675293 18.862817764282227,22.63808822631836 12.627628326416016,20.612152099609375 12.627628326416016,14.056087493896484 18.862817764282227,12.030152320861816 22.71637535095215,17.33411979675293 \" stroke-width=\"2\"/>"
		"</svg>"},

		[SVG_PASTE] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path stroke-width=\"0\" stroke=\"#F9F9F9\" fill=\"#F9F9F9\" id=\"svg_1\" d=\"m17.391,3l2.609,2.808l0,7.736c0,4.106 -6,2.456 -6,2.456s1.518,6 -2.638,6l-7.362,0l0,-16.192l2.666,-2.808l-4.666,0l0,21l20,0l0,-21l-4.609,0zm2.609,19l-3.824,0c1.377,-1.103 2.751,-2.51 3.824,-3.865l0,3.865zm-14.297,-15l12.651,0l-3.312,-3.569l0,-0.41c0.001,-1.668 -1.352,-3.021 -3.021,-3.021c-1.667,0 -3.021,1.332 -3.021,3l0.001,0.431l-3.298,3.569zm6.297,-5c0.553,0 1,0.448 1,1s-0.447,1 -1,1s-1,-0.448 -1,-1s0.447,-1 1,-1z\"/>"
		"</svg>"},

		[SVG_PIC] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path stroke-width=\"0\" stroke=\"#F9F9F9\" fill=\"#F9F9F9\" id=\"svg_1\" d=\"m5,8.5c0,-0.828 0.672,-1.5 1.5,-1.5s1.5,0.672 1.5,1.5c0,0.829 -0.672,1.5 -1.5,1.5s-1.5,-0.671 -1.5,-1.5zm9,0.5l-2.519,4l-2.481,-1.96l-4,5.96l14,0l-5,-8zm8,-4l0,14l-20,0l0,-14l20,0zm2,-2l-24,0l0,18l24,0l0,-18z\"/>"
		"</svg>"},

		[SVG_PLINE] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <line stroke=\"#F9F9F9\" fill=\"none\" stroke-dasharray=\"null\" stroke-linejoin=\"null\" stroke-linecap=\"null\" x1=\"4.88883\" y1=\"19.67113\" x2=\"15.86997\" y2=\"14.13833\" id=\"svg_2\" stroke-width=\"2\"/>"
		"  <rect fill=\"none\" stroke-dasharray=\"null\" stroke-linejoin=\"null\" stroke-linecap=\"null\" x=\"15.59544\" y=\"12.49116\" width=\"2.32293\" height=\"2.2807\" id=\"svg_6\" stroke=\"#F9F9F9\"/>"
		"  <rect fill=\"none\" stroke-dasharray=\"null\" stroke-linejoin=\"null\" stroke-linecap=\"null\" x=\"2.54479\" y=\"19.14319\" width=\"2.32293\" height=\"2.2807\" stroke=\"#F9F9F9\" id=\"svg_7\"/>"
		"  <path id=\"svg_8\" d=\"m17.89726,12.91351c4.18127,-2.0273 3.50551,-6.37752 2.66081,-7.85574c-0.8447,-1.47822 -3.88564,-3.84341 -8.36257,-1.14035\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" stroke=\"#F9F9F9\" fill=\"none\"/>"
		"  <rect id=\"svg_9\" fill=\"none\" stroke-dasharray=\"null\" stroke-linejoin=\"null\" stroke-linecap=\"null\" x=\"9.8937\" y=\"3.47395\" width=\"2.32293\" height=\"2.2807\" stroke=\"#F9F9F9\"/>"
		"</svg>"},

		[SVG_PRINT] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path stroke-width=\"0\" stroke=\"#F9F9F9\" fill=\"#F9F9F9\" id=\"svg_1\" d=\"m24,5l-4,0l0,-5l-16,0l0,5l-4,0l0,13l4,0l0,6l9.519,0c2.947,0 6.029,-3.577 6.434,-6l4.047,0l0,-13zm-18,-3l12,0l0,3l-12,0l0,-3zm8.691,16.648s1.469,3.352 -2,3.352l-6.691,0l0,-8l12,0l0,2.648c0,3.594 -3.309,2 -3.309,2zm6.809,-10.648c-0.276,0 -0.5,-0.224 -0.5,-0.5s0.224,-0.5 0.5,-0.5s0.5,0.224 0.5,0.5s-0.224,0.5 -0.5,0.5zm-5.5,9l-8,0l0,-1l8,0l0,1zm-3,1l-5,0l0,1l5,0l0,-1z\"/>"
		"</svg>"},

		[SVG_RECT] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <rect stroke=\"#F9F9F9\" id=\"svg_4\" height=\"21.42416\" width=\"21.62439\" y=\"1.32857\" x=\"1.18781\" fill-opacity=\"0\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" fill=\"#F9F9F9\"/>"
		"</svg>"},

		[SVG_RIGTH] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path stroke-width=\"0\" stroke=\"#F9F9F9\" fill=\"#F9F9F9\" id=\"svg_1\" d=\"m12,0c-6.627,0 -12,5.373 -12,12s5.373,12 12,12s12,-5.373 12,-12s-5.373,-12 -12,-12zm1,17l0,-4l-8,0l0,-2l8,0l0,-4l6,5l-6,5z\"/>"
		"</svg>"},

		[SVG_ROT] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path fill=\"none\" stroke-width=\"2\" stroke-dasharray=\"null\" stroke-linejoin=\"null\" stroke-linecap=\"null\" d=\"m2.08207,12.98311c-1.67772,3.78771 -0.70964,7.21218 2.74407,9.22866l-0.0799,-3.20376\" id=\"svg_34\" stroke=\"#F9F9F9\"/>"
		"  <polygon stroke=\"#F9F9F9\" fill=\"none\" points=\"18.550737380981445,12.387857437133789 14.295660972595215,18.24446678161621 7.410806655883789,16.007436752319336 7.410806655883789,8.76827621459961 14.295660972595215,6.531249046325684 18.550737380981445,12.387857437133789 \" stroke-width=\"2\" id=\"svg_35\"/>"
		"  <path fill=\"none\" stroke-width=\"2\" stroke-dasharray=\"null\" stroke-linejoin=\"null\" stroke-linecap=\"null\" d=\"m22.08846,11.0308c1.68761,-3.78332 0.72848,-7.2103 -2.71995,-9.23579l0.07153,3.20395\" stroke=\"#F9F9F9\" id=\"svg_36\"/>"
		"</svg>"},

		[SVG_SCALE] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <polygon stroke=\"#F9F9F9\" id=\"svg_27\" fill=\"none\" points=\"11.877136651162573,7.013555272593294 7.765459919976593,12.672793143192735 1.1126270294189453,10.51115570784225 1.1126270294189453,3.5159548373443386 7.765459919976593,1.354318618774414 11.877136651162573,7.013555272593294 \" stroke-width=\"2\" opacity=\"0.4\"/>"
		"  <polygon stroke=\"#F9F9F9\" fill=\"none\" points=\"22.77474594116211,13.618485450744629 16.132076263427734,22.761337280273438 5.384016036987305,19.269075393676758 5.384016036987305,7.967897415161133 16.132076263427734,4.475639820098877 22.77474594116211,13.618485450744629 \" stroke-width=\"2\" id=\"svg_35\"/>"
		"</svg>"},

		[SVG_TEXT] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path stroke-width=\"0\" stroke=\"#F9F9F9\" id=\"svg_4\" d=\"m0.49638,22.29413c0,0 6.65615,-20.50484 6.61312,-20.54891c-0.04303,-0.04407 3.84655,0.04407 3.84655,0.04407c0,0 6.60333,20.50484 6.60333,20.50484c0,0 -3.01112,0 -3.05415,-0.04407c-0.04303,-0.04407 -1.33046,-4.33823 -1.37349,-4.3823c-0.04303,-0.04407 -8.3564,0.04407 -8.3564,0.04407c0,0 -1.42631,4.49051 -1.42631,4.49051c0,0 -2.85264,-0.10821 -2.85264,-0.10821l-0.00001,0zm8.50169,-17.78115c0,0 -3.48655,10.9828 -3.48655,10.9828c0,0 6.86746,0 6.86746,0c0,0 -3.38091,-10.9828 -3.38091,-10.9828z\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" fill=\"#F9F9F9\"/>"
		"  <line id=\"svg_1\" y2=\"22.42414\" x2=\"20.82556\" y1=\"1.90422\" x1=\"20.82556\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke=\"#F9F9F9\" fill=\"none\"/>"
		"  <line id=\"svg_2\" y2=\"1.42907\" x2=\"23.67291\" y1=\"1.42907\" x1=\"21.30774\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke=\"#F9F9F9\" fill=\"none\"/>"
		"  <line id=\"svg_3\" y2=\"1.41147\" x2=\"20.35041\" y1=\"1.41147\" x1=\"17.98524\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke=\"#F9F9F9\" fill=\"none\"/>"
		"  <line id=\"svg_5\" y2=\"22.83349\" x2=\"23.66939\" y1=\"22.83349\" x1=\"21.30422\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke=\"#F9F9F9\" fill=\"none\"/>"
		"  <line id=\"svg_6\" y2=\"22.81589\" x2=\"20.34689\" y1=\"22.81589\" x1=\"17.98172\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke=\"#F9F9F9\" fill=\"none\"/>"
		"</svg>"},

		[SVG_TEXT_E] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path stroke=\"#F9F9F9\" fill=\"#F9F9F9\" stroke-dasharray=\"null\" stroke-linejoin=\"null\" stroke-linecap=\"null\" d=\"m0.05995,13.01825c0,0 4.16833,-12.84091 4.14139,-12.8685c-0.02695,-0.0276 2.40886,0.0276 2.40886,0.0276c0,0 4.13526,12.84091 4.13526,12.84091c0,0 -1.88568,0 -1.91263,-0.0276c-0.02695,-0.0276 -0.83318,-2.71677 -0.86014,-2.74436c-0.02695,-0.0276 -5.2331,0.0276 -5.2331,0.0276c0,0 -0.89321,2.81212 -0.89321,2.81212c0,0 -1.78642,-0.06777 -1.78642,-0.06777l0,0l-0.00001,0.00001zm5.32407,-11.13522c0,0 -2.1834,6.87784 -2.1834,6.87784c0,0 4.30066,0 4.30066,0c0,0 -2.11725,-6.87784 -2.11725,-6.87784l-0.00001,0z\" id=\"svg_4\" stroke-width=\"0\"/>"
		"  <path id=\"svg_34\" d=\"m3.86111,14.12894c1.00308,4.01937 3.87068,6.12674 7.83461,5.59615l-2.03119,-2.47885\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" fill=\"none\" stroke=\"#F9F9F9\"/>"
		"  <path stroke=\"#F9F9F9\" fill=\"#F9F9F9\" stroke-width=\"0\" stroke-dasharray=\"null\" stroke-linejoin=\"null\" stroke-linecap=\"null\" d=\"m15.11238,10.88974c0,0 0.03956,12.73585 0.03956,12.73585c0,0 3.50038,0.01978 3.50038,0.01978c0,0 4.54852,-0.13843 4.82539,-3.12464c0.27686,-2.9862 -1.65625,-3.60916 -2.76372,-3.62893c-1.10747,-0.01977 2.38796,0.38563 2.36819,-2.95654c-0.01977,-3.34217 -3.87612,-3.02575 -3.89161,-3.04123c-0.01547,-0.01547 -4.07819,-0.0043 -4.07819,-0.0043zm1.77986,1.48322c0,0 0.01978,3.99478 0.01978,3.99478c0,0 1.85896,0 1.85896,0c0,0 2.29403,0.03956 2.45224,-2.01717c0.15821,-2.05672 -2.43246,-1.99739 -2.43246,-1.99739c0,0 -1.89851,0.01978 -1.89851,0.01978zm-0.01978,5.41867c0,0 0.01978,4.39031 0.01978,4.39031c0,0 2.01727,0 2.00169,-0.01548c-0.01557,-0.01547 2.62593,0.15966 2.78414,-2.04124c0.15821,-2.2009 -2.65001,-2.33358 -2.65001,-2.33358c0,0 -2.1556,0 -2.1556,0z\" fill-rule=\"evenodd\" id=\"svg_6\"/>"
		"</svg>"},

		[SVG_TRASH] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" fill-rule=\"evenodd\" clip-rule=\"evenodd\"><path d=\"M19 24h-14c-1.104 0-2-.896-2-2v-16h18v16c0 1.104-.896 2-2 2m-9-14c0-.552-.448-1-1-1s-1 .448-1 1v9c0 .552.448 1 1 1s1-.448 1-1v-9zm6 0c0-.552-.448-1-1-1s-1 .448-1 1v9c0 .552.448 1 1 1s1-.448 1-1v-9zm6-5h-20v-2h6v-1.5c0-.827.673-1.5 1.5-1.5h5c.825 0 1.5.671 1.5 1.5v1.5h6v2zm-12-2h4v-1h-4v1z\" fill=\"#F9F9F9\"/></svg>"},

		[SVG_UP] = (char[]){"<svg width=\"24\" height=\"24\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">"
		"  <path stroke-width=\"0\" stroke=\"#F9F9F9\" fill=\"#F9F9F9\" id=\"svg_1\" d=\"m0,12c0,6.627 5.373,12 12,12s12,-5.373 12,-12s-5.373,-12 -12,-12s-12,5.373 -12,12zm17,-1l-4,0l0,8l-2,0l0,-8l-4,0l5,-6l5,6z\"/>"
		"</svg>"},

	};
	//return svg_data;
//}

NSVGimage ** i_svg_all_curves(void){
	static NSVGimage *curves[SVG_MEDIA_SIZE];
	int i = 0;
	//char ** media = svg_media();
	
	for (i = 0; i <  SVG_MEDIA_SIZE; i++){
		curves[i] = nsvgParse(svg_data[i], "px", 96.0f);
	}
	return curves;
}

void i_svg_free_curves(NSVGimage **curves){
	int i = 0;
	
	for (i = 0; i <  SVG_MEDIA_SIZE; i++){
		nsvgDelete(curves[i]);
	}
}

bmp_img ** i_svg_all_bmp(NSVGimage **curves, int w, int h){
	int i = 0;
	bmp_color transp = {.r = 255, .g = 255, .b = 255, .a = 0};
	bmp_color red = {.r = 255, .g = 0, .b =0, .a = 255};
	static bmp_img * img[SVG_MEDIA_SIZE];
	double ofs_x, ofs_y, zoom;
	
	NSVGrasterizer *rast = nsvgCreateRasterizer();
	if (rast == NULL) {
		return NULL;
	}
	
	for (i = 0; i <  SVG_MEDIA_SIZE; i++){
		img[i]  = bmp_new(w, h, transp, red);
		if (img[i] && curves[i]){
			bmp_fit(img[i], 0, 0, curves[i]->width, curves[i]->height, &zoom, &ofs_x, &ofs_y);
			nsvgRasterize(rast, curves[i], -ofs_x, -ofs_y, zoom, img[i]->buf, w, h, w*4);
		}
	}
	
	nsvgDeleteRasterizer(rast);
	return img;
}

void i_svg_free_bmp(bmp_img **img){
	int i = 0;
	
	for (i = 0; i <  SVG_MEDIA_SIZE; i++){
		bmp_free(img[i]);
	}
}