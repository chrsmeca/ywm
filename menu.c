#include "menu.h"

#define TITLE_BAR_HEIGHT 22

#define MENU_TITLE_COLOR "#999aba"
#define MENU_TITLE_LIGHT_STRIP "#bfbfcc"
#define MENU_TITLE_DARK_STRIP "#5d5d68"
#define MENU_BG_COLOR "#AAAAAA"
#define FLASH_COLOR "#ccc8d1"

GC flash_gc;
GC menu_title_gc;
GC menu_light_strip_gc;
GC menu_dark_strip_gc;

void destroy_menuItem(void *data) {
  MenuItem *item = (MenuItem *)data;
  free(item->label);
  free(item->command);
  free(item);
}

MenuItem* menuItem_new(char *label, char *command, Window win) 
{
  MenuItem *menuItem = malloc(sizeof(MenuItem));
  menuItem->label = malloc(sizeof(char) * strlen(label));
  menuItem->command = malloc(sizeof(char) * strlen(command));
  menuItem->window = win;
  strncpy(menuItem->label, label, strlen(label));
  strncpy(menuItem->command, command, strlen(command));
	
  return menuItem;
}

Window create_menu()
{	
  XColor bg = create_color(MENU_BG_COLOR);
  XGCValues gcv;
  gcv.function = GXcopy;

  gcv.foreground = create_color(FLASH_COLOR).pixel;
  flash_gc = XCreateGC(dpy, root, GCFunction|GCForeground, &gcv);  
  gcv.foreground = create_color(MENU_TITLE_COLOR).pixel;
  menu_title_gc = XCreateGC(dpy, root, GCFunction|GCForeground, &gcv);
  gcv.foreground = create_color(MENU_TITLE_LIGHT_STRIP).pixel;
  menu_light_strip_gc = XCreateGC(dpy, root, GCFunction|GCForeground, &gcv);
  gcv.foreground = create_color(MENU_TITLE_DARK_STRIP).pixel;
  menu_dark_strip_gc = XCreateGC(dpy, root, GCFunction|GCForeground, &gcv);
  
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
     
  char menupath[1024]; 
  snprintf(menupath, sizeof(menupath), "%s/.ywm/ywm.menu", getenv("HOME"));
  fp = fopen(menupath, "r");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open %s/.ywm/ywm.menu\n", getenv("HOME"));
    exit(EXIT_FAILURE);
  } 	
  int menu_size = 0;
  while(!feof(fp)) {
    char ch = fgetc(fp);
    if(ch == '\n') {
      menu_size++;
    }
  }
  rewind(fp);
  ylist_init(&menu_items, destroy_menuItem);
	  
  Window menu_win = XCreateSimpleWindow(dpy,
                                        DefaultRootWindow(dpy),
                                        0, 0, MENU_MAX_WIDTH, MENU_ITEM_HEIGHT * menu_size + TITLE_BAR_HEIGHT, 0,
                                        BlackPixel(dpy, 0),
                                        bg.pixel);
  XSelectInput(dpy, menu_win, ButtonMask | ExposureMask);

  int index = 0;
  while ((read = getline(&line, &len, fp)) != -1) {
    size_t ln = strlen(line) - 1;
    if (line[ln] == '\n') {
      line[ln] = '\0';
    }
    char *label = strtok(line, "|");
    char *command = strtok(NULL, "|");
    Window menu_item = XCreateSimpleWindow(dpy,
                                           menu_win,
                                           0, MENU_ITEM_HEIGHT * index + TITLE_BAR_HEIGHT, MENU_MAX_WIDTH, MENU_ITEM_HEIGHT, 0,
                                           BlackPixel(dpy, 0),
                                           bg.pixel);
		
    XSelectInput(dpy, menu_item, ButtonMask | ExposureMask);
    MenuItem *menuItem = menuItem_new(label, command, menu_item);
    ylist_ins_next(&menu_items, ylist_head(&menu_items), menuItem);
    index++;
  }
	
  if (line) {
    free(line);
  }
  
  fclose(fp);	
  XMapSubwindows(dpy, menu_win);

  return menu_win;
}

