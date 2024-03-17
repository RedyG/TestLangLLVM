#pragma once
struct TextPos {
    int LineX;
    int LineY;
    int Pos;

    TextPos(int lineX, int lineY, int pos) : LineX(lineX), LineY(lineY), Pos(pos) {}
    TextPos() : LineX(0), LineY(0), Pos(0) {}
};