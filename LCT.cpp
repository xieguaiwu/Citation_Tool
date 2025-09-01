#include "includes/files.h"

void print_usage();
void handle_add_command(int argc, char* argv[]);
void handle_list_command(int argc, char* argv[]);
void handle_delete_command(int argc, char* argv[]);
void handle_clear_command();
void handle_export_command(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    build_shits();
    reset();

    //print usage if has no arguments
    if (argc < 2) {
        print_usage();
        return 0;
    }

    string command = argv[1];
    //change the command to lower case
    transform(command.begin(), command.end(), command.begin(), ::tolower);

    if (command == command_names[add] || command == command_full_names[add]) {
        handle_add_command(argc, argv);
    } else if (command == command_names[list] || command == command_full_names[list]) {
        handle_list_command(argc, argv);
    } else if (command == command_names[remove_index] || command == command_full_names[remove_index]) {
        handle_delete_command(argc, argv);
    } else if (command == command_names[clear] || command == command_full_names[clear]) {
        handle_clear_command();
    } else if (command == command_names[output_index] || command == command_full_names[output_index]) {
        handle_export_command(argc, argv);
    } else if (command == command_names[help] || command == command_full_names[help]) {
        print_usage();
    } else {
        cout << "Unknown command: " << command << endl;
        print_usage();
        return 1;
    }

    return 0;
}

void print_usage() {
    cout << "Usage: CitationTool <command> [options]\n";
    cout << "Commands:\n";
    cout << "  " << command_full_names[add] << "/" << command_names[add];
    cout << " <author> <title> <year> [translator] [publisher]\n";
    cout << "  " << command_full_names[list] << "/" << command_names[list];
    cout << " [index]\n";
    cout << "  " << command_full_names[remove_index] << "/" << command_names[remove_index];
    cout << " <index>\n";
    cout << "  " << command_full_names[clear] << "/" << command_names[clear];
    cout << "\n";
    cout << "  " << command_full_names[output_index] << "/" << command_names[output_index];
    cout << " <filename> [format=APA]\n";
    cout << "  " << command_full_names[help] << "/" << command_names[help];
    cout << "\n";
}

void handle_add_command(int argc, char* argv[]) {
    if (argc < 5) {
        cout << "Error: add command requires at least author, title and year\n";
        print_usage();
        return;
    }

    string author = argv[2];
    string title = argv[3];
    string year = argv[4];
    string translator = (argc > 5) ? argv[5] : "";
    string publisher = (argc > 6) ? argv[6] : "";

    //add to arrays
    pushin(authors, author);
    pushin(titles, title);
    pushin(years, year);
    pushin(trans, translator);
    pushin(publish, publisher);

    //save to local file
    shit_app(authors);
    shit_app(titles);
    shit_app(years);
    shit_app(trans);
    shit_app(publish);
    pt_write();

    cout << "Citation added successfully!\n";
}

void handle_list_command(int argc, char* argv[]) {
    if (pt[titles] == 0) {
        cout << "No citations found.\n";
        return;
    }

    if (argc > 2) {
        //show certain index
        int index = stoi(argv[2]);
        if (index < 1 || index > pt[titles]) {
            cout << "Error: Index out of range. Valid range: 1-" << pt[titles] << endl;
            return;
        }
        APA_print(index - 1);
    } else {
        //show all indexs
        standard_APA(titles);
    }
}

void handle_delete_command(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Error: delete command requires an index\n";
        print_usage();
        return;
    }

    int index = stoi(argv[2]);
    if (index < 1 || index > pt[titles]) {
        cout << "Error: Index out of range. Valid range: 1-" << pt[titles] << endl;
        return;
    }

    //remove_index from arrays
    for (int i = index - 1; i < pt[titles] - 1; i++) {
        content[authors][i] = content[authors][i + 1];
        content[titles][i] = content[titles][i + 1];
        content[years][i] = content[years][i + 1];
        content[trans][i] = content[trans][i + 1];
        content[publish][i] = content[publish][i + 1];
    }
    for (int i = 0; i < types; i++) {
        pt[i]--;
    }
    //rewrite local files
    for (int i = 0; i < types; i++) {
        shit_re(i);
    }
    pt_write();

    cout << "Citation deleted successfully!\n";
}

void handle_clear_command() {
    for (int i = 0; i < types; i++) {
        pt[i] = 0;
        empty_which(i);
    }
    pt_write();
    cout << "All citations cleared!\n";
}

void handle_export_command(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Error: export command requires a filename\n";
        print_usage();
        return;
    }

    string filename = argv[2];
    string format = (argc > 3) ? argv[3] : "apa";

    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cout << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }
    streambuf* coutbuf = cout.rdbuf();
    cout.rdbuf(outfile.rdbuf());
    if (format == "apa") {
        standard_APA(titles);
    } else {
        cout << "Unsupported format: " << format << ". Using APA format.\n";
        standard_APA(titles);
    }
    cout.rdbuf(coutbuf);
    outfile.close();
    cout << "Citations exported to " << filename << " successfully!\n";
}