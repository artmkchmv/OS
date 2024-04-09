#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <map>

using namespace std;

void copyFile() {

    cout << "Enter the name of the copied file >> ";
    string fileName;
    cin >> fileName;

    cout << "Enter a name for the file copy >> ";
    string newFileName;
    cin >> newFileName;

    if (fileName == newFileName) cout << "Invalid name of the file copy!" << endl;

    ifstream fin;
    size_t bufsize = 4;
    char* buf = new char[bufsize];
    fin.open(fileName, ios::binary);

    if (fin.is_open()) {

        ofstream fout;
        fout.open(newFileName, ios::binary);

        while (!fin.eof()) {
            fin.read(buf, bufsize);
            if (fin.gcount()) fout.write(buf, fin.gcount());
        }

        cout << "Copy complete!" << endl;

        delete[] buf;
        fin.close();
        fout.close();
    }
    else {
        cout << "Error! Invalid name of the copied file!" << endl;
    }
}

void moveFile() {

    cout << "Enter the old file path >> ";
    string oldFilePath;
    cin >> oldFilePath;

    cout << "Enter the new file path >> ";
    string newFilePath;
    cin >> newFilePath;

    ifstream in(oldFilePath, ios::binary);

    if (!in) cout << "Error! Invalid old path!" << endl;
    else {
        ofstream out(newFilePath, ios::binary);
        if(!out) cout << "Error! Invalid new path!" << endl;
        else {
            out << in.rdbuf();
            in.close();
            out.close();
            remove(oldFilePath.c_str());
            cout << "File move complete!" << endl;
        }
    }
}

string showPermissions(mode_t mode) {

    string tempPerm;

    tempPerm += "owner_";
    if (mode & S_IRWXU) tempPerm += "rwx_";
    else tempPerm += "-";

    if (mode & S_IRUSR) tempPerm += "r_";
    else tempPerm += "-";

    if (mode & S_IWUSR) tempPerm += "w_";
    else tempPerm += "-";

    if (mode & S_IXUSR) tempPerm += "x_";
    else tempPerm += "-";
    
    tempPerm += "group_";
    if (mode & S_IRWXG) tempPerm += "rwx_";
    else tempPerm += "-";

    if (mode & S_IRGRP) tempPerm += "r_";
    else tempPerm += "-";

    if (mode & S_IWGRP) tempPerm += "w_";
    else tempPerm += "-";

    if (mode & S_IXGRP) tempPerm += "x_";
    else tempPerm += "-";

    tempPerm += "other_";
    if (mode & S_IRWXO) tempPerm += "rwx_";
    else tempPerm += "-";

    if (mode & S_IROTH) tempPerm += "r_";
    else tempPerm += "-";

    if (mode & S_IWOTH) tempPerm += "w_";
    else tempPerm += "-";

    if (mode & S_IXOTH) tempPerm += "x_";
    else tempPerm += "-";

    return tempPerm;
}

void infoFile() {

    cout << "Enter the file path >> ";
    string buf;
    cin >> buf;
    const char* filePath = buf.c_str();

    struct stat fileInfo;if (!stat(filePath, &fileInfo)) { //0 - work, else -1
        cout << "Device ID of device containing file: " << fileInfo.st_dev << endl;
        cout << "File serial number: " << fileInfo.st_ino << endl; 
        cout << "Mode of file: " << showPermissions(fileInfo.st_mode) << endl;
        cout << "Number of hard links to the file: " << fileInfo.st_nlink << endl;
        cout << "User ID of file: " << fileInfo.st_uid << endl; 
        cout << "Group ID of file: " << fileInfo.st_gid << endl;
        cout << "Device ID: " << fileInfo.st_rdev << endl;
        cout << "File size in bytes: " << fileInfo.st_size << endl;
        cout << "A file system-specific preferred I/O block size for this object: " << fileInfo.st_blksize << endl;
        cout << "Number of blocks allocated for this object: " << fileInfo.st_blocks << endl; 
        cout << "Time of last access: " << fileInfo.st_atime << endl;
        cout << "Time of last data modification: " << fileInfo.st_mtime << endl; 
        cout << "Time of last status change: " << fileInfo.st_ctime << endl;
    }
    else {
        cout << "Error! Invalid file path!" << endl;
    }
}

void changePermissionFile() {

    cout << "Enter the file path >> ";
    string buf;
    cin >> buf;
    const char* filePath = buf.c_str();

    cout << "Enter new file mask (owner, group, other) >> ";
    string tempMask;
    cin >> tempMask;

    cout << "Enter new file permissions (r, w, e, rw, re, we, rwe) >> ";
    string tempPermissions;
    cin >> tempPermissions;

    mode_t newPermissions;

    if (tempMask == "owner") {
        if (tempPermissions == "r") newPermissions = S_IRUSR;
        if (tempPermissions == "w") newPermissions = S_IWUSR;
        if (tempPermissions == "e") newPermissions = S_IXUSR;
        if (tempPermissions == "rw") newPermissions = S_IRUSR | S_IWUSR;
        if (tempPermissions == "re") newPermissions = S_IRUSR | S_IXUSR;
        if (tempPermissions == "we") newPermissions = S_IWUSR | S_IXUSR;
        if (tempPermissions == "rwe") newPermissions = S_IRWXU;
    }

    if (tempMask == "group") {
        if (tempPermissions == "r") newPermissions = S_IRGRP;
        if (tempPermissions == "w") newPermissions = S_IWGRP;
        if (tempPermissions == "e") newPermissions = S_IXGRP;
        if (tempPermissions == "rw") newPermissions = S_IRGRP | S_IWGRP;
        if (tempPermissions == "re") newPermissions = S_IRGRP | S_IXGRP;
        if (tempPermissions == "we") newPermissions = S_IWGRP | S_IXGRP;
        if (tempPermissions == "rwe") newPermissions = S_IRWXG;
    }

    if (tempMask == "other") {
        if (tempPermissions == "r") newPermissions = S_IROTH;
        if (tempPermissions == "w") newPermissions = S_IWOTH;
        if (tempPermissions == "e") newPermissions = S_IXOTH;
        if (tempPermissions == "rw") newPermissions = S_IROTH | S_IWOTH;
        if (tempPermissions == "re") newPermissions = S_IROTH | S_IXOTH;
        if (tempPermissions == "we") newPermissions = S_IWOTH | S_IXOTH;
        if (tempPermissions == "rwe") newPermissions = S_IRWXO;
    }

    if (!chmod(filePath, newPermissions)) {
        cout << "Permissions change complete!" << endl;
    }
    else {
        cout << "Error! Invalid file path/permission/mask!" << endl;
    }
}

void helpFile() {
    cout << "Keys:" << endl;
    cout << "--copy - copy the file" << endl;
    cout << "--move - move the file" << endl;
    cout << "--info - information about the file" << endl;
    cout << "--chp - change file permissions" << endl;
    cout << "--help - call Help menu" << endl;
}

int main(int argc, char* argv[]) {

    map<string, int> array = {{"--copy", 1}, {"--move", 2}, {"--info", 3}, {"--chp", 4}, {"--help", 5}};

    string key = argv[1];
    
    switch (array.at(key)) {
        case 1: 
            copyFile(); 
            break;
        case 2: 
            moveFile(); 
            break;
        case 3:    
            infoFile(); 
            break;
        case 4: 
            changePermissionFile();
            break;
        case 5: 
            helpFile(); 
            break;
    }

    return 0;
}
