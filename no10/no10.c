#include <gtk/gtk.h>
#include <math.h>

void button_clicked(GtkWidget *widget, gpointer data);
void calculate(GtkWidget *widget, gpointer data);
double eval_expression(const char *expression);

int main(int argc, char *argv[]) {

	gtk_init(&argc, &argv);
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "간단한 계산기");
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 400);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	GtkWidget *vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	GtkWidget *entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(vbox), entry, TRUE, TRUE, 0);
	
	GtkWidget *grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 5);			
	gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);
						
	const char *button_labels[] = {"7", "8", "9", "/",
		"4", "5", "6", "*",							
		"1", "2", "3", "-",				
		"0", ".", "=", "+"};
				
	for (int i = 0; i < 16; ++i) {
		GtkWidget *button = gtk_button_new_with_label(button_labels[i]);									            
		g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), entry);

		
		if (i == 14) {
			g_signal_connect(button, "clicked", G_CALLBACK(calculate), entry);													
		}		
		int row = i / 4;											
		int col = i % 4;
				
		gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1);
	}
							        
	gtk_widget_show_all(window);								    
	gtk_main();
				
	return 0;
}

void button_clicked(GtkWidget *widget, gpointer data) {
	const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));	
	GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(data));
	gtk_entry_buffer_insert_text(buffer, -1, button_label, -1);
}

void calculate(GtkWidget *widget, gpointer data) {	
    	GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(data));
	const gchar *expression = gtk_entry_buffer_get_text(buffer);
	double result = eval_expression(expression);
	gchar *result_str = g_strdup_printf("%g", result);
	gtk_entry_buffer_set_text(buffer, result_str, -1);
	g_free(result_str);
}

double eval_expression(const char *expression) {	 
     	char op;
	double num1, num2;
	if (sscanf(expression, "%lf%c%lf", &num1, &op, &num2) == 3) {		          
	      	switch (op) {					             
		     	case '+':		
				return num1 + num2;			
			case '-':						
				return num1 - num2;				
			case '*':						
				return num1 * num2;				
			case '/':						
				if (num2 != 0) {				
					return num1 / num2;						
				} else {
					fprintf(stderr, "Division by zero\n");
								
					return 0.0;
																											                    }					
			default:				
				fprintf(stderr, "Invalid operator: %c\n", op);																		                return 0.0;
				

		}				
	} else {		
		fprintf(stderr, "Invalid input: %s\n", expression);		
		return 0.0;
								   
       	}
}

