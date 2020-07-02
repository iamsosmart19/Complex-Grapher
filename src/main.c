#include "main.h"
/* #ifdef test */
/* 	setenv("CUDA_CACHE_DISABLE", "1", 1); */
/* #endif */

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  g_print( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

int main(int argc, char* argv[]) {
	/* setenv("CUDA_CACHE_DISABLE", "1", 1); */
	yydebug = !!getenv("YYDEBUG");

	GtkApplication *app;
	GlApplication glMainApp;
	int gtk_app_status;

	/* FILE* sample = fopen("input.txt", "r"); */
	/* /1* FILE* sample = fopen("input3.txt", "r"); *1/ */
	/* char function[1024]; */
	/* fgets(function, 1024, sample); */
	/* printf("|%s|\n", function); */
	/* fclose(sample); */

	char funcString[1024] = "z\n";

	// Possibly enable parser runtime debugging.
	queue out = queueInit();
	result res = parse_string(funcString, &out);
	if( !res.nerrs ) {
		glMainApp.operations = (cplx*)malloc(256*sizeof(cplx));

		glMainApp.opSize = 0;
		while(front(out) != -DBL_MAX-DBL_MAX*I) {
			glMainApp.operations[glMainApp.opSize++] = dequeue(&out);
		}
		/* glMainApp.operations = (cplx*)realloc(glMainApp.operations, glMainApp.opSize*sizeof(cplx)); */
		printf("count: %d\n", glMainApp.opSize);
	}

	glMainApp.funcString = funcToString(glMainApp.operations, glMainApp.opSize);

	glMainApp.clProg = create_cl_program(&glMainApp, 500);

	glMainApp.gridOn = 1;
	glMainApp.shadOn = 1;
	glMainApp.axesOn = 0;

	app = gtk_application_new("org.s1m7u.cplxgrapher", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), &glMainApp);
	gtk_app_status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return gtk_app_status;
}

