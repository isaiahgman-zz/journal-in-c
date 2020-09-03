#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char userName[50];
char printUser[50];
void creatingEntry();
void readingEntry();

int printList(char **list, int count){

        int highlight = 1;
        int choice = 0;
        int c;

        initscr();
        keypad(stdscr, TRUE);
        curs_set(0);
	
	mvprintw(8, 10, "Username: %s", printUser);

        for(int i = 0; i < count; i++){
                if(highlight == i + 1){
                        attron(A_REVERSE);
                        mvprintw(i + 10, 10, "%s", list[i]);
                        attroff(A_REVERSE);
                }else{
                        mvprintw(i + 10, 10, "%s", list[i]);
                }
        }
        refresh();

        while(1){

                c = getch();

                switch(c){
                        case KEY_UP:
                                if(highlight == 1){
                                        highlight = count;
                                }else{
                                        --highlight;
                                }
                                break;
                        case KEY_DOWN:
                                if(highlight == count){
                                        highlight = 1;
                                }else{
                                        ++highlight;
                                }
                                break;
                        case 10:
                                choice = highlight;
                                break;
                }
                for(int i = 0; i < count; i++){
                        if(highlight == i + 1){
                                attron(A_REVERSE);
                                mvprintw(i + 10, 10, "%s", list[i]);
                                attroff(A_REVERSE);
                        }else{
                                mvprintw(i + 10, 10, "%s", list[i]);
                        }
                }
                refresh();

		if(choice != 0){
                        break;
                }

        }

        refresh();
        endwin();
        return choice;
}

void welcome(){

        erase();

        char *options[] = {
                                "Create New Entry",
                                "Read Existing Entries",
                                "Exit"
                          };

        int n_choices = sizeof(options) / sizeof(options[0]);

        int choice = printList(options, n_choices);

        if(choice == 1){
                creatingEntry();
        }else if(choice == 2){
                readingEntry();
        }else{
                return;
        }
}

void readingEntry(){
	
	char dataArray[500][500];

	FILE *fp;
	fp = fopen(userName, "r");
	
	int count = 0;

	while(fgets(dataArray[count], 500, fp) != NULL){
		++count;
	}
	
	int highlight = 1;
	int choice = 0;
	int c;

	initscr();
	keypad(stdscr, TRUE);
	
	char readList[100][100];
	int j = 0;

	for (int i = 0; i < count; i+=3){
		strcpy(readList[j], dataArray[i]);
		strcat(readList[j], "\t  ");
		strcat(readList[j], dataArray[i+2]);
		++j;		
	}
	
	mvprintw(8, 10, "Username: %s", printUser);

	for(int i = 0; i < j; i++){
		if(highlight == i + 1){
			attron(A_REVERSE);
			mvprintw((i+i)+10, 10, readList[i]);
			attroff(A_REVERSE);
		}else{
			mvprintw((i+i)+10, 10, readList[i]);
		}
	}
        refresh();
        
	while(1){

                c = getch();

                switch(c){
                        case KEY_UP:
                                if(highlight == 1){
                                        highlight = j;
                                }else{
                                        --highlight;
                                }
                                break;
                        case KEY_DOWN:
                                if(highlight == j){
                                        highlight = 1;
                                }else{
                                        ++highlight;
                                }
                                break;
                        case 10:
                                choice = highlight;
                                break;
                }
                for(int i = 0; i < j; i++){
                	if(highlight == i + 1){
                        	attron(A_REVERSE);
                        	mvprintw((i+i)+10, 10, readList[i]);
                        	attroff(A_REVERSE);
                	}else{
                        	mvprintw((i+i)+10, 10, readList[i]);
                	}
        	}
                refresh();

                if(choice != 0){
                        break;
                }
        }
	erase();
	mvprintw(8, 10, "Username: %s", printUser);
        mvprintw(10, 10, "Entry Title : %s", dataArray[((choice-1) * 3)]);
	mvprintw(12, 10, dataArray[((choice-1) * 3) + 1]);
	refresh();	
	getch();
	endwin();
	welcome();
	return;
}
void creatingEntry(){

        char title[50];
	char entry[500];
	
	initscr();
	erase();
	
	mvprintw(8, 10, "Username: %s\n", printUser);
	curs_set(1);
	mvprintw(10, 10, "Enter Title: ");
	getstr(title);
	
	mvprintw(12, 10, "Enter Entry: ");
	getstr(entry);

        //We are creating file output to save entry
        FILE * fp;
        fp = fopen(userName, "a");

        fprintf(fp, title);
        fprintf(fp, "\n");
        fprintf(fp, entry);
	fprintf(fp, "\n");
        //Here we timestamp the note to make it easier to find in the Read Menu
        char cur_time[128];
	time_t      t;
        struct tm*  ptm;

        t = time(NULL);
        ptm = localtime(&t);

        strftime(cur_time, 128, "%d-%b-%Y %H:%M", ptm);
        fprintf(fp, cur_time);
        fprintf(fp, "\n");
	endwin();
	welcome();
        return;
}

int main(){

	initscr();	
	keypad(stdscr, TRUE);

	mvprintw(10, 10, "Enter Username: ");	
	getstr(printUser);
	strcpy(userName, printUser);

	char txt[10] = ".txt";
        strcat(userName, txt);
	
	welcome();
	return 0;
}
