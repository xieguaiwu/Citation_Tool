#pragma once
#include"includes.h"
#include"basicfunc.h"

#ifdef _WIN32//for trans platform
string local_file_path = ".\\log\\";
#else
string local_file_path = "./log/";
#endif
//change this to change the directory to store local files
string content[types][books];//main content of each reference
unsigned int pt[types];//pushed for how many times, starts from zero

string shit, info;//used when reading files
unsigned int StartNum;//StartNum starts from 1, while pt starts from 0
string page_num[2];//page_num[] notes down the starting and ending page

string filenames[all_types];//filenames[] - filenames of local files
string insert_info[types];//insert_info[] - used in edit_content()
string command_names[command_types];//abbreviation of commands
string command_full_names[command_types];//full names of commands
//used when reading files
ifstream eat;
ofstream poop;

void pushin(unsigned char, string);//to add content[][] and pt[]

//print content[][] after validate the index
void print_content(unsigned char PrintWhichContent, unsigned int PrintContentNum) {
	if (PrintContentNum < books && PrintContentNum < pt[PrintWhichContent]) {
		cout << content[PrintWhichContent][PrintContentNum];
	}
}

//transform user's author input to APA format (e.g. Carl Schmitt -> Schmitt, C.)
string APA_author_format(string FormatAuthorWhich) {
	vector<string> NameParts;
	string SinglePart;

	if (FormatAuthorWhich.empty())return "";

	istringstream iss(FormatAuthorWhich);

	//devide name into different parts
	while (iss >> SinglePart) {
		NameParts.push_back(SinglePart);
	}

	//return the name if it is too short
	if (NameParts.size() < 2) {
		return FormatAuthorWhich;
	}

	//get surname
	string surname = NameParts.back();

	//process: first letter capital, other letters lower case
	if (!surname.empty()) {
		surname[0] = toupper(surname[0]);
		for (size_t i = 1; i < surname.size(); i++) {
			surname[i] = tolower(surname[i]);
		}
		//make other part of the name lower case
	}

	//deal with name and abbreviation
	string givenNameInitials;
	for (size_t i = 0; i < NameParts.size() - 1; i++) {
		if (!NameParts[i].empty()) {
			char initial = toupper(NameParts[i][0]);
			givenNameInitials += initial;
		}
	}

	//delete space at the end
	if (!givenNameInitials.empty() && givenNameInitials[givenNameInitials.size() - 1] == ' ') {
		givenNameInitials.pop_back();
	}

	//return formatted name
	return surname + ", " + givenNameInitials;
}

string cita_author(unsigned int ChangeNum) {//change author format from "FamilyName GivenName" to "FamilyName"
	vector<string> CitaAuthorsNameParts;
	string CitaAuthorSinglePart;
	istringstream iss(content[authors][ChangeNum]);
	while (iss >> CitaAuthorSinglePart) {//separate each SinglePart of the full name
		CitaAuthorsNameParts.push_back(CitaAuthorSinglePart);
	}
	if (!CitaAuthorsNameParts.empty()) {//
		return CitaAuthorsNameParts[0];
	}
	else return "";
}

void APA_print(unsigned int order_num) {//order_num must start from zero
	//Beware of the limit of index
	cout << order_num + 1 << ". ";
	print_content(authors, order_num);
	cout << ". (";
	print_content(years, order_num);
	cout << "). ";
	print_content(titles, order_num);
	cout << ". ";
	if (order_num <= pt[trans] && content[trans][order_num] != "") {
		cout << "(" ;
		cout << content[trans][order_num];
		cout << ", Trans.). " ;
	}
	print_content(publish, order_num);
	cout << ".\n";
}

//print out all the ciatations in APA form
void standard_APA(unsigned char APAType) {
	for (int i = 0; i < pt[APAType]; ++i) {
		APA_print(i);
	}
}

//print out certain ciatations in APA form
void partial_APA() {
	APA_print(StartNum);
}

//print a citation with page number
void APA_cita(unsigned int WhichCita, string APACitaStart, string APACitaEnd) {//1st arg:the order number of the reference
	cout << "(";
	cout << cita_author(WhichCita);
	//print_content(authors, WhichCita);
	cout << ", ";
	print_content(years, WhichCita);
	cout << ", p";
	if (APACitaStart != APACitaEnd)cout << "p";
	cout << ". " << APACitaStart;//p. x or pp. x-y
	if (APACitaStart != APACitaEnd)cout << "-" << APACitaEnd;
	cout << ")\n";
}

//after using this function, remember to write "eat.close()" in the end
void open_shit(unsigned char OpenWhich) {
	eat.open(filenames[OpenWhich], ios::in | ios::app);//open to append
}

void write_shit_app(unsigned char WriteAppWhich) {
	poop.open(filenames[WriteAppWhich], ios::out | ios::app);//open to append
}

//when trying to rewrite the whole file
void rewrite_shit(unsigned char RewriteWhich) {
	poop.open(filenames[RewriteWhich], ios::out | ios::trunc);//open in trunc
}

//check if the file is empty.
//use .open() and .close() less frequently
bool check_empty(unsigned char IsEmpty, bool CloseAfter = true) {
	ifstream temp_file(filenames[IsEmpty]);
	if (!temp_file.is_open()) return true;
	//if file do not exist

	//check if the file is empty
	temp_file.seekg(0, ios::end);
	bool isEmpty = temp_file.tellg() == 0;
	temp_file.close();

	return isEmpty;
}

bool is_directory(string CheckDirPath) {//see if a directory is empty
	return (filesystem::exists(CheckDirPath) && filesystem::is_directory(CheckDirPath));
}

