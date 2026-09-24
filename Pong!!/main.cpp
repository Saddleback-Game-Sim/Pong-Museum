#include <raylib.h>
#include <raymedia.h>

#include <algorithm>
#include <cmath>
#include <string>

static constexpr int window_width = 960;
static constexpr int window_height = 540;

static constexpr float paddle_width = 20.0f;
static constexpr float paddle_height = 100.0f;
static constexpr float paddle_speed = 420.0f;
static constexpr float ball_radius = 10.0f;
static constexpr float ball_speed = 360.0f;

static bool left_paddle_flying_out = false;
static bool right_paddle_flying_out = false;

static bool start_video=false;
static bool video_countdown_started = false;
static bool forget_about_ball=false;

static float video_countdown_remaining = 2.0f;

static Vector2 left_paddle_exit_velocity{};
static Vector2 right_paddle_exit_velocity{};

bool is_paddle_outside_window(const Rectangle& paddle)
{
    return paddle.x + paddle.width < 0.0f || paddle.x > window_width ||
           paddle.y + paddle.height < 0.0f || paddle.y > window_height;
}

bool update_video_countdown(const Rectangle& left_paddle, const Rectangle& right_paddle,
                            float delta_time)
{
    const bool paddles_have_flown_out = left_paddle_flying_out && right_paddle_flying_out &&
                                        is_paddle_outside_window(left_paddle) &&
                                        is_paddle_outside_window(right_paddle);
    if (!paddles_have_flown_out)
        return false;

    video_countdown_started = true;
    forget_about_ball=true;

    video_countdown_remaining -= delta_time;
    return video_countdown_remaining <= 0.0f;
}


void reset_ball(Vector2& ball_position, Vector2& ball_velocity, float direction)
{
    ball_position = Vector2{window_width / 2.0f, window_height / 2.0f};

    ball_velocity = Vector2{ball_speed * direction, ball_speed * 0.35f};
}

void move_paddles(Rectangle& left_paddle, Rectangle& right_paddle, float delta_time)
{
    if (!left_paddle_flying_out && IsKeyDown(KEY_W))
        left_paddle.y -= paddle_speed * delta_time;
    if (!left_paddle_flying_out && IsKeyDown(KEY_S))
        left_paddle.y += paddle_speed * delta_time;

    if (!right_paddle_flying_out && IsKeyDown(KEY_UP))
        right_paddle.y -= paddle_speed * delta_time;
    if (!right_paddle_flying_out && IsKeyDown(KEY_DOWN))
        right_paddle.y += paddle_speed * delta_time;
}

void keep_paddle_in_window(Rectangle& paddle)
{
    if (paddle.y < 0.0f)
        paddle.y = 0.0f;

    if (paddle.y > window_height - paddle.height)
        paddle.y = window_height - paddle.height;
}

void move(Vector2& ball_position, const Vector2& ball_velocity, 
            Rectangle& left_paddle, Rectangle& right_paddle,float delta_time)
{
    ball_position.x += ball_velocity.x * delta_time;
    ball_position.y += ball_velocity.y * delta_time;

    if (left_paddle_flying_out)
    {
        left_paddle.x += left_paddle_exit_velocity.x * delta_time;
        left_paddle.y += left_paddle_exit_velocity.y * delta_time;
    }

    if (right_paddle_flying_out)
    {
        right_paddle.x += right_paddle_exit_velocity.x * delta_time;
        right_paddle.y += right_paddle_exit_velocity.y * delta_time;
    }
}

void bounce_ball_on_walls(Vector2& ball_position, Vector2& ball_velocity)
{
    if(forget_about_ball)
        return;

    if (ball_position.x - ball_radius <= 0.0f)
    {
        ball_position.x = ball_radius;
        ball_velocity.x = std::abs(ball_velocity.x);
    }
    else if (ball_position.x + ball_radius >= window_width)
    {
        ball_position.x = window_width - ball_radius;
        ball_velocity.x = -std::abs(ball_velocity.x);
    }

    if (ball_position.y - ball_radius <= 0.0f)
    {
        ball_position.y = ball_radius;
        ball_velocity.y = std::abs(ball_velocity.y);
    }
    else if (ball_position.y + ball_radius >= window_height)
    {
        ball_position.y = window_height - ball_radius;
        ball_velocity.y = -std::abs(ball_velocity.y);
    }
}

