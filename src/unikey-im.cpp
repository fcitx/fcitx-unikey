/***************************************************************************
 *   Copyright (C) 2012~2012 by CSSlayer                                   *
 *   wengxt@gmail.com                                                      *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/

#include <iconv.h>

#include <string>

#include <fcitx/fcitx.h>
#include <fcitx/ime.h>
#include <fcitx/hook.h>
#include <fcitx/instance.h>
#include <fcitx-config/xdg.h>
#include <fcitx-utils/log.h>
#include <errno.h>

#include "config.h"
#include "unikey.h"
#include "keycons.h"
#include "vnconv.h"
#include "unikey-config.h"
#include "unikey-im.h"
#include "unikey-ui.h"

#define CONVERT_BUF_SIZE 1024

#ifdef LIBICONV_SECOND_ARGUMENT_IS_CONST
typedef const char* IconvStr;
#else
typedef char* IconvStr;
#endif

static void* FcitxUnikeyCreate(FcitxInstance* instance);
static void FcitxUnikeyDestroy(void* arg);
static INPUT_RETURN_VALUE FcitxUnikeyDoInput(void* arg, FcitxKeySym sym, unsigned int state);
static boolean FcitxUnikeyInit(void* arg);
static void FcitxUnikeyReset(void* arg);
static void FcitxUnikeyResetUI(void* arg);
static INPUT_RETURN_VALUE FcitxUnikeyDoInputPreedit(FcitxUnikey* unikey, FcitxKeySym sym, unsigned int state);
static void FcitxUnikeyEraseChars(FcitxUnikey *unikey, int num_chars);
static void  FcitxUnikeyUpdatePreedit(FcitxUnikey *unikey);

static boolean LoadUnikeyConfig(UnikeyConfig* config);
static void ConfigUnikey(FcitxUnikey* unikey);
static void ReloadConfigFcitxUnikey(void* arg);
static void SaveUnikeyConfig(UnikeyConfig* fa);

static int latinToUtf(unsigned char* dst, unsigned char* src, int inSize, int* pOutSize);

FCITX_EXPORT_API
FcitxIMClass ime = {
    FcitxUnikeyCreate,
    FcitxUnikeyDestroy
};
FCITX_EXPORT_API
int ABI_VERSION = FCITX_ABI_VERSION;
static const unsigned int    Unikey_OC[]         = {CONV_CHARSET_XUTF8,
                                                    CONV_CHARSET_TCVN3,
                                                    CONV_CHARSET_VNIWIN,
                                                    CONV_CHARSET_VIQR,
                                                    CONV_CHARSET_BKHCM2,
                                                    CONV_CHARSET_UNI_CSTRING,
                                                    CONV_CHARSET_UNIREF,
                                                    CONV_CHARSET_UNIREF_HEX};
static const unsigned int    NUM_OUTPUTCHARSET   = sizeof(Unikey_OC)/sizeof(Unikey_OC[0]);

static const unsigned char WordBreakSyms[] =
{
    ',', ';', ':', '.', '\"', '\'', '!', '?', ' ',
    '<', '>', '=', '+', '-', '*', '/', '\\',
    '_', '~', '`', '@', '#', '$', '%', '^', '&', '(', ')', '{', '}', '[', ']',
    '|'
};

static const unsigned char WordAutoCommit[] =
{
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'b', 'c', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n',
    'p', 'q', 'r', 's', 't', 'v', 'x', 'z',
    'B', 'C', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N',
    'P', 'Q', 'R', 'S', 'T', 'V', 'X', 'Z'
};



int FcitxUnikeyUcs4ToUtf8(FcitxUnikey* unikey, const unsigned int c, char buf[UTF8_MAX_LENGTH + 1])
{
    unsigned int str[2];
    str[0] = c;
    str[1] = 0;

    size_t ucslen = 1;
    size_t len = UTF8_MAX_LENGTH;
    len *= sizeof(char);
    ucslen *= sizeof(unsigned int);
    char* p = buf;
    IconvStr src = (IconvStr) str;
    iconv(unikey->conv, &src, &ucslen, &p, &len);
    return (UTF8_MAX_LENGTH - len) / sizeof(char);
}


void* FcitxUnikeyCreate(FcitxInstance* instance)
{
    FcitxUnikey* unikey = (FcitxUnikey*) fcitx_utils_malloc0(sizeof(FcitxUnikey));

    if (!LoadUnikeyConfig(&unikey->config))
    {
        free(unikey);
        return NULL;
    }
    unikey->owner = instance;
    unikey->preeditstr = new std::string;
    union {
        short s;
        unsigned char b[2];
    } endian;
    endian.s = 0x1234;
    if (endian.b[0] == 0x12)
        unikey->conv = iconv_open("utf-8", "ucs-4be");
    else
        unikey->conv = iconv_open("utf-8", "ucs-4le");

    FcitxIMIFace iface;
    memset(&iface, 0, sizeof(FcitxIMIFace));
    iface.Init = FcitxUnikeyInit;
    iface.ResetIM = FcitxUnikeyReset;
    iface.DoInput = FcitxUnikeyDoInput;
    iface.ReloadConfig = ReloadConfigFcitxUnikey;

    FcitxInstanceRegisterIMv2(
        instance,
        unikey,
        "unikey",
        _("Unikey"),
        "unikey",
        iface,
        1,
        "vi"
    );

    UnikeySetup();

    InitializeBar(unikey);
    InitializeMenu(unikey);

    ConfigUnikey(unikey);

    FcitxIMEventHook hk;
    hk.arg = unikey;
    hk.func = FcitxUnikeyResetUI;

    FcitxInstanceRegisterResetInputHook(instance, hk);

    return unikey;
}

void FcitxUnikeyDestroy(void* arg)
{
    UnikeyCleanup();
}


boolean FcitxUnikeyInit(void* arg)
{
    return true;
}

void FcitxUnikeyReset(void* arg)
{
    FcitxUnikey* unikey = (FcitxUnikey*) arg;

    UnikeyResetBuf();
    unikey->preeditstr->clear();
}

void FcitxUnikeyCommit(FcitxUnikey* unikey)
{
    if (unikey->preeditstr->length() > 0) {
        FcitxInstanceCommitString(unikey->owner, FcitxInstanceGetCurrentIC(unikey->owner), unikey->preeditstr->c_str());
    }
    FcitxUnikeyReset(unikey);
}

INPUT_RETURN_VALUE FcitxUnikeyDoInput(void* arg, FcitxKeySym sym, unsigned int state)
{
    FcitxUnikey* unikey = (FcitxUnikey*) arg;
    INPUT_RETURN_VALUE tmp;
    FcitxInputState* input = FcitxInstanceGetInputState(unikey->owner);
    /* use origin sym and state here */
    sym = (FcitxKeySym) FcitxInputStateGetKeySym(input);
    state = FcitxInputStateGetKeyState(input);

    tmp = FcitxUnikeyDoInputPreedit(unikey, sym, state);

    // check last keyevent with shift
    if (sym >= FcitxKey_space && sym <=FcitxKey_asciitilde)
    {
        unikey->last_key_with_shift = state & FcitxKeyState_Shift;
    }
    else
    {
        unikey->last_key_with_shift = false;
    } // end check last keyevent with shift

    return tmp;
}

