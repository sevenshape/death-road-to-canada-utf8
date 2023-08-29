#include <SDL.h>
#include <SDL_opengl.h>
#include "NERVE SEP-27-2022.07.43.h"
#include "SDL_mixer.h"

#include <libintl.h>
#include <iconv.h>
#include <png.h>

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
int btn_str_s_xy_p = 0;

char *str_rep_temp;

char *str_result_temp;

//start
//font

GLuint textureID;

typedef struct {
    unsigned char r, g, b, a;
} RGBA;

typedef struct {
    int width, height;
    RGBA* data;
} Texture2D;

Texture2D getTextureData(GLuint textureId, int width, int height) {
    Texture2D texture;
    texture.width = width;
    texture.height = height;
    texture.data = (RGBA*)malloc(width * height * sizeof(RGBA));

    // 绑定纹理，以便我们可以读取它
    glBindTexture(GL_TEXTURE_2D, textureId);

    // 获取纹理数据
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);

    // 释放绑定
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void copyTextureSubData(Texture2D* srcTexture, Texture2D* dstTexture, int srcX, int srcY, int dstX, int dstY, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            RGBA srcPixel = srcTexture->data[(srcY + y) * srcTexture->width + (srcX + x)];
            dstTexture->data[(dstY + y) * dstTexture->width + (dstX + x)] = srcPixel;
        }
    }
}

