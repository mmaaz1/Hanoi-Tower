//Maaz Mazharul     Ms. Cullum      22nd November, 2018
//This program is basically the Hanoi tower. User can play it or let the computer do it for them
//For Allegro, only keyboard functionality is enabled
#include "Hanoi.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

//initializing allegro stuff
ALLEGRO_DISPLAY *display;
ALLEGRO_TIMER *timer;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_FONT *font2 = nullptr;
ALLEGRO_FONT *font1 = nullptr;
ALLEGRO_EVENT ev;

void hanoi(bool input, int num, Hanoi &Left, Hanoi &Middle, Hanoi &Right, int totalDisc);
void userInput(int num, Hanoi &Left, Hanoi &Middle, Hanoi &Right);
int initializeAllegro(int width, int height, const char title[]);
bool escapeKeypress();
void drawDiscs(Hanoi &stack1);
void drawAllegro(Hanoi &Left, Hanoi &Middle, Hanoi &Right);
int userSelect(int xPrev, Hanoi &Left, Hanoi &Middle, Hanoi &Right);
void userMove(int &fromVal, int &toVal, int moveFrom, int moveTo, Hanoi &Left, Hanoi &Middle, Hanoi &Right);

int main(){

    int numDisks = -1;
    bool userInput;
    int userInputTemp = -1;

    cout << "Welcome to Maaz's Hanoi Tower" << endl;
    //runs loop until user inputs a valid value: Number of discs on peg
    while(numDisks < 1 || numDisks > 10){
        cout << "How many discs do you want on the peg? (1-10) ";
        cin >> numDisks;
    }
    //runs loop until user inputs a valid value: Whether user wants to play or not
    while(userInputTemp != 0 && userInputTemp != 1){
        cout << endl << "Type \"1\" if you want to play the game. Type \"0\" if you want me to do it for you! ";
        cin >> userInputTemp;
        cout << userInputTemp;
        userInput = userInputTemp;
    }

    initializeAllegro(1500, 700, "cool");

    //initializing the three pegs
    Hanoi RightStack("Right", numDisks);
    Hanoi MiddleStack("Middle", numDisks);
    Hanoi LeftStack("Left", numDisks, numDisks);

    //drawing the screen with the background, pegs and disks
    drawAllegro(LeftStack, MiddleStack, RightStack);

    hanoi(userInput, numDisks, LeftStack, MiddleStack, RightStack, numDisks);

    //Winner scene once game is over
    ALLEGRO_COLOR BLACK = al_map_rgb(0, 0, 0);
    al_draw_text(font2, BLACK, 750, 10, ALLEGRO_ALIGN_CENTRE, "WINNER!");
    al_draw_text(font1, BLACK, 750, 100, ALLEGRO_ALIGN_CENTRE, "Press Esc To Quit");
    al_flip_display();
    cout << endl <<"Congrats! You have won the game!" << endl;

    //Keeps the allegro window open until user presses Esc Key
    bool true1 = false;
    while(true1 == false){
        true1 = escapeKeypress();
    }

    return 0;
}

void hanoi(bool input, int num, Hanoi &Left, Hanoi &Middle, Hanoi &Right, int totalDisc){

    ALLEGRO_COLOR BLACK = al_map_rgb(0, 0, 0);
    bool game = true;
    int n;

    //Code for the computer playing Hanoi
    if(input == false){
        //recursion stuff
        if(num == 1){
            n = Left.pop();
            Right.push(n);
            //delay added so that the user can actually see what's going on
            al_rest(0.41);
            drawAllegro(Left, Middle, Right);
        }
        else{
            hanoi(input, num-1, Left, Right, Middle, totalDisc);
            n = Left.pop();
            Right.push(n);
            al_rest(0.41);
            drawAllegro(Left, Middle, Right);
            hanoi(input, num-1, Middle, Left, Right, totalDisc);
        }
    }
    //code for user playing
    else{
        //x determines the position of the black square. -1 = Left, 0 = Middle, 1 = Right
        int x = 0;
        drawAllegro(Left, Middle, Right);
        al_draw_rectangle(500 + (x*495), 175, 1000 + (x*495), 650, BLACK, 14);
        al_flip_display();
        //This loop runs until the user picks enter and chooses which peg they want to take a disk out from
        while(game == true){
            ev.keyboard.keycode = ALLEGRO_KEY_BUTTON_L1;
            al_wait_for_event(event_queue, &ev);

            switch(ev.keyboard.keycode) {
            //If user presses left, move the square left if applicable and redraw the screen
            case ALLEGRO_KEY_LEFT:
                if(ALLEGRO_EVENT_KEY_UP == ev.type){
                    if(x > -1){
                        x--;
                        drawAllegro(Left, Middle, Right);
                        al_draw_rectangle(500 + (x*495), 175, 1000 + (x*495), 650, BLACK, 14);
                        al_flip_display();
                    }
                }
                break;
            //If user presses right, move the square right if applicable and redraw the screen
            case ALLEGRO_KEY_RIGHT:
                if(ALLEGRO_EVENT_KEY_UP == ev.type){
                    if(x < 1){
                        x++;
                        drawAllegro(Left, Middle, Right);
                        al_draw_rectangle(500 + (x*495), 175, 1000 + (x*495), 650, BLACK, 14);
                        al_flip_display();
                    }
                }
                break;
            //if the user presses enter, it goes to the next part: picking which peg to put the disk in
            case ALLEGRO_KEY_ENTER:
                if(ALLEGRO_EVENT_KEY_UP == ev.type){
                    int val = 0;
                    //determines which peg was chosen based on the value of x
                    if(x == -1){
                        val = Left.getStackNum();
                    }
                    if(x == 0){
                        val = Middle.getStackNum();
                    }
                    if(x == 1){
                        val = Right.getStackNum();
                    }
                    //if statement was created as a bug fix for trying to remove a disk from a peg even if had no disks
                    if(val != 0){
                        x = userSelect(x, Left, Middle, Right);
                    }
                    //ends the hanoi function if all the disks are on the right side
                    if(Right.getStackNum() == totalDisc){
                        game = false;
                    }
                    //draws the screen
                    drawAllegro(Left, Middle, Right);
                    al_draw_rectangle(500 + (x*495), 175, 1000 + (x*495), 650, BLACK, 14);
                    al_flip_display();
                }
            }
        }
    }
}

