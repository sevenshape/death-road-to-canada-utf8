#include <SDL.h>
#include <SDL_syswm.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>
#include <SDL2/SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_opengl.h>

//dllexport start

HMODULE SDL2_mixer_module = NULL;

void (*real_Mix_CloseAudio)(void) = NULL;
void (*real_Mix_FreeMusic)(Mix_Music *music) = NULL;
Mix_Music* (*real_Mix_LoadMUS)(const char *file) = NULL;
int (*real_Mix_OpenAudio)(int frequency, Uint16 format, int channels, int chunksize) = NULL;
void (*real_Mix_Pause)(int channel) = NULL;
void (*real_Mix_PauseMusic)(void) = NULL;
int (*real_Mix_PlayMusic)(Mix_Music *music, int loops) = NULL;
int (*real_Mix_RegisterEffect)(int chan, Mix_EffectFunc_t f, Mix_EffectDone_t d, void *arg) = NULL;
void (*real_Mix_Resume)(int channel) = NULL;
void (*real_Mix_ResumeMusic)(void) = NULL;
void (*real_Mix_RewindMusic)(void) = NULL;
int (*real_Mix_SetMusicPosition)(double position) = NULL;
int (*real_Mix_VolumeMusic)(int volume) = NULL;

__declspec(dllexport) void Mix_CloseAudio(void){return real_Mix_CloseAudio();}
__declspec(dllexport) void Mix_FreeMusic(Mix_Music *music){return real_Mix_FreeMusic(music);}
__declspec(dllexport) Mix_Music* Mix_LoadMUS(const char *file){return real_Mix_LoadMUS(file);}
__declspec(dllexport) int Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize){return real_Mix_OpenAudio(frequency, format, channels, chunksize);}
__declspec(dllexport) void Mix_Pause(int channel){return real_Mix_Pause(channel);}
__declspec(dllexport) void Mix_PauseMusic(void){return real_Mix_PauseMusic();}
__declspec(dllexport) int Mix_PlayMusic(Mix_Music *music, int loops){return real_Mix_PlayMusic(music, loops);}
__declspec(dllexport) int Mix_RegisterEffect(int chan, Mix_EffectFunc_t f, Mix_EffectDone_t d, void *arg){return real_Mix_RegisterEffect(chan, f, d, arg);}
__declspec(dllexport) void Mix_Resume(int channel){return real_Mix_Resume(channel);}
__declspec(dllexport) void Mix_ResumeMusic(void){return real_Mix_ResumeMusic();}
__declspec(dllexport) void Mix_RewindMusic(void){return real_Mix_RewindMusic();}
__declspec(dllexport) int Mix_SetMusicPosition(double position){return real_Mix_SetMusicPosition(position);}
__declspec(dllexport) int Mix_VolumeMusic(int volume){return real_Mix_VolumeMusic(volume);}

//dllexport end

char *btn_str_u;
DWORD btn_str_u_cpy;
wchar_t btn_str_s[65535];
int btn_str_s_p = 0;
double btn_str_s_xy[65535][2];
int btn_str_s_xy_p = 0;

char utf8_str[196605];

int btn_str_s_p_cpy = 0;
int btn_str_s_p_cpy_more = 0;
int btn_str_s_xy_p_cpy = 0;

char *btn_str_long_text;

//jmp start
BYTE layout_more_constprop_8_jmp[5];
//jmp end

//font start
/*double font_h = 30.0f;
double font_w = 10.0f;
double font_x_offset = 1.0f;
double font_y_offset = 0.25f;*/
double font_h = 10.0f;
double font_w = 10.0f;
double font_x_offset = 1.0f;
double font_y_offset = 1.0f;

HFONT hFont;

TTF_Font *ttf_font;
SDL_Renderer *ttf_renderer;

void font_set()
{
    int result = AddFontResource("Silver.ttf");
    if(result > 0)
    {
        hFont = CreateFont(
                -38,              // 字体高度
                0,                    // 字体宽度
                0,                    // 字体倾斜的角度
                0,                    // 字体底线的角度
                FW_NORMAL,            // 字体的粗细
                FALSE,                // 是否斜体
                FALSE,                // 是否带下划线
                FALSE,                // 是否带删除线
                DEFAULT_CHARSET,         // 字符集
                OUT_TT_PRECIS,        // 输出精度
                CLIP_TT_ALWAYS,  // 剪切精度
                CLEARTYPE_QUALITY,      // 输出质量
                DEFAULT_PITCH | FF_DONTCARE,  // 字体间距和族名
                "Silver"        // 字体名
        );
        SelectObject(wglGetCurrentDC(), hFont);
    }
    else
    {
        // 字体添加失败，处理错误
        MessageBox(NULL, "steam_wrapper error: Silver.ttf not find", "", MB_OK);
    }

    //ttf
    IMG_Init(IMG_INIT_PNG);
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
    }

    // 加载字体
    ttf_font = TTF_OpenFont("Silver.ttf", 36);
    if (ttf_font == NULL) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
}