static void activate (GtkApplication *app, GlApplication* glMainApp) {
	//Windows
	gtkWindow *window;
	gtkFixed* windowFixed = gtk_fixed_new();

	//Header
	GtkWidget* header = gtk_header_bar_new();

	//Screen Switcher
	gtkStack* gStack;
	/* gtkStackSwitcher* screenSwitch; */
	gtkStackSidebar* stackSidebar = gtk_stack_sidebar_new();
	GtkWidget* stackSeperator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
	gtkBox* stackBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtkLabel* menuLabel[4];

	//CSS style elements
	GtkStyleContext *context;
	GtkCssProvider *cssProvider = gtk_css_provider_new();

	//GLarea
	GtkWidget* GLdisplay_box;
	/* Glprogram shaderProgram; */

	//UI elements
	gtkFrame* funcFrame;
	gtkFixed* funcFixed = gtk_fixed_new();
	gtkLabel* funcLabel;
	gtkBox* funcLabelBox;
	gtkEntry* funcInput;
	GtkEntryBuffer* funcBuffer;
	gtkBox* funcInputBox;

	gtkButton* level_of_detail[4];

	//GTK_GL_AREA INITIALISATION
	glMainApp->area = gtk_gl_area_new();
	gtk_gl_area_set_required_version(GTK_GL_AREA(glMainApp->area), 3, 3);
	gtk_widget_set_name(glMainApp->area, "display");
	g_signal_connect(glMainApp->area, "realize", G_CALLBACK (on_gl_realise), glMainApp);
	g_signal_connect(glMainApp->area, "unrealize", G_CALLBACK (on_gl_unrealise), glMainApp);
	g_signal_connect(glMainApp->area, "render", G_CALLBACK (render), glMainApp);
	gtk_widget_set_size_request(glMainApp->area, 600, 600);

	//WINDOW INITILISATION
	//window
	window = gtk_application_window_new (app);
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	gtk_window_set_type_hint((GtkWindow*)window, GDK_WINDOW_TYPE_HINT_DIALOG);

	//display
	glMainApp->display = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(glMainApp->display), "display");
	gtk_window_set_default_size(GTK_WINDOW(glMainApp->display), 600, 600);
	gtk_window_set_type_hint((GtkWindow*)glMainApp->display, GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_keep_above(GTK_WINDOW(glMainApp->display), TRUE);

	//WINDOW SIGNALS
	gtk_widget_add_events(glMainApp->display, GDK_KEY_PRESS_MASK);
	gtk_widget_add_events(glMainApp->display, GDK_KEY_RELEASE_MASK);
	g_signal_connect(G_OBJECT(glMainApp->display), "key_press_event", G_CALLBACK(display_controls_press), glMainApp);
	g_signal_connect(G_OBJECT(glMainApp->display), "key_release_event", G_CALLBACK(display_controls_release), glMainApp);

	//Modify to bring window to focus
	g_signal_connect(glMainApp->display, "delete_event", G_CALLBACK(send_window_to_back), glMainApp);

	//Modify to closing the entire application
	g_signal_connect(window, "delete_event", G_CALLBACK(close_application), glMainApp->display);

	GLdisplay_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_widget_set_size_request(GLdisplay_box, 600, 600);
	gtk_container_add(GTK_CONTAINER(GLdisplay_box), glMainApp->area);
	gtk_container_add(GTK_CONTAINER(glMainApp->display), GLdisplay_box);

	//HEADER
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
	gtk_window_set_titlebar(GTK_WINDOW(window), header);
	gtk_window_set_title (GTK_WINDOW (window), "Complex Function Grapher");

	//FUNC INPUT
	funcLabel = gtk_label_new("Enter a function below (format described in guide page)");
	funcLabelBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_widget_set_size_request(funcLabel, 400, 34);
	gtk_container_add(GTK_CONTAINER(funcLabelBox), funcLabel);
	gtk_fixed_put(GTK_FIXED(funcFixed), funcLabelBox, 320 - 200, 120 - 34);

	funcBuffer = gtk_entry_buffer_new("", 0);

	funcInput = gtk_entry_new_with_buffer(funcBuffer);
	gtk_widget_set_name((funcInput), "funcIn");
	gtk_entry_set_max_width_chars(GTK_ENTRY(funcInput), 256);
	gtk_entry_set_max_length(GTK_ENTRY(funcInput), 256);
	gtk_entry_set_placeholder_text(GTK_ENTRY(funcInput), "Enter function here");
	g_signal_connect(funcInput, "activate", G_CALLBACK(on_entry_activate), glMainApp);

	context = gtk_widget_get_style_context(funcInput);

	funcInputBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_widget_set_size_request(funcInputBox, 400, 34);
	gtk_container_add(GTK_CONTAINER(funcInputBox), funcInput);
	gtk_fixed_put(GTK_FIXED(funcFixed), funcInputBox, 320 - 200, 120);

	char level_of_detail_text[64];
	sprintf(level_of_detail_text, "%d by %d", 500, 500);
	level_of_detail[0] = gtk_radio_button_new_with_label(NULL, level_of_detail_text);
	g_signal_connect(GTK_TOGGLE_BUTTON(level_of_detail[0]), "toggled", G_CALLBACK(radio_toggled), glMainApp);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(level_of_detail[0]), TRUE);
	gtk_fixed_put(GTK_FIXED(funcFixed), level_of_detail[0], 95, 165);
	for(int i = 1; i < 4; i++) {
		sprintf(level_of_detail_text, "%d by %d", 250*(i+2), 250*(i+2));
		level_of_detail[i] = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(level_of_detail[0]), level_of_detail_text);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(level_of_detail[i]), FALSE);
		g_signal_connect(GTK_TOGGLE_BUTTON(level_of_detail[i]), "toggled", G_CALLBACK(radio_toggled), glMainApp);
		gtk_fixed_put(GTK_FIXED(funcFixed), level_of_detail[i], 88 + 115*i, 165);
	}

	funcFrame = gtk_frame_new(NULL);
	gtk_container_add(GTK_CONTAINER(funcFrame), funcFixed);
	/* gtk_fixed_put(GTK_FIXED(windowFixed), funcFrame, 0, 0); */

	//Screen switchers
	gStack = gtk_stack_new();
	gtk_stack_set_transition_type(GTK_STACK(gStack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);

	/* gtk_orientable_set_orientation(GTK_ORIENTABLE(screenSwitch), GTK_ORIENTATION_VERTICAL); */
	/* gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(screenSwitch), GTK_STACK(gStack)); */
	gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(stackSidebar), GTK_STACK(gStack));
	gtk_box_pack_start(GTK_BOX(stackBox), stackSidebar, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(stackBox), stackSeperator, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(stackBox), gStack, TRUE, TRUE, 0);

	char* formatString;
	int fontsize;
	char* final_string;
	char menuLabelName[64];
	gtkFixed* menu_fixed = gtk_fixed_new();
	/* gtkButton* = */ 
	for(int i = 0; i < 4; i++) {
		switch(i) {
			case 0:
				sprintf(menuLabelName, "Title screen                          ");
				menu_fixed = gtk_fixed_new();

				formatString = "<span size=\"%d\">Complex Function Grapher</span>";
				fontsize = 38 * PANGO_SCALE;
				final_string = g_markup_printf_escaped(formatString, fontsize);

				menuLabel[i] = gtk_label_new("");
				gtk_label_set_markup(GTK_LABEL(menuLabel[i]), final_string);
				gtk_fixed_put(GTK_FIXED(menu_fixed), menuLabel[i], 7, 70);

				char* instructionString = "<span style=\"italic\">Press start to begin!</span>";
				gtkLabel* instructionLabel = gtk_label_new("");
				gtk_label_set_markup(GTK_LABEL(instructionLabel), instructionString);
				gtk_fixed_put(GTK_FIXED(menu_fixed), instructionLabel, 250, 160);

				gtk_stack_add_named(GTK_STACK(gStack), menu_fixed, menuLabelName);
				gtk_container_child_set(GTK_CONTAINER(gStack), menu_fixed, "title", menuLabelName, NULL);
				continue;
				break;

			case 1:
				sprintf(menuLabelName, "Start");
				menuLabel[i] = gtk_label_new(menuLabelName);
				/* gtk_stack_add_titled(GTK_STACK(gStack), funcLabel, menuLabelName, menuLabelName); */
				gtk_stack_add_named(GTK_STACK(gStack), funcFrame, menuLabelName);
				gtk_container_child_set(GTK_CONTAINER(gStack), funcFrame, "title", menuLabelName, NULL);
				/* gtk_container_child_set_property(GTK_CONTAINER(gStack), funcFrame, ); */
				continue;
				break;

			case 2:
				sprintf(menuLabelName, "Guide");
				gtkWindow* scrolled = gtk_scrolled_window_new(NULL, NULL);
				menu_fixed = gtk_fixed_new();

				formatString = "<span size=\"%d\">%s</span>";
				fontsize = 24 * PANGO_SCALE;
				final_string = g_markup_printf_escaped(formatString, fontsize, menuLabelName);

				menuLabel[i] = gtk_label_new("");
				gtk_label_set_markup(GTK_LABEL(menuLabel[i]), final_string);
				gtk_fixed_put(GTK_FIXED(menu_fixed), menuLabel[i], 35, 30);

				gtkLabel* instructions = gtk_label_new("This is a program for graphing complex functions.\n"
														"A function (allowed functions described below) is entered into the textbox\n"
														"on the Start screen. Once the return/enter key is pressed a window will be\n"
														"presented displaying the function. At this point the camera can be controlled\n"
														"with the keyboard (buttons specified below).\n"
														"Finally, when closing the window an option will be presented to save the \n"
														"current screen as an image. If cancelled it will return the user to the Start\n"
														"screen.\n"
														"\n"
														"More explicit details can be found in the user manual.");

				gtk_fixed_put(GTK_FIXED(menu_fixed), instructions, 35, 80);

				GtkTreeStore *funcStore = gtk_tree_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
				GtkTreeIter funcIter;
				GtkWidget *funcTree;
				GtkTreeViewColumn *column;
				GtkCellRenderer *renderer = gtk_cell_renderer_text_new();

				for(int i = 0; i < 30; i++) {
					gtk_tree_store_append(funcStore, &funcIter, NULL);
					gtk_tree_store_set(funcStore, &funcIter, 0, guide_function_table[i][0], 1, guide_function_table[i][1], 2, guide_function_table[i][2], -1);
				}

				funcTree = gtk_tree_view_new_with_model (GTK_TREE_MODEL (funcStore));
				g_object_unref(G_OBJECT(funcStore));

				column = gtk_tree_view_column_new_with_attributes("Syntax", renderer, "text", 0, NULL);
				gtk_tree_view_append_column (GTK_TREE_VIEW (funcTree), column);

				renderer = gtk_cell_renderer_text_new ();
				column = gtk_tree_view_column_new_with_attributes ("Sample usage       ", renderer, "text", 1, NULL);
				gtk_tree_view_append_column (GTK_TREE_VIEW (funcTree), column);

				renderer = gtk_cell_renderer_text_new ();
				column = gtk_tree_view_column_new_with_attributes ("Description                                ", renderer, "text", 2, NULL);
				gtk_tree_view_append_column (GTK_TREE_VIEW (funcTree), column);

				gtk_fixed_put(GTK_FIXED(menu_fixed), GTK_WIDGET(funcTree), 40, 80 + 200);

				GtkTreeStore *controlStore = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
				GtkTreeIter controlIter;
				GtkWidget *controlTree;

				for(int i = 0; i < 13; i++) {
					gtk_tree_store_append(controlStore, &controlIter, NULL);
					gtk_tree_store_set(controlStore, &controlIter, 0, guide_control_table[i][0], 1, guide_control_table[i][1], -1);
				}

				funcTree = gtk_tree_view_new_with_model (GTK_TREE_MODEL (funcStore));
				g_object_unref(G_OBJECT(funcStore));

				column = gtk_tree_view_column_new_with_attributes("Control", renderer, "text", 0, NULL);
				gtk_tree_view_append_column (GTK_TREE_VIEW (funcTree), column);

				renderer = gtk_cell_renderer_text_new ();
				column = gtk_tree_view_column_new_with_attributes ("Result", renderer, "text", 1, NULL);
				gtk_tree_view_append_column (GTK_TREE_VIEW (funcTree), column);

				gtk_fixed_put(GTK_FIXED(menu_fixed), GTK_WIDGET(funcTree), 40, 880 + 200);


				gtk_container_add(GTK_CONTAINER(scrolled), menu_fixed);

				gtk_stack_add_named(GTK_STACK(gStack), scrolled, menuLabelName);
				gtk_container_child_set(GTK_CONTAINER(gStack), scrolled, "title", menuLabelName, NULL);
				continue;
				break;

			case 3:
				sprintf(menuLabelName, "Settings");
				menu_fixed = gtk_fixed_new();

				formatString = "<span size=\"%d\">%s</span>";
				fontsize = 24 * PANGO_SCALE;
				final_string = g_markup_printf_escaped(formatString, fontsize, menuLabelName);

				menuLabel[i] = gtk_label_new("");
				gtk_label_set_markup(GTK_LABEL(menuLabel[i]), final_string);
				gtk_fixed_put(GTK_FIXED(menu_fixed), menuLabel[i], 35, 30);

				gtkButton* buttons;
				buttons = gtk_check_button_new_with_label("Grid on");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttons), TRUE);
				g_signal_connect(buttons, "toggled", G_CALLBACK(settings_toggled), glMainApp);
				gtk_fixed_put(GTK_FIXED(menu_fixed), buttons, 100, 100);

				buttons = gtk_check_button_new_with_label("Shading on");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttons), TRUE);
				g_signal_connect(buttons, "toggled", G_CALLBACK(settings_toggled), glMainApp);
				gtk_fixed_put(GTK_FIXED(menu_fixed), buttons, 100, 130);

				buttons = gtk_check_button_new_with_label("Axes off");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttons), FALSE);
				g_signal_connect(buttons, "toggled", G_CALLBACK(settings_toggled), glMainApp);
				gtk_fixed_put(GTK_FIXED(menu_fixed), buttons, 100, 160);

				gtk_stack_add_named(GTK_STACK(gStack), menu_fixed, menuLabelName);
				gtk_container_child_set(GTK_CONTAINER(gStack), menu_fixed, "title", menuLabelName, NULL);
				continue;
				break;
		}
	}

	/* gtk_fixed_put(GTK_FIXED(windowFixed), screenSwitch, 0, 0); */
	/* gtk_fixed_put(GTK_FIXED(windowFixed), screenSwitch, 400 - 50, 220); */
	/* gtk_widget_set_halign(screenSwitch, GTK_ALIGN_CENTER); */

	/* gtk_container_add(GTK_CONTAINER(window), windowFixed); */
	gtk_container_add(GTK_CONTAINER(window), stackBox);

	gtk_widget_show_all(glMainApp->display);
	gtk_widget_show_all(window);
	/* gtk_window_move(GTK_WINDOW(window), 120, 200); */
	/* gtk_window_move(GTK_WINDOW(glMainApp->display), 950, 200); */
}

