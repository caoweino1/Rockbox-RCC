/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 by Alan Korr
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#include "config.h"
#include "hwcompat.h"

#ifdef HAVE_LCD_CHARCELLS

#include "lcd.h"
#include "kernel.h"
#include "thread.h"
#include <string.h>
#include <stdlib.h>
#include "file.h"
#include "debug.h"
#include "system.h"
#include "font.h"

/*** definitions ***/

#define OLD_LCD_CONTRAST_SET ((char)0xA8)
#define OLD_LCD_CRAM         ((char)0xB0) /* Characters */
#define OLD_LCD_PRAM         ((char)0x80) /*  Patterns  */
#define OLD_LCD_IRAM         ((char)0xE0) /*    Icons   */

#define NEW_LCD_CONTRAST_SET ((char)0x50)
#define NEW_LCD_CRAM         ((char)0x80) /* Characters */
#define NEW_LCD_PRAM         ((char)0xC0) /*  Patterns  */
#define NEW_LCD_IRAM         ((char)0x40) /*    Icons   */

#define LCD_CURSOR(x,y)    ((char)(lcd_cram+((y)*16+(x))))
#define LCD_ICON(i)        ((char)(lcd_iram+i))

#define SCROLLABLE_LINES 2

#define SCROLL_MODE_OFF   0
#define SCROLL_MODE_PAUSE 1
#define SCROLL_MODE_RUN   2

/*** generic code ***/

struct scrollinfo {
    int mode;
    char text[MAX_PATH];
    char line[32];
    int textlen;
    int offset;
    int startx;
    int starty;
    int space;
};

static void scroll_thread(void);
static char scroll_stack[DEFAULT_STACK_SIZE];
static char scroll_name[] = "scroll";
static char scroll_speed = 8; /* updates per second */
static char scroll_delay = HZ/2; /* delay before starting scroll */
static char scroll_spacing = 3; /* spaces between end and start of text */
static long scroll_start_tick;


static struct scrollinfo scroll[SCROLLABLE_LINES];

static const unsigned char new_lcd_ascii[] = {
   0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
   0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
   0x10,0x11,0x05,0x13,0x14,0x15,0x16,0x17,
   0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
   0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
   0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
   0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
   0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
   0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
   0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
   0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,
   0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
   0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,
   0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
   0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,
   0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,
   0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
   0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
   0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
   0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
   0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,
   0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,
   0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,
   0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,
   0x41,0x41,0x41,0x41,0x41,0x41,0x20,0x43,
   0x45,0x45,0x45,0x45,0x49,0x49,0x49,0x49,
   0x44,0x4e,0x4f,0x4f,0x4f,0x4f,0x4f,0x20,
   0x20,0x55,0x55,0x55,0x55,0x59,0x20,0x20,
   0x61,0x61,0x61,0x61,0x61,0x61,0x20,0x63,
   0x65,0x65,0x65,0x65,0x69,0x69,0x69,0x69,
   0x6f,0x6e,0x6f,0x6f,0x6f,0x6f,0x6f,0x20,
   0x20,0x75,0x75,0x75,0x75,0x79,0x20,0x79
};

static const unsigned char old_lcd_ascii[] = {
   0x00,0x01,0x02,0x03,0x00,0x84,0x85,0x89,
   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
   0xec,0xe3,0xe2,0xe1,0xe0,0xdf,0x15,0x00,
   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
   0x24,0x25,0x26,0x37,0x06,0x29,0x2a,0x2b,
   0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,
   0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,
   0x3c,0x3d,0x3e,0x3f,0x40,0x41,0x42,0x43,
   0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,
   0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,
   0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,
   0x5c,0x5d,0x5e,0xa9,0x33,0xce,0x00,0x15,
   0x00,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,
   0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,
   0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,
   0x7c,0x7d,0x7e,0x24,0x24,0x24,0x24,0x24,
   0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
   0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
   0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
   0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
   0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,
   0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,
   0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,
   0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,
   0x45,0x45,0x45,0x45,0x45,0x45,0x24,0x47,
   0x49,0x49,0x49,0x49,0x4d,0x4d,0x4d,0x4d,
   0x48,0x52,0x53,0x53,0x53,0x53,0x53,0x24,
   0x24,0x59,0x59,0x59,0x59,0x5d,0x24,0x24,
   0x65,0x65,0x65,0x65,0x65,0x65,0x24,0x67,
   0x69,0x69,0x69,0x69,0x6d,0x6d,0x6d,0x6d,
   0x73,0x72,0x73,0x73,0x73,0x73,0x73,0x24,
   0x24,0x79,0x79,0x79,0x79,0x7d,0x24,0x7d
};

