#include <gtk/gtk.h>
/* 콜백 함수 */
static void destroy(GtkWidget*widget, gpointer data)
{
gtk_main_quit ();
}
int main(int argc, char*argv[])
{
/* GtkWidget
은 위젯들에 대한 저장 타입임. */
GtkWidget*window, *button;
gtk_init (&argc, &argv);

window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
gtk_container_set_border_width (GTK_CONTAINER (window), 10);

button = gtk_button_new_with_label ("Hello World");

g_signal_connect(button, "clicked",G_CALLBACK (destroy), NULL);

gtk_container_add (GTK_CONTAINER (window), button);
/* 생성한 모든 위젯들을 화면에 표시 */
gtk_widget_show(button);
gtk_widget_show(window);
/* 무한루프를 돌며 시그널과 이벤트를 처리하는 메인 함수 실행 */
gtk_main ();
return 0;
}