void font_free()
{
    // 设回默认字体
    SelectObject(wglGetCurrentDC(), GetStockObject(DEFAULT_GUI_FONT));
    // 然后，删除字体对象
    if (hFont)
        DeleteObject(hFont);
}

//font end
//idapro全局属性
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

long double
(__cdecl *glyph_h)(DWORD *a1, unsigned __int8 a2) = (long double (__cdecl *)(DWORD *,
                                                                             unsigned __int8)) 0x004039B0;

char *
(__cdecl *wrap_text_ex)(const char **a1, int a2, unsigned int a3, int a4) = (char *(__cdecl *)(const char **,
                                                                                               int,
                                                                                               unsigned int,
                                                                                               int)) 0x00476080;


int (__cdecl *main_btn_framed)(int a1, int a2) = (int (__cdecl *)(int, int)) 0x0047AAB0;

int (__cdecl *wrap_text_lines)(const char *a1, unsigned int a2) = (int (__cdecl *)(const char *,
                                                                                   unsigned int)) 0x00476510;

int (__cdecl *turtle_move)(double a1) = (int (__cdecl *)(double)) 0x00409F60;

int
(__cdecl *plot_text_ex)(char *a1, char *a2, int a3, int a4) = (int (__cdecl *)(char *, char *, int, int)) 0x00474840;

int (__cdecl *sound_pip)(float, int) = (int (__cdecl *)(float, int)) 0x004AEDE0;

long double (__cdecl *frnd)(float, float) = (long double (__cdecl *)(float, float)) 0x00405CD0;

long double (*rndsign)() = (long double (*)()) 0x00405E10;

BOOL (__cdecl *main_is_cursor_hover)(float *) = (BOOL (__cdecl *)(float *)) 0x00478D40;

long double (*mad_h)() = (long double (*)()) 0x00404E20;

BOOL (__cdecl *cursors_disable)(int) = (BOOL (__cdecl *)(int)) 0x00431820;

int (*main_buttons_start)() = (int (*)()) 0x004768F0;

float *(*button_box)() = (float *(*)()) 0x0041C770;

int
(__cdecl *game_screen_title_ex)(char *, int, const char *) = (int (__cdecl *)(char *, int, const char *)) 0x00450D90;

int (__cdecl *main_set_default_start_button)(int) = (int (__cdecl *)(int)) 0x00475050;

int (__cdecl *game_screen_title)(char *) = (int (__cdecl *)(char *)) 0x00450E90;

DWORD *(__cdecl *game_lower_box)(DWORD *) = (DWORD *(__cdecl *)(DWORD *)) 0x00450D30;

DWORD *(__cdecl *game_full_box)(DWORD *) = (DWORD *(__cdecl *)(DWORD *)) 0x00450CF0;

void (__cdecl *button_set_layout)(float, float) = (void (__cdecl *)(float, float)) 0x0041C7F0;

int *(__cdecl *button_ex)(float, float, int, int, int) = (int *(__cdecl *)(float, float, int, int, int)) 0x0041CB70;

int (__cdecl *button_init)(int) = (int (__cdecl *)(int)) 0x0041CB30;

int (__cdecl *main_calc_image_wrap_lines)(int, DWORD *, DWORD *) = (int (__cdecl *)(int, DWORD *, DWORD *)) 0x004762C0;

int (__cdecl *cursors_reset_nearest)(float, float) = (int (__cdecl *)(float, float)) 0x004327B0;

int (__cdecl *cyoa_push_popup_state_btn)(int, int) = (int (__cdecl *)(int, int)) 0x00432E90;

int (__cdecl *cyoa_btn_choice)(int, int) = (int (__cdecl *)(int, int)) 0x004329E0;

int (__cdecl *cyoa_more_btn)(int, int) = (int (__cdecl *)(int, int)) 0x004337A0;

int (__cdecl *draw_icon)(int, int) = (int (__cdecl *)(int, int)) 0x00432ED0;

int (__cdecl *cyoa_btn_image_wrap)(int, int) = (int (__cdecl *)(int, int)) 0x00432FD0;

int (__cdecl *main_set_default_back_button)(int) = (int (__cdecl *)(int)) 0x00475060;


float *flt_4D048C = (float *) 0x004D048C;
float *flt_4D0498 = (float *) 0x004D0498;
float *flt_4D0490 = (float *) 0x004D0490;
float *flt_4D0494 = (float *) 0x004D0494;

int font6x8 = 0x00A39D04;

