#ifndef TURBULENCE_H
#define TURBULENCE_H


// --- Animation constants ---
#define SCREEN_WIDTH    (int) 1200  // Divisible by N_x
#define SCREEN_HEIGHT   (int) 400   // Divisible by N_y
#define FPS             (int) 60
#define steps_per_frame (int) 10
#define dt              (float) (1.0/((double)FPS))

// --- Lattice constants --- 
#define N_x     (int) 240   // Grid width
#define N_y     (int) 80    // Grid height
#define size    (int) (SCREEN_WIDTH/N_x)

// --- Physics constants ---
#define tau (double) 0.52              // Relaxation time
#define cs2 (double) (1.0/3.0)        // Speed of sound squared
#define nu  (double) (cs2*(tau-0.5))  // Fluid viscosity

// --- D2Q9 constants ---
#define Q (int) 9
int c[Q][2] = {
    {0,0},
    {1,0},
    {1,-1},
    {0,-1},
    {-1,-1},
    {-1,0},
    {-1,1},
    {0,1},
    {1,1}
};
double cd[Q][2] = {
    {0.0,0.0},
    {1.0,0.0},
    {1.0,-1.0},
    {0.0,-1.0},
    {-1.0,-1.0},
    {-1.0,0.0},
    {-1.0,1.0},
    {0.0,1.0},
    {1.0,1.0}
};
double w[Q] = {4.0/9.0, 1.0/9.0, 1.0/36.0, 1.0/9.0, 1.0/36.0, 1.0/9.0, 1.0/36.0, 1.0/9.0, 1.0/36.0};

// --- Macroscopic quantities ---
double rho[N_x][N_y]; 
double u[N_x][N_y][2];
double F[N_x][N_y][2];
double A_F = 0.00001;
double Re;

// --- Probability distribution functions ---
double f1[N_x][N_y][Q];
double f2[N_x][N_y][Q];
double feq[N_x][N_y][Q];
double ff[N_x][N_y][Q];

// --- Obstacle ---
int obstacle_grid[N_x][N_y];
int obstacle_width = 20;
int obstacle_height = 40;
int obstacle_shift = 1;

// --- Functions ---
double u_max();
double dot(int n, double v[n], double w[n]);
int modskip(int x, int m);

// --- Variables used in loops ---
double uc, u2, fdot, total; // Variables used in loops
int x_i, y_i, k_i;

#endif