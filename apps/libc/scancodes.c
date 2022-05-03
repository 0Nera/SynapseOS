unsigned  char keyboard_map[] = {
    0, 
    27, //`
    '1', '2',  '3', '4',  '5', '6',  '7', '8',	'9',  '0', 
    '-', '=',  
    '\b',	/*  Backspace  */
    '\t',			/*  Tab  */
    'q', 'w',  'e', 'r',	/*  19 */
    't', 'y',  'u', 'i',  'o', 'p',  '[', ']',  '\n',	/*  Enter  key  */
    0,			/*  29 - Control */
    'a', 's',  'd', 'f',  'g', 'h',  'j', 'k',  'l', ';',	/*  39 */
    '\'',  '`', 
    -2,		/*  Left shift 42 */
    '\\',  'z', 'x',  'c', 'v',  'b', 'n',			/*  49 */
    'm', ',',  '.', '/',  
    -2,				/*  Right  shift  54*/
    '*',
    0,	/*  Alt  */
    ' ',	/*  Space  bar  */
    -1,	/*  Caps lock  */
    0,	/*  59 - F1  key  ...  >  */
    0, 0,  0, 0,  0, 0,  0, 0,
    0,	/*  <  ...  F10  */
    0,	/*  69 - Num lock*/
    0,	/*  Scroll Lock  */
    0,	/*  Home key */
    0,	/*  Up Arrow */
    0,	/*  Page Up  */
    '-',
    0,	/*  Left Arrow */
    0,
    0,	/*  Right  Arrow  */
    '+',
    0,	/*  79 - End key*/
    0,	/*  Down Arrow */
    0,	/*  Page Down  */
    0,	/*  Insert Key */
    0,	/*  Delete Key */
    0, 0,  0,
    0,	/*  F11  Key  */
    0,	/*  F12  Key  */
    0,	/*  All  other  keys are undefined */
};

unsigned  char keyboard_map_shifted[] = {
    0, 
    27, //`
    '!', '@',  '#', '$',  '%', '^',  '&', '*',	'(',  ')', 
    '_', '+',  
    '\b',	/*  Backspace  */
    '\t',			/*  Tab  */
    'Q', 'W',  'E', 'R',	/*  19 */
    'T', 'Y',  'U', 'I',  'O', 'P',  '{', '}',  '\n',	/*  Enter  key  */
    0,			/*  29 - Control */
    'A', 'S',  'D', 'F',  'G', 'H',  'J', 'K',  'L', ':',	/*  39 */
    '"',  '`', 
    1,		/*  Left shift */
    '|',  'Z', 'X',  'C', 'V',  'B', 'N',			/*  49 */
    'M', '<',  '>', '?',  
    1,				/*  Right  shift  */
    '*',
    0,	/*  Alt  */
    ' ',	/*  Space  bar  */
    -1,	/*  Caps lock  */
    0,	/*  59 - F1  key  ...  >  */
    0, 0,  0, 0,  0, 0,  0, 0,
    0,	/*  <  ...  F10  */
    0,	/*  69 - Num lock*/
    0,	/*  Scroll Lock  */
    0,	/*  Home key */
    0,	/*  Up Arrow */
    0,	/*  Page Up  */
    '-',
    0,	/*  Left Arrow */
    0,
    0,	/*  Right  Arrow  */
    '+',
    0,	/*  79 - End key*/
    0,	/*  Down Arrow */
    0,	/*  Page Down  */
    0,	/*  Insert Key */
    0,	/*  Delete Key */
    0, 0,  0,
    0,	/*  F11  Key  */
    0,	/*  F12  Key  */
    0,	/*  All  other  keys are undefined */
};