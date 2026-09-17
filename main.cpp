#include <windows.h>
#include <mmsystem.h>
#include <GL/glut.h>
#define PI 3.1416
#include <math.h>
#include <cstdlib>

#pragma comment(lib, "winmm.lib")


//variable declare
float cloudX[4] = { -0.80f, -0.30f, 0.20f, 0.72f };
float birdX[4]  = { -0.80f, -0.35f, 0.10f, 0.55f };
float boatX = -0.95f;
float footballT = 0.0f;
float animationTime = 0.0f;
float oarAngle = -25.0f;
bool animationOn = true;

float roadPersonT[2] = { 0.0f, 0.48f };

//primitive shapes

void setColorAlpha(int r, int g, int b, float alpha) {
    glColor4f(r / 255.0f, g / 255.0f, b / 255.0f, alpha);
}

void drawFilledCircle(float cx, float cy, float radius, int segments = 30) {
    int i;
    float tp2 = 2.0f * PI;
    glPushMatrix();
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (i = 0; i <= segments; i++) {
        glVertex2f(cx + radius * cos(i * tp2 / segments), cy + radius * sin(i * tp2 / segments));
    }
    glEnd();
    glPopMatrix();
}

void drawEllipse(float cx, float cy, float rx, float ry, int segments = 30) {
    int i;
    float tp2 = 2.0f * PI;
    glPushMatrix();
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (i = 0; i <= segments; i++) {
        glVertex2f(cx + rx * cos(i * tp2 / segments), cy + ry * sin(i * tp2 / segments));
    }
    glEnd();
    glPopMatrix();
}

void drawLine(float x1, float y1, float x2, float y2, float width = 1.0f) {
    glPushMatrix();
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    glLineWidth(1.0f);
    glPopMatrix();
}

void drawRectangle(float left, float bottom, float right, float top) {
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
    glVertex2f(left, top);
    glEnd();
    glPopMatrix();
}

void drawPolygon(const float points[][2], int count) {
    int i;
    glPushMatrix();
    glBegin(GL_POLYGON);
    for (i = 0; i < count; i++) glVertex2f(points[i][0], points[i][1]);
    glEnd();
    glPopMatrix();
}

// sky function