int find_min_pt() {//find out the minimum value in all pt[]
	return min({pt[0], pt[1], pt[2], pt[3], pt[4]});
}

//delete certain line of citation
void delete_content(unsigned int DeleteWhichLine) {//DeleteWhichLine starts from 1
	if (DeleteWhichLine >= books) return;
	for (int i = DeleteWhichLine - 1; i < find_min_pt(); i++) {
		for (int j = 0; j < types; j++) {
			content[j][i] = content[j][i + 1];//up by one
		}
	}
	for (int j = 0; j < types; j++) {
		pt[j]--;
	}
}

/*after eat_shit() load the contents of the file in variable info,
this function will give the value of info to the variables within the program*/
void read_and_write(unsigned char Read_WriteWhich) {
	if (pt[Read_WriteWhich] != 0) {//pt[] is already loaded ahead
		content[Read_WriteWhich][pt[Read_WriteWhich]] = info;
	}
}

//use a vector to get all the text within a file, then return the given line
vector<string> ReadLine;
string inbuf;//additional string to load text when reading a file
string eat_certain_shit(unsigned char EatWhichCertain, unsigned int CertainLine) {//**CertainLine must start from zero!**
	ifstream temp_file(filenames[EatWhichCertain]);
	if (!temp_file.is_open()) {
		return "";
	}

	vector<string> lines;
	string line;
	while (getline(temp_file, line)) {
		lines.push_back(line);
	}
	temp_file.close();

	if (CertainLine < lines.size()) {
		return lines[CertainLine];
	}
	else {
		return "";
	}
}

unsigned int ManyLines;//for eat_shit() and find_lines();
void eat_shit(unsigned char EatWhich) {//open file and load them
	eat.open(filenames[EatWhich]);
	if (!eat.is_open()) {
		cout << "Warning: Could not open file\n";
		return;
	}

	ManyLines = 0;
	while (getline(eat, info) && ManyLines < books) {
		content[EatWhich][ManyLines] = info;
		ManyLines++;
	}
	eat.close();

	pt[EatWhich] = ManyLines;
}

void reset_load() {
	eat.open(filenames[pushed]);
	if (eat.is_open()) {
		for (int i = 0; i < types && getline(eat, info); i++) {
			if (!info.empty()) {
				try {
					pt[i] = stoi(info);
				} catch (const std::invalid_argument&) {
					cout << "Warning: Invalid data in pushed.txt for type\n";
					pt[i] = 0;
				}
			} else {
				pt[i] = 0;
			}
		}
		eat.close();
	} else {
		//if there are no local files, then set pt[] to zero
		for (int i = 0; i < types; i++) {
			pt[i] = 0;
		}
	}

	//then read other files
	for (int i = 0; i < types; i++) {//read all the local files when program is started
		eat_shit(i);
	}
}

//initialization: filenames[], inser_info[], and load information from local files.
void reset() {
	filenames[authors] = local_file_path + "authors.txt";
	filenames[titles] = local_file_path + "titles.txt";
	filenames[years] = local_file_path + "years.txt";
	filenames[trans] = local_file_path + "trans.txt";
	filenames[publish] = local_file_path + "publish.txt";
	filenames[pushed] = local_file_path + "pushed.txt";//this will note down pushed times

	insert_info[authors] = " author names ";
	insert_info[titles] = " titles of the citing literature ";
	insert_info[years] = " years of publishment for the citing literature ";
	insert_info[trans] = " translators of the citing literature (if exist) ";
	insert_info[publish] = " publishing agency of the citing literature";
	//these are for the notice when using edit_content()

	command_names[add] = "a";
	command_full_names[add] = "add";
	command_names[list] = "l";
	command_full_names[list] = "list";
	command_names[remove_index] = "d";
	command_full_names[remove_index] = "delete";
	command_names[clear] = "cl";
	command_full_names[clear] = "clear";
	command_names[output_index] = "e";
	command_full_names[output_index] = "export";
	command_names[citation] = "c";
	command_full_names[citation] = "citation";
	command_names[help] = "h";
	command_full_names[help] = "help";

	reset_load();
}

//write in local files
//write at the end of the file
void shit_app(unsigned char ShitAppWhat) {//make sure ShitWhat != pushed (ShitAppWhat ∈ [author, publish])
	write_shit_app(ShitAppWhat);
	poop << content[ShitAppWhat][pt[ShitAppWhat] - 1] << "\n";
	poop.close();
}

//rewrite the whole file
void shit_re(unsigned char ShitReWhat) {//ShitReWhat ∈ [author, publish]
	rewrite_shit(ShitReWhat);
	for (int i = 0; i < pt[ShitReWhat]; i++) {
		poop << content[ShitReWhat][i] << "\n";
	}
	poop.close();
}

void pt_write() {
	rewrite_shit(pushed);
	for (int i = 0; i < types; i++) {
		poop << pt[i] << "\n";
	}
	poop.close();
}

void empty_which(unsigned char EmptyWhich) {
	rewrite_shit(EmptyWhich);
	poop.close();
}

//to change content[][] and pt[]
void pushin(unsigned char PushWhich, string PushInWhich) {
	if (pt[PushWhich] <= books - 1) {
		content[PushWhich][pt[PushWhich]] = PushInWhich;
		++pt[PushWhich];
	}
}

void build_shits() {//create all the text files, if they do not exist yet
	if (!fs::exists(local_file_path)) {//check if the dir is empty
		fs::create_directories(local_file_path);
	}
	for (int i = 0; i < types_count; ++i) {
		ifstream file(filenames[i]);
		if (!file.good()) {
			ofstream outfile(filenames[i]);
			outfile.close();
		}
		file.close();
	}
}