static void on_entry_activate(GtkEntry* entry, GlApplication *app) {
	printf("on_entry_activate: start\n");
	ClProgram* clProg = &app->clProg;

	const char *funcString;
	funcString = gtk_entry_get_text(entry);

	queue out = queueInit();
	result res = parse_string(funcString, &out);
	if( !res.nerrs ) {
		app->operations = (cplx*)malloc(256*sizeof(cplx));

		app->opSize = 0;
		while(front(out) != -DBL_MAX-DBL_MAX*I) {
			app->operations[app->opSize++] = dequeue(&out);
		}
	}
	else {
		quick_message(GTK_WINDOW(gtk_widget_get_parent_window(GTK_WIDGET(entry))), res.err);
		free(res.err);
		/* warning popup: "with error message"*/
		return;
	}

	app->funcString = funcToString(app->operations, app->opSize);

	app->clProg = create_cl_program(app, app->width);
	g_signal_emit_by_name(app->area, "realize", app);

	write_to_clBuffer(app);

	gtk_window_deiconify(GTK_WINDOW(app->display));
	gtk_window_present_with_time(GTK_WINDOW(app->display), GDK_CURRENT_TIME);
	/* gtk_window_present_with_time((app->display), GDK_CURRENT_TIME); */

	printf("on_entry_activate: end\n");
}