float *global_scale = (float *) 0x004D04AC;
float *gui_rumble = (float *) 0x0086B834;
float *gui_rumble_decay = (float *) 0x0086CA1C;
float *gui_rumble_reset = (float *) 0x0086CA18;

int *dword_95B6B4 = (int *) 0x0095B6B4;
int *dword_95CC54 = (int *) 0x0095CC54;
int *dword_95CC64 = (int *) 0x0095CC64;
int *dword_95CA4C = (int *) 0x0095CA4C;
int *dword_95CA54 = (int *) 0x0095CA54;
int *dword_95CC34 = (int *) 0x0095CC34;
int *dword_95CA50 = (int *) 0x0095CA50;
int *dword_95B6B8 = (int *) 0x0095B6B8;
int *dword_95CA5C = (int *) 0x0095CA5C;
int *dword_95CC3C = (int *) 0x0095CC3C;

char *cyoa_info = (char *) 0x0095B660;
char *Destination = (char *) 0x0095B689;
char *Str = (char *) 0x0095CA44;

int *textbox = (int *) 0x0079CB60;

__int64 *word_95CA64 = (__int64 *) 0x0095CA64;
int *btn_count = (int *) 0x006C92E0;
int *btns = (int *) 0x006C9300;
int *scaled_w = (int *)0x0086B940;
int *glypher_shadow = (int *)0x007CAA38;

int (*layout_more_constprop_8)() = (int (*)()) 0x00433390;
long double (__cdecl *particle_pre_draw_and_fade)(int) = (long double (__cdecl *)(int)) 0x0041F710;
int (__cdecl *game_set_turtle_lighting_for_pos_ex)(float, float, float, float) = (int (__cdecl *)(float, float, float, float)) 0x0044A0E0;
char* (__cdecl *plot_text_wrapped_ex)(char*, char*, int, int, int, int) = (char* (__cdecl *)(char*, char*, int, int, int, int)) 0x00476600;

int* display_h = (int*)0x004D0128;
int* display_w = (int*)0x004D012C;

//idapro全局属性 end

size_t utf8_strlen(const char* s) {
    size_t len = 0;
    while (*s) {
        if ((*s & 0xc0) != 0x80) {  // 如果此字节不是后续字节
            len++;
        }
        s++;
    }
    return len;
}

char *get_btn_str_u(char *str)
{
    btn_str_u = (char *) btn_str_u_cpy;
    if (((uintptr_t) btn_str_u & 0x00F00000) == 0x00400000 || btn_str_u == NULL)
    {
        btn_str_u = (char *) btn_str_u_cpy;
    }
    if (str == NULL || strlen(str) < 3)
        return str;

    char *long_text_start = strstr(btn_str_long_text, str);
    if (long_text_start)
    {
        //如果显示的是长文本内的文本, 如果`号数为奇数就在开头加`号
        int p_count = 0;
        for (int i = 0; i < strlen(str); i++)
        {
            if (str[i] == '`')
            {
                p_count++;
            }
        }
        if (p_count % 2 != 0)
        {
            sprintf(btn_str_u, "`%s", str);
        } else
        {
            strcpy(btn_str_u, str);
        }
    } else
    {
        strcpy(btn_str_u, str);  // 创建一个str的拷贝
    }

    const char *start_marker = "`";
    const char *end_marker = "`";
    size_t start_marker_len = strlen(start_marker);
    size_t end_marker_len = strlen(end_marker);

//    if(btn_str_u[0]=='\xE5' || btn_str_u[1]=='\xE5'){
//        printf("cs");
//    }

    char *start = strstr(btn_str_u, start_marker);

//    if(start && start[1]=='\x0'){
//        sprintf(btn_str_u, "`%s", str);
//        start = btn_str_u;
//    }

    while (start)
    {
        char *end = strstr(start + start_marker_len, end_marker);
        if (!end)
        {
            int len = strlen(start);
            for (int i = 0; i < len / 3; i++)
            {
                start[i] = '\x10';
                if (i + 1 >= len / 3)
                {
                    start[++i] = '\xE7';
                    start[++i] = '\0';
                }
            }
            return btn_str_u;
        }

        //start
        int len = (end - start - start_marker_len);
        char *us = (char *) malloc((len + 1) * sizeof(char));
        strncpy(us, start + start_marker_len, len);  // 从源字符串中复制
        us[len] = '\0';  // 确保字符串以 '\0' 结束

        int us_w_len = utf8_strlen(us);
        MultiByteToWideChar(CP_UTF8, 0, us, -1, btn_str_s + btn_str_s_p, us_w_len);
        free(us);
        btn_str_s_p += us_w_len ;
        //end

        int count = us_w_len ;

        for (int i = 0; i < count; i++)
        {
            start[i] = '\xE8';
        }

        memmove(start + count, end + end_marker_len, strlen(end + end_marker_len) + 1);

        start = strstr(start + count, start_marker);
    }

    return btn_str_u;
}