static bool new_lcd;

unsigned const char *lcd_ascii;
static char lcd_contrast_set;
static char lcd_cram;
static char lcd_pram;
static char lcd_iram;

#ifndef SIMULATOR
void lcd_clear_display(void)
{
    int i;
    lcd_write(true,LCD_CURSOR(0,0));
    for (i=0;i<32;i++)
        lcd_write(false,lcd_ascii[' ']);
}

void lcd_puts(int x, int y, unsigned char *string)
{
    int i;
    lcd_write(true,LCD_CURSOR(x,y));
    for (i=0; *string && x++<11; i++)
        lcd_write(false,lcd_ascii[*(unsigned char*)string++]);
    for (; x<11; x++)
        lcd_write(false,lcd_ascii[' ']);
}

void lcd_putc(int x, int y, unsigned char ch)
{
    lcd_write(true,LCD_CURSOR(x,y));
    lcd_write(false,lcd_ascii[ch]);
}

void lcd_define_pattern (int which,char *pattern,int length)
{
    int i;
    lcd_write(true,lcd_pram | which);
    for (i=0;i<length;i++)
        lcd_write(false,pattern[i]);
}

void lcd_double_height(bool on)
{
    if(new_lcd)
        lcd_write(true,on?9:8);
}

static char icon_pos[] =
{
    0, 0, 0, 0, /* Battery */
    2, /* USB */
    3, /* Play */
    4, /* Record */
    5, /* Pause */
    5, /* Audio */
    6, /* Repeat */
    7, /* 1 */
    9,  /* Volume */
    9,  /* Volume 1 */
    9,  /* Volume 2 */
    10, /* Volume 3 */
    10, /* Volume 4 */
    10, /* Volume 5 */
    10, /* Param */
};

static char icon_mask[] =
{
    0x02, 0x08, 0x04, 0x10, /* Battery */
    0x04, /* USB */
    0x10, /* Play */
    0x10, /* Record */
    0x02, /* Pause */
    0x10, /* Audio */
    0x02, /* Repeat */
    0x01, /* 1 */
    0x04, /* Volume */
    0x02, /* Volume 1 */
    0x01, /* Volume 2 */
    0x08, /* Volume 3 */
    0x04, /* Volume 4 */
    0x01, /* Volume 5 */
    0x10, /* Param */
};

void lcd_icon(int icon, bool enable)
{
    static unsigned char icon_mirror[11] = {0};
    int pos, mask;

    pos = icon_pos[icon];
    mask = icon_mask[icon];
    
    lcd_write(true, LCD_ICON(pos));
    
    if(enable)
        icon_mirror[pos] |= mask;
    else
        icon_mirror[pos] &= ~mask;
    
    lcd_write(false, icon_mirror[pos]);
}

void lcd_set_contrast(int val)
{
    lcd_write(true, lcd_contrast_set);
    lcd_write(false, 31-val);
}
#endif /* SIMULATOR */

void lcd_init (void)
{
    new_lcd = has_new_lcd();

    if(new_lcd) {
        lcd_ascii = new_lcd_ascii;
        lcd_contrast_set = NEW_LCD_CONTRAST_SET;
        lcd_cram = NEW_LCD_CRAM;
        lcd_pram = NEW_LCD_PRAM;
        lcd_iram = NEW_LCD_IRAM;
    } else {
        lcd_ascii = old_lcd_ascii;
        lcd_contrast_set = OLD_LCD_CONTRAST_SET;
        lcd_cram = OLD_LCD_CRAM;
        lcd_pram = OLD_LCD_PRAM;
        lcd_iram = OLD_LCD_IRAM;
    }
    
    create_thread(scroll_thread, scroll_stack,
                  sizeof(scroll_stack), scroll_name);
}

void lcd_puts_scroll(int x, int y, unsigned char* string )
{
    struct scrollinfo* s;
    int index;

    scroll_start_tick = current_tick + scroll_delay;

    /* search for the next free entry */
    for (index = 0; index < SCROLLABLE_LINES; index++) {
        s = &scroll[index];
        if (s->mode == SCROLL_MODE_OFF) {
            break;
        }
    }

    s->space = 11 - x;

    lcd_puts(x,y,string);
    s->textlen = strlen(string);

    if ( s->textlen > s->space ) {
        s->mode = SCROLL_MODE_RUN;
        s->offset=s->space;
        s->startx=x;
        s->starty=y;
        strncpy(s->text,string,sizeof s->text);
        s->text[sizeof s->text - 1] = 0;
        memset(s->line, 0, sizeof s->line);
        strncpy(s->line,string,
                s->space > (int)sizeof s->line ?
                (int)sizeof s->line : s->space );
        s->line[sizeof s->line - 1] = 0;
    }
}