static void on_gl_realise(GtkGLArea *area, GlApplication *app) {
	printf("on_gl_realise: start\n");
	guint* program = &(app->prog);

	gtk_gl_area_make_current(GTK_GL_AREA(area));
	if(gtk_gl_area_get_error(GTK_GL_AREA(area)) != NULL) {
		printf("Could not get area\n");
		return;
	}

	/* for(int i = 0; i < app->width * 4; i++) { */
	/* 	printf("(%d) %1.3f\n", i, app->posData[i]); */
	/* } */

	GError *internal_error = NULL;

	glViewport(0, 0, 800, 600);
	glEnable(GL_PROGRAM_POINT_SIZE);

	glEnable              ( GL_DEBUG_OUTPUT );
	glDebugMessageCallback( MessageCallback, 0 );

	//Points
	glPointSize(app->interval * 500);
	/* glPointSize(12); */

	init_shader(&program);

	glUseProgram(*program);

	glGenVertexArrays(1, &app->vao);
	glBindVertexArray(app->vao);

	glGenBuffers(1, &app->triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, app->triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * app->width * app->height + sizeof(GLfloat) * 3 * app->width * app->height, NULL, GL_DYNAMIC_DRAW);

	/* printf("GLfloat: %ld, float: %ld\n", sizeof(GLfloat), sizeof(float)); */

	//Position
	GLint posAttrb = glGetAttribLocation(*program, "position");
	glVertexAttribPointer(posAttrb, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrb);

	//Color
	printf("colors size: %ld\n", sizeof(GLfloat) * 3 * app->width * app->height);
	GLint colorAttrb = glGetAttribLocation(*program, "color");
	glVertexAttribPointer(colorAttrb, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * 2 * app->width * app->height));
	glEnableVertexAttribArray(colorAttrb);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 2 * app->width * app->height, app->posData);

	gtk_widget_queue_draw(app->area);

	printf("on_gl_realise: end\n");
	return;
}

static void on_gl_unrealise(GtkGLArea *area, GlApplication *app) {
	printf("on_gl_unrealise: start\n");
	/* gtkGLArea** area = &app->area; */
	guint* program = &(app->prog);

	gtk_gl_area_make_current(GTK_GL_AREA(area));

	if (gtk_gl_area_get_error(GTK_GL_AREA(area)) != NULL) {
		return;
	}

	if (app->vao != 0) {
		glDeleteVertexArrays(1, &app->vao);
	}
	if (program != 0) {
		glDeleteProgram(*program);
	}
	printf("on_gl_unrealise: end\n");
}

static gboolean render(GtkGLArea *area, GdkGLContext* context, GlApplication* app) {
	/* g_print("render: start\n"); */
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(app->prog);

	glBufferSubData(GL_ARRAY_BUFFER, app->posSize, app->colorSize, app->colors);
	glBindVertexArray(app->vao);
	glDrawArrays(GL_POINTS, 0, app->width * app->height);

	glFlush();

	return FALSE;
}

