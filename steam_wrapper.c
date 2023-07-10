#include <GL/gl.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>


HMODULE steam_wrapperModule = NULL;

//steam_api_init
typedef int (*steam_api_init_type)();

extern steam_api_init_type real_steam_api_init;

//steam_api_persona_name
typedef int (*steam_api_persona_name_type)();

extern steam_api_persona_name_type real_steam_api_persona_name;

//steam_api_set_achievement
typedef int (*steam_api_set_achievement_type)();

extern steam_api_set_achievement_type real_steam_api_set_achievement;

//steam_api_store_stats
typedef int (*steam_api_store_stats_type)();

extern steam_api_store_stats_type real_steam_api_store_stats;

steam_api_init_type real_steam_api_init = NULL;
steam_api_persona_name_type real_steam_api_persona_name = NULL;
steam_api_set_achievement_type real_steam_api_set_achievement = NULL;
steam_api_store_stats_type real_steam_api_store_stats = NULL;


//steam_api_init
//steam_api_persona_name
//steam_api_set_achievement
//steam_api_store_stats
__declspec(dllexport) int steam_api_init()
{
    return real_steam_api_init();
}

__declspec(dllexport) int steam_api_persona_name()
{
    return real_steam_api_persona_name();
}

__declspec(dllexport) int steam_api_set_achievement()
{
    return real_steam_api_set_achievement();
}

__declspec(dllexport) int steam_api_store_stats()
{
    return real_steam_api_store_stats();
}

char *btn_str_u;
DWORD btn_str_u_cpy;
wchar_t btn_str_s[65535];
int btn_str_s_p = 0;
double btn_str_s_xy[65535][2];
int btn_str_s_xy_p = 0;
int btn_str_s_p_cpy = 0;

char *get_btn_str_u(char *str)
{
    btn_str_u = (char *) btn_str_u_cpy;
    if (((uintptr_t) btn_str_u & 0x00F00000) == 0x00400000 || btn_str_u == NULL)
    {
        btn_str_u = (char *) btn_str_u_cpy;
    }
    if (str == NULL || strlen(str) < 3)
        return str;

    strcpy(btn_str_u, str);  // 创建一个str的拷贝

    const char *start_marker = "`";
    const char *end_marker = "`";
    size_t start_marker_len = strlen(start_marker);
    size_t end_marker_len = strlen(end_marker);

    char *start = strstr(btn_str_u, start_marker);
    while (start)
    {
        char *end = strstr(start + start_marker_len, end_marker);
        if (!end)
        {
            break;
        }
        int count = 0;
        int len = (end - start - start_marker_len);
        for (int i = 1; i < len + 1; i++)
        {
            if (start[i] < 0)
            {
                i += 2;
            }
            count++;
        }



        //start
        char *us = (char *) malloc((len + 1) * sizeof(char));
        strncpy(us, start + start_marker_len, len);  // 从源字符串中复制
        us[len] = '\0';  // 确保字符串以 '\0' 结束

        int us_w_len = MultiByteToWideChar(CP_UTF8, 0, us, -1, NULL, 0);
        MultiByteToWideChar(CP_UTF8, 0, us, -1, btn_str_s + btn_str_s_p, us_w_len);
        free(us);
        btn_str_s_p += us_w_len - 1;
        //end

        for (int i = 0; i < count; i++)
        {
            start[i] = '\xE8';
        }

        memmove(start + count, end + end_marker_len, strlen(end + end_marker_len) + 1);

        start = strstr(start + count, start_marker);
    }

    return btn_str_u;
}

void drawCNString(const char *str, float x, float y, float w, float h)
{
    int len, i;
    float text_line;
    wchar_t *wstring;
    HDC hDC = wglGetCurrentDC();
    GLuint list = glGenLists(1);

    float font_h = 32.0f;
    float font_w = 7.0f;


    HFONT hFont = CreateFont(
            (int) font_h,                // 字体高度
            (int) font_w,                 // 字体宽度
            0, 0,                  // 不使用倾斜
            FW_NORMAL,             // 正常字体粗细
            FALSE, FALSE, FALSE,   // 不使用斜体、下划线和删除线
            DEFAULT_CHARSET,       // 使用默认字符集
            OUT_DEFAULT_PRECIS,    // 默认输出精度
            CLIP_DEFAULT_PRECIS,   // 默认裁剪精度
            DEFAULT_QUALITY,       // 默认品质
            DEFAULT_PITCH | FF_DONTCARE, // 默认音高和字体系列
            "Arial"               // 字体名
    );
    //设置字体
    HGDIOBJ oldObject = SelectObject(hDC, hFont);

    len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    wstring = (wchar_t *) malloc(sizeof(wchar_t) * len);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, wstring, len);

    // 取消纹理映射
    glDisable(GL_TEXTURE_2D);
    text_line = 1.0f;
    glRasterPos2f(x - w / 2 + font_w, y - h / 2 + font_h);
    glColor3f(1.0f, 1.0f, 1.0f);  // 第二个浮点数无效, 所有这里设置红色
    // 逐个输出字符
    for (i = 0; i < len; ++i)
    {
        GLfloat rasterPos[4];
        glGetFloatv(GL_CURRENT_RASTER_POSITION, rasterPos);
        if (rasterPos[0] > (x - w / 2 + w - font_w * 2))
        {
            ++text_line;
            glRasterPos2f(x - w / 2 + font_w, y - h / 2 + font_h * text_line);
        }

        wglUseFontBitmapsW(hDC, wstring[i], 1, list);
        glCallList(list);
    }

    // 回收所有临时资源
    free(wstring);
    glDeleteLists(list, 1);
    glEnable(GL_TEXTURE_2D);
    // 当你不再需要这个字体时，你应该先将原来的对象重新选回设备上下文
    SelectObject(hDC, oldObject);
    // 然后，删除字体对象
    DeleteObject(hFont);
}

