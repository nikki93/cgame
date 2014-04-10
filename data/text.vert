#version 150

in vec2 pos;
in vec2 cell;
in float is_cursor;

out vec2 pos_;
out vec2 cell_;
out float is_cursor_;

void main()
{
    pos_ = pos;
    cell_ = cell;
    is_cursor_ = is_cursor;
}
