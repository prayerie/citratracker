#include <3ds.h>
#include <libmodplug/modplug.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "gfx/bitbutton.h"
#include "gfx/bitmaps/icon_disk_raw.h"
#include "gfx/bitmaps/icon_flp_raw.h"
#include "gfx/bitmaps/icon_sample_raw.h"
#include "gfx/bitmaps/icon_song_raw.h"
#include "gfx/bitmaps/icon_trumpet_raw.h"
#include "gfx/bitmaps/icon_wrench_raw.h"
#include "gfx/button.h"
#include "gfx/gui.h"
#include "gfx/label.h"
#include "gfx/listbox.h"
#include "gfx/memoryindicator.h"
#include "gfx/numberslider.h"
#include "gfx/patternview.h"
#include "gfx/piano.h"
#include "gfx/tabbox.h"
#include "gfx/widget.h"

typedef struct {
    ModPlugFile *plug;
    ModPlug_Settings settings;
} ModplugDecoder;

#define SAMPLERATE 22050
#define SAMPLESPERBUF (SAMPLERATE / 30)
#define BYTESPERSAMPLE 4

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

static const size_t decoderBufSize = 800 * 2 * 2;
static ModplugDecoder decoder;
static ndspWaveBuf wavebufs[2];
static int nextBuf = 0;

bool redraw_main_requested = false;

void fill_buffer(void *audioBuffer, size_t offset, size_t size, int frequency) {
    u32 *dest = (u32 *)audioBuffer;

    for (int i = 0; i < size; i++) {
        // This is a simple sine wave, with a frequency of `frequency` Hz, and
        // an amplitude 30% of maximum.
        s16 sample = 0.3 * 0x7FFF *
                     sin(frequency * (2 * M_PI) * (offset + i) / SAMPLERATE);

        // Stereo samples are interleaved: left and right channels.
        dest[i] = (sample << 16) | (sample & 0xffff);
    }

    DSP_FlushDataCache(audioBuffer, size);
}

void audioCallback(void *const data) {
    if (wavebufs[nextBuf].status == NDSP_WBUF_DONE) {
        size_t decoded = ModPlug_Read(
            decoder.plug, wavebufs[nextBuf].data_pcm16, decoderBufSize);
        if (decoded != 0) {
            wavebufs[nextBuf].nsamples = ((decoded / 2) / sizeof(int16_t));
            DSP_FlushDataCache(wavebufs[nextBuf].data_pcm16, decoded);

            ndspChnWaveBufAdd(0, &wavebufs[nextBuf]);

            nextBuf ^= 1;
        }
    }
}