INPUT_RETURN_VALUE FcitxUnikeyDoInputPreedit(FcitxUnikey* unikey, FcitxKeySym sym, unsigned int state)
{

    if (state & FcitxKeyState_Ctrl
             || state & FcitxKeyState_Alt // alternate mask
             || sym == FcitxKey_Control_L
             || sym == FcitxKey_Control_R
             || sym == FcitxKey_Tab
             || sym == FcitxKey_Return
             || sym == FcitxKey_Delete
             || sym == FcitxKey_KP_Enter
             || (sym >= FcitxKey_Home && sym <= FcitxKey_Insert)
             || (sym >= FcitxKey_KP_Home && sym <= FcitxKey_KP_Delete)
        )
    {
        FcitxUnikeyCommit(unikey);
        return IRV_FLAG_FORWARD_KEY;
    }

    else if ((sym >= FcitxKey_Caps_Lock && sym <= FcitxKey_Hyper_R)
            || (!(state & FcitxKeyState_Shift) && (sym == FcitxKey_Shift_L || sym == FcitxKey_Shift_R))  // when press one shift key
        )
    {
        return IRV_TO_PROCESS;
    }

    // capture BackSpace
    else if (sym == FcitxKey_BackSpace)
    {
        UnikeyBackspacePress();

        if (UnikeyBackspaces == 0 || unikey->preeditstr->empty())
        {
            FcitxUnikeyCommit(unikey);
            return IRV_FLAG_FORWARD_KEY;
        }
        else
        {
            if (unikey->preeditstr->length() <= (unsigned int)UnikeyBackspaces)
            {
                unikey->preeditstr->clear();
                unikey->auto_commit = true;
            }
            else
            {
                FcitxUnikeyEraseChars(unikey, UnikeyBackspaces);
                FcitxUnikeyUpdatePreedit(unikey);
            }

            // change tone position after press backspace
            if (UnikeyBufChars > 0)
            {
                if (unikey->config.oc == UKCONV_XUTF8)
                {
                    unikey->preeditstr->append((const char*)UnikeyBuf, UnikeyBufChars);
                }
                else
                {
                    static unsigned char buf[CONVERT_BUF_SIZE];
                    int bufSize = CONVERT_BUF_SIZE;

                    latinToUtf(buf, UnikeyBuf, UnikeyBufChars, &bufSize);
                    unikey->preeditstr->append((const char*)buf, CONVERT_BUF_SIZE - bufSize);
                }

                unikey->auto_commit = false;
                FcitxUnikeyUpdatePreedit(unikey);
            }
        }
        return IRV_DISPLAY_MESSAGE;
    } // end capture BackSpace

    else if (sym >=FcitxKey_KP_Multiply && sym <=FcitxKey_KP_9)
    {
        FcitxUnikeyCommit(unikey);
        return IRV_FLAG_FORWARD_KEY;
    }

    // capture ascii printable char
    else if ((sym >= FcitxKey_space && sym <=FcitxKey_asciitilde)
            || sym == FcitxKey_Shift_L || sym == FcitxKey_Shift_R) // sure this have FcitxKey_SHIFT_MASK
    {
        unsigned int i = 0;

        UnikeySetCapsState(state & FcitxKeyState_Shift, state & FcitxKeyState_CapsLock);

        // process sym

        // auto commit word that never need to change later in preedit string (like consonant - phu am)
        // if macro enabled, then not auto commit. Because macro may change any word
        if (unikey->ukopt.macroEnabled == 0 && (UnikeyAtWordBeginning() || unikey->auto_commit))
        {
            for (i =0; i < sizeof(WordAutoCommit); i++)
            {
                if (sym == WordAutoCommit[i])
                {
                    UnikeyPutChar(sym);
                    unikey->auto_commit = true;
                    return IRV_FLAG_FORWARD_KEY;
                }
            }
        } // end auto commit

        if ((unikey->config.im == UkTelex || unikey->config.im == UkSimpleTelex2)
            && unikey->config.process_w_at_begin == false
            && UnikeyAtWordBeginning()
            && (sym == FcitxKey_w || sym == FcitxKey_W))
        {
            UnikeyPutChar(sym);
            if (unikey->ukopt.macroEnabled == 0)
            {
                return IRV_TO_PROCESS;
            }
            else
            {
                unikey->preeditstr->append(sym==FcitxKey_w?"w":"W");
                FcitxUnikeyUpdatePreedit(unikey);
                return IRV_DISPLAY_MESSAGE;
            }
        }

        unikey->auto_commit = false;

        // shift + space, shift + shift event
        if ((unikey->last_key_with_shift == false && state & FcitxKeyState_Shift
                    && sym == FcitxKey_space && !UnikeyAtWordBeginning())
            || (sym == FcitxKey_Shift_L || sym == FcitxKey_Shift_R) // (&& state & FcitxKey_SHIFT_MASK), sure this have FcitxKey_SHIFT_MASK
           )
        {
            UnikeyRestoreKeyStrokes();
        } // end shift + space, shift + shift event

        else
        {
            UnikeyFilter(sym);
        }
        // end process sym

        // process result of ukengine
        if (UnikeyBackspaces > 0)
        {
            if (unikey->preeditstr->length() <= (unsigned int)UnikeyBackspaces)
            {
                unikey->preeditstr->clear();
            }
            else
            {
                FcitxUnikeyEraseChars(unikey, UnikeyBackspaces);
            }
        }

        if (UnikeyBufChars > 0)
        {
            if (unikey->config.oc == UKCONV_XUTF8)
            {
                unikey->preeditstr->append((const char*)UnikeyBuf, UnikeyBufChars);
            }
            else
            {
                unsigned char buf[CONVERT_BUF_SIZE + 1];
                int bufSize = CONVERT_BUF_SIZE;

                latinToUtf(buf, UnikeyBuf, UnikeyBufChars, &bufSize);
                unikey->preeditstr->append((const char*)buf, CONVERT_BUF_SIZE - bufSize);
            }
        }
        else if (sym != FcitxKey_Shift_L && sym != FcitxKey_Shift_R) // if ukengine not process
        {
            int n;
            char s[7] = {0, 0, 0, 0, 0, 0, 0};

            n = FcitxUnikeyUcs4ToUtf8(unikey, (unsigned int)sym, s); // convert ucs4 to utf8 char
            unikey->preeditstr->append(s, n);
        }
        // end process result of ukengine

        // commit string: if need
        if (unikey->preeditstr->length() > 0)
        {
            unsigned int i;
            for (i = 0; i < sizeof(WordBreakSyms); i++)
            {
                if (WordBreakSyms[i] == unikey->preeditstr->at(unikey->preeditstr->length()-1)
                    && WordBreakSyms[i] == sym)
                {
                    FcitxUnikeyCommit(unikey);
                    return IRV_DO_NOTHING;
                }
            }
        }
        // end commit string

        FcitxUnikeyUpdatePreedit(unikey);
        return IRV_DISPLAY_MESSAGE;
    } //end capture printable char

    // non process key

    FcitxUnikeyCommit(unikey);
    return IRV_FLAG_FORWARD_KEY;
}


