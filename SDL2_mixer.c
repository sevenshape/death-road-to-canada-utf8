#include <SDL.h>
#include <SDL_syswm.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>
#include <SDL2/SDL_mixer.h>
#include <SDL_opengl.h>
#include <wchar.h>
#include <ctype.h>
#include "NERVE SEP-27-2022.07.43.h"

#include <libintl.h>
#include <locale.h>

#define _(STRING) gettext(STRING)

//start
//dllexport
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
//end

char *btn_str_u;
wchar_t btn_str_s[4095];
int btn_str_s_p = 0;
double btn_str_s_xy[4095][2];
int btn_str_s_xy_p = 0;

char *str_rep_temp;

char *str_result_temp;

//start
//font
double font_h = 4.0f;
double font_w = 4.0f;
double font_x_offset = 3.0f;
double font_y_offset = 1.0f;

HFONT hFont;

void font_set()
{
    int result = AddFontResource("Zpix.ttf");
    if(result > 0)
    {
//        HDC hdc = wglGetCurrentDC(); // 获取屏幕设备环境
//        int nHeight = -MulDiv(30, GetDeviceCaps(wglGetCurrentDC(), LOGPIXELSY), 72); // 转换字体大小为像素值
        hFont = CreateFont(
                -23,              // 字体高度
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
                "Zpix"        // 字体名
        );
        SelectObject(wglGetCurrentDC(), hFont);
    }
    else
    {
        // 字体添加失败，处理错误
        MessageBox(NULL, "steam_wrapper error: Silver.ttf not find", "", MB_OK);
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
//end

//start
//jmp util
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
//end

char* utf8_str_rep(const wchar_t * s) {
    int s_len = wcslen(s);
    str_rep_temp[s_len] = '\0';
    int len = 0;
    while (*s) {
        if (*s < 256) {
            str_rep_temp[len] = (char)*s;
        }else{
            str_rep_temp[len] = '\xE8';
        }

        len++;
        s++;
    }

    return str_rep_temp;
}

void remove_ascii(wchar_t* str) {
    wchar_t* i = str;
    wchar_t* j = str;

    while (*j != L'\0') {
        *i = *j++;
        if (*i < 256) {
            i--;
        }
        i++;
    }

    *i = L'\0';
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
    glCallList(list);

    // 回收所有临时资源
    glDeleteLists(list, 1);
    glEnable(GL_TEXTURE_2D);
}

void my_SDL_GL_SwapWindow(SDL_Window *window)
{
    for (int i = 0; i < btn_str_s_xy_p ; i++)
    {
        draw_btn_s(btn_str_s[i], btn_str_s_xy[i][0], btn_str_s_xy[i][1]);
    }
    btn_str_s_xy_p = 0;
    btn_str_s_p = 0;

    SDL_GL_SwapWindow(window);
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
    //start
    //寻找未翻译的文本
//    char *p = strstr((char *) a2, "strength");
//    if (p)
//    {
//        void *ret_addr;
//        __asm__("movl 4(%%ebp), %0" : "=r"(ret_addr));
//        printf("Return address is %p\n", ret_addr);
//    }
    //end

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
                //start
                //记录渲染的utf8字符位置
                if ((a2[v13] == 232) && (*glypher_shadow == 0))
                {
                    btn_str_s_xy[btn_str_s_xy_p][0] = *turtle;
                    btn_str_s_xy[btn_str_s_xy_p][1] = *dbl_4D00C8;
                    btn_str_s_xy_p++;
                }
                //end
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

//start
//寻找未翻译的文本

int __cdecl my_plot_text_ex(char *a1, char *a2, int a3, int a4)
{
    //start
    //寻找未翻译的文本
//    char *p = strstr(a1, "strength");
//    if (p)
//    {
//        void *ret_addr;
//        __asm__("movl 4(%%ebp), %0" : "=r"(ret_addr));
//        printf("Return address is %p\n", ret_addr);
//    }
    //end

    int v4; // edx
    float v6; // [esp+Ch] [ebp-90h]
    char v7[96]; // [esp+20h] [ebp-7Ch] BYREF

    v4 = a4;
    if ( a3 )
    {
//        *glypher_shadow = 1;
//        memcpy(v7, turtle, sizeof(v7));
//        v6 = *flt_4D011C * *flt_4D0498;
//        turtle_set_rgba(*shadow, *flt_4D0490, *flt_4D0494, v6);
//        turtle_trans(1.0, -1.0);
//        glyphs_batch_plot(a4, a1, (int)a2, *textglow_);
//        v4 = a4;
//        *glypher_shadow = 0;
//        memcpy(turtle, v7, 0x60u);
        *glypher_shadow = 0;
    }

    //start
//    printf("%s\n",a1);
    int temp = btn_str_s_p;
    int temp_xy = btn_str_s_xy_p;
//    btn_str_u = get_btn_str_u(a1);

    int str_len = strlen(a1);

    strcpy(btn_str_u, a1);
    int us_w_len = MultiByteToWideChar(CP_UTF8, 0, btn_str_u, -1, NULL, 0);
    wchar_t* us_w = (wchar_t *) malloc((str_len+10) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, btn_str_u, -1, us_w, us_w_len);

    int str_c_index = 0;
    for(int i=0;i<us_w_len-1;i++){

        if(us_w[i]<256){
            str_c_index++;
        }else if(us_w[i]==0xFFFD){
            if('\xFF'>=btn_str_u[str_c_index] && btn_str_u[str_c_index]=='\xF0' && '\xBF'>=btn_str_u[str_c_index+1] && btn_str_u[str_c_index+1]>='\x80'){
                wchar_t * s = us_w;
                int pos = i;
                int len = us_w_len-1;
                wmemmove(&s[pos + 1], &s[pos], len - pos + 1);

                us_w[i] = (unsigned char)btn_str_u[str_c_index];  // 如果你想在移动的位置插入特定的字符，可以在这里指定

                us_w_len++;
                str_c_index++;
                if(us_w[i]==0xE8){
                    us_w[i]='?';
                }
                i++;

                us_w[i]=(unsigned char)btn_str_u[str_c_index];
                str_c_index++;
            }else{
                us_w[i]=(unsigned char)btn_str_u[str_c_index];
                str_c_index++;
            }
        }else{
            str_c_index+=3;
        }
        if(us_w[i]==0xE8){
            us_w[i]='?';
        }
    }
    char* rep = utf8_str_rep(us_w);

    int count = strlen(rep);

    strncpy(btn_str_u, rep, count);

    btn_str_u[count] = '\0';

    //-----

    int result = glyphs_batch_plot(v4, btn_str_u, (int)a2, *textglow_);

    //----
    remove_ascii(us_w);

    us_w_len = wcslen(us_w);
    wcsncpy(btn_str_s+btn_str_s_p,us_w,us_w_len);

    btn_str_s_p += us_w_len;



    if(btn_str_s_p!=btn_str_s_xy_p){
        btn_str_s_p=temp;
        btn_str_s_xy_p=temp_xy;
    }

    int rep_len = 0;
    for (int i = 0; i<result; i++) {
        if (rep[i] == '\xE8') {
            rep_len+=3;
        }else{
            rep_len++;
        }
    }

    free(us_w);

    return rep_len;
    //end
}

char *__cdecl my_wrap_text_ex(const char **a1, int a2, unsigned int a3, int a4){

    //start
    //test
//    reset_jmp_rep((LPVOID) wrap_text_ex, wrap_text_ex_jmp);
//    char * result = wrap_text_ex(a1,a2,a3,a4);
//    jmp_rep((LPVOID) wrap_text_ex, my_wrap_text_ex);
//    return result;
    //end

//    btn_str_u = get_btn_str_u_sub(*a1,false);

    int str_len = strlen(*a1);

    strcpy(btn_str_u, *a1);
    int us_w_len = MultiByteToWideChar(CP_UTF8, 0, btn_str_u, -1, NULL, 0);
    wchar_t* us_w = (wchar_t *) malloc((str_len+10) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, btn_str_u, -1, us_w, us_w_len);

    char *cn = (char *) malloc((str_len*3+10) * sizeof(char));
    int cn_len = 0;

    int str_c_index = 0;
    for(int i=0;i<us_w_len-1;i++){

        if(us_w[i]<256){
            str_c_index++;
        }else if(us_w[i]==0xFFFD){
            if('\xFF'>=btn_str_u[str_c_index] && btn_str_u[str_c_index]=='\xF0' && '\xBF'>=btn_str_u[str_c_index+1] && btn_str_u[str_c_index+1]>='\x80'){
                wchar_t * s = us_w;
                int pos = i;
                int len = us_w_len-1;
                wmemmove(&s[pos + 1], &s[pos], len - pos + 1);

                us_w[i] = (unsigned char)btn_str_u[str_c_index];  // 如果你想在移动的位置插入特定的字符，可以在这里指定

                us_w_len++;
                str_c_index++;
                if(us_w[i]==0xE8){
                    us_w[i]='?';
                }
                i++;

                us_w[i]=(unsigned char)btn_str_u[str_c_index];
                str_c_index++;
            }else{
                us_w[i]=(unsigned char)btn_str_u[str_c_index];
                str_c_index++;
            }
        }else{
            cn[cn_len++] = btn_str_u[str_c_index++];
            cn[cn_len++] = btn_str_u[str_c_index++];
            cn[cn_len++] = btn_str_u[str_c_index++];
//            str_c_index+=3;
        }
        if(us_w[i]==0xE8){
            us_w[i]='?';
        }
    }
    char* rep = utf8_str_rep(us_w);

    reset_jmp_rep((LPVOID) wrap_text_ex, wrap_text_ex_jmp);
    char * result = wrap_text_ex(&rep,a2,a3,a4);
    jmp_rep((LPVOID) wrap_text_ex, my_wrap_text_ex);

    int rep_result_len = 0;
    int len =0;
    if(result!=NULL){
        len= strlen(result);
    }
    for (int i = 0; i<len; i++) {
        if (result[i] == '\xE8') {
            rep_result_len+=3;
        }else{
            rep_result_len++;
        }
    }

    int rep_len = 0;
    len = strlen(rep);
    for (int i = 0; i<len; i++) {
        if (rep[i] == '\xE8') {
            rep_len+=3;
        }else{
            rep_len++;
        }
    }

    if(rep_result_len!=0){
        strcpy(str_result_temp, result);
        len = strlen(str_result_temp);
        int cn_idx = 0;
        for (int i = 0; i<len; i++) {
            if (str_result_temp[i] == '\xE8') {
                //wmemmove(&s[pos + 1], &s[pos], len - pos + 1);
                memmove(&str_result_temp[i + 2], &str_result_temp[i], len - i + 2);
                len+=3;
                str_result_temp[i++] = cn[cn_idx++];
                str_result_temp[i++] = cn[cn_idx++];
                str_result_temp[i] = cn[cn_idx++];
            }
        }
    }
    else{
        str_result_temp[0]='\0';
    }

    *a1+=str_len-rep_len;

    free(us_w);
    free(cn);

    if(result==NULL){
        return result;
    }

    return str_result_temp;
}

int __cdecl my_team_btn_char_stats(int a1, int a2)
{
    long double v2; // fst5
    int selected_charid; // eax
    long double v4; // fst7
    int v5; // ebx
    long double v6; // fst7
    const char *Str; // esi
    signed int v8; // edi
    int i; // ebp
    int v10; // esi
    size_t Count; // edi
    const char *v12; // eax
    int v14; // eax
    int v15; // [esp+1Ch] [ebp-200h]
    int v16; // [esp+1Ch] [ebp-200h]
    int v17; // [esp+20h] [ebp-1FCh]
    float v18; // [esp+24h] [ebp-1F8h]
    unsigned int v19; // [esp+24h] [ebp-1F8h]
    int safe; // [esp+28h] [ebp-1F4h]
    float v21; // [esp+2Ch] [ebp-1F0h]
    int v22; // [esp+30h] [ebp-1ECh]
    float v23; // [esp+34h] [ebp-1E8h]
    char v24[96]; // [esp+40h] [ebp-1DCh] BYREF
    char v25[96]; // [esp+A0h] [ebp-17Ch] BYREF
    char Destination[284]; // [esp+100h] [ebp-11Ch] BYREF

    if ( a2 != 5 )
    {
        if ( a2 == 7 )
        {
            v2 = *(float *)(a1 + 32) / *global_scale - 12.0;
            v15 = (int)(*(float *)(a1 + 44) / *global_scale - 12.0);
            v18 = glyph_h((DWORD*)font6x8, 88);
            memcpy(v24, turtle, sizeof(v24));
            selected_charid = get_selected_charid();
            safe = (int)chara_get_safe(selected_charid);
            v4 = (long double)((int)v2 / 3);
            v21 = v4;
            v5 = (int)(v4 - 24.0);
            memcpy(v25, turtle, sizeof(v25));
            memcpy(turtle, v25, 0x60u);
            turtle_trans(6.0, (double)v15);
            memcpy(v25, turtle, sizeof(v25));
            v6 = -(v18 + 1.0);
            v23 = v6;
            turtle_trans(0.0 * v21, v6 * 0.0);
            v17 = 0;
            v19 = 9;
            v22 = v5 + 8;
            while ( 1 )
            {
                Str = my_chara_stat_name(v19);
                v8 = strlen(Str);
                if ( v8 <= 0 )
                {
                    Count = 1;
                    v10 = 1;
                    i = 0;
                }
                else
                {
                    v16 = 0;
                    for ( i = 0; i != v8; ++i )
                    {
                        v16 = (int)(glyph_w((DWORD*)font6x8, Str[i]) + (long double)v16);
                        if ( v5 < v16 )
                            break;
                    }
                    v10 = i + 1;
                    Count = i + 1;
                }
                v12 = (char*)my_chara_stat_name(v19);
                strncpy(Destination, v12, Count);
                if ( Destination[i] )
                {
                    Destination[i] = 46;
                    Destination[v10] = 0;
                }
                if ( chara_stat_known(safe, v19) )
                {
                    plot_text(Destination, 0);
                    turtle_trans((double)v22, 0.0);
                    v14 = chara_effective_stat(safe, v19);
                    game_plot_stat_smiley(v14);
                }
                else
                {
                    *flt_4D0110 = *flt_4D0110 * 0.25;
                    *flt_4D0114 = 0.25 * *flt_4D0114;
                    *flt_4D0118 = 0.5 * *flt_4D0118;
                    plot_text(Destination, 0);
                    turtle_trans((double)v22, 0.0);
                    plot_text("?", (char *)1);
                }
                ++v17;
                memcpy(turtle, v25, 0x60u);
                if ( v17 == 10 )
                    break;
                v19 = display_order[v17];
                turtle_trans((long double)(v17 % 3) * v21, (long double)(v17 / 3) * v23);
                if ( v17 == 9 )
                    turtle_trans(v21, 0.0);
            }
            memcpy(turtle, v24, 0x60u);
        }
        return main_btn_image_wrap(a1, a2);
    }
    if ( *dword_A0A534 == *old_selected )
        return main_btn_image_wrap(a1, a2);
    main_btn_image_wrap(a1, 5);
    *old_selected = *dword_A0A534;
    main_tabscreen_refresh();
    return 1;
}

const char *__cdecl my_chara_stat_name(unsigned int a1)
{
    if ( a1 > 0xC )
        return "invalid";
    else
        return (const char *)my_chara_stat_name_part_0(a1);
}

int my_chara_stat_name_part_0(int a1)
{
    int v2[13]; // [esp+Ch] [ebp-34h]

    v2[0] = (int) _("morale");
    v2[2] = (int) _("composure");
    v2[3] = (int) _("charm");
    v2[4] = (int) _("wits");
    v2[1] = (int) _("attitude");
    v2[5] = (int) _("loyalty");
    v2[9] = (int) _("strength");
    v2[11] = (int) _("fitness");
    v2[10] = (int) _("dexterity");
    v2[6] = (int) _("medical");
    v2[7] = (int) _("mechanical");
    v2[8] = (int) _("shooting");
    v2[12] = (int) _("vitality");
    return v2[a1];
}

int my_customize_layout()
{
    int custom; // ebx
    int v1; // eax
    int v2; // eax
    int v3; // eax
    int v4; // eax
    int v5; // esi
    int v6; // eax
    int v7; // eax
    int v8; // esi
    int v9; // ebp
    int v10; // eax
    DWORD *v11; // eax
    DWORD *v12; // eax
    int v13; // ebp
    int v14; // eax
    DWORD *v15; // eax
    DWORD *v16; // eax
    int v17; // ebp
    int v18; // eax
    DWORD *v19; // eax
    int v20; // edi
    int v21; // eax
    int v22; // ebx
    int v23; // ebp
    int v24; // eax
    DWORD *v25; // eax
    int v26; // edi
    int v27; // eax
    DWORD *v28; // eax
    int v29; // ebx
    int v30; // ebx
    int v31; // eax
    int v32; // ebx
    int v33; // eax
    int result; // eax
    DWORD *v35; // eax
    int v36; // ebp
    int v37; // eax
    DWORD *v38; // eax
    int v39; // edi
    int v40; // ebx
    DWORD *v41; // eax
    int v42; // edi
    int v43; // eax
    DWORD *v44; // eax
    char *Str; // [esp+0h] [ebp-5Ch]
    float v46; // [esp+4h] [ebp-58h]
    float v47; // [esp+4h] [ebp-58h]
    float v48; // [esp+4h] [ebp-58h]
    float v49; // [esp+4h] [ebp-58h]
    float v50; // [esp+4h] [ebp-58h]
    char v51[44]; // [esp+30h] [ebp-2Ch] BYREF

    custom = (int)chara_get_custom(*dword_71D248);
    cursor_restore();
    button_set_layout(6.0, 8.0);
    v1 = (int)button_ex(0.0, 1.0, *dword_71D248, 0, (int)customize_btn_char_panel);
    *(DWORD *)(v1 + 232) = 0x0071D100;
    v46 = 3.0 * *(float *)(v1 + 36);
    button_set_h(v1, v46);
    button_set_layout(6.0, 8.0);
    button_ex(1.0, 0.5, 0, (int)_("HEAD"), (int)btn_char_edit_mode);
    button_ex(1.0, 1.5, 1, (int)_("BODY"), (int)btn_char_edit_mode);
    button_set_layout(6.0, 8.0);
    v2 = button(2.0, 0.0, 0, (int)_("NAME"));
    labelify(v2);
    v3 = button(2.0, 1.0, 0, (int)_("PERK"));
    labelify(v3);
    v4 = button(2.0, 2.0, 0, (int)_("TRAIT"));
    labelify(v4);
    button_set_layout(6.0, 8.0);
    v5 = (int)button_ex(4.0, 0.0, 0, custom + 28, (int)main_btn_edit_focus);
    *(DWORD *)(v5 + 256) = strlen((const char *)(custom + 28));
    v47 = 3.0 * *(float *)(v5 + 32);
    button_set_w(v5, v47);
    v6 = (int)button_ex(4.0, 1.0, 0, 0, (int)btn_select_perk);
    *(DWORD *)(v6 + 296) = 1082130432;
    v48 = 3.0 * *(float *)(v6 + 32);
    button_set_w(v6, v48);
    v7 = (int)button_ex(4.0, 2.0, 0, 0, (int)btn_select_trait);
    *(DWORD *)(v7 + 296) = 1082130432;
    v49 = 3.0 * *(float *)(v7 + 32);
    button_set_w(v7, v49);
    if ( *char_edit_mode )
    {
        button_set_layout(1.0, 8.0);
        v8 = (int)button_ex(0.0, 3.0, 0, (int)_("BODY SETTINGS"), (int)main_btn_default);
        button_set_layout(10.0, 8.0);
        v35 = (DWORD *)my_selection_constprop_5((int)_("\x12 GENDER %d \x12"), 0, 4, custom + 148, 2, *dword_71D248);
        v35[68] = custom + 148;
        v35[57] = (DWORD)btn_roll_datafunc;
        v35[58] = (DWORD)gender_randfunc;
        v35[59] = 1;
        v36 = *dword_71D248;
        v37 = char_body_count(*(unsigned __int16 *)(custom + 148));
        v38 = (DWORD *)my_selection_constprop_5((int)_("\x12 TOP %d \x12"), 1, 4, custom + 192, v37, v36);
        v38[68] = custom + 192;
        v38[57] = (DWORD)btn_roll_datafunc;
        v39 = custom + 188;
        v38[58] = (DWORD)tops_randfunc;
        v38[59] = 1;
        Str = (char *)(custom + 188);
        v40 = custom + 194;
        v41 = (DWORD *)my_selection_constprop_5((int)_("\x12 SIZE %d \x12"), 0, 5, (int)Str, 4, *dword_71D248);
        v41[68] = v39;
        v41[57] = (DWORD)btn_roll_datafunc;
        v41[58] = (DWORD)bodysize_randfunc;
        v41[59] = 1;
        v42 = *dword_71D248;
        v43 = char_body_count(*(unsigned __int16 *)(v40 - 46));
        v44 = (DWORD *)my_selection_constprop_5((int)_("\x12 BOTTOMS %d \x12"), 1, 5, v40, v43, v42);
        v44[57] = (DWORD)btn_roll_datafunc;
        v44[58] = (DWORD)bottoms_randfunc;
        v44[68] = v40;
        v44[59] = 1;
    }
    else
    {
        button_set_layout(1.0, 8.0);
        v8 = (int)button_ex(0.0, 3.0, 0, (int)_("HEAD SETTINGS"), (int)main_btn_default);
        button_set_layout(10.0, 8.0);
        v9 = *dword_71D248;
        v10 = char_head_count(*(unsigned __int16 *)(custom + 148));
        v11 = (DWORD *)my_selection_constprop_5((int)_("\x12 FACE %d \x12"), 0, 4, custom + 190, v10, v9);
        v11[68] = custom + 190;
        v11[57] = (DWORD)btn_roll_datafunc;
        v11[58] = (DWORD)face_randfunc;
        v11[59] = 1;
        v12 = (DWORD *)my_selection_constprop_5((int)_("\x12 SKIN CLR %d \x12"), 1, 4, custom + 216, *skin_pal, *dword_71D248);
        v12[68] = custom + 216;
        v12[57] = (DWORD)btn_roll_datafunc;
        v12[58] = (DWORD)skinclr_randfunc;
        v12[59] = 1;
        v13 = *dword_71D248;
        v14 = char_hair_count(*(unsigned __int16 *)(custom + 148));
        v15 = (DWORD *)my_selection_constprop_5((int)_("\x12 HAIR %d \x12"), 0, 5, custom + 198, v14 + 1, v13);
        v15[68] = custom + 198;
        v15[57] = (DWORD)btn_roll_datafunc;
        v15[58] = (DWORD)hair_randfunc;
        v16 = (DWORD *)my_selection_constprop_5((int)_("\x12 HAIR CLR %d \x12"), 1, 5, custom + 218, *hair_pal, *dword_71D248);
        v16[68] = custom + 218;
        v16[57] = (DWORD)btn_roll_datafunc;
        v16[58] = (DWORD)hairclr_randfunc;
        v16[59] = 1;
        v17 = *dword_71D248;
        v18 = char_hat_count(*(unsigned __int16 *)(custom + 148));
        v19 = (DWORD *)my_selection_constprop_5((int)_("\x12 HAT %d \x12"), 0, 6, custom + 200, v18 + 1, v17);
        v19[68] = custom + 200;
        v19[57] = (DWORD)btn_roll_datafunc;
        v20 = custom + 202;
        v19[58] = (DWORD)hat_randfunc;
        v21 = *(unsigned __int16 *)(custom + 148);
        v22 = custom + 196;
        v23 = *dword_71D248;
        v24 = char_glasses_count(v21);
        v25 = (DWORD *)my_selection_constprop_5((int)_("\x12 SHADES %d \x12"), 1, 6, v20, v24 + 1, v23);
        v25[68] = v20;
        v25[57] = (DWORD)btn_roll_datafunc;
        v25[58] = (DWORD)shades_randfunc;
        v26 = *dword_71D248;
        v27 = char_beard_count(*(unsigned __int16 *)(v22 - 48));
        v28 = (DWORD *)my_selection_constprop_5((int)_("\x12 EXTRA %d \x12"), 0, 7, v22, v27 + 1, v26);
        v28[57] = (DWORD)btn_roll_datafunc;
        v28[58] = (DWORD)beard_randfunc;
        v28[68] = v22;
    }
    v50 = 1.5 * *(float *)(v8 + 32);
    button_set_w(v8, v50);
    *(BYTE *)(v8 + 190) = -1;
    *(DWORD *)(v8 + 76) = 1065353216;
    *(float *)(v8 + 64) = 0.25;
    *(DWORD *)(v8 + 8) = *TITLE_BAR_SPRITE;
    *(float *)(v8 + 68) = 0.25;
    *(float *)(v8 + 72) = 0.25;
    game_lower_box(v51);
    button_set_layout(3.0, 1.0);
    v29 = (int)button_ex(0.0, 0.0, 0, (int)_("Random"), (int)btn_roll_char);
    iconify(v29);
    *(DWORD *)(v29 + 276) = *icons16_id + 10;
    v30 = (int)button_ex(1.0, 0.0, 0, (int)_("Save"), (int)main_btn_image_push_state);
    iconify(v30);
    v31 = *icons16_id;
    *(DWORD *)(v30 + 224) = charsave_state;
    *(DWORD *)(v30 + 276) = v31 + 7;
    v32 = (int)button_ex(2.0, 0.0, 0, (int)_("Load"), (int)main_btn_image_push_state);
    iconify(v32);
    v33 = *icons16_id;
    *(DWORD *)(v32 + 224) = charload_state;
    result = v33 + 6;
    *(DWORD *)(v32 + 276) = result;
    return result;
}
    
int my_selection_constprop_5(int a1, int a2, int a3, int a4, int a5, int a6){
    /*printf("%d,",a1);
    reset_jmp_rep((LPVOID) selection_constprop_5, selection_constprop_5_jmp);
    int result = selection_constprop_5(a1,a2,a3,a4,a5,a6);
    jmp_rep((LPVOID) selection_constprop_5, my_selection_constprop_5);
    return result;*/
    int v6; // ebx
    DWORD *v8; // edi
    int v9; // esi
    DWORD *v10; // ebx
    float v12; // [esp+0h] [ebp-3Ch]
    float v13; // [esp+0h] [ebp-3Ch]
    float v14; // [esp+0h] [ebp-3Ch]
    float v15; // [esp+4h] [ebp-38h]
    float v16; // [esp+4h] [ebp-38h]
    float v17; // [esp+1Ch] [ebp-20h]

    int off_4D3115 = 0x004D3115;
    int (__cdecl *btn_rotate_uint16_val)(int a1, int a2) = (int (__cdecl *)(int a1, int a2))0x0042B7B0;
    float *(__cdecl *game_colour_swap)(float *a1) = (float *(__cdecl *)(float *a1))0x0044DFD0;
    int (*validate_body_reenter)() = (int (*)())0x0042B620;
    int (__cdecl *btn_rotate_uint16_val_dec)(DWORD *a1, int a2) = (int (__cdecl *)(DWORD *a1, int a2))0x0042B720;
    int (__cdecl *btn_rotate_uint16_val_inc)(DWORD *a1, int a2) = (int (__cdecl *)(DWORD *a1, int a2))0x0042B6A0;

    v6 = 5 * a2;
    v15 = (float)a3;
    v17 = v15;
    v12 = (float)(5 * a2);
    v8 = (DWORD *)button(v12, v15, 0, off_4D3115);
    v8[58] = a4;
    v8[57] = (DWORD)btn_rotate_uint16_val;
    v8[63] = a5;
    button_init((int)v8);
    game_colour_swap(v8);
    v8[62] = (DWORD)validate_body_reenter;
    v8[57] = (DWORD)btn_rotate_uint16_val_dec;
    v8[1] = a6;
    v13 = (float)(v6 + 2);
    v9 = button(v13, v17, 0, a1);
    v16 = 3.0 * *(float *)(v9 + 32);
    button_set_w(v9, v16);
    v14 = (float)(v6 + 4);
    v10 = (DWORD *)button(v14, v17, 0, 5058839);
    v10[58] = a4;
    v10[57] = (DWORD)btn_rotate_uint16_val;
    v10[63] = a5;
    button_init((int)v10);
    game_colour_swap(v10);
    v10[62] = (DWORD)validate_body_reenter;
    v10[57] = (DWORD)btn_rotate_uint16_val_inc;
    v10[1] = a6;
    return v9;
}


//end

//start
//逆向分析
int __cdecl my_sprite_batch_plot(int a1, int a2, int a3)
{
    int (*quad_batch)() = (int (*)())0x00406120;
    double *dbl_4D00D8 = (double *)0x004D00D8;

    int v3; // esi
    int v4; // edx
    int v5; // eax
    int v6; // eax
    int v7; // eax
    long double v8; // fst6
    int v9; // eax
    int result; // eax
    char v11[96]; // [esp+20h] [ebp-7Ch] BYREF

    memcpy(v11, turtle, sizeof(v11));
    v3 = *(DWORD *)(a1 + 28) + 28 * a3;
    if ( *(DWORD *)(v3 + 32) )
    {
        v4 = *(DWORD *)(v3 + 36);
        v5 = *(DWORD *)(v3 + 44);
        if ( v4 >= v5 )
        {
            *(DWORD *)(v3 + 36) = v5;
        }
        else
        {
            v6 = *(DWORD *)(v3 + 40);
            if ( v4 >= v6 )
            {
                do
                {
                    v7 = v6 + 1024;
                    *(DWORD *)(v3 + 40) = v7;
                    *(DWORD *)(v3 + 32) = (DWORD)realloc(*(void **)(v3 + 32), v7 << 7);
                    v6 = *(DWORD *)(v3 + 40);
                }
                while ( v6 <= *(DWORD *)(v3 + 36) );
            }
        }
    }
    else
    {
        *(DWORD *)(v3 + 40) = 1024;
        *(DWORD *)(v3 + 44) = 0x10000;
        *(DWORD *)(v3 + 32) = (DWORD)calloc(1u, 0x20000u);
    }
    v8 = 1.0;
    if ( a2 )
        v8 = -1.0;
    v9 = -*(__int16 *)(a1 + 22);
    *dbl_4D00D8 = *dbl_4D00D8 * v8;
    turtle_trans((double)*(__int16 *)(a1 + 20), (double)v9);
    result = quad_batch();
    memcpy(turtle, v11, 0x60u);
    return result;
}
//end

// DLL被加载、卸载时调用
BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            //start
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
            //end

            //start
            //gettext
            //注释这段解决游戏内%s格式化错误
//            setlocale(LC_ALL, "");
            bindtextdomain ("SDL2_mixer", "locale");
            textdomain ("SDL2_mixer");
            bind_textdomain_codeset("SDL2_mixer", "UTF-8");
            //end

            //start
            //命令行显示和初始化
//            AllocConsole();
//            freopen("CONOUT$", "w", stdout);
            btn_str_u = (char *) malloc(12285 * sizeof(char));
            str_result_temp = (char *) malloc(12285 * sizeof(char));
            str_rep_temp = (char *) malloc(12285 * sizeof(char));
            //end

            //start
            //utf8文本记录和文本显示
            jmp_rep((LPVOID) real_SDL_GL_SwapWindow, my_SDL_GL_SwapWindow);
            jmp_rep((LPVOID) glyphs_batch_plot_ex, my_glyphs_batch_plot_ex);
            jmp_rep((LPVOID) glyphs_set_callback, my_glyphs_set_callback);
            //end

            //start
            //utf8文本和原文的替换逻辑
            jmp_rep((LPVOID) plot_text_ex, my_plot_text_ex);

            memcpy(wrap_text_ex_jmp, (void *) wrap_text_ex, 5);
            jmp_rep((LPVOID) wrap_text_ex, my_wrap_text_ex);
            //end

            //start
            //代码内部文本翻译
            jmp_rep((LPVOID) customize_layout, my_customize_layout);
            jmp_rep((LPVOID) team_btn_char_stats, my_team_btn_char_stats);
            //end

            break;
        case DLL_PROCESS_DETACH:
            // 手动卸载原DLL
            font_free();
            FreeLibrary(SDL2_mixer_module);
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}