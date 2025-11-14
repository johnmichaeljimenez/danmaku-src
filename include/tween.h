#pragma once
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define MAX_TWEENS 4096
#define TWEEN_ID_MAX 32

typedef enum
{
    TWEEN_FLOAT,
    TWEEN_VECTOR2,
    TWEEN_VECTOR3
} TweenType;

typedef enum
{
    EASING_LINEAR,
    EASING_EASEINQUAD,
    EASING_EASEOUTQUAD,
    EASING_EASEINOUTQUAD,
    EASING_PINGPONG,
    EASING_SHAKE
} EasingType;

typedef void (*TweenCallback)(const char *id);

typedef struct Tween
{
    bool active;
    char id[TWEEN_ID_MAX];
    void *target;
    TweenType type;
    float duration;
    float elapsed;
    EasingType easing;
    TweenCallback onComplete;
    union
    {
        struct
        {
            float startF, endF;
        };
        struct
        {
            Vector2 startV2, endV2;
        };
        struct
        {
            Vector3 startV3, endV3;
        };
    };
} Tween;

typedef struct
{
    Tween tweens[MAX_TWEENS];
} TweenManager;

extern TweenManager gTweenManager;

static float Tween_Ease(float t, EasingType type)
{
    switch (type)
    {
    case EASING_LINEAR:
        return t;
    case EASING_EASEINQUAD:
        return t * t;
    case EASING_EASEOUTQUAD:
        return t * (2 - t);
    case EASING_EASEINOUTQUAD:
        return (t < 0.5f) ? 2 * t * t : -1 + (4 - 2 * t) * t;
    case EASING_PINGPONG:
        return (t < 0.5f) ? (t * 2.0f) : (1.0f - (t - 0.5f) * 2.0f);
    case EASING_SHAKE:
    {
        float linear = t;

        const float frequency = 12.0f;
        const float amplitude = 0.07f;
        
        float shake = sinf(frequency * PI * t) * amplitude * (1.0f - t);

        return linear + shake;
    }
    default:
        return t;
    }
}

static void TweenManager_Init(void)
{
    for (int i = 0; i < MAX_TWEENS; i++)
        gTweenManager.tweens[i].active = false;
}

static void TweenManager_Clear(void)
{
    for (int i = 0; i < MAX_TWEENS; i++)
        gTweenManager.tweens[i].active = false;
}

static void TweenManager_ClearByID(const char *id)
{
    if (!id || !*id)
        return;
    for (int i = 0; i < MAX_TWEENS; i++)
    {
        Tween *t = &gTweenManager.tweens[i];
        if (t->active && strncmp(t->id, id, TWEEN_ID_MAX) == 0)
            t->active = false;
    }
}

static void TweenManager_ClearByPrefix(const char *prefix)
{
    if (!prefix || !*prefix)
        return;

    size_t len = strnlen(prefix, TWEEN_ID_MAX);
    for (int i = 0; i < MAX_TWEENS; i++)
    {
        Tween *t = &gTweenManager.tweens[i];
        if (t->active && strncmp(t->id, prefix, len) == 0)
            t->active = false;
    }
}

static void TweenManager_Update(float dt)
{
    for (int i = 0; i < MAX_TWEENS; i++)
    {
        Tween *t = &gTweenManager.tweens[i];
        if (!t->active)
            continue;

        t->elapsed += dt;
        float progress = t->elapsed / t->duration;
        if (progress > 1.0f)
            progress = 1.0f;

        float e = Tween_Ease(progress, t->easing);

        switch (t->type)
        {
        case TWEEN_FLOAT:
        {
            float val = t->startF + (t->endF - t->startF) * e;
            *(float *)t->target = val;
        }
        break;
        case TWEEN_VECTOR2:
        {
            Vector2 val = {
                t->startV2.x + (t->endV2.x - t->startV2.x) * e,
                t->startV2.y + (t->endV2.y - t->startV2.y) * e};
            *(Vector2 *)t->target = val;
        }
        break;
        case TWEEN_VECTOR3:
        {
            Vector3 val = {
                t->startV3.x + (t->endV3.x - t->startV3.x) * e,
                t->startV3.y + (t->endV3.y - t->startV3.y) * e,
                t->startV3.z + (t->endV3.z - t->startV3.z) * e};
            *(Vector3 *)t->target = val;
        }
        break;
        }

        if (t->elapsed >= t->duration)
        {
            switch (t->type)
            {
            case TWEEN_FLOAT:
                *(float *)t->target = (t->easing == EASING_PINGPONG) ? t->startF : t->endF;
                break;
            case TWEEN_VECTOR2:
                *(Vector2 *)t->target = (t->easing == EASING_PINGPONG) ? t->startV2 : t->endV2;
                break;
            case TWEEN_VECTOR3:
                *(Vector3 *)t->target = (t->easing == EASING_PINGPONG) ? t->startV3 : t->endV3;
                break;
            }
            if (t->onComplete)
                t->onComplete(t->id);
            t->active = false;
        }
    }
}