int main(int argc, char **argv) {
    // #ifdef BUILD_DEBUG

    // #endif

    gfxInitDefault();
    ndspInit();
    romfsInit();
    gfxSetDoubleBuffering(GFX_TOP, false);
    gfxSetDoubleBuffering(GFX_BOTTOM, false);

    u8 *fb_main_l = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
    // u8 *fb_main_r = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
    u8 *fb_sub = gfxGetFramebuffer(GFX_BOTTOM, GFX_RIGHT, NULL, NULL);

    GUI *gui = new GUI(fb_main_l, fb_sub);
    bool was_just_touch = false;

    // ----------------------------------------
    // test widgets
    Theme *t = new Theme();
    Button *b = new Button(10, 10, 50, 12);
    b->setFramebuf(fb_sub);
    b->setCaption("hello!!!");
    Widget *bgg = new Widget(0, 1, 400, 240, true, false);
    Widget *bggg = new Widget(0, 1, 320, 240, true, false);

    Label *l_l = new Label(200, 120, 50, 14);
    Label *l_r = new Label(203, 120, 50, 14);
    NumberSlider *ns = new NumberSlider(105, 60, 32, 17, 12, 1, 256, true);
    ns->setFramebuf(fb_sub);
    ns->setTheme(t, t->col_dark_bg);

    PrintConsole rightWindow;
    bgg->setFramebuf(fb_main_l);
    bgg->setTheme(t, t->col_dark_bg);
    bggg->setFramebuf(fb_sub);
    bggg->setTheme(t, t->col_dark_bg);

    int last_x = 0;
    int last_y = 0;
    ListBox *l = new ListBox(141, 32, 114, 89, 0x80, true, true, false);

    l->setFramebuf(fb_sub);
    b->setTheme(t, t->col_dark_bg);

    bgg->drawFullBg();
    bggg->drawFullBg();
    l_l->setFramebuf(fb_main_l);
    l_l->setTheme(t, t->col_dark_bg);
    l_l->setCaption("hi");
    gui->setTheme(t, t->col_bg);
    u16 *map_base = new u16[768];
    Piano *kb = new Piano(0, 152, 224, 40, map_base);

    BitButton *buttonswitchsub =
        new BitButton(234, 1, 21, 21, icon_flp_raw, 18, 18);

    TabBox *tabbox = new TabBox(1, 1, 139, 151, TABBOX_ORIENTATION_TOP, 16,
                                true, fb_main_l, fb_sub);

    ListBox *lbpot = new ListBox(4, 21, 50, 78, 1, true);
    lbpot->set(0, " 0");
    // lbpot->registerChangeCallback(handlePotPosChangeFromUser);
    Button *buttonpotup = new Button(70, 47, 14, 12);
    buttonpotup->setCaption(">");
    // buttonpotup->registerPushCallback(handlePotInc);
    Button *buttonpotdown = new Button(55, 47, 14, 12);
    buttonpotdown->setCaption("<");
    // buttonpotdown->registerPushCallback(handlePotDec);
    Button *buttonins = new Button(55, 21, 29, 12);
    buttonins->setCaption("ins");
    // buttonins->registerPushCallback(handlePotIns);
    Button *buttondel = new Button(55, 60, 29, 12);
    buttondel->setCaption("del");
    // buttondel->registerPushCallback(handlePotDel);
    Button *buttoncloneptn = new Button(55, 34, 29, 12);
    buttoncloneptn->setCaption("cln");
    // buttoncloneptn->registerPushCallback(handlePtnClone);
    MemoryIndicator *memoryiindicator = new MemoryIndicator(87, 90, 50, 8);
    State *state = new State();
    PatternView *pv = new PatternView(0, 1, 400, 192, state);
    Song *song = new Song();
    song->channelAdd();
    song->channelAdd();
    song->channelAdd();
    song->channelAdd();
    song->channelAdd();
    song->channelAdd();
    song->channelAdd();
    pv->setSong(song);
    tabbox->setTheme(t, t->col_dark_bg);
    tabbox->addTab(icon_song_raw, 0);
    tabbox->addTab(icon_disk_raw, 1);
    tabbox->addTab(icon_sample_raw, 2);
    tabbox->addTab(icon_trumpet_raw, 3);
    tabbox->addTab(icon_wrench_raw, 4);

    tabbox->registerWidget(lbpot, 0, 0);
    tabbox->registerWidget(buttonpotup, 0, 0);
    tabbox->registerWidget(buttonpotdown, 0, 0);
    tabbox->registerWidget(buttonins, 0, 0);
    tabbox->registerWidget(buttondel, 0, 0);
    tabbox->registerWidget(buttoncloneptn, 0, 0);
    tabbox->setTheme(t, t->col_dark_bg);
    // ----------------------------------------
    gui->registerWidget(tabbox, 0, SUB_SCREEN);
    gui->registerWidget(b, 0, SUB_SCREEN);
    gui->registerWidget(ns, 0, SUB_SCREEN);
    gui->registerWidget(l, 0, SUB_SCREEN);
    gui->registerWidget(l_l, 0, MAIN_SCREEN);
    gui->registerWidget(kb, 0, SUB_SCREEN);
    gui->registerWidget(buttonswitchsub, 0, SUB_SCREEN);
    // gui->registerWidget(memoryiindicator, 0, SUB_SCREEN);

    pv->setTheme(t, t->col_dark_bg);
    gui->registerWidget(pv, 0, MAIN_SCREEN);
    gui->revealAll();

    // consoleInit(GFX_TOP, NULL);
    // consoleDebugInit(debugDevice_SVC);

    printf("poo\n");
    // for now using libmodplug for testing purposes only
    // if there are performance issues maybe switch to ported
    // version of libntxm7..?

    // TEST AUDIO STUFF ----------------
    decoder.settings.mFlags = MODPLUG_ENABLE_OVERSAMPLING;
    decoder.settings.mChannels = 2;
    decoder.settings.mBits = 16;
    decoder.settings.mFrequency = 44100;
    decoder.settings.mResamplingMode = MODPLUG_RESAMPLE_NEAREST;

    /* Fill with modplug defaults */
    decoder.settings.mStereoSeparation = 128;
    decoder.settings.mMaxMixChannels = 16;
    decoder.settings.mReverbDepth = 0;
    decoder.settings.mReverbDelay = 0;
    decoder.settings.mBassAmount = 0;
    decoder.settings.mBassRange = 0;
    decoder.settings.mSurroundDepth = 0;
    decoder.settings.mSurroundDelay = 0;
    decoder.settings.mLoopCount = -1;

    ModPlug_SetSettings(&decoder.settings);

    if (false) {
        struct stat fileStat;
        stat("romfs:/space_debris.mod", &fileStat);
        size_t bufferSize = fileStat.st_size;

        FILE *file = fopen("romfs:/space_debris.mod", "rb");

        void *buffer = (void *)malloc(bufferSize);
        fread(buffer, bufferSize, 1, file);

        decoder.plug = ModPlug_Load(buffer, bufferSize);

        free(buffer);
        fclose(file);
    }

    if (decoder.plug == 0) {
        printf("Couldn't load mod file!\n");
    } else {
        // ModPlug_SetMasterVolume(decoder.plug, 128);

        ndspChnReset(0);
        ndspChnSetFormat(0, NDSP_FORMAT_STEREO_PCM16);
        ndspChnSetRate(0, 44100);
        ndspChnSetInterp(0, NDSP_INTERP_POLYPHASE);

        // Set up audiobuffers using linearAlloc
        // This ensures audio data is in contiguous physical ram
        wavebufs[0].data_pcm16 = (int16_t *)linearAlloc(decoderBufSize);
        wavebufs[0].looping = false;
        wavebufs[0].status = NDSP_WBUF_DONE;

        wavebufs[1].data_pcm16 = (int16_t *)linearAlloc(decoderBufSize);
        wavebufs[1].looping = false;
        wavebufs[1].status = NDSP_WBUF_DONE;

        printf("Playing modplug file!\n");

        // Fill the two audio buffers
        audioCallback(NULL);
        audioCallback(NULL);

        // and chain the rest of the audio using the callback
        ndspSetCallback(audioCallback, NULL);
    }
    // ---------------------------------
    gui->drawSubScreen();
    gui->drawMainScreen();
    while (aptMainLoop()) {
        // fprintf(stderr, "wtf????");
        if (hidKeysDown() & KEY_START) break;

        hidScanInput();

        touchPosition touch;

        u16 ptnlen = song->getPatternLength(song->getPotEntry(state->potpos));
        if (hidKeysHeld() & KEY_TOUCH) {
            hidTouchRead(&touch);
            if (touch.px != last_x || touch.py != last_y)
                gui->penMove(touch.px, touch.py);

            last_x = touch.px;
            last_y = touch.py;
            if (!was_just_touch) gui->penDown(touch.px, touch.py);
            was_just_touch = true;
        } else {
            if (was_just_touch) {
                // redraw_requested = true;
                gui->penUp(last_x, last_y);
                last_x = -255;
                last_y = -255;
                was_just_touch = false;
            }
        }
        bool fastscroll = hidKeysHeld() & KEY_B;
        if (hidKeysDown() & KEY_DUP) {
            int newrow = state->getCursorRow();

            if (fastscroll == false) {
                newrow--;
            } else {
                newrow -= 4;
            }
            while (newrow < 0) newrow += ptnlen;

            state->setCursorRow(newrow);

            pv->updateSelection();
            redraw_main_requested = true;

        } else if (hidKeysDown() & KEY_DDOWN) {
            int newrow = state->getCursorRow();

            if (fastscroll == false) {
                newrow++;
            } else {
                newrow += 4;
            }

            newrow %= ptnlen;

            state->setCursorRow(newrow);

            pv->updateSelection();
            redraw_main_requested = true;
        }

        if ((hidKeysDown() & KEY_DLEFT) && (!false)) {
            if (state->channel > 0) {
                state->channel--;
                pv->updateSelection();
                redraw_main_requested = true;
            }
        } else if ((hidKeysDown() & KEY_DRIGHT) && (!false)) {
            if (state->channel < song->getChannels() - 1) {
                state->channel++;
                pv->updateSelection();
                redraw_main_requested = true;
            }
        }
        if (redraw_main_requested) {
            gui->drawMainScreen();
            redraw_main_requested = false;
        }
        gfxFlushBuffers();
        gfxSwapBuffers();

        gspWaitForVBlank();
    }

    if (decoder.plug != 0) {
        ModPlug_Unload(decoder.plug);
        linearFree(wavebufs[0].data_pcm16);
        linearFree(wavebufs[1].data_pcm16);
    }

    romfsExit();
    ndspExit();
    gfxExit();
    return 0;
}