const char* utf8_index(const char* s, size_t pos) {
    size_t i = 0;

    while (*s) {
        // 到达所求位置，返回当前字符的指针
        if (i == pos) {
            return s;
        }

        // 如果当前字节是一个新字符的开始，增加字符计数器
        if ((*s & 0xc0) != 0x80) {
            i++;
        }

        // 移动到下一个字节
        s++;
    }

    // 如果位置超出字符串长度，返回NULL
    if (i < pos) {
        return NULL;
    }

    // 返回指向字符串终止字符的指针
    return s;
}

void CreateFontDisplayList(TTF_Font* font, const char* character, GLuint listBase) {
    // 使用SDL_ttf创建一个位图
    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, character, color);

    // 将SDL_Surface转换为一个适合OpenGL使用的像素格式
    SDL_Surface* conv_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);

    // 为当前的字符创建一个显示列表
    glNewList(listBase, GL_COMPILE);
    glPixelZoom(1, -1);
    glDrawPixels(conv_surface->w, conv_surface->h, GL_RGBA, GL_UNSIGNED_BYTE, conv_surface->pixels);
    glPixelZoom(1, 1);  // 恢复像素缩放
    glEndList();

    SDL_FreeSurface(conv_surface);
    SDL_FreeSurface(surface);
}

void draw_btn_s(wchar_t wchar, double x, double y)
{
    HDC hDC = wglGetCurrentDC();
    GLuint list = glGenLists(1);

    x -= font_w * font_x_offset;
    y += font_h * font_y_offset;

    if (!hFont)
        font_set();

    // 取消纹理映射
    glDisable(GL_TEXTURE_2D);
    glRasterPos2f(x, y);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    // 逐个输出字符
    wglUseFontBitmapsW(hDC, wchar, 1, list);
//    CreateFontDisplayList(ttf_font, "你好", list);
    glCallList(list);

    // 回收所有临时资源
    glDeleteLists(list, 1);
    glEnable(GL_TEXTURE_2D);

    /*GLuint list_b = glGenLists(1);
    glDisable(GL_TEXTURE_2D);
    glRasterPos2f(x, y);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    CreateFontDisplayList(ttf_font,"测试",list_b);
    glPushMatrix();
    glCallList(1000 );
    glPopMatrix();
    glDeleteLists(list_b, 1);
    glEnable(GL_TEXTURE_2D);*/
}