void draw_btn_s(wchar_t wchar, double x, double y)
{
    HDC hDC = wglGetCurrentDC();
    GLuint list = glGenLists(1);

    double font_h = 30.0f;
    double font_w = 10.0f;
    x -= font_w;
    y += font_h / 4;

    HFONT hFont = CreateFont(
            (int) font_h,                // 字体高度
            (int) font_w,                 // 字体宽度
            0, 0,                  // 不使用倾斜
            FW_NORMAL,             // 正常字体粗细
            FALSE, FALSE, FALSE,   // 不使用斜体、下划线和删除线
            DEFAULT_CHARSET,       // 使用默认字符集
            OUT_DEFAULT_PRECIS,    // 默认输出精度
            CLIP_DEFAULT_PRECIS,   // 默认裁剪精度
            DEFAULT_QUALITY,       // 默认品质
            DEFAULT_PITCH | FF_DONTCARE, // 默认音高和字体系列
            "Arial"               // 字体名
    );
    //设置字体
    HGDIOBJ oldObject = SelectObject(hDC, hFont);

    // 取消纹理映射
    glDisable(GL_TEXTURE_2D);
    glRasterPos2f(x, y);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    // 逐个输出字符
    wglUseFontBitmapsW(hDC, wchar, 1, list);
    glCallList(list);

    // 回收所有临时资源
    glDeleteLists(list, 1);
    glEnable(GL_TEXTURE_2D);
    // 当你不再需要这个字体时，你应该先将原来的对象重新选回设备上下文
    SelectObject(hDC, oldObject);
    // 然后，删除字体对象
    DeleteObject(hFont);
}

void my_SDL_GL_SwapWindow(SDL_Window *window)
{
    /*int btn_count = *(int*)0x006C92E0;
    for (int i = 0; i < btn_count; i++) {
        float x = btn_arr_x[i];
        float y = btn_arr_y[i];
        char* str = btn_arr_str[i];
        float w = btn_arr_w[i];
        float h = btn_arr_h[i];

        drawCNString(str, x, y, w, h);
    }*/

    for (int i = 0; i < btn_str_s_xy_p; i++)
    {
        draw_btn_s(btn_str_s[i], btn_str_s_xy[i][0], btn_str_s_xy[i][1]);
    }
    btn_str_s_xy_p = 0;
    btn_str_s_p = 0;

    SDL_GL_SwapWindow(window);
}

int __cdecl my_button_init(int a1)
{
    void (__cdecl *v1)(int, DWORD); // eax
    typedef int(__cdecl *FuncType)(int, DWORD);
    FuncType button_set_text = (FuncType) 0x0041C9C0;

    v1 = *(void (__cdecl **)(int, DWORD)) (a1 + 228);
    if (v1)
        v1(a1, 0);
    //v2 = *(int(__cdecl **)(int, int))(0x0041C9C0);
    //return v2(a1, *(DWORD*)(a1 + 200));

    float x = *((float *) a1 + 4) + *((float *) a1 + 6);
    float y = *((float *) a1 + 5) + *((float *) a1 + 7);
    //float x = *((float *)a1 + 43) * *((float *)a1 + 41);
    //float y = *((float *)a1 + 44) * *((float *)a1 + 42);
    char *Str = *(char **) (a1 + 200);
    float w = *(float *) (a1 + 32);
    float h = *(float *) (a1 + 36);


    int btn_count = *(int *) 0x006C92E0;

    return button_set_text(a1, *(DWORD *) (a1 + 200));
}