void font_set(int atlas)
{
    Texture2D dstTexture;
    dstTexture.width = atlas_width+3072;
    dstTexture.height = 3072;
    dstTexture.data = (RGBA*)calloc(dstTexture.width * dstTexture.height,sizeof(RGBA));
    Texture2D srcTexture = getTextureData(atlas,atlas_width,atlas_height);
    copyTextureSubData(&srcTexture,&dstTexture,0,0,0,0,atlas_width-1, atlas_height);
    GLuint font = load_texture("./gfx/fonts/font_ucs2_12x12.png");
    Texture2D fontTexture = getTextureData(font,3072,3072);
    copyTextureSubData(&fontTexture,&dstTexture,0,0,atlas_width,0,3072, 3072);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dstTexture.width, dstTexture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dstTexture.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void font_free()
{
    // 释放字体

}

GLuint load_texture(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    png_init_io(png_ptr, file);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);

    png_bytepp rows = png_get_rows(png_ptr, info_ptr);

    GLubyte *imageData = malloc(4 * width * height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            png_bytep row = rows[y];
            png_bytep px = &(row[x * 4]);
            imageData[(y * width + x) * 4] = px[0];
            imageData[(y * width + x) * 4 + 1] = px[1];
            imageData[(y * width + x) * 4 + 2] = px[2];
            imageData[(y * width + x) * 4 + 3] = px[3];
        }
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(file);
    free(imageData);

    return textureID;
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

//start
//utf8 util
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

char* utf8_str_char_rep(const char * s, size_t s_len) {
    int len = 0;
    for(int i=0;i<s_len;){
        wchar_t ucs2 = ((unsigned char)s[i] << 8) | (unsigned char)s[i+1];
        if (ucs2 < 256) {
            str_rep_temp[len] = (char)ucs2;
        }else{
            str_rep_temp[len] = '\xE8';
        }

        len++;
        i+=2;
    }
    str_rep_temp[len] = '\0';

    return str_rep_temp;
}


int convert_encoding(const char *from_charset, const char *to_charset, char *input, size_t input_len, char **output, size_t *output_len) {
    iconv_t conv;
    char *output_ptr;
    size_t initial_output_len, original_input_len;

    conv = iconv_open(to_charset, from_charset);
    if (conv == (iconv_t)-1) {
        perror("iconv_open");
        return -1;
    }

    initial_output_len = input_len * 4;  // Roughly estimate the required space.
    *output = malloc(initial_output_len);

    if (*output == NULL) {
        perror("malloc");
        iconv_close(conv);
        return -1;
    }

    output_ptr = *output;
    original_input_len = input_len;

    while (input_len > 0) {
        if (iconv(conv, &input, &input_len, &output_ptr, &initial_output_len) == (size_t)-1) {
            if (errno == EILSEQ) {
                // Invalid sequence found, copy one byte from input to output and move on.
                *output_ptr++ = 0;
                *output_ptr++ = *input++;
                input_len--;
                initial_output_len-=2;
            } else if(errno == EINVAL){
                // 最后一个字节是一个不完整的多字节字符序列
                *output_ptr++ = 0;
                *output_ptr++ = *input++;
                input_len--;
                initial_output_len-=2;
            } else {
                // Some other error occurred.
                perror("iconv");
                free(*output);
                iconv_close(conv);
                return -1;
            }
        }
    }

    *output_len = output_ptr - *output;  // Calculate the actual size of the converted string.

    iconv_close(conv);
    return 0;
}
//end

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
                v16 = v21 + 32 * v14; //sprite_get(*a1) + 32 * (char - a1[1]) sprite_get(*a1)是字符显示sprite指针, 默认为6451136变量名(sprites_). a1[1]默认为0
                if (v13)
                    turtle_trans((long double) *(__int16 *) (v16 + 12) * 0.5, 0.0);//前后x偏移纹理宽度的0.5保证纹理在上中央渲染
                //start
                //分别渲染中文和英文字符
                if (a2[v13] == 0xE8 && utf8_plot)
                {
                    my_sprite_batch_plot_ucs4(v21 + 32 * 0x10, 0, a5,btn_str_s[btn_str_s_xy_p]);

                    btn_str_s_xy_p++;
                } else{
                    sprite_batch_plot(v16, 0, a5);
                }
                //end
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
    int v4; // edx
    float v6; // [esp+Ch] [ebp-90h]
    char v7[96]; // [esp+20h] [ebp-7Ch] BYREF

    v4 = a4;

    //start
    int str_len = strlen(a1);

    char *ucs2_result;
    size_t ucs2_len;
    int err = convert_encoding("UTF-8", "UCS-2", a1, str_len, &ucs2_result, &ucs2_len);
    if(err!=0){
        sprintf(msg,"convert_encoding error");
        MessageBox(NULL, msg, "", MB_OK);
    }

    int len=0;
    int str_s_len = 0;
    for(int i=0;i<ucs2_len;){
        wchar_t ucs2 = ((unsigned char)ucs2_result[i] << 8) | (unsigned char)ucs2_result[i+1];
        if (ucs2 < 256) {
            str_rep_temp[len] = (char)ucs2;
        }else{
            str_rep_temp[len] = '\xE8';
            btn_str_s[btn_str_s_p+str_s_len] = ucs2;
            str_s_len++;
        }

        len++;
        i+=2;
    }
    str_rep_temp[len] = '\0';
    btn_str_s_p+=str_s_len;

    //start
    //显示逻辑
    utf8_plot = TRUE;
    //阴影字体
    int xy_p_bak = btn_str_s_xy_p;
    if ( a3 )
    {
        *glypher_shadow = 1;
        memcpy(v7, turtle, sizeof(v7));
        v6 = *flt_4D011C * *flt_4D0498;
        turtle_set_rgba(*shadow, *flt_4D0490, *flt_4D0494, v6);
        turtle_trans(1.0, -1.0);
        glyphs_batch_plot(a4, str_rep_temp, (int)a2, *textglow_);
        v4 = a4;
        *glypher_shadow = 0;
        memcpy(turtle, v7, 0x60u);
    }
    btn_str_s_xy_p = xy_p_bak;
    //标准字体
    int result = glyphs_batch_plot(v4, str_rep_temp, (int)a2, *textglow_);
    utf8_plot = FALSE;
    //end
    

    int rep_len = 0;
    for (int i = 0; i<result; i++) {
        if (str_rep_temp[i] == '\xE8') {
            rep_len+=3;
        }else{
            rep_len++;
        }
    }

    free(ucs2_result);

    return rep_len;
    //end
}

