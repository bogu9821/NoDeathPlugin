META
{
  Parser = Menu;
  After = zUnionMenu.d;
  Namespace = NoDeathPlugin;
};

// Namespace = NoDeathPlugin
// Important !!!
// Supporting Unicode symbols.
// File encoding: UTF-8 (without BOM).

// ------ Constants ------
const int Start_PY  = 1400;
const int Title_PY  = 450;
const int Menu_DY   = 550;
// Text
const int Text_PX   = 400;  // Position at left side
const int Text_SX   = 8000; // Size X
const int Text_SY   = 750;  // Size Y
const int Text_DY   = 120;  // Delta alignment
// Slider
const int Slider_PX = 6400; // Position at left side
const int Slider_SX = 1500; // Size X
const int Slider_SY = 600;  // Size Y
const int Slider_DY = 0;    // Delta alignment
// Choice
const int Choice_PX = 6400; // Position at left side
const int Choice_SX = 1500; // Size X
const int Choice_SY = 350;  // Size Y
const int Choice_DY = 120;  // Delta alignment

const string MenuBackPic   = "UnionMenu_BackPic.tga";
const string ItemBackPic   = "";
const string ChoiceBackPic = "MENU_CHOICE_BACK.TGA";
const string SliderBackPic = "MENU_SLIDER_BACK.TGA";
const string SliderPosPic  = "MENU_SLIDER_POS.TGA";
const string FontSmall     = "font_old_10_white.tga";
const string FontBig       = "font_old_20_white.tga";

var int CurrentMenuItem_PY;

// ------ Prototypes ------
func void InitializeBackPicturesAndFonts()
{
  MenuBackPic   = MENU_BACK_PIC;
  ItemBackPic   = MENU_ITEM_BACK_PIC;
  ChoiceBackPic = MENU_CHOICE_BACK_PIC;
  SliderBackPic = MENU_SLIDER_BACK_PIC;
  SliderPosPic  = MENU_SLIDER_POS_PIC;
  FontSmall     = MENU_FONT_SMALL;
  FontBig       = MENU_FONT_DEFAULT;
};

prototype C_EMPTY_MENU_DEF(C_MENU)
{
  InitializeBackPicturesAndFonts();
  C_MENU_DEF();
  backpic    = MenuBackPic;
  items[0]   = "";
  items[100] = "Union_menuitem_back";
  flags      = flags | MENU_SHOW_INFO;
};

instance C_MENU_ITEM_TEXT_BASE(C_MENU_ITEM_DEF)
{
  backpic        = ItemBackPic;
  posx           = Text_PX;
  posy           = Start_PY;
  dimx           = Text_SX;
  dimy           = Text_SY;
  flags          = flags | IT_EFFECTS_NEXT;
  onselaction[0] = SEL_ACTION_UNDEF;
};

instance C_MENUITEM_CHOICE_BASE(C_MENU_ITEM_DEF)
{
  backpic  = ChoiceBackPic;
  type     = MENU_ITEM_CHOICEBOX;
  fontname = FontSmall;
  posx     = Choice_PX;
  posy     = Start_PY + Choice_DY;
  dimx     = Choice_SX;
  dimy     = Choice_SY;
  flags    = flags & ~IT_SELECTABLE;
  flags    = flags | IT_TXT_CENTER;
};

instance C_MENUITEM_SLIDER_BASE(C_MENU_ITEM_DEF)
{
  backpic       = SliderBackPic;
  type          = MENU_ITEM_SLIDER;
  fontname      = FontSmall;
  posx          = Slider_PX;
  posy          = Start_PY + Slider_DY;
  dimx          = Slider_SX;
  dimy          = Slider_SY;
  flags         = flags & ~IT_SELECTABLE;
  flags         = flags | IT_TXT_CENTER;
  userstring[0] = SliderPosPic;
};

instance MenuItem_Opt_Headline(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = Title_PY;
  dimx    = 8100;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = Str_GetLocalizedString(
    "NoDeathPlugin НИСТРОЙКИ",
    "NoDeathPlugin SETTINGS",
    "NoDeathPlugin EINSTELLUNGEN",
    "USTAWIENIA NoDeathPlugin"
  );
};

func int Act_OpenWebLink()
{
  Open_Link("https://github.com/bogu9821/NoDeathPlugin");
  return 0;
};

instance MenuItem_Opt_Open_Link(C_MENU_ITEM_DEF)
{
  C_MENU_ITEM_TEXT_BASE();
  posy += MENU_DY * 8;

  posx             = 64;
  onselaction[0]   = SEL_ACTION_UNDEF;
  oneventaction[1] = Act_OpenWebLink;
  flags            = flags | IT_TXT_CENTER;
  text[0]          = Str_GetLocalizedString(
    "Открыть јтраницј проекта",
    "Open project page",
    "Projektseite öffnen",
    "Otwórz stronę projektu"
  );

  text[1] = "github.com/bogu9821/NoDeathPlugin";
};

