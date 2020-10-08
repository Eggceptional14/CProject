#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <raylib.h>

//----------------------------------------------------------
// Defines
//----------------------------------------------------------

#define MIDNIGHT (Color) {43,43,45}

//----------------------------------------------------------
// Type and Structure definition
//----------------------------------------------------------

typedef struct Cactus{
    
    Texture2D cactus;
    
}Cactus;

typedef struct Map{
    
    Texture2D foreground;
    Texture2D background;
    Texture2D moon;
    Texture2D meteor;
    
}Map;

typedef struct Unit{
    
    Texture2D unit_still;
    Texture2D motion;
    Texture2D sleep;
    Texture2D gameover;
    Texture2D surprise;
    
}Unit;

typedef struct ETC{
    
    SpriteFont font;
    Texture2D screen;
    
}ETC;


//----------------------------------------------------------
// Structure variable
//----------------------------------------------------------

static Cactus cactus[4];
static Unit unit;
static Map map;
static ETC gamefont, loadpage;

//----------------------------------------------------------
// Global variable
//----------------------------------------------------------


// Window variable
//-------------------------------

static const int Windowwidth = 1280;
static const int Windowheight = 720;
bool first_open = true;


// Unit variable
//-------------------------------

int unit_x, unit_y; //position

int isJumping; //Jump
int jump_delay;
int jump_check;

int loading_unit;

float timer; //frame update
int frame;

// Score and Other
//-------------------------------

bool gameOver = false;
bool isPause = false;
int score = 0;
int hiscore = 0;
float tick = 0.0;

int key_press_check = 0;

int meteor_unit;
float load_timer;
int load_frame;

// Map variable
//-------------------------------
float scrollingfore = 0.0f;
float scrollingback = 0.0f;

float scrolling_speed = 9.0; //spped of map & cactus

// Cactus Variable
//-------------------------------

int style, style_2, style_3;
int cactus_x, cactus_x_2, cactus_x_3;

//----------------------------------------------------------
// Function declaration
//----------------------------------------------------------

void cactusRender(void); //load cactus texture
void mapRender(void); //load map texture
void unitRender(void); //load character texture
void etcRender(void); //load other stuff
void textureload(void); //load texture of cactus, map and character

void unloadGame(void); //unload all of the texture
void InitGameVar(void); //initialize game variable

void DrawGame(void); //Draw game oneframe
void UpdateGame(void); //Update game oneframe
void UpdateDrawGame(void); //Update and draw game oneframe

void jump(void); //Jump function of character


//----------------------------------------------------------
// Main entry point
//----------------------------------------------------------

int main(){
    
    // Initialization
    //----------------------------------------------------------
    
    InitWindow(Windowwidth, Windowheight, "Roger The T-Rex");
    
    HideCursor();
    
    textureload();
    
    SetTargetFPS(60);
    
    InitGameVar();
    
    while ( !WindowShouldClose() ) {
        
        UpdateDrawGame();
        
    }
    
    //De-Initialization
    //----------------------------------------------------------
    
    unloadGame(); //Unload all texture
    
    CloseWindow(); //Close window
    
}

//----------------------------------------------------------
// Unload texture function
//----------------------------------------------------------

void unloadGame(void){
    
    UnloadTexture(map.foreground);
    UnloadTexture(map.background);
    UnloadTexture(unit.motion);
    UnloadTexture(unit.unit_still);
    UnloadTexture(unit.sleep);
    UnloadTexture(unit.gameover);
    UnloadTexture(unit.surprise);
    UnloadTexture(map.meteor);
    UnloadTexture(loadpage.screen);
    for (int i = 0; i < 4; i++)
        UnloadTexture(cactus[i].cactus);
    UnloadFont(gamefont.font);
    
}

//----------------------------------------------------------
// Texture rendering function
//----------------------------------------------------------

void cactusRender(void){
    
    cactus[0].cactus = LoadTexture("Cactus1.png");
    cactus[1].cactus = LoadTexture("Cactus2.png");
    cactus[2].cactus = LoadTexture("Cactus3.png");
    cactus[3].cactus = LoadTexture("MultiCactus.png");
    
}