//GTK GL FUNCTIONS

static gboolean init_shader(guint** programExt) {
	printf("init_shader: start\n");
	GBytes* source;
	guint program = 0;
	guint vertex = 0, fragment = 0;
	GError* error;

	source = g_resources_lookup_data("/io/s1m7u/cplxgrapher/glfiles/vertShad.vert", 0, NULL);
	create_shader(GL_VERTEX_SHADER, g_bytes_get_data(source, NULL), &error, &vertex);
	g_bytes_unref(source);

	source = g_resources_lookup_data("/io/s1m7u/cplxgrapher/glfiles/fragShad.frag", 0, NULL);
	create_shader(GL_FRAGMENT_SHADER, g_bytes_get_data(source, NULL), &error, &fragment);
	g_bytes_unref(source);

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	int status = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		int log_len = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);

		char *buffer = g_malloc(log_len + 1);
		glGetProgramInfoLog(program, log_len, NULL, buffer);

		/* g_set_error (error, GLAREA_ERROR, GLAREA_ERROR_SHADER_LINK, */
		/* 		"Linking failure in program: %s", buffer); */
		g_print("Linking failure in progam %s", buffer);

		g_free(buffer);

		glDeleteProgram(program);
		program = 0;

		goto out;
	}
out:

	if (vertex != 0) {
		glDeleteShader (vertex);
	}
	if (fragment != 0) {
		glDeleteShader (fragment);
	}

	if (program) {
		*programExt = &program;
	}

	printf("init_shader: end\n");
	return program != 0;
}

static guint create_shader(int shader_type, const char *source, GError **error, guint *shader_out) {
	printf("create_shader: start\n");
	guint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		int log_len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

		char *buffer = g_malloc(log_len + 1);
		glGetShaderInfoLog(shader, log_len, NULL, buffer);

		/* g_set_error(error, GLAREA_ERROR, GLAREA_ERROR_SHADER_COMPILATION, */
		/* 		"Compilation failure in %s shader: %s", */
		/* 		shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment", */
		/* 		buffer); */

		g_free(buffer);

		glDeleteShader(shader);
		shader = 0;
	}

	if (shader_out != NULL) {
		*shader_out = shader;
	}

	printf("create_shader: end\n");
	return shader != 0;
}

//UNFINISHED

ClProgram create_cl_program(GlApplication* app, int width) {
	printf("create_cl_program: start\n");
	ClProgram clProg;

	/* app->width = 500; */
	/* app->height = 500; */
	/* app->interval = 0.004; */
	/* app->width = 1000; */
	/* app->height = 1000; */
	/* app->interval = 0.002; */
	/* app->width = 2000; */
	/* app->height = 2000; */
	/* app->interval = 0.001; */
	/* app->width = 4000; */
	/* app->height = 4000; */
	/* app->interval = 0.0005; */
	app->width = width;
	app->height = width;
	app->interval = (float)2/(float)width;
	app->n = app->width * app->height;

	gtk_gl_area_make_current(GTK_GL_AREA(app->area));

	glPointSize(app->interval * 500);

    app->colorSize = sizeof(GLfloat) * 3 * app->n;
    app->posSize = sizeof(float) * 2 * app->n;

	app->posData = malloc(app->posSize);
	app->colors = malloc(app->colorSize);

	for(int i = 0; i < app->width; i++) {
		for(int j = 0; j < app->height*2; j+=2) {
			app->posData[i*app->height*2+j] = -1.0 + i*app->interval;
			app->posData[i*app->height*2+j+1] = -1.0 + (j/2)*app->interval;
		}
	}

	app->posOffset[0] = 0;
	app->posOffset[1] = 0;
	app->zoom = 10;
	app->zoomc = 0.001;

    cl_int err;

    // Number of work items in each local work group
    app->localSize = 512;

    // Number of total work items - localSize must be divisor
    app->globalSize = ceil(app->n/(float)app->localSize)*app->localSize;

    // Bind to platform
    err = clGetPlatformIDs(1, &(clProg.cpPlatform), NULL);

	if(err != CL_SUCCESS) {
		printf("platform error: %d\n", err);
	}

    // Get ID for the device
    err = clGetDeviceIDs(clProg.cpPlatform, CL_DEVICE_TYPE_GPU, 1, &(clProg.device_id), NULL);
	if(err != CL_SUCCESS) {
		printf("device error: %d\n", err);
	}

    // Create a clProg.context  
    clProg.context = clCreateContext(0, 1, &(clProg.device_id), NULL, NULL, &err);
	if(err != CL_SUCCESS) {
		printf("context error: %d\n", err);
	}
 
    // Create a command clProg.queue 
    clProg.queue = clCreateCommandQueueWithProperties(clProg.context, clProg.device_id, 0, &err);

	GBytes* kernelSource;
	kernelSource = g_resources_lookup_data("/io/s1m7u/cplxgrapher/clfiles/graph.cl", 0, NULL);
	const char* initialkSrcPtr = g_bytes_get_data(kernelSource, NULL);
	char* kSrcPtr = malloc(strlen(initialkSrcPtr) + strlen(app->funcString) + 2*sizeof(char) + 1);
	strcpy(kSrcPtr, initialkSrcPtr);
	strcat(kSrcPtr, app->funcString);
	strcat(kSrcPtr, ";}");

    clProg.program = clCreateProgramWithSource(clProg.context, 1, (const char **)&kSrcPtr, NULL, &err);

	if(err != CL_SUCCESS) {
		printf("error: %d\n", err);
	}
 
    // Build the clProg.program executable 
    err = clBuildProgram(clProg.program, 0, NULL, "-cl-fast-relaxed-math", NULL, NULL);
	if(err != CL_SUCCESS) {
		printf("build error: %d\n", err);
		size_t errorSize;
		clGetProgramBuildInfo(clProg.program, clProg.device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &errorSize);
		
		char* log = malloc(errorSize);
		clGetProgramBuildInfo(clProg.program, clProg.device_id, CL_PROGRAM_BUILD_LOG, errorSize, log, NULL);

		printf("|%s|\n", log);
	}

    // Create the compute clProg.kernel in the clProg.program we wish to run
    clProg.kernel = clCreateKernel(clProg.program, "graph", &err);
	if(err != CL_SUCCESS) {
		printf("link error: %d\n", err);
	}

	/* printf("count*sizeof(cplx): %ld\n", count * sizeof(cplx)); */
 
    // Create the input and output arrays in device memory for our calculation
    clProg.posBuffer = clCreateBuffer(clProg.context, CL_MEM_READ_ONLY, app->posSize, NULL, NULL);
	clProg.opBuffer = clCreateBuffer(clProg.context, CL_MEM_READ_ONLY, 256*sizeof(cplx), NULL, NULL);

    clProg.colorBuffer = clCreateBuffer(clProg.context, CL_MEM_WRITE_ONLY, app->colorSize, NULL, NULL);
 
	g_print("create_cl_program: end\n");
	return clProg;
}