// ------ Menu ------
instance MenuItem_Union_Auto_NoDeathPlugin(C_MENU_ITEM_UNION_DEF)
{
  text[0]          = "NoDeathPlugin";
  text[1] = Str_GetLocalizedString(
    "Наѝтройте параметры NoDeathPlugin",
    "NoDeathPlugin-Einstellungen konfigurieren",
    "Bildschirmverblassungszeit in Sekunden",
    "Zmień ustawienia NoDeathPlugin"
  );
  onselaction[0]   = SEL_ACTION_STARTMENU;
  onselaction_s[0] = "NoDeathPlugin:Menu_Opt_NoDeathPlugin";
};

instance Menu_Opt_NoDeathPlugin(C_EMPTY_MENU_DEF)
{
  Menu_SearchItems("NoDeathPlugin:MENUITEM_OPT_NoDeathPlugin_*");
};

instance MenuItem_Opt_NoDeathPlugin_Headline(C_MENU_ITEM)
{
  MenuItem_Opt_Headline();
};

instance MenuItem_Opt_NoDeathPlugin_AfterDeath(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 1;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = Str_GetLocalizedString(
    "Поѝле ѝмерти",
    "After death",
    "Nach dem Tod",
    "Po śmierci"
  );
  text[1] = Str_GetLocalizedString(
    "Дейѝтвиѝ поѝле ѝмерти",
    "Action taken after death",
    "Handlung nach dem Tod",
    "Czynność wykonana po śmierci"
  );
};

instance MenuItem_Opt_NoDeathPlugin_AfterDeath_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "AfterDeath";
  onchgsetoptionsection = "NODEATH";
  text[0]               = Str_GetLocalizedString(
    "Ничего|Выйти из игры|Новаѝ игра|Только удалить",
    "Nothing|Quit game|New game|Delete only",
    "Nichts|Spiel verlassen|Neues Spiel|Löschen",
    "Nic|Wyjdź z gry|Nowa gra|Tylko skasuj"
  );
};

instance MenuItem_Opt_NoDeathPlugin_BlockInput(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 2;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = Str_GetLocalizedString(
    "Блокировать ввод",
    "Block input",
    "Eingabe blockieren",
    "Zablokuj klawiaturę"
  );
  text[1] = Str_GetLocalizedString(
    "Блокировать ввод поѝле ѝмерти (рекомендуетѝѝ Вкл.)",
    "Block input after death (recommended On)",
    "Eingabe blockieren nach dem Tod (empfohlen Ein)",
    "Zablokuj klawiaturę po śmierci (zalecane Wł.)"
  );
};

instance MenuItem_Opt_NoDeathPlugin_BlockInput_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "BlockInput";
  onchgsetoptionsection = "NODEATH";
  text[0]               = Str_GetLocalizedString(
    "Выкл.|Вкл.",
    "Off|On",
    "Aus|Ein",
    "Wył.|Wł."
  );
};

instance MenuItem_Opt_NoDeathPlugin_WaitTime(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 3;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = Str_GetLocalizedString(
    "Времѝ ожиданиѝ",
    "Wait time",
    "Wartezeit",
    "Czas oczekiwania"
  );
  text[1] = Str_GetLocalizedString(
    "Времѝ затуханиѝ ѝкрана в ѝекундах",
    "Screen fade time in seconds",
    "Bildschirmverblassungszeit in Sekunden",
    "Czas zanikania ekranu w sekundach"
  );
};

instance MenuItem_Opt_NoDeathPlugin_WaitTime_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "WaitTime";
  onchgsetoptionsection = "NODEATH";
  text[0]               = "0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15";
};

instance MenuItem_Opt_NoDeathPlugin_CompatibilityMode(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 4;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = Str_GetLocalizedString(
    "Режим ѝовмеѝтимоѝти",
    "Compatibility mode",
    "Kompatibilitätsmodus",
    "Tryb kompatybilności"
  );
  text[1] = Str_GetLocalizedString(
    "Режим ѝовмеѝтимоѝти ѝ двигателем. (рекомендуетѝѝ Вкл.)",
    "Engine compatibility mode. (recommended On)",
    "Engine-Kompatibilitätsmodus. (empfohlen Ein)",
    "Tryb kompatybilności z silnikiem. (zalecane Wł.)"
  );
};

instance MenuItem_Opt_NoDeathPlugin__CompatibilityMode_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "CompatibilityMode";
  onchgsetoptionsection = "NODEATH";
  text[0]               = Str_GetLocalizedString(
    "Выкл.|Вкл.",
    "Off|On",
    "Aus|Ein",
    "Wył.|Wł."
  );
};


instance MenuItem_Opt_NoDeathPlugin_Open_Link(C_MENU_ITEM)
{
  MenuItem_Opt_Open_Link();
};

