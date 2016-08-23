#include "charselect.h"

void Courtroom::setCharSelect()
{
  ui->charselect->show();
  //T0D0 fix this
  //char_select_list = getCharSelectList();

  //HACK, debug code until we get the networking sorted out
  char_type phoenix;
  phoenix.name = "Phoenix";
  phoenix.description = "The worst lawyer";
  phoenix.taken = true;
  phoenix.passworded = false;

  char_type miles;
  miles.name = "Miles";
  miles.description = "The best lawyer";
  miles.taken = false;
  miles.passworded = true;

  char_type oldbag;
  oldbag.name = "Oldbag";
  oldbag.description = "que";
  oldbag.taken = false;
  oldbag.passworded = true;

  char_type judge;
  judge.name = "Judge";
  judge.description = "que";
  judge.taken = true;
  judge.passworded = true;

  char_type miles_d;
  miles_d.name = "Miles D";
  miles_d.description = "que";
  miles_d.taken = true;
  miles_d.passworded = true;

  character_list.insert(0, phoenix);
  character_list.insert(1, miles);
  character_list.insert(2, oldbag);
  character_list.insert(3, judge);
  character_list.insert(4, miles_d);

  //might seem redundant, but we are going to use this value A LOT
  int char_list_size = character_list.size();

  //T0D0 uncomment this, fix implementation
  //setTakenChars();

  //you raise me uup
  //srs, tho. brings the ui in front of the rest of the courtroom
  ui->charselect->raise();
  ui->charselect_left->raise();
  ui->charselect_right->raise();
  ui->spectator->raise();

  //so when we show() and hide() charselect, children follow suit
  ui->charselect_left->setParent(ui->charselect);
  ui->charselect_right->setParent(ui->charselect);
  ui->spectator->setParent(ui->charselect);

  //setting up the grid and positions
  const int base_x_pos{25};
  const int base_y_pos{36};

  const int x_modifier{67};
  int x_mod_count{0};

  const int y_modifier{67};
  int y_mod_count{0};

  //QSignalMapper* signalMapper = new QSignalMapper (this) ;



  //in this loop we simply make 90 widgets and connect them, they start off empty
  for(int n_icon{0} ; n_icon < 90 ; ++n_icon)
  {
    int x_pos = base_x_pos + (x_modifier * x_mod_count);
    int y_pos = base_y_pos + (y_modifier * y_mod_count);

    charicon_list.insert(n_icon, new charicon(x_pos, y_pos, ui->charselect));

    connect (charicon_list.at(n_icon), SIGNAL(clicked()), mapper, SLOT(map())) ;
    mapper -> setMapping (charicon_list.at(n_icon), n_icon) ;

    ++x_mod_count;

    //if char number is divisible by ten then the next charicon should start on a new line
    if (n_icon % 10 == 0 && n_icon != 0)
    {
      ++y_mod_count;
      x_mod_count = 0;
    }
  }

  connect (mapper, SIGNAL(mapped(int)), this, SLOT(charChoose(int))) ;

  QString char_select_path = g_theme_path + "charselect_background.png";
  QString left_arrow_path = g_theme_path + "arrow_left.png";
  QString right_arrow_path = g_theme_path + "arrow_right.png";

  if (fileExists(char_select_path))
    ui->charselect->setPixmap(QPixmap(char_select_path));

  if (fileExists(left_arrow_path))
    ui->charselect_left->setStyleSheet("border-image:url(" + left_arrow_path + ")");

  if (fileExists(right_arrow_path))
    ui->charselect_right->setStyleSheet("border-image:url(" + right_arrow_path + ")");


  if (char_list_size % 90 == 0)
    char_select_pages = char_list_size / 90;
  else
    char_select_pages = (char_list_size / 90) + 1;

  char_select_current_page = 1;

  setCharSelectPage();
}

void Courtroom::setCharSelectPage()
{
  int char_list_size = character_list.size();

  //start by hiding left and right arrows because were
  //not certain at this point if they should appear or not
  ui->charselect_left->hide();
  ui->charselect_right->hide();

  //we reset all charicons first(this includes hiding)
  for(charicon *f_charicon : charicon_list)
  {
    f_charicon->reset();
  }

  int chars_on_page = -1;

  //SANITY CHECK
  if(char_select_pages < 1)
    callError("amount of character pages appear to be zero or negative");

  //check if there is only one page of characters
  else if(char_select_pages == 1)
    chars_on_page = char_list_size;

  //check if we are on the first page
  else if(char_select_current_page == 1)
  {
    if(char_select_pages == 1)
      chars_on_page = char_list_size;
    else if(char_select_pages > 1)
      chars_on_page = 90;
    else
      callError("char_select_current_page == 1 returned true but somehow char_select_pages was set wrong");
  }

  //check if we are on the last page
  else if(char_select_current_page == char_select_pages)
  {
    //then we check if the amount of chars we have is a multiple of 90
    if (char_list_size % 90 == 0)
      chars_on_page = 90;
    else
      chars_on_page = (char_list_size % 90);
  }

  //if none of the above are true, we have to be somewhere in the middle, which means 90 chars
  else if(char_select_current_page < char_select_pages && char_select_current_page > 1)
    chars_on_page = 90;

  else if(chars_on_page == -1)
    callFatalError("emotes_on_page was not set properly (-1)");

  else
    callFatalError("Something broke with the charselect idk. blame the terrible developers."
                   "seriously, though. chars_on_page failed to set properly. who knows why."
                   "this error should never appear ever ever");


  //used to detemine if its char # 1, 91, 181 etc.
  int real_char_modifier = (90 * (char_select_current_page - 1));

  for(int local_char_number{0} ; local_char_number < chars_on_page ; ++local_char_number)
  {
    int real_char_number = local_char_number + real_char_modifier;

    charicon *f_charicon = charicon_list.at(local_char_number);
    char_type f_char = character_list.at(real_char_number);

    f_charicon->setIcon(f_char.name);

    if (f_char.taken)
      f_charicon->set_taken();

    if (f_char.passworded)
      f_charicon->set_passworded();

    f_charicon->show();
  }

  //anything higher than the first page must have the left arrow
  if (char_select_current_page > 1)
    ui->charselect_left->show();

  //as long as the current page is less than max amount of pages, right arrow is shown
  if (char_select_current_page < char_select_pages)
    ui->charselect_right->show();
}


void Courtroom::charChoose(int local_charnumber)
{
  //we first need to figure out which character we have based on page number and
  //icon number

  int real_char_number;
  QString real_char;

  real_char_number = local_charnumber + (90 * (char_select_current_page - 1));
  real_char = character_list.at(real_char_number).name;

  //T0D0
  //hello mister server, can we has real_char pls?
  //if (serversaysyes)
  playerChar = real_char;

  enter_courtroom();

  //else
  //ui->errorlabel->setText("Character is already taken.");
}
