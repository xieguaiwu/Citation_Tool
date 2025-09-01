#include"includes/files.h"

bool restart = true;//control main loop
void main_notice();
void main_choice();
void _biblio();

int main() {
	build_shits();
	reset();
	//debug();
	while (restart) {
		hyphen();
		main_notice();
		main_choice();
	}
	return 0;
}

//new function to avoid nesting
void confirmation_loop_in_new_startnum(unsigned char StartTypeForConfirmLoop) {
	while (1) {
		key = _getch();
		if (yes()) break;
		if (no()) {
			StartNum = pt[StartTypeForConfirmLoop];
			//if don't jump out of the current order, simply let the user edit from pt[]
			break;
		}
	}
	cout << "\n";
}

bool quit_edit = false;
void deal_with_StartNum(unsigned char DealWithWhich, string SNNotice) {
	while (1) {
		cout << SNNotice << " ";
		if (pt[DealWithWhich] > 1)cout << "(type 1~" << pt[DealWithWhich] << ")";
		cout << "(type -1 to quit):";
		cin >> StartNum;//Beware: StartNum also starts from one
		badint();
		if (!isbadint && StartNum == -1) {//inserting -1 = quiting the menu
			quit_edit = true;
			break;
		}
		else quit_edit = false;
		StartNum--;
		if (!isbadint && StartNum >= 0 && StartNum <= pt[DealWithWhich]) {
			cout << "\n";
			break;
		}
	}
}

void new_startnum(unsigned char NewStartType) {
	deal_with_StartNum(NewStartType, "Input the place you want to start");
	if (!quit_edit && content[NewStartType][StartNum] != "") {//if the new starting position is not empty
		cout << "This place is occupied. Edit anyway?";
		confirmation();
		confirmation_loop_in_new_startnum(NewStartType);//avoid nesting
	}
}

void write_after_editing(unsigned char WriteAfterWhich) {//post-editing process
	if (StartNum < pt[WriteAfterWhich]) {
		content[WriteAfterWhich][StartNum] = shit;//no need to add pt[]
		shit_re(WriteAfterWhich);
	}
	else {//the condition equals to StartNum = pt[]
		pushin(WriteAfterWhich, shit);
		shit_app(WriteAfterWhich);//change the value of pt[] after appending
		pt_write();
	}
}

//to edit info of all the four kinds
void edit_content(unsigned char EditWhichContent) {
	quit_edit = false;
	StartNum = pt[EditWhichContent];//initialization
	if (!check_empty(EditWhichContent)) {//check
		hyphen();
		standard_APA(EditWhichContent);
	}
	if (pt[EditWhichContent] > 0) {
		cout << "\nInsert" << insert_info[EditWhichContent] << "in current order?";
		confirmation();
	}
	while (pt[EditWhichContent] > 0) {
		key = _getch();
		if (yes()) {
			StartNum = pt[EditWhichContent];
			break;
		}
		if (no()) {
			new_startnum(EditWhichContent);
			break;
		}
	}
	hyphen();
	if (quit_edit)return;
	if (content[EditWhichContent][StartNum] != "")partial_APA();
	cout << "\n(Editing" << insert_info[EditWhichContent];
	if (EditWhichContent == authors) cout << " - FamilyName + SPACE + FirstLetterOfGivenName";
	cout << "):";
	shit.clear();
	getline(cin, shit);
	/*if (EditWhichContent == authors) {
		content[authors][StartNum] = APA_author_format(StartNum);
	}*/
	write_after_editing(EditWhichContent);
}

bool used_biblio = false;
void _biblio() {//F)Check bibliography
	used_biblio = true;
	if (check_empty(titles))cout << "NO CONTENT\n";
	else {
		standard_APA(titles);
	}
}

string page_num[2];//page_num[] notes down the starting and ending page
void cita_page_insert(bool InsertStart) {
	isbadint = true;
	while (isbadint) {
		if (InsertStart)cout << "Starting page:";
		else cout << "Ending page:";
		cin >> page_num[InsertStart];
		badint();
	}
}

void _cita() {//G)Check full list of citation
	quit_edit = false;
	if (check_empty(titles))return;
	StartNum = 0;
	if (!used_biblio)_biblio(); //if the user has never seen the full biliography, then print it
	hyphen();
	deal_with_StartNum(titles, "Input the order number for your reference");//a loop to deal with StartNum
	if (quit_edit) return;
	cita_page_insert(1);
	cita_page_insert(0);
	APA_cita(StartNum, page_num[0], page_num[1]);
}

void main_notice() {//main menu
	cout << "A)Edit authors of the bibliography\nB)Edit titles of the bibliography\n";
	cout << "C)Edit publication years of the bibliography\nD)Edit translators (if there are any) of the bibliography\n";
	cout << "E)Edit publishing houses of the bibliography\nF)Check bibliography\n";
	cout << "G)Generate index with page number\nZ)Quit\n";
}

void main_choice() {
	while (restart) {
		key = _getch();
		if (toupper(key) >= 'A' && toupper(key) <= 'E') {
			//clear_screen();
			edit_content(toupper(key) - 65);
			//do this to avoid writing if conditions for all kinds of editing options
			break;
		}
		else if (toupper(key) == 'F') {
			//clear_screen();
			_biblio();
			break;
		}
		else if (toupper(key) == 'G') {
			//clear_screen();
			_cita();
			break;
		}
		else if (toupper(key) == 'Z') {
			restart = false;
			break;
		}
	}
}