static Tween *TweenManager_FindFree(void)
{
    for (int i = 0; i < MAX_TWEENS; i++)
        if (!gTweenManager.tweens[i].active)
            return &gTweenManager.tweens[i];
    return NULL;
}

static void TweenManager_OverwriteByID(const char *id)
{
    if (!id || !*id)
        return;

    for (int i = 0; i < MAX_TWEENS; i++)
    {
        Tween *t = &gTweenManager.tweens[i];
        if (t->active && strncmp(t->id, id, TWEEN_ID_MAX) == 0)
        {
            t->active = false;
        }
    }
}

static void TweenManager_SetID(Tween *t, const char *id)
{
    if (id && *id)
        strncpy(t->id, id, TWEEN_ID_MAX - 1);
    else
        t->id[0] = '\0';
    t->id[TWEEN_ID_MAX - 1] = '\0';
}

#define TWEEN_SETUP_COMMON(t, idStr, typ, ptr, dur, ease, cb) \
    (t)->active = true;                                       \
    TweenManager_SetID(t, idStr);                             \
    (t)->target = ptr;                                        \
    (t)->type = typ;                                          \
    (t)->duration = dur;                                      \
    (t)->elapsed = 0.0f;                                      \
    (t)->easing = ease;                                       \
    (t)->onComplete = cb;

static void TweenManager_AddFloat(float *target, float to, float duration, EasingType easing, const char *id, TweenCallback cb)
{
    TweenManager_OverwriteByID(id);
    Tween *t = TweenManager_FindFree();
    if (!t)
        return;
    TWEEN_SETUP_COMMON(t, id, TWEEN_FLOAT, target, duration, easing, cb);
    t->startF = *target;
    t->endF = to;
}

static void TweenManager_AddFloatFrom(float *target, float from, float to, float duration, EasingType easing, const char *id, TweenCallback cb)
{
    TweenManager_OverwriteByID(id);
    Tween *t = TweenManager_FindFree();
    if (!t)
        return;
    TWEEN_SETUP_COMMON(t, id, TWEEN_FLOAT, target, duration, easing, cb);
    t->startF = from;
    t->endF = to;
    *target = from;
}

static void TweenManager_AddVector2(Vector2 *target, Vector2 to, float duration, EasingType easing, const char *id, TweenCallback cb)
{
    TweenManager_OverwriteByID(id);
    Tween *t = TweenManager_FindFree();
    if (!t)
        return;
    TWEEN_SETUP_COMMON(t, id, TWEEN_VECTOR2, target, duration, easing, cb);
    t->startV2 = *target;
    t->endV2 = to;
}

static void TweenManager_AddVector2From(Vector2 *target, Vector2 from, Vector2 to, float duration, EasingType easing, const char *id, TweenCallback cb)
{
    TweenManager_OverwriteByID(id);
    Tween *t = TweenManager_FindFree();
    if (!t)
        return;
    TWEEN_SETUP_COMMON(t, id, TWEEN_VECTOR2, target, duration, easing, cb);
    t->startV2 = from;
    t->endV2 = to;
    *target = from;
}

static void TweenManager_AddVector3(Vector3 *target, Vector3 to, float duration, EasingType easing, const char *id, TweenCallback cb)
{
    TweenManager_OverwriteByID(id);
    Tween *t = TweenManager_FindFree();
    if (!t)
        return;
    TWEEN_SETUP_COMMON(t, id, TWEEN_VECTOR3, target, duration, easing, cb);
    t->startV3 = *target;
    t->endV3 = to;
}

static void TweenManager_AddVector3From(Vector3 *target, Vector3 from, Vector3 to, float duration, EasingType easing, const char *id, TweenCallback cb)
{
    TweenManager_OverwriteByID(id);
    Tween *t = TweenManager_FindFree();
    if (!t)
        return;
    TWEEN_SETUP_COMMON(t, id, TWEEN_VECTOR3, target, duration, easing, cb);
    t->startV3 = from;
    t->endV3 = to;
    *target = from;
}