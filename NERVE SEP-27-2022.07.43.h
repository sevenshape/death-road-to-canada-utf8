#include <stdbool.h>

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

int (*quad_batch)(int a1, int a2) = (int (*)(int a1, int a2))0x00406120;
double *dbl_4D00D8 = (double *)0x004D00D8;
double *dbl_4D00E0 = (double *)0x004D00E0;
double *dbl_4D0108 = (double *)0x004D0108;
double *dbl_4D0100 = (double *)0x004D0100;
double *dbl_4D00F8 = (double *)0x004D00F8;
double *dbl_4D00F0 = (double *)0x004D00F0;
double *dbl_4D00D0 = (double *)0x004D00D0;

char* destbuf_19740 = (char*)0x0086C100;
BYTE* byte_86C4FF = (BYTE*)0x0086C4FF;

//SDL_CreateWindow(const char *title, int x, int y, int w,int h, Uint32 flags);
BOOL (*is_screen_size_changed)() = (BOOL (*)())0x004722E0;
void (__cdecl *draw_clear)(float red, float green, float blue) = (void (__cdecl *)(float, float, float))0x00402930;
char *(__cdecl *atlas_get)(char a1) = (char *(__cdecl *)(char))0x00401480;
int (__cdecl *sprite_batches_reset)(int a1) = (int (__cdecl *)(int))0x00406670;
void (__cdecl *turtle_set_pos)(double a1, double a2) = (void (__cdecl *)(double, double))0x00409C70;
int (__cdecl *main_sprite_batch_plot_centered)(int a1, int a2, int a3) = (int (__cdecl *)(int, int, int))0x0047C220;
void (__cdecl *sprite_batch_draw)(int a1) = (void (__cdecl *)(int))0x00406690;
int (*game_fade_draw)() = (int (*)())0x0044C790;
int (*main_draw_filmgrain_fx)() = (int (*)())0x0047BE90;
void (__cdecl *main_sprite_batches_draw_ex)(int a1) = (void (__cdecl *)(int))0x0047C310;

int* gogo_ = (int*)0x0086CB1C;
int* scaled_h = (int*)0x0086B93C;
int* sprites_0 = (int*)0x0086BA3C;
int* skeletons_base_id = (int*)0x0086B8A8;
int* dword_A0B22C = (int*)0x00A0B22C;
int (*mad_fps)() = (int (*)())0x00404E40;

void (__cdecl *draw_bind_texture)(int *a1) = (void (__cdecl *)(int *)) 0x004028B0;

int* element_init = (int*)0x00566F80;
int element_list_0 = 0x00566FA0;
int unk_626FA0 = 0x00626FA0;

int* last_texture = (int*)0x004D0004;
int (__cdecl *text_w)(char *) = (int (__cdecl *)(char *)) 0x0047A470;

DWORD *dword_9DC2E4 = (DWORD *) 0x009DC2E4;
DWORD *dword_A0B200 = (DWORD *) 0x00A0B200;
DWORD *dword_A0A588 = (DWORD *) 0x00A0A588;
DWORD *dword_A0A584 = (DWORD *) 0x00A0A584;
DWORD *dword_A0A5A0 = (DWORD *) 0x00A0A5A0;
DWORD *dword_A0ACA0 = (DWORD *) 0x00A0ACA0;
DWORD *dword_A0A59C = (DWORD *) 0x00A0A59C;
DWORD *dword_A0A5C8 = (DWORD *) 0x00A0A5C8;
DWORD *dword_A0A598 = (DWORD *) 0x00A0A598;

int (__cdecl *main_common_page_setup)(int, int) = (int (__cdecl *)(int, int)) 0x00478270;
int (__cdecl *main_do_quit_event_btn)(int, int) = (int (__cdecl *)(int, int)) 0x0047B060;
float *(__cdecl *game_colour_swap)(float *) = (float *(__cdecl *)(float *)) 0x0044DFD0;

