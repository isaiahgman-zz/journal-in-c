#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//These are variable used by multiple methods, therefore they are out here.
char userName[50];
char printUser[50];
//welcome method wasn't able to see these methods unless initilazed here.
void creatingEntry();
void readingEntry();

//Method that prints the menu
int printList(char **list, int count){

	//highlight used to tell printer which one to highlight
	//choice is carried out of the menu to determine next steps
	//c is where the keypad return is stored
        int highlight = 1;
        int choice = 0;
        int c;

	//ncurses intializing the standard window
        initscr();
	//this allows the use of arrows
        keypad(stdscr, TRUE);
	//setting cursor to invisible because it is not needed in this part
        curs_set(0);
	
	//keeps the username at the top
	mvprintw(8, 10, "Username: %s", printUser);
	
	//initial print before input is received to update
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
	
	//while loop continues until enter is pressed
        while(1){

		//gets keypress
                c = getch();

		//determines what to do based on which arrow key was pressed
                switch(c){
			case KEY_UP:
				//this loops the highlighted item from the top to bottom if up is pressed again
                                if(highlight == 1){
                                        highlight = count;
                                }else{
                                        --highlight;
                                }
                                break;
                        case KEY_DOWN:
				//this loops from bottom to top if option is the last and user presses down again 
                                if(highlight == count){
                                        highlight = 1;
                                }else{
                                        ++highlight;
                                }
                                break;
                        case 10:
				//when user presses enter choice to set to something other than zero so loop breaks
                                choice = highlight;
                                break;
                }
		//prints everytime arrow key is pressed
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

		//break out of the loop when enter is pressed
		if(choice != 0){
                        break;
                }

        }

        refresh();
        endwin();
        return choice;
}

//this method was created so that after creatingEntry or readingEntry method is over than user returns to main menu to choose to continue or exit
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
	
	//This array hold each line from the file
	char dataArray[500][500];

	//opening file
	FILE *fp;
	fp = fopen(userName, "r");
	
	int count = 0;

	//file is read into dataArray
	while(fgets(dataArray[count], 500, fp) != NULL){
		++count;
	}
	
	//same as from the printList method
	int highlight = 1;
	int choice = 0;
	int c;

	initscr();
	keypad(stdscr, TRUE);
	
	char readList[100][100];
	int j = 0;

	//initial print before user input
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
	
	//based on what the user chose, that entry is displayed on the screen
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

	//these hold the title and entry entered by the user until they are place into the txt file of that username.
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

	//printing into file
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

	//time is formatted and placed on the third line of each entry section
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
	
	//this is the login screen
	mvprintw(10, 10, "Enter Username: ");
	//this takes the username and adds ".txt" so that that text file can be accessed
	getstr(printUser);
	//username saved in printUser to print userName at the top in other pages
	strcpy(userName, printUser);

	char txt[10] = ".txt";
        strcat(userName, txt);
	
	welcome();
	return 0;
}
//END 