void radio_toggled(GtkWidget* button, GlApplication* app) {
	const char* size = gtk_button_get_label(GTK_BUTTON(button));
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button))) {
		g_print("%s was turned on\n", size);
		/* app->clProg = create_cl_program(app, atoi(size)); */
		app->width = atoi(size);
	}
}

void settings_toggled(GtkToggleButton* button, GlApplication* app) {
	const char* type = gtk_button_get_label(GTK_BUTTON(button));
	switch(type[0]) {
		case 'G':
			app->gridOn = 1 - app->gridOn;
			if(app->gridOn) {
				gtk_button_set_label(GTK_BUTTON(button), "Grid on");
			}
			else {
				gtk_button_set_label(GTK_BUTTON(button), "Grid off");
			}
			break;
		case 'S':
			app->shadOn = 1 - app->shadOn;
			if(app->shadOn) {
				gtk_button_set_label(GTK_BUTTON(button), "Shading on");
			}
			else {
				gtk_button_set_label(GTK_BUTTON(button), "Shading off");
			}
			break;
		case 'A':
			app->axesOn = 1 - app->axesOn;
			if(app->axesOn) {
				gtk_button_set_label(GTK_BUTTON(button), "Axes on");
			}
			else {
				gtk_button_set_label(GTK_BUTTON(button), "Axes off");
			}
			break;
	}
}

static gboolean display_controls_press(GtkWidget* widget, GdkEventKey* event, GlApplication* app) {
	ClProgram* clProg = &app->clProg;
	switch(event->keyval) {
		/* case GDK_KEY_: */
		/* 	break; */
		case GDK_KEY_Shift_L:
			app->shift_pressed = TRUE;
			return FALSE;

		case GDK_KEY_space:
			app->posOffset[0] = 0;
			app->posOffset[1] = 0;
			app->zoom = 10;
			write_to_clBuffer(app);
			/* g_signal_emit_by_name(GTK_GL_AREA(app->area), "render", gtk_gl_area_get_context(GTK_GL_AREA(app->area)), app); */
			gtk_gl_area_queue_render(GTK_GL_AREA(app->area));
			break;

		case GDK_KEY_comma:
			app->zoom += app->zoom/20;
			app->zoomc = app->zoom < 1.0 ? powl(0.001, 2.0-app->zoom) : 0.001;
			write_to_clBuffer(app);
			gtk_gl_area_queue_render(GTK_GL_AREA(app->area));
			break;

		case GDK_KEY_less:
			app->zoom += app->zoom/5;
			app->zoomc = app->zoom < 1.0 ? powl(0.001, 2.0-app->zoom) : 0.001;
			write_to_clBuffer(app);
			gtk_gl_area_queue_render(GTK_GL_AREA(app->area));
			break;

		case GDK_KEY_period:
			app->zoom -= app->zoom/20;
			app->zoomc = app->zoom < 1.0 ? powl(0.001, 2.0-app->zoom) : 0.001;
			write_to_clBuffer(app);
			gtk_gl_area_queue_render(GTK_GL_AREA(app->area));
			break;

		case GDK_KEY_greater:
			app->zoom -= app->zoom/5;
			app->zoomc = app->zoom < 1.0 ? powl(0.001, 2.0-app->zoom) : 0.001;
			write_to_clBuffer(app);
			gtk_gl_area_queue_render(GTK_GL_AREA(app->area));
			break;

		case GDK_KEY_uparrow:
		case GDK_KEY_Up:
			if(app->shift_pressed) {
				app->posOffset[1] += fabs(app->zoom) / 10;
			}
			else {
				app->posOffset[1] += fabs(app->zoom) / 100;
			}
			write_to_clBuffer(app);
			gtk_gl_area_queue_render(GTK_GL_AREA(app->area));
			break;

		case GDK_KEY_downarrow:
		case GDK_KEY_Down:
			if(app->shift_pressed) {
				app->posOffset[1] -= fabs(app->zoom) / 10;
			}
			else {
				app->posOffset[1] -= fabs(app->zoom) / 100;
			}
			write_to_clBuffer(app);
			gtk_gl_area_queue_render(GTK_GL_AREA(app->area));
			break;

		case GDK_KEY_rightarrow:
		case GDK_KEY_Right:
			if(app->shift_pressed) {
				app->posOffset[0] += fabs(app->zoom) / 10;
			}
			else {
				app->posOffset[0] += fabs(app->zoom) / 100;
			}
			write_to_clBuffer(app);
			gtk_gl_area_queue_render(GTK_GL_AREA(app->area));
			break;

		case GDK_KEY_leftarrow:
		case GDK_KEY_Left:
			if(app->shift_pressed) {
				app->posOffset[0] -= fabs(app->zoom) / 10;
			}
			else {
				app->posOffset[0] -= fabs(app->zoom) / 100;
			}
			write_to_clBuffer(app);
			gtk_gl_area_queue_render(GTK_GL_AREA(app->area));
			break;

		default:
			break;
	}
	return FALSE;
}

