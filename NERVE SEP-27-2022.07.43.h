//idapro全局属性
void (__cdecl *turtle_trans)(double a1, double a2) =
(void (__cdecl *)(double, double)) 0x00409F10;

int (__cdecl *sprite_batch_plot)(int, int, int) =
(int (__cdecl *)(int, int, int)) 0x00406460;

char *(__cdecl *sprite_get)(__int16 a1) =
(char *(__cdecl *)(__int16)) 0x00406930;

double *turtle = (double *) 0x004D00C0;
double *dbl_4D00C8 = (double *) 0x004D00C8;
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

void (__cdecl *real_SDL_GL_SwapWindow)(SDL_Window *) = (void (__cdecl *)(SDL_Window *)) 0x004C7DF8;

int (__cdecl *main_btn_wrap)(int a1, int a2) = (int (__cdecl *)(int a1, int a2)) 0x0047B0C0;
int (__cdecl *main_btn_default)(int a1, int a2) = (int (__cdecl *)(int a1, int a2)) 0x00478E10;
int (__cdecl *main_btn_image_wrap)(int a1, int a2) = (int (__cdecl *)(int a1, int a2)) 0x0047AB10;
int (__cdecl *glyphs_batch_plot_ex)(DWORD *a1, unsigned __int8 *a2, int a3, int a4, int a5) = (int (__cdecl *)(DWORD *a1, unsigned __int8 *a2, int a3, int a4, int a5)) 0x00403630;
int (__cdecl *glyphs_set_callback)(int (__cdecl *a1)(DWORD, DWORD, DWORD), int a2) = (int (__cdecl *)(int (__cdecl *)(DWORD, DWORD, DWORD), int )) 0x00403610;


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

int (__cdecl *glyphs_batch_plot)(int a1, char *Str, int a3, int a4) = (int (__cdecl *)(int a1, char *Str, int a3, int a4))0x004038B0;
void (__cdecl *turtle_set_rgba)(float a1, float a2, float a3, float a4) = (void (__cdecl *)(float a1, float a2, float a3, float a4))0x00409D20;

float *shadow = (float*)0x004D048C;
int*textglow_ = (int*)0x0086CB24;

int (__cdecl *plot_text)(char *a1, char *a2) = (int (__cdecl *)(char *a1, char *a2))0x00474940;

int __CFADD__(int x, int y) {
    return x > (x + y);
}

char* buffer_20313 = (char*)0x0086BAA0;
char* byte_86BB9F = (char*)0x0086BB9F;

long double (__cdecl *glyph_w)(DWORD *a1, unsigned __int8 a2) = (long double (__cdecl *)(DWORD *a1, unsigned __int8 a2))0x00403970;


int (*customize_layout)() = (int (*)()) 0x0042BE70;
int (__cdecl *chara_stat_name_part_0)(int) = (int (__cdecl *)(int)) 0x00428650;


