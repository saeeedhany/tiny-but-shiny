#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BRUSH_SIZE 5    
#define MAX_COLORS 5   

typedef struct {
    int r, g, b, a;     
} Color;

int main() {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Improved Paint Program",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if(window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(renderer == NULL) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *canvas = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        WINDOW_WIDTH, WINDOW_HEIGHT
    );

    if(canvas == NULL) {
        printf("Error creating canvas texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Color colors[MAX_COLORS] = {
        {255, 0, 0, 255},     // Red
        {0, 255, 0, 255},     // Green
        {0, 0, 255, 255},     // Blue
        {255, 255, 0, 255},   // Yellow
        {0, 0, 0, 255}        // Black
    };

    int currentColorIndex = 0;
    Color currentColor = colors[currentColorIndex];

    int brushSize = BRUSH_SIZE;
    bool eraseMode = false;
    
    SDL_SetRenderTarget(renderer, canvas);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    int prevX = -1;
    int prevY = -1;
    
    bool running = true;
    while(running) {
        SDL_Event event;
        
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
            }
            
            if(event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_c:
                        SDL_SetRenderTarget(renderer, canvas);
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderClear(renderer);
                        SDL_SetRenderTarget(renderer, NULL);
                        break;
                    case SDLK_e:
                        eraseMode = !eraseMode;
                        break;
                    case SDLK_PLUS:
                    case SDLK_EQUALS:
                        brushSize = (brushSize < 50) ? brushSize + 1 : brushSize;
                        break;
                    case SDLK_MINUS:
                        brushSize = (brushSize > 1) ? brushSize - 1 : brushSize;
                        break;
                    case SDLK_RIGHT:
                        currentColorIndex = (currentColorIndex + 1) % MAX_COLORS;
                        currentColor = colors[currentColorIndex];
                        eraseMode = false;
                        break;
                    case SDLK_LEFT:
                        currentColorIndex = (currentColorIndex + MAX_COLORS - 1) % MAX_COLORS;
                        currentColor = colors[currentColorIndex];
                        eraseMode = false;
                        break;
                }
            }

            if(event.type == SDL_MOUSEBUTTONDOWN) {
                if(event.button.button == SDL_BUTTON_LEFT) {
                    prevX = event.button.x;
                    prevY = event.button.y;
                    
                    SDL_SetRenderTarget(renderer, canvas);
                    if(eraseMode) {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    } else {
                        SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
                    }
                    
                    for(int dy = -brushSize/2; dy <= brushSize/2; dy++) {
                        for(int dx = -brushSize/2; dx <= brushSize/2; dx++) {
                            if(dx*dx + dy*dy <= (brushSize/2)*(brushSize/2)) {
                                SDL_RenderDrawPoint(renderer, prevX + dx, prevY + dy);
                            }
                        }
                    }
                    SDL_SetRenderTarget(renderer, NULL);
                }
            }
            
            if(event.type == SDL_MOUSEBUTTONUP) {
                if(event.button.button == SDL_BUTTON_LEFT) {
                    prevX = -1;
                    prevY = -1;
                }
            }

            if(event.type == SDL_MOUSEMOTION) {
                if(event.motion.state & SDL_BUTTON_LMASK) {
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;
                    
                    SDL_SetRenderTarget(renderer, canvas);
                    if(eraseMode) {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    } else {
                        SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
                    }
                    
                    if(prevX != -1 && prevY != -1) {
                        float dist = SDL_sqrt((mouseX - prevX) * (mouseX - prevX) + 
                                             (mouseY - prevY) * (mouseY - prevY));
                        if(dist < 1) dist = 1;
                        
                        float dx = (mouseX - prevX) / dist;
                        float dy = (mouseY - prevY) / dist;
                        
                        for(float i = 0; i < dist; i += 0.5f) {
                            int x = prevX + (int)(dx * i);
                            int y = prevY + (int)(dy * i);
                            
                            for(int cy = -brushSize/2; cy <= brushSize/2; cy++) {
                                for(int cx = -brushSize/2; cx <= brushSize/2; cx++) {
                                    if(cx*cx + cy*cy <= (brushSize/2)*(brushSize/2)) {
                                        SDL_RenderDrawPoint(renderer, x + cx, y + cy);
                                    }
                                }
                            }
                        }
                    }
                    
                    prevX = mouseX;
                    prevY = mouseY;
                    SDL_SetRenderTarget(renderer, NULL);
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); 
        SDL_RenderClear(renderer);
        
        SDL_RenderCopy(renderer, canvas, NULL, NULL);
        
        for(int i = 0; i < MAX_COLORS; i++) {
            SDL_Rect colorRect = {10 + i * 30, 10, 25, 25};
            SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
            SDL_RenderFillRect(renderer, &colorRect);
            
            if(i == currentColorIndex && !eraseMode) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect selectionRect = {8 + i * 30, 8, 29, 29};
                SDL_RenderDrawRect(renderer, &selectionRect);
            }
        }
        
        SDL_Rect eraserRect = {10 + MAX_COLORS * 30, 10, 25, 25};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &eraserRect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &eraserRect);
        if(eraseMode) {
            SDL_Rect selectionRect = {8 + MAX_COLORS * 30, 8, 29, 29};
            SDL_RenderDrawRect(renderer, &selectionRect);
        }
        
        SDL_Rect brushRect = {10, 45, brushSize, brushSize};
        if(eraseMode) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
        }
        SDL_RenderFillRect(renderer, &brushRect);
        
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(canvas);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