// code from x-unikey, for convert charset that not is XUtf-8
int latinToUtf(unsigned char* dst, unsigned char* src, int inSize, int* pOutSize)
{
    int i;
    int outLeft;
    unsigned char ch;

    outLeft = *pOutSize;

    for (i=0; i<inSize; i++)
    {
        ch = *src++;
        if (ch < 0x80)
        {
            outLeft -= 1;
            if (outLeft >= 0)
                *dst++ = ch;
        }
        else
        {
            outLeft -= 2;
            if (outLeft >= 0)
            {
                *dst++ = (0xC0 | ch >> 6);
                *dst++ = (0x80 | (ch & 0x3F));
            }
        }
    }

    *pOutSize = outLeft;
    return (outLeft >= 0);
}


static void FcitxUnikeyEraseChars(FcitxUnikey *unikey, int num_chars)
{
    int i, k;
    unsigned char c;
    k = num_chars;

    for ( i = unikey->preeditstr->length()-1; i >= 0 && k > 0; i--)
    {
        c = unikey->preeditstr->at(i);

        // count down if byte is begin byte of utf-8 char
        if (c < (unsigned char)'\x80' || c >= (unsigned char)'\xC0')
        {
            k--;
        }
    }

    unikey->preeditstr->erase(i+1);
}