int (__cdecl *btn_music_slider_check)(int, int) = (int (__cdecl *)(int, int)) 0x004873D0;
int (__cdecl *btn_music_slider)(int, int) = (int (__cdecl *)(int, int)) 0x004872E0;
int (__cdecl *btn_settings_checkmark)(int, int) = (int (__cdecl *)(int, int)) 0x00487210;
int (*wrapper_fullscreen)() = (int (*)()) 0x0040C210;
int (__cdecl *btn_set_display_mode)(int, int) = (int (__cdecl *)(int, int)) 0x00487250;
int (__cdecl *btn_settings_vsync)(int, int) = (int (__cdecl *)(int, int)) 0x00487510;
int (__cdecl *btn_change_size)(int, int) = (int (__cdecl *)(int, int)) 0x00487220;
int (__cdecl *btn_slider_check)(int, int) = (int (__cdecl *)(int, int)) 0x00487340;
int (__cdecl *btn_sound_slider)(int, int) = (int (__cdecl *)(int, int)) 0x00487410;
int (*layout_constprop_1)() = (int (*)()) 0x00487560;
int (__cdecl *main_btn_checkmark)(int, int) = (int (__cdecl *)(int, int)) 0x0047B350;
int (__cdecl *btn_joy_select)(int, int) = (int (__cdecl *)(int, int)) 0x0048B370;
int (__cdecl *main_btn_slider)(int, int) = (int (__cdecl *)(int, int)) 0x0047BA60;


DWORD *remap_state1 = (DWORD *) 0x004D053C;
int *remap_state2 = (int *) 0x004D052C;
int *remap_state3 = (int *) 0x004D051C;
int *remap_state4 = (int *) 0x004D050C;

int *main_options_return_override_ = (int *) 0x00A0B0C4;
int *main_state_reenter = (int *) 0x004D045C;
int *touch_setup_state = (int *) 0x004D0560;

int *listentime = (int *) 0x00875378;
int *mad_ticks = (int *) 0x0052F320;
int *selected_player = (int *) 0x00875374;
int *listening_ = (int *) 0x0087537C;
int *options_state = (int *) 0x004D04BC;

DWORD *dword_A0A5A4 = (DWORD *) 0x00A0A5A4;
DWORD *dword_A0A77C = (DWORD *) 0x00A0A77C;
DWORD *dword_A0A778 = (DWORD *) 0x00A0A778;
DWORD *dword_A0A774 = (DWORD *) 0x00A0A774;
DWORD *dword_A0A780 = (DWORD *) 0x00A0A780;

int *glyphs = (int *) 0x0086BA40;

int (__cdecl *shop_sign_action)(int, int, __int64, int) = (int (__cdecl *)(int, int, __int64, int)) 0x004B8BE0;

int (__cdecl *btn_reset)(int, int) = (int (__cdecl *)(int, int)) 0x0048B2D0;
int (__cdecl *main_button_label)(float, float, int) = (int (__cdecl *)(float, float, int)) 0x00478190;
int (__cdecl *btn_keymap)(int, int) = (int (__cdecl *)(int, int)) 0x0048BA20;
int (*layout_6)() = (int (*)()) 0x0048B5E0;
int (__cdecl *tile_action_default)(unsigned __int8*, int, __int64, int) = (int (__cdecl *)(unsigned __int8*, int, __int64, int))0x004B7CF0;
int* (__cdecl *mapgen_location)(unsigned int) = (int* (__cdecl *)(unsigned int))0x00480500;

//idapro全局属性
int my_keymap(int a1, int a2, float a3, float a4);
char *__cdecl my_wrap_text_ex(const char **a1, int a2, unsigned int a3, int a4);
BYTE selection_constprop_5_jmp[5];
BYTE wrap_text_ex_jmp[5];
BYTE button_ex_jmp[5];
BYTE main_btn_wrap_jmp[5];
BYTE btn_music_slider_jmp[5];
BYTE btn_sound_slider_jmp[5];

