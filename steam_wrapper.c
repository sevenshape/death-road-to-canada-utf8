#include <GL/gl.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

//dllexport start

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

//dllexport end

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
    if(start && start[1]=='\0'){
        sprintf(btn_str_u, "`%s", str);
        start=btn_str_u;
    }
    while (start)
    {
        char *end = strstr(start + start_marker_len, end_marker);
        if (!end)
        {
            int len = strlen(start);
            for (int i = 0; i < len/3; i++)
            {
                start[i]='\x10';
                if(i+1 >= len/3){
                    start[++i]='\xE7';
                    start[++i]='\0';
                }
            }
            return btn_str_u;
            break;
        }

        //start
        int len = (end - start - start_marker_len);
        char *us = (char *) malloc((len + 1) * sizeof(char));
        strncpy(us, start + start_marker_len, len);  // 从源字符串中复制
        us[len] = '\0';  // 确保字符串以 '\0' 结束

        int us_w_len = MultiByteToWideChar(CP_UTF8, 0, us, -1, NULL, 0);
        MultiByteToWideChar(CP_UTF8, 0, us, -1, btn_str_s + btn_str_s_p, us_w_len);
        free(us);
        btn_str_s_p += us_w_len - 1;
        //end

        int count = us_w_len-1;

        for (int i = 0; i < count; i++)
        {
            start[i] = '\xE8';
        }

        memmove(start + count, end + end_marker_len, strlen(end + end_marker_len) + 1);

        start = strstr(start + count, start_marker);
    }

    return btn_str_u;
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
    for (int i = 0; i < btn_str_s_xy_p; i++)
    {
        draw_btn_s(btn_str_s[i], btn_str_s_xy[i][0], btn_str_s_xy[i][1]);
    }
    btn_str_s_xy_p = 0;
    btn_str_s_p = 0;

    SDL_GL_SwapWindow(window);
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