static void  FcitxUnikeyUpdatePreedit(FcitxUnikey *unikey)
{
    FcitxInputState* input = FcitxInstanceGetInputState(unikey->owner);
    FcitxMessages* preedit = FcitxInputStateGetPreedit(input);
    FcitxMessages* clientPreedit = FcitxInputStateGetClientPreedit(input);
    FcitxInputContext* ic = FcitxInstanceGetCurrentIC(unikey->owner);
    FcitxProfile* profile = FcitxInstanceGetProfile(unikey->owner);
    FcitxInstanceCleanInputWindowUp(unikey->owner);
    if (ic && ((ic->contextCaps & CAPACITY_PREEDIT) == 0 || !profile->bUsePreedit)) {
        FcitxMessagesAddMessageAtLast(preedit, MSG_INPUT, "%s", unikey->preeditstr->c_str());
        FcitxInputStateSetCursorPos(input, unikey->preeditstr->size());
    }
    FcitxMessagesAddMessageAtLast(clientPreedit, MSG_INPUT, "%s", unikey->preeditstr->c_str());
    FcitxInputStateSetClientCursorPos(input, unikey->preeditstr->size());
}

CONFIG_DESC_DEFINE(GetUnikeyConfigDesc, "fcitx-unikey.desc")

boolean LoadUnikeyConfig(UnikeyConfig* config)
{
    FcitxConfigFileDesc *configDesc = GetUnikeyConfigDesc();
    if (!configDesc)
        return false;

    FILE *fp = FcitxXDGGetFileUserWithPrefix("conf", "fcitx-unikey.config", "r", NULL);

    if (!fp)
    {
        if (errno == ENOENT)
            SaveUnikeyConfig(config);
    }
    FcitxConfigFile *cfile = FcitxConfigParseConfigFileFp(fp, configDesc);

    UnikeyConfigConfigBind(config, cfile, configDesc);
    FcitxConfigBindSync(&config->gconfig);

    if (fp)
        fclose(fp);
    return true;
}

