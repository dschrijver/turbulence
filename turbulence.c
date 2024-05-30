#include <stdio.h>
#include <math.h>
#include <raylib.h>

#include "turbulence.h"


double u_max() {
    double max = 0;
    double u_i;
    for (int i = 0; i < N_x; i++) {
        for (int j = 0; j < N_y; j++) {
            u_i = sqrt(dot(2, u[i][j], u[i][j]));
            if (u_i>max) max = u_i;
        }
    }
    return max;
}

double dot(int n, double v[n], double w[n]) {
    double result = 0.0;
    for (int i = 0; i < n; i++) {
        result += v[i]*w[i];
    }
    return result;
}

int modskip(int x, int m) {
    if (x < 0) return x+m-1;
    if (x >= m) return x-m+1;
    return x;
}

void update() {
    // --- Streaming step ---
    for (int i = 0; i < N_x; i++) {
        for (int j = 0; j < N_y; j++) {
            for (int k = 0; k < Q; k++) {
                if (obstacle_grid[i][j]) {
                    f2[i][j][k] = f1[i][j][k];
                } else  {
                    x_i = modskip(i-c[k][0],N_x);
                    y_i = j-c[k][1];
                    k_i = k;
                    if ((y_i < 0) || (y_i == N_y)) {
                        x_i = i;
                        y_i = j;
                        k_i = ((k+3)%8)+1;
                    } else if (obstacle_grid[x_i][y_i]) {
                        x_i = i;
                        y_i = j;
                        k_i = ((k+3)%8)+1;
                    }
                    f2[i][j][k] = f1[x_i][y_i][k_i];
                }
            }
        }
    }
    // --- Calculate macroscopic variables ---
    for (int i = 0; i < N_x; i++) {
        for (int j = 0; j < N_y; j++) {
            rho[i][j] = 0.0;
            u[i][j][0] = 0.0; 
            u[i][j][1] = 0.0;
            if (!obstacle_grid[i][j]) {
                for (int k = 0; k < Q; k++) {
                    rho[i][j] += f2[i][j][k];
                    u[i][j][0] += f2[i][j][k]*cd[k][0];
                    u[i][j][1] += f2[i][j][k]*cd[k][1];
                }
                u[i][j][0] = u[i][j][0]/rho[i][j] + F[i][j][0]/2.0;
                u[i][j][1] = u[i][j][1]/rho[i][j] + F[i][j][1]/2.0;
            }
        }
    }
    // --- Compute the equilibrium distribution ---
    for (int i = 0; i < N_x; i++) {
        for (int j = 0; j < N_y; j++) {
            for (int k = 0; k < Q; k++) {
                uc = dot(2,u[i][j],cd[k]);
                u2 = dot(2,u[i][j],u[i][j]);
                feq[i][j][k] = w[k]*rho[i][j]*(1.0 + uc/cs2 + (uc*uc-cs2*u2)/(2.0*cs2*cs2));
            }
        }
    }
    // --- Calculate the external forcing ---
    for (int i = 0; i < N_x; i++) {
        for (int j = 0; j < N_y; j++) {
            for (int k = 0; k < Q; k++) {
                uc = dot(2,u[i][j],cd[k]);
                fdot = 0.0;
                for (int r = 0; r < 2; r++) {
                    fdot += ((cd[k][r]-u[i][j][r])/cs2 + uc/(cs2*cs2)*cd[k][r])*F[i][j][r];
                }
                ff[i][j][k] = (1.0-1.0/(2.0*tau))*w[k]*fdot;
            }
        }
    }
    // --- Apply the collisional operator ---
    for (int i = 0; i < N_x; i++) {
        for (int j = 0; j < N_y; j++) {
            for (int k = 0; k < Q; k++) {
                f1[i][j][k] = f2[i][j][k] - 1.0/tau*(f2[i][j][k] - feq[i][j][k]) + ff[i][j][k];
            }
        }
    }
}

int main(void) {
    // --- Set-up obstacle ---
    for (int i = 0; i < N_x; i++) {
        for (int j = 0; j < N_y; j++) {
            if ((i >= N_x/2-obstacle_width/2) && (i < N_x/2+obstacle_width/2) && 
                (j >= N_y/2+obstacle_shift-obstacle_height/2) && (j < N_y/2+obstacle_shift+obstacle_height/2)) {
                obstacle_grid[i][j] = 1;
            } else {
                obstacle_grid[i][j] = 0;
            }
        }
    }

    // --- Initialize macroscopic quantities ---
    for (int i = 0; i < N_x; i++) {
        for (int j = 0; j < N_y; j++) {
            if (obstacle_grid[i][j]) {
                rho[i][j] = 0.0;
                F[i][j][0] = 0.0;
                F[i][j][1] = 0.0;
            } else {
                rho[i][j] = 1.0;
                F[i][j][0] = A_F;
                F[i][j][1] = 0.0;
            }
            u[i][j][0] = 0.0;
            u[i][j][1] = 0.0;
        }
    }

    // Initial distribution is the equilibrium distribution
    for (int i = 0; i < N_x; i++) {
        for (int j = 0; j < N_y; j++) {
            for (int k = 0; k < Q; k++) {
                uc = dot(2,u[i][j],cd[k]);
                u2 = dot(2,u[i][j],u[i][j]);
                f1[i][j][k] = w[k]*rho[i][j]*(1.0 + uc/cs2 + 
                                                (uc*uc-cs2*u2)/(2.0*cs2*cs2));
            }
        }
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "");
    double start_time, end_time, frame_time, new_frame, last_frame = 0.0;
    double u_d, max; int u_i;
    char frame_string[32];
    Color cell_color = {0,0,0,255};
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        PollInputEvents(); // Poll input events (SUPPORT_CUSTOM_FRAME_CONTROL)
        start_time = GetTime();
        for (int i = 0; i < steps_per_frame; i++) {
            update();
        }
        total = 0;
        for (int i = 0; i < N_x/4; i++) {
            for (int j = 0; j < N_y; j++) {
                total += sqrt(dot(2, u[i][j], u[i][j]));
            }
        }
        
        Re = obstacle_height*total/(((double)(N_y*N_x/4))*nu);
        max = u_max();
        BeginDrawing();
            ClearBackground(BLACK);
            new_frame = GetTime();

            for (int x = 0; x < N_x; x++) {
                for (int y = 0; y < N_y; y++) {
                    u_d = sqrt(dot(2, u[x][y], u[x][y]));
                    u_i = (int)(u_d/max*255.0);
                    cell_color.r = u_i;
                    cell_color.b = 255-u_i;
                    DrawRectangle(x*size, y*size, size, size, cell_color);
                }
            }
            sprintf(frame_string, "Re = %.0f", Re);
            DrawText(frame_string, 20, 20, 20, BLACK);
            sprintf(frame_string, "FPS = %.2f", 1.0/(new_frame-last_frame));
            DrawText(frame_string, SCREEN_WIDTH - 140, 20, 20, BLACK);
            last_frame = new_frame;
        EndDrawing();
        SwapScreenBuffer(); // Flip the back buffer to screen (front buffer)
        end_time = GetTime();
        frame_time = end_time - start_time;
        if ((dt-frame_time) > 0) WaitTime(dt-frame_time);
    }
    CloseWindow();
    return 0;
}
