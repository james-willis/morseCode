/* a simple morse code interpretter */

// emuneration of the types of morse code characters
enum {
  blank,
  dot,
  dash,
};

// length (ms) of different morse code symbols
int dot_l = 150;  // dot
int dash_l = 3 * dot_l; // dash
int char_delay_l = 3 * dot_l; // time delay between characters
int word_delay_l = 7 * dot_l; // time delay between words

// buffer for the morse code symbols of the character currently being typed
const int buff_size = 8;
char curr_char[buff_size] = {blank, blank, blank, blank, blank, blank, blank, blank};
int char_index = 0;

// entry in a morse to symbol dictionary
struct entry {
  char morse[buff_size];
  char symbol;
};

struct entry a = {{dot, dash, blank, blank, blank, blank, blank, blank}, 'a'};
struct entry b = {{dash, dot, dot, dot, blank, blank, blank, blank}, 'b'};
struct entry c = {{dash, dot, dash, dot, blank, blank, blank, blank}, 'c'};
struct entry d = {{dash, dot, dot, blank, blank, blank, blank, blank}, 'd'};
struct entry e = {{dot, blank, blank, blank, blank, blank, blank, blank}, 'e'};
struct entry f = {{dot, dot, dash, dot, blank, blank, blank, blank}, 'f'};
struct entry g = {{dash, dash, dot, blank, blank, blank, blank, blank}, 'g'};
struct entry h = {{dot, dot, dot, dot, blank, blank, blank, blank}, 'h'};
struct entry i = {{dot, dot, blank, blank, blank, blank, blank, blank}, 'i'};
struct entry j = {{dot, dash, dash, dash, blank, blank, blank, blank}, 'j'};
struct entry k = {{dash, dot, dash, blank, blank, blank, blank, blank}, 'k'};
struct entry l = {{dot, dash, dot, dot, blank, blank, blank, blank}, 'l'};
struct entry m = {{dash, dash, blank, blank, blank, blank, blank, blank}, 'm'};
struct entry n = {{dash, dot, blank, blank, blank, blank, blank, blank}, 'n'};
struct entry o = {{dash, dash, dash, dash, blank, blank, blank, blank}, 'o'};
struct entry p = {{dot, dash, dash, dot, blank, blank, blank, blank}, 'p'};
struct entry q = {{dash, dash, dot, dash, blank, blank, blank, blank}, 'q'};
struct entry r = {{dot, dash, dot, blank, blank, blank, blank, blank}, 'r'};
struct entry s = {{dot, dot, dot, blank, blank, blank, blank, blank}, 's'};
struct entry t = {{dash, blank, blank, blank, blank, blank, blank, blank}, 't'};
struct entry u = {{dot, dot, dash, blank, blank, blank, blank, blank}, 'u'};
struct entry v = {{dot, dot, dot, dash, blank, blank, blank, blank}, 'v'};
struct entry w = {{dot, dash, dash, blank, blank, blank, blank, blank}, 'w'};
struct entry x = {{dash, dot, dot, dash, blank, blank, blank, blank}, 'x'};
struct entry y = {{dash, dot, dash, dash, blank, blank, blank, blank}, 'y'};
struct entry z = {{dash, dash, dot, dot, blank, blank, blank, blank}, 'z'};
const int dict_size = 26;
struct entry symbol_dictionary[dict_size] = {
  a, b, c, d, e, f
};
//booleans
bool button_unpressed = false; // was the button just unpressed?
bool space_printed = true; // was a space the last thing printed?
bool char_printed = true; // was the current character printed?

//times
unsigned int time_released; // when the button was last released
unsigned int time_started = 0; // when the current press began

//pin constants
const int button_pin = 2;

bool matching_char(char* char_buff, struct entry ent, int buff_size) {
  for (int i = 0; i < buff_size; i++) {
    if (char_buff[i] != ent.morse[i]) {
      return false;
    }
  }
  return true;
}

void clear_buffer(char* char_buff, int buff_size) {
  for (int i = 0; i < buff_size; i++){
    char_buff[i] = blank;
  }
}

char morse_to_symbol(char* char_buff, int buff_size, struct entry* dict, int dict_size) {
  for (int i = 0; i < dict_size; i++) {
    if (matching_char(char_buff, dict[i], buff_size)) {
      clear_buffer(char_buff, buff_size);
      return dict[i].symbol;
    }
  }
  clear_buffer(char_buff, buff_size);
  return 0;
}

void setup() {
  pinMode(button_pin, INPUT);
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Type some Morse Code:");
}

void loop() {
  if (digitalRead(button_pin) == LOW) { // button pressed
    if (time_started == 0) { // button just pressed
      time_started = millis();
    }
  }
  else if (time_started != 0) { // button just released
    button_unpressed = true;
    space_printed = false;
    char_printed = false;
          time_released = millis();
    unsigned int press_len = millis() - time_started;
    time_started = 0;
    if (press_len > dash_l) { // dash
      curr_char[char_index] = dash;
      char_index = (char_index + 1) % buff_size;
    }
    else if (press_len > dot_l) {
      curr_char [char_index]= dot;
      char_index= (char_index + 1) % buff_size;
    }
  }
  else { // button has been release at least one loop iteration
    int diff = millis() - time_released;
    if (diff > word_delay_l && !space_printed) { //space
     space_printed = true;
     Serial.print(" ");
     char_index = 0;
    }
    else if (diff > char_delay_l && !char_printed){
     char_printed = true;
     char new_letter = morse_to_symbol(curr_char, buff_size, symbol_dictionary, 6);
     if (new_letter != 0) {
      Serial.print(new_letter);
     }
     char_index= 0;
    }
  }
}