void unitRender(void){
    
    unit.unit_still = LoadTexture("Still.png");
    unit.motion = LoadTexture("motion.png");
    unit.sleep = LoadTexture("sleeping.png");
    unit.gameover = LoadTexture("Gameover.png");
    unit.surprise = LoadTexture("Surprise.png");
    
}

void mapRender(void){
    
    map.foreground = LoadTexture("foreground.png");
    map.background = LoadTexture("Background.png");
    map.moon = LoadTexture("Moon.png");
    map.meteor = LoadTexture("Meteor.png");
    
}

void etcRender(void){
    
    gamefont.font = LoadFont("PressStart2P-Regular.ttf");
    loadpage.screen = LoadTexture("Firstpage.png");
    
}

void textureload(){
    
    cactusRender();
    unitRender();
    mapRender();
    etcRender();
    
}

//----------------------------------------------------------
// Initializing variable
//----------------------------------------------------------

void InitGameVar(){
    
    unit_x = 80;
    unit_y = 300;

    isJumping = 0;
    jump_delay = 21;
    jump_check = 0;

    timer = 0.0f;
    frame = 0;

    gameOver = false;
    isPause = false;
    score = 0;

    key_press_check = 0;
    
    scrollingfore = 0.0f;
    scrollingback = 0.0f;

    scrolling_speed = 9;
    
    style = GetRandomValue(0, 3);
    style_2 = GetRandomValue(0, 3);
    style_3 = GetRandomValue(0, 3);
    cactus_x = 1800;
    cactus_x_2 = 2900;
    cactus_x_3 = 3500;
    
    loading_unit = 400;
    meteor_unit = 1;
    
    load_frame = 0;
    load_timer = 0.0f;
    
}

//----------------------------------------------------------
// Character jumping function
//----------------------------------------------------------

void jump(){
    
    if ( isJumping == 0 )  { unit_y -= jump_delay; jump_delay--; }
    
    //falling
//    else if( isJumping == 1 ) {
//        if (unit_y >= 300)   isJumping = 0;
//        unit_y += jump_delay;
//        jump_delay++ ;
//    }
    
    if( unit_y == 300 ) {  //Floor position -> at floor reset everything
        jump_delay = 21;
        isJumping = 0;
        jump_check = 0;
    }
}

//----------------------------------------------------------
// Draw and Update game frame
//----------------------------------------------------------

