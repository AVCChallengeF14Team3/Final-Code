#include <stdio.h>
#include <time.h>
#include <string.h>

extern "C" int init (int d_lev);
extern "C" int connect_to_server ( char server_addr[], int port);
extern "C" int send_to_server (char message[12]);
extern "C" int recieve_from_server (char message[12]);

extern "C" int Sleep( int sec, int usec );
extern "C" int set_motor( int motor, int speed );
extern "C" int take_picture();
extern "C" int open_screen_stream();
extern "C" char get_pixel(int row, int col, int colour);


int main(){
    init(1);
    
    // NETWORK CODE START
    connect_to_server("130.195.6.196", 1024);
    send_to_server("Please");
    char message[12];
    recieve_from_server(message);
    send_to_server(message);
    printf("%s", message);

    const int SPEED = 70;
    int whiteValue=0;
    int whiteOrBlack[15];   // White = 1, Black = 0
    int startOfWhite, endOfWhite;
    float centerOfWhite;
    
    bool reversed = false;
    bool redColorRecognised = false;        // This turning to true will signify it has reached the start of quadrant 4.
    
    //LINE FOLLOWING METHOD (QUADRANT ONE TO THREE)
    while(redColorRecognised == false){
    
        startOfWhite = 0;
        endOfWhite = 0;
        reversed = false;
    
        take_picture();
    
        int open_screen_stream();
    
        for(int i = 0; i < 16; i++){
            whiteValue = get_pixel(i*20, 120, 3);
            if(whiteValue > 100){    //127 was too high - white values started at about 110
                whiteOrBlack[i] = 1;
            }
        
            else{
                whiteOrBlack[i] = 0;
            }
        }
        
        //finds the start and end of white
        for(int i=0;i<15;i++){
            if( (whiteOrBlack[i]==0) && (whiteOrBlack[i+1]==1) ){
                startOfWhite = i;
            }
        
            if((whiteOrBlack[i] == 1) && (whiteOrBlack[i + 1] == 0)){ 
                endOfWhite = i;
            }
        }
        
        //in other words, if it cannot see a line in front of it
        if(startOfWhite == 0 && endOfWhite == 0){ //SET THESE VALUES EQUAL TO VALUES YOU RECIEVE WHEN NO LINE IS DETECTED.
            set_motor(1, -1*SPEED); //this is reversed
            set_motor(2, SPEED); //reversed
            reversed = true;

        }
    
            //CHECK IF AT QUAD 3.
        bool Q3 = false;
        
        if((get_pixel(80, 90,3) && get_pixel(240, 90,3) ){ // need to check these pixels to see if they are what we get at an intersection
                Q3 = true;
            }
        if((Q3 == true)){
            if(get_pixel(120, 190,3)>100 || (get_pixel(120,160,3))){
                set_motor(1, 70);//FORWARD VALUE
                set_motor(2, -70);//IDENTICAL VALUE TO ABOVE
            }
            else if((get_pixel (70, 90, 3)> 100|| get_pixel (90,90,3)>100)  ) { //LEFT WHEN LEFT OR RIGHT, IF LEFT IS TRUE AND FORWARD ISNT WE AWAYS WANT IT TO GO LEFT 
                set_motor(1, 0);// //LEFT VALUE 90 DEGREE TURN ON THE SPOT
                set_motor(2, -70);
            }
            else if(get_pixel (240, 90, 3)> 100|| get_pixel (220,90,3)>100 ) {
                set_motor(1, 70);//IF RIGHT IS THE ONLY OPTION, TURN RIGHT
                set_motor(2, 0);
            }
            
        }
        
        if(reversed == false){                            // If the robot has just reversed we dont want it to then go off the previous picture which must have had no white in it, therefore we want it to take another picture and try moving again
            
            float centerofWhite = (startOfWhite + endOfWhite)/2;
            float errorValue = centerOfWhite - 7.5;           // 7.5 is the center, therefore when it is equal the error value will be zero                            // We need to test this variable as different sizes in order to find a size that corrects/ prevents oscillation.

            double kp=1;

            double proportionalError = (double)errorValue*kp;

            int adjust = (int) proportionalError;

            set_motor(1, SPEED+adjust);
            set_motor(2, -1*(SPEED-adjust));
        }

    } //this while loop will be broken only when red colour is detected. This signals the start of quadrant 4.
    
}