static gboolean display_controls_release(GtkWidget* widget, GdkEventKey* event, GlApplication* app) {
	if(event->keyval == GDK_KEY_Shift_L) {
		app->shift_pressed = FALSE;
		/* g_print("shift released\n"); */
		return FALSE;
	}
	/* printf("RELEASE: event->keyval: %d\n", event->keyval); */
	return FALSE;
}

void write_to_clBuffer(GlApplication* app) {
	ClProgram* clProg = &app->clProg;

    cl_int err;
    err = clEnqueueWriteBuffer(clProg->queue, clProg->posBuffer, CL_TRUE, 0, app->posSize, app->posData, 0, NULL, NULL);
    err |= clEnqueueWriteBuffer(clProg->queue, clProg->opBuffer, CL_TRUE, 0, app->opSize*sizeof(cplx), app->operations, 0, NULL, NULL);

    // Set the arguments to our compute clProg->kernel
    err  = clSetKernelArg(clProg->kernel, 0, sizeof(cl_mem), &(clProg->posBuffer));
    err |= clSetKernelArg(clProg->kernel, 1, sizeof(cl_mem), &(clProg->colorBuffer));
    err |= clSetKernelArg(clProg->kernel, 2, sizeof(cl_mem), &(clProg->opBuffer));
    err |= clSetKernelArg(clProg->kernel, 3, sizeof(int), &app->opSize);
    err |= clSetKernelArg(clProg->kernel, 4, sizeof(double), &app->zoom);
    err |= clSetKernelArg(clProg->kernel, 5, sizeof(float), &app->zoomc);
    err |= clSetKernelArg(clProg->kernel, 6, sizeof(float)*2, &app->posOffset);
    err |= clSetKernelArg(clProg->kernel, 7, sizeof(unsigned int), &app->n);
    err |= clSetKernelArg(clProg->kernel, 8, sizeof(unsigned int), &app->gridOn);
    err |= clSetKernelArg(clProg->kernel, 9, sizeof(unsigned int), &app->shadOn);
    err |= clSetKernelArg(clProg->kernel, 10, sizeof(unsigned int), &app->axesOn);

    // Execute the clProg->kernel over the entire range of the data set  
    err = clEnqueueNDRangeKernel(clProg->queue, clProg->kernel, 1, NULL, &app->globalSize, &app->localSize, 0, NULL, NULL);
 
    // Wait for the command clProg->queue to get serviced before reading back results
    clFinish(clProg->queue);
 
    // Read the results from the device
    clEnqueueReadBuffer(clProg->queue, clProg->colorBuffer, CL_TRUE, 0, app->colorSize, app->colors, 0, NULL, NULL);
}

static gboolean send_window_to_back(GtkWindow* window, GdkEvent *event, GlApplication* app) {
	GtkWidget *dialog;
	GtkFileChooser *chooser;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
	gint res;

	dialog = gtk_file_chooser_dialog_new("Save Function as Image", window, action, "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);
	chooser = GTK_FILE_CHOOSER(dialog);

	gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);

	gtk_file_chooser_set_current_name(chooser, "cplxgrapher.png");

	res = gtk_dialog_run(GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT) {
		char *filename;
		filename = gtk_file_chooser_get_filename(chooser);

		g_print("%s\n", filename);

		gtk_gl_area_make_current(GTK_GL_AREA(app->area));
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		char *data = (char*)malloc((size_t)(viewport[2] * viewport[3] * 3));

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(viewport[0], viewport[1], viewport[2], viewport[3], GL_RGB, GL_UNSIGNED_BYTE, data);

		int saved = stbi_write_png(filename, viewport[2], viewport[3], 3, data, 0);

		free(data);

		/* save_to_file (filename); */
		/* g_free (filename); */
	}
	gtk_widget_destroy (dialog);

	gtk_window_iconify(GTK_WINDOW(window));
	return TRUE;
}

static gboolean close_application(GtkWindow* window, GdkEvent* event, gtkWindow* display) {
	gtk_widget_destroy(display);
	return FALSE;
}