void my_SDL_GL_SwapWindow(SDL_Window *window)
{
    /*if(ttf_renderer == NULL)
    {
        ttf_renderer = SDL_GetRenderer(window);
    }*/
    /*for (int i = 0; i < *btn_count; i++)
    {
        int btn = (int)&btns[84 * i];
        char *str = *(char **) (btn + 200);
        if(!str)continue;
        char *p = strstr(str, "\xE7\x85\x8E");
    if (p)
    {
        void *ret_addr;
        __asm__("movl 4(%%ebp), %0" : "=r"(ret_addr));
        printf("Return address is %p\n", ret_addr);
    }
//        if(str)
//            printf("%s\n", str);
    }*/
    for (int i = 0; i < btn_str_s_xy_p; i++)
    {
//        if(btn_str_s[i]!='\x0')
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

void reset_jmp_rep(LPVOID address, BYTE *layout_more_constprop_8)
{
    DWORD oldProtect;

    // 修改内存保护，使其可写
    VirtualProtect(address, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

    // 写入jmp指令
    memcpy((LPVOID) address, layout_more_constprop_8, 5);

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
    int (__cdecl *plot_text_ex)(char *a1, char *a2, int a3, int a4) = (int (__cdecl *)(char *, char *, int,
                                                                                       int)) 0x00474840;
    int (__cdecl *sound_pip)(float, int) = (int (__cdecl *)(float, int)) 0x004AEDE0;
    long double (__cdecl *frnd)(float, float) = (long double (__cdecl *)(float, float)) 0x00405CD0;
    long double (*rndsign)() = (long double (*)()) 0x00405E10;
    BOOL (__cdecl *main_is_cursor_hover)(float *) = (BOOL (__cdecl *)(float *)) 0x00478D40;

    float *flt_4D0118 = (float *) 0x004D0118;
    float *flt_4D0114 = (float *) 0x004D0114;
    float *flt_4D011C = (float *) 0x004D011C;
    float *flt_4D0110 = (float *) 0x004D0110;
    float *flt_4D048C = (float *) 0x004D048C;
    float *flt_4D0498 = (float *) 0x004D0498;
    float *flt_4D0490 = (float *) 0x004D0490;
    float *flt_4D0494 = (float *) 0x004D0494;

    int font6x8 = 0x00A39D04;

    float *global_scale = (float *) 0x004D04AC;
    float *gui_rumble = (float *) 0x0086B834;
    float *gui_rumble_decay = (float *) 0x0086CA1C;
    float *gui_rumble_reset = (float *) 0x0086CA18;


    char v2; // al
    int result; // eax
    int v4; // eax
    int v5; // esi
    int v6; // eax
    int v7; // ebx
    char *v8; // edx
    char *v9; // ecx
    long double v10; // fst6
    long double v11; // fst5
    long double v12; // fst7
    long double v13; // fst5
    long double v14; // fst6
    long double v15; // fst7
    long double v16; // fst6
    bool v17; // c0
    bool v18; // c3
    long double v19; // fst7
    long double v20; // fst7
    long double v21; // fst6

    switch (a2)
    {
        case 1:
            v4 = sound_pip(1.0, 1);
            *(DWORD *) (v4 + 60) = 1048576000;
            v5 = v4;
            *(DWORD *) (v4 + 40) = 1;
            *(float *) (v4 + 148) = 0.89999998;
            *(float *) (v4 + 76) = 0.75 * *(float *) (v4 + 72);
            frnd(0.89999998, 1.1);
            result = a2;
            *(float *) (v5 + 68) = 0.89999998 * *(float *) (v5 + 68);
            return result;
        case 3:
            v6 = sound_pip(0.75, 1);
            *(DWORD *) (v6 + 60) = 1048576000;
            v7 = v6;
            *(DWORD *) (v6 + 40) = 1;
            *(float *) (v6 + 148) = 0.89999998;
            *(float *) (v6 + 76) = 1.25 * *(float *) (v6 + 72);
            frnd(0.89999998, 1.1);
            result = 1;
            *(float *) (v7 + 68) = (0.89999998 + 0.89999998) * *(float *) (v7 + 68);
            return result;
        case 5:
            if (*gui_rumble != 0.0)
            {
                *(float *) (a1 + 24) = rndsign() * *gui_rumble * *global_scale + *(float *) (a1 + 24);
                v20 = rndsign() * *gui_rumble * *global_scale + *(float *) (a1 + 28);
                v21 = *gui_rumble_decay;
                *(float *) (a1 + 24) = *(float *) (a1 + 24) * *gui_rumble_decay;
                *(float *) (a1 + 28) = v20 * v21;
            }
            if (!gui_rumble_reset)
                return 0;
            result = 0;
            *(float *) (a1 + 24) = 0.0;
            *(float *) (a1 + 28) = 0.0;
            return result;
        case 6:
            if (*(BYTE *) (a1 + 188))
            {
                if (*(char *) (a1 + 188) <= 0)
                {
                    *flt_4D0110 = *(float *) (a1 + 64);
                    *flt_4D0114 = *(float *) (a1 + 68);
                    *flt_4D0118 = *(float *) (a1 + 72);
                    *flt_4D011C = *(float *) (a1 + 76);
                } else
                {
                    *flt_4D0110 = *(float *) (a1 + 80);
                    *flt_4D0114 = *(float *) (a1 + 84);
                    *flt_4D0118 = *(float *) (a1 + 88);
                    *flt_4D011C = *(float *) (a1 + 92);
                }
                v2 = *(BYTE *) (a1 + 190);
            } else
            {
                v2 = *(BYTE *) (a1 + 190);
                if (v2 > 0)
                {
                    result = 0;
                    *flt_4D0110 = 0.0;
                    *flt_4D0114 = 0.0;
                    *flt_4D0118 = 0.0;
                    return result;
                }
            }
            if (v2 || !main_is_cursor_hover((float *) a1))
                return 0;
            if (*flt_4D0110 == 0.0)
            {
                if (*flt_4D0114 == 0.0)
                {
                    v12 = *flt_4D0118;
                    if (*flt_4D0118 == 0.0)
                        return 0;
                    v11 = *flt_4D0110;
                    v10 = *flt_4D0114;
                } else
                {
                    v12 = *flt_4D0118;
                    v11 = *flt_4D0110;
                    v10 = *flt_4D0114;
                }
            } else
            {
                v10 = *flt_4D0114;
                v11 = *flt_4D0110;
                v12 = *flt_4D0118;
            }
            v13 = v11 + v11;
            if (v13 < 0.25)
            {
                v13 = 0.25;
            } else if (v13 >= 1.0)
            {
                v13 = 1.0;
            }
            *flt_4D0110 = v13;
            v14 = v10 + v10;
            if (v14 < 0.25)
            {
                v14 = 0.25;
            } else if (v14 >= 1.0)
            {
                v14 = 1.0;
            }
            *flt_4D0114 = v14;
            v15 = v12 + v12;
            v16 = v15;
            if (v15 < 0.25)
            {
                v19 = 0.25;
            } else
            {
                v17 = v15 > 1.0;
                v18 = 1.0 == v15;
                v19 = 1.0;
                if (!v17 && !v18)
                    v19 = v16;
            }
            *flt_4D0118 = v19;
            return 0;
        case 7:
            //start
            btn_str_s_p_cpy = btn_str_s_p;
            btn_str_u = get_btn_str_u(*(char **) (a1 + 200));
//            if (*(char **) (a1 + 200) && strstr(*(char **) (a1 + 200), "HONK"))
//            {
//                printf("a");
//            }
            //end
            v8 = btn_str_u;
            result = 1;
            if (v8)
            {
                v9 = (char *) *(char *) (a1 + 193);
                *flt_4D048C = 0.0;
                *flt_4D0498 = 1.0;
                *flt_4D0490 = 0.0;
                *flt_4D0494 = 0.0;
                plot_text_ex(v8, v9, 1, font6x8);
                //start
                if (btn_str_s_p > btn_str_s_xy_p)
                {
                    btn_str_s_p = btn_str_s_p_cpy;
                }
                //end
                return 1;
            }
            //start
            if (btn_str_s_p > btn_str_s_xy_p)
            {
                btn_str_s_p = btn_str_s_p_cpy;
            }
            //end
            return result;
        default:
            return 0;
    }
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
    btn_str_s_xy_p_cpy = btn_str_s_xy_p;
    btn_str_u = get_btn_str_u(*(char **) (a1 + 200));
//    if (strstr(*(char **) (a1 + 200), "HONK"))
//    {
//        printf("a");
//    }
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
            btn_str_s_xy_p = btn_str_s_xy_p_cpy;
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
            btn_str_s_xy_p = btn_str_s_xy_p_cpy;
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
    //长文本判断 start
    char *a1_200 = *(char **) (a1 + 200);
    *(char **) (a1 + 200) = btn_str_u;
    char *a1_200_new = *(char **) (a1 + 200);
    v23 = main_calc_image_wrap_lines(a1, &v28, v29);
    *(char **) (a1 + 200) = a1_200;
    //长文本判断 end
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
            {
                //大文本处理逻辑
                btn_str_s_p = btn_str_s_xy_p;
                strcpy(btn_str_long_text, *(char **) (a1 + 200));
                break;
            }
        }
        v9 = v27;
        v8 = v13;
    }
    //start
//    if (btn_str_s_p > btn_str_s_xy_p)
//    {
//        btn_str_s_p = btn_str_s_p_cpy;
//        btn_str_s_xy_p = btn_str_s_xy_p_cpy;
//    }
    //end
    return 1;
}

bool more_flag = false;
bool more_rep_flag = false;
char **a1_200_p;
char *a1_200;
int more_btn_count = 0;

int __cdecl my_main_calc_image_wrap_lines(int a1, DWORD *a2, DWORD *a3)
{
    long double
    (__cdecl *glyph_h)(DWORD *a1, unsigned __int8 a2) = (long double (__cdecl *)(DWORD *,
                                                                                 unsigned __int8)) 0x004039B0;
    char *
    (__cdecl *wrap_text_ex)(const char **a1, int a2, unsigned int a3, int a4) = (char *(__cdecl *)(const char **,
                                                                                                   int,
                                                                                                   unsigned int,
                                                                                                   int)) 0x00476080;

    int font6x8 = 0x00A39D04;
    float *global_scale = (float *) 0x004D04AC;

//    if(more_rep_flag && a1_200==*(char **)(a1 + 200)){
//        *a1_200_p = a1_200;
//        more_flag = false;
//    }
    char *a1_200_cpy = *(char **) (a1 + 200);
    if (more_flag)
    {
        btn_str_s_p_cpy_more = btn_str_s_p;
        btn_str_u = get_btn_str_u(*(char **) (a1 + 200));
        btn_str_s_p = btn_str_s_p_cpy_more;
        *(char **) (a1 + 200) = btn_str_u;
    }

    long double v3; // fst7
    long double v4; // fst6
    int v5; // eax
    long double v6; // fst5
    char *v7; // esi
    int v8; // ebp
    int v9; // edi
    int i; // ebx
    BYTE *v11; // eax
    int v12; // edx
    int v14; // [esp+14h] [ebp-58h]
    int v15; // [esp+18h] [ebp-54h]
    float v16; // [esp+1Ch] [ebp-50h]
    int v17; // [esp+20h] [ebp-4Ch]
    int v18; // [esp+24h] [ebp-48h]
    int v19; // [esp+2Ch] [ebp-40h]
    char *v20; // [esp+4Ch] [ebp-20h] BYREF

    v20 = *(char **) (a1 + 200);
    v16 = glyph_h((DWORD *) font6x8, *v20) + 1.0;
    v3 = *(float *) (a1 + 296);
    v4 = *(float *) (a1 + 300);
    v5 = (int) (*(float *) (a1 + 44) / *global_scale);
    v6 = *(float *) (a1 + 32) / *global_scale - (v3 + v3);
    v18 = *(DWORD *) (a1 + 236);
    if (a2)
        *a2 = 0;
    v7 = v20;
    v8 = 0;
    v19 = 2 * v5;
    v17 = 0;
    v15 = (int) *(float *) (a1 + 284);
    v9 = (int) ((long double) (int) ((__int64) v6 - (int) *(float *) (a1 + 280)) - v3 * 0.5);
    for (i = (int) v4; v20; ++v8)
    {
        v14 = (int) v16;
        v11 = (BYTE *) wrap_text_ex(&v20, font6x8, v9, 0);
        if (!v11)
            break;
        if (!*v11)
            v14 = (int) ((long double) v14 * 0.5);
        v12 = i + v14;
        if (v18 != 0 && i <= v15 && v15 < v12)
            v9 = (__int64) v6;
        if (!v17 && v18)
        {
            if ((long double) v12 >= (long double) v19 - v16 && a2)
            {
                *a2 = v8;
                v17 = 1;
                if (a3)
                    *a3 = (DWORD) &v7[-*(DWORD *) (a1 + 200)];
            } else
            {
                v17 = 0;
            }
        }
        v7 = v20;
        i += v14;
    }
    *(char **) (a1 + 200) = a1_200_cpy;
    return v8;
}


int my_layout_more_constprop_8()
{
    more_flag = true;
    more_btn_count = *btn_count;

    reset_jmp_rep((LPVOID) 0x00433390, layout_more_constprop_8_jmp);
    int result = layout_more_constprop_8();
    jmp_rep((LPVOID) 0x00433390, my_layout_more_constprop_8);

//    *a1_200_p = a1_200;
    more_flag = false;
//    more_rep_flag = false;
//    for (int i = 0; i < *btn_count; i++)
//    {
//        int btn = (int)&btns[84 * i];
//        char *str = *(char **) (btn + 200);
//        if(str)
//            printf("%s\n", str);
//        else
//            printf("%d\n", i);
//    }

    return result;
}

char *__cdecl my_plot_text_wrapped_ex(char *a1, char *a2, int a3, int a4, int a5, int a6)
{
//    start

    btn_str_s_p_cpy = btn_str_s_p;
    btn_str_u = get_btn_str_u(a1);
//    if (strstr(a1, "HONK"))
//    {
//        printf("a");
//    }
//    end

    long double v6; // fst7
    char *result; // eax
    float v8; // [esp+1Ch] [ebp-30h]
    char *v9; // [esp+2Ch] [ebp-20h] BYREF

    v6 = glyph_h((DWORD *)a6, *a1);
    v9 = btn_str_u;
    do
    {
        result = (char *)wrap_text_ex(&v9, font6x8, a4, 0);
        if ( !result )
            break;
        plot_text_ex(result, a2, a3, a6);
        v8 = -(v6 + (long double)a5);
        turtle_trans(0.0, v8);
        result = v9;
    }
    while ( v9 );
    //start
    //浮动字体有些不会打印
    if(btn_str_s_p>btn_str_s_xy_p)
    {
        btn_str_s_p = btn_str_s_xy_p;
    }
    //end
    return result;
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

    //test start
//    char *p = strstr((char *) a2, "\xE7\x85\x8E");
//    if (p)
//    {
//        void *ret_addr;
//        __asm__("movl 4(%%ebp), %0" : "=r"(ret_addr));
//        printf("Return address is %p\n", ret_addr);
//    }
    //test end

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
                int sdw = *(int *) 0x007CAA38;
                if ((a2[v13] == 232) && (sdw == 0))
                {
                    btn_str_s_xy[btn_str_s_xy_p][0] = *(double *) 0x004D00C0;
                    btn_str_s_xy[btn_str_s_xy_p][1] = *(double *) 0x004D00C8;
                    btn_str_s_xy_p++;

//                    int * glyphs_set = (int *)0x007CAA20;
//                    for(int i=0;i<6;i++)
//                        printf("%d,",glyphs_set[i]);
//                    printf("%d\n",glyphs_set[6]);

//                    for(int i=0;i<11;i++)
//                        printf("%f,",turtle[i]);
//                    printf("%f\n",turtle[11]);
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
//            AllocConsole();
//            freopen("CONOUT$", "w", stdout);

            btn_str_u = (char *) malloc(196605 * sizeof(char));
            btn_str_u_cpy = (DWORD) btn_str_u;
            btn_str_long_text = (char *) malloc(196605 * sizeof(char));

            // 加载原DLL，获取真正的SDL2_mixer地址
            SDL2_mixer_module = LoadLibrary("real_SDL2_mixer.dll");

            real_Mix_CloseAudio = (void (*)(void)) GetProcAddress(SDL2_mixer_module, "Mix_CloseAudio");
            real_Mix_FreeMusic = (void (*)(Mix_Music *)) GetProcAddress(SDL2_mixer_module, "Mix_FreeMusic");
            real_Mix_LoadMUS = (Mix_Music *(*)(const char *)) GetProcAddress(SDL2_mixer_module, "Mix_LoadMUS");
            real_Mix_OpenAudio = (int (*)(int, Uint16, int, int)) GetProcAddress(SDL2_mixer_module, "Mix_OpenAudio");
            real_Mix_Pause = (void (*)(int)) GetProcAddress(SDL2_mixer_module, "Mix_Pause");
            real_Mix_PauseMusic = (void (*)(void)) GetProcAddress(SDL2_mixer_module, "Mix_PauseMusic");
            real_Mix_PlayMusic = (int (*)(Mix_Music *, int)) GetProcAddress(SDL2_mixer_module, "Mix_PlayMusic");
            real_Mix_RegisterEffect = (int (*)(int, Mix_EffectFunc_t, Mix_EffectDone_t, void *)) GetProcAddress(SDL2_mixer_module, "Mix_RegisterEffect");
            real_Mix_Resume = (void (*)(int)) GetProcAddress(SDL2_mixer_module, "Mix_Resume");
            real_Mix_ResumeMusic = (void (*)(void)) GetProcAddress(SDL2_mixer_module, "Mix_ResumeMusic");
            real_Mix_RewindMusic = (void (*)(void)) GetProcAddress(SDL2_mixer_module, "Mix_RewindMusic");
            real_Mix_SetMusicPosition = (int (*)(double)) GetProcAddress(SDL2_mixer_module, "Mix_SetMusicPosition");
            real_Mix_VolumeMusic = (int (*)(int)) GetProcAddress(SDL2_mixer_module, "Mix_VolumeMusic");

            if (real_Mix_CloseAudio == NULL)
            {
                printf("SDL2_mixer error: real_SDL2_mixer.dll not find");
                return FALSE;
            }

            //jmp_rep((LPVOID)0x004C7F38, my_glDrawElements);

            jmp_rep((LPVOID) 0x004C7DF8, my_SDL_GL_SwapWindow);

            //jmp_rep((LPVOID)0x004039F0, my_glyphs_w);

//            jmp_rep((LPVOID) 0x0041CB30, my_button_init);

            //jmp_rep((LPVOID)0x00403630, my_glyphs_batch_plot_ex);

            //jmp_rep((LPVOID)0x0041BC00, my_buttons_draw_ex);


            jmp_rep((LPVOID) 0x0047B0C0, my_main_btn_wrap);


            jmp_rep((LPVOID) 0x00478E10, my_main_btn_default);


//            jmp_rep((LPVOID) 0x0047B610, my_main_btn_range);
//            jmp_rep((LPVOID) 0x0047BA60, my_main_btn_slider);

            jmp_rep((LPVOID) 0x0047AB10, my_main_btn_image_wrap);

            //大文本辅助 start
            jmp_rep((LPVOID) 0x004762C0, my_main_calc_image_wrap_lines);


            memcpy(layout_more_constprop_8_jmp, (void *) 0x00433390, 5);
            jmp_rep((LPVOID) 0x00433390, my_layout_more_constprop_8);

//            jmp_rep((LPVOID) 0x00474940,my_plot_text);
//            jmp_rep((LPVOID) 0x00474970,my_plot_text_noshadow);
//            jmp_rep((LPVOID) 0x0041BC00,my_buttons_draw_ex);
//            jmp_rep((LPVOID) 0x004765A0,my_plot_textf);
//            jmp_rep((LPVOID) 0x0047C310, my_main_sprite_batches_draw_ex);
//            jmp_rep((LPVOID) 0x0044A520, my_float_text_draw);
            //大文本辅助 end

            //浮动文本 武器 start
            jmp_rep((LPVOID) 0x00476600, my_plot_text_wrapped_ex);
            //浮动文本 武器 end

            jmp_rep((LPVOID) 0x00403630, my_glyphs_batch_plot_ex);

            jmp_rep((LPVOID) 0x00403610, my_glyphs_set_callback);

            break;
        case DLL_PROCESS_DETACH:
            // 手动卸载原DLL
            FreeLibrary(SDL2_mixer_module);
//            font_free();
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}