void bounce_ball_on_paddles(Vector2& ball_position, Vector2& ball_velocity,
                            const Rectangle& left_paddle, const Rectangle& right_paddle)
{
    if (!left_paddle_flying_out && CheckCollisionCircleRec(ball_position, ball_radius, left_paddle) &&
        ball_velocity.x < 0.0f)
    {
        left_paddle_flying_out = true;
        left_paddle_exit_velocity = ball_velocity;

        ball_position.x = left_paddle.x + left_paddle.width + ball_radius;
        ball_velocity.x *= -1.0f;
    }

    if (!right_paddle_flying_out && CheckCollisionCircleRec(ball_position, ball_radius, right_paddle) &&
        ball_velocity.x > 0.0f)
    {
        right_paddle_flying_out = true;
        right_paddle_exit_velocity = ball_velocity;

        ball_position.x = right_paddle.x - ball_radius;
        ball_velocity.x *= -1.0f;
    }
}


void draw_game(const Rectangle& left_paddle, const Rectangle& right_paddle,
               const Vector2& ball_position)
{
    DrawRectangleRec(left_paddle, RAYWHITE);
    DrawRectangleRec(right_paddle, RAYWHITE);
    DrawCircleV(ball_position, ball_radius, RAYWHITE);
}


int main()
{
    InitWindow(window_width, window_height, "Simple Pong");
    SetTargetFPS(60);
    InitAudioDevice();

    const bool audio_ready = IsAudioDeviceReady();
    if (!audio_ready)
        TraceLog(LOG_WARNING, "PONG: Audio device unavailable; playing silent video.");

    const std::string video_path = std::string(GetApplicationDirectory()) + "res/Bad_Apple.mp4";

    const int media_flags = MEDIA_FLAG_LOOP | MEDIA_FLAG_NO_AUTOPLAY |
                            (audio_ready ? MEDIA_LOAD_AV : MEDIA_LOAD_NO_AUDIO);

    MediaStream background = LoadMediaEx(video_path.c_str(), media_flags);

    if (!IsMediaValid(background) || !GetMediaProperties(background).hasVideo)
    {
        TraceLog(LOG_WARNING, "PONG: Background video unavailable: %s", video_path.c_str());
        UnloadMedia(&background);
    }

    Rectangle left_paddle{40.0f, window_height / 2.0f - paddle_height / 2.0f, paddle_width,
                          paddle_height};

    Rectangle right_paddle{window_width - 40.0f - paddle_width,
                           window_height / 2.0f - paddle_height / 2.0f, paddle_width,
                           paddle_height};

    Vector2 ball_position{};
    Vector2 ball_velocity{};

    reset_ball(ball_position, ball_velocity, 1.0f);


    while (!WindowShouldClose())
    {
        const float delta_time = GetFrameTime();

        if (!start_video && update_video_countdown(left_paddle, right_paddle, delta_time))
        {
            start_video = true;
            if (IsMediaValid(background))
                SetMediaState(background, MEDIA_STATE_PLAYING);
        }

        // update media
        if (start_video && IsMediaValid(background) && !UpdateMedia(&background))
        {
            TraceLog(LOG_WARNING, "PONG: Background decoding failed; continuing with a black background.");
            UnloadMedia(&background);
        }

        // input
        move_paddles(left_paddle, right_paddle, delta_time);

        // update
        if (!left_paddle_flying_out)
            keep_paddle_in_window(left_paddle);
        if (!right_paddle_flying_out)
            keep_paddle_in_window(right_paddle);
        move(ball_position, ball_velocity,left_paddle,right_paddle, delta_time);
        bounce_ball_on_walls(ball_position, ball_velocity);
        bounce_ball_on_paddles(ball_position, ball_velocity, left_paddle, right_paddle);

        // render
        BeginDrawing();
        ClearBackground(BLACK);

        if (start_video && IsMediaValid(background) && IsTextureValid(background.videoTexture))
        {
            const Texture2D texture = background.videoTexture;
            const float scale = std::min(static_cast<float>(window_width) / texture.width,
                                         static_cast<float>(window_height) / texture.height);
            const Rectangle source{0.0f, 0.0f, static_cast<float>(texture.width),
                                   static_cast<float>(texture.height)};
            const Rectangle destination{(window_width - texture.width * scale) / 2.0f,
                                        (window_height - texture.height * scale) / 2.0f,
                                        texture.width * scale, texture.height * scale};
            DrawTexturePro(texture, source, destination, Vector2{}, 0.0f, WHITE);
        }

        draw_game(left_paddle, right_paddle, ball_position);

        EndDrawing();
    }

    UnloadMedia(&background);

    if (audio_ready)
        CloseAudioDevice();

    CloseWindow();

    return 0;
}