char* (__cdecl *chara_get_custom)(unsigned int) = (char* (__cdecl *)(unsigned int)) 0x00428BD0;
void (*cursor_restore)(void) = (void (*)(void)) 0x00432720;
int (__cdecl *customize_btn_char_panel)(int, int) = (int (__cdecl *)(int, int)) 0x0042BAB0;
int (__cdecl *button_set_h)(int, float) = (int (__cdecl *)(int, float)) 0x0041CAF0;
int (__cdecl *btn_char_edit_mode)(int, int) = (int (__cdecl *)(int, int)) 0x0042B640;
int (__cdecl *button)(float, float, int, int) = (int (__cdecl *)(float, float, int, int)) 0x0041CE70;
int (__cdecl *labelify)(int) = (int (__cdecl *)(int)) 0x0047A290;
int (__cdecl *main_btn_edit_focus)(long double, int, int) = (int (__cdecl *)(long double, int, int)) 0x0047B770;
int (__cdecl *button_set_w)(int, float) = (int (__cdecl *)(int, float)) 0x0041CA70;
int (__cdecl *btn_select_perk)(int, int) = (int (__cdecl *)(int, int)) 0x0042B9E0;
int (__cdecl *btn_select_trait)(int, int) = (int (__cdecl *)(int, int)) 0x0042B910;
//不使用, 改为使用自定义函数my_selection_constprop_5
int (__cdecl *selection_constprop_5)(int, int, int, int, int, int) = (int (__cdecl *)(int, int, int, int, int, int)) 0x0042BD20;
int (__cdecl *btn_roll_datafunc)(long double, int, int) = (int (__cdecl *)(long double, int, int)) 0x0042BBF0;
int (*gender_randfunc)() = (int (*)()) 0x0042B5F0;
int (__cdecl *char_body_count)(int) = (int (__cdecl *)(int)) 0x0042AA00;
int (*tops_randfunc)() = (int (*)()) 0x0042B5B0;
int (*bodysize_randfunc)() = (int (*)()) 0x0042B130;
int (*bottoms_randfunc)() = (int (*)()) 0x0042B570;
int (__cdecl *char_head_count)(int) = (int (__cdecl *)(int)) 0x0042A730;
int (*face_randfunc)() = (int (*)()) 0x0042B530;
int (*skinclr_randfunc)() = (int (*)()) 0x0042B500;
int (__cdecl *char_hair_count)(int) = (int (__cdecl *)(int)) 0x0042A810;
int (*hair_randfunc)() = (int (*)()) 0x0042B4C0;
int (*hairclr_randfunc)() = (int (*)()) 0x0042B0F0;
int (__cdecl *char_hat_count)(int) = (int (__cdecl *)(int)) 0x0042A8F0;
int (*hat_randfunc)() = (int (*)()) 0x0042B480;
int (__cdecl *char_glasses_count)(int) = (int (__cdecl *)(int)) 0x0042AB00;
int (*shades_randfunc)() = (int (*)()) 0x0042B440;
int (__cdecl *char_beard_count)(int) = (int (__cdecl *)(int)) 0x0042ABE0;
int (*beard_randfunc)() = (int (*)()) 0x0042B400;
int (__cdecl *btn_roll_char)(long double, int, int) = (int (__cdecl *)(long double, int, int)) 0x0042C780;
int (__cdecl *iconify)(int) = (int (__cdecl *)(int)) 0x0047A210;
int (__cdecl *main_btn_image_push_state)(int, int) = (int (__cdecl *)(int, int)) 0x0047B580;

DWORD *(__cdecl *error_log_context)(int, int, int, int, int) = (DWORD *(__cdecl *)(int, int, int, int, int))0x00402C80;
void (*console_write)(char *, ...) = (void (*)(char *, ...))0x00430110;
char *(*main_stextf)(char *, ...) = (char *(*)(char *, ...))0x00474CA0;
char *(__cdecl *main_textpad_append)(char *) = (char *(__cdecl *)(char *))0x00479240;
int (__cdecl *chara_dump_stats)(int) = (int (__cdecl *)(int))0x00429090;
int (*get_selected_charid)() = (int (*)())0x004B0BA0;
char *(__cdecl *chara_get_safe)(unsigned int) = (char *(__cdecl *)(unsigned int))0x00428BB0;
const char *(__cdecl *chara_stat_name)(unsigned int) = (const char *(__cdecl *)(unsigned int))0x00428980;
int (__cdecl *chara_stat_known)(int, unsigned int) = (int (__cdecl *)(int, unsigned int))0x00428100;
int (__cdecl *chara_effective_stat)(int, unsigned int) = (int (__cdecl *)(int, unsigned int))0x00427F90;
int (__cdecl *game_plot_stat_smiley)(int) = (int (__cdecl *)(int))0x00455CC0;
int (*main_tabscreen_refresh)() = (int (*)())0x00478C30;
int (__cdecl *team_btn_char_stats)(int, int) = (int (__cdecl *)(int, int))0x004B0BF0;


int* dword_71D248 = (int*)0x0071D248;
int *char_edit_mode = (int *)0x0071D0EC;
int* TITLE_BAR_SPRITE = (int*)0x004D5CDC;
int* skin_pal = (int*)0x00A0ACC0;
int* hair_pal = (int*)0x00A39900;
int* icons16_id = (int*)0x0086B884;
int charsave_state = 0x004D02A0;
int charload_state = 0x004D0290;


int game_characters = 0x004D0424;

int *dword_A0B848 = (int*)0x00A0B848;

int* display_order = (int*)0x004E0A40;
int* dword_A0A534 = (int*)0x00A0A534;
int* old_selected = (int*)0x008C892C;
//idapro全局属性

char *__cdecl my_wrap_text_ex(const char **a1, int a2, unsigned int a3, int a4);
BYTE selection_constprop_5_jmp[5];
BYTE wrap_text_ex_jmp[5];

int my_selection_constprop_5(int a1, int a2, int a3, int a4, int a5, int a6);
int my_chara_stat_name_part_0(int a1);
const char *__cdecl my_chara_stat_name(unsigned int a1);
int __cdecl my_sprite_batch_plot(int a1, int a2, int a3);