char *__cdecl my_wrap_text_ex(const char **a1, int a2, unsigned int a3, int a4)
{
    int str_len = strlen(*a1);

    strcpy(btn_str_u, *a1);

    char *ucs2_result;
    size_t ucs2_len;
    int err = convert_encoding("UTF-8", "UCS-2", btn_str_u, str_len, &ucs2_result, &ucs2_len);
    if(err!=0){
        sprintf(msg,"convert_encoding error");
        MessageBox(NULL, msg, "", MB_OK);
    }

    char *cn = (char *) malloc((str_len*3+10) * sizeof(char));
    int cn_len = 0;


    for(int i=0;i<ucs2_len;){
        wchar_t ucs2 = ((unsigned char)ucs2_result[i] << 8) | (unsigned char)ucs2_result[i+1];
        if (ucs2 < 256) {

        }else{
            if (ucs2 < 0x0800 || ucs2 > 0xFFFF)
            {

            }else{
                cn[cn_len] = 0xE0 | ((ucs2 >> 12) & 0x0F);        // 1110xxxx
                cn[cn_len+1] = 0x80 | ((ucs2 >> 6) & 0x3F);         // 10xxxxxx
                cn[cn_len+2] = 0x80 | (ucs2 & 0x3F);                // 10xxxxxx
                cn_len+=3;
            }
        }

        i+=2;
    }
    char* rep = utf8_str_char_rep(ucs2_result,ucs2_len);

    int len;

    //计算处理前的rep长度
    int rep_len = 0;
    len = strlen(rep);
    for (int i = 0; i<len; i++) {
        if (rep[i] == '\xE8') {
            rep_len+=3;
        }else{
            rep_len++;
        }
    }

    reset_jmp_rep((LPVOID) wrap_text_ex, wrap_text_ex_jmp);
    char * result = wrap_text_ex(&rep,a2,a3,a4);
    jmp_rep((LPVOID) wrap_text_ex, my_wrap_text_ex);

    int rep_result_len = 0;
    len =0;
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

    //计算处理后的rep长度
    int rep_warp_len = 0;
    len = strlen(rep);
    for (int i = 0; i<len; i++) {
        if (rep[i] == '\xE8') {
            rep_warp_len+=3;
        }else{
            rep_warp_len++;
        }
    }

    //还原rep
    if(rep_result_len!=0){
        strcpy(str_result_temp, result);
        len = strlen(str_result_temp);
        int cn_idx = 0;
        for (int i = 0; i<len; i++) {
            if (str_result_temp[i] == '\xE8') {
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

    *a1 += rep_len - rep_warp_len;

    free(ucs2_result);
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
    float v23;
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
int __cdecl my_sprite_batch_plot(int a1, int a2, int a3)
{
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
    result = my_quad_batch(a1,a3);
    memcpy(turtle, v11, 0x60u);
    return result;
}

int __cdecl my_quad_batch(int a1, int a2)
{
int v2; // ebp
DWORD *v3; // edx
__int16 v4; // si
__int16 v5; // di
__int16 v6; // bx
long double v7; // fst7
__int16 v8; // cx
__int16 v9; // ax
long double v10; // fst6
long double v11; // fst5
long double v12; // fst4
long double v13; // fst3
int v14; // ebp
long double v15; // fst2
int result; // eax
long double v17; // fst1
long double v18; // fst4
long double v19; // rt0
long double v20; // rt1
__int16 v21; // bp
long double v22; // fst1
long double v23; // fst3
long double v24; // fst1
long double v25; // fst5
long double v26; // rt1
long double v27; // fst1
long double v28; // fst6
__int16 v29; // di
__int16 v30; // [esp+2h] [ebp-4Ah]
float v31; // [esp+4h] [ebp-48h]
float v32; // [esp+8h] [ebp-44h]
float v33; // [esp+Ch] [ebp-40h]
float v34; // [esp+10h] [ebp-3Ch]
float v35; // [esp+14h] [ebp-38h]
float v36; // [esp+18h] [ebp-34h]
float v37; // [esp+1Ch] [ebp-30h]
float v38; // [esp+20h] [ebp-2Ch]
float v39; // [esp+24h] [ebp-28h]
float v40; // [esp+28h] [ebp-24h]
__int16 v41; // [esp+2Ch] [ebp-20h]
float v42; // [esp+34h] [ebp-18h]
float v43; // [esp+34h] [ebp-18h]
float v44; // [esp+34h] [ebp-18h]
float v45; // [esp+34h] [ebp-18h]
float v46; // [esp+34h] [ebp-18h]

v2 = *(DWORD *)(a1 + 28);//v2固定为5287968
v3 = (DWORD *)(v2 + 28 * a2);//a2默认为0
v34 = (float)*(int *)(v2 + 8);
v4 = (__int16)(*flt_4D0110 * 255.0);
v5 = *(WORD *)a1;
v7 = (long double)*(__int16 *)(a1 + 2);
v8 = *(WORD *)(a1 + 4);
v9 = *(WORD *)(a1 + 6);
v6 = (__int16)(*flt_4D0114 * 255.0);
v30 = (__int16)(*flt_4D0118 * 255.0);
v41 = (__int16)(255.0 * *flt_4D011C);

v37 = (long double)v9 + v7;//纹理高度
v42 = (long double)v8 * 0.5 * *dbl_4D00D8;
v10 = v42;
v31 = 0.5 * (long double)v9 * *dbl_4D00E0;
v43 = *dbl_4D0108;
v11 = v43;
v38 = -v43;
v44 = *dbl_4D0100;
v12 = v44;
v35 = *dbl_4D00F8;
v45 = *dbl_4D00F0;
v13 = v45;
v39 = -v45;
v32 = *turtle;
v33 = *dbl_4D00C8;
v46 = *dbl_4D00D0;
v14 = v3[9];
if ( v14 >= v3[10] )
return 0;
v15 = v11 * v31;
result = v3[8] + (v14 << 7);//v3[8]是开辟的动态内存, 每个字符站128 byte
v3[9] = v14 + 1;
v17 = -v10 * v35;
v40 = v12;
v18 = -v31 * v12;
v19 = v17;
v20 = v13 * v10;
v21 = (__int16)(v34 - v7);
v22 = v38 * v31;
v23 = v22;
v24 = v31 * v40;
v25 = v20 + v24;
v26 = v24;
v27 = v35 * v10;
v28 = v10 * v39;

v36 = (long double)v5 + (long double)v8;
v29 = (__int16)v36;

*(float *)result = v15 + v17 + v32;
*(float *)(result + 4) = v18 + v13 * v10 + v33;
*(float *)(result + 8) = v46;
*(WORD *)(result + 12) = v5;//左下
*(WORD *)(result + 14) = v21;
*(BYTE *)(result + 16) = v4;
*(BYTE *)(result + 17) = v6;
*(BYTE *)(result + 18) = v30;
*(BYTE *)(result + 19) = v41;
*(float *)(result + 32) = v19 + v22 + v32;
*(float *)(result + 36) = v25 + v33;
*(float *)(result + 40) = v46;
*(WORD *)(result + 44) = v5;//左上
*(WORD *)(result + 46) = (__int16)(v34 - v37);
*(BYTE *)(result + 48) = v4;
*(BYTE *)(result + 49) = v6;
*(BYTE *)(result + 50) = v30;
*(BYTE *)(result + 51) = v41;
*(float *)(result + 64) = v15 + v27 + v32;
*(float *)(result + 68) = v18 + v28 + v33;
*(float *)(result + 72) = v46;
*(WORD *)(result + 76) = v29;//右下
*(WORD *)(result + 78) = v21;
*(BYTE *)(result + 80) = v4;
*(BYTE *)(result + 81) = v6;
*(BYTE *)(result + 82) = v30;
*(BYTE *)(result + 83) = v41;
*(float *)(result + 96) = v32 + v27 + v23;
*(float *)(result + 100) = v28 + v26 + v33;
*(float *)(result + 104) = v46;
*(WORD *)(result + 108) = v29;//右上
*(WORD *)(result + 110) = (__int16)(v34 - v37);
*(BYTE *)(result + 112) = v4;
*(BYTE *)(result + 113) = v6;
*(BYTE *)(result + 114) = v30;
*(BYTE *)(result + 115) = v41;

v3[14] = result;
return result;
}

int __cdecl my_sprite_batch_plot_ucs4(int a1, int a2, int a3, int s)
{
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
    result = my_quad_batch_ucs4(a1, a3, s);
    memcpy(turtle, v11, 0x60u);
    return result;
}

int __cdecl my_quad_batch_ucs4(int a1, int a2, int s)
{
    int v2; // ebp
    DWORD *v3; // edx
    __int16 v4; // si
    __int16 v5; // di
    __int16 v6; // bx
    long double v7; // fst7
    __int16 v8; // cx
    __int16 v9; // ax
    long double v10; // fst6
    long double v11; // fst5
    long double v12; // fst4
    long double v13; // fst3
    int v14; // ebp
    long double v15; // fst2
    int result; // eax
    long double v17; // fst1
    long double v18; // fst4
    long double v19; // rt0
    long double v20; // rt1
    __int16 v21; // bp
    long double v22; // fst1
    long double v23; // fst3
    long double v24; // fst1
    long double v25; // fst5
    long double v26; // rt1
    long double v27; // fst1
    long double v28; // fst6
    __int16 v29; // di
    __int16 v30; // [esp+2h] [ebp-4Ah]
    float v31; // [esp+4h] [ebp-48h]
    float v32; // [esp+8h] [ebp-44h]
    float v33; // [esp+Ch] [ebp-40h]
    float v34; // [esp+10h] [ebp-3Ch]
    float v35; // [esp+14h] [ebp-38h]
    float v36; // [esp+18h] [ebp-34h]
    float v37; // [esp+1Ch] [ebp-30h]
    float v38; // [esp+20h] [ebp-2Ch]
    float v39; // [esp+24h] [ebp-28h]
    float v40; // [esp+28h] [ebp-24h]
    __int16 v41; // [esp+2Ch] [ebp-20h]
    float v42; // [esp+34h] [ebp-18h]
    float v43; // [esp+34h] [ebp-18h]
    float v44; // [esp+34h] [ebp-18h]
    float v45; // [esp+34h] [ebp-18h]
    float v46; // [esp+34h] [ebp-18h]

    v2 = *(DWORD *)(a1 + 28);//v2固定为5287968
    v3 = (DWORD *)(v2 + 28 * a2);//a2默认为0
    v34 = (float)*(int *)(v2 + 8);
    v4 = (__int16)(*flt_4D0110 * 255.0);
    v5 = *(WORD *)a1;
    v7 = (long double)*(__int16 *)(a1 + 2);
    v8 = *(WORD *)(a1 + 4);
    v9 = *(WORD *)(a1 + 6);
    v6 = (__int16)(*flt_4D0114 * 255.0);
    v30 = (__int16)(*flt_4D0118 * 255.0);
    v41 = (__int16)(255.0 * *flt_4D011C);

    v37 = (long double)v9 + v7;//纹理高度
    v42 = (long double)v8 * 0.5 * *dbl_4D00D8;
    v10 = v42;
    v31 = 0.5 * (long double)v9 * *dbl_4D00E0;
    v43 = *dbl_4D0108;
    v11 = v43;
    v38 = -v43;
    v44 = *dbl_4D0100;
    v12 = v44;
    v35 = *dbl_4D00F8;
    v45 = *dbl_4D00F0;
    v13 = v45;
    v39 = -v45;
    v32 = *turtle;
    v33 = *dbl_4D00C8;
    v46 = *dbl_4D00D0;
    v14 = v3[9];
    if ( v14 >= v3[10] )
        return 0;
    v15 = v11 * v31;
    result = v3[8] + (v14 << 7);//v3[8]是开辟的动态内存, 每个字符占128 byte
    v3[9] = v14 + 1;
    v17 = -v10 * v35;
    v40 = v12;
    v18 = -v31 * v12;
    v19 = v17;
    v20 = v13 * v10;
    v21 = (__int16)(v34 - v7);
    v22 = v38 * v31;
    v23 = v22;
    v24 = v31 * v40;
    v25 = v20 + v24;
    v26 = v24;
    v27 = v35 * v10;
    v28 = v10 * v39;

    v36 = (long double)v5 + (long double)v8;
    v29 = (__int16)v36;


    int c_offset = s;
    int y_offset = c_offset / 256 * 12;
    int x_offset = c_offset % 256 * 12;

    //坐标取整, 解决像素字体渲染不一致的问题
    v32=floorf(v32);
    v33=floorf(v33);

    *(float *)result = v15 + v17 + v32;
    *(float *)(result + 4) = v18 + v13 * v10 + v33;
    *(float *)(result + 8) = v46;
    *(WORD *)(result + 12) = 2048+x_offset;//左下
    *(WORD *)(result + 14) = y_offset;
    *(BYTE *)(result + 16) = v4;
    *(BYTE *)(result + 17) = v6;
    *(BYTE *)(result + 18) = v30;
    *(BYTE *)(result + 19) = v41;
    *(float *)(result + 32) = v19 + v22 + v32;
    *(float *)(result + 36) = v25 + v33;
    *(float *)(result + 40) = v46;
    *(WORD *)(result + 44) = 2048+x_offset;//左上
    *(WORD *)(result + 46) = 12+y_offset;
    *(BYTE *)(result + 48) = v4;
    *(BYTE *)(result + 49) = v6;
    *(BYTE *)(result + 50) = v30;
    *(BYTE *)(result + 51) = v41;
    *(float *)(result + 64) = v15 + v27 + v32;
    *(float *)(result + 68) = v18 + v28 + v33;
    *(float *)(result + 72) = v46;
    *(WORD *)(result + 76) = 2048+12+x_offset;//右下
    *(WORD *)(result + 78) = y_offset;
    *(BYTE *)(result + 80) = v4;
    *(BYTE *)(result + 81) = v6;
    *(BYTE *)(result + 82) = v30;
    *(BYTE *)(result + 83) = v41;
    *(float *)(result + 96) = v32 + v27 + v23;
    *(float *)(result + 100) = v28 + v26 + v33;
    *(float *)(result + 104) = v46;
    *(WORD *)(result + 108) = 2048+12+x_offset;//右上
    *(WORD *)(result + 110) = 12+y_offset;
    *(BYTE *)(result + 112) = v4;
    *(BYTE *)(result + 113) = v6;
    *(BYTE *)(result + 114) = v30;
    *(BYTE *)(result + 115) = v41;

    v3[14] = result;
    return result;
}

void __cdecl my_sprite_batch_draw(int a1)
{
    WORD *v1; // eax
    int v2; // edx
    __int16 v3; // cx
    __int16 v4; // bx
    __int16 v5; // si
    int v6; // eax

    if ( !*element_init )
    {
        v1 = (WORD *)element_list_0;
        v2 = 0;
        do
        {
            v3 = v2 + 1;
            v4 = v2 + 2;
            v5 = v2 + 3;
            *v1 = v2;
            v1 += 6;
            v2 += 4;
            *(v1 - 5) = v3;
            *(v1 - 4) = v4;
            *(v1 - 3) = v5;
            *(v1 - 2) = v4;
            *(v1 - 1) = v3;
        }
        while ( v1 != (WORD *)unk_626FA0 );
        *element_init = 1;
    }
    if ( a1 && (*(DWORD *)(a1 + 36) || *(DWORD *)(a1 + 64)) )
    {
        glEnableClientState(0x8078u);
        glEnable(0xDE1u);
//        draw_bind_texture((int *)a1);

        if(textureID == 0){
            glBindTexture(GL_TEXTURE_2D,((int*)a1)[3]);

            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &atlas_width);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &atlas_height);

            font_set(((int*)a1)[3]);
            glBindTexture(0xDE1u, textureID);

            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &ft_width);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &ft_height);
        }
        //重新记录utf8字符
        btn_str_s_xy_p = 0;
        btn_str_s_p = 0;

        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        GLfloat x = 1.0f / ft_width;
        GLfloat y = 1.0f / ft_height;
        glScalef(x, y, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glBindTexture(0xDE1u, textureID);
        if ( !*(DWORD *)(a1 + 36) )
        {
            if ( !*(DWORD *)(a1 + 64) )
                return;
            LABEL_8:
            //战斗场景才会用到的渲染方式
            glBlendFunc(0x302u, 1u);
            glVertexPointer(3, 0x1406u, 32, *(const GLvoid **)(a1 + 60));
            glTexCoordPointer(2, 0x1402u, 32, (const GLvoid *)(*(DWORD *)(a1 + 60) + 12));
            glColorPointer(4, 0x1401u, 32, (const GLvoid *)(*(DWORD *)(a1 + 60) + 16));
            glDrawElements(4u, 6 * *(DWORD *)(a1 + 64), 0x1403u, (GLvoid *)element_list_0);
            *(DWORD *)(a1 + 64) = 0;
            glBlendFunc(0x302u, 0x303u);
            return;
        }
        //UI场景的渲染方式, 四边形渲染模式
        glVertexPointer(3, 0x1406u, 32, *(const GLvoid **)(a1 + 32));
        glTexCoordPointer(2, 0x1402u, 32, (const GLvoid *)(*(DWORD *)(a1 + 32) + 12));
        glColorPointer(4, 0x1401u, 32, (const GLvoid *)(*(DWORD *)(a1 + 32) + 16));
        glDrawElements(4u, 6 * *(DWORD *)(a1 + 36), 0x1403u, (GLvoid *)element_list_0);

        *(DWORD *)(a1 + 36) = 0;
        if ( v6 )
            goto LABEL_8;
    }
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
            str_rep_temp = (char *) calloc(12285 , sizeof(char));
            //end

            //start
            //utf8文本记录和文本显示
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

            //start
            //获取drtc其它变量
            jmp_rep((LPVOID) sprite_batch_draw, my_sprite_batch_draw);
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