int initializeAllegro(int width, int height, const char title[]) {
    //Allegro initializations
    const float FPS = 16;            // set frame rate
    al_init();// Initialize Allegro

    display = al_create_display(width, height);    // initialize display
    if (!display) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
           return -1;
    }
    al_set_window_title(display, title);

    if (!al_install_keyboard()) {// Initialize keyboard routines
        al_show_native_message_box(display, "Error", "Error", "failed to initialize the keyboard!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
         return -1;
       }

    if (!al_init_image_addon()) {// need to add image processor
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize image addon!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    timer = al_create_timer(1.0 / FPS);    // 2. setup timer
       if (!timer) {
           al_show_native_message_box(display, "Error", "Error", "Failed to create timer!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    // Add fonts
   al_init_font_addon(); // initialize the font addon
   al_init_ttf_addon();// initialize the ttf (True Type Font) addon

    if (!al_init_primitives_addon()) {// Initialize primative add on
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize primatives addon!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {// set up event queue
        al_show_native_message_box(display, "Error", "Error", "Failed to create event_queue!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
          return -1;
    }
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));         // 3. register timer events

    font2 = al_load_ttf_font("Pokemon GB.ttf", 50, 0);
    if (!font2){
        al_show_native_message_box(display, "Error", "Error", "Could not load Pokemon GB.ttf", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    font1 = al_load_ttf_font("Pokemon GB.ttf", 15, 0);
    if (!font1){
        al_show_native_message_box(display, "Error", "Error", "Could not load Pokemon GB.ttf", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
}

bool escapeKeypress(){
    //function for when the escape key is pressed at the end of the game
    ALLEGRO_KEYBOARD_STATE key_state;
    ALLEGRO_EVENT ev;

    al_wait_for_event(event_queue, &ev);
    if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return true;
    }
    al_get_keyboard_state(&key_state);
    return al_key_down(&key_state, ALLEGRO_KEY_ESCAPE);
}

void drawAllegro(Hanoi &Left, Hanoi &Middle, Hanoi &Right){
    //Color for background and pegs
    ALLEGRO_COLOR Bckgrnd = al_map_rgb(225, 255, 250);
    ALLEGRO_COLOR Pegs = al_map_rgb(100, 60, 20);
    //Drawing background and pegs
    al_draw_filled_rectangle(000, 000, 1500, 700, Bckgrnd);
    al_draw_filled_rectangle(243, 250, 257, 600, Pegs);
    al_draw_filled_rectangle(743, 250, 757, 600, Pegs);
    al_draw_filled_rectangle(1243, 250, 1257, 600, Pegs);
    al_draw_filled_rectangle(000, 600, 1600, 650, Pegs);
    //Drawing the discs on pegs if there are any
    drawDiscs(Left);
    drawDiscs(Middle);
    drawDiscs(Right);
    al_flip_display();
}

void drawDiscs(Hanoi &stack1){
    //Color for all the pegs
    ALLEGRO_COLOR color[10] = {al_map_rgb(70, 240, 240), al_map_rgb(230, 25, 75), al_map_rgb(60, 180, 75), al_map_rgb(230, 190, 255), al_map_rgb(255, 225, 25), al_map_rgb(245, 130, 48),
    al_map_rgb(210, 245, 60), al_map_rgb(250, 190, 190), al_map_rgb(170, 110, 40), al_map_rgb(0, 128, 128)};
    //runs through this only if there are disks on pegs
    if(stack1.getStackNum() != -1){
        for(int i = 0; i < stack1.getStackNum(); i++){
            //draws the disks for 1 peg
            int val1 = 250 * stack1.getPegNum() + 25 * (stack1.getStackVal(i)) - 250;
            int val2 = 570 - 30 * i;
            int val3 = 250 * stack1.getPegNum() + 500 - 25 * (stack1.getStackVal(i)) - 250;
            int val4 = 600 - 30 * i;
            al_draw_filled_rounded_rectangle(val1-1, val2-1, val3+1, val4+1, 25, 10, al_map_rgb(0,0,0));
            al_draw_filled_rounded_rectangle(val1, val2, val3, val4, 25, 10, color[stack1.getStackVal(i)]);
        }
    }
}

int userSelect(int xPrev, Hanoi &Left, Hanoi &Middle, Hanoi &Right){
    //function for user selecting which peg to put the disk in
    //Basically the same code as the previous screen. I didn't know how to use the same function to lessen the code as
    //pressing "enter" does different things and I would have to somehow recurse it once and set base cases and stuff
    ALLEGRO_COLOR WHITE = al_map_rgb(128,0,128);
    ALLEGRO_COLOR BLACK = al_map_rgb(0, 0, 0);

    int x = xPrev;
    bool game = true;

    al_draw_rectangle(500 + (xPrev*495), 175, 1000 + (xPrev*495), 650, BLACK, 14);
    al_draw_rectangle(500 + (x*495), 175, 1000 + (x*495), 650, WHITE, 14);
    al_flip_display();
    while(game == true){
        ev.keyboard.keycode = ALLEGRO_KEY_BUTTON_L1;
        al_wait_for_event(event_queue, &ev);
        switch(ev.keyboard.keycode) {
        case ALLEGRO_KEY_LEFT:
            if(ALLEGRO_EVENT_KEY_UP == ev.type){
                if(x > -1){
                    x--;
                    drawAllegro(Left, Middle, Right);
                    al_draw_rectangle(500 + (xPrev*495), 175, 1000 + (xPrev*495), 650, BLACK, 14);
                    al_draw_rectangle(500 + (x*495), 175, 1000 + (x*495), 650, WHITE, 14);
                    al_flip_display();
                }
            }
            break;
        case ALLEGRO_KEY_RIGHT:
            if(ALLEGRO_EVENT_KEY_UP == ev.type){
                if(x < 1){
                    x++;
                    drawAllegro(Left, Middle, Right);
                    al_draw_rectangle(500 + (xPrev*495), 175, 1000 + (xPrev*495), 650, BLACK, 14);
                    al_draw_rectangle(500 + (x*495), 175, 1000 + (x*495), 650, WHITE, 14);
                    al_flip_display();
                }
            }
            break;
        case ALLEGRO_KEY_ENTER:
            if(ALLEGRO_EVENT_KEY_UP == ev.type){
                //to is the value of the disk that the user is trying to put on top of. from is the value of the disk that the user wants to move
                int to;
                int from;
                userMove(from, to, xPrev, x, Left, Middle, Right);
                if(from > to){
                    //if the value of the disk on top is smaller than the disk on bottom, mission accomplished
                    game = false;
                    return x;
                }
                else{
                    //or else, reverse the move and keep the loop going
                    userMove(to, from, x, xPrev, Left, Middle, Right);
                }
            }
        case ALLEGRO_KEY_ESCAPE:
            if(ALLEGRO_EVENT_KEY_UP == ev.type){
                //cancels the previous commend if Esc is pressed
                game = false;
                return x;
            }
        }
    }
}

void userMove(int &fromVal, int &toVal, int moveFrom, int moveTo, Hanoi &Left, Hanoi &Middle, Hanoi &Right){
    //this funcion interprets what the moveFrom and moveTo values are and pulls and pushes accordingly
    int n;
    if(moveFrom == -1){
        fromVal = Left.getStackVal();
        n = Left.pop();
    }
    else if(moveFrom == 0){
        fromVal = Middle.getStackVal();
        n = Middle.pop();
    }
    else if(moveFrom == 1){
        fromVal = Right.getStackVal();
        n = Right.pop();
    }

    if(moveTo == -1){
        toVal = Left.getStackVal();
        Left.push(n);
    }
    if(moveTo == 0){
        toVal = Middle.getStackVal();
        Middle.push(n);
    }
    if(moveTo == 1){
        toVal = Right.getStackVal();
        Right.push(n);
    }
}