//Draw all of the game texture
void DrawGame(){
    
    float framewidth = (float)(unit.motion.width / 2);
    
    int screenmid_x = GetScreenWidth() / 2;
    int screenmid_y = GetScreenHeight() / 2;
    
    BeginDrawing();
        
    ClearBackground(WHITE);
    
    if ( !gameOver ){
        
        if ( first_open ){
            
            if ( loading_unit <= ( 640 - unit.surprise.width / 2 ) )
                DrawTextureRec(unit.motion, (Rectangle){ ( framewidth * load_frame ), 0, framewidth, (float)unit.motion.height},
                (Vector2){loading_unit, 300}, WHITE);
            
            if ( loading_unit >= ( 640 - unit.surprise.width / 2 ) )
                DrawTexture(unit.surprise, loading_unit , 300, WHITE);
            
            DrawTexture(map.meteor, 750 - meteor_unit, 100 + meteor_unit, WHITE);
            
            DrawTexture(loadpage.screen, 0, 0, WHITE);
            
        }
        
        else if ( key_press_check == 0  && first_open == false ){

            DrawTextureEx(map.foreground, (Vector2){ scrollingfore, 20 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(map.background, (Vector2){ scrollingback, 20 }, 0.0f, 2.0f, WHITE);
            DrawTexture(unit.unit_still, unit_x,unit_y,WHITE);

        }
        
        else if ( key_press_check == 1 ){
            
            if ( isPause ){
                ClearBackground(MIDNIGHT);
                DrawTexture(unit.sleep, screenmid_x - unit.sleep.width / 2 + 10,350, WHITE);
                DrawTexture(map.moon, 800, 190, WHITE);
                DrawTextEx(gamefont.font, "Game Paused",
                           (Vector2){ screenmid_x - MeasureText("Game Paused", 30) / 2, screenmid_y - 100 },
                           20, 0, GRAY);
            }
            
            else{
            
                DrawTextureEx(map.foreground, (Vector2){ scrollingfore, 20 }, 0.0f, 2.0f, WHITE);
                DrawTextureEx(map.foreground, (Vector2){ map.foreground.width*2 + scrollingfore, 20 }, 0.0f, 2.0f, WHITE);
                        
                DrawTextureEx(map.background, (Vector2){ scrollingback, 20 }, 0.0f, 2.0f, WHITE);
                DrawTextureEx(map.background, (Vector2){ map.background.width*2 + scrollingback, 20 }, 0.0f, 2.0f, WHITE);
                
                DrawTexture(cactus[style].cactus, cactus_x, 300, WHITE);
                DrawTexture(cactus[style_2].cactus, cactus_x_2, 300, WHITE);
                DrawTexture(cactus[style_3].cactus, cactus_x_3, 300, WHITE);
                
                if ( jump_check == 1 ){
                    
                    DrawTexture(unit.unit_still, unit_x,unit_y,WHITE);
                    
                }
                else{
                    
                    DrawTextureRec(unit.motion,
                    (Rectangle){ ( framewidth * frame ),
                     0, framewidth, (float)unit.motion.height},
                    (Vector2){unit_x, unit_y}, WHITE);
                    
                }
            
            }
            
            DrawTextEx(gamefont.font, FormatText("High Score %05d",hiscore), (Vector2){1024,30}, 13, 0, BLACK);
            DrawTextEx(gamefont.font, FormatText("Score %05d",score), (Vector2){1089,60}, 13, 0, BLACK);
            
        }
        
        
        
    }
    else{
        
        DrawTexture(unit.gameover, screenmid_x - ( unit.gameover.width / 2 ) + 55, screenmid_y - 230, WHITE);
        DrawTextEx(gamefont.font, "Game Over!",
                   (Vector2){ screenmid_x - MeasureText("Game Over!", 30) / 2, screenmid_y - 30 }
                   , 20, 0, BLACK);
        DrawTextEx(gamefont.font, FormatText("High Score %05d",hiscore),
                   (Vector2){ screenmid_x - MeasureText("High Score %05d", 25) / 2, screenmid_y + 20 }
                   , 15, 0, BLACK);
        DrawTextEx(gamefont.font, "Press [Enter] to play again",
                   (Vector2){ screenmid_x - MeasureText("Press [Enter] to play again", 25) / 2, screenmid_y + 60 }
                   , 15, 0, BLACK);
        
    }
    
    EndDrawing();

}

//Update game ( All game function )
void UpdateGame(){
    
    // Local variable
    float framewidth = (float)(unit.motion.width / 2);
    int maxframe = ((int)unit.motion.width / (int)framewidth);
    
    float cactus_framewidth = (float)cactus[style].cactus.width;
    float cactus_frameheight = (float)cactus[style].cactus.height;

    float cactus2_framewidth = (float)cactus[style_2].cactus.width;
    float cactus2_frameheight = (float)cactus[style_2].cactus.height;
    
    float cactus3_framewidth = (float)cactus[style_3].cactus.width;
    float cactus3_frameheight = (float)cactus[style_3].cactus.height;
    
    Rectangle unitrec = {unit_x, unit_y, framewidth, framewidth };
    Rectangle cactusrec = { cactus_x, 300, cactus_framewidth, cactus_frameheight};
    Rectangle cactusrec_2 = { cactus_x_2, 300, cactus2_framewidth, cactus2_frameheight};
    Rectangle cactusrec_3 = { cactus_x_3, 300, cactus3_framewidth, cactus3_frameheight};
    
    if ( !gameOver ){
        
        if ( first_open ){
            
            load_timer += GetFrameTime(); //Update Unit frame (when running)
            
            if ( load_timer >= 0.13f ){
                load_timer = 0.0f;
                load_frame += 1;
            }
            
            load_frame = load_frame % maxframe;
            
            if ( loading_unit <= ( 640 - unit.surprise.width / 2 ) )
                loading_unit += 1;
            
            if ( loading_unit >= ( 640 - unit.surprise.width / 2 ) && meteor_unit <= 70)
                meteor_unit += 1;
            
        }
        
        if ( !gameOver && ( IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) ) ) {
            
            key_press_check = 1;
            first_open = false;
            
        }
        if ( IsKeyPressed(KEY_P) ) isPause = !isPause;
        
        if ( key_press_check == 1 && !isPause ){
            
            if ( tick == 1.0 ) { score += 1; tick = 0.0; }//Score Counting
            tick += 0.25;
            
            timer += GetFrameTime();
            
            if ( timer >= 0.12f ){
                timer = 0.0f;
                frame += 1;
            }
            
            frame = frame % maxframe;
            
            //Foreground and Background Scrolling
            scrollingfore -= scrolling_speed;
            scrollingback -= 0.5;
                    
            if ( scrollingfore <= -map.foreground.width * 2 ) scrollingfore = 0;
            if ( scrollingback <= -map.background.width * 2 ) scrollingback = 0;
            
            // Character jump
            if ( ( IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) ) ) jump_check = 1;
            
            if ( jump_check == 1 ){
                jump();
            }
            
            //Check for collision
            if ( CheckCollisionRecs( unitrec, cactusrec ) || CheckCollisionRecs( unitrec, cactusrec_2) || CheckCollisionRecs( unitrec, cactusrec_3)){
                gameOver = true;
            }
            
            //Increase Speed
            if ( score != 0 && score % 250 == 0 ) scrolling_speed += 0.25;
            
            //Speed of cactus
            cactus_x -= scrolling_speed;
            cactus_x_2 -= scrolling_speed;
            cactus_x_3 -= scrolling_speed;
            
            //Re-Generating cactus
            if ( cactus_x < -cactus_framewidth ){
                
                style = GetRandomValue(0, 3);
                cactus_x = GetRandomValue(15, 20) * 100;
            
            }
                
            if ( cactus_x_2 < -cactus2_framewidth ){
                
                style_2 = GetRandomValue(0, 3);
                cactus_x_2 = GetRandomValue(25, 30) * 100;
                    
            }
            
            if ( cactus_x_3 < -cactus3_framewidth ){
                
                style_3 = GetRandomValue(0, 3);
                cactus_x_3 = GetRandomValue(35, 40) * 100;
                
            }
            
            if ( cactus_x_2 > cactus_x ){
                
                    if ( cactus_x_2 - cactus_x < 500 )
                    cactus_x_2 += 500;
                    
            }
            if ( cactus_x > cactus_x_2 ){
                
                if ( cactus_x - cactus_x_2 < 500 )
                    cactus_x += 500;
                
            }
            
            if ( cactus_x_3 > cactus_x_2 ){
                
                if ( cactus_x_3 - cactus_x_2 < 500 )
                    cactus_x_3 += 500;
                    
            }
            
            if ( cactus_x_2 > cactus_x_3 ){
                
                if ( cactus_x_2 - cactus_x_3 < 500 )
                    cactus_x_2 += 500;
                
            }
            
            if ( cactus_x > cactus_x_3 ){
                
                if ( cactus_x - cactus_x_3 < 500 )
                    cactus_x += 500;
                
            }
            
            if ( cactus_x_3 > cactus_x ){
                
                if ( cactus_x_3 - cactus_x < 500 )
                    cactus_x_3 += 500;
                
            }
                
        }
        
    }
    
    else{ //If game over
        
        //Set hiscore
        if ( score > hiscore ) hiscore = score;
        
        //Start new game
        if ( IsKeyPressed(KEY_ENTER) ){
            
            InitGameVar();
            gameOver = false;
            
        }
        
    }

}

void UpdateDrawGame(){
    
    UpdateGame();
    DrawGame();
    
}
