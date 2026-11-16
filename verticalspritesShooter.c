#include "objects.h" /* definições das structs */
#include "raylib.h"
#include "resource_dir.h"

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 800;

const int NUM_BULLETS = 8;
const int NUM_COMETS = 10;
const int NUM_MISSILES = 8;

const float SHIP_SCALE = 0.5f;
const float COMET_SCALE = 0.4f;
const float BULLET_SCALE = 0.6f;
const int MISSILE_SCALE = 8;

bool debug = false;

/* protótipos */
void init_ship(SpaceShip *ship, Texture2D tex);
void draw_ship(const SpaceShip *ship, Texture2D tex);
void move_ship(SpaceShip *ship, float dt);

void init_bullets(Bullet b[], int size);
void fire_bullet(Bullet b[], int size, const SpaceShip *ship);
void draw_bullets(const Bullet b[], int size, Texture2D tex);
void update_bullets(Bullet b[], int size, float dt);

void init_comets(Comet c[], int size, Texture2D tex);
void spawn_comet(Comet c[], int size);
void draw_comets(const Comet c[], int size, Texture2D tex);
void update_comets(Comet c[], int size, float dt);

void init_missiles(Missile m[], int size, Texture2D tex);
void spawn_missiles(Missile m[], int size, SpaceShip ship);
void draw_missiles(const Missile m[], int size, Texture2D tex);
void update_missiles(Missile m[], int size, float dt);

void colide_bullets(Bullet b[], int bSize, Comet c[], int cSize,
                    SpaceShip *ship);
void colide_comets(Comet c[], int size, SpaceShip *ship);