void jmp_rep(LPVOID address, LPVOID new_address)
{
    DWORD jmpOffset;
    BYTE jmpInstruction[5];
    DWORD oldProtect;
    // 我们在这里修改SDL_GL_SwapWindow函数
    jmpOffset = ((DWORD) new_address - (DWORD) address - 5);
    // 定义jmpInstruction变量
    jmpInstruction[0] = 0xE9;
    jmpInstruction[1] = jmpOffset & 0xFF;
    jmpInstruction[2] = (jmpOffset >> 8) & 0xFF;
    jmpInstruction[3] = (jmpOffset >> 16) & 0xFF;
    jmpInstruction[4] = (jmpOffset >> 24) & 0xFF;


    // 修改内存保护，使其可写
    VirtualProtect(address, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

    // 写入jmp指令
    memcpy((LPVOID) address, jmpInstruction, 5);

    // 恢复内存保护
    VirtualProtect((LPVOID) address, 5, oldProtect, NULL);
}

void __cdecl my_buttons_draw_ex(int a1, int a2)
{
    int (__cdecl *turtle_set_angle)(double) = (int (__cdecl *)(double)) 0x00409C60;
    long double (*mad_w)() = (long double (*)()) 0x00404E00;

    long double (*mad_h)() = (long double (*)()) 0x00404E20;

    void (__cdecl *turtle_set_pos_unscaled)(double, double) =
    (void (__cdecl *)(double, double)) 0x00409CA0;

    void (__cdecl *turtle_set_scalex)(double) =
    (void (__cdecl *)(double)) 0x00409D00;

    void (__cdecl *turtle_set_scaley)(double) =
    (void (__cdecl *)(double)) 0x00409D10;

    void (__cdecl *turtle_set_rgba)(float, float, float, float) =
    (void (__cdecl *)(float, float, float, float)) 0x00409D20;

    int (__cdecl *sprite_batch_plot)(int, int, int) =
    (int (__cdecl *)(int, int, int)) 0x00406460;

    long double (*mad_seconds)() = (long double (*)()) 0x00404C60;

    long double (__cdecl *mad_lerp)(float, float, float) =
    (long double (__cdecl *)(float, float, float)) 0x00404C80;

    int (__cdecl *glyphs_batch_plot_ex)(DWORD *, unsigned __int8 *, int, int, int) =
    (int (__cdecl *)(DWORD *, unsigned __int8 *, int, int, int)) 0x00403630;

    int (__cdecl *glyphs_batch_plot)(DWORD *, char *, int, int) =
    (int (__cdecl *)(DWORD *, char *, int, int)) 0x004038B0;

    double turtle = *(double *) 0x004D00C0;
    int sprites__0 = *(int *) 0x006C92CC;
    int font_ = *(int *) 0x006C92D0;
    int btn_count = *(int *) 0x006C92E0;
    int *btns = (int *) 0x006C9300;
    float fade_ = *(float *) 0x004D01F8;
    float *_sine_table = (float *) 0x0094C920;

    int v2; // edi
    int *v3; // ebx
    long double v4; // fst7
    long double v5; // fst6
    long double v6; // fst6
    int v7; // esi
    long double v8; // fst7
    long double v9; // fst6
    long double v10; // fst7
    int v11; // esi
    long double v12; // fst7
    int (__cdecl *v13)(int *, int); // eax
    long double v14; // fst6
    long double v15; // fst5
    long double v16; // fst6
    char v17; // al
    long double v18; // fst7
    int (__cdecl *v19)(int *, int); // eax
    char *v20; // eax
    int v21; // ecx
    long double v22; // fst7
    long double v23; // fst7
    long double v24; // fst7
    float v25; // [esp+8h] [ebp-D4h]
    float v26; // [esp+Ch] [ebp-D0h]
    float v27; // [esp+Ch] [ebp-D0h]
    float v28; // [esp+Ch] [ebp-D0h]
    float v29; // [esp+Ch] [ebp-D0h]
    float v30; // [esp+Ch] [ebp-D0h]
    float v31; // [esp+Ch] [ebp-D0h]
    float v32; // [esp+Ch] [ebp-D0h]
    float v33; // [esp+Ch] [ebp-D0h]
    float v34; // [esp+20h] [ebp-BCh]
    double v35; // [esp+20h] [ebp-BCh]
    float v36; // [esp+20h] [ebp-BCh]
    float v37; // [esp+20h] [ebp-BCh]
    float v38; // [esp+20h] [ebp-BCh]
    float v39; // [esp+28h] [ebp-B4h]
    float v40; // [esp+28h] [ebp-B4h]
    float v41; // [esp+28h] [ebp-B4h]
    float v42; // [esp+28h] [ebp-B4h]
    float v43; // [esp+28h] [ebp-B4h]
    float v44; // [esp+28h] [ebp-B4h]
    float v45; // [esp+28h] [ebp-B4h]
    double v46; // [esp+28h] [ebp-B4h]
    float v47; // [esp+30h] [ebp-ACh]
    float v48; // [esp+30h] [ebp-ACh]
    float v49; // [esp+38h] [ebp-A4h]
    float v50; // [esp+44h] [ebp-98h]
    float v51; // [esp+44h] [ebp-98h]
    float v52; // [esp+48h] [ebp-94h]
    int v53; // [esp+4Ch] [ebp-90h]
    int v54; // [esp+50h] [ebp-8Ch]
    float v55; // [esp+5Ch] [ebp-80h]
    float v56; // [esp+5Ch] [ebp-80h]
    float v57; // [esp+5Ch] [ebp-80h]
    float v58; // [esp+5Ch] [ebp-80h]
    char v59[96]; // [esp+60h] [ebp-7Ch] BYREF

    memcpy(v59, &turtle, sizeof(v59));
    v54 = sprites__0;
    if (a1)
    {
        if (sprites__0)
            goto LABEL_3;
    } else
    {
        turtle_set_angle(0.0);
        if (v54)
            goto LABEL_3;
    }
    if (!font_)
        return;
    v54 = *(DWORD *) (font_ + 12);
    LABEL_3:
    v2 = 0;
    if (btn_count > 0)
    {
        while (1)
        {
            if (a2)
            {
                if (a2 <= v2)
                    goto LABEL_37;
                v2 = a2;
            }
            v3 = &btns[84 * v2];
            v4 = *((float *) v3 + 4) + *((float *) v3 + 6);
            v5 = 1.0;
            if (!*((BYTE *) v3 + 160))
                v5 = 1.0 - *((float *) v3 + 37) * *((float *) v3 + 38);
            v39 = v5;
            v6 = *((float *) v3 + 8);
            if (v4 + v6 < 0.0)
                goto LABEL_36;
            v7 = v3[2];
            v49 = *((float *) v3 + 5);
            v53 = v3[82];
            v50 = *((float *) v3 + 7);
            v52 = v4;
            v47 = v4 - v6;
            if (v47 > mad_w())
                goto LABEL_36;
            v8 = *((float *) v3 + 5) + *((float *) v3 + 7);
            v9 = *((float *) v3 + 9);
            if ((float) 0.0 > v8 + v9)
                goto LABEL_36;
            v34 = v8 - v9;
            if (v34 > mad_h())
                goto LABEL_36;
            v10 = v49 + v50;
            if (v7 <= 0)
            {
                v14 = v10;
                v12 = v52;
                v35 = v14;
            } else
            {
                v48 = v39;
                v35 = v10;
                turtle_set_pos_unscaled(v52, v10);
                v11 = v54 + 32 * v7;
                v40 = (*((float *) v3 + 11) + *((float *) v3 + 11)) / (long double) *(__int16 *) (v11 + 14);
                v55 = (*((float *) v3 + 10) + *((float *) v3 + 10)) / (long double) *(__int16 *) (v11 + 12);
                turtle_set_scalex(v55 * v48);
                turtle_set_scaley(v48 * v40);
                if (*((BYTE *) v3 + 189))
                {
                    v32 = *((float *) v3 + 31) * fade_;
                    turtle_set_rgba(*((float *) v3 + 28), *((float *) v3 + 29), *((float *) v3 + 30), v32);
                    v12 = v52;
                } else
                {
                    if (*((BYTE *) v3 + 188))
                    {
                        v29 = *((float *) v3 + 23) * fade_;
                        turtle_set_rgba(*((float *) v3 + 20), *((float *) v3 + 21), *((float *) v3 + 22), v29);
                    } else
                    {
                        v26 = *((float *) v3 + 19) * fade_;
                        turtle_set_rgba(*((float *) v3 + 16), *((float *) v3 + 17), *((float *) v3 + 18), v26);
                    }
                    v12 = v52;
                }
                v13 = (int (__cdecl *)(int *, int)) v3[57];
                if (!v13 || (v41 = v12, v12 = v41, !v13(&btns[84 * v2], 6)))
                {
                    v42 = v12;
                    sprite_batch_plot(v11, 0, *((char *) v3 + 144));
                    v12 = v42;
                }
            }
            v15 = 1.0;
            v16 = 1.0 - *((float *) v3 + 37) * *((float *) v3 + 38);
            if (!*((BYTE *) v3 + 160))
                v15 = v16;
            v17 = *((BYTE *) v3 + 193);
            if (!v17)
                break;
            if (v17 == 2)
            {
                v43 = v16;
                turtle_set_pos_unscaled(v12 + v15 * *((float *) v3 + 10), v35);
                v18 = v43;
                goto LABEL_27;
            }
            v44 = v16;
            turtle_set_pos_unscaled(v12, v35);
            v18 = v44;
            if (!*((BYTE *) v3 + 160))
            {
                LABEL_40:
                v36 = v18;
                turtle_set_scalex(v18 * *((float *) v3 + 43) * *((float *) v3 + 41));
                turtle_set_scaley(v36 * *((float *) v3 + 44) * *((float *) v3 + 42));
                if (*((BYTE *) v3 + 189))
                {
                    v30 = *((float *) v3 + 35) * fade_;
                    turtle_set_rgba(*((float *) v3 + 32), *((float *) v3 + 33), *((float *) v3 + 34), v30);
                } else if (*((BYTE *) v3 + 188))
                {
                    v28 = *((float *) v3 + 27) * fade_;
                    turtle_set_rgba(*((float *) v3 + 24), *((float *) v3 + 25), *((float *) v3 + 26), v28);
                } else
                {
                    v33 = *((float *) v3 + 15) * fade_;
                    turtle_set_rgba(*((float *) v3 + 12), *((float *) v3 + 13), *((float *) v3 + 14), v33);
                }
                goto LABEL_31;
            }
            LABEL_28:
            if (v18 < 1.0)
            {
                v56 = 0.5 * _sine_table[(int) (mad_seconds() * 1460.0 * 22.755556) & 0x1FFF] + 0.5;
                v51 = v56;
                v46 = 0.5;
                v57 = v46 *
                      _sine_table[(int) ((mad_seconds() * (double) 1460.0 + 120.0) * (double) 22.755556) & 0x1FFF] +
                      v46;
                v37 = v57;
                v58 = v46 + v46 * _sine_table[(int) ((mad_seconds() * (double) 1460.0 + 240.0) * (double) 22.755556) &
                                              0x1FFF];
                v22 = *((float *) v3 + 37);
                mad_lerp(*((float *) v3 + 37), *((float *) v3 + 12), v51);
                *(float *) &v46 = v22;
                v23 = *((float *) v3 + 37);
                mad_lerp(*((float *) v3 + 37), *((float *) v3 + 13), v37);
                v38 = v23;
                v24 = *((float *) v3 + 37);
                mad_lerp(*((float *) v3 + 37), *((float *) v3 + 14), v58);
                v31 = *((float *) v3 + 15) * fade_;
                v25 = v24;
                turtle_set_rgba(*(float *) &v46, v38, v25, v31);
            } else
            {
                v27 = *((float *) v3 + 15) * fade_;
                turtle_set_rgba(*((float *) v3 + 12), *((float *) v3 + 13), *((float *) v3 + 14), v27);
            }
            turtle_set_scalex(*((float *) v3 + 43) * *((float *) v3 + 41));
            turtle_set_scaley(*((float *) v3 + 44) * *((float *) v3 + 42));
            LABEL_31:
            v19 = (int (__cdecl *)(int *, int)) v3[57];
            if (v19 && v19(&btns[84 * v2], 7))
                goto LABEL_36;
            v20 = (char *) v3[50];
            if (!v20)
                goto LABEL_36;
            v21 = v3[51];
            if (v21)
            {
                glyphs_batch_plot_ex((DWORD *) v53, v20, v21, *((char *) v3 + 193), *((char *) v3 + 145));
                LABEL_36:
                if (btn_count <= ++v2)
                    goto LABEL_37;
            } else
            {
                ++v2;
                glyphs_batch_plot((DWORD *) v53, v20, *((char *) v3 + 193), *((char *) v3 + 145));
                if (btn_count <= v2)
                    goto LABEL_37;
            }
        }
        v45 = v16;
        turtle_set_pos_unscaled(v12 - v15 * *((float *) v3 + 10), v35);
        v18 = v45;
        LABEL_27:
        if (!*((BYTE *) v3 + 160))
            goto LABEL_40;
        goto LABEL_28;
    }
    LABEL_37:
    memcpy(&turtle, v59, 0x60u);
}

int __cdecl my_main_btn_wrap(int a1, int a2)
{
    int (__cdecl *main_btn_framed)(int a1, int a2) = (int (__cdecl *)(int, int)) 0x0047AAB0;
    long double
    (__cdecl *glyph_h)(DWORD *a1, unsigned __int8 a2) = (long double (__cdecl *)(DWORD *, unsigned __int8)) 0x004039B0;
    int (__cdecl *wrap_text_lines)(const char *a1, unsigned int a2) = (int (__cdecl *)(const char *,
                                                                                       unsigned int)) 0x00476510;
    char *
    (__cdecl *wrap_text_ex)(const char **a1, int a2, unsigned int a3, int a4) = (char *(__cdecl *)(const char **, int,
                                                                                                   unsigned int,
                                                                                                   int)) 0x00476080;
    int (__cdecl *turtle_move)(double a1) = (int (__cdecl *)(double)) 0x00409F60;
    void (__cdecl *turtle_trans)(double a1, double a2) = (void (__cdecl *)(double, double)) 0x00409F10;
    int (__cdecl *plot_text_ex)(char *a1, char *a2, int a3, int a4) = (int (__cdecl *)(char *, char *, int,
                                                                                       int)) 0x00474840;

    float *global_scale = (float *) 0x004D04AC;
    int font6x8 = 0x00A39D04;
    //start
    btn_str_s_p_cpy = btn_str_s_p;
    btn_str_u = get_btn_str_u(*(char **) (a1 + 200));
    //end

    long double v3; // fst7
    int v4; // esi
    int v5; // edi
    int v6; // esi
    long double v7; // fst7
    char v8; // dl
    char *v9; // eax
    double v10; // [esp+8h] [ebp-44h]
    float v11; // [esp+18h] [ebp-34h]
    float v12; // [esp+18h] [ebp-34h]
    char *v13; // [esp+3Ch] [ebp-10h] BYREF


    if (a2 == 5)
    {
        goto LABEL_6;
    }
    if (a2 != 7)
    {
        if (a2)
        {
            //start
            if (btn_str_s_p > btn_str_s_xy_p)
            {
                btn_str_s_p = btn_str_s_p_cpy;
            }
            //end
            return main_btn_framed(a1, a2);
        }

        *(DWORD *) (a1 + 252) = 12;
        LABEL_6:
        *(float *) (a1 + 164) = 1.0;
        *(float *) (a1 + 168) = 1.0;
        //start
        if (btn_str_s_p > btn_str_s_xy_p)
        {
            btn_str_s_p = btn_str_s_p_cpy;
        }
        //end
        return main_btn_framed(a1, a2);
    }
    v3 = *(float *) (a1 + 32) / *global_scale;
    v13 = btn_str_u;
    v4 = (__int64) (v3 - (long double) *(int *) (a1 + 252));
    v5 = v4;
    v11 = glyph_h((DWORD *) font6x8, *v13) + 1.0;
    v6 = wrap_text_lines(btn_str_u, v4) - 1;
    if (*(DWORD *) (a1 + 236))
    {
        turtle_move((double) (int) (*(float *) (a1 + 44) / *global_scale - 12.0));
        v7 = 0.0;
    } else
    {
        v7 = 0.5;
    }
    v8 = *(BYTE *) (a1 + 193);
    v10 = v7 * ((long double) v6 * v11);
    if (v8)
    {
        if (v8 == 2)
            turtle_trans((double) (*(DWORD *) (a1 + 252) / -2), v10);
        else
            turtle_trans(0.0, v10);
    } else
    {
        turtle_trans((double) (*(DWORD *) (a1 + 252) / 2), v10);
    }
    v13 = btn_str_u;

    if (v13)
    {
        v12 = -v11;
        do
        {
            v9 = (char *) wrap_text_ex(&v13, font6x8, v5, 0);
            if (!v9)
                break;
            plot_text_ex(v9, (char *) *(char *) (a1 + 193), 1, font6x8);
            turtle_trans(0.0, v12);
        } while (v13);
    }
    //start
    if (btn_str_s_p > btn_str_s_xy_p)
    {
        btn_str_s_p = btn_str_s_p_cpy;
    }
    //end
    return 1;
}

int __cdecl my_main_btn_default(int a1, int a2)
{
    return 1;
}

int __cdecl my_main_btn_range(int a1, int a2)
{
    return 1;
}

int __cdecl my_main_btn_image_wrap(int a1, int a2)
{
    int (__cdecl *main_btn_framed)(int a1, int a2) = (int (__cdecl *)(int, int)) 0x0047AAB0;
    long double
    (__cdecl *glyph_h)(DWORD *a1, unsigned __int8 a2) = (long double (__cdecl *)(DWORD *,
                                                                                 unsigned __int8)) 0x004039B0;
    int (__cdecl *wrap_text_lines)(const char *a1, unsigned int a2) = (int (__cdecl *)(const char *,
                                                                                       unsigned int)) 0x00476510;
    char *
    (__cdecl *wrap_text_ex)(const char **a1, int a2, unsigned int a3, int a4) = (char *(__cdecl *)(const char **,
                                                                                                   int,
                                                                                                   unsigned int,
                                                                                                   int)) 0x00476080;
    int (__cdecl *turtle_move)(double a1) = (int (__cdecl *)(double)) 0x00409F60;
    void (__cdecl *turtle_trans)(double a1, double a2) = (void (__cdecl *)(double, double)) 0x00409F10;
    int (__cdecl *plot_text_ex)(char *a1, char *a2, int a3, int a4) = (int (__cdecl *)(char *, char *, int,
                                                                                       int)) 0x00474840;
    int (__cdecl *main_calc_image_wrap_lines)(int a1, DWORD *a2, DWORD *a3) = (int (__cdecl *)(int, DWORD *,
                                                                                               DWORD *)) 0x004762C0;

    float *global_scale = (float *) 0x004D04AC;
    int font6x8 = 0x00A39D04;
    float *turtle = (float *) 0x004D00C0;

    //start
    btn_str_s_p_cpy = btn_str_s_p;
    btn_str_u = get_btn_str_u(*(char **) (a1 + 200));
    //end

    int v2; // eax
    void (__cdecl *v3)(int, int); // ebx
    long double v4; // fst7
    int v5; // esi
    int v6; // ebx
    int v7; // esi
    int v8; // edi
    int v9; // eax
    int v10; // ebx
    char *v11; // eax
    int v12; // eax
    int v13; // ebx
    void (__cdecl *v14)(int, DWORD); // eax
    int v16; // [esp+18h] [ebp-C4h]
    float v17; // [esp+20h] [ebp-BCh]
    int v18; // [esp+20h] [ebp-BCh]
    float v19; // [esp+24h] [ebp-B8h]
    bool v20; // [esp+2Bh] [ebp-B1h]
    unsigned int v21; // [esp+2Ch] [ebp-B0h]
    int v22; // [esp+30h] [ebp-ACh]
    int v23; // [esp+34h] [ebp-A8h]
    float v24; // [esp+34h] [ebp-A8h]
    int v25; // [esp+38h] [ebp-A4h]
    int v26; // [esp+3Ch] [ebp-A0h]
    int v27; // [esp+54h] [ebp-88h] BYREF
    int v28; // [esp+58h] [ebp-84h] BYREF
    char v29[4]; // [esp+5Ch] [ebp-80h] BYREF
    char v30[96]; // [esp+60h] [ebp-7Ch] BYREF

    if (!a2)
    {
        v14 = *(void (__cdecl **)(int, DWORD)) (a1 + 232);
        *(BYTE *) (a1 + 193) = 0;
        *(float *) (a1 + 296) = 6.0;
        *(float *) (a1 + 300) = 6.0;
        *(float *) (a1 + 164) = 1.0;
        *(float *) (a1 + 168) = 1.0;
        if (v14)
            v14(a1, 0);
        //start
        if (btn_str_s_p > btn_str_s_xy_p)
        {
            btn_str_s_p = btn_str_s_p_cpy;
        }
        //end
        return main_btn_framed(a1, a2);
    }
    if (a2 != 7)
    {
        //start
        if (btn_str_s_p > btn_str_s_xy_p)
        {
            btn_str_s_p = btn_str_s_p_cpy;
        }
        //end
        return main_btn_framed(a1, a2);
    }
    v27 = (int) btn_str_u;
    v19 = glyph_h((DWORD *) font6x8, 88) + 1.0;
    v22 = (int) (*(float *) (a1 + 44) / *global_scale - *(float *) (a1 + 300));
    v17 = *(float *) (a1 + 296);
    *(__int64 *) &v16 = (__int64) (*(float *) (a1 + 32) / *global_scale - (v17 + v17));
    v21 = v16;
    v23 = main_calc_image_wrap_lines(a1, &v28, v29);
    v2 = *(DWORD *) (a1 + 236);
    v3 = *(void (__cdecl **)(int, int)) (a1 + 232);
    v4 = v17;
    memcpy(v30, turtle, sizeof(v30));
    v20 = v2 != 0;
    if (v3)
    {
        v26 = v2;
        v5 = (int) *(float *) (a1 + 280);
        v25 = (int) *(float *) (a1 + 284);
        turtle_trans((long double) (v5 / 2) + v4, (double) (v20 * (v22 - v25 / 2)));
        v3(a1, 7);
        v2 = v26;
        v4 = v17;
        v16 = (int) ((long double) (int) (v16 - v5) - v17 * 0.5);
        v18 = (int) ((long double) v5 + v17);
    } else
    {
        v18 = 0;
        v25 = 0;
    }
    memcpy(turtle, v30, 0x60u);
    v6 = v23 - 1;
    if (v2)
        turtle_move((double) (int) ((long double) v22 - v19 * 0.5));
    else
        turtle_move((long double) v6 * v19 * 0.5);
    if (!*(BYTE *) (a1 + 193))
    {
        v24 = v4;
        turtle_trans(v24, (long double) v6 * v19 * 0.0);
    }
    v7 = 0;
    v8 = 0;
    turtle_trans((double) v18, 0.0);
    v9 = (int) btn_str_u;
    v27 = v9;
    while (v9)
    {
        v10 = (int) v19;
        v11 = (char *) wrap_text_ex(&v27, font6x8, v16, 0);
        if (!v11)
            break;
        if (!*v11)
            v10 = (int) ((long double) v10 * 0.5);
        plot_text_ex(v11, (char *) *(char *) (a1 + 193), 1, (int) font6x8);
        v12 = v10;
        v13 = v8 + v10;
        turtle_trans(0.0, (double) -v12);
        if (v8 <= v25 && v20 && v13 > v25)
        {
            turtle_trans((double) -v18, 0.0);
            v16 = v21;
        }
        if (++v7 >= v28)
        {
            if (v28)
                break;
        }
        v9 = v27;
        v8 = v13;
    }
    //start
    if (btn_str_s_p > btn_str_s_xy_p)
    {
        btn_str_s_p = btn_str_s_p_cpy;
    }
    //end
    return 1;
}

int (*glyph_callback_func_)(DWORD, DWORD, DWORD);

int glyph_callback_context_;

int __cdecl my_glyphs_set_callback(int (__cdecl *a1)(DWORD, DWORD, DWORD), int a2)
{
    int result; // eax

    glyph_callback_func_ = a1;
    result = a2;
    glyph_callback_context_ = a2;
    return result;
}

int __cdecl my_glyphs_batch_plot_ex(DWORD *a1, unsigned __int8 *a2, int a3, int a4, int a5)
{
    void (__cdecl *turtle_trans)(double a1, double a2) =
    (void (__cdecl *)(double, double)) 0x00409F10;
    int (__cdecl *sprite_batch_plot)(int, int, int) =
    (int (__cdecl *)(int, int, int)) 0x00406460;
    char *(__cdecl *sprite_get)(__int16 a1) =
    (char *(__cdecl *)(__int16)) 0x00406930;

    double *turtle = (double *) 0x004D00C0;
    float *flt_4D0118 = (float *) 0x004D0118;
    float *flt_4D0114 = (float *) 0x004D0114;
    float *flt_4D011C = (float *) 0x004D011C;
    float *flt_4D0110 = (float *) 0x004D0110;

    int v5; // ebp
    unsigned int v6; // edx
    unsigned int v8; // eax
    long double v9; // fst7
    unsigned __int8 *v10; // edx
    long double v11; // fst6
    int v12; // eax
    int v13; // esi
    unsigned int v14; // ebx
    unsigned int v15; // eax
    int v16; // edi
    float v17; // ebx
    float v18; // eax
    float v19; // ebp
    float v20; // edx
    int v21; // [esp+18h] [ebp-94h]
    float v22; // [esp+1Ch] [ebp-90h]
    float v23; // [esp+1Ch] [ebp-90h]
    char v24[96]; // [esp+30h] [ebp-7Ch] BYREF

    if (!a1)
        return 0;
    v5 = a1[1];
    v21 = (int) sprite_get(*a1);
    memcpy(v24, turtle, sizeof(v24));
    v6 = a1[2];
    if (!v6)
        return 0;
    v8 = *a2 - v5;
    if (v6 <= v8)
        v8 = v6 - 1;
    v22 = (long double) *(__int16 *) (v21 + 32 * v8 + 12) * 0.5;
    v9 = v22;
    if (a3 <= 0)
    {
        v11 = 0.0;
    } else
    {
        v10 = a2;
        v11 = 0.0;
        do
        {
            v12 = *v10++;
            v11 = v11 + (long double) *(__int16 *) (v21 + 32 * (v12 - v5) + 12);
        } while (v10 != &a2[a3]);
    }
    if (a4 == 2)
    {
        v9 = v9 - v11;
    } else if (a4 == 1)
    {
        v23 = v9 + -v11 * 0.5;
        v9 = v23;
    }
    v13 = 0;
    turtle_trans(v9, 0.0);
    if (a3 > 0)
    {
        do
        {
            v14 = a2[v13] - v5;
            if (!glyph_callback_func_ || !glyph_callback_func_((char) v14, 0, glyph_callback_context_))
            {
                v15 = a1[2];
                if (v14 >= v15)
                    v14 = v15 - 1;
                v16 = v21 + 32 * v14;
                if (v13)
                    turtle_trans((long double) *(__int16 *) (v16 + 12) * 0.5, 0.0);
                if (a2[v13] == 232 && *(int *) 0x007CAA38 == 0)
                {
                    btn_str_s_xy[btn_str_s_xy_p][0] = *(double *) 0x004D00C0;
                    btn_str_s_xy[btn_str_s_xy_p][1] = *(double *) 0x004D00C8;
                    btn_str_s_xy_p++;
                }
                /*if(btn_str_u_start==0){
                    btn_str_u_count--;
                    double x=*(double *) 0x004D00C0;
                    double y=*(double *) 0x004D00C8;
                    drawCNString_point("字", x, y);
                    btn_str_s_xy[btn_str_s_xy_p][0] = *(double *) 0x004D00C0;
                    btn_str_s_xy[btn_str_s_xy_p][1] = *(double *) 0x004D00C8;
                    btn_str_s_xy_p++;
                }
                btn_str_u_start--;
                if(btn_str_u_start<0 && btn_str_u_count>0){
                    btn_str_u_start=0;
                }*/
                sprite_batch_plot(v16, 0, a5);
                turtle_trans((long double) *(__int16 *) (v16 + 12) * 0.5, 0.0);
            }
            if (glyph_callback_func_)
                glyph_callback_func_((char) v14, 1, glyph_callback_context_);
            ++v13;
        } while (a3 != v13);
    }
    v17 = *flt_4D0118;
    v18 = *flt_4D0114;
    v19 = *flt_4D011C;
    v20 = *flt_4D0110;
    memcpy(turtle, v24, sizeof(v24));
    *flt_4D0118 = v17;
    *flt_4D0114 = v18;
    *flt_4D011C = v19;
    *flt_4D0110 = v20;
    return a3;
}

// DLL被加载、卸载时调用
BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            AllocConsole();
            freopen("CONOUT$", "w", stdout);

            btn_str_u = (char *) malloc(256 * sizeof(char));
            btn_str_u_cpy = (DWORD) btn_str_u;

            //GetModuleFileName(GetModuleHandle(NULL), processPath, MAX_PATH);
            //_tcscpy_s(msg, _T("注入了进程"));
            //_tcscat_s(msg, processPath);
            //MessageBox(NULL, msg, _T(""), MB_OK);

            // 加载原DLL，获取真正的Direct3DCreate9地址
            steam_wrapperModule = LoadLibrary("real_steam_wrapper.dll");
            //steam_api_init
            //steam_api_persona_name
            //steam_api_set_achievement
            //steam_api_store_stats
            real_steam_api_init = (steam_api_init_type) GetProcAddress(steam_wrapperModule, "steam_api_init");
            real_steam_api_persona_name = (steam_api_persona_name_type) GetProcAddress(steam_wrapperModule,
                                                                                       "steam_api_persona_name");
            real_steam_api_set_achievement = (steam_api_set_achievement_type) GetProcAddress(steam_wrapperModule,
                                                                                             "steam_api_set_achievement");
            real_steam_api_store_stats = (steam_api_store_stats_type) GetProcAddress(steam_wrapperModule,
                                                                                     "steam_api_store_stats");
            if (real_steam_api_init == NULL)
            {
                MessageBox(NULL, "steam_wrapper error: real_steam_wrapper.dll not find", "", MB_OK);
                return FALSE;
            }

            // 我们在这里修改glDrawElements函数
            //jmp_rep((LPVOID)0x004C7F38, my_glDrawElements);
            // 我们在这里修改SDL_GL_SwapWindow函数
            jmp_rep((LPVOID) 0x004C7DF8, my_SDL_GL_SwapWindow);
            // 我们在这里修改glyphs_w函数
            //jmp_rep((LPVOID)0x004039F0, my_glyphs_w);
            // 我们在这里修改button_init函数
            //jmp_rep((LPVOID)0x0041CB30, my_button_init);
            // 我们在这里修改glyphs_batch_plot_ex函数
            //jmp_rep((LPVOID)0x00403630, my_glyphs_batch_plot_ex);
            // 我们在这里修改glyphs_batch_plot_ex函数
            //jmp_rep((LPVOID)0x00474940, plot_text);
            // 我们在这里修改buttons_draw_ex函数
            //jmp_rep((LPVOID)0x0041BC00, my_buttons_draw_ex);


            jmp_rep((LPVOID) 0x0047B0C0, my_main_btn_wrap);


            //jmp_rep((LPVOID)0x00478E10, my_main_btn_default);


            //jmp_rep((LPVOID)0x0047B610, my_main_btn_range);

            jmp_rep((LPVOID) 0x0047AB10, my_main_btn_image_wrap);

            jmp_rep((LPVOID) 0x00403630, my_glyphs_batch_plot_ex);

            jmp_rep((LPVOID) 0x00403610, my_glyphs_set_callback);

            break;
        case DLL_PROCESS_DETACH:
            // 手动卸载原DLL
            FreeLibrary(steam_wrapperModule);

            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}