void quick_message (GtkWindow *parent, gchar *message) {
	GtkWidget *dialog, *label, *content_area;
	GtkDialogFlags flags;

	// Create the widgets
	flags = GTK_DIALOG_DESTROY_WITH_PARENT;
	dialog = gtk_dialog_new_with_buttons("Error",
			parent,
			flags,
			"_OK",
			GTK_RESPONSE_NONE,
			NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 75);
	content_area = gtk_dialog_get_content_area(GTK_DIALOG (dialog));
	label = gtk_label_new(message);

	// Ensure that the dialog box is destroyed when the user responds
	g_signal_connect_swapped(dialog, "response", G_CALLBACK (gtk_widget_destroy), dialog);

	// Add the label, and show everything we’ve added
	gtk_container_add(GTK_CONTAINER(content_area), label);
	gtk_widget_show_all(dialog);
}

char* funcToString(cplx* op, int opnum) {
	char funcTable[25][7] = {
		"csqrt",
		"croot",
		"clog",
		"clog",
		"cabs",
		"floor",
		"ceil",
		"casin",
		"cacos",
		"catan",
		"csinh",
		"ccosh",
		"ctanh",
		"ccosh",
		"csinh",
		"ctanh",
		"csin",
		"ccos",
		"ctan",
		"csin",
		"ccos",
		"ctan",
		"creal", 
		"cimag", 
		"cabs"
	};

	strstack s = strstackInit();

	char* oprnd[2];
	for(int i = 0; i < opnum; i++) {
		if( cimag(op[i]) == DBL_MAX) {
			if( creal(op[i]) == DBL_MAX) {
				sstr_push(&s, "z");
			}
			else {
				switch((int)creal(op[i])) {
					case 0:
					case 2:
					case 4:
					case 7:
					case 8:
					case 9:
					case 10:
					case 11:
					case 12:
					case 16:
					case 17:
					case 18:
					case 22:
					case 23:
					case 24:
						oprnd[0] = sstr_pop(&s);
						sstr_push(&s, "");
						sprintf(sstr_top(s), "%s(%s)", funcTable[(int)creal(op[i])], oprnd[0]);
						break;

					case 1:
						oprnd[0] = sstr_pop(&s);
						oprnd[1] = sstr_pop(&s);
						sstr_push(&s, "");
						sprintf(sstr_top(s), "cpow(%s,cdiv((cplx)(1,0),%s))", oprnd[1], oprnd[0]);
						break;

					case 3:
						oprnd[0] = sstr_pop(&s);
						oprnd[1] = sstr_pop(&s);
						sstr_push(&s, "");
						sprintf(sstr_top(s), "cdiv(clog(%s),clog(%s))", oprnd[0], oprnd[1]);
						break;

					case 5:
					case 6:
						oprnd[0] = sstr_pop(&s);
						sstr_push(&s, "");
						sprintf(sstr_top(s), "(cplx)(%s(%s),%s(%s))", funcTable[(int)creal(op[i])], oprnd[0], funcTable[(int)creal(op[i])], oprnd[0]);
						break;

					case 13:
					case 14:
					case 15:
					case 19:
					case 20:
					case 21:
						oprnd[0] = sstr_pop(&s);
						sstr_push(&s, "");
						sprintf(sstr_top(s), "%s(cdiv((cplx)(1,0),%s))", funcTable[(int)creal(op[i])], oprnd[0]);
						break;
				}
			}
		}
		else if( cimag(op[i]) == -DBL_MAX) {
			switch((int)creal(op[i])) {
				case 0:
					oprnd[0] = sstr_pop(&s);
					oprnd[1] = sstr_pop(&s);
					sstr_push(&s, "");
					sprintf(sstr_top(s), "cadd(%s,%s)", oprnd[1], oprnd[0]);
					break;

				case 1:
					oprnd[0] = sstr_pop(&s);
					oprnd[1] = sstr_pop(&s);
					sstr_push(&s, "");
					sprintf(sstr_top(s), "csub(%s,%s)", oprnd[1], oprnd[0]);
					break;

				case 2:
					oprnd[0] = sstr_pop(&s);
					oprnd[1] = sstr_pop(&s);
					sstr_push(&s, "");
					sprintf(sstr_top(s), "cmult(%s,%s)", oprnd[0], oprnd[1]);
					break;

				case 3:
					oprnd[0] = sstr_pop(&s);
					oprnd[1] = sstr_pop(&s);
					sstr_push(&s, "");
					sprintf(sstr_top(s), "cdiv(%s,%s)", oprnd[1], oprnd[0]);
					break;

				case 4:
					oprnd[0] = sstr_pop(&s);
					oprnd[1] = sstr_pop(&s);
					sstr_push(&s, "");
					sprintf(sstr_top(s), "cpow(%s,%s)", oprnd[1], oprnd[0]);
					break;

				case 5:
					oprnd[0] = sstr_pop(&s);
					sstr_push(&s, "");
					sprintf(sstr_top(s), "csub((cplx)(0,0),%s)", oprnd[0]);
					break;
			}
		}
		else {
			sstr_push(&s, "");
			sprintf(sstr_top(s), "(cplx)(%.5lf,%.5lf)", creal(op[i]), cimag(op[i]));
		}
	}

	return (sstr_pop(&s));
	// return cpow((cplx)(M_E, 0), (cplx)(1.5, 1.5));
	// return cpow((cplx)(M_E, 0), val );
}

