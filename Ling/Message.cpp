#include "Precompiled.hpp"
#include "Message.hpp"

std::ostream& operator<< (std::ostream& os, const OpenFileEvent& evt){
    os << " OpenFileEvent (file_name = ";
    const wchar_t* old_file_name = evt.file_name.c_str();
    char new_file_name[MAX_PATH];
    WideCharToMultiByte(CP_ACP, NULL, old_file_name, -1, new_file_name, MAX_PATH, NULL, NULL);
    os <<  new_file_name ;
    return os;
}
std::ostream& operator<< (std::ostream& os, const UpdatePosEvent& evt){
    return os << " UpdatePosEvent ";
}

std::ostream& operator<< (std::ostream& os, const SetStartPosEvent& evt){
    return os << " SetStartPosEvent ";
}

std::ostream& operator<< (std::ostream& os, const SetEndPosEvent& evt){
    return os << " SetEndPosEvent ";
}