int my_selection_constprop_5(int a1, int a2, int a3, int a4, int a5, int a6);
int my_chara_stat_name_part_0(int a1);
const char *__cdecl my_chara_stat_name(unsigned int a1);

int (*main_tabscreen_start)() = (int (*)()) 0x004788D0;
int (__cdecl *main_tabscreen_add_tab)(char *Source, int a2) = (int (__cdecl *)(char*, int)) 0x00478980;
int (__cdecl *main_tabscreen_set_startfunc)(int a1) = (int (__cdecl *)(int)) 0x004789E0;
int (*main_tabscreen_layout)() = (int (*)()) 0x00478D20;
int (__cdecl *main_no_esc)(int a1) = (int (__cdecl *)(int)) 0x004768D0;
int (*customize_reenter)() = (int (*)()) 0x0042B2F0;
int (*back_to_start)() = (int (*)()) 0x0042B360;

int *first = (int*)0x0071D0E4;

int (__cdecl *main_set_screen_title)(int) = (int (__cdecl *)(int)) 0x00477F90;
int (__cdecl *main_button_nav)(int, int, int) = (int (__cdecl *)(int, int, int)) 0x004767A0;
int (*main_bottom_box)() = (int (*)()) 0x00478200;

int *main_common_page_return_override_ = (int*)0x00A0A564;

int* tab_index = (int*)0x0086C9F8;
int* tabs = (int*)0x0086C920;

DWORD *dword_86C924 = (DWORD *) 0x0086C924;
BYTE *byte_86C94B = (BYTE *) 0x0086C94B;

int (__cdecl *plot_text_noshadow)(char*, int) = (int (__cdecl *)(char*, int)) 0x00474970;
void (__cdecl *map_do_tile_lighting_ex)(int, float, float, int, BYTE*, int, int) = (void (__cdecl *)(int, float, float, int, BYTE*, int, int)) 0x0047DEC0;
int (__cdecl *main_sprite_batch_plot)(int, int, int) = (int (__cdecl *)(int, int, int)) 0x0047A4E0;

int (__cdecl *real_vsnprintf)(char *const Buffer, const size_t BufferCount, const char *const Format, va_list ArgList)=
(int (__cdecl *)(char *const Buffer, const size_t BufferCount, const char *const Format, va_list ArgList))0x004C85D0;

//start
//暂存
int __cdecl my_sprite_batch_plot(int a1, int a2, int a3);
int __cdecl my_quad_batch(int a1, int a2);
//end

int __cdecl my_sprite_batch_plot_ucs4(int a1, int a2, int a3, int s);
int __cdecl my_quad_batch_ucs4(int a1, int a2, int s);


void __cdecl my_sprite_batch_draw(int a1);

void font_set(int atlas);

GLsizei atlas_width = 0;
GLsizei atlas_height = 0;
GLsizei ft_width;
GLsizei ft_height;

char msg[1024];

int __cdecl my_plot_text_ex(char *a1, char *a2, int a3, int a4);
GLuint load_texture(const char *filename);

bool utf8_plot = FALSE;

int __cdecl my_plot_text_noshadow(char *a1, int a2);
int gettext_vsnprintf(char *str, size_t size, char *format, va_list ap);


char* get_hex_str(char *str);
char* get_escape_str(char *str);
char* gettext_ex(char* str);


char btn_str_u[12285];
wchar_t btn_str_s[4095];
int btn_str_s_p = 0;
int btn_str_s_xy_p = 0;

char str_rep_temp[12285];

char str_result_temp[12285];

char escape_str_gl[12285];

void append_if_not_present(const char *filename, const char *str);

int *perks_selected = (int*)0x870524;

int *dword_872C40 = (int *) 0x00872C40;

int *dword_872C44 = (int *) 0x00872C44;

char *my_perks[9992+312*64];
char *perks = (char *) 0x00870540;
