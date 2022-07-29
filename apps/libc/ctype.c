int isalnum(int ch) {
	if(ch>='0' && ch<='9') return 1;
	if(ch>='A' && ch<='Z') return 1;
	if(ch>='a' && ch<='z') return 1;
	return 0;
}

int isspace(int ch) {
	if(ch==0x20 /*Whitespace*/
	   || ch==0x09 /*Tab*/
	   || ch==0x0a /*Newline*/
	   || ch==0x0b /*Vertical tab*/
	   || ch==0x0c /*Feed*/
	   || ch==0x0d /*Carriage return*/) return 1;
	else return 0;
}

int isalpha(int ch) {
	if(ch>='A' && ch<='Z') return 1;
	if(ch>='a' && ch<='z') return 1;
	return 0;
}

int isdigit(int ch) {
	if(ch>='0' && ch<='9') return 1;
	return 0;
}

int tolower(int ch) {
	if(ch >= 'A' && ch <= 'Z') return ch + ('a' - 'A');
	return ch;
}

int toupper(int ch) {
	if(ch >= 'a' && ch <= 'z') return ch - ('a' - 'A');
	return ch;
}

