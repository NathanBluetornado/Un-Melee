#ifndef MEX_H_TEXT
#define MEX_H_TEXT

#include "structs.h"
#include "datatypes.h"
#include "obj.h"
#include "color.h"

/*** Structs ***/

struct TextAllocInfo
{
    void *next; // 0x0
    void *prev; // 0x4,
    int size;   // 0x8 data remaining after this alloc?
    // data after this....
};

struct Text
{
    Vec3 trans;  // 0x0-0xC
    Vec2 aspect; // 0xC-0x14 CHANGE THIS TO BOUND
    int x14;
    int x18;
    int x1c;
    int x20;
    Vec2 scale; // 0x24-0x2C
    int x2C;
    GXColor color;
    Vec2 stretch;  // 0x34-0x3C
    int x3c;       // 0x3c
    int x40;       // 0x40
    int x44;       // 0x44
    u8 use_aspect; // 0x48
    u8 kerning;    // 0x49
    u8 align;      // 0x4a
    u8 x4b;
    u8 x4c;
    u8 hidden; // 0x4D
    u8 x4e;
    u8 sis_id; // 0x4F, id of the premade text file to use
    int x50;
    GOBJ *gobj;               // 0x54
    void *callback;           // 0x58, read at 803a878c
    u8 *text_start;           // 0x5C, start parsing text data at this ptr
    u8 *text_end;             // 0x60, stops parsing text data at this ptr
    TextAllocInfo *allocInfo; // 0x64
    void *x68;                // 0x68, some alloc used for dialogue?
    u16 x6c;                  // 0x6c
    u16 x6e;                  // 0x6e
    int x70;                  // 0x70
    int x74;                  // 0x74
    int x78;                  // 0x78
    int x7c;                  // 0x7c
    int x80;                  // 0x80
    int x84;                  // 0x84
    int x88;                  // 0x88
    int x8c;                  // 0x8c
    int x90;                  // 0x90
    int x94;                  // 0x94
    int char_display_num;     // 0x98, how many characters to display
    u8 is_fit;                // 0x9c
    int x100;                 // 0x100
};

/*** Functions ***/

int Text_CreateCanvas(int unk, int no_create_cam_gobj, int text_gobjkind, int text_gobjsubclass, int text_gobjflags, int text_gxlink, int text_gxpri, int cobj_gxpri); // the optional gobj and cobj_gxlink are used to create a cobj as well. set gobj
Text *Text_CreateText(int SisIndex, int canvasID);
Text *Text_CreateText2(int SisIndex, int canvasID, float pos_x, float pos_y, float pos_z, float limit_x, float limit_y);
void Text_Destroy(Text *text);
int Text_AddSubtext(Text *text, float xPos, float yPos, char *string, ...);
void Text_SetScale(Text *text, int subtext, float x, float y);
void Text_SetColor(Text *text, int subtext, GXColor *color);
void Text_SetPosition(Text *text, int subtext, float x, float y);
void Text_SetText(Text *text, int subtext, char *string, ...);
u8 *Text_Alloc(int size);
void Text_DestroyAlloc(u8 *alloc);
void Text_DestroyAllAlloc(Text *text);
int Text_ConvertToMenuText(char *out, char *in);
void Text_GX(GOBJ *gobj, int pass);
void Text_LoadSdFile(int index, char *filename, char *symbol);
void Text_SetSisText(Text *text, int text_index);
void Text_DestroySisCanvas(int canvas_id);

/*** Variables ***/
// Text data
int *stc_textheap_size = R13 + (-0x3d38);
TextAllocInfo **stc_textheap_start = R13 + (-0x3d34);
TextAllocInfo **stc_textheap_next = R13 + (-0x3d30);
TextAllocInfo **stc_textheap_first = R13 + (-0x3d2c);

// Text object
TextAllocInfo **stc_textobj_first = R13 + (-0x3d28);

// Text canvas
TextAllocInfo **stc_textcanvas_first = R13 + (-0x3d24);

// Sis Library
ArchiveInfo **stc_sis_archives = 0x804d1110; // array of sis file archive pointers
void **stc_sis_data = 0x804d1124;            // array of loaded sis data

#endif