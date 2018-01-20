package com.learnopengles.android.lesson6;

interface Action {
    void setMinFilter(int filter);

    void setMagFilter(int filter);

    void setDelta(float deltaX, float deltaY);
}