void ConfigUnikey(FcitxUnikey* unikey)
{
    unikey->ukopt.macroEnabled          = unikey->config.macro;
    unikey->ukopt.spellCheckEnabled     = unikey->config.spellCheck;
    unikey->ukopt.autoNonVnRestore      = unikey->config.autoNonVnRestore;
    unikey->ukopt.modernStyle           = unikey->config.modernStyle;
    unikey->ukopt.freeMarking           = unikey->config.freeMarking;
    UnikeySetInputMethod(unikey->config.im);
    UnikeySetOutputCharset(Unikey_OC[unikey->config.oc]);
    UnikeySetOptions(&unikey->ukopt);

    UpdateUnikeyUI(unikey);
}

void ReloadConfigFcitxUnikey(void* arg)
{
    FcitxUnikey* unikey = (FcitxUnikey*) arg;
    LoadUnikeyConfig(&unikey->config);
    ConfigUnikey(unikey);
}

void SaveUnikeyConfig(UnikeyConfig* fa)
{
    FcitxConfigFileDesc *configDesc = GetUnikeyConfigDesc();
    FILE *fp = FcitxXDGGetFileUserWithPrefix("conf", "fcitx-unikey.config", "w", NULL);
    FcitxConfigSaveConfigFileFp(fp, &fa->gconfig, configDesc);
    if (fp)
        fclose(fp);
}

void FcitxUnikeyResetUI(void* arg)
{
    FcitxUnikey* unikey = (FcitxUnikey*) arg;
    FcitxInstance* instance = unikey->owner;
    FcitxIM* im = FcitxInstanceGetCurrentIM(instance);
    boolean visible;
    if (!im || strcmp(im->uniqueName, "unikey") != 0)
        visible = false;
    else
        visible = true;
    FcitxUISetStatusVisable(instance, "unikey-input-method", visible);
    FcitxUISetStatusVisable(instance, "unikey-output-charset", visible);
    FcitxUISetStatusVisable(instance, "unikey-spell-check", visible);
    FcitxUISetStatusVisable(instance, "unikey-macro", visible);
}

void UpdateUnikeyConfig(FcitxUnikey* unikey)
{
    ConfigUnikey(unikey);
    SaveUnikeyConfig(&unikey->config);
}