void colide_missiles(Missile m[], int size, SpaceShip *ship);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter");
    SetTargetFPS(60);

    SearchAndSetResourceDir("resources");
    Texture2D ship_tex = LoadTexture("hiena.png");
    Texture2D comet_tex = LoadTexture("dogy.png");
    Texture2D bullet_tex = LoadTexture("bullet.png");
    Texture2D missile_tex = LoadTexture("dogyRun.png");

    SpaceShip ship;
    init_ship(&ship, ship_tex);

    Bullet bullets[NUM_BULLETS];
    init_bullets(bullets, NUM_BULLETS);

    Comet comets[NUM_COMETS];
    init_comets(comets, NUM_COMETS, comet_tex);

    Missile missiles[NUM_MISSILES];
    init_missiles(missiles, NUM_MISSILES, missile_tex);

    bool game_over = false;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        ship.damage_anim -= dt;
        if (ship.damage_anim < 0)
            ship.damage_anim = 0.0f;

        if (IsKeyPressed(KEY_D))
            if (debug)
                debug = false;
            else
                debug = true;

        if (!game_over || debug) {
            /* ATUALIZAR */
            move_ship(&ship, dt);
            if (IsKeyPressed(KEY_SPACE)) {
                fire_bullet(bullets, NUM_BULLETS, &ship);
            }
            if (IsKeyDown(KEY_SPACE) & debug == true)
                ship.score++;
            update_bullets(bullets, NUM_BULLETS, dt);
            spawn_comet(comets, NUM_COMETS);
            update_comets(comets, NUM_COMETS, dt);
            spawn_missiles(missiles, NUM_MISSILES, ship);
            update_missiles(missiles, NUM_MISSILES, dt);
            colide_bullets(bullets, NUM_BULLETS, comets, NUM_COMETS, &ship);
            colide_comets(comets, NUM_COMETS, &ship);
            colide_missiles(missiles, NUM_MISSILES, &ship);
            if (ship.lives <= 0)
                game_over = true;
        } else if (IsKeyPressed(KEY_ENTER)) { /* reiniciar */
            init_ship(&ship, ship_tex);
            init_bullets(bullets, NUM_BULLETS);
            init_missiles(missiles, NUM_MISSILES, missile_tex);
            init_comets(comets, NUM_COMETS, comet_tex);
            game_over = false;
        }
        /* DESENHAR */
        if (!game_over || debug) {
            BeginDrawing();
            ClearBackground(BLACK);
            draw_ship(&ship, ship_tex);
            draw_bullets(bullets, NUM_BULLETS, bullet_tex);
            draw_comets(comets, NUM_COMETS, comet_tex);
            draw_missiles(missiles, NUM_MISSILES, missile_tex);
            DrawText(
                TextFormat("Lives: %d   Score: %d", ship.lives, ship.score), 5,
                5, 20, WHITE);

        } else {
            const char *msg = TextFormat("End of game. Score: %d", ship.score);
            int txt_width = MeasureText(msg, 30);
            DrawText(msg, (SCREEN_WIDTH - txt_width) / 2,
                     SCREEN_HEIGHT / 2 - 30, 30, WHITE);
            DrawText("ENTER to restart",
                     (SCREEN_WIDTH - MeasureText("ENTER to restart", 20)) / 2,
                     SCREEN_HEIGHT / 2 + 10, 20, GRAY);
        }
        if (debug) {
            DrawText("DEBUGANDO", 30, 50, 20, YELLOW);
            int MissileR = 0;
            int MissileL = 0;
            for (int i = 0; i < NUM_MISSILES; i++) {
                if (missiles[i].Dir == 1)
                    MissileR++;
                if (missiles[i].Dir == -1)
                    MissileL++;
            }
            DrawText(TextFormat("Missile left: %d, Missile right: %d", MissileL,
                                MissileR),
                     30, 70, 20, WHITE);
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void init_ship(SpaceShip *ship, Texture2D tex) {
    ship->x = 20.0f;
    ship->y = SCREEN_HEIGHT / 2.0f;
    ship->speed = 350.0f;
    ship->lives = 3;
    ship->score = 0;
    ship->limit_x = (int)(tex.height * SHIP_SCALE / 3);
    ship->limit_y = (int)(tex.width * SHIP_SCALE / 3);
    ship->damage_anim = 0.0f;
}

void draw_ship(const SpaceShip *ship, Texture2D tex) {
    Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
    Rectangle dst = {ship->x, ship->y, tex.width * SHIP_SCALE,
                     tex.height * SHIP_SCALE};
    Vector2 origin = {dst.width / 2, dst.height / 2}; /* centro */
    if ((ship->damage_anim > 0.2f && ship->damage_anim < 0.4f) ||
        (ship->damage_anim > 0.8f && ship->damage_anim < 1.0f))
        DrawTexturePro(tex, src, dst, origin, 90.0f, RED);
    else
        DrawTexturePro(tex, src, dst, origin, 90.0f, WHITE);
}

void move_ship(SpaceShip *ship, float dt) {
    if (IsKeyDown(KEY_LEFT_SHIFT))
        ship->speed = ship->speed / 2;
    if (IsKeyDown(KEY_UP))
        ship->y -= ship->speed * dt;
    if (IsKeyDown(KEY_DOWN))
        ship->y += ship->speed * dt;
    if (IsKeyDown(KEY_LEFT))
        ship->x -= ship->speed * dt;
    if (IsKeyDown(KEY_RIGHT))
        ship->x += ship->speed * dt;
    if (IsKeyDown(KEY_LEFT_SHIFT))
        ship->speed = ship->speed * 2;

    /* limites: só pode andar no primeiro terço da tela */
    if (ship->x < 0)
        ship->x = 0;
    if (ship->x > SCREEN_WIDTH)
        ship->x = SCREEN_WIDTH;
    if (ship->y < ((SCREEN_HEIGHT / 4) * 3))
        ship->y = ((SCREEN_HEIGHT / 4) * 3);
    if (ship->y > SCREEN_HEIGHT - 10)
        ship->y = SCREEN_HEIGHT - 10;
}

void init_bullets(Bullet b[], int size) {
    for (int i = 0; i < size; i++) {
        b[i].active = false;
        b[i].speed = 600.0f;
        b[i].size = 10;
    }
}

void fire_bullet(Bullet b[], int size, const SpaceShip *ship) {
    for (int i = 0; i < size; i++) {
        if (!b[i].active) { /* primeiro tiro livre */
            b[i].x = ship->x;
            b[i].y = ship->y - 15;
            b[i].active = true;
            break; /* só um tiro por disparo */
        }
    }
}

void draw_bullets(const Bullet b[], int size, Texture2D tex) {
    Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
    for (int i = 0; i < size; i++) {
        if (b[i].active) {
            Rectangle dst = {b[i].x, b[i].y, tex.width * BULLET_SCALE,
                             tex.height * BULLET_SCALE};
            Vector2 origin = {dst.width / 2, dst.height / 2};
            DrawTexturePro(tex, src, dst, origin, 90.0f, WHITE);
        }
    }
}

void update_bullets(Bullet b[], int size, float dt) {
    for (int i = 0; i < size; i++) {
        if (b[i].active) {
            b[i].y -= b[i].speed * dt;
            if (b[i].y <= 0)
                b[i].active = false; /* saiu da tela */
        }
    }
}

void init_comets(Comet c[], int size, Texture2D tex) {
    for (int i = 0; i < size; i++) {
        c[i].active = false;
        c[i].speed = 300.0f;
        c[i].limit_x = (int)(tex.width * COMET_SCALE / 2);
        c[i].limit_y = (int)(tex.height * COMET_SCALE / 2);
    }
}

void spawn_comet(Comet c[], int size) {
    int limit = 0;
    for (int i = 0; i < size; i++) {
        if (c[i].active && c[i].y < 100) {
            limit++;
            if (limit > 2)
                break;
        }
        if (!c[i].active) {
            if (GetRandomValue(0, 499) == 0) { /* chance 1/500 por frame */
                c[i].active = true;
                c[i].y = 0;
                c[i].x = (float)GetRandomValue(30, SCREEN_WIDTH - 30);
                break;
            }
        }
    }
}

void draw_comets(const Comet c[], int size, Texture2D tex) {
    for (int i = 0; i < size; i++) {
        if (c[i].active) {
            /* DrawTextureEx usa o canto superior esquerdo: descontar os limites
             */
            Vector2 pos = {c[i].x - c[i].limit_x, c[i].y - c[i].limit_y};
            DrawTextureEx(tex, pos, 0.0f, COMET_SCALE, WHITE);
        }
    }
}

void update_comets(Comet c[], int size, float dt) {
    for (int i = 0; i < size; i++) {
        if (c[i].active) {
            c[i].y += c[i].speed * dt;
        }
    }
}

void init_missiles(Missile m[], int size, Texture2D tex) {
    for (int i = 0; i < size; i++) {
        m[i].active = false;
        m[i].speed = -200.0f;
        m[i].limit_x = (int)(tex.width * MISSILE_SCALE / 4);
        m[i].limit_y = (int)(tex.height * MISSILE_SCALE / 4);
        m[i].Dir = 0;
    }
}

void spawn_missiles(Missile m[], int size, SpaceShip s) {
    for (int i = 0; i < size; i++) {
        if (!m[i].active) {
            int chance = 4999 - (25 * s.score);
            if (chance < 100)
                chance = 100;
            if (GetRandomValue(0, chance) == 0) { /* chance 1/5000 por frame */
                m[i].active = true;
                int SCREEN_ZONE = ((SCREEN_HEIGHT / 4) * 3);
                m[i].y = SCREEN_ZONE +
                         GetRandomValue(1, SCREEN_HEIGHT - SCREEN_ZONE - 10);
                m[i].Dir = GetRandomValue(0, 1);
                if (m[i].Dir == 0) {
                    m[i].Dir = -1;
                    m[i].x = -300;
                } else
                    m[i].x = SCREEN_WIDTH + 300;
                break;
            }
        }
    }
}

void draw_missiles(const Missile m[], int size, Texture2D tex) {
    for (int i = 0; i < size; i++)
        if (m[i].active)
            if (m[i].x < -10) {
                DrawRectangle(0, (int)m[i].y - 15, 80, 30, MAROON);
                DrawRectangle(50, (int)m[i].y + 8, 8, 7, RED);
                DrawRectangle(50, (int)m[i].y - 13, 8, 12, RED);

            } else if (m[i].x > SCREEN_WIDTH + 10) {
                DrawRectangle(SCREEN_WIDTH - 80, (int)m[i].y - 15, 80, 30,
                              MAROON);
                DrawRectangle(SCREEN_WIDTH - 60, (int)m[i].y + 8, 8, 7, RED);
                DrawRectangle(SCREEN_WIDTH - 60, (int)m[i].y - 13, 8, 12, RED);
            } else {
                Vector2 pos = {m[i].x - m[i].limit_x, m[i].y - m[i].limit_y};
                DrawTextureEx(tex, pos, 0.0f, MISSILE_SCALE, WHITE);
            }
}

void update_missiles(Missile m[], int size, float dt) {
    for (int i = 0; i < size; i++) {
        if (m[i].active) {
            m[i].x += m[i].speed * dt * m[i].Dir;
        }
    }
}

void colide_bullets(Bullet b[], int bSize, Comet c[], int cSize,
                    SpaceShip *ship) {
    for (int i = 0; i < bSize; i++) {
        if (!b[i].active)
            continue;
        for (int j = 0; j < cSize; j++) {
            if (!c[j].active)
                continue;
            if (b[i].x + b[i].size > c[j].x - c[j].limit_x &&
                b[i].x - b[i].size < c[j].x + c[j].limit_x &&
                b[i].y > c[j].y - c[j].limit_y &&
                b[i].y < c[j].y + c[j].limit_y) {
                b[i].active = false;
                c[j].active = false;
                ship->score++;
            }
        }
    }
}

void colide_comets(Comet c[], int size, SpaceShip *ship) {
    for (int i = 0; i < size; i++) {
        if (!c[i].active)
            continue;
        if (c[i].x - c[i].limit_x < ship->x + ship->limit_x &&
            c[i].x + c[i].limit_x > ship->x - ship->limit_x &&
            c[i].y - c[i].limit_y < ship->y + ship->limit_y &&
            c[i].y + c[i].limit_y > ship->y - ship->limit_y) {
            ship->lives--;
            ship->damage_anim = 1.0f;
            c[i].active = false;
        } else if (c[i].y > SCREEN_HEIGHT) { /* cometa passou pela nave */
            c[i].active = false;
            ship->lives--;
            ship->damage_anim = 1.0f;
        }
    }
}

void colide_missiles(Missile m[], int size, SpaceShip *ship) {
    for (int i = 0; i < size; i++) {
        if (!m[i].active)
            continue;
        if (m[i].x - m[i].limit_x < ship->x + ship->limit_x &&
            m[i].x + m[i].limit_x > ship->x - ship->limit_x &&
            m[i].y - m[i].limit_y < ship->y + ship->limit_y &&
            m[i].y + m[i].limit_y > ship->y - ship->limit_y) {
            ship->lives--;
            ship->damage_anim = 1.0f;
            m[i].active = false;
            m[i].Dir = 0;
        } else if (m[i].x * -m[i].Dir >
                   (SCREEN_WIDTH / 2) +
                       (SCREEN_WIDTH / 2 *
                        -m[i].Dir)) { /* cometa passou pela nave */
            m[i].active = false;
            m[i].Dir = 0;
        }
    }
}