void draw_menu_title() {
  int width = MENU_MAX_WIDTH - 1;
  int height = MENU_ITEM_HEIGHT - 1;
	
  XFillRectangle(dpy, root_menu, menu_title_gc, 0, 0, width, height);
  XDrawLine(dpy, root_menu, focused_light_grey_gc, 0, 0, width, 0);
  XDrawLine(dpy, root_menu, focused_light_grey_gc, 0, 1, 0, height);
  XDrawLine(dpy, root_menu, focused_dark_grey_gc, 1, height - 1, width, height - 1);
  XDrawLine(dpy, root_menu, focused_dark_grey_gc, width - 1, 1, width - 1, height - 1);
  XDrawLine(dpy, root_menu, XDefaultGC(dpy, DefaultScreen(dpy)), width, 0, width, height);
  XDrawLine(dpy, root_menu, XDefaultGC(dpy, DefaultScreen(dpy)), 0, height, width, height);
  for (int i = 0; i < 12; i++) {
    int y = 5 + i;
    if (i % 2 == 0) {
      XDrawLine(dpy, root_menu, menu_light_strip_gc, 5, y, MENU_MAX_WIDTH - 8, y);
    } else {
      XDrawLine(dpy, root_menu, menu_dark_strip_gc, 6, y, MENU_MAX_WIDTH - 7, y);
    }
  }	
}

static void draw_menu_item(MenuItem* menu_item, int flash) {
  int width = MENU_MAX_WIDTH - 1;
  int height = MENU_ITEM_HEIGHT - 1;
  Window menu_btn = menu_item->window;
  	
  XDrawLine(dpy, menu_btn, unfocused_light_grey_gc, 0, 0, width, 0);
  XDrawLine(dpy, menu_btn, unfocused_light_grey_gc, 0, 1, 0, height);
  XDrawLine(dpy, menu_btn, unfocused_dark_grey_gc, 1, height - 1, width, height - 1);
  XDrawLine(dpy, menu_btn, unfocused_dark_grey_gc, width - 1, 1, width - 1, height - 1);
  XDrawLine(dpy, menu_btn, XDefaultGC(dpy, DefaultScreen(dpy)), width, 0, width, height);
  XDrawLine(dpy, menu_btn, XDefaultGC(dpy, DefaultScreen(dpy)), 0, height, width, height);
  if (flash) {
    XFillRectangle(dpy, menu_btn, flash_gc, 0, 0, width - 1, height - 2);
  } else {
    XFillRectangle(dpy, menu_btn, focused_frame_gc, 2, 2, width - 3, height - 3);
  }
  if (strlen(menu_item->label) > 0) {
    XDrawString(dpy, menu_btn, XDefaultGC(dpy, DefaultScreen(dpy)), 7, 15, menu_item->label, strlen(menu_item->label));
  }	
}

void draw_menu() {	
  YNode *curr = ylist_head(&menu_items);
  while (curr != NULL) {
    MenuItem *menuItem = (MenuItem *)curr->data;
    draw_menu_item(menuItem, 0);
    curr = curr->next;
  }
  draw_menu_title();
}

void flash_menu(MenuItem* menu_item) {
  for (int i = 0; i < 6; i++) {	
    if (i % 2 != 0) {
      draw_menu_item(menu_item, 1);
    } else {
      draw_menu_item(menu_item, 0);
    }

    XSync(dpy, True);
    usleep(22500);
  }
}

void free_menu() {
  printf("Freeing menu\n");
  ylist_destroy(&menu_items);
  XFreeGC(dpy, menu_title_gc);
  XFreeGC(dpy, flash_gc);
  XFreeGC(dpy, menu_light_strip_gc);
  XFreeGC(dpy, menu_dark_strip_gc);
  XDestroyWindow(dpy, root_menu);
}