void lcd_stop_scroll(void)
{
    struct scrollinfo* s;
    int index;

    for ( index = 0; index < SCROLLABLE_LINES; index++ ) {
        s = &scroll[index];
        if ( s->mode == SCROLL_MODE_RUN ||
             s->mode == SCROLL_MODE_PAUSE ) {
            /* restore scrolled row */
            lcd_puts(s->startx, s->starty, s->text);
            s->mode = SCROLL_MODE_OFF;
        }
    }

    lcd_update();
}

void lcd_stop_scroll_line(int line)
{
    struct scrollinfo* s;
    int index;

    for ( index = 0; index < SCROLLABLE_LINES; index++ ) {
        s = &scroll[index];
        if ( s->startx == line && 
             ( s->mode == SCROLL_MODE_RUN ||
               s->mode == SCROLL_MODE_PAUSE )) {
            /* restore scrolled row */
            lcd_puts(s->startx, s->starty, s->text);
            s->mode = SCROLL_MODE_OFF;
        }
    }

    lcd_update();
}

void lcd_scroll_pause(void)
{
    struct scrollinfo* s;
    int index;

    for ( index = 0; index < SCROLLABLE_LINES; index++ ) {
        s = &scroll[index];
        if ( s->mode == SCROLL_MODE_RUN ) {
            s->mode = SCROLL_MODE_PAUSE;
        }
    }
}

void lcd_scroll_pause_line(int line)
{
    struct scrollinfo* s;
    int index;

    for ( index = 0; index < SCROLLABLE_LINES; index++ ) {
        s = &scroll[index];
        if ( s->startx == line &&
             s->mode == SCROLL_MODE_RUN ) {
            s->mode = SCROLL_MODE_PAUSE;
        }
    }
}

void lcd_scroll_resume(void)
{
    struct scrollinfo* s;
    int index;

    scroll_start_tick = current_tick + scroll_delay;

    for ( index = 0; index < SCROLLABLE_LINES; index++ ) {
        s = &scroll[index];
        if ( s->mode == SCROLL_MODE_PAUSE ) {
            s->mode = SCROLL_MODE_RUN;
        }
    }
}

void lcd_scroll_resume_line(int line)
{
    struct scrollinfo* s;
    int index;

    scroll_start_tick = current_tick + scroll_delay;

    for ( index = 0; index < SCROLLABLE_LINES; index++ ) {
        s = &scroll[index];
        if ( s->startx == line &&
             s->mode == SCROLL_MODE_PAUSE ) {
            s->mode = SCROLL_MODE_RUN;
        }
    }
}

void lcd_scroll_speed(int speed)
{
    scroll_speed = speed;
}

void lcd_scroll_delay(int ms)
{
    scroll_delay = ms / (HZ / 10);
}
static void scroll_thread(void)
{
    struct scrollinfo* s;
    int index;
    int i;
    bool update;

    /* initialize scroll struct array */
    for (index = 0; index < SCROLLABLE_LINES; index++) {
        scroll[index].mode = SCROLL_MODE_OFF;
    }

    scroll_start_tick = current_tick;

    while ( 1 ) {

        update = false;

        /* wait 0.5s before starting scroll */
        if ( TIME_AFTER(current_tick, scroll_start_tick) ) {

            for ( index = 0; index < SCROLLABLE_LINES; index++ ) {
                s = &scroll[index];
                if ( s->mode == SCROLL_MODE_RUN ) {
                    update = true;

                    for ( i = 0; i < s->space - 1; i++ )
                        s->line[i] = s->line[i+1];

                    if ( s->offset < s->textlen ) {
                        s->line[(int)s->space - 1] = s->text[(int)s->offset];
                        s->offset++;
                    }
                    else {
                        s->line[s->space - 1] = ' ';
                        if ( s->offset < s->textlen + scroll_spacing - 1 )
                            s->offset++;
                        else
                            s->offset = 0;
                    }

                    lcd_puts(s->startx,s->starty,s->line);
                }
            }

            if (update) {
                lcd_update();
            }
        }

        sleep(HZ/scroll_speed);
    }
}

#endif /* HAVE_LCD_CHARCELLS */

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rockbox-mode.el")
 * end:
 */
