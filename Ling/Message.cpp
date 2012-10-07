#include "Precompiled.hpp"
#include "Message.hpp"
#include <iostream>

std::ostream& operator<< (std::ostream& os, const OpenFileEvent& evt){
    os << " OpenFileEvent (file_name = ";
    const wchar_t* old_file_name = evt.file_name.c_str();
    char new_file_name[MAX_PATH];
    WideCharToMultiByte(CP_ACP, NULL, old_file_name, -1, new_file_name, MAX_PATH, NULL, NULL);
    os <<  new_file_name ;
    return os;
}
std::ostream& operator<< (std::ostream& os, const Event& evt){
	if (evt.Id() == EVENT_ID_UPDATE_POS) {
		return os << " UpdatePosEvent ";
	}else if( evt.Id() == EVENT_ID_SET_START_POS) {
		return os << " SetStartPosEvent ";
	}else if ( evt.Id() == EVENT_ID_SET_END_POS) {
		return os << " SetEndPosEvent ";
	}
	return os;
}