void drawSky() {
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3ub(51, 51, 107);
    glVertex2f(-1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glColor3ub(140, 77, 107);
    glVertex2f(1.0f, 0.55f);
    glVertex2f(-1.0f, 0.55f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3ub(140, 77, 107);
    glVertex2f(-1.0f, 0.55f);
    glVertex2f(1.0f, 0.55f);
    glColor3ub(235, 115, 71);
    glVertex2f(1.0f, 0.32f);
    glVertex2f(-1.0f, 0.32f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3ub(235, 115, 71);
    glVertex2f(-1.0f, 0.32f);
    glVertex2f(1.0f, 0.32f);
    glColor3ub(250, 153, 46);
    glVertex2f(1.0f, 0.12f);
    glVertex2f(-1.0f, 0.12f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3ub(250, 153, 46);
    glVertex2f(-1.0f, 0.12f);
    glVertex2f(1.0f, 0.12f);
    glColor3ub(255, 209, 77);
    glVertex2f(1.0f, -0.15f);
    glVertex2f(-1.0f, -0.15f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3ub(255, 209, 77);
    glVertex2f(-1.0f, -0.15f);
    glVertex2f(1.0f, -0.15f);
    glColor3ub(245, 140, 41);
    glVertex2f(1.0f, -0.40f);
    glVertex2f(-1.0f, -0.40f);
    glEnd();
    glPopMatrix();
}

// sun function

void drawSun() {
    float sunX = 0.68f, sunY = 0.58f, radius = 0.14f;

    glPushMatrix();
    setColorAlpha(255, 170, 40, 0.14f);
    drawFilledCircle(sunX, sunY, radius + 0.055f, 50);
    glPopMatrix();

    glPushMatrix();
    setColorAlpha(255, 195, 50, 0.22f);
    drawFilledCircle(sunX, sunY, radius + 0.025f, 50);
    glPopMatrix();

    glPushMatrix();
    setColorAlpha(255, 225, 35, 1.0f);
    drawFilledCircle(sunX, sunY, radius, 50);
    glPopMatrix();

    glPushMatrix();
    setColorAlpha(255, 245, 100, 0.35f);
    drawFilledCircle(sunX - 0.025f, sunY + 0.025f, radius * 0.65f, 40);
    glPopMatrix();
}

// cloud function

void drawCloud(float x, float y) {
    float s = 0.0025f;

    glPushMatrix();
    setColorAlpha(225, 235, 242, 0.85f);
    drawFilledCircle(x, y - 2 * s, 24 * s, 40);
    drawFilledCircle(x + 28 * s, y + 7 * s, 30 * s, 40);
    drawFilledCircle(x + 58 * s, y - 2 * s, 23 * s, 40);
    drawFilledCircle(x + 30 * s, y - 10 * s, 22 * s, 40);
    glPopMatrix();

    glPushMatrix();
    setColorAlpha(255, 255, 255, 0.92f);
    drawFilledCircle(x - 4 * s, y + 5 * s, 16 * s, 32);
    drawFilledCircle(x + 23 * s, y + 15 * s, 21 * s, 36);
    drawFilledCircle(x + 52 * s, y + 6 * s, 16 * s, 32);
    glPopMatrix();
}

// bird function

void drawBird(float x, float y, float scale, float flap) {
    glPushMatrix();
    glColor3ub(10, 10, 13);
    drawLine(x, y, x - 0.025f * scale, y + 0.015f * scale + flap, 2.0f);
    drawLine(x, y, x + 0.025f * scale, y + 0.015f * scale - flap, 2.0f);
    glPopMatrix();
}

void drawBirds() {
    float yBase[4] = { 0.65f, 0.78f, 0.58f, 0.70f };
    float size[4]  = { 1.0f, 0.8f, 0.7f, 1.1f };
    int i;

    for (i = 0; i < 4; i++) {
        float y = yBase[i] + 0.035f * sin(birdX[i] * 5.0f + i);
        float flap = 0.010f * sin(animationTime * 5.0f + i);
        drawBird(birdX[i], y, size[i], flap);
    }
}

// hills ellipse function

void drawMountains() {
    glPushMatrix();
    glColor3ub(92, 97, 77);
    drawFilledCircle(-0.72f, 0.13f, 0.24f, 40);
    drawFilledCircle(-0.42f, 0.10f, 0.30f, 40);
    drawFilledCircle(-0.05f, 0.14f, 0.25f, 40);
    drawFilledCircle(0.30f, 0.11f, 0.30f, 40);
    drawFilledCircle(0.67f, 0.14f, 0.25f, 40);
    drawRectangle(-1.0f, -0.02f, 1.0f, 0.08f);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(122, 107, 77);
    drawEllipse(-0.48f, 0.19f, 0.19f, 0.12f, 35);
    drawEllipse(0.26f, 0.19f, 0.20f, 0.12f, 35);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(56, 97, 56);
    drawFilledCircle(-0.78f, 0.02f, 0.26f, 40);
    drawFilledCircle(-0.48f, 0.02f, 0.33f, 40);
    drawFilledCircle(-0.10f, 0.00f, 0.27f, 40);
    drawFilledCircle(0.25f, 0.03f, 0.34f, 40);
    drawFilledCircle(0.67f, 0.02f, 0.29f, 40);
    drawRectangle(-1.0f, -0.08f, 1.0f, 0.00f);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(107, 128, 71);
    drawEllipse(-0.48f, 0.13f, 0.20f, 0.12f, 35);
    drawEllipse(0.24f, 0.15f, 0.21f, 0.13f, 35);
    drawEllipse(0.68f, 0.12f, 0.16f, 0.10f, 35);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(41, 77, 43);
    drawEllipse(-0.78f, -0.01f, 0.24f, 0.09f, 30);
    drawEllipse(0.05f, -0.02f, 0.25f, 0.09f, 30);
    drawEllipse(0.78f, -0.02f, 0.20f, 0.08f, 30);
    glPopMatrix();
}

// ================= GRASS =================

void drawGrass() {
    int i;
    glPushMatrix();
    glColor3ub(89, 110, 38);
    for (i = 0; i < 18; i++) {
        float x = -0.95f + i * 0.065f;
        float y = 0.02f - (i % 5) * 0.045f;
        drawLine(x, y, x + 0.012f, y + 0.035f, 1.0f);
    }
    glPopMatrix();
}

// tree function

void drawTree(float x, float y, float scale) {
    glPushMatrix();
    glColor3ub(41, 26, 18);
    drawRectangle(x - 0.018f * scale, y, x + 0.018f * scale, y + 0.15f * scale);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(11, 38, 31);
    drawFilledCircle(x, y + 0.18f * scale, 0.09f * scale);
    drawFilledCircle(x - 0.065f * scale, y + 0.15f * scale, 0.07f * scale);
    drawFilledCircle(x + 0.065f * scale, y + 0.15f * scale, 0.07f * scale);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(31, 64, 43);
    drawFilledCircle(x, y + 0.22f * scale, 0.06f * scale);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(61, 77, 41);
    drawEllipse(x - 0.025f * scale, y + 0.20f * scale, 0.025f * scale, 0.018f * scale, 20);
    glPopMatrix();
}

// bush function

void drawBush(float x, float y, float scale) {
    glPushMatrix();
    glColor3ub(26, 56, 20);
    drawFilledCircle(x, y, 0.045f * scale);
    drawFilledCircle(x + 0.045f * scale, y + 0.005f, 0.05f * scale);
    drawFilledCircle(x - 0.045f * scale, y + 0.005f, 0.04f * scale);
    glPopMatrix();
}

// green field

void drawGreenField() {
    glPushMatrix();
    glColor3ub(51, 107, 31);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, 0.12f);
    glVertex2f(-0.70f, 0.08f);
    glVertex2f(-0.45f, 0.00f);
    glVertex2f(-0.15f, -0.05f);
    glVertex2f(0.10f, -0.18f);
    glVertex2f(0.20f, -0.40f);
    glVertex2f(-1.0f, -0.40f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(64, 122, 36);
    glBegin(GL_POLYGON);
    glVertex2f(-0.98f, 0.08f);
    glVertex2f(-0.70f, 0.04f);
    glVertex2f(-0.45f, -0.02f);
    glVertex2f(-0.20f, -0.08f);
    glVertex2f(0.02f, -0.20f);
    glVertex2f(0.10f, -0.32f);
    glVertex2f(-0.98f, -0.32f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(41, 87, 20);
    drawLine(-0.95f, -0.08f, -0.25f, -0.12f, 2.0f);
    drawLine(-0.90f, -0.20f, -0.15f, -0.25f, 2.0f);
    drawLine(-0.80f, 0.02f, -0.30f, -0.02f, 2.0f);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(77, 133, 41);
    int i;
    for (i = 0; i < 35; i++) {
        float x = -0.95f + i * 0.027f;
        float y = -0.04f - (i % 8) * 0.035f;
        drawLine(x, y, x + 0.005f, y + 0.015f, 1.0f);
    }
    glPopMatrix();
}

// riverside

void drawRiverBank() {
    glPushMatrix();
    glColor3ub(31, 77, 26);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -0.40f);
    glVertex2f(-0.70f, -0.38f);
    glVertex2f(-0.45f, -0.37f);
    glVertex2f(-0.15f, -0.39f);
    glVertex2f(0.20f, -0.40f);
    glVertex2f(1.0f, -0.40f);
    glVertex2f(1.0f, -0.44f);
    glVertex2f(-1.0f, -0.44f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(77, 115, 36);
    drawLine(-0.95f, -0.405f, -0.65f, -0.395f, 1.5f);
    drawLine(-0.45f, -0.395f, -0.15f, -0.405f, 1.5f);
    drawLine(0.10f, -0.405f, 0.40f, -0.410f, 1.5f);
    drawLine(0.55f, -0.410f, 0.90f, -0.405f, 1.5f);
    glPopMatrix();
}

// riverr

void drawRiver() {
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3ub(9, 41, 79);
    glVertex2f(-1.0f, -0.40f);
    glVertex2f(1.0f, -0.40f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    // panir moddhe reflection ta
    glBegin(GL_QUADS);
    glColor3ub(230, 110, 36);
    glVertex2f(0.48f, -0.40f);
    glVertex2f(0.84f, -0.40f);
    glVertex2f(0.70f, -1.0f);
    glVertex2f(0.57f, -1.0f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(43, 92, 125);//ripple line er moto vashe
    int i;
    for (i = 0; i < 14; i++) {
        float y = -0.46f - i * 0.038f;
        float width = 0.12f + (i % 4) * 0.06f;
        float x = -0.85f + (i % 5) * 0.40f;
        x += 0.025f * sin(animationTime + i);
        drawLine(x, y, x + width, y, 1.2f);
    }
    glPopMatrix();

    glPushMatrix();
    //reflection er ripple line
    glColor3ub(255, 166, 51);
    for (i = 0; i < 10; i++) {
        float y = -0.46f - i * 0.045f;
        float x = 0.58f + (i % 3) * 0.05f;
        float width = 0.08f + (i % 4) * 0.025f;
        x += 0.015f * sin(animationTime * 1.5f + i);
        drawLine(x, y, x + width, y, 2.0f);
    }
    glPopMatrix();
}

// boat,boatman

void drawBoat(float x, float y, float scale, float oarAngle) {
    glPushMatrix();
    //boatman head
    glColor3ub(194, 138, 94);
    drawFilledCircle(x, y + 0.070f * scale, 0.021f * scale);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(242, 242, 235);
    //head ar hair white
    drawEllipse(x, y + 0.084f * scale, 0.022f * scale, 0.011f * scale, 20);
    drawFilledCircle(x - 0.016f * scale, y + 0.078f * scale, 0.009f * scale);
    drawFilledCircle(x + 0.016f * scale, y + 0.078f * scale, 0.009f * scale);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(179, 120, 77);
    //pant ar haat
    drawRectangle(x - 0.022f * scale, y + 0.020f * scale, x + 0.022f * scale, y + 0.052f * scale);
    drawRectangle(x - 0.032f * scale, y + 0.032f * scale, x + 0.032f * scale, y + 0.047f * scale);
    drawLine(x - 0.025f * scale, y + 0.040f * scale, x - 0.045f * scale, y + 0.025f * scale, 4.0f);
    drawLine(x + 0.025f * scale, y + 0.040f * scale, x + 0.045f * scale, y + 0.025f * scale, 4.0f);
    drawFilledCircle(x - 0.045f * scale, y + 0.025f * scale, 0.007f * scale);
    drawFilledCircle(x + 0.045f * scale, y + 0.025f * scale, 0.007f * scale);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(26, 26, 31);
    //nicher body
    drawRectangle(x - 0.023f * scale, y - 0.025f * scale, x + 0.023f * scale, y + 0.020f * scale);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(179, 120, 77);
    //legs
    drawRectangle(x - 0.018f * scale, y - 0.080f * scale, x - 0.005f * scale, y - 0.025f * scale);
    drawRectangle(x + 0.005f * scale, y - 0.080f * scale, x + 0.018f * scale, y - 0.025f * scale);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(115, 71, 41);
    drawLine(x - 0.019f * scale, y - 0.080f * scale, x - 0.002f * scale, y - 0.080f * scale, 3.0f);
    drawLine(x + 0.002f * scale, y - 0.080f * scale, x + 0.019f * scale, y - 0.080f * scale, 3.0f);
    glPopMatrix();
//laathi
    float angle = oarAngle * PI / 180.0f;
    float ox = x + 0.13f * scale * cos(angle);
    float oy = y + 0.13f * scale * sin(angle);

    glPushMatrix();
    glColor3ub(140, 82, 31);
    //laathi
    drawLine(x + 0.035f * scale, y + 0.030f * scale, ox, oy, 2.5f);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(102, 56, 20);
    //boittha
    drawLine(ox, oy, ox + 0.02f * scale, oy - 0.035f * scale, 3.5f);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(82, 38, 13);
    float hull[5][2] = {
        { x - 0.20f * scale, y },
        { x - 0.17f * scale, y - 0.075f * scale },
        { x, y - 0.105f * scale },
        { x + 0.17f * scale, y - 0.075f * scale },
        { x + 0.20f * scale, y }
    };
    drawPolygon(hull, 5);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(122, 64, 20);
    drawLine(x - 0.18f * scale, y, x + 0.18f * scale, y, 4.0f);
    glPopMatrix();
}

// football player

void drawPlayer(float x, float y, float scale, int r, int g, int b, int pose, float legMove) {
    glPushMatrix();
    glColor3ub(209, 158, 110);
    drawFilledCircle(x, y + 0.062f * scale, 0.020f * scale, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(9, 9, 9);
    drawEllipse(x, y + 0.075f * scale, 0.021f * scale, 0.010f * scale, 20);
    drawRectangle(x - 0.020f * scale, y + 0.065f * scale, x - 0.012f * scale, y + 0.075f * scale);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(r, g, b);
    drawRectangle(x - 0.025f * scale, y + 0.005f * scale, x + 0.025f * scale, y + 0.045f * scale);
    if (pose == 2) {
        drawRectangle(x - 0.045f * scale, y + 0.025f * scale, x - 0.022f * scale, y + 0.038f * scale);
        drawRectangle(x + 0.022f * scale, y + 0.025f * scale, x + 0.045f * scale, y + 0.038f * scale);
    } else {
        drawRectangle(x - 0.050f * scale, y + 0.022f * scale, x - 0.022f * scale, y + 0.036f * scale);
        drawRectangle(x + 0.022f * scale, y + 0.022f * scale, x + 0.050f * scale, y + 0.036f * scale);
    }
    glPopMatrix();

    glPushMatrix();
    glColor3ub(209, 158, 110);
    drawFilledCircle(x - 0.052f * scale, y + 0.028f * scale, 0.007f * scale, 12);
    drawFilledCircle(x + 0.052f * scale, y + 0.028f * scale, 0.007f * scale, 12);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(14, 15, 19);
    drawRectangle(x - 0.025f * scale, y - 0.035f * scale, x + 0.025f * scale, y + 0.005f * scale);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(11, 11, 14);
    if (pose == 0) {
        drawRectangle(x - 0.022f * scale + legMove, y - 0.090f * scale, x - 0.006f * scale + legMove, y - 0.035f * scale);
        drawRectangle(x + 0.006f * scale - legMove, y - 0.090f * scale, x + 0.022f * scale - legMove, y - 0.035f * scale);
    } else if (pose == 1) {
        drawRectangle(x - 0.020f * scale, y - 0.090f * scale, x - 0.005f * scale, y - 0.035f * scale);
        drawLine(x + 0.010f * scale, y - 0.035f * scale, x + 0.050f * scale, y - 0.065f * scale, 5.0f);
        drawLine(x + 0.045f * scale, y - 0.065f * scale, x + 0.065f * scale, y - 0.065f * scale, 4.0f);
    } else {
        drawRectangle(x - 0.021f * scale, y - 0.090f * scale, x - 0.005f * scale, y - 0.035f * scale);
        drawRectangle(x + 0.005f * scale, y - 0.090f * scale, x + 0.021f * scale, y - 0.035f * scale);
    }
    glPopMatrix();

    glPushMatrix();
    glColor3ub(5, 5, 5);
    drawRectangle(x - 0.025f * scale, y - 0.095f * scale, x - 0.002f * scale, y - 0.088f * scale);
    drawRectangle(x + 0.002f * scale, y - 0.095f * scale, x + 0.025f * scale, y - 0.088f * scale);
    glPopMatrix();
}

void drawFootball(float x, float y, float radius) {
    glPushMatrix();
    glColor3ub(255, 255, 255);
    drawFilledCircle(x, y, radius, 24);
    glPopMatrix();
}

// football match

void drawFootballMatch() {
    float px[7] = { -0.83f, -0.63f, -0.42f, -0.20f, -0.02f, -0.70f, -0.30f };
    float py[7] = { -0.10f, -0.02f, -0.08f, -0.01f, -0.12f, -0.22f, -0.22f };
    int colors[7][3] = {
        { 217, 26, 26 }, { 26, 89, 217 }, { 242, 179, 20 }, { 26, 166, 89 },
        { 204, 51, 153 }, { 242, 115, 26 }, { 51, 179, 191 }
    };
    int i;

    for (i = 0; i < 7; i++) {
        int pose = i % 3;
        float legMove = 0.008f * sin(animationTime * 3.0f + i);
        drawPlayer(px[i], py[i], 1.0f, colors[i][0], colors[i][1], colors[i][2], pose, legMove);
    }

    float t = footballT, bx, by;
    if (t < 0.5f) {
        float p = t / 0.5f;
        bx = -0.68f + p * 0.42f;
        by = -0.16f + 0.10f * sin(p * PI);
    } else {
        float p = (t - 0.5f) / 0.5f;
        bx = -0.26f + p * 0.34f;
        by = -0.16f + 0.08f * sin(p * PI);
    }

    drawFootball(bx, by, 0.018f);
}

// curve road

void drawRoad() {
    const float centerLine[9][2] = {
        { 0.52f, 0.24f }, { 0.55f, 0.16f }, { 0.51f, 0.08f }, { 0.45f, 0.00f },
        { 0.48f, -0.08f }, { 0.58f, -0.16f }, { 0.69f, -0.22f }, { 0.80f, -0.28f }, { 0.88f, -0.33f }
    };
    const int numPts = 9;
    const float roadHalfWidth = 0.045f;
    float leftEdge[9][2], rightEdge[9][2];
    int i;

    for (i = 0; i < numPts; i++) {
        float dx, dy;
        if (i == 0) { dx = centerLine[1][0] - centerLine[0][0]; dy = centerLine[1][1] - centerLine[0][1]; }
        else if (i == numPts - 1) { dx = centerLine[i][0] - centerLine[i - 1][0]; dy = centerLine[i][1] - centerLine[i - 1][1]; }
        else { dx = centerLine[i + 1][0] - centerLine[i - 1][0]; dy = centerLine[i + 1][1] - centerLine[i - 1][1]; }

        float len = sqrt(dx * dx + dy * dy);
        if (len < 0.0001f) len = 0.0001f;
        float nx = -dy / len, ny = dx / len;

        leftEdge[i][0] = centerLine[i][0] + nx * roadHalfWidth;
        leftEdge[i][1] = centerLine[i][1] + ny * roadHalfWidth;
        rightEdge[i][0] = centerLine[i][0] - nx * roadHalfWidth;
        rightEdge[i][1] = centerLine[i][1] - ny * roadHalfWidth;
    }

    glPushMatrix();
    glColor3ub(77, 69, 61);
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i < numPts; i++) {
        glVertex2f(leftEdge[i][0], leftEdge[i][1]);
        glVertex2f(rightEdge[i][0], rightEdge[i][1]);
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(140, 115, 77);
    for (i = 0; i < numPts - 1; i++) {
        drawLine(leftEdge[i][0], leftEdge[i][1], leftEdge[i + 1][0], leftEdge[i + 1][1], 2.0f);
        drawLine(rightEdge[i][0], rightEdge[i][1], rightEdge[i + 1][0], rightEdge[i + 1][1], 2.0f);
    }
    glPopMatrix();

    glPushMatrix();
    glColor3ub(230, 191, 102);
    for (i = 0; i < numPts - 1; i++) {
        float mx1 = centerLine[i][0] + (centerLine[i + 1][0] - centerLine[i][0]) * 0.35f;
        float my1 = centerLine[i][1] + (centerLine[i + 1][1] - centerLine[i][1]) * 0.35f;
        float mx2 = centerLine[i][0] + (centerLine[i + 1][0] - centerLine[i][0]) * 0.65f;
        float my2 = centerLine[i][1] + (centerLine[i + 1][1] - centerLine[i][1]) * 0.65f;
        drawLine(mx1, my1, mx2, my2, 2.0f);
    }
    glPopMatrix();
}

// house

void drawHouse(float x, float y, float scale) {
    glPushMatrix();
    glColor3ub(184, 125, 77);
    drawRectangle(x - 0.10f * scale, y, x + 0.10f * scale, y + 0.12f * scale);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(51, 23, 26);
    float roof[3][2] = { { x - 0.125f * scale, y + 0.12f * scale }, { x, y + 0.22f * scale }, { x + 0.125f * scale, y + 0.12f * scale } };
    drawPolygon(roof, 3);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(56, 31, 20);
    drawRectangle(x - 0.025f * scale, y, x + 0.025f * scale, y + 0.075f * scale);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(255, 199, 87);
    drawRectangle(x - 0.075f * scale, y + 0.045f * scale, x - 0.035f * scale, y + 0.085f * scale);
    drawRectangle(x + 0.035f * scale, y + 0.045f * scale, x + 0.075f * scale, y + 0.085f * scale);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(77, 46, 31);
    drawLine(x - 0.055f * scale, y + 0.045f * scale, x - 0.055f * scale, y + 0.085f * scale, 1.0f);
    drawLine(x + 0.055f * scale, y + 0.045f * scale, x + 0.055f * scale, y + 0.085f * scale, 1.0f);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(89, 46, 36);
    drawRectangle(x + 0.055f * scale, y + 0.16f * scale, x + 0.078f * scale, y + 0.23f * scale);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(255, 219, 115);
    drawFilledCircle(x, y + 0.083f * scale, 0.008f * scale, 16);
    glPopMatrix();
}

// walks people on the road

void getRoadPoint(float t, float &x, float &y) {
    const float path[9][2] = {
        { 0.52f, 0.24f }, { 0.55f, 0.16f }, { 0.51f, 0.08f }, { 0.45f, 0.00f },
        { 0.48f, -0.08f }, { 0.58f, -0.16f }, { 0.69f, -0.22f }, { 0.80f, -0.28f }, { 0.88f, -0.33f }
    };
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    float scaled = t * 8.0f;
    int i = (int)scaled;
    if (i >= 8) { i = 7; scaled = 8.0f; }
    float local = scaled - i;

    x = path[i][0] + (path[i + 1][0] - path[i][0]) * local;
    y = path[i][1] + (path[i + 1][1] - path[i][1]) * local;
}

void drawRoadPerson(float x, float y, float scale, int r, int g, int b, float legMove) {
    glPushMatrix();
    glColor3ub(199, 138, 92);
    drawFilledCircle(x, y + 0.045f * scale, 0.014f * scale, 16);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(9, 8, 6);
    drawEllipse(x, y + 0.055f * scale, 0.015f * scale, 0.007f * scale, 16);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(r, g, b);
    drawRectangle(x - 0.018f * scale, y, x + 0.018f * scale, y + 0.030f * scale);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(199, 138, 92);
    drawLine(x - 0.014f * scale, y + 0.022f * scale, x - 0.030f * scale, y + 0.008f * scale, 2.5f);
    drawLine(x + 0.014f * scale, y + 0.022f * scale, x + 0.030f * scale, y + 0.008f * scale, 2.5f);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(18, 18, 20);
    drawLine(x - 0.008f * scale, y, x - 0.018f * scale + legMove, y - 0.040f * scale, 3.0f);
    drawLine(x + 0.008f * scale, y, x + 0.018f * scale - legMove, y - 0.040f * scale, 3.0f);
    drawLine(x - 0.018f * scale + legMove, y - 0.040f * scale, x - 0.030f * scale + legMove, y - 0.040f * scale, 2.5f);
    drawLine(x + 0.018f * scale - legMove, y - 0.040f * scale, x + 0.030f * scale - legMove, y - 0.040f * scale, 2.5f);
    glPopMatrix();
}

void drawRoadPeople() {
    float x1, y1, x2, y2;
    getRoadPoint(roadPersonT[0], x1, y1);
    getRoadPoint(roadPersonT[1], x2, y2);
    float legMove = 0.008f * sin(animationTime * 4.0f);

    drawRoadPerson(x1, y1, 0.72f, 64, 97, 140, legMove);
    drawRoadPerson(x2, y2, 0.62f, 140, 61, 46, -legMove);
}

// animation

void updateAnimation(int value) {
    int i;
    if (animationOn) {
        cloudX[0] -= 0.0018f; cloudX[1] -= 0.0025f; cloudX[2] -= 0.0014f; cloudX[3] -= 0.0021f;
        for (i = 0; i < 4; i++) if (cloudX[i] < -1.25f) cloudX[i] = 1.15f;

        for (i = 0; i < 4; i++) {
            birdX[i] += 0.0030f + i * 0.0004f;
            if (birdX[i] > 1.15f) birdX[i] = -1.15f;
        }

        boatX += 0.0022f;
        if (boatX > 1.20f) boatX = -1.20f;

        animationTime += 0.08f;

        footballT += 0.006f;
        if (footballT > 1.0f) footballT = 0.0f;

        roadPersonT[0] += 0.0018f;
        roadPersonT[1] += 0.0015f;
        if (roadPersonT[0] > 1.0f) roadPersonT[0] = 0.0f;
        if (roadPersonT[1] > 1.0f) roadPersonT[1] = 0.0f;

        oarAngle = -25.0f + 25.0f * sin(animationTime * 1.5f);
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateAnimation, 0);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    drawSky();
    glPopMatrix();

    glPushMatrix();
    drawSun();
    glPopMatrix();

    glPushMatrix();
    drawMountains();
    glPopMatrix();

    glPushMatrix();
    drawCloud(cloudX[0], 0.76f);
    drawCloud(cloudX[1], 0.86f);
    drawCloud(cloudX[2], 0.68f);
    drawCloud(cloudX[3], 0.82f);
    glPopMatrix();

    glPushMatrix();
    drawBirds();
    glPopMatrix();

    glPushMatrix();
    drawRiverBank();
    glPopMatrix();

    glPushMatrix();
    drawGreenField();
    glPopMatrix();

    glPushMatrix();
    drawRoad();
    glPopMatrix();

    glPushMatrix();
    drawHouse(0.72f, -0.12f, 0.80f);
    drawHouse(0.90f, -0.17f, 0.62f);
    glPopMatrix();

    glPushMatrix();
    drawRoadPeople();
    glPopMatrix();

    glPushMatrix();
    drawTree(-0.92f, 0.02f, 1.25f);
    drawTree(-0.52f, -0.02f, 0.85f);
    drawTree(-0.72f, 0.04f, 0.70f);
    drawTree(-0.25f, -0.02f, 0.65f);
    drawTree(0.12f, -0.04f, 0.72f);
    glPopMatrix();

    glPushMatrix();
    drawBush(-0.35f, -0.12f, 1.0f);
    drawBush(-0.10f, -0.20f, 0.8f);
    drawBush(0.05f, -0.28f, 0.7f);
    glPopMatrix();

    glPushMatrix();
    drawGrass();
    glPopMatrix();

    glPushMatrix();
    drawFootballMatch();
    glPopMatrix();

    glPushMatrix();
    drawRiver();
    glPopMatrix();

    glPushMatrix();
    float boatY = -0.57f + 0.018f * sin(boatX * 6.0f);
    drawBoat(boatX, boatY, 1.0f, oarAngle);
    glPopMatrix();

    glutSwapBuffers();
}

// reshape projection

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



void keyboard(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'Q' || key == 27) exit(0);

    if (key == ' ') animationOn = !animationOn;

    if (key == 'r' || key == 'R') {
        boatX = -0.95f;
        footballT = 0.0f;
        cloudX[0] = -0.80f; cloudX[1] = -0.30f; cloudX[2] = 0.20f; cloudX[3] = 0.72f;
        birdX[0] = -0.80f; birdX[1] = -0.35f; birdX[2] = 0.10f; birdX[3] = 0.55f;
        roadPersonT[0] = 0.0f;
        roadPersonT[1] = 0.48f;
    }

    glutPostRedisplay();
}

// blending alpha function

void setup() {



    glClearColor(0.05f, 0.08f, 0.18f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);


}



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1100, 700);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Sunset Riverside Scene");
    setup();
    glutDisplayFunc(display);
    PlaySound(TEXT("backgroundmusic.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, updateAnimation, 0);
    glutMainLoop();
    return 0